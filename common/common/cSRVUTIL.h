// cSRVUTIL.h: interface for the cSRVUTIL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSRVUTIL_H__B0E486B9_8C99_4A5C_B17B_CA3852AC9428__INCLUDED_)
#define AFX_CSRVUTIL_H__B0E486B9_8C99_4A5C_B17B_CA3852AC9428__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <string>
#include <vector>

typedef union 
{
	struct 
	{
		DWORD	year		:	6;	//	연
		DWORD	month		:	4;	//	월
		DWORD	day			:	5;	//	일
		DWORD	hour		:	5;	//	시
		DWORD	minute		:	6;	//	분
		DWORD	second		:	6;	//	초
	};

	DWORD	m_dwValue;
}	cpUTime;	//	기존의 UTime과 완벽하게 동일하다. 단지, include 하는 파일을 없애기 위함.

struct	CM_COMPRESS;

class cSRVUTIL
{
public:
	enum
	{
		eDATECHECK_YY,
		eDATECHECK_MM,
		eDATECHECK_DD,
		eDATECHECK_HH,
		eDATECHECK_MIN,
		eDATECHECK_SS,

		eDATACHECK_COUNT,
	};

	////////////////////////////////////////////////////////////////////
	//	unicode 관련
	static char m_pStrChar[1024];		//	간단한 텍스트 변역용
	static wchar_t m_pStrWChar[1024];	//	간단한 텍스트 변역용
	//	return value 
	//	0 : 잘못된 데이터가 입력되거나, 주어진 포인터가 잘못되었다.
	//	>0 : 변환된 사이즈가 전달된다.
	static const char * UnicodeToAscii(const wchar_t * _lpStr1,int opt = CP_ACP);
	static const wchar_t * AsciiToUnicode(const char * _lpStr1,int opt = CP_ACP);
	static int UnicodeToAscii(const wchar_t * _lpStr1,char * _lpStr2, int _size =0,int opt = CP_ACP);
	static int AsciiToUnicode(char * _lpStr1,wchar_t * _lpStr2, int _size =0,int opt = CP_ACP);
	//	unicode 관련
	////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//	IP 관련
	static std::vector<std::string>	getIPFilterList(std::vector<std::string>& _filteringList);	//	주어진 정보를 제외한 IP리스트를 넘긴다.
	static std::string getMyIp();
	static std::string getMacAddressByAdapter(std::string _ipaddress);
	//
	//////////////////////////////////////////////////////////////////////////


	static int	GetTime();//int형의 YYYYMMDD
	static int	GetNextTime();//int형의 YYYYMMDD
	static int	GetPrevTime();//int형의 YYYYMMDD
	static int	GetNextTime(int _istandardDate);//int형의 YYYYMMDD
	static int	GetPrevTime(int _istandardDate);//int형의 YYYYMMDD
	static int	GetMin();//int형의 MIN
	static int	GetHour();//int형의 HOUR
	static int	GetYear();//int형의 YEAR , + 1900 필요하면 직접 할 것 !!
	static int	GetMonth();//int형의 Month
	static int	GetDay();//	int형의 Day
	static int	GetWeekDay();		//	 0 ~ 6 : 0 -> 일요일
	static int	GetSec();
	static int	GetTimes();			//	시간 + 분 + 초 
	static void GetUTime(cpUTime *_pUTime,DWORD _sec =0);//현재를 UTime에 담아서 올려보낸다.
	static void GetMSSQLDBDateNTime(char * _date);//	yyyy-mm-dd hh:mm:ss
	static void GetMSSQLDBDateNTime(char * _date, cpUTime *_pUTime);//	yyyy-mm-dd hh:mm:ss
	static void getTimeToUTime(char *_pstrDate,cpUTime *_pUTime);//DB에서 얻어온 날짜를 UTime으로 변경한다.

	static int	CalcDayOnToday(int _yy,int _mm, int _dd/*, int _iReturnType = eDATECHECK_MIN*/);	//	오늘과 며칠 차이일까?
	static int	CalcTermToday(cpUTime _pUTime,int _iReturnType = eDATECHECK_MIN);	//	오늘과 며칠 차이일까?
	static unsigned int	CalcTermOfDayToDay(cpUTime _pPrevUTime, cpUTime _pUTime, int _iReturnType = eDATECHECK_MIN);//두 날짜의 시간차를 계산한다.
	static int	CalcMin(int _a,int _b,int _isPlus, int * _Hour, int * _Week);
	static int	CalcHour(int _a,int _b,int _isPlus, int * _Week);
	static int	GetWeekDayOfaday(int _yy, int _mm, int _dd);	//	 0 ~ 6 : 0 -> 일요일
	static unsigned long GetHashCode(BYTE* _lpString);
#ifdef	__USE_ZLIB
	static WORD MakeCompressPacket(CM_COMPRESS *_lpCompPacket,void *_lpPacket,int _iPacketSize);
	static WORD Compress(unsigned char * _sourcedata, unsigned char * _targetdata, WORD _datasize);
	static WORD Uncompress(unsigned char * _sourcedata, unsigned char * _targetdata, WORD _datasize, WORD _sourcesize);
#endif	//	__USE_ZLIB
};

#endif // !defined(AFX_CSRVUTIL_H__B0E486B9_8C99_4A5C_B17B_CA3852AC9428__INCLUDED_)
