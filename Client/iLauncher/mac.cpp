#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>


#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <string>
#include <nb30.h>
#include <wsipx.h>
#include <wsnwlink.h>

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

#define MAX_MAC_ADDRESS_IN_BYTE         6
#define MAX_MAC_ADDRESS_IN_STRING      12

BOOL __GetMacAddressFromSock(LPBYTE lpMacAddress)
{
	int          iAdapters ;
	int          iOpt  = sizeof(iAdapters) ;
	int          iSize = sizeof(SOCKADDR_IPX) ;
	BOOL         bRet  = FALSE ;
	SOCKET       sock ;
	SOCKADDR_IPX adrIPX ;
	WSADATA      wsaData ;

#define WINSOCK_11 0x0101

	if	(WSAStartup(WINSOCK_11,&wsaData)!=0)
		return	bRet;

	if	((sock=socket(AF_IPX,SOCK_DGRAM,NSPROTO_IPX))!=INVALID_SOCKET)
	{
		memset(&adrIPX,0x00,sizeof(SOCKADDR_IPX)) ;
		adrIPX.sa_family = AF_IPX ;

		if	(bind(sock,(SOCKADDR*)&adrIPX,iSize)!=SOCKET_ERROR)
		{
			if	(getsockopt(sock,NSPROTO_IPX,IPX_MAX_ADAPTER_NUM,(char*)&iAdapters,&iOpt)!=SOCKET_ERROR)
			{
				while(iAdapters)
				{
					IPX_ADDRESS_DATA ipxAdrData ;
					memset( &ipxAdrData,0x00,
						sizeof(IPX_ADDRESS_DATA)) ;
					ipxAdrData.adapternum = --iAdapters ;
					iOpt = sizeof(IPX_ADDRESS_DATA) ;
					if(getsockopt(sock,NSPROTO_IPX,IPX_ADDRESS,
						(char*)&ipxAdrData,&iOpt)!=SOCKET_ERROR)
					{
						if(ipxAdrData.status==TRUE && 
							ipxAdrData.wan==FALSE)
						{
							memcpy(lpMacAddress,
								&ipxAdrData.nodenum,
								MAX_MAC_ADDRESS_IN_BYTE) ;
							bRet = TRUE ;
							break ;
						}
					}
				}
			}
		}
		closesocket(sock) ;
	}

	WSACleanup() ;
	return(bRet) ;
}

BOOL __GetMacAddressFromNetBIOS(LPBYTE lpMacAddress)
{
	NCB ncb ;
	LANA_ENUM le ;

	struct tagADAPTER
	{
		ADAPTER_STATUS adapt ;
		NAME_BUFFER    NameBuff[256] ;
	} adapter ;

	memset(&ncb,0x00,sizeof(NCB)) ;                  
	memset(&le,0x00,sizeof(LANA_ENUM)) ;             
	ncb.ncb_command = NCBENUM ;
	ncb.ncb_buffer = (UCHAR*)&le ;
	ncb.ncb_length = sizeof(LANA_ENUM) ;

	if(Netbios(&ncb)==NRC_GOODRET)
	{
		memset(&ncb,0x00,sizeof(NCB)) ;
		ncb.ncb_command = NCBRESET ;
		//          ncb.ncb_callname[0] = 20 ;                   
		ncb.ncb_lana_num = le.lana[0] ;              

		if(Netbios(&ncb)==NRC_GOODRET)
		{
			memset(&ncb,0x00,sizeof(NCB)) ;
			memset(&ncb.ncb_callname,' ',NCBNAMSZ) ;
			ncb.ncb_callname[0] = '*' ;
			//              ncb.ncb_callname[NCBNAMSZ-1] = NULL ;    
			ncb.ncb_command = NCBASTAT ;
			ncb.ncb_lana_num = le.lana[0] ;          
			ncb.ncb_buffer = (UCHAR*)&adapter ;
			ncb.ncb_length = sizeof(tagADAPTER) ;

			if(Netbios(&ncb)==NRC_GOODRET)
			{
				memcpy(lpMacAddress,&adapter.adapt.adapter_address,
					MAX_MAC_ADDRESS_IN_BYTE) ;
				return(TRUE) ;
			}
		}
	}
	return(FALSE) ;
}

