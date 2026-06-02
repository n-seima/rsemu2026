// cSRVUTIL.cpp: implementation of the cSRVUTIL class.
//
//////////////////////////////////////////////////////////////////////
// #pragma comment(lib, "iphlpapi.lib")
// #pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
// #include <ws2tcpip.h>
// #include <iphlpapi.h>
#include <windows.h>

#include "cSRVUTIL.h"
#include "time.h"
#include "stdio.h"
#include "../packet/cPACKET_BASE.h"


#ifdef	__USE_ZLIB
	#include "zlib.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static int monthend[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

int cSRVUTIL::GetTime()
{
	int m_iResult = 0;

	time_t tTime;
	struct tm *tmTime;
	tTime = time(NULL);
	tmTime = localtime(&tTime);
	m_iResult = (tmTime->tm_year+1900)*10000;
	m_iResult += (tmTime->tm_mon+1)*100;
	m_iResult += tmTime->tm_mday;

	return m_iResult;
}

void cSRVUTIL::GetUTime(cpUTime *_pUTime,DWORD _sec)
{
	time_t tTime;
	struct tm *tmTime;
	tTime = time(NULL);
	tmTime = localtime(&tTime);

	_pUTime->year = tmTime->tm_year-100;
	_pUTime->month = tmTime->tm_mon+1;
	_pUTime->day = tmTime->tm_mday;
	_pUTime->hour = tmTime->tm_hour;
	_pUTime->minute = tmTime->tm_min;
	_pUTime->second = tmTime->tm_sec;
	
	if(_sec)	//	이 초만큼 더해서 보내준다.
	{
		_pUTime->second += _sec;
		if(_pUTime->second>=60)
		{
			++_pUTime->minute;
			_pUTime->second -= 60;
			if(_pUTime->minute>=60)
			{
				++_pUTime->hour;
				_pUTime->minute-=60;
				if(_pUTime->hour>=24)
				{
					++_pUTime->day;
					_pUTime->hour -= 24;
					
					if( (_pUTime->year%4 == 0) && (_pUTime->year%100 !=0) || _pUTime->year%400 == 0)	monthend[2] = 29;
					else	monthend[2] = 28;
					if(monthend[_pUTime->month] > (int)_pUTime->day)
					{
						_pUTime->day -= monthend[_pUTime->month];
						++_pUTime->month;
						if(_pUTime->month>12)
						{
							++_pUTime->year;
							_pUTime->month -= 12;
						}	//	end month
					}	//	end day
				}	//	end hour
			}	//	end minute
		}	//	end sec
	}
}

int	cSRVUTIL::GetNextTime()
{
	int m_iY=0,m_iM=0,m_iD=0;

	time_t tTime;
	struct tm *tmTime;
	tTime = time(NULL);
	tmTime = localtime(&tTime);
	m_iY = (tmTime->tm_year+1900);
	m_iM = tmTime->tm_mon+1;
	m_iD = tmTime->tm_mday;
	++m_iD;
	if( (m_iY%4 == 0) && (m_iY%100 !=0) || m_iY%400 == 0)
		monthend[2] = 29;
	else
		monthend[2] = 28;

	if(monthend[m_iM]+1==m_iD)
	{
		m_iD=1;
		++m_iM;
		if(m_iM>12)
		{
			++m_iY;
			m_iM=1;
		}
	}

	return m_iY*10000+m_iM*100+m_iD;
}

int	cSRVUTIL::GetNextTime(int _istandardDate)
{
	int m_iY=0,m_iM=0,m_iD=0;

	m_iY = _istandardDate/10000;
	m_iM = (_istandardDate%10000)/100;
	m_iD = _istandardDate%100;
	m_iD++;
	if( (m_iY%4 == 0) && (m_iY%100 !=0) || m_iY%400 == 0)
		monthend[2] = 29;
	else
		monthend[2] = 28;

	if(monthend[m_iM]+1==m_iD)
	{
		m_iD=1;
		++m_iM;
		if(m_iM>12)
		{
			++m_iY;
			m_iM=1;
		}
	}

return m_iY*10000+m_iM*100+m_iD;
}

