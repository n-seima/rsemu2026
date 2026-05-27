#ifndef	_classAREA_H
#define	_classAREA_H

#include "SFC.H"
#include "cDialog.H"
#include "doorDefine.H"
#include "areaDefine.H"

#define	dMAX_AREA				256

#define	dAREA_HERO_RANGE		11
#define	dMAX_EVENT_AREA_IN_MAP	64
#define	dMAX_TRAP_AREA_IN_MAP	128
#define	dMAX_ARCA_AREA_IN_MAP	128

class	cFIELD;
class	cACTOR;

#pragma pack(2)

class	cAREA_OLD_SAVE_DATA
{
public:
	WORD			m_wSerial;
	int				x1,y1,x2,y2;
	WORD			m_wKind;			//	영역 분류
	WORD			m_wValue;			//	open?close? / direct? / string size /
	BYTE			m_bGate;
	BYTE			m_gateDirect: 3;
	BYTE			m_gateShape	: 5;
	WORD			m_wMoveGate;//	입구,출구
	char			m_strName[dSIZEOF_AREA_NAME];
	WORD			m_bf15LinkObject : 15;	//	이 영역과 링크된 오브젝트
	WORD			m_bf1IsTestPortal: 1;	//	테스트중인 영역이다.
	char			m_strMoveGateName[dSIZEOF_AREA_NAME];
};

class	cAreaSaveData	:	public cAreaToolBaseData
{
public:
	char			m_aBuffer[100];
	WORD			m_wSeasonVariable;
};

class	cAreaDefaultInfo
{
public:
	char					*m_pstrText;
	WORD					m_wIsTestPortal;
	CConversationContainer	m_cc;
};

class	cAreaTrapInfo	:	 public cAreaDefaultInfo,public cAreaBaseInfo,public cAreaTrapDefine
{
public:
	WORD			m_wIsActive;
	DWORD			m_dwBookedRegenTime;

	WORD			m_awTrapActiveChance[2];
	WORD			m_awTrapSkill[2];

	cFIELD			*m_lpField;

	void			initTrap();
	void			disarm(int _iUpkeepTime=0);
	void			explosionTrap(cACTOR *_lpActor);
	void			operate1SecondWork();
	void			tryDisarm(cACTOR *_lpActor);
};

class	cAbility;
class	CHitInfo;

class	cAreaDoorInfo	:	public cAreaDefaultInfo,public cAreaBaseInfo,public cAreaDoorDefine
{
public:
	int				m_iHP;

	WORD			m_bf1IsActive		:	1;		//	이 문은 현재 활성화 되어 있다.
	WORD			m_bf1IsOpened		:	1;
	WORD			m_bf1IsDisarmed		:	1;
	WORD			m_bf1IsUnlocked		:	1;
	WORD			m_bf1IsHide			:	1;		//	이 문은 현재 숨어 있다.
	WORD			m_bf1IsActiveByKarma:	1;		//	이 문은 현재 숨어 있다.

	WORD			m_wRemainRegenCheckTime;	//	비밀문 리젠체크까지 남은 시간.
	WORD			m_wUpkeepActiveTime;		//	비밀문 생성후 유지되는 시간

	WORD			m_wUpkeepUnlockTime;
	WORD			m_wUpkeepDisarmedTime;
	int				m_iCurrentDurability;

	WORD			m_awLinkObject[dLINK_WITH_AREA_DOOR_COUNT];

	WORD			m_awTrapActiveChance[2];
	WORD			m_awTrapSkill[2];
	
	cFIELD			*m_lpField;

	void			open();
	void			close();
	void			unlock(int _iUnlockTime=-1);
	void			lock(int _iUnlockLevel=-1);
	void			generate();
	void			getDamage(cACTOR *_lpAttacker,cAbility *_lpAbility,CHitInfo *_lpHitInfo);
	void			hitDamage(cACTOR *_lpAttacker,int _iDamage);
	void			setFreeOpen();

