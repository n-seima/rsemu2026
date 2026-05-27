#ifndef _classTEXT_H
#define _classTEXT_H

#include "SFC.H"

const	WHITE		=	RGB(255,255,255);
const	BLACK		=	0;

const	LTGRAY		=	RGB(188,188,188);
const	GRAY		=	RGB(128,128,128);
const	DKGRAY		=	RGB(64,64,64);

const	LTRED		=	RGB(255,0,0);
const	RED			=	RGB(188,0,0);
const	DKRED		=	RGB(128,0,0);

const	LTGREEN		=	RGB(0,255,0);
const	GREEN		=	RGB(0,188,0);
const	DKGREEN		=	RGB(0,128,0);

const	LTBLUE		=	RGB(0,0,255);
const	BLUE		=	RGB(0,0,188);
const	DKBLUE		=	RGB(0,0,128);

const	LTYELLOW	=	RGB(255,255,0);
const	YELLOW		=	RGB(188,188,0);
const	DKYELLOW	=	RGB(128,128,0);

const	LTCYAN		=	RGB(0,255,255);
const	CYAN		=	RGB(0,188,188);
const	DKCYAN		=	RGB(0,128,128);

const	LTPURPLE	=	RGB(255,0,255);
const	PURPLE		=	RGB(188,0,188);
const	DKPURPLE	=	RGB(128,0,128);
const	SKYBLUE		=	RGB(165,203,247);

class cTEXT 
{
	static	DWORD		s_dwShadowColor,s_dwPushShadowColor;
	static	DWORD		s_dwPutColor;
	static	BOOL		m_isPutShadow,m_isPushShadow;
	static	int			Width,Height,maxLines,fitSize,bkMODE;
	static	char		strFont[64];

	static	HWND		m_hWND;
	static	HDC			m_hDC;
	static	HFONT		m_hFONT;
	static	HFONT		m_smallFONT;

public:
						cTEXT();
						~cTEXT();

	static	void		Active(HDC hdc);
	static	void		Close();

	static	void		PushShadow()	{s_dwPushShadowColor=s_dwShadowColor,m_isPushShadow=m_isPutShadow;}
	static	void		PopShadow()		{s_dwShadowColor=s_dwPushShadowColor,m_isPutShadow=m_isPushShadow;}
	static	void		PushShadow(BOOL put,DWORD color=0);

	static	void		SetShadow(BOOL shadow,WORD color=0)		{m_isPutShadow=shadow,s_dwShadowColor=color;}

	static	void		Destroy();
	static	void		SetFont	(int xs,int ys,char *font);

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
};


BOOL	IsJongsung(char *word);

#endif