int	cSRVUTIL::GetPrevTime(int _istandardDate)
{
	int m_iY=0,m_iM=0,m_iD=0;

	m_iY = _istandardDate/10000;
	m_iM = (_istandardDate%10000)/100;
	m_iD = _istandardDate%100;
	--m_iD;

	if( ((m_iY%4 == 0) && (m_iY%100 !=0)) || (m_iY%400 == 0)  )
		monthend[2] = 29;
	else
		monthend[2] = 28;

	if(m_iD == 0)
	{
		--m_iM;	
		if(m_iM==0)
		{
			m_iM = 12;	--m_iY;
		}
		m_iD=monthend[m_iM];
	}

	return m_iY*10000+m_iM*100+m_iD;
}

int	cSRVUTIL::GetPrevTime()
{
	int m_iY=0,m_iM=0,m_iD=0;

	time_t tTime;
	struct tm *tmTime;
	tTime = time(NULL);
	tmTime = localtime(&tTime);
	m_iY = (tmTime->tm_year+1900);
	m_iM = tmTime->tm_mon+1;
	m_iD = tmTime->tm_mday;
	--m_iD;
	if( ((m_iY%4 == 0) && (m_iY%100 !=0)) || (m_iY%400 == 0))
		monthend[2] = 29;
	else
		monthend[2] = 28;

	if(m_iD == 0)
	{
		--m_iM;	
		if(m_iM==0)
		{
			m_iM = 12;	--m_iY;
		}
		m_iD=monthend[m_iM];
	}

	return m_iY*10000+m_iM*100+m_iD;
}

int	cSRVUTIL::GetMin()
{
	int m_iResult=0;
	time_t tTime;
	struct tm *tmTime;
	tTime = time(NULL);
	tmTime = localtime(&tTime);
	m_iResult = (tmTime->tm_min);

	return m_iResult;
}

int	cSRVUTIL::GetHour()
{
	int m_iResult=0;
	time_t tTime;
	struct tm *tmTime;
	tTime = time(NULL);
	tmTime = localtime(&tTime);
	m_iResult = (tmTime->tm_hour);

	return m_iResult;
}
int	cSRVUTIL::GetYear()
{
	int m_iResult=0;
	time_t tTime;
	struct tm *tmTime;
	tTime = time(NULL);
	tmTime = localtime(&tTime);
	m_iResult = (tmTime->tm_year);

	return m_iResult;
}

int	cSRVUTIL::GetMonth()
{
	int m_iResult=0;
	time_t tTime;
	struct tm *tmTime;
	tTime = time(NULL);
	tmTime = localtime(&tTime);
	m_iResult = (tmTime->tm_mon);

	return m_iResult;
}

int	cSRVUTIL::GetDay()
{
	int m_iResult=0;
	time_t tTime;
	struct tm *tmTime;
	tTime = time(NULL);
	tmTime = localtime(&tTime);
	m_iResult = (tmTime->tm_mday);

	return m_iResult;
}

int	cSRVUTIL::GetWeekDay()		//	 0 ~ 6 : 0 -> 일요일
{
	int m_iResult=0;
	time_t tTime;
	struct tm *tmTime;
	tTime = time(NULL);
	tmTime = localtime(&tTime);
	m_iResult = (tmTime->tm_wday);

	return m_iResult;
}

int cSRVUTIL::GetSec()
{
	int m_iResult=0;
	time_t tTime;
	struct tm *tmTime;
	tTime = time(NULL);
	tmTime = localtime(&tTime);
	m_iResult = (tmTime->tm_sec);

	return m_iResult;
}

int cSRVUTIL::GetTimes()
{
	int m_iResult=0;
	m_iResult = GetHour()*10000+ GetMin()*100+ GetSec();

	return m_iResult;
}

