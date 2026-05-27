#ifndef	_classITEM_ADDITIONAL_DATA
#define	_classITEM_ADDITIONAL_DATA

#include	"SFC.H"
#include	"cDECODER.H"
#include	"cITEM.H"
#include	"cWINDOW.H"
#include	"cITEM_PREFIX.H"

enum
{
eEE_BASE_ABILITY				,	//	장비부가 효과 - 기본 능력치 조절
eEE_DAMAGE_DEFENSE_HP_CP		,	//	장비부가 효과 - 기본 능력치 조절
eEE_PROBABILITY					,	//	장비부가 효과 - 각종 확률 조절
eEE_CORRECT_KEEP_TIME			,	//	장비부가 효과 - 각종 확률 조절
eEE_ADDITINAL_EFFECT			,	//	장비부가 효과 - 공격 성공시 추가 효과
eEE_MAGIC_REGISTENCE			,	//	장비부가 효과 - 마법 저항
eEE_TOLERANCE					,	//	장비부가 효과 - 각종 저항및 면역
eEE_COUNTER_SKILL				,	//	장비부가 효과 - 반격/스킬
eEE_ENCHANT						,	//	장비부가 효과 - 인챈트
eEE_CORRECT_MAGIC_DAMAGE		,	//	장비부가 효과 - 원소 대미지 보정
eEE_ETC_SPECIAL_ABILITY			,	//	장비부가 효과 - 기타/특수 능력
};
//////////////////////////////////////////////////////
//	컨트롤
//////////////////////////////////////////////////////

#define	IDC_ITEM_COMBO						1000
#define	IDC_ITEM_SPIN						2000
#define	IDC_ITEM_SPINBOX					3000

class	cPREFIX_EFFECT_REFERENCE
{
public:
	int							m_iSerial;
	char						m_strReferenceText[256];
};

class	cPREFIX_EFFECT
{
public:
	static	HWND						s_hDlg;
	static	cPREFIX_EFFECT_REFERENCE	s_aHyperText[dMAX_ITEM_PREFIX_TYPE_COUNT];
	static	cTREEVIEW					s_treeView;
	static	char						s_strFilter[256];

	static	int							GetIndex(int serial);
	static	int							GetCategory(int _iEffect);
	static	int							GetEffectCountByCategory(int _iCategory);

	static	cPREFIX_EFFECT_REFERENCE*	Get(int serial);

//	인테페이스 관련
	static	int							s_iSelectEffect,s_iSelectEffectCategory;
	static	BOOL						s_isJongSung;
	static	int							s_iLastCategory;

	static	cSTATIC						s_aStream[10];
	static	cCOMBOBOX					s_aCombo[10];
	static	cEDITBOX					s_aSpinBox[10];
	static	cSPIN						s_aSpin[10];

	static	cDECODER					s_Reader;
	static	cKEYWORD_ITEM				s_aKeyword[100];
	static	int							s_aPrefixEffectListByCategory[20][60];
	static	cITEM_EFFECT				s_Effect;
	static	int							s_iSpinIndex,s_iSpinBoxIndex,s_iComboIndex,s_iStreamIndex;
	static	char						s_strComboStr[100][20][40];
	static	char						*s_strPrefixEffectCategoryList[];


	static	BOOL						AddEffect(HWND hwnd,cITEM_EFFECT *lpEffect=NULL);


	static	void						CheckJongsung(char *str);

	static	void						TransCode(char *lpStream);

	static	void						InitItemEffectDlg(HWND hDlg);

	static	int							CustomComboBox(HWND hwnd,int x,int y,int kind,int select);
	static	int							AttackBonusSkillComboBox(HWND hwnd,int x,int y,int kind,int select);
	static	int							RevengeSkillComboBox(HWND hwnd,int x,int y,int kind,int select);
	static	int							PassiveSkillComboBox(HWND hwnd,int x,int y,int kind,int select);
	static	int							AllSkillComboBox(HWND hwnd,int x,int y,int kind,int select);
	static	int							SpinEditBox(HWND hwnd,int x,int y,int select);

	static	char*						GetComment(cITEM_EFFECT *lpEffect,BOOL _bSpecial,char *_strValue1=NULL,char *_strValue2=NULL);

	static	char*						GetCustomComboText(int kind,int data);
	static	char*						GetSpinText(int data);

	static	void						FillTree();

	
	static	BOOL						ReadEffectData(HWND hwnd,cITEM_EFFECT *lpEffect);
	static	BOOL						WriteItemEffectText(HWND hwnd,cITEM_EFFECT *lpEffect);

	static	LRESULT CALLBACK			s_procItemEffect(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
};

#define	dIKW_SPIN					0
#define	dIKW_PLUS_MINUS_SET			1
#define	dIKW_MIN_MAX_DAMAGE			2
#define	dIKW_BASE_POWER				3
#define	dIKW_CHARGEABLE_SKILL		4
#define	dIKW_ATTACK_BONUS_SKILL		5
#define	dIKW_REVENGE_SKILL			6
#define	dIKW_PASSIVE_SKILL			7
#define	dIKW_ALL_SKILL				8
#define	dIKW_PLUS_MINUS				9
#define	dIKW_AP_DP					10
#define	dIKW_MAGIC_ATTRIBUTE		11
#define	dIKW_JOB					12

#endif