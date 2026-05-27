//	개     요 : 트리거 시스템에 이은 카르마 시스템
//	
//	작성 날짜 : 2000.12.04

#ifndef _classKARMA_H
#define	_classKARMA_H

#include "SFC.H"
#include "cWINDOW.H"
#include "CKarmaKeyword.H"
#include "CKarmaDefine.H"
#include "cMESSAGE.H"

#define	dKARMA_GROUP_COUNT			64		//	카르마 그룹 최대 수치
#define	dKARMA_COUNT				1024	//	카르마 최대 수치

#define	dTRIGGER_COUNT				1024	//	트리거 최대 수치
#define	dREACTION_COUNT				1024	//	반응 최대 수치

#define	dINSTANT_KARMA_GROUP		0		//	현재 맵에서만 적용되는 카르마 그룹
#define	dPROJECT_KARMA_GROUP		1		//	프로젝트 전체에서 적용되는 카르마 그룹

#define	IDC_KARMA_COMBO				1000
#define	IDC_KARMA_SPIN				2000
#define	IDC_KARMA_SPINBOX			3000
#define	IDC_KARMA_NUMBERBOX			4000
#define	IDC_KARMA_EDITBOX			5000

#define	dKARMA_LOCAL_KARMA_SET		0
#define	dKARMA_PROJECT_KARMA_SET	1

extern	int				g_iSelectKarma,g_iSelectTrigger,g_iSelectReaction,g_iLastTrigger,g_iLastReaction;
extern	int				g_iSelectTerm;

class	CTriggerActiveCondition
{
public:
	WORD				m_isAnd				: 1;
	WORD				m_isOr				: 1;
	WORD				m_isWantNotComplete	: 1;
};

class CKarmaItemBasicInfo
{
public:
	WORD		m_wItem;
	int			m_aValue[6];
	WORD		m_wStringSize	:	15;
	WORD		m_bf1IsAbsolute	:	1;
	BOOL		isSame(CKarmaItemBasicInfo* _lpKarmaItemBasicInfo)
	{
		if(m_wItem != _lpKarmaItemBasicInfo->m_wItem)
			return FALSE;
		if(memcmp(m_aValue,_lpKarmaItemBasicInfo->m_aValue , sizeof(int) * 6) != 0)
			return FALSE;
		if(m_wStringSize != _lpKarmaItemBasicInfo->m_wStringSize)
			return FALSE;
		if(m_bf1IsAbsolute != _lpKarmaItemBasicInfo->m_bf1IsAbsolute)
			return FALSE;

		return TRUE;
	}
};

//	카르마의 반응
class CKarmaItem : public CKarmaItemBasicInfo
{
public:
	char		*m_strString;

				CKarmaItem()
				{	m_wStringSize	=	0;
					m_strString		=	NULL;
					m_wItem			=	0xffff;
					memset(m_aValue,0,sizeof(m_aValue));
				}

				~CKarmaItem()
				{
					pKILL(m_strString);
				}

	void		Reset()
				{
					m_wStringSize	=	0;
					m_wItem			=	0xffff;
					memset(m_aValue,0,sizeof(m_aValue));

					pKILL(m_strString);
				}

	void		SetString(char *str)
				{	pKILL(m_strString);

					if (str == NULL)
					{	m_wStringSize	=	0;
						return;
					}

					m_wStringSize	=	strlen(str)+1;
					m_strString		=	new char [m_wStringSize];
					strcpy(m_strString,str);
				}

	void		copy(CKarmaItem *reaction)
				{
					m_bf1IsAbsolute=	reaction->m_bf1IsAbsolute;
					m_wItem		=	reaction->m_wItem;
					m_aValue[0]	=	reaction->m_aValue[0];
					m_aValue[1]	=	reaction->m_aValue[1];
					m_aValue[2]	=	reaction->m_aValue[2];
					m_aValue[3]	=	reaction->m_aValue[3];
					m_aValue[4]	=	reaction->m_aValue[4];
					m_aValue[5]	=	reaction->m_aValue[5];

					SetString(reaction->m_strString);
				}
	BOOL		isSame(CKarmaItem* _lpKarmaItem)
	{
		if(!((CKarmaItemBasicInfo*)this)->isSame((CKarmaItemBasicInfo*)_lpKarmaItem))
			return FALSE;

		if(m_wStringSize)
		{
			if(strcmp(m_strString,_lpKarmaItem->m_strString) != 0)
				return FALSE;
		}

		return TRUE;
	}
};

