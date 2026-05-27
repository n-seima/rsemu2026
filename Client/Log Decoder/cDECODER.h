#ifndef	_classDECODER_H
#define	_classDECODER_H

#include "SFC.H"

#define dBEGIN					1	//	문자 정의문들.. '{'
#define dEND					2	//	'}'
#define dSEMICOLON				3	//	';'
#define dSPACE					4	//	공백
#define dCHAR					5	//	화면에 나타낼수 있는 문자와 기호들 
#define dNUMBER					6	//	숫자 
#define dGARBAGE				7	//	필요없는 문자열들..
#define	dSIGN					8	//	그이외의 기호들..

#define	dCODE_FINISH			0x7ffffff7		//	;
#define	dCODE_END				0x7ffffff8
#define	dCODE_OK				0x7ffffff9
#define	dCODE_CLOSE				0x7ffffffa
#define	dCODE_OPEN				0x7ffffffb
#define	dCODE_ERROR				0x7ffffffc
#define	dCODE_UNDEFINE			0x7fffffff
#define	dCODE_SYSTEM			0x7ffffffe
#define	dCODE_VALID_NUMBER		0x7fffffff
#define	dDECODER_MAX_STREAM		1024*10

#define	dMAX_WORD_LENGTH		256
#define	dMAX_STREAM_LENGTH		2048

class	cKEYWORD_ITEM
{
public:
	char	m_strKeyword[dMAX_WORD_LENGTH];
	DWORD	m_dwCode;
};

class cDECODER
{
		char			m_strWord[dMAX_WORD_LENGTH];		//	단어를 위한 버퍼
		char			m_strStream[dMAX_STREAM_LENGTH];	//	스트림(텍스트) 를 위한 버퍼
		int				m_iBP,m_iFileLength;				//	버퍼 포인터와 화일 사이즈
		BOOL			m_isActive;

		cKEYWORD_ITEM	*m_lpKeyWord;						//	업로드된 키워드들
		char			*m_pStream;							//	화일에서 읽어 들인 버퍼

		inline	BOOL	isChar(char ch);
		inline	BOOL	isCloseCode(char ch);
		inline	BOOL	isFinishCode(char ch);

		inline	BOOL	isBeginCode(char ch);
		inline	BOOL	isNotStreamCode(char ch);
		inline	BOOL	isSystemCode(char ch);
		inline	BOOL	isNumber(char ch);

		inline	BOOL	findNumber();
		inline	char	findCharExact();
		inline	char	findCharExact(char ch);
		inline	BOOL	findCharOver();
		inline	BOOL	findCharOver(char ch);
public:
						cDECODER();
						~cDECODER();

		BOOL			upload(char *fn,cKEYWORD_ITEM *keyword);	//	화일을 열고 키워드를 업로드 한다.
		BOOL			upload(cKEYWORD_ITEM *keyword);				//	키워드를 업로드 한다.
		void			reset();									//	초기화 시킨다.
		inline	BOOL	isActive(){return	m_isActive;}

		BOOL			findOpenCode();				//	'{'를 찾는다.
		BOOL			findCloseCode();			//	'}'를 찾는다.
		BOOL			findFinishCode();			//	';'를 찾는다.

		char			*getStream();				//	따옴표로 둘러쌓인 문장을 읽어 들인다.
		char			*getWord();					//	단어를 읽어 들인다.
		BOOL			findWord(char *cmpWord);	//	특정 단어를 찾는다.
		int				getNumber();				//	숫자를 읽어 들인다.

		DWORD			analCode(char *cmpKeyword);	//	특정 키워드를 판독한다.
		inline	BOOL	isArriveEnd(){if	(m_iBP	>=	m_iFileLength)	return	TRUE;return	FALSE;}

		static	DWORD	AnalCode(cKEYWORD_ITEM *keyword,char *cmpKeyword);

};

#endif