	void			appear(int _iUpkeepTime=-1);
	void			disapper(int _iUpkeepTime=-1);
	void			armedTrap();
	void			disarmedTrap(int _iUpkeepTime=-1);

	void			explosionTrap(cACTOR *_lpActor);

	void			sendDoorStatus();

	void			operate1SecondWork();
	void			operate1MinuteWork();

	void			operateMoveToTherSecretDungeon(cACTOR *_lpActor);
	void			operatePlayerRequest(int _iWork,cACTOR *_lpActor);
};

class	cAreaArcaInfo	:	public cAreaDefaultInfo,public cAreaBaseInfo,public cAreaArcaDefine
{
public:
	WORD			m_wLinkActiveArca;
	WORD			m_wIsEnable;
	cFIELD			*m_lpField;

	void			operate1SecondWork();
	BOOL			generate();
};
class	cAreaHuntingInfo	:	public cAreaDefaultInfo,public cAreaBaseInfo,public cAreaHuntingDefine
{
public:

};
//
//	영역의 기본 데이터
class	cAreaInfo		:	public cAreaDefaultInfo,public cAreaBaseInfo
{
public:
					cAreaInfo()
					{	
						m_wSerial			=	0xffff;
						m_wKind				=	dAREA_NORMAL;
						m_wValue			=	0;
						m_bGate				=	0xff;
						m_wMoveGate			=	0xff;
						m_pstrText			=	NULL;
						m_wIsTestPortal		=	FALSE;
					}

					~cAreaInfo()
					{
						m_cc.reset();
						pKILL(m_pstrText);
					}

	void			set(cAreaSaveData *_lpArea);
	inline	void	resetForInit()
	{
		m_wSerial			=	0xffff;
		m_wKind				=	dAREA_NORMAL;
		m_wValue			=	0;
		m_bGate				=	0xff;
		m_wMoveGate			=	0xff;
		m_pstrText			=	NULL;
		m_wIsTestPortal		=	FALSE;
	}
	inline	void	reset()	{m_wSerial	=	0xffff;}
	inline	BOOL	isIn(int x,int y)
	{
		if	(x >= x1 && x <= x2 && y >= y1 && y <= y2)
			return TRUE;

		return FALSE;		
	}
	inline	void	setString(char *_strText)
	{
		pKILL(m_pstrText);

		if (_strText)
		{
			m_pstrText	=	new char [strlen(_strText)+1];

			strcpy(m_pstrText,_strText);
		}
	}

	void			operateKarma(cACTOR	*_lpAsker);
	BOOL			selectSpeech(cACTOR *_lpAsker,int _iConversationIndex,int _iSelectSpeech,BOOL _bIsInnerSelect=FALSE);
	inline	char*	getMapName()	{return	m_pstrText;}
	void			getCenterPos(cPOINT *_lpPos);
	void			getRandomPos(cPOINT *_lpPos);

	inline	BOOL	isMonsterActiveArea(){if (m_wKind==	dAREA_MONSTER_ACTIVE)	return	TRUE;	return	FALSE;}
	inline	BOOL	isStartArea()		{if (m_wKind==	dAREA_START_AREA	)	return	TRUE;	return	FALSE;}
	inline	BOOL	isSecretDungeonExit(){if (m_wKind==	dAREA_SECRET_DUNGEON_EXIT)	return	TRUE;	return	FALSE;}
	inline	BOOL	isGuildBattleArea()	{if (m_wKind==	dAREA_GUILD_BATTLE	)	return	TRUE;	return	FALSE;}
	inline	BOOL	isPortal()			{if (m_wKind==	dAREA_PORTAL		)	return	TRUE;	return	FALSE;}

