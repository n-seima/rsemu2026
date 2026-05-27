//
//	제작 일시	2006.03.15
//
//	목적		파일 제어
//
//	제작		이영찬
//

// 사용예
//	cFile	file;
//
//	file.open("aa.dat","rb");
//
//	file.write(data,sizeof(data));
//
//	file.close();

#pragma	once

#include "lnk_define.h"
#include <vector>


const	int	c_iPushFileLocateInfoCount	=	16;




class cFile
{
	UINT				m_auiPushLocateInfo[c_iPushFileLocateInfoCount];

public:
	FILE*				m_pFile;			// 현재 파일 포인터 / 현재 패킹 버퍼 포인터
	UINT				m_uiFrontPos;		// 처음 위치
	UINT				m_uiEndPos;			// 끝 위치
	UINT				m_uiLength;			// 파일 사이즈
	bool				m_bIsMergeFile;		//
	bool				m_bfIsRef;			//
	bool				m_bIsFFlush;		//


public:
						cFile();
						cFile(FILE *_lpFP);
	virtual				~cFile();



//	파일을 오픈한다. _lpstrMode에 관해서는 fopen 옵션을 참조. 아래는 자주 쓰이는 옵션
//	wb  바이너리 쓰기(기존 내용이 초기화됨)
//	wt  텍스트 쓰기(기존 내용이 초기화됨)
//	rb+ 바이너리 파일 갱신(기존 내용이 초기화 되지 안음)
//	rt+ 텍스트 파일 갱신(기존 내용이 초기화 되지 안음)
//	rb  바이너리 읽기
//	rt  텍스트 읽기



	// _bReadFileFromPacker를 TRUE로 설정하면 패킹 파일에서 파일로 직접 읽어온다. 단, dPACKINGLOAD가 define될 때만 작동된다...
	bool open( const TCHAR* _strFullFileName, const TCHAR* _lpstrMode, BOOL _bIsFFlush=false);	//	파일 오픈
	bool close();													//	닫기	
	int	read( void* _lpBuffer, UINT _uiSize, UINT _uiItemSize=1 );	//	읽기
	int write( void* _lpBuffer, UINT _uiSize, UINT _uiItemSize=1 );	//	쓰기
	int write( int _iLocate, void* _lpData, int _iSize, UINT _uiItemSize=1 );	// 쓰기
	bool seek( int _iPos, int _iSeekTerm=SEEK_CUR );	//	찾기
	bool skip( int _iSkipPoint );						//	스킵
	bool rewind();										//	화일 처음으로
	bool end();											//	화일 끝으로
	UINT getCurrentLocate();							//	파일 현재 위치
	bool set( FILE* _pFile );							//
	bool set( cFile *_lpFile, UINT _uiSize );			//	특정 파일을 세	팅한다. ... 왜 만들었는지 까먹음.. - -;;




	bool skipStringAndSize();							//	스트링 문자와 사이즈 스킵 하기
	bool	_cdecl		writeString(const TCHAR *_lpstrText,...);		//	문자열 쓰기 1kb를 넘어가면 안된다.
	bool	_cdecl		writeStringNP(const TCHAR *_lpstrText);			//	문자열 쓰기
	bool				writeStringAndSize(const TCHAR *_lpstrText);
	bool				readStringAndSize(tstring *_lpstrString);
	bool				writeVecInt(VecInt *_lpVecInt);
	bool				readVecInt(VecInt *_lpVecInt);

	void				pushCurrentLocate(int _iSlot);
	bool				popCurrentLocate(int _iSlot);
	bool				writeCurrentPos(int _iLocate=-1);






//	정적 멤버 : 패킹 지원 안함!!
	static	UINT		GetSize( const TCHAR* _lpstrFn );	//	파일 사이즈 얻기
	static	UINT		GetSize( FILE* _lpFP );

//	파일 저장 도중 죽어서 데이터가 깨지는 경우 대비 : 패킹 지원 안함!!
	static	TCHAR*		ReadyToSafeSave( const TCHAR* _lpOrgFile );	//	_lpOrgFile을 안전하게 저장하기 위해 사용
	static	bool		EndSafeSave( const TCHAR* _lpOrgFile );		//	마무리



#ifdef	WIN32
	//	윈도우 공용 컨트롤
	static	bool		LoadBox(TCHAR *fname,TCHAR *filter,TCHAR *defaultExt,TCHAR *folder=NULL,TCHAR *title=_T("Load data"),UINT _uiStyle=0);
	static	bool		SaveBox(TCHAR *fname,TCHAR *filter,TCHAR *defaultExt,TCHAR *folder=NULL,TCHAR *title=_T("Save data"),UINT _uiStyle=0);
#endif
};








extern	bool	IsFile(const TCHAR *fn);
extern	bool	IsFolder(const TCHAR *fn);
extern	int		GetFileList(std::vector<tstring> *_lpList,const TCHAR *_lpstrFullPathFilter);
extern	int		GetFileList(TCHAR *_lpstrRoot,TCHAR *_lpstrFilter,std::vector<tstring> *_lpList,BOOL _bIsIncludeSubFolder,int (*_lpfnOperateFilter)(BOOL,TCHAR*,void*)=NULL,void *_lpData=NULL);
extern	int		GetFolderList(TCHAR *_lpstrRoot,std::vector<tstring> *_lpList,int (*_lpfnOperateFilter)(BOOL,TCHAR*,void*)=NULL,void *_lpData=NULL);

extern	BOOL	CreateFolder(TCHAR *_lpstrFolder,BOOL _bIsChangeFolder);	//	폴더를 만든다. 
extern	BOOL	DeleteFolder(TCHAR *_lpstrDelDir,BOOL _bIsWantRemoveRoot);		//	화일을 포함한 폴더를 지운다.
