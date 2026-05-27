#ifndef _classBARMENU_H
#define _classBARMENU_H

#include "SFC.H"
#include "cIMAGE.H"

#define		dMAX_MENUBAR			100

#define		dBAR_RADIO				1
#define		dBAR_CHECK				2

#define		dBI_LOGIN				0
#define		dBI_CREATE_ID			1
#define		dBI_CHANGE_PASS			2

class	cBARCOLOR
{
public:
	DWORD				NormalText,FocusText,CheckText,DisableText;
	WORD				NormalColor,FocusColor,CheckColor,DisableColor,NormalBox,FocusBox,CheckBox,DisableBox;
};

class cBAR
{
public:
	char				string[40];
	WORD				Normal,Active,Select,Check,Disable;
	DWORD				Menu;
	cRECT				CrashBox;
	cPOINT				Pos;
	BYTE				bCHECK,bCLICK,bFOCUS,bENABLE,bVIRGIN;
	BOOL				m_isDrawParents;

						cBAR();
						~cBAR();

	void				Init(cSPRITE *sprite,int x,int y,DWORD menu,WORD normal,WORD active=0xffff,WORD select=0xffff,WORD check=0xffff,WORD disable=0xffff);
	void				Init(char *str,DWORD menu,int x,int y,int xs,int ys);
	void				Reset(char *str,DWORD menu,int x,int y,int xs,int ys);

	void				Draw(cSPRITE *sprite,int alpha=32);
	void				Draw(cBARCOLOR *colorSet);

	void				SetVirgin(BOOL virgin)	{bVIRGIN	=	virgin;}
	void				SetSize(int xs,int ys);
	void				SetSize(int x,int y,int xs,int ys);
	void				SetStr(char *str)	{ustrncpy(string,str,38);}
	BOOL				isSELECT(int x,int y);
	void				Move(int x,int y);
	void				Move(int x,int y,int xs,int ys);
	void				Resize(int xs,int ys);
	void				Update(BOOL focus,BOOL click);
};

class cBARMENU : public cBARCOLOR
{
public:
	WORD				Count,FOCUS,MaxBar;	//	메뉴수,새로운 시리얼,현재 선택된거
	WORD				CheckMode;
	cSPRITE				*pIMAGE;
	cBAR				*pMENU;
	BOOL				m_isDrawParents;

						cBARMENU();
						~cBARMENU();

	void				Init(cSPRITE *image,int maxbar	= 80,WORD checkmode=dBAR_CHECK,BOOL drawParents=TRUE);
	void				Init(int maxbar	= 80,WORD checkmode=dBAR_CHECK,BOOL drawParents=TRUE);
	void				InitColor(	DWORD normalText ,DWORD focusText ,DWORD checkText ,DWORD disableText,
									WORD  noramlColor,WORD	focusColor,WORD  checkColor,WORD  disableColor);

	void				Reset();
	void				Close();
	BOOL				Add(char *str,DWORD menu,int x,int y,int xs,int ys);
	BOOL				Add(char *str,DWORD menu,int x,int y);
	BOOL				Add(int x,int y,DWORD menu,WORD normal=0xffff,WORD active=0xffff,WORD select=0xffff,WORD check=0xffff,WORD disable=0xffff);

	void				SetSize(DWORD menu,int xs,int ys);
	void				SetSize(int xs,int ys);
	void				SetSize(int x,int y,int xs,int ys);
	void				SetSize(DWORD menu,int x,int y,int xs,int ys);
	DWORD				Update(int x,int y,BOOL click,BOOL checkNow = TRUE);
	DWORD				GetMenu(int x,int y);
	int					GetIndex(DWORD menu);

	void				Draw(int alpha=32);
	BOOL				Move(DWORD menu,int x,int y);
	BOOL				Move(DWORD menu,int x,int y,int xs,int ys);
	BOOL				Resize(DWORD menu,int x,int y);

	void				Check(DWORD menu,WORD check=0xffff);
	BOOL				IsCheck(DWORD menu);
	BOOL				IsActive(DWORD menu);

	void				Enable(DWORD menu);
	void				Disable(DWORD menu);
	BOOL				IsEnable(DWORD menu);

	void				SetVirgin(DWORD menu,BOOL virgin);
	BOOL				Set(DWORD menu,char *str);
	BOOL				Remove(DWORD menu);


	int					GetRightPos();
};

#endif