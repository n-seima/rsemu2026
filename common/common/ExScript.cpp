#include <stdio.h>
#include "ExScript.h"

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃class	CWWTime
static int monthend[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

int CWWTime::GetDate()
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

int	CWWTime::GetNextDate()
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

int	CWWTime::GetNextDate(int _istandardDate)
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

int	CWWTime::GetPrevDate(int _istandardDate)
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

int	CWWTime::GetPrevDate()
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

int	CWWTime::GetMin()
{
	int m_iResult=0;
	time_t tTime;
	struct tm *tmTime;
	tTime = time(NULL);
	tmTime = localtime(&tTime);
	m_iResult = (tmTime->tm_min);

	return m_iResult;
}

int	CWWTime::GetHour()
{
	int m_iResult=0;
	time_t tTime;
	struct tm *tmTime;
	tTime = time(NULL);
	tmTime = localtime(&tTime);
	m_iResult = (tmTime->tm_hour);

	return m_iResult;
}
int	CWWTime::GetYear()
{
	int m_iResult=0;
	time_t tTime;
	struct tm *tmTime;
	tTime = time(NULL);
	tmTime = localtime(&tTime);
	m_iResult = (tmTime->tm_year);

	return m_iResult;
}

int	CWWTime::GetMonth()
{
	int m_iResult=0;
	time_t tTime;
	struct tm *tmTime;
	tTime = time(NULL);
	tmTime = localtime(&tTime);
	m_iResult = (tmTime->tm_mon);

	return m_iResult;
}

int	CWWTime::GetDay()
{
	int m_iResult=0;
	time_t tTime;
	struct tm *tmTime;
	tTime = time(NULL);
	tmTime = localtime(&tTime);
	m_iResult = (tmTime->tm_mday);

	return m_iResult;
}

int	CWWTime::GetWeekDay()		//	 0 ~ 6 : 0 -> 일요일
{
	int m_iResult=0;
	time_t tTime;
	struct tm *tmTime;
	tTime = time(NULL);
	tmTime = localtime(&tTime);
	m_iResult = (tmTime->tm_wday);

	return m_iResult;
}

int CWWTime::GetSec()
{
	int m_iResult=0;
	time_t tTime;
	struct tm *tmTime;
	tTime = time(NULL);
	tmTime = localtime(&tTime);
	m_iResult = (tmTime->tm_sec);

	return m_iResult;
}

int CWWTime::GetTimes()
{
	int m_iResult=0;
	m_iResult = GetHour()*10000+ GetMin()*100+ GetSec();

	return m_iResult;
}

/*void CWWTime::GetMSSQLDBDateNTime(char * _date)//	yyyy-mm-dd hh:mm:ss
{
	int m_iResult=0;
	time_t tTime;
	struct tm *tmTime;
	tTime = time(NULL);
	tmTime = localtime(&tTime);

	sprintf(_date,"%d-%02d-%02d %02d:%02d:%02d",tmTime->tm_year+1900,tmTime->tm_mon,tmTime->tm_mday,tmTime->tm_hour,tmTime->tm_min,tmTime->tm_sec);
}
*/

int	CWWTime::CalcMin(int _a,int _b,int _isPlus, int * _Hour, int * _Week)
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

int	CWWTime::CalcDayOnToday(int _yy,int _mm, int _dd)	//	오늘과 며칠 차이일까?
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

int	CWWTime::CalcHour(int _a,int _b,int _isPlus, int * _Week)
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

int	CWWTime::GetWeekDayOfaday(int _yy, int _mm, int _dd)	//	 0 ~ 6 : 0 -> 일요일
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
//┃class	CWWTime
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃class	CWWScript
CWWScript::CWWScript()
{
	m_strFileName[0] = NULL;
	m_cScp = NULL;
	m_iCount = 0;
}

CWWScript::~CWWScript()
{
	if	(m_cScp)
	{	
		delete [] m_cScp;
		m_cScp = NULL;
	}
}

/////////////////////////////////////////////////////
//	these are public functions.
int	CWWScript::Read(char * _strFilename,int _iMaxCount)
{	// 정상적인 파일이 아니거나 카운트 설정이 잘못되면 무효 ~ 
	if(_strFilename[0] == NULL, strcmp(_strFilename,"")==0 || _iMaxCount<=0)	
		return 0;
	if(m_cScp)
	{	delete [] m_cScp; m_cScp = NULL;
	}
	m_cScp = new cScpData[_iMaxCount];
	if(!m_cScp)
		return 0;
	m_iCount = _iMaxCount;

	strcpy(m_strFileName,_strFilename);

	FILE * fp= NULL;
	if( (fp = fopen( m_strFileName, "r" )) == NULL )
	{
		delete [] m_cScp; m_cScp = NULL;
		return 0;
	}
	m_iRunning = m_iScpCount = m_iTotCount = m_isChildData = m_isHaveChild = 0;
	m_strParentName[0] = 0;

	int iRet = 0;
	while(1)
	{
		iRet = ReadLine(fp);
		if(iRet<1)
			break;
	}
	fclose(fp);

	if(iRet== -1)
		return 0;	//	에러가 있어서 정상적으로 읽어들이지 못했다.

	return 1;
}

int	CWWScript::Save(char * _strFilename)
{
	FILE * fp = NULL;
	char	strNewFilename[eMAXFILENAMELENGTH];
	if(_strFilename == NULL)	//	기존의 파일을 백업하고, 기존 이름으로 기록한다.
		strcpy(strNewFilename,m_strFileName);
	else
		strcpy(strNewFilename,_strFilename);
	
	FILE * tmpFp=  NULL;
	if( (tmpFp = fopen(strNewFilename,"r")) )//	같은 이름의 파일이 이미 있다면
	{
		fclose(tmpFp);	tmpFp = NULL;
		//	이름을 바꾼다.
		char	strTmpFilename[eMAXFILENAMELENGTH];
		sprintf(strTmpFilename,"%s_backup_%d_%d",strNewFilename,CWWTime::GetDate(),CWWTime::GetTimes() );
		if(rename(strNewFilename,strTmpFilename))
			return 0;	//	파일명 변환에 실패 했습니다.
	}
	//	새로운 파일을 생성해서 처리합니다. 
	fp = fopen(strNewFilename,"w+");
	if(!fp)
		return 0;	// 파일 생성에 실패 했습니다.

	{	//	데이터를 출력한다.
		int isChild = 0;
		for(int i = 0;i<m_iCount;i++)
		{
			if(m_cScp[i].m_dwHashCode==0)
				continue;
			if(m_cScp[i].m_strChildName[0]==NULL)
			{
				if(isChild)
				{
					fprintf(fp,"}\n\n");
					isChild = 0;
				}
				fprintf(fp,"%s = ",m_cScp[i].m_strName);
			}else
			{
				if(!isChild)
				{
					isChild = 1;
					fprintf(fp,"%s;\n",m_cScp[i].m_strName);
					fprintf(fp,"{\n");
				}
				fprintf(fp,"	%s = ",m_cScp[i].m_strChildName);
			}
			for(int j=1;j<dWWS_MAXDATACOUNT;j++)
			{
				if(m_cScp[i].m_bData[j][0] == NULL)	
					break;
				if(j>1)	fprintf(fp," | ");
				if(strcmp((char *)m_cScp[i].m_bData[j],dWWS_TSTEXT)==0)
					fprintf(fp,"\"%s\"",m_cScp[i].m_strBigData);
				else
					fprintf(fp," %s ",m_cScp[i].m_bData[j] );
			}
			fprintf(fp,";\n");
		}
	}
	fclose(fp);
	return 1;
}

int	CWWScript::GetData(int _iType,unWW_DataType * _ret,char * _strName,int count,char * _strChildName)
{
	if(count>=dWWS_MAXDATACOUNT)	return 0;

	char strCheckName[dWWS_MAXNAMESIZE*2];
	if(_strChildName)
		sprintf(strCheckName,"%s-%s",_strName,_strChildName);
	else
		strcpy(strCheckName,_strName);
	
	DWORD dwHashCode = cSRVUTIL::GetHashCode((BYTE *)strCheckName);
	for(int i = m_iCount;i;)
	{	--i;
		if(m_cScp[i].m_dwHashCode != dwHashCode)	continue;
		
		if(m_cScp[i].m_bData[count][0] == NULL)		return 0;
		switch(_iType)
		{
		case eWWS_DT_CHAR:
			_ret->dtChar = (char )m_cScp[i].m_bData[count][0];				break;
		case eWWS_DT_UCHAR:
			_ret->dtUChar = (unsigned char )m_cScp[i].m_bData[count][0];	break;
		case eWWS_DT_SHORT:
			_ret->dtShort = (short )(atoi((char *)m_cScp[i].m_bData[count]));break;
		case eWWS_DT_USHORT:
			_ret->dtUShort = (unsigned short )(atoi((char *)m_cScp[i].m_bData[count]));break;
		case eWWS_DT_INT:
			_ret->dtInt = (atoi((char *)m_cScp[i].m_bData[count]));break;
		case eWWS_DT_UINT:
			_ret->dtUInt = (unsigned int )(atoi((char *)m_cScp[i].m_bData[count]));break;
		case eWWS_DT_LONG:
			_ret->dtLong = atol((char *)m_cScp[i].m_bData[count]);		break;
		case eWWS_DT_ULONG:
			_ret->dtULong = (unsigned long )atol((char *)m_cScp[i].m_bData[count]);		break;
		case eWWS_DT_STRING:
			{
				if(strcmp((char *)m_cScp[i].m_bData[count],dWWS_TSTEXT)==0)
					_ret->dtString = (char *)m_cScp[i].m_strBigData;
				else
					_ret->dtString = (char *)m_cScp[i].m_bData[count];
				break;
			}
		}
		return 1;
	}
	return 0;
}

int	CWWScript::SetData(char * _value, char * _strName,int count,char * _strChildName)
{
	if(count>=dWWS_MAXDATACOUNT)	return 0;

	char strCheckName[dWWS_MAXNAMESIZE*2];
	if(_strChildName)
		sprintf(strCheckName,"%s-%s",_strName,_strChildName);
	else
		strcpy(strCheckName,_strName);
	
	DWORD dwHashCode = cSRVUTIL::GetHashCode((BYTE *)strCheckName);
	for(int i = m_iCount;i;)
	{	--i;
		if(m_cScp[i].m_dwHashCode != dwHashCode)	continue;
		
		if(strcmp((char *)m_cScp[i].m_bData[count],dWWS_TSTEXT)==0)
			strcpy((char *)m_cScp[i].m_strBigData,_value);
		else
			strcpy((char *)m_cScp[i].m_bData[count],_value);
		break;
	}
	return 1;
}

/////////////////////////////////////////////////////
//	these are private functions.

int CWWScript::ReadLine(FILE *f)
{
	int		tmp=0;
	int		count=0;

	char	_readline[eMAXLENGTHOFALINE];
	memset(_readline,0,eMAXLENGTHOFALINE);

	tmp = fgetc(f);
	while(tmp!=EOF && tmp!='\n')
	{
		_readline[count++] = (char)tmp;
		tmp = fgetc(f);
	}
	_readline[count]=0;
	if(tmp==EOF)
	{
		if(count>0)
		{
			int iRet = ParseLine(_readline, count);
			if (iRet == -1)	//에러가 있다면
				return iRet;
		}
		return 0;
	}
	if( count==0)	return 1;

	return ParseLine(_readline, count);
}

int CWWScript::ParseLine(char *_readline,int size)
{
	int		parse		=	0;
	int		cnt			=	0;
	int		count		=	0;
	int		isTXT		=	0;
	int		isTexted	=	0;	//""로 감싸진 데이터가 해당 순번에 있을 경우 
	int		isData		=	0;
	char	tmp[eMAXLENGTHOFALINE];			//	임시 저장 공간.
	memset(tmp,0,eMAXLENGTHOFALINE);

	for(count=0;count<size; count++)
	{
		//	지나쳐야 할 것들 
		if(!isTXT && _readline[count]==' ' || _readline[count]==9 || _readline[count]==13)
			continue;

		//	특수 기호들
		if(!isTXT && _readline[count]=='/')
		{
			if( _readline[count-1]=='/')
				return 1;
		}
		//	텍스트의 끝과 시작을 표기
		if(_readline[count]=='\"')
		{
			if(isTXT)	isTXT = 0;
			else
			{
				isTXT = 1;
				isTexted  = 1;
			}

			continue;
		}
		if(!isTXT && _readline[count]=='{')
		{
			if(!m_isHaveChild)	//	규칙에 어긋났다.
				return -1;
			if(m_isChildData)
				return -1;

			m_isChildData = 1;

			continue;
		}

		if(!isTXT && _readline[count]=='}')
		{
			if(!m_isChildData)
				return -1;
			m_isHaveChild = m_isChildData = 0;

			continue;
		}

		if(!isTXT && (_readline[count]==dWWS_DATAENDCHAR || _readline[count]==dWWS_PARSECHAR || _readline[count]==dWWS_DATAPARSECHAR) )
		{
			if(tmp[0]!=0)
			{
				memset(m_cScp[m_iScpCount].m_bData[parse],0,dWWS_MAXDATASIZE);
				if(parse == 0)	//	이녀석은 이름이다.
				{
					if(!m_isHaveChild && !m_isChildData)	//	부모이거나 독립적인 데이터
					{
						if(strstr(tmp,dWWS_HAVECHILD))//	자식을 가진 경우 , 데이터는 없으니 세팅만 하고 넘긴다.
						{
							m_isHaveChild = 1;
							memcpy(m_strParentName,tmp,cnt);
							m_strParentName[cnt] = 0;
							return 1;
						}
						else//	독립적인 데이터
						{	
							memcpy(m_cScp[m_iScpCount].m_bData[parse],tmp,cnt);
							m_cScp[m_iScpCount].m_bData[parse][cnt] = 0;
							strcpy(m_cScp[m_iScpCount].m_strName,(char *)m_cScp[m_iScpCount].m_bData[parse]);
							m_cScp[m_iScpCount].m_dwHashCode = cSRVUTIL::GetHashCode((BYTE *)m_cScp[m_iScpCount].m_strName);
						}
					}else	//	자식으로 처리해야 한다.
					{
						//	잘못된 문법이라면 
						if(!m_isHaveChild || !m_isChildData || m_strParentName[0]==NULL)
							return -1;
						//	정상적이라면..
						memcpy(m_cScp[m_iScpCount].m_bData[parse],tmp,cnt);
						m_cScp[m_iScpCount].m_bData[parse][cnt] = 0;
						strcpy(m_cScp[m_iScpCount].m_strName,m_strParentName);
						strcpy(m_cScp[m_iScpCount].m_strChildName,(char *)m_cScp[m_iScpCount].m_bData[parse]);
						char strTmpName[dWWS_MAXNAMESIZE*2];
						sprintf(strTmpName,"%s-%s",m_cScp[m_iScpCount].m_strName,m_cScp[m_iScpCount].m_strChildName);
						m_cScp[m_iScpCount].m_dwHashCode = cSRVUTIL::GetHashCode((BYTE *)strTmpName);
					}
				}else
				{
					if(isTexted)
					{
						isTexted = 0;
						strcpy((char *)m_cScp[m_iScpCount].m_bData[parse],dWWS_TSTEXT);
						int length = strlen(tmp);
						if(m_cScp[m_iScpCount].m_strBigData)
							return -1;
						m_cScp[m_iScpCount].m_strBigData = new char[length+1];
						memcpy(m_cScp[m_iScpCount].m_strBigData,tmp,cnt);
						m_cScp[m_iScpCount].m_strBigData[cnt] = 0;
					}else
					{
						memcpy(m_cScp[m_iScpCount].m_bData[parse],tmp,cnt);
						m_cScp[m_iScpCount].m_bData[parse][cnt] = 0;
					}
				}
				++parse;
				cnt			=	0;
				tmp[0]		=	0;
				if(_readline[count]==dWWS_DATAENDCHAR)
				{
					++m_iTotCount;
					++m_iScpCount;
					parse=0;
					return 1;
				}
				continue;
			}else
				return 1;
		}
		if(_readline[count]==EOF)
			return 0;
		if(_readline[count]=='\\')
		{
			tmp[cnt++] = '\r';
			tmp[cnt++] = '\n';
		}
		else
			tmp[cnt++] = _readline[count];
		isData = 1;
	}
	if(isData)	return 0;
	else		return 1;
}
//┃class	CLogSystem
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━