void cSRVUTIL::getTimeToUTime(char *_pstrDate,cpUTime *_pUTime)
{
/*	int	iYear,iMonth,iDay,iHour,iMinute,iSec;
#ifdef _FOR_KOREA
	{
		char	strTime[8];
		int idx = 0;
		memcpy(strTime,_lpstrDate+idx,4);		//	연
		strTime[4]	=	0;
		iYear		=	atoi(strTime);
		
		idx += 5;

		memcpy(strTime,_lpstrDate+idx,2);	//	월
		strTime[2]	=	0;
		iMonth		=	atoi(strTime);

		idx += 3;

		memcpy(strTime,_lpstrDate+idx,2);	//	일
		strTime[2]	=	0;
		iDay		=	atoi(strTime);

		idx += 3;

		int addHour = 0;
		if(strncmp(_lpstrDate+idx,"오전",4)==0)
		{
			idx += 4;
		}else
		if(strncmp(_lpstrDate+idx,"오후",4)==0)
		{
			addHour = 1;
			idx += 4;
		}else
		{
			addHour = 2;
		}

		if(_lpstrDate[idx+2]==':')
		{
			memcpy(strTime,_lpstrDate+idx,2);	//	시
			idx -= 1;
		}else{
			memcpy(strTime,_lpstrDate+idx+1,2);	//	시
		}
		strTime[2]	=	0;
		iHour		=	atoi(strTime);
		if(addHour)
		{
			iHour += 12;
		}else if(addHour == 0)
		{
			if(iHour==12)
				iHour = 0;
		}
		if(iHour>=24)
		{
			iHour -= 12;
		}

		idx += 4;

		memcpy(strTime,_lpstrDate+idx,2);	//	분
		strTime[2]	=	0;
		iMinute		=	atoi(strTime);

		idx += 3;

		memcpy(strTime,_lpstrDate+idx,2);	//	분
		strTime[2]	= 0;
		iSec		=	atoi(strTime);
	}
#elif _FOR_JAPAN
	{
		char	strTime[8];
		int idx = 0;
		memcpy(strTime,_lpstrDate+idx,4);		//	연
		strTime[4]	=	0;
		iYear		=	atoi(strTime);
		
		idx += 5;

		memcpy(strTime,_lpstrDate+idx,2);	//	월
		strTime[2]	=	0;
		iMonth		=	atoi(strTime);

		idx += 3;

		memcpy(strTime,_lpstrDate+idx,2);	//	일
		strTime[2]	=	0;
		iDay		=	atoi(strTime);

		idx += 3;

		//	일본은 yyyy-mm-dd hh:mm:ss형태 
		if(_lpstrDate[idx+1]==':')
		{
			memcpy(strTime,_lpstrDate+idx,1);	//	시
			idx -= 1;
			strTime[1]	=	0;
		}else
			memcpy(strTime,_lpstrDate+idx,2);	//	시
		strTime[2]	=	0;
		iHour		=	atoi(strTime);
		idx += 3;

		memcpy(strTime,_lpstrDate+idx,2);	//	분
		strTime[2]	=	0;
		iMinute		=	atoi(strTime);

		idx += 3;

		memcpy(strTime,_lpstrDate+idx,2);	//	분
		strTime[2]	= 0;
		iSec		=	atoi(strTime);
	}
#endif

	if (iYear	<	2000)
		_lpUTime->m_dwValue	=	0;
	else
	{
		_lpUTime->year	=	iYear-2000;
		_lpUTime->month	=	iMonth;
		_lpUTime->day	=	iDay;
		_lpUTime->hour	=	iHour;
		_lpUTime->minute=	iMinute;
		_lpUTime->second=	iSec;
	}
*/
}

void cSRVUTIL::GetMSSQLDBDateNTime(char * _date)//	yyyy-mm-dd hh:mm:ss
{
	int m_iResult=0;
	time_t tTime;
	struct tm *tmTime;
	tTime = time(NULL);
	tmTime = localtime(&tTime);

	sprintf(_date,"%d-%02d-%02d %02d:%02d:%02d",tmTime->tm_year+1900,tmTime->tm_mon,tmTime->tm_mday,tmTime->tm_hour,tmTime->tm_min,tmTime->tm_sec);
}

void cSRVUTIL::GetMSSQLDBDateNTime(char * _date, cpUTime *_pUTime)//	yyyy-mm-dd hh:mm:ss
{
	sprintf(_date,"%d-%02d-%02d %02d:%02d:%02d",_pUTime->year,_pUTime->month,_pUTime->day,_pUTime->hour,_pUTime->minute,_pUTime->second);
}

int	cSRVUTIL::CalcMin(int _a,int _b,int _isPlus, int * _Hour, int * _Week)
{
	if(_isPlus)
	{
		if(_a+_b >= 60)
		{
			if(_Hour)
			{
				*_Hour = CalcHour(*_Hour,1,1,_Week);
			}
			return (_a+_b)%60;
		}
		else			return _a+_b;
	}else{
		if(_a-_b < 0)
		{
			if(_Hour)
			{
				*_Hour = CalcHour(*_Hour,1,0,_Week);
			}
			return 60 + (_a-_b);
		}
		else			return _a-_b;
	}
}

