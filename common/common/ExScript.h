#ifndef __EXSCRIPT_H
#define __EXSCRIPT_H

#include <windows.h>
#include "cSRVUTIL.h"

///////////////////////////////////////////////////////////////////
//	사용하지 않는 기능은 삭제하도록 하자.

#define dPS_SLEEP			Sleep
#define dPS_HANDLE_THREAD	HANDLE


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃class	CWWTime
#include <time.h>

class CWWTime
{
public:
	static int	GetDate();//int형의 YYYYMMDD
	static int	GetTimes();//시간 + 분 + 초 
	static int	GetNextDate();//int형의 YYYYMMDD
	static int	GetPrevDate();//int형의 YYYYMMDD
	static int	GetNextDate(int _istandardDate);//int형의 YYYYMMDD
	static int	GetPrevDate(int _istandardDate);//int형의 YYYYMMDD
	static int	GetMin();//int형의 MIN
	static int	GetHour();//int형의 HOUR
	static int	GetYear();//int형의 YEAR , + 1900 필요하면 직접 할 것 !!
	static int	GetMonth();//int형의 Month
	static int	GetDay();//	int형의 Day
	static int	GetWeekDay();// 0 ~ 6 : 0 -> 일요일
	static int	GetSec();
//	static void GetMSSQLDBDateNTime(char * _date);//	yyyy-mm-dd hh:mm:ss

	static int	CalcDayOnToday(int _yy,int _mm, int _dd);	//	오늘과 며칠 차이일까? 시작일부터 어제까지의 기간
	static int	CalcMin(int _a,int _b,int _isPlus, int * _Hour, int * _Week);
	static int	CalcHour(int _a,int _b,int _isPlus, int * _Week);
	static int	GetWeekDayOfaday(int _yy, int _mm, int _dd);	//	 0 ~ 6 : 0 -> 일요일
};
//┃class	CWWTime
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃class	CWWScript
#include <stdio.h>
/////////////////////////////////////////
//	스크립트의 일반적인 형태는 아래와 같이 구성된다.
//  name = data1 | data2 | data3 ...; 
//  .
//  .
//	예외적으로 아래와 같은 경우 .. 자신의 자식 데이터를 지정할 수 있다. 단, 부모는 자식을 한번만 가질 수 있다.
//	name_HAVE_CHILD;
//	{
//		child name = data1 | data2 | ...;
//		.
//		.
//	}
//
//	기본적으로 라인단위로 읽어 분석하는 스크립트이다.
//	긴 텍스트 데이터는 ""로 감싸고, 한 이름아래(부모든, 자식이든) 한개만이 존재 가능하다.
/////////////////////////////////////////

#define dWWS_MAXNAMESIZE	128
#define dWWS_MAXDATACOUNT	8
#define dWWS_MAXDATASIZE	128	//	데이터의 길이가 128을 넘어서는 안되지만.. 예외가 7있다.
#define dWWS_PARSECHAR		'='
#define dWWS_DATAPARSECHAR	'|'
#define dWWS_DATAENDCHAR	';'
#define dWWS_COMMENTS		"//"
#define	dWWS_NOTFILE		-1	//	파일을 찾을수가 없다.
#define dWWS_ENDMESSAGE		"END"//스크립트의 마지막을 표기할 때 사용하나, 특별한 경우가 아니라면 표기할 필요는 없다.
#define dWWS_HAVECHILD		"_HAS_CHILD"
#define dWWS_TSTEXT			"__THIS_IS_TEXT_DATA"	//	m_strBigData에 데이터가 저장될 경우.
#define dWWW_CHILD_START	'{'
#define dWWW_CHILD_END		'}'

class	cScpData
{
public:
	cScpData()
	{ 
		m_dwHashCode = 0; m_strBigData = NULL; m_strName[0] = NULL; m_strChildName[0] = NULL;
		memset(m_bData,0,sizeof(BYTE)*dWWS_MAXDATACOUNT*dWWS_MAXDATASIZE);
	}
	~cScpData(){ if(m_strBigData) { delete [] m_strBigData; m_strBigData = NULL; } }

	DWORD	m_dwHashCode;//이름을 hash 코드로 가지고 있다. 자식의 경우 부모와 자식 이름을 합친 후 HashCode를 찾아낸다.
	char	m_strName[dWWS_MAXNAMESIZE];//자식이 없는 경우 이 이름만 사용한다.
	char	m_strChildName[dWWS_MAXNAMESIZE];//자식일 경우, 부모의 이름과 자신의 이름을 다 가지고 있다.
	BYTE	m_bData[dWWS_MAXDATACOUNT][dWWS_MAXDATASIZE];//설정한 데이터들
	char*	m_strBigData;//예외적으로 큰 데이터.. " " 로 둘러싸여진 데이터.
};

class	CWWScript
{
public:

	enum
	{
		eWWS_DT_CHAR = 0,
		eWWS_DT_UCHAR,
		eWWS_DT_SHORT,
		eWWS_DT_USHORT,
		eWWS_DT_INT,
		eWWS_DT_UINT,
		eWWS_DT_LONG,
		eWWS_DT_ULONG,
		eWWS_DT_STRING,

		eMAXSCPDATACOUNT = 512,
		eMAXFILENAMELENGTH = 1024,
		eMAXLENGTHOFALINE = 4196,	//	한줄이 4K이하여야만 한다.
	};

	typedef union 
	{	char			dtChar;
		unsigned char	dtUChar;
		short			dtShort;
		unsigned short	dtUShort;
		int				dtInt;
		unsigned int	dtUInt;
		long			dtLong;
		unsigned long	dtULong;
		char*			dtString;
	}unWW_DataType;

private:
	char		m_strFileName[eMAXFILENAMELENGTH];	//	path까지 같이 전달 받는다.

	int			m_iTotCount;			//	읽어들인 총 데이타 종류의 수
	int			m_iScpCount;
	int			m_iRunning;
	int			m_isHaveChild;	//	어미의 이름뒤에 dWWS_HAVECHILD가 있냐?
	int			m_isChildData;	//	{를 만났냐?
	char		m_strParentName[dWWS_MAXNAMESIZE];

	cScpData*	m_cScp;	//	STL을 사용하고 싶으면 코드를 복사해서 변경하세요. 기본 코드는 다른 코드들과의 의존성을 최소한으로 유지하겠습니다.
	int			m_iCount;	//	m_cScp의 카운트

public:
	CWWScript();
	~CWWScript();

	int		Read(char * _strFilename,int _iMaxCount = eMAXSCPDATACOUNT);
	int		Save(char * _strFilename = NULL);//변경된 내용을 저장할 수 있다. NULL이면 기존의 파일을 백업하고, 똑같은 이름으로 생성한다. 혹은 주어진 이름으로 생성 가능하다.

	int		GetData(int _iType,unWW_DataType * _ret,char * _strName,int count=1,char * _strChildName = NULL);
	int		SetData(char * _value, char * _strName,int count=1,char * _strChildName = NULL);

private:
	int		ReadLine(FILE *f);
	int		ParseLine(char *_readline,int size);
};

#endif	//	__WWCOMMON_H



