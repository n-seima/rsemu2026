#ifndef _classTEXT_H
#define _classTEXT_H

#include "SFC.H"

#define	WHITE		(RGB(255,255,255))
#define	BLACK		(0)

#define	LTGRAY		(RGB(188,188,188))
#define	GRAY		(RGB(128,128,128))
#define	DKGRAY		(RGB(64,64,64))

#define	LTRED		(RGB(255,0,0))
#define	RED			(RGB(188,0,0))
#define	DKRED		(RGB(128,0,0))

#define	LTGREEN		(RGB(0,255,0))
#define	GREEN		(RGB(0,188,0))
#define	DKGREEN		(RGB(0,128,0))

#define	BTBLUE		(RGB(80,160,255))
#define	LTBLUE		(RGB(0,0,255))
#define	BLUE		(RGB(0,0,188))
#define	DKBLUE		(RGB(0,0,128))

#define	LTYELLOW	(RGB(255,255,0))
#define	YELLOW		(RGB(188,188,0))
#define	DKYELLOW	(RGB(128,128,0))

#define	LTCYAN		(RGB(0,255,255))
#define	CYAN		(RGB(0,188,188))
#define	DKCYAN		(RGB(0,128,128))

#define	LTPURPLE	(RGB(255,0,255))
#define	PURPLE		(RGB(188,0,188))
#define	DKPURPLE	(RGB(128,0,128))
#define	SKYBLUE		(RGB(165,203,247))
#define	GOLDYELLOW	(RGB(255,215,0))

class cTEXT 
{
	static	DWORD		s_dwShadowColor,s_dwPushShadowColor;
	static	DWORD		s_dwPutColor,s_dwOutlineColor;
	static	BOOL		m_isPutShadow,m_isPushShadow,s_bIsPutOutline;
	static	int			Width,Height,maxLines,fitSize,bkMODE,s_iStackLineGap;
	static	char		strFont[64];

	static	HWND		m_hWND;
	static	HDC			m_hDC;
	static	HFONT		m_hFONT;
	static	HFONT		m_hBoldFont;
	static	HFONT		m_smallFONT;

public:
						cTEXT();
						~cTEXT();

	static	int			GetLineHeight()	
	{
		return	Height+2;
	}
	static	void		Init();
	static	void		OutlineOn(DWORD _dwColor=0xfffffff7)
	{
		if (_dwColor	!=	0xfffffff7)
			s_dwOutlineColor=	_dwColor;

		s_bIsPutOutline		=	TRUE;
	}
	static	void		OutlineOff()
	{
		s_bIsPutOutline		=	FALSE;
	}

	static	void		Active(HDC hdc);
	static	void		Close();

	static	void		PushShadow()	{s_dwPushShadowColor=s_dwShadowColor,m_isPushShadow=m_isPutShadow;}
	static	void		PopShadow()		{s_dwShadowColor=s_dwPushShadowColor,m_isPutShadow=m_isPushShadow;}
	static	void		PushShadow(BOOL put,DWORD color=0);

	static	void		SetLineGap(int _iGap)
	{
		s_iStackLineGap	=	Height;
		Height			=	_iGap;
	}
	static	void		RestoreLineGap()
	{
		Height			=	s_iStackLineGap;
	}

	static	void		SetShadow(BOOL shadow,WORD color=0)		{m_isPutShadow=shadow,s_dwShadowColor=color;}

	static	void		SetBoldFont();
	static	void		SetNormalFont();

	static	void		Destroy();
	static	void		SetFont	(int xs,int ys,char *font,DWORD dwStyle=FW_NORMAL);

	static	void		Put		(int xPos,int yPos,DWORD color,char *pStr);			//	Device Cotext에 찍어준다.
	static	void		ScriptPut(int xPos,int yPos,DWORD color,char *pStr);			//	Device Cotext에 찍어준다.
	static	void		Put		(int xPos,int yPos,int begin,int end,DWORD color,DWORD reverseColor,char *pStr);
	static	void		PutReverse(int xPos,int yPos,DWORD color,DWORD reverceColor,char *pStr);		//	Device Cotext에 찍어준다.

	static	void		cPut	(int xPos,int yPos,DWORD color,char *pStr);
	static	void		cPut	(int xPos,int yPos,DWORD color,DWORD reverceColor,char *pStr);
	static	void		FitPut	(int xPos,int yPos,int xs,int ys,DWORD color,char *pStr,int base=0);	//	base번째 라인부터 찍어준다.
	static	char *		GetLine	(int xs,int line,char *pStr);
	static	int			GetLines(int xs,char *pStr);

	static	void		FitPut	(int xPos,int yPos,int xs,int ys,DWORD color,char **pStr,int base=0);
	static	char *		GetLine	(int xs,int line,char **pStr);
	static	int			GetLines(int xs,char **pStr);

	static	int			GetSize	(char *_strText);
	static	int			GetNotCrackeWordPoint(BYTE *_lpString,int _iPoint);

};


BOOL	IsJongsung(char *word);

extern	char	g_strThaiFont[64];
extern	int		g_iThaiFontWidth,g_iThaiFontHeight;


#endif