int	cSRVUTIL::CalcTermToday(cpUTime _pUTime,int _iReturnType)
{
	time_t tTime,tSpTime,tRetTime;
	struct tm tmTime;
	struct tm tmSpTime;
	tTime = time(NULL);
	tmTime = *localtime(&tTime);

	tmSpTime.tm_year = _pUTime.year+100;
	tmSpTime.tm_mon  = _pUTime.month-1;
	tmSpTime.tm_mday = _pUTime.day;
	tmSpTime.tm_hour = _pUTime.hour;
	tmSpTime.tm_min = _pUTime.minute;
	tmSpTime.tm_sec = _pUTime.second;

	tSpTime = mktime(&tmSpTime);
	
	tRetTime = abs(tTime - tSpTime);
	int ret = 0;
	switch(_iReturnType)
	{
	case eDATECHECK_YY:	//	365일 기준.
		ret = tRetTime  / 60 / 60 / 24 / 30 / 365;
		break;
	case eDATECHECK_MM:	//	30일 기준.
		ret = tRetTime  / 60 / 60 / 24 / 30;
		break;
	case eDATECHECK_DD:
		ret = tRetTime  / 60 / 60 / 24;
		break;
	case eDATECHECK_HH:
		ret = tRetTime  / 60 / 60 ;
		break;
	case eDATECHECK_MIN:
		ret = tRetTime  / 60 ;
		break;
	case eDATECHECK_SS:
		ret = tRetTime;
		break;
	}
	
	return ret;
}

unsigned int cSRVUTIL::CalcTermOfDayToDay(cpUTime _pPrevUTime, cpUTime _pUTime, int _iReturnType)
{
	time_t tTime,tSpTime,tRetTime;
	struct tm tmTime;
	struct tm tmSpTime;

	tmTime.tm_year = _pPrevUTime.year+100;
	tmTime.tm_mon  = _pPrevUTime.month-1;
	tmTime.tm_mday = _pPrevUTime.day;
	tmTime.tm_hour = _pPrevUTime.hour;
	tmTime.tm_min = _pPrevUTime.minute;
	tmTime.tm_sec = _pPrevUTime.second;
	tTime = mktime(&tmTime);

	tmSpTime.tm_year = _pUTime.year+100;
	tmSpTime.tm_mon  = _pUTime.month;
	tmSpTime.tm_mday = _pUTime.day;
	tmSpTime.tm_hour = _pUTime.hour;
	tmSpTime.tm_min = _pUTime.minute;
	tmSpTime.tm_sec = _pUTime.second;
	tSpTime = mktime(&tmSpTime);
	
	tRetTime = abs(tSpTime - tTime);
	int ret = 0;
	switch(_iReturnType)
	{
	case eDATECHECK_YY:	//	365일 기준.
		ret = tRetTime  / 60 / 60 / 24 / 30 / 365;
		break;
	case eDATECHECK_MM:	//	30일 기준.
		ret = tRetTime  / 60 / 60 / 24 / 30;
		break;
	case eDATECHECK_DD:
		ret = tRetTime  / 60 / 60 / 24;
		break;
	case eDATECHECK_HH:
		ret = tRetTime  / 60 / 60 ;
		break;
	case eDATECHECK_MIN:
		ret = tRetTime  / 60 ;
		break;
	case eDATECHECK_SS:
		ret = tRetTime;
		break;
	}
	
	return ret;
}

int	cSRVUTIL::CalcDayOnToday(int _yy,int _mm, int _dd)
{
	time_t tTime,tSpTime,tRetTime;
	struct tm tmTime;
	struct tm tmSpTime;
	tTime = time(NULL);
	tmTime = *localtime(&tTime);

	tmSpTime.tm_year = _yy - 1900;
	tmSpTime.tm_mon  = _mm-1;
	tmSpTime.tm_mday = _dd;
	tmSpTime.tm_hour = 0;
	tmSpTime.tm_min = 0;
	tmSpTime.tm_sec = 0;

	tSpTime = mktime(&tmSpTime);
	
	tRetTime = abs(tTime - tSpTime);
	int ret = tRetTime  / 60 / 60 / 24;
	return ret;
}