#define	dKARMA_NAME_LENGTH	32


class CKarmaBasicInfo
{
public:
	WORD					m_wSerial;			//	카르마 시리얼

	WORD					m_isWantAndCondition: 1;
	WORD					m_isWantNotComplete	: 1;
	WORD					m_bf1IsDisable		: 1;
	WORD					m_bf1IsForEnterFieldPlayer	:	1;
	WORD					m_bf7ActivateTriggerCount	:	7;
	WORD					m_bf1IsApplyAllMember		:	1;
	WORD	:	4;
	

	WORD					m_wReactionCount,m_wTriggerCount;		//	조건의 수,반응의 수
	WORD					m_wTitleLength;
	WORD					m_wActivateChance;
	
	WORD					m_wActiveCount;
	WORD					m_wActivePeriod;

	BOOL					isSame(CKarmaBasicInfo* _lpKarmaBasicInfo)
	{
		if(m_wSerial != _lpKarmaBasicInfo->m_wSerial)
			return FALSE;

		if(m_isWantAndCondition != _lpKarmaBasicInfo->m_isWantAndCondition)
			return FALSE;
		if(m_isWantNotComplete != _lpKarmaBasicInfo->m_isWantNotComplete)
			return FALSE;
		if(m_bf1IsDisable != _lpKarmaBasicInfo->m_bf1IsDisable)
			return FALSE;
		if(m_bf1IsForEnterFieldPlayer != _lpKarmaBasicInfo->m_bf1IsForEnterFieldPlayer)
			return FALSE;
		if(m_bf7ActivateTriggerCount != _lpKarmaBasicInfo->m_bf7ActivateTriggerCount)
			return FALSE;
		if(m_bf1IsApplyAllMember != _lpKarmaBasicInfo->m_bf1IsApplyAllMember)
			return FALSE;
		
		if(m_wReactionCount != _lpKarmaBasicInfo->m_wReactionCount)
			return FALSE;
		if(m_wTriggerCount != _lpKarmaBasicInfo->m_wTriggerCount)
			return FALSE;
		if(m_wTitleLength != _lpKarmaBasicInfo->m_wTitleLength)
			return FALSE;
		if(m_wActivateChance != _lpKarmaBasicInfo->m_wActivateChance)
			return FALSE;
		
		if(m_wActiveCount != _lpKarmaBasicInfo->m_wActiveCount)
			return FALSE;
		if(m_wActivePeriod != _lpKarmaBasicInfo->m_wActivePeriod)
			return FALSE;
		
		return TRUE;
	}
};

//	카르마 시스템
class CKarma	: public CKarmaBasicInfo
{
public:

	char				*m_pstrTitle;
	CKarmaItem			*m_pReaction,*m_pTrigger;

						CKarma()
						{
							m_pTrigger	=	NULL;
							m_pReaction	=	NULL;
							m_pstrTitle	=	NULL;

							reset();

							m_pstrTitle	=	new char [dKARMA_NAME_LENGTH];
							strcpy(m_pstrTitle,"nameless Karma");
						}

						~CKarma()
						{
							pKILL(m_pTrigger);
							pKILL(m_pReaction);
							pKILL(m_pstrTitle);
						}

	static	int			s_iNewKarma;

	void				reset(BOOL full=FALSE);
	void				reset(int iTriggerCount,int iReactionCount);
	void				allocBuffer(int iTriggerCount=-1,int iReactionCount=-1);


	void				copy(CKarma *lpKarma,BOOL full=FALSE);

	BOOL				RemoveReaction(int index,int count=1);			//	리액션 제거
	BOOL				RemoveTrigger(int index,int count=1);			//	트리거 제거

	void				PasteReaction(int pos,CKarma *ClipKarma);
	void				PasteTrigger(int pos,CKarma *ClipKarma);

