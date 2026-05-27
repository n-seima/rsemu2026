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

#define	dMAX_WORD_LENGTH		32
#define	dMAX_STREAM_LENGTH		2048

class	CDecoderKeyword
{
public:
	char	m_strKeyword[dMAX_WORD_LENGTH];
	DWORD	m_dwCode;
};

class CDecoder
{
	char			m_strWORD[dMAX_WORD_LENGTH];							//	단어
	char			m_strStream[dMAX_STREAM_LENGTH];
	int				m_iBP,m_iFileLength;

	CDecoderKeyword	*m_lpKeyWord;
	char			*m_pSTREAM;

public:
	BOOL			m_isDone;

					CDecoder();
					~CDecoder();

	BOOL			Upload(char *fn,CDecoderKeyword *keyword);
	BOOL			UploadBuffer(char *stream,CDecoderKeyword *keyword);
	BOOL			Upload(CDecoderKeyword *keyword);
	void			Reset();
	void			rewind()	{m_iBP	=	0;}	//	처음으로 돌린다.

	inline	BOOL	isChar(char ch);
	inline	BOOL	isCloseCode(char ch);
	inline	BOOL	isFinishCode(char ch);

	inline	BOOL	isBeginCode(char ch);
	inline	BOOL	isNotStreamCode(char ch);
	inline	BOOL	isSystemCode(char ch);
	inline	BOOL	isSkipChar(char ch);	//	스킵해야 하는 문자냐?
	inline	BOOL	isNumber(char ch);

	inline	BOOL	FindNumber();
	inline	char	FindAnyCharExact(BOOL _bIsExceptStream=FALSE);
	inline	char	FindCharExact(char ch,BOOL _bIsExceptStream=FALSE);
	inline	BOOL	FindCharOver();
	inline	BOOL	FindCharOver(char ch);

	char			getFirstChar();
	BOOL			increaseBP(int _iBP=1);

	BOOL			FindOpenCode();
	BOOL			FindCloseCode();
	BOOL			FindFinishCode();

	char			*getStream();
	char			*getStream(char cEndSymbol);

	char			*getWord(BOOL _bIsExceptStream=FALSE,char _cEndSymbol=-1);	//	따옴표 안에 문장은 제외
	char			*getLongWord(char _cEndSymbol=' ');	//	_cEndSymbol까지 얻어오기
	BOOL			FindWord(char *cmpWord);
	int				getNumber(BOOL *_lpbIsCorrect=NULL);

	int				getNextChar();

	DWORD			analCode(char *cmpKeyword);
	static	DWORD	AnalCode(CDecoderKeyword *keyword,char *cmpKeyword);

};

#endif
