#ifndef	_areaDefine_h
#define	_areaDefine_h

//
//	Door Area Satus
enum
{
	eDAS_HIDE,
	eDAS_UNLOCK,
	eDAS_CLOSE,
	eDAS_OPEN,
};

//
//	Gate Area Shape
enum
{
	eGAS_NORMAL,
	eGAS_ENTER_BUILDING,
	eGAS_ONE_WAY,
	eGAS_HIDE,
	eGAS_INNER_PORTAL,
	eGAS_HIDE_INNER_PORTAL,
};

enum
{
	ePAS_NORMAL,
	ePAS_BUILDING_GATE,
	ePAS_ONE_WAY_GATE,
	ePAS_HIDDEN,
	ePAS_INNER_PORTAL,
	ePAS_HIDDEN_INNER_PORTAL,
	ePAS_ENTER_GUILD_HALL,
	ePAS_LEAVE_GUILD_HALL,
	ePAS_INNER_DUNGEON,	// insu add
	ePAS_INNER_TOWER,	// insu add
	ePAS_INNER_BUILDING,	// insu add
};

#define	dAREA_NORMAL				0
#define	dAREA_NOTICE_BOARD			1
#define	dAREA_DOOR					2
#define	dAREA_PORTAL				3
#define	dAREA_MONSTER_ACTIVE		4
#define	dAREA_START_AREA			5
#define	dAREA_NAMED_AREA			6
#define	dAREA_GUILD_BATTLE			7
#define	dAREA_OX_AREA_O				8
#define	dAREA_OX_AREA_X				9
#define	dAREA_REVIVE_AREA			10
#define	dAREA_TRAP					11
#define	dAREA_EVENT_AREA			12
#define	dAREA_ARCA					13
#define	dAREA_GUILD_CREST			14
#define	dAREA_GUILD_STRUCTURE		15
#define	dAREA_SECRET_DUNGEON_EXIT	16
#define dAREA_HUNTING_AREA			17	// insu add

#define	dSIZEOF_AREA_NAME		32

#pragma	pack(2)

enum
{
	eTRM_2_1,
	eTRM_1_1,
	eTRM_1_2,
};

class	cAreaBaseInfo
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

	inline	void*	getBasePoint()	{return	(void *)&m_wSerial;}
};

class	cAreaToolBaseData
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
};

class	cAreaTrapDefine
{
public:
	WORD			m_wTrapLevel;
	WORD			m_wTrapRefreshTime;
	WORD			m_wDetectTrapLevel,m_wDisarmTrapLevel;
	WORD			m_wTrapSkill1,m_wTrapSkill2;
	WORD			m_wTrap1ActiveChance,m_wTrap2ActiveChance;
	WORD			m_wTrapRange;
	WORD			m_wTrapRangeMethod;
	WORD			m_wTrapTarget;

	inline	void*	getSaveDataPoint()	{return	(void*)&m_wTrapLevel;}
};

class	cAreaTrap	:	public cAreaToolBaseData,public cAreaTrapDefine
{
};

class	cAreaArcaDefine
{
public:
	WORD			m_wLinkArca;

	inline	void*	getSaveDataPoint()	{return	(void*)&m_wLinkArca;}
};

class	cAreaArca	:	public cAreaToolBaseData,public cAreaArcaDefine
{
public:
};

class	cAreaDoorDefine
{
public:
	WORD			m_wRegenPeriod;		//	생성 빈도 (분)	40	40	해당 시각 마다 비밀 문이 생성될지 결정한다.	
	WORD			m_wUpkeepPeriod;	//	지속 시간 (분)	30	30	시간이 지나면 사라진다.	이미 파티에 종속된 비밀 문이 있을 경우 비밀 문의 존재 여부와는 상관없이 해당 파티에게는 탐지 했던 위치에 표시되고 다른 사람들은 볼 수 없다.
	WORD			m_wGenChance;		//	생성 확률 설정 (%)	1	100	비밀 문이 각각 생성될 확률	

	int				m_iDurability;	//	보유 HP
	int				m_iDefense;		//	물리/원소 저항치
	WORD			m_wDestroyMinimumLevel;		//	파괴 가능 최소 레벨

	BYTE			m_bSecretLevel;		//	비밀 레벨
	BYTE			m_bUnlockLevel;	//	자물쇠 해체 레벨
	WORD			m_wUpkeepDismantlingTime;	//	해체 상태 지속 시간
	WORD			m_bf1IsDestroyable	:	1;	//	파괴 가능 여부

	WORD			m_wTrapLevel,m_wTrapTarget;
	BYTE			m_bDetectTrapLevel,m_bDisarmTrapLevel;	//	함정 탐지 레벨,함정 해체 레벨
	WORD			m_wTrapSkill1,m_wTrapSkill2;
	WORD			m_wTrapSkill1ActiveChance,m_wTrapSkill2ActiveChance;
	WORD			m_wLinkSecretDungeon;
	BYTE			m_bDefensePercent;					// 09.08.13  물리/마법 저항력 %

	inline	void*	getSaveDataPoint()	{return	(void*)&m_wRegenPeriod;}
};

class	cAreaDoor	:	public cAreaToolBaseData,public cAreaDoorDefine
{
public:
};
// insu add
class	cAreaHuntingDefine
{
public:
	WORD			m_wTemp;
	DWORD			m_dwIconX;
	DWORD			m_dwIconY;
	DWORD			m_dwLevel_Low;	
	DWORD			m_dwLevel_High;

	inline	void*	getSaveDataPoint()	{return	(void*)&m_wTemp;}
};

class	cAreaHunting : public cAreaToolBaseData, public cAreaHuntingDefine
{
};
// insu add end
#pragma	pack()

#endif