int	cSRVUTIL::CalcHour(int _a,int _b,int _isPlus, int * _Week)
{
	if(_isPlus)
	{
		if(_a+_b >= 24)
		{
			if(_Week)
			{
				*_Week += 1;
				if(*_Week>6)	*_Week = 7 - *_Week;
			}
			return (_a+_b)%24;
		}
		else			return _a+_b;
	}else{
		if(_a-_b < 0)
		{
			if(_Week)
			{
				*_Week -= 1;
				if(*_Week<0)	*_Week = 7 + *_Week;
			}
			return 24 + (_a-_b);
		}
		else			return _a-_b;
	}
}

int	cSRVUTIL::GetWeekDayOfaday(int _yy, int _mm, int _dd)	//	 0 ~ 6 : 0 -> 일요일
{
	time_t tTime,tAfterTime;
	struct tm tmTime;
	tTime = time(NULL);
	tmTime = *localtime(&tTime);

	_yy -= 1900;
	_mm -= 1;

	tmTime.tm_year = _yy;
	tmTime.tm_mon  = _mm;
	tmTime.tm_mday = _dd;

	tAfterTime = mktime(&tmTime);
	tmTime = *localtime(&tAfterTime);
	
	return tmTime.tm_wday;
}

unsigned long cSRVUTIL::GetHashCode(BYTE* _lpString)
{
	int	iResult=0,iXOR=0,iCH=0,i=0;

	for (;_lpString[i];i++)
	{
		iCH		=	_lpString[i];

		if (iCH >= 'a' && iCH <= 'z')
			iCH	-=	('a'-'A');

		iResult	=	( ( iResult << 8 ) | iCH ) % 16777213UL; // 16777213UL
		iXOR		^=	iCH;
	}

	return iResult | (iXOR<<24);
}


#ifdef	__USE_ZLIB
WORD
cSRVUTIL::MakeCompressPacket(CM_COMPRESS *_lpCompPacket,void *_lpPacket,int _iPacketSize)
{
	WORD	wCompSize	=	Compress((unsigned char *)_lpPacket,(unsigned char *)_lpCompPacket->data,_iPacketSize);

	if	(wCompSize)
	{
		_lpCompPacket->wSourceSize	=	_iPacketSize;
		_lpCompPacket->set(wCompSize + sizeof(WORD)*3,dCM_COMPRESS);
	}

	return	wCompSize;	
}

WORD cSRVUTIL::Compress(unsigned char * _sourcedata, unsigned char * _targetdata, WORD _datasize)
{
	WORD wResult = 0;
	z_stream	z;
	int flush, status , MAXINBUF;
	MAXINBUF = _datasize*2;

	z.zalloc = Z_NULL;
	z.zfree = Z_NULL;
	z.opaque = Z_NULL;
	if (deflateInit(&z, Z_DEFAULT_COMPRESSION) != Z_OK) 
	{
//		printf("deflateInit: %s\n", (z.msg) ? z.msg : "???");
		return 0; 
	}

	z.avail_in = 0;
	z.next_out = (unsigned char *)_targetdata;
	z.avail_out = _datasize;

	flush = Z_NO_FLUSH;
	while (1)
	{
		if (z.avail_in == 0)
		{
			z.next_in = (unsigned char * )_sourcedata;
			z.avail_in = _datasize;

    		if (z.avail_in < (UINT)MAXINBUF) flush = Z_FINISH;
		}
		status = deflate(&z, flush);
		if (status == Z_STREAM_END) break;
		if (status != Z_OK)
		{
//			printf("deflate: %s\n", (z.msg) ? z.msg : "???");
			return 0;
		}
		if (z.avail_out == 0)
		{
			z.next_out = (unsigned char *)_targetdata;
			z.avail_out = _datasize;
		}
	}
	wResult = _datasize - z.avail_out;
	if(_datasize - wResult>0)
	{
//		printf("compress success.. befor %d  ,  after %d \n",_datasize , wResult);
	}else
	{
//		printf("compress fail..befor %d  ,  after %d \n",_datasize , wResult);
	}
	if (deflateEnd(&z) != Z_OK)
	{
//		printf("deflateEnd: %s\n", (z.msg) ? z.msg : "???");
		return 0;
	}
	return wResult;
}

