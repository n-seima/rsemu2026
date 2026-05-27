#include "cSVNentriesParser.h"
#include <io.h>
#include <windows.h>

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+ class cSVNentriesParser
//+ SVN의entries 파일을 파서하기 위한 클래스  
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

//--------------------------------------------------------------------------------------------------//
//			생성자 / 소멸자
//--------------------------------------------------------------------------------------------------//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+ cSVNentriesParser(void)
//+ 생성자  
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
cSVNentriesParser::cSVNentriesParser(void)
{
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+ ~cSVNentriesParser(void)
//+ 소멸자
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
cSVNentriesParser::~cSVNentriesParser(void)
{
	clear();
}

//--------------------------------------------------------------------------------------------------//
//			public Funtion()
//--------------------------------------------------------------------------------------------------//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+ 함수이름 : void	clear()
//+ 설명	 : 클래스를 리셋시킨다.(m_infoMap을 비운다.)
//+ 리턴값	 : 
//+ 발생에러 : 
//+ 매개변수 :  
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void	
cSVNentriesParser::clear()
{
	//할당된 stInfo*을 제거한다. 
	for(infoMap::iterator iter = m_infoMap.begin(); iter != m_infoMap.end(); iter++)
	{
		if(iter->second != NULL) 
			delete iter->second;
	}
	m_infoMap.clear();//map 초기화 
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+ 함수이름 : bool	loadFile(TCHAR* _strFileName)
//+ 설명	 : entries 파일을 로드 한다.(m_infoMap에 파일정보 추출 하여 담는다.)
//+ 리턴값	 : 파일 로드 성공 유무
//+ 발생에러 : 
//+ 매개변수 : TCHAR* _strFileName	로드할 entries 파일 이름 
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
bool	
cSVNentriesParser::loadFile(TCHAR* _strFileName)
{
	//m_infoMap를 초기화 합니다. 
	if(!m_infoMap.empty())	clear();
	
	//entries 파일 열기
	FILE* file;
	file = _tfopen(_strFileName, _T("rb"));
	if(! file )	return	false; //파일 열기 실패시 false

	int		u4Size = _filelength(_fileno(file))+1;	//파일사이즈 설정
	char*	strBuf2 = new char[u4Size];				//버퍼 할당
	ZeroMemory(strBuf2, sizeof(char)*(u4Size) );	//버퍼 초기화 
	fread(strBuf2, 1, u4Size, file);				//파일데이터 읽기	
	fclose(file);

	TCHAR* strBuf = new TCHAR[u4Size];
	MultiByteToWideChar(CP_UTF8, 0, strBuf2, -1, strBuf, u4Size);
/*
	FILE* file;
	file =	_tfopen(_strFileName, _T("rt, ccs=UTF-8")); //UTF-8형식 으로 열기 
	if(! file )	return	false; //파일 열기 실패시 false

	int		u4Size = _filelength(_fileno(file))*2+1;//파일사이즈 설정
	TCHAR*	strBuf = new TCHAR[u4Size];				//버퍼 할당
	ZeroMemory(strBuf, sizeof(TCHAR)*(u4Size) );	//버퍼 초기화 
	fread(strBuf, 1, u4Size, file);					//파일데이터 읽기	
	fclose(file);
*/
	_tcslwr(strBuf);//전부 소문자로 변환 

	//_tprintf(_T("%s \n\n"), strBuf);

	TCHAR*	pstrTmep	= NULL;	//줄단위의 파일 데이터 읽기
	bool	bfilename	= false;//파일이름으로 읽을것인지 아닌지에 대한 플래그 
	bool	bformat		= false;//폴더인지 아닌지로 읽을것인지에 대한 플래그 
	stInfo*	l_stInfo	= NULL;	//해당 파일의 정보를 가르킬 stInfo* 포인터

	pstrTmep = _tcstok(strBuf, _T("\r\n")); 
	
	//정보를 담을 stInfo 할당
	l_stInfo = new stInfo();
	ZeroMemory( l_stInfo, sizeof(stInfo) );

	//파일정보 추출
	while(pstrTmep != NULL)
	{	
		pstrTmep = _tcstok(NULL, _T("\r\n"));
		
		//더이상 추출 안될때..
		if(!pstrTmep) 
		{
			//작업해야할 stInfo를 제거한다. (어차피 파일 데이터를 가질수 없으므로)
			if(l_stInfo != NULL)
			{
				delete l_stInfo;
				l_stInfo = NULL;
			}
			break;
		}
		//추출 블럭이 끝났을때..(파일 단위를 로 구분한다.)
		else if( _tcscmp( pstrTmep, _T("")) == 0 )
		{
			//파일과 폴더 중 하나라도 불러오지 못했을 경우
			if(!bfilename || !bformat)
			{				
				ZeroMemory( l_stInfo, sizeof(stInfo) );//재활용			
			}
			//맵에 추가
			else 
			{					
				std::pair<String, stInfo*> p(l_stInfo->strFileName, l_stInfo);
				m_infoMap.insert(p);

				//새로 작업할 stInfo 할당
				l_stInfo = NULL;
				l_stInfo = new stInfo();
				ZeroMemory( l_stInfo, sizeof(stInfo) );
			}

			bfilename = bformat = false;//플래그 다시 세팅
		}
		//파일 이름 입력
		else if(!bfilename)
		{
			if(_tcscmp( pstrTmep, _T("")) != 0)	
			{				
				bfilename = true;//파일 이름 읽었음
				l_stInfo->strFileName = pstrTmep;				
			}
		}
		//폴더 인지 파일인지 구분
		else if(!bformat)
		{
			if(_tcscmp( pstrTmep, _T("file")) == 0)	
			{				
				bformat = true;//구분 했음
				l_stInfo->iFileFormat = cSVNentriesParser::eFormat_File;				
			}
			else if(_tcscmp( pstrTmep, _T("dir")) == 0)	
			{
				bformat = true;//구분 했음
				l_stInfo->iFileFormat = cSVNentriesParser::eFormat_Folder;
			}
		}
		//락 유무 검사
		else if( _tcsstr(pstrTmep, _T("opaquelocktoken:")) )
		{
			l_stInfo->bisLock = true;			
		}
	}//while(pstrTmep != NULL)	

	delete[] strBuf;

	return true;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+ 함수이름 : const	stInfo*	const getInfo(TCHAR* _strFileName);
//+ 설명	 : 해당파일의 stInfo*를 리턴
//+ 리턴값	 : 해당파일의 stInfo* (해당 파일이 존재 하지 않을 경우 NULL을 리턴한다. )
//+ 발생에러 : 
//+ 매개변수 : TCHAR* _strFileName	stInfo*를 리턴 받을 파일명  
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
const cSVNentriesParser::stInfo*	const
cSVNentriesParser::getInfo(TCHAR* _strFileName)
{
	TCHAR strTemp[1024];
	_tcscpy(strTemp, _strFileName);
	_tcslwr(strTemp);

	infoMap::iterator iter = m_infoMap.find(strTemp);//해당파일이 있는지 검색

	if(iter != m_infoMap.end())
	{
		return iter->second;//해당 정보 리턴 
	}

	//해당 파일이 없다면 NULL리턴 
	return NULL;
}