	char*				getTitle(){return m_pstrTitle;}
	void				setTitle(char *_lpstr)
	{	
		pKILL(m_pstrTitle);

		if	(_lpstr	== NULL)
		{	
			m_wTitleLength=	0;

			return;
		}

		m_wTitleLength	=	strlen(_lpstr)+1;
		m_pstrTitle		=	new char [m_wTitleLength];
		strcpy(m_pstrTitle,_lpstr);
	}
	BOOL				isSame(CKarma* _lpKarma)
	{
		if(!((CKarmaBasicInfo*)this)->isSame((CKarmaBasicInfo*)_lpKarma))
			return FALSE;
		
		if(m_wTitleLength)
		{
			if(strcmp(m_pstrTitle, _lpKarma->m_pstrTitle) != 0)
			return FALSE;
		}

		for(int iReaction = 0;iReaction < m_wReactionCount ; ++iReaction)
		{
			if(!m_pReaction[iReaction].isSame(&_lpKarma->m_pReaction[iReaction]))
			return FALSE;
		}
		for(int iTrigger = 0; iTrigger < m_wTriggerCount; ++iTrigger)
		{
			if(!m_pTrigger[iTrigger].isSame(&_lpKarma->m_pTrigger[iTrigger]))
			return FALSE;
		}

		
		return TRUE;
	}

	static	void		RESET();
};	//	class CKarma

class	CKarmaContainerBasicElement
{
public:
	WORD				m_wSerial;	//	대사 길이
	WORD				m_wTitleLength;	//	대사 길이
	WORD				m_wKarmaCount;	//	이 대화가 처음에 시작될 조건 수
	BOOL				isSame(CKarmaContainerBasicElement* _lpKarmaContainerBasicElement)
	{

		if(m_wSerial != _lpKarmaContainerBasicElement->m_wSerial)
			return FALSE;

		if(m_wTitleLength != _lpKarmaContainerBasicElement->m_wTitleLength)
			return FALSE;
		
		if(m_wKarmaCount != _lpKarmaContainerBasicElement->m_wKarmaCount)
			return FALSE;

		return TRUE;
	}
};

class	CKarmaContainer : public CKarmaContainerBasicElement
{
public:
	char				*m_pstrTitle;	//	카르마 이름
	CKarma				*m_pKarma;		//	카르마 데이터

						CKarmaContainer()
						{
							m_pstrTitle	=	NULL;	//	선택문
							m_pKarma	=	NULL;				//	선택 항목

							reset();
						}

						~CKarmaContainer()
						{
							reset();
						}

	void				setTitle(char *_lpstr)
	{	
		pKILL(m_pstrTitle);

		if (_lpstr	== NULL)
		{	m_wTitleLength=	0;
			return;
		}

		m_wTitleLength	=	strlen(_lpstr)+1;
		m_pstrTitle		=	new char [m_wTitleLength];
		strcpy(m_pstrTitle,_lpstr);
	}

	void				selectKarma(int karma)	{	g_iSelectKarma	=	karma;}
	CKarma				*getSelectKarma()		
	{
		if	(g_iSelectKarma	>=	m_wKarmaCount)
			g_iSelectKarma	=	m_wKarmaCount;

		if	(g_iSelectKarma	>=	m_wKarmaCount)
			return	NULL;

		return	&m_pKarma[g_iSelectKarma];
	}
	int					getKarmaCount()			{	return	m_wKarmaCount;}

	void				allocBuffer(int _iKarmaCount);
	void				allocBufferForEdit();

	void				reset();
	void				copy(CKarmaContainer *_lpKarma,BOOL _bIsForEdit=FALSE);

	int					add(CKarma *_lpKarma);
	BOOL				remove(int _iIndex);
	BOOL				isSame(CKarmaContainer* _lpKarmaContainer)
	{

		if(!((CKarmaContainerBasicElement*)this)->isSame((CKarmaContainerBasicElement*)_lpKarmaContainer))
			return FALSE;

		if(m_wTitleLength)
		{
			if(strcmp(m_pstrTitle, _lpKarmaContainer->m_pstrTitle) != 0)
				return FALSE;
		}
		
		for(int i=0;i<m_wKarmaCount;++i)
		{
			if(!m_pKarma[i].isSame(&_lpKarmaContainer->m_pKarma[i]))
				return FALSE;		
		}

		return TRUE;
	}
};	//	class	CKarmaContainer