WORD cSRVUTIL::Uncompress(unsigned char * _sourcedata, unsigned char * _targetdata, WORD _datasize, WORD _sourcesize)
{
	WORD wResult = 0;
	z_stream	z;
	int status, MAXINBUF;
	MAXINBUF = _sourcesize;

	z.zalloc = Z_NULL;
	z.zfree = Z_NULL;
	z.opaque = Z_NULL;

	z.next_in = Z_NULL;
	z.avail_in = 0;
	
	if (inflateInit(&z) != Z_OK) {
//		printf("inflateInit: %s\n", (z.msg) ? z.msg : "???");
		return wResult;
	}

	z.next_out = (unsigned char *)_targetdata;
	z.avail_out = _sourcesize;
	status = Z_OK;

	while (status != Z_STREAM_END) 
	{
		if (z.avail_in == 0) {  
			z.next_in = (unsigned char *)_sourcedata;  
			z.avail_in = _datasize; 
		}
		status = inflate(&z, Z_NO_FLUSH); 
		if (status == Z_STREAM_END) break; 
		if (status != Z_OK) {   
//			printf("inflate: %s\n", (z.msg) ? z.msg : "???");
			return wResult;
		}
		if (z.avail_out == 0) 
		{ 
			z.next_out = (unsigned char *)_targetdata; 
			z.avail_out = _sourcesize; 
		}
	}
	wResult = _sourcesize - z.avail_out;
	if (inflateEnd(&z) != Z_OK) {
//		printf("inflateEnd: %s\n", (z.msg) ? z.msg : "???");
		return wResult;
	}
	return wResult;
}

#endif	//	__USE_ZLIB


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃class	cUniCode
char cSRVUTIL::m_pStrChar[1024];		//	간단한 텍스트 변역용
wchar_t cSRVUTIL::m_pStrWChar[1024];	//	간단한 텍스트 변역용

const char * cSRVUTIL::UnicodeToAscii(const wchar_t * _lpStr1,int opt)
{
	if(!_lpStr1)
		return 0;
	
	memset(m_pStrChar,0,1024);

	size_t size = wcslen(_lpStr1);
	if( !size )	
		return 0;
	
	size_t asize = (size_t)WideCharToMultiByte( opt, 0, _lpStr1, (int)size, NULL, 0, NULL, NULL );
	WideCharToMultiByte(opt,0,_lpStr1,(int)size+1,m_pStrChar,(int)asize,NULL,NULL);
//	size_t ret = wcstombs( m_pStrChar, _lpStr1, size ); 한글 짤린다. -_-;;
	return m_pStrChar;
}

const wchar_t * cSRVUTIL::AsciiToUnicode(const char * _lpStr1,int opt)
{
	if(!_lpStr1)
		return 0;

	memset(m_pStrWChar,0,2048);

	size_t size = strlen(_lpStr1);
	if( ! size )
		return 0;

	size_t wsize = (size_t)MultiByteToWideChar(opt, 0, _lpStr1, (int)size, NULL, 0);
	wsize = MultiByteToWideChar(opt, 0, _lpStr1, (int)size, m_pStrWChar, (int)wsize);
//	mbstowcs( m_pStrWChar, _lpStr1, size );

	return m_pStrWChar;
}

int	cSRVUTIL::UnicodeToAscii(const wchar_t * _lpStr1,char * _lpStr2, int _size,int opt)
{
	if(!_lpStr1 || !_lpStr2)
		return 0;

	size_t size = wcslen(_lpStr1);

	if( !size )
		return 0;
	++size;	//	NULL이 들어갈 위치
	if(!_size)
		_size = (int)size;
	memset( _lpStr2, 0, size+ 1);

	size_t sizeConverted = wcstombs(_lpStr2, _lpStr1, size );

	return (int)size;
}

int	cSRVUTIL::AsciiToUnicode(char * _lpStr1,wchar_t * _lpStr2, int _size,int opt)
{
	if(!_lpStr1 || !_lpStr2)
		return 0;
	size_t size = strlen(_lpStr1);
	if( ! size )
		return 0;

	memset( _lpStr2, 0, (size+1)*sizeof(wchar_t));

	size_t wsize = (size_t)MultiByteToWideChar(CP_ACP, 0, _lpStr1, (int)size, NULL, 0);
	wsize = MultiByteToWideChar(opt, 0, _lpStr1, (int)size, _lpStr2, (int)wsize);

	return (int)wsize;
}
//┃class	cUniCode
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


