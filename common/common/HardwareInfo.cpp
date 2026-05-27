//#include "StdAfx.h"
#include "HardwareInfo.h"

#include <pdh.h>

#define dCONTEXT_SWITCHES_COUNT	TEXT("\\System\\Context Switches/sec")
#define dCPU_USAGE_TOTAL		TEXT("\\Processor(_Total)\\% Processor Time")


#pragma comment(lib, "Pdh.lib")

void cHDDINFO::reflash()
{
	DWORD dwCom=0,dwFSf=0,dwVS=0;

	ULARGE_INTEGER Fbatc;
	ULARGE_INTEGER lnTotBytes;
	ULARGE_INTEGER lnAvailable;

	cHDDINFO	hddInfo;
	if( m_strHddName[0] == NULL)
		return;
	if(GetVolumeInformation(m_strHddName, NULL,NULL, &dwVS,&dwCom,&dwFSf,NULL,NULL))
	{
		//	NTFS만 확인한다. 나머지는 무시하자.
		if(dwFSf==459007 && GetDiskFreeSpaceEx(m_strHddName, &Fbatc, &lnTotBytes, &lnAvailable))
		{
			// 총용량
			double dTotalBytes = lnTotBytes.LowPart + (double)lnTotBytes.HighPart * (double)4294967296;		
			m_iTotalMB = (int)(dTotalBytes / 1024 / 1024);		// Convert (MB)     

			// 남은용량
			double dFreeBytes = lnAvailable.LowPart + (double)lnAvailable.HighPart * (double)4294967296;
			m_iFreeMB = (int)(dFreeBytes / 1024 / 1024);			// Convert (MB)
		}
	}
}

void cMEMORYINFO::reflash()
{
	MEMORYSTATUS memoryStatus;

	memset (&memoryStatus, 0, sizeof (MEMORYSTATUS));
	memoryStatus.dwLength = sizeof (MEMORYSTATUS);

	GlobalMemoryStatus (&memoryStatus);

	m_lTotalSize = (long)(memoryStatus.dwTotalPhys / 1024);
	m_lFreeSize = (long)(memoryStatus.dwAvailPhys / 1024);
	m_lVirtualSize = (long)(memoryStatus.dwTotalVirtual / 1024);
}

