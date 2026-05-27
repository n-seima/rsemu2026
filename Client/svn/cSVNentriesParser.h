#pragma once

#include <TCHAR.h>
#include <string>
#include <map>

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//+ class cSVNentriesParser
//+ SVN의entries 파일을 파서하기 위한 클래스  
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
class cSVNentriesParser
{
public:
	//해당 파일이 폴더 인지 파일인지 구분
	enum
	{
		eFormat_File,	//해당 파일이 파일 이다.
		eFormat_Folder,	//해당 파일이 폴더 이다.
	};

public:
	#ifdef	_UNICODE
		typedef std::wstring String;	//유니코드 일때 std::wstring 사용 
	#else 
		typedef std::string String;		//멀티바이트 일때 std::string 사용
	#endif

	/////////////////////////////////////////////////////////////////////
	//+ struct stInfo
	//+ 해당파일의 정보를 기록하기 위한 구조체
	struct stInfo
	{
		String	strFileName;	//파일이름
		int		iFileFormat;	//파일 포맷(파일? 폴더?)
		bool	bisLock;		//파일의 Lock 유무
	}typedef stInfo;
	
	//파일정보를 담을 맵 typedef
	typedef std::map<String, stInfo*>	infoMap;	
	
private:
	infoMap m_infoMap;	//파일정보를 담을 맵

public:
			cSVNentriesParser(void);	//생성자
			~cSVNentriesParser(void);	//소멸자

			void	clear();							//클래스를 초기화 한다.
			bool	loadFile(TCHAR* _strFileName);		//entries 파일을 로드 한다.(파일정보 추출)
	const	stInfo*	const getInfo(TCHAR* _strFileName);	//해당파일의 stInfo*를 리턴
};