//	특정 IP를 입력하면 해당 IP가 세팅된 네트웍 아답터의 mad address를 돌려주며
//	IP를 주지 않으면 첫번째 확인된 값을 리턴합니다.
//	원 코드 출처는 이곳입니다. http://msdn.microsoft.com/en-us/library/aa365819(v=VS.85).aspx
std::string getMacAddressByAdapter(std::string _ipaddress)
{
	/* Some general variables */
	ULONG ulOutBufLen;
	DWORD dwRetVal;
	int i;
	std::string retMacAddress = "";

	/* variables used for GetAdapterInfo */
	IP_ADAPTER_INFO *pAdapterInfo;
	IP_ADAPTER_INFO *pAdapter;

	pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(sizeof (IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL) {
		printf("Error allocating memory needed to call GetAdapterInfo\n");
		return retMacAddress;
	}
	ulOutBufLen = sizeof (IP_ADAPTER_INFO);

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(ulOutBufLen);
		if (pAdapterInfo == NULL) {
			printf("Error allocating memory needed to call GetAdapterInfo\n");
			return retMacAddress;
		}
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) != NO_ERROR) {
		printf("GetAdaptersInfo failed with error %d\n", dwRetVal);
		if (pAdapterInfo)
			FREE(pAdapterInfo);
		return retMacAddress;
	}

	pAdapter = pAdapterInfo;
	std::string incorrectIP = "0.0.0.0";	//	가상 adapter
	while (pAdapter) 
	{
		if(incorrectIP.compare(pAdapter->IpAddressList.IpAddress.String) !=0)
		{
			if(pAdapter->Address[1]==0 && pAdapter->Address[2]==0)	//	mac address가 빈 경우. netbios에서 발생했음. 혹시 몰라 삽입해 둠.
			{
				pAdapter = pAdapter->Next;
				continue;
			}

			if(_ipaddress.length() > 0)
			{
				if(_ipaddress.compare(pAdapter->IpAddressList.IpAddress.String)!= 0 )
				{
					pAdapter = pAdapter->Next;
					continue;
				}
			}

			char temp[4];
			for (i = 0; i < (int) pAdapter->AddressLength; i++) 
			{
				if (i == (pAdapter->AddressLength - 1))
					sprintf(temp,"%.2X", (int) pAdapter->Address[i]);
				else
					sprintf(temp,"%.2X-", (int) pAdapter->Address[i]);
				retMacAddress.append(temp);
			}
			return retMacAddress;
		}
		pAdapter = pAdapter->Next;
	}

	return retMacAddress;
}

std::string getMyIp() 
{
	WORD wVersion;
	WSADATA wsaData;
	char name[255];
	std::string ip;

	PHOSTENT hostinfo;
	wVersion = MAKEWORD(2,0);
	if(WSAStartup(wVersion, &wsaData) == 0) 
	{
		if(gethostname(name,sizeof(name)) == 0) 
		{
			if((hostinfo = gethostbyname(name)) != NULL) 
				ip = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
		}
		WSACleanup();
	}
	return ip;
}

std::string 
GetMacAddress()
{
	std::string	strMacAddress	=	getMacAddressByAdapter(getMyIp());

	if	(strMacAddress.length())
		return	strMacAddress;
	
	BYTE	strMac[256];
	char	strResultMacAddress[256];

	memset(strMac,0,sizeof(strMac));

	if	(!__GetMacAddressFromSock(strMac))
		__GetMacAddressFromNetBIOS(strMac);

	sprintf(strResultMacAddress,"%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",strMac[0],strMac[1],strMac[2], strMac[3], strMac[4], strMac[5]);

	strMacAddress	=	strResultMacAddress;

	return	strMacAddress;
}

std::string 
GetMacAddressByAdapter()
{
	return	getMacAddressByAdapter(getMyIp());
}

std::string 
GetMacAddressFromSock()
{
	std::string	strMacAddress;

	BYTE	strMac[256];
	char	strResultMacAddress[256];

	memset(strMac,0,sizeof(strMac));

	__GetMacAddressFromSock(strMac);

	sprintf(strResultMacAddress,"%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",strMac[0],strMac[1],strMac[2], strMac[3], strMac[4], strMac[5]);

	strMacAddress	=	strResultMacAddress;

	return	strMacAddress;
}

std::string 
GetMacAddressFromNetBIOS()
{
	std::string	strMacAddress;
	BYTE	strMac[256];
	char	strResultMacAddress[256];

	memset(strMac,0,sizeof(strMac));
	__GetMacAddressFromNetBIOS(strMac);

	sprintf(strResultMacAddress,"%.2X-%.2X-%.2X-%.2X-%.2X-%.2X",strMac[0],strMac[1],strMac[2], strMac[3], strMac[4], strMac[5]);

	strMacAddress	=	strResultMacAddress;

	return	strMacAddress;
}