enum
{
	eADD_KARMA_NORMAL,
	eADD_KARMA_SELECT_DIALOG,
	eADD_KARMA_SPEECH,
};

const	c_iCopyKarmaBufferSize	=	100;

class	cKARMA_WND	:	public CKarma
{
public:
	static	BOOL			s_bIsFieldKarma;
	static	BOOL			s_isJongSung;
	static	char			s_strTitlebar[128];
	static	char			s_strFilter[256];
	static	BOOL			s_bIsEditDialog, s_bIsDrawTitle;
	static	int				s_iDungeonvalueComboBox,s_iSelectDungeonValue,s_iQuestJobQuest,s_iQuestArbeitJobQuest;
	static	int				s_iSelectMainQuestValue,s_iSelectMainQuestSwitch;
	static	int				s_iMainQuestValueComboBox,s_iMainQuestSwitchComboBox;
	static	cCOMBOBOX		s_aCombo[10];

	static	BOOL			s_iSelectKarmaSet;			//	선택한 탭
	static	HWND			s_hTab;						//	탭의 윈도우 핸들..
	static	int				s_iPasteReactionPos,s_iPasteTriggerPos,s_iCopiedKaramaCount;

	static	CKarma			*s_lpkcEdit,*s_lpSelectKarma,s_aStoredKarma[c_iCopyKarmaBufferSize],s_storedKarmaForDetail;

	static	int				s_iKarmaDetailWndPos;		//	카르마의 정보를 표시하는 윈도우의 스크롤바 위치
	static	cDIBWND			s_hKarmaDetailWnd;
	static	HWND			s_hMaster;
	static	HINSTANCE		s_hInst;
	static	BOOL			s_isEditKarma,s_isEditTrigger,s_isEditReaction;	//	에디터 모드냐?

	static	BOOL			Open(HWND hwnd,HINSTANCE hinst,CKarmaContainer *_lpKC,BOOL _bIsKarma=FALSE,BOOL _bIsFieldKarma=FALSE);//	카르마 대화상자 띄우기.
	static	void			InitKarmaDlg(HWND hwnd, BOOL _bTitle = TRUE, CKarmaContainer *_lpKC=NULL);
	static	void			UpdateKarmaDlg(HWND hwnd,CKarmaContainer *_lpKC=NULL);
	static	void			DrawKarmaDetail(HWND hDlg);		//	카르마의 세부사항을 그려준다.

	static	void			UpKarma(HWND hDlg);
	static	void			DownKarma(HWND hDlg);
	static  void			UpTrigger(HWND _hDlg);		// insu add
	static  void			DownTrigger(HWND _hDlg);	// insu add
	static	void			CopyKarma(HWND hDlg);			//	카르마 리스트에서 카르마를 선택
	static	void			PasteKarma(HWND hDlg);	//	카르마 그룹 선택
	static	void			SelectKarma(HWND hDlg);			//	카르마 리스트에서 카르마를 선택
	static	void			SelectKarmaGroup(HWND hDlg);	//	카르마 그룹 선택
	static	void			RemoveKarma(HWND hDlg);			//	선택된 카르마 제거


