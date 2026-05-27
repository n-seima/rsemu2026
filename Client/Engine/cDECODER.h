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
#define	dMAX_STREAM_LENGTH		1024*10

class	CDecoderKeyword
{
public:
	char	m_strKeyword[dMAX_WORD_LENGTH];
	DWORD	m_dwCode;
};

//
//	get number result
enum
{
	eGNR_FAILED,
	eGNR_SUCCED,
	eGNR_IS_FINISH_CODE,
};

class CDecoder
{
	char			m_strWORD[dMAX_WORD_LENGTH];							//	단어
	char			m_strStream[dMAX_STREAM_LENGTH];
	int				m_iBP,m_iFileLength;

	CDecoderKeyword	*m_lpKeyWord;
	char			*m_pBuffer;

public:
	BOOL			m_isDone;

					CDecoder();
					~CDecoder();

	BOOL			Upload(char *fn,CDecoderKeyword *keyword=NULL);
	BOOL			UploadBuffer(char *stream,CDecoderKeyword *keyword=NULL);
	BOOL			Upload(CDecoderKeyword *keyword);
	void			Reset();
	void			rewind()	{m_iBP	=	0;}	//	처음으로 돌린다.

	char			*get()	{return	m_pBuffer+m_iBP;}

	inline	BOOL	isChar(char ch);
	inline	BOOL	isCloseCode(char ch);
	inline	BOOL	isFinishCode(char ch);

	inline	BOOL	isBeginCode(char ch);
	inline	BOOL	isNotStreamCode(char ch);
	inline	BOOL	isSystemCode(char ch);
	inline	BOOL	isSkipChar(char ch);	//	스킵해야 하는 문자냐?
	inline	BOOL	isNumber(char ch);

	BOOL			findNumber();
	char			findAnyCharExact(BOOL _bIsExceptStream=FALSE);
	char			findCharExact(char ch,BOOL _bIsExceptStream=FALSE);
	BOOL			findCharOver();
	BOOL			findCharOver(char ch);

	char			getFirstChar();
	BOOL			increaseBP(int _iBP=1);

	BOOL			findOpenCode();
	BOOL			findCloseCode();
	BOOL			findFinishCode();

	char			*getStream();
	char			*getStream(char cEndSymbol);
	char			*readStream(char _cCheckSymbol);

	char			*getWord(BOOL _bIsExceptStream=FALSE,char _cEndSymbol=-1);	//	따옴표 안에 문장은 제외
	char			*getLongWord(char _cEndSymbol=' ');	//	_cEndSymbol까지 얻어오기
	BOOL			findWord(char *cmpWord);
	int				getNumber(int *_lpiResult=NULL,BOOL _bIsCheckFinishCode=FALSE);

	char			*getCSVWord(BOOL *_lpbIsEnd=NULL);
	int				getCSVNumber(BOOL *_lpbIsEnd=NULL);
	int				getCSVNumberList(int *_lpiList=NULL,BOOL *_lpbIsEnd=NULL);
	char*			doCSVParsing(BOOL *_lpbIsEnd=NULL);

	int				getNextChar();

	DWORD			analCode(char *cmpKeyword);
	static	DWORD	AnalCode(CDecoderKeyword *keyword,char *cmpKeyword);

};

#define	dDC_WORD_SYSTEM		"__isSystemCode"
#define	dDC_WORD_OPEN		"__isOpenCode"
#define	dDC_WORD_CLOSE		"__isCloseCode"
#define	dDC_WORD_END		"__isEndCode"
#define	dDC_WORD_VALID		"__isValidCode"
#define	dDC_WORD_FINISH		"__isFinishCode"

#endif
