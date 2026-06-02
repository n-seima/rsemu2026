// HardwareInfo.h
//
// 2007-02-08 : Kwon Yong Jae (amadeusj@gmail.com)
// 2007-05-31, 2007-06-04 : Lee Ji Hyun (ljh6341@hotmail.com)
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <deque>
#include <windows.h>

class cHDDINFO
{
public:
	TCHAR m_strHddName[16];
	int m_iTotalMB;
	int m_iFreeMB;

	cHDDINFO()
	{
		m_strHddName[0] = NULL;
	}

	void reflash();	//	현재 HDD에 대한 정보를 갱신한다.
};

class cMEMORYINFO
{
public:
	long m_lTotalSize;
	long m_lFreeSize;
	long m_lVirtualSize;

	cMEMORYINFO()
	{
		m_lTotalSize = 0;
	}
	void reflash();	//	현재 Memory에 대한 정보를 갱신한다.
};

#define dCPU_USAGE_LIMIT		70//CPU가 70%이상을 dCPU_USAGE_LIMIT_COUNT동안 지속적으로 사용하고 있다면 경고.
#define dCPU_USAGE_LIMIT_COUNT	30//초 이상 유지된다면
#define dCPU_USAGE_TEST_COUNT	10//10회의 평균을 낸다.

#define dMEMORY_USAGE_LIMIT		80	//	물리 메모리의 80%이상을 사용한다면 경고.

#define dHDD_FREESIZE_LIMIT		500	//	MB

class cCPUINFO
{
public:
	int m_iCPUSpeed;
	int m_iCPUPhysicalCount;
	int	m_iCPUCoreCount;
	int	m_iTotalUsage;
	int m_iCountOfLimitUsage;	//	이 카운터가 얼마 이상 유지된다면 문제가 되는 거겠지.
	TCHAR szCPUType[32];

	std::deque<int>		m_dqCpuUsage;

	cCPUINFO()
	{
		m_iCPUSpeed = -1;
		m_iTotalUsage = 0;
	}
	int getCPUUsage();
	void reflash();	//	현재 CPU의 사용량을 갱신한다.
};

typedef enum
{
	eHW_WARNING_NOTHING,
	eHW_WARNING_OVER_CPU_USAGE,	//	CPU가 일정 사용량을 지속적으로 넘어서고 있다.
	eHW_WARNING_OVER_MEMORY_USAGE,//메모리 사용량이 일정 기준치를 넘어섰다. leak 인가를 의심해보자. 혹은 엉뚱한 프로세스가 돌고 있는 건가?
	eHW_WARNING_HAVENOT_FREESIZE,	//	HDD에 빈공간이 없다. 조취를 취하자.

}eHW_WARNING;

class cHardwareInfo
{
public:
	std::deque< cHDDINFO > m_dqHDDList;
	int		m_iErrorHddIndex;
	cMEMORYINFO	m_cMemoryInfo;
	cCPUINFO	m_cCPUInfo;

public:
	cHardwareInfo(void);
	~cHardwareInfo(void);

	//	실시간으로 검사하기
	int getCPUUsage();	//	sleep 사용. 주의할 것!!
	void getCPUDetailInfo(cCPUINFO * _info);	//	sleep 사용. 주의할 것!!
	cCPUINFO* getCPUDetailInfo() { return &m_cCPUInfo; }	//	sleep 사용. 주의할 것!!
	void getMemoryInfo(cMEMORYINFO * _info);
	cMEMORYINFO* getMemoryInfo() { return &m_cMemoryInfo; }

	std::deque<cHDDINFO> * getHDDInfoAll();
	cHDDINFO * getHDDInfo(int _idx);

	void reflash();	//	수동으로 정보를 갱신하도록 한다.
	int	 checkHW();	//	기준점을 넘어선 HW가 있는지를 확인한다.
//	void reflashThread();//한개의 Thread를 이용해서 정보를 갱신하도록 하며, 한개 이상이 만들어 질 수는 없다. 귀찮다. -- 나중에 하자.
};