	static	LRESULT CALLBACK	s_procKarmaMain(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procKarmaDetailWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//
//	카르마 추가
	static	cDIBWND		s_hTriggerWnd,s_hReactionWnd;
	static	cSCROLL_BAR s_sbReaction,s_sbTrigger;
	static	HWND		s_hAddKarmaDlg;
	static	int			s_iTriggerWndPos;		//	위치
	static	int			s_iReactionWndPos;
	static	int			s_iEditType;

	static	CKarmaItem	s_storedTrigger,s_storedReaction;

	static	CKarma*		AddKarma(HWND hdlg,char *_lpstrTitlebar,CKarma *lpEditKarma=NULL,int _iType=eADD_KARMA_NORMAL);	//	카르마 추가
	static	BOOL		ReadAddKarmaDlg(HWND hdlg);

	static	void		CutReaction(HWND hdlg);
	static	void		CopyReaction(HWND hdlg);
	static	void		PasteReaction(HWND hdlg);
	static	void		RemoveReaction(HWND hdlg,BOOL safe=TRUE);

	static	void		CutTrigger(HWND hdlg);
	static	void		CopyTrigger(HWND hdlg);
	static	void		PasteTrigger(HWND hdlg);
	static	void		RemoveTrigger(HWND hdlg,BOOL safe=TRUE);

	static	void		InitAddKarmaDlg(HWND hwnd);		//	카르마 추가 대화상자 초기화
	static	void		DrawTriggerWnd(HWND hwnd);		//	등록된 조건들을 그려준다.
	static	void		DrawReactionWnd(HWND hwnd);		//	등록된 반응들을 그려준다.
	static	int			SelectTrigger(HWND hwnd);		//	트리거 선택
	static	int			SelectReaction(HWND hwnd);

	static	int			GetSelectReaction(HWND hwnd);	//	선택한 리액션을 얻어온다.
	static	int			GetSelectTrigger(HWND hwnd);	//	선택한 트리거를 얻어온다.

	static	void		PopupReaction(HWND hdlg);		//	리액션의 팝업
	static	void		PopupTrigger(HWND hdlg);		//	트리거의 팝업
	static	BOOL		IsSelectReaction(int index);	//	선택된 리액션이냐?
	static	BOOL		IsSelectTrigger(int index);		//	선택된 트리거냐?

	static	void		UpReaction(HWND _hDlg);
	static	void		DownReaction(HWND _hDlg);
	
	static	void		OperateChangeComboBoxText(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
	static	void		OperateSelectChangeComboBox(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam,BOOL _bIsForReactioin);

	static	LRESULT CALLBACK	s_procAddKarma(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procTriggerWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static	LRESULT CALLBACK	s_procReactionWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//
//	트리거 추가
//
	static	int			GetTriggerIndex(int item);
	static	char*		GetTriggerText(int item);
	static	int			AddTrigger(HWND hdlg,CKarmaItem *_lpTrigger=NULL);			//	트리거를 추가하자!!
	static	void		InitAddTriggerDlg(HWND hDlg,int _x = 18,int _y = 14);		//	트리거 추가 윈도우 초기화.
	static	void		FillTriggerList(HWND hDlg,int _x = 18,int _y = 14);

	static	void		SelectTriggerItem(HWND hDlg,int _x = 18,int _y = 14);		//	트리거 선택
	static	void		ReadTriggerData(HWND hDlg);

	static	LRESULT CALLBACK	s_procAddTrigger(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);

//
//	액션 추가
//
	static	int			GetReactionIndex(int item);
	static	char*		GetReactionText(int item);
	static	int			AddReaction(HWND hdlg);			//	액션을 추가하자!!
	static	void		InitAddReactionDlg(HWND hDlg,int _x = 18,int _y = 14);	//	액션 추가 윈도우 초기화.
	static	void		FillReactionList(HWND hDlg,int _x = 18,int _y = 14);

	static	void		SelectReactionItem(HWND hDlg,int _x = 18,int _y = 14);	//	액션 선택
	static	void		ReadReactionData(HWND hDlg);


	static	LRESULT CALLBACK	s_procAddReaction(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam);
	static	void		AddCopyTrigger(HWND hDlg);
	static	void		EditCopyTrigger(HWND hDlg);

	static	void		AddCopyReaction(HWND hDlg);
	static	void		EditCopyReaction(HWND hDlg);
//
//	카르마 텍스트
//
	static	void		CheckJongsung(char *str);			//	종성이 있나 확인
	static	void		TransCode(char *lpStream);			//	바로전의 단어를 확인해서 보정 

	static	int			CustomComboBox		(HWND hwnd,int x,int y,int kind,int select);
	static	int			ComboCaster			(HWND hwnd,int x,int y,int select);
	static	int			ComboPlayerJob		(HWND hwnd,int x,int y,int select);
	static	int			ComboQuestJob		(HWND hwnd,int x,int y,int select);
	static	int			ComboArbeitJob		(HWND hwnd,int x,int y,int select);
	static	int			ComboSetElementItem	(HWND hwnd,int x,int y,int select);
	static	int			ComboItemPiece		(HWND hwnd,int x,int y,int select);


	static	int			ComboHero			(HWND hwnd,int x,int y,int select);
	static	int			ComboJob			(HWND hwnd,int x,int y,int select);
	static	int			ComboOnlyHero		(HWND hwnd,int x,int y,int select);
	static	int			ComboCharacter		(HWND hwnd,int x,int y,int select);
	static	int			ComboAllCharacter	(HWND hwnd,int x,int y,int select);
	static	int			ComboAllCharacterTargetPlayer(HWND hwnd,int x,int y,int select);
	static	int			ComboAllCharacterTargetPlayer2(HWND hwnd,int x,int y,int select);

	static	int			ComboArea			(HWND hwnd,int x,int y,int select);
	static	int			ComboTrapArea		(HWND hwnd,int x,int y,int select);
	static	int			ComboAllArea		(HWND hwnd,int x,int y,int select);

	static	int			ComboSkill			(HWND hwnd,int x,int y,int select);
	static	int			ComboFieldValue		(HWND hwnd,int x,int y,int select);
	static	int			ComboEventTimeName	(HWND hwnd,int x,int y,int select);
	static	int			ComboWorldValue		(HWND hwnd,int x,int y,int select);
	static	int			CustomSoundFile		(HWND hwnd,int x,int y,char *_lpstrSelectText);

	static	int			ComboDungeonValue	(HWND hwnd,int x,int y,int select);
	static	int			ComboBookList		(HWND hwnd,int x,int y,int select);
	static	int			ComboMainQuestValue	(HWND hwnd,int x,int y,int select);
	static	int			ComboMainQuestSwitch(HWND hwnd,int x,int y,int select);
	static	int			ComboChapter		(HWND hwnd,int x,int y,int select);
	static	int			ComboPattern		(HWND hwnd,int x,int y,int select);
	static	int			ComboGuildStatueSkill(HWND hwnd,int x,int y,int select);
	static	int			ComboRelatedPlaceOfGuild(HWND hwnd,int x,int y,int select);

	static	int			ComboMonster		(HWND hwnd,int x,int y,int select);
	static	int			ComboKarma			(HWND hwnd,int x,int y,int select);
	static	int			ComboOnlyArea		(HWND hwnd,int x,int y,int select);
	static	int			ComboValue			(HWND hwnd,int x,int y,int select);
	static	int			ComboMagic			(HWND hwnd,int x,int y,int select);
	static	int			ComboItem			(HWND hwnd,int x,int y,int select);
	static	int			ComboEquipment		(HWND hwnd,int x,int y,int select);
	static	int			ComboSpecificItem	(HWND hwnd,int x,int y,int select);
	static	int			ComboSpecialItem	(HWND hwnd,int x,int y,int select);
	static	int			ComboQuestItem		(HWND hwnd,int x,int y,int select);
	static	int			ComboTitle			(HWND hwnd,int x,int y,int select);

	
	static	int			ComboArmor			(HWND hwnd,int x,int y,int select);
	static	int			ComboMagicEffect	(HWND hwnd,int x,int y,int select);
	static	int			ComboMusicFile		(HWND hwnd,int x,int y,char *lpString);
	static	int			ComboSoundFile		(HWND hwnd,int x,int y,char *lpString);
	static	int			ComboImageFile		(HWND hwnd,int x,int y,char *lpString);
	static	int			ComboDoor			(HWND hwnd,int x,int y,int select);
	static	int			ComboBox			(HWND hwnd,int x,int y,int select);
	static	int			ComboBattleInclination	(HWND hwnd,int x,int y,int select);

	static	int			ComboScenario		(HWND hwnd,int x,int y,int select);
	static	int			ComboQuest			(HWND hwnd,int x,int y,int select);
	static	int			ComboQuestArbeit	(HWND hwnd,int x,int y,int select);
	static	int			ComboPower			(HWND hwnd,int x,int y,int select);

	static	int			ComboSpeech			(HWND hwnd,int x,int y,int select);
	static	int			ComboShop			(HWND hwnd,int x,int y,int select);

	static	int			ComboLimitedPower	(HWND hwnd,int x,int y,int select);
	static	int			ComboStatus			(HWND hwnd,int x,int y,int select);
	static	int			ComboAbility		(HWND hwnd,int x,int y,int select);

	static	int			KarmaTextSpin		(HWND hwnd,int x,int y,int select);
	static	int			KarmaTextNumber		(HWND hwnd,int x,int y,int select);
	static	int			KarmaTextEditBox	(HWND hwnd,int x,int y,char *lpStr);
	static	void		KarmaTextBox		(HWND hwnd,int x,int y,char *lpStr);


	static	char*		GetCustomComboText	(int kind,int data);

	static	char*		GetHeroText			(int data);
	static	char*		GetJobText			(int data);
	static	char*		GetCharacterText	(int data);
	static	char*		GetOnlyHeroText		(int data);
	static	char*		GetOnlyCharacterText(int data);
	static	char*		GetAreaText			(int data);
	static	char*		GetOnlyAreaText		(int data);
	static	char*		GetOInlyAreaText	(int data);
	static	char*		GetValueText		(int data);
	static	char*		GetMagicText		(int data);
	static	char*		GetItemText			(int data);
	static	char*		GetFieldValueText	(int data);
	static	char*		GetEventTimeNameText(int data);
	static	char*		GetMonsterText		(int data);
	
	static	char*		GetAllCharacterTargetPlayerText(int data);

	static	char*		GetSetElementItemText(int data);
	static	char*		GetItemPieceText(int data);

	static	char*		GetPlayerJobText	(int data);
	static	char*		GetTitleText		(int data);


	static	char*		GetSpecificItemText	(int data);
	static	char*		GetSkillText		(int data);
	static	char*		GetDungeonValueText	(int data);
	static	char*		GetBookText			(int data);
	static	char*		GetMainQuestValueText	(int data);
	static	char*		GetMainQuestSwitchText	(int data);
	static	char*		GetChapterText			(int data);
	static	char*		GetPatternText			(int data);
	static	char*		GetGuildStatueSkillText	(int data);
	static	char*		GetRelatedPlaceOfGuildText(int data);


	static	char*		GetBattleInclinationText(int data);

	static	char*		GetArmorText		(int data);
	static	char*		GetMagicEffectText	(int data);
	static	char*		GetMusicFileText	(int data);
	static	char*		GetSoundFileText	(int data);
	static	char*		GetDoorText			(int data);
	static	char*		GetBoxText			(int data);
	static	char*		GetScenarioText		(int data);
	static	char*		GetQuestText		(int data);
	static	char*		GetQuestArbeitText	(int data);
	static	char*		GetFieldText		(int _iData);
	static	char*		GetPowerText		(int data);

	static	char*		GetSpeechText		(int data);
	static	char*		GetShopText			(int data);

	static	char*		GetLimitedPowerText	(int data);
	static	char*		GetStatusText		(int data);
	static	char*		GetAbilityText		(int data);
	static	char*		GetSpinText			(int data);
	static	char*		GetNumberText		(int data);



	static	BOOL		WriteKarmaText(HWND hwnd,char *lpText,int *lpValue,char *lpString, int _x = 18, int _y = 14, int _Enable = IDOK);
	static	BOOL		ReadKarmaText(HWND hwnd,int iItem,char *lpText,void *lpData);
	static	char*		KarmaText(void *lpData,BOOL isTrigger);
	
	static  char*		KarmaTextOut(void *lpData,BOOL isTrigger);// 09.07.21 추가
	static	char*		KarmaTextOut(void *lpData,BOOL* _bSpeech, BOOL isTrigger);// 09.07.21 추가
};

//
//	Active Karma Container
extern	CKarmaContainer	*g_lpAKC;

extern	char			g_strGroupName[128];
extern	char			g_strKarmaName[128];

extern	CKarma			g_EditKarma;										//	현재 편집중인 카르마
extern	CKarmaItem		g_EditReaction,g_EditTrigger;

extern	int				g_iStreamIndex,g_iComboIndex,g_iSpinIndex,g_iEditBoxIndex,g_iButtonIndex,g_iNumberIndex,g_iSpinIndex;

#endif