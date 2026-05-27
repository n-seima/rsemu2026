#ifndef _classITEM_EFFECT_H
#define _classITEM_EFFECT_H

#include "cITEM.H"
#include "CItemEffectDefine.H"
#include "cWINDOW.H"
#include "cDECODER.H"

class	cITEM_GENERATE_EFFECT_INDEX
{
public:
	char					m_strName[128];
	int						m_iSerial;
};

class	cITEM_GENERATE_EFFECT
{
public:
	char						m_strName[128];
	int							m_iSerial;
	char						m_strReferenceText[256];

	static	char				s_strFilter[256];

	static	HWND				s_hDlg;

	static	int					GetIndex(int serial);
	static	int					GetCategory(int _iEffect);
	static	int					GetEffectCount(int _iCategory);

	static	cITEM_GENERATE_EFFECT*	Get(int serial);


//	인테페이스 관련
	static	int					s_iSelectEffect,s_iSelectEffectCategory;
	static	BOOL				s_isJongSung;
	static	int					s_iLastCategory;

	static	cSTATIC				s_aStream[10];
	static	cCOMBOBOX			s_aCombo[10];
	static	cEDITBOX			s_aSpinBox[10];
	static	cSPIN				s_aSpin[10];
	static	cTREEVIEW			s_treeView;

	static	cDECODER			s_Reader;
	static	cKEYWORD_ITEM		s_aKeyword[100];
	static	cITEM_GENERATE_EFFECT		s_aHyperText[dMAX_ITEM_EFFECT_DATA];
	static	cITEM_GENERATE_EFFECT_INDEX	s_aItemEffectIndex[20][60];
	static	cITEM_GENERATE_DATA			s_itemData;
	static	int					s_iSpinIndex,s_iSpinBoxIndex,s_iComboIndex,s_iStreamIndex;
	static	char				s_strComboStr[100][20][40];
	static	WORD				s_aValue[2][2];

	static	BOOL				AddEffect(HWND hwnd,cBASIC_ITEM *_lpItem,cITEM_GENERATE_EFFECT *lpEffect=NULL);


	static	void				CheckJongsung(char *str);

	static	void				TransCode(char *lpStream);

	static	void				InitItemEffectDlg(HWND hDlg);
	static	void				FillTree();	//	트리 내용 채우기

	static	int					CustomComboBox(HWND hwnd,int x,int y,int kind,int select);
	static	int					AttackBonusSkillComboBox(HWND hwnd,int x,int y,int kind,int select);
	static	int					RevengeSkillComboBox(HWND hwnd,int x,int y,int kind,int select);
	static	int					PassiveSkillComboBox(HWND hwnd,int x,int y,int kind,int select);
	static	int					AllSkillComboBox(HWND hwnd,int x,int y,int kind,int select);
	static	int					SpinEditBox(HWND hwnd,int x,int y,int select);

	static	char*				GetComment(cITEM_GENERATE_DATA *_lpData,cBASIC_ITEM *_lpItem,BOOL _bSpecial=FALSE,int _iValue1=-1,int _iValue2=-1);

	static	char*				GetCustomComboText(int kind,int data);
	static	char*				GetSpinText(int data);

	
	static	BOOL				ReadEffectData(HWND hwnd,cITEM_GENERATE_DATA *lpItemData);
	static	BOOL				WriteItemEffectText(HWND hwnd,cITEM_GENERATE_DATA *lpItemData);

	static	LRESULT CALLBACK	s_procItemEffect(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
};


#endif