	inline	BOOL	isTrap()			{if (m_wKind==	dAREA_TRAP			)	return	TRUE;	return	FALSE;}
	inline	BOOL	isDoor()			{if (m_wKind==	dAREA_DOOR			)	return	TRUE;	return	FALSE;}
	inline	BOOL	isEventArea()		{if (m_wKind==	dAREA_EVENT_AREA	)	return	TRUE;	return	FALSE;}
	inline	BOOL	isGuildCrest()		{if (m_wKind==	dAREA_GUILD_CREST	)	return	TRUE;	return	FALSE;}
	inline	BOOL	isArca()			{if (m_wKind==	dAREA_ARCA			)	return	TRUE;	return	FALSE;}

};	//	class	cAreaInfo

#pragma pack()

#define	dMAX_PORTAL_AREA_COUNT					256
#define	dAREA_START_POINT						256
#define	dPLACENAME_COUNT						100
#define	dMAX_GUILD_BATTLE_AREA_COUNT			10
#define	dMAX_SECRET_DUNGEON_GATE_COUNT_IN_FIELD	4	

//
//	영역들
class cArea
{
public:
	cAreaInfo		*m_pArea[dMAX_AREA];
	int				m_iField;
	int				m_iCount;
	int				m_aPortal[dMAX_PORTAL_AREA_COUNT+1],m_iTestPortalCount,m_iSecretDungeonExit;
	WORD			m_wGuildBattleAreaCount,m_awGuildBattleArea[dMAX_GUILD_BATTLE_AREA_COUNT];
	WORD			m_wMonsterActiveAreaCount,m_awMonsterActiveAreaList[dMAX_AREA];
	WORD			m_wDoorCount,m_awDoorList[dMAX_DOOR_IN_MAP];
	WORD			m_wEventAreaCount,m_awEventAreaList[dMAX_EVENT_AREA_IN_MAP];
	WORD			m_wTrapCount,m_awTrapList[dMAX_TRAP_AREA_IN_MAP];
	WORD			m_wArcaCount,m_awArcaList[dMAX_ARCA_AREA_IN_MAP];

					cArea();
					~cArea();

	void			reset();					//	초기화한다.
	BOOL			build(cFIELD *_lpField);	//	영역 정보를 설정한다(포탈수가 0이면 FALSE를 리턴한다.)

	int				findAreaIndexBySerial(int _iSerial);//시리얼을 기준으로 영역 인덱스를 찾아낸다.
	void			operate1SecondWork();
	void			operate1MinuteWork();

	cAreaTrapInfo*	getFieldTrapBySerial(int _iTrapSerial);
	cAreaTrapInfo*	getFieldTrap(int _iTrapIndex);
	cAreaTrapInfo*	getActiveTrap(int _iX,int _iY);
	cAreaInfo*		getOXQuizOArea();	//	OX 퀴즈의 O 영역
	cAreaInfo*		getOXQuizXArea();	//	OX 퀴즈의 X 영역
	cAreaInfo*		getGuildBattleArea(int _iIndex);	//	길드전 영역 리턴~~
	cAreaInfo*		getFirstPortal();			//	아무튼 첫번째 출입구-_-를 찾는다.
	cAreaInfo*		getGuildHallGate();
	cAreaInfo*		getStartArea();				//	시작 영역
	cAreaInfo*		getFocusArea(int _iX,int _iY);
	cAreaInfo*		getDoorByGeneratePeriod(int _iPeriod,int _iIndex);
	cAreaInfo*		getPortal(int _iIndex)
	{
		if (_iIndex<0 || _iIndex>=dMAX_AREA)
			return	NULL;

		if (m_pArea[_iIndex] == NULL || m_pArea[_iIndex]->m_wSerial	==	0xffff)
			return	NULL;
		if (!m_pArea[_iIndex]->isPortal()			)
			return	NULL;

		return	m_pArea[_iIndex];
	}
	cAreaInfo*		getArea(int _iIndex,BOOL _bIsAlloc=FALSE,int _iKind=0);
};

#endif