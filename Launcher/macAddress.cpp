#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>


#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <string>

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

//	특정 IP를 입력하면 해당 IP가 세팅된 네트웍 아답터의 mad address를 돌려주며
//	IP를 주지 않으면 첫번째 확인된 값을 리턴합니다.
//	원 코드 출처는 이곳입니다. http://msdn.microsoft.com/en-us/library/aa365819(v=VS.85).aspx
std::string getMacAddressByAdapter(std::string _ipaddress)
{
	/* Some general variables */
	ULONG ulOutBufLen;
	DWORD dwRetVal;
	int i;
	std::string retMacAddress;

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
					sprintf_s(temp,4,"%.2X", (int) pAdapter->Address[i]);
				else
					sprintf_s(temp,4,"%.2X-", (int) pAdapter->Address[i]);
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
GetMacAddressByAdapter()
{
	return	getMacAddressByAdapter(getMyIp());
}