void cCPUINFO::reflash()
{
	if(m_iCPUSpeed < 0)		return;

	HQUERY          hQuery;
	PDH_STATUS      pdhStatus;
	DWORD           dwFormat = PDH_FMT_DOUBLE; 
	PDH_FMT_COUNTERVALUE  *lpItemBuffer;
	HCOUNTER        hCounter;

	// Open a query object.
	pdhStatus = PdhOpenQuery (NULL,0,&hQuery);

	if( pdhStatus != ERROR_SUCCESS )
		return ;

	// Add the counter that created the data in the log file.
	pdhStatus = PdhAddCounter (hQuery,dCPU_USAGE_TOTAL,0,&hCounter);

	// Allocate the counter value structure.
	lpItemBuffer = (PDH_FMT_COUNTERVALUE *) GlobalAlloc(GPTR, sizeof(PDH_FMT_COUNTERVALUE));

	if( NULL == lpItemBuffer )
		return ;

	pdhStatus = PdhCollectQueryData(hQuery);
	pdhStatus = PdhGetFormattedCounterValue (hCounter,dwFormat,(LPDWORD)NULL,lpItemBuffer);

	int iTmpCount = 0;
	int valueList[dCPU_USAGE_TEST_COUNT];

	while(iTmpCount < dCPU_USAGE_TEST_COUNT)
	{ 
		// Format the performance data record.
		pdhStatus = PdhCollectQueryData(hQuery);
		pdhStatus = PdhGetFormattedCounterValue (hCounter,dwFormat,(LPDWORD)NULL,lpItemBuffer);

		valueList[iTmpCount] = (int)lpItemBuffer->doubleValue;

		Sleep(20);
		iTmpCount++;
	}

	// Close the query and the log file.
	pdhStatus = PdhCloseQuery(hQuery);

	for(int i=1;i<dCPU_USAGE_TEST_COUNT; i++)
	{
		valueList[0] += valueList[i];
	}

	m_iTotalUsage = (int)(valueList[0] / dCPU_USAGE_TEST_COUNT);
	if(m_iTotalUsage >= dCPU_USAGE_LIMIT)
		++m_iCountOfLimitUsage;
	else
		m_iCountOfLimitUsage = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc :
///////////////////////////////////////////////////////////////////////////////////////////////////////////
cHardwareInfo::cHardwareInfo(void)
{
	getHDDInfoAll();
	getCPUDetailInfo(&m_cCPUInfo);
	getMemoryInfo(&m_cMemoryInfo);
	m_iErrorHddIndex = -1;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc :
///////////////////////////////////////////////////////////////////////////////////////////////////////////
cHardwareInfo::~cHardwareInfo(void)
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : 메모리의 현황을 알아낸다
//        iMemTotalSize : 실제 메모리의 전체 크기 (KB 단위)
//        iFreeMemSize  : 사용 가능한 실제 메모리의 크기 (KB 단위)
//        iVirtualSize  : 가상 메모리의 전체 크기  (KB 단위)
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cHardwareInfo::getMemoryInfo(cMEMORYINFO * _info)
{
	_info->reflash();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : CPU 의 현황을 알아낸다
//        어쩔수 없이 Sleep() 를 사용해야한다
///////////////////////////////////////////////////////////////////////////////////////////////////////////
int cHardwareInfo::getCPUUsage()
{
	m_cCPUInfo.reflash();

	return m_cCPUInfo.m_iTotalUsage;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : CPU 종류를 알아낸다

//        _pm_iCPUSpeed : CPU의 속도. 기록된 시스템에서만 읽어온다.
//        _pm_iCPUPhysicalCount   : 프로세서의 개수. NT의 경우에만 의미가 있다.
//        _pszCPUType : 프로세서의 종류
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void cHardwareInfo::getCPUDetailInfo(cCPUINFO * _info)
{
	SYSTEM_INFO sysInfo;
	LONG lResult;
	HKEY hKey;
	DWORD dwData;
	DWORD dwDataSize;

	// 프로세서의 속도를 얻어낸다.
	lResult = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		TEXT("Hardware\\Description\\System\\CentralProcessor\\0"), 0, KEY_QUERY_VALUE, &hKey);
	if(lResult == ERROR_SUCCESS) {
		lResult = ::RegQueryValueEx(hKey, TEXT("~MHz"), NULL, NULL,(LPBYTE)&dwData, &dwDataSize);
		_info->m_iCPUSpeed = dwData;
	}
	RegCloseKey(hKey);

	// 하드웨어 정보를 얻어낸다.
	GetSystemInfo(&sysInfo);

	// 프로세서 타입부터 검사한다.
	if(sysInfo.dwProcessorType  == PROCESSOR_INTEL_386){
		strcpy(_info->szCPUType, TEXT("Intel 386"));
	}else if(sysInfo.dwProcessorType  == PROCESSOR_INTEL_486){
		strcpy(_info->szCPUType,  TEXT("Intel 486"));
	}else if(sysInfo.dwProcessorType  == PROCESSOR_INTEL_PENTIUM){
		if(sysInfo.wProcessorLevel == 6){
			strcpy(_info->szCPUType,  TEXT("Intel Pentium (II/Pro)"));
		}else{
			strcpy(_info->szCPUType,  TEXT("Intel Pentium"));
		}
	}else{
		strcpy(_info->szCPUType,  TEXT("Not Find!"));
	}

	// 프로세서의 갯수를 검사한다.
	_info->m_iCPUPhysicalCount = sysInfo.dwNumberOfProcessors;

	//	사용량을 측정한다.
	_info->m_iTotalUsage = getCPUUsage();
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Name : 
// Desc : HDD 현황 , 동시에 접속하지 말 것.
///////////////////////////////////////////////////////////////////////////////////////////////////////////
std::deque<cHDDINFO> * cHardwareInfo::getHDDInfoAll()
{
	m_dqHDDList.clear();

	int iHDDCnt = 0;

	ULARGE_INTEGER Fbatc;
	ULARGE_INTEGER lnTotBytes;
	ULARGE_INTEGER lnAvailable;
	
	//	네트웍 드라이브까지 잡는다. volume Serial이 일정한거 같긴 한데.. 테스트 해봐야 되는 문제니. 'Z'까지 말고.. 대충 'R'정도까지만 읽던가.
	for(int i='A'; i<'R'; i++)
	{
		DWORD dwCom=0,dwFSf=0,dwVS=0;
		cHDDINFO	hddInfo;

		sprintf(hddInfo.m_strHddName,"%c:/",i);
		if(GetVolumeInformation(hddInfo.m_strHddName, NULL,NULL, &dwVS,&dwCom,&dwFSf,NULL,NULL))
		{
			//	NTFS만 확인한다. 나머지는 무시하자.
			if(dwFSf==459007 && GetDiskFreeSpaceEx(hddInfo.m_strHddName, &Fbatc, &lnTotBytes, &lnAvailable))
			{
				// 총용량
				double dTotalBytes = lnTotBytes.LowPart + (double)lnTotBytes.HighPart * (double)4294967296;		
				hddInfo.m_iTotalMB = (int)(dTotalBytes / 1024 / 1024);		// Convert (MB)     

				// 남은용량
				double dFreeBytes = lnAvailable.LowPart + (double)lnAvailable.HighPart * (double)4294967296;
				hddInfo.m_iFreeMB = (int)(dFreeBytes / 1024 / 1024);			// Convert (MB)

				m_dqHDDList.push_back( hddInfo );
			}
		}
	}

	return &m_dqHDDList;
}

void cHardwareInfo::reflash()
{
	for(unsigned int i=0;i<m_dqHDDList.size(); i++)
		m_dqHDDList.at(i).reflash();

	m_cMemoryInfo.reflash();
	m_cCPUInfo.reflash();
}

int	 cHardwareInfo::checkHW()
{
	m_cCPUInfo.reflash();
	if(m_cCPUInfo.m_iCountOfLimitUsage>dCPU_USAGE_LIMIT_COUNT && m_cCPUInfo.m_iTotalUsage>dCPU_USAGE_LIMIT)
		return eHW_WARNING_OVER_CPU_USAGE;

	m_cMemoryInfo.reflash();
	if( (m_cMemoryInfo.m_lTotalSize - m_cMemoryInfo.m_lFreeSize) < (m_cMemoryInfo.m_lTotalSize * (dMEMORY_USAGE_LIMIT/100)) )
		return eHW_WARNING_OVER_MEMORY_USAGE;

	for(int i=0;i<m_dqHDDList.size(); i++)
	{
		if(m_dqHDDList.at(i).m_iFreeMB < dHDD_FREESIZE_LIMIT)
		{
			m_iErrorHddIndex = i;
			return eHW_WARNING_HAVENOT_FREESIZE;
		}
	}

	return eHW_WARNING_NOTHING;
}