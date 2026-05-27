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

#ifndef	_lnk_file_h
#define	_lnk_file_h

#include	<stdio.h>
#include	"lnk_define.h"

class	cFile
{
public:
	FILE				*m_pFile;
	u4					m_u4FrontPos,m_u4EndPos,m_u4Length;
	bool				m_bIsMergeFile;
	bool				m_bIsFFlush;

						cFile();
	virtual				~cFile();


//	파일을 오픈한다. _lpstrMode에 관해서는 fopen 옵션을 참조. 아래는 자주 쓰이는 옵션
//	wb  바이너리 쓰기(기존 내용이 초기화됨)
//	wt  텍스트 쓰기(기존 내용이 초기화됨)
//	rb+ 바이너리 파일 갱신(기존 내용이 초기화 되지 안음)
//	rt+ 텍스트 파일 갱신(기존 내용이 초기화 되지 안음)
//	rb  바이너리 읽기
//	rt  텍스트 읽기
	bool				open(char *_lpstrFn,char *_lpstrMode,bool _bIsFFlush=false);	//	파일 오픈

	bool				close();	//	닫기
	bool				seek(s4 _s4Pos,s4 _s4SeekTerm=SEEK_CUR);	//	찾기
	bool				skip(s4 _s4SkipPoint);//	스킵
	bool				rewind();			//	화일 처음으로
	bool				end();				//	화일 끝으로
	u4					getCurrentLocate();	//	파일 현재 위치
	s4					read(void *_lpBuffer,u4 _u4Size,u4 _u4ItemSize=1);		//	읽기
	s4					write(void *_lpBuffer,u4 _u4Size,u4 _u4ItemSize=1);	//	쓰기

	bool	_cdecl		writeString(char *_lpstrText,...);		//	문자열 쓰기 1kb를 넘어가면 안된다.
	bool	_cdecl		writeStringNP(char *_lpstrText);		//	문자열 쓰기
	bool				set(cFile *_lpFile,u4 _u4Size);		//	특정 파일을 세팅한다. ... 왜 만들었는지 까먹음.. - -;;

//	정적 멤버
	static	u4			GetSize(char *_lpstrFn);	//	파일 사이즈 얻기
	static	u4			GetSize(FILE *_lpFP);

#ifdef	WIN32
	//	윈도우 공용 컨트롤
	static	bool		LoadBox(char *fname,char *filter,char *defaultExt,char *folder=NULL,char *title="Load data",u4 _u4Style=0);
	static	bool		SaveBox(char *fname,char *filter,char *defaultExt,char *folder=NULL,char *title="Save data",u4 _u4Style=0);
#endif
};

#endif