//////////////////////////////////////////////////////////////////////////
//


#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))


std::vector<std::string>
cSRVUTIL::getIPFilterList(std::vector<std::string>& _filteringList)
{
	char name[32];
	char addr[20];
	gethostname(name,sizeof(name));
	struct hostent* he = gethostbyname(name);
	struct in_addr iaddr;
	memset(&iaddr,0,sizeof(iaddr));
	
	std::vector<std::string>	retList;
	
	for(int i =0; he->h_addr_list[i] !=0; i++)
	{
		if(	he->h_addrtype!=AF_INET)	continue;
		memcpy(&iaddr,he->h_addr_list[i],sizeof(iaddr));
		strcpy(addr,inet_ntoa(iaddr));
		
		
		for(int j=0;j<_filteringList.size();j++)
		{
			if(strncmp(addr,_filteringList[j].c_str(),_filteringList[j].size())==0)
			{
				break;
			}
		}
		if(j >= _filteringList.size())
			retList.push_back(addr);
	}
	
	return retList;
}

std::string 
cSRVUTIL::getMyIp()
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
			{
				ip = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
				printf(" %s \n", ip.c_str());
			}
		}
		WSACleanup();
	}
	return ip;
}

std::string 
cSRVUTIL::getMacAddressByAdapter(std::string _ipaddress)
{
	/* Some general variables */
// 	ULONG ulOutBufLen;
// 	DWORD dwRetVal;
// 	int i;
	std::string retMacAddress;
// 	
// 	/* variables used for GetAdapterInfo */
// 	IP_ADAPTER_INFO *pAdapterInfo;
// 	IP_ADAPTER_INFO *pAdapter;
// 	
// 	pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(sizeof (IP_ADAPTER_INFO));
// 	if (pAdapterInfo == NULL) {
// 		printf("Error allocating memory needed to call GetAdapterInfo\n");
// 		return retMacAddress;
// 	}
// 	ulOutBufLen = sizeof (IP_ADAPTER_INFO);
// 	
// 	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
// 		FREE(pAdapterInfo);
// 		pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(ulOutBufLen);
// 		if (pAdapterInfo == NULL) {
// 			printf("Error allocating memory needed to call GetAdapterInfo\n");
// 			return retMacAddress;
// 		}
// 	}
// 	
// 	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) != NO_ERROR) {
// 		printf("GetAdaptersInfo failed with error %d\n", dwRetVal);
// 		if (pAdapterInfo)
// 			FREE(pAdapterInfo);
// 		return retMacAddress;
// 	}
// 	
// 	pAdapter = pAdapterInfo;
// 	std::string incorrectIP = "0.0.0.0";	//	가상 adapter
// 	while (pAdapter) 
// 	{
// 		if(incorrectIP.compare(pAdapter->IpAddressList.IpAddress.String) !=0)
// 		{
// 			if(pAdapter->Address[1]==0 && pAdapter->Address[2]==0)	//	mac address가 빈 경우. netbios에서 발생했음. 혹시 몰라 삽입해 둠.
// 			{
// 				pAdapter = pAdapter->Next;
// 				continue;
// 			}
// 			
// 			if(_ipaddress.length() > 0)
// 			{
// 				if(_ipaddress.compare(pAdapter->IpAddressList.IpAddress.String)!= 0 )
// 				{
// 					pAdapter = pAdapter->Next;
// 					continue;
// 				}
// 			}
// 			
// 			char temp[4];
// 			for (i = 0; i < (int) pAdapter->AddressLength; i++) 
// 			{
// 				if (i == (pAdapter->AddressLength - 1))
// 					sprintf(temp,"%.2X", (int) pAdapter->Address[i]);
// 				else
// 					sprintf(temp,"%.2X-", (int) pAdapter->Address[i]);
// 				retMacAddress.append(temp);
// 			}
// 			return retMacAddress;
// 		}
// 		pAdapter = pAdapter->Next;
// 	}
	
	return retMacAddress;
}

//
//////////////////////////////////////////////////////////////////////////