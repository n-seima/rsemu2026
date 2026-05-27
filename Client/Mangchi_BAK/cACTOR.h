#ifndef	_classACTOR_H
#define	_classACTOR_H

#include	"SFC.H"
#include	"cJOB.H"
#include	"cSKILL.H"
#include	"cITEM.H"
#include	"cCHARACTER.H"
#include	"CDialog.H"

#define	dTEAM_PLAYER					0		//	Player 1
#define	dTEAM_NPC						1		//	NPC
#define	dTEAM_MONSTER					2		//	Monster

#define	dACTOR_BODY_SIZE_CORRECT		8

#define	dBORDER_ACTOR					10000		//	소트할때 경계
#define	dMAX_LINK_MONSTER_COUNT			46


#define dLINK_NULL						0
#define	dTWO_WAY_LINK					1
#define	dONE_WAY_LINK					2

#define	dPOWER_VALUE					0
#define	dPOWER_PERCENTAGE				1

//
//	Actor Basic Power
enum
{
	eABP_LEVEL				,
	eABP_STRENGTH			,
	eABP_DEXTERITY			,
	eABP_CONSTITUTION		,
	eABP_WISDOM				,
	eABP_INTELLIGENCE		,
	eABP_CHARISMA			,
	eABP_LUCK				,
	eABP_ATTACK				,
	eABP_DEFENSE			,
	eABP_FIRE_RESIST		,
	eABP_WATER_RESIST		,
	eABP_WIND_RESIST		,
	eABP_EARTH_RESIST		,
	eABP_LIGHT_RESIST		,
	eABP_DARK_RESIST		,
	eABP_MAGIC_RESIST		,
};

#define	dMAX_ACTOR_COUNT				1024

#define	dDIRECT_NORTH					0
#define	dDIRECT_NORTH_EAST				1
#define	dDIRECT_EAST					2
#define	dDIRECT_SOUTH_EAST				3
#define	dDIRECT_SOUTH					4
#define	dDIRECT_SOUTH_WEST				5
#define	dDIRECT_WEST					6
#define	dDIRECT_NORTH_WEST				7
#define	dDIRECT_RANDOM					8

class	cPOWER_INFO		//	캐릭터 능력치의 정보
{
public:
	int					m_iType;
	int					m_iValueType;
	char				m_strName[32];
	DWORD				m_dwLowValue;
	DWORD				m_dwHighValue;
};

#define	dBI_PASSIVE_SOLO			0	// 성향. 0 맞으면 때린다. 주위에 누군가 맞고 있던 어쨌던 신경 안쓴다.
#define	dBI_PASSIVE_AGGRESSIVE		1	//       1 맞으면 때린다. 주위에 누군가 맞고 있으면 덤빈다.(링크 안되어 있어도/상대가 어떻든..)
#define	dBI_AGGRESSIVE_SOLO			2	//       3 근처에 오면 공격한다. 주위에 누군가 맞고 있던 어쨌던 신경 안쓴다.
#define	dBI_AGGRESSIVE_AGGRESSIVE	3	//       4 근처에 오면 공격한다. 주위에 누군가 맞고 있으면 덤빈다.(링크 안되어 있어도/상대가 어떻든..)

#define	dMONSTER_GENERATE_SET_POS		0
#define	dMONSTER_GENERATE_ACTIVE_AREA	1
#define dMONSTER_GENERATE_AROUND		2	// insu add
class	cACTOR_SAVE_DATA
{
public:
	int					m_iSerial;
	WORD				m_wCharacter;
	WORD				m_wType;

	WORD				m_wDirect;			// 어느쪽을 보고 있을꺼냐?
	WORD				m_isChangeDirect;	// 서 있는 동안 방향을 바꿀거냐?
	DWORD				m_dwRegenerationCycle;// 리젠 주기

	WORD				m_wMoveArea;	// 행동 영역
	WORD				m_wPatrolRoute;	// 순찰 루트(없으면 행동 반경 안에서 얼레 벌레~)
	WORD				m_wWanderCycle;	// 0 안움직인다. 10단계로 이동 주기 설정
	WORD				m_wBattleInclination;// 전투 성향.

	WORD				m_bf1IsLeader		:	1;	//	리더
	WORD				m_bf1IsGuildCrest	:	1;	//	길드 문장
	WORD				m_bf1IsGuildObject	:	1;	//	길드 문장
	WORD				m_bf1IsHideName		:	1;	//	네임바 숨기기
	WORD				:	0;		//	초기화?

	WORD				m_isWard;		// 보호해야할 인물
	WORD				m_wLinkMonsterCount;
	WORD				m_wLinkType;	//	쌍방향/단방향 링크
	WORD				m_awLinkMonster[dMAX_LINK_MONSTER_COUNT];//	링크되어 있는 몹
	WORD				m_wGenerateAroundPosX1;
	WORD				m_wGenerateAroundPosY1;
	WORD				m_wGenerateAroundPosX2;
	WORD				m_wGenerateAroundPosY2;
	WORD				m_wGenerateMethod;

	cPOINT				m_pos;			// 위치

	char				m_strName[30];
	WORD				m_wIsBlockToAutoRegen;
};


class	cACTOR	:	public cACTOR_SAVE_DATA
{		// 마지막 수정일 : 09.10.30
public:
	CConversationContainer	*m_pCC;		//	대화 컨테이너

	WORD					m_wJob;
	BOOL					m_isSelected;

							cACTOR()
							{
								m_pCC	=	NULL;

								reset();
							}
							~cACTOR();

	inline	int				getClassSize()	{return sizeof(cACTOR)-6;}	//	클래스 사이즈
	inline	void			reset()
	{
		KILL(m_pCC);

		memset(this,0,sizeof(cACTOR_SAVE_DATA));

		m_iSerial			=	0xffff;

		m_bf1IsLeader		=	FALSE;
		m_bf1IsGuildCrest	=	FALSE;
		m_bf1IsGuildObject	=	FALSE;
		m_isWard			=	FALSE;
		m_wLinkType			=	dLINK_NULL;
		m_wLinkMonsterCount	=	0;
		memset(m_awLinkMonster,0xff,sizeof(m_awLinkMonster));//	링크되어 있는 몹

		m_wDirect			=	dDIRECT_RANDOM;		// 어느쪽을 보고 있을꺼냐?
		m_isChangeDirect	=	TRUE;	// 서 있는 동안 방향을 바꿀거냐?
		m_dwRegenerationCycle=	60*2;	// 리젠 주기

		m_wMoveArea			=	0;	// 행동 반경
		m_wPatrolRoute		=	0xffff;	// 순찰 루트(없으면 행동 반경 안에서 얼레 벌레~)
		m_wWanderCycle		=	30;	// 0 안움직인다. 10단계로 이동 주기 설정

		m_wBattleInclination=	dBI_PASSIVE_SOLO;// 전투 성향.
		m_wGenerateMethod	=	dMONSTER_GENERATE_ACTIVE_AREA;

		m_isSelected		=	FALSE;
	}

	void					setConversation(CConversationContainer	*_pCc);
	inline	BOOL			isHero()		{if(getBody()<dJOB_NPC	)	return	TRUE;return	FALSE;}
	inline	BOOL			isCharacter()	{if(getBody()>=dJOB_NPC	)	return	TRUE;return	FALSE;}

	void					deepCopy(cACTOR* _pActor);						// 09.07.29   깊은 복사.
	void					setLinkNpc(int *_lpLinkList,int _iCount);	//	링크 설정
	void					setLeader(BOOL _isLeader);	//	리더로 설정
	void					setWard(BOOL _isWard);		//	요인으로 설정

	void					put(int dx,int dy);
	void					putName(int dx,int dy);
	void					putScale(int dx,int dy,int _iTBSize,int _iScale);//화면에 찍어준다.

	BOOL					isCrash(int _iX1,int _iY1,int _iX2,int _iY2);
	BOOL					isMisplaced();

	BOOL					isTouchedRange(int x,int y,int _iRange=0);

	void					removeLinkMonster(int _iIndex);	//	링크된 몬스터 리스트에서 _iIndex를 제거

	cJOB*					getJob()	{return	g_denJob.getJob(m_wJob);}
	cCHARACTER*				getCharacter();
	char*					getName()	{return	m_strName;}
	inline	int				getBody()	{return	getJob()->m_wBody;}
	inline	int				getBodySize(){return	g_pBody[getBody()]->m_iCrashSize;}
	inline	int				getXpos()	{return m_pos.x;	}	//----------------	새로 만듬 추가
	inline	int				getYpos()	{return m_pos.y;	}//	----------------	새로 만듬 추가	
};

class	cACTOR_SET
{		// 마지막 수정일 : 09.10.30
public:
	int			m_iCount;
	int			m_iCurrentMissplaceActor;

	cACTOR		m_aActor[dMAX_ACTOR_COUNT];
	int			m_aSelectedActor[dMAX_ACTOR_COUNT+1];
	int			m_iSelectedActorCount;

				cACTOR_SET();

	void		reset();

	int			getValidIndex();

	int			add(cACTOR *_lpActor);
	void		changeName(WORD _wCharacterSerial,char *_lpstrName);

	BOOL		remove(int	index);
	BOOL		removeHero(int	job);
	void		removeAllNpc();						//	-------------	새로 만듬 추가
	void		removeAllMonster();	// 09.11.23 추가
	cACTOR		*getActor(int index);
	cACTOR		*getSpeaker(int _iIndex);	//	스피커들 중에 _iIndex번째 녀석을 찾는다.
	int			getSpeakerIndex(int _iIndex);


	BOOL		moveSelectActor(int _iDx,int _iDy);
	int			getFreeIndex();
	int			getFreeCharacterIndex();
	int			getActors(cINDEX *indexes,cRECT *viewport);	//	뷰포트 안에 캐릭터들을 구한다.
	void		linkSelectedMonster();						//	선택된 몬스터들을 서로 링크 시킨다.
	void		setLinkType(int _iIndex,int _iType);	//	링크 방법
	void		removeSelectedMonstersLink();				//	선택된 몬스터들의 링크를 제거한다.
	void		removeThisActorAtLinkMonsterList(int _iIndex);// _iIndex를 링크하고 있는 것-_-들의 리스트에서 _iIndex를 제거
	void		freeSelect();
	void		select(cRECT *rect,cRECT *screenRect);
	void		selectAll();
	void		selectSameKind();
	void		removeAll();
	void		removeSelectedActors();
	int			getCrashActor(int x,int y,int checkSize=0xffff);
	BOOL		isCrash(int x,int y,int _iBodySize=0xffff);

	int			getSelectedActor();	//	선택되어 있는 액터들(0번에 카운트)
	int			getCorrectCount();				//	제대로 찍혀 있는 Actor의 수
	int			getCorrectHeroCount();			//	제대로 찍혀 있는 영웅의 수
	int			getCorrectCharacterCount();		//	제대로 찍혀 있는 캐릭터의 수

	void		setLeader(int _iIndex,BOOL _isLeader=TRUE);			//	리더 설정
	void		setWard(int _iIndex,BOOL _isWard=TRUE);			//	요인 설정

	int			getCharacterCount(int _iCharacter);	//	_iCharacter인 액터의 수 리턴

	BOOL		reBuild();						//	지워진 캐릭터 
	void			drawMonsterAreaInfo(int _index, cRECT *_rect);
	inline BOOL	isAvail(int index)		{if (m_aActor[index].m_iSerial	==	0xffff)	return	FALSE;return	TRUE;}
	inline BOOL	isHero(int index)		{return	m_aActor[index].isHero();}
	inline BOOL	isCharacter(int index)	{return	m_aActor[index].isCharacter();}
};


class	cGROUP
{
public:
	int					m_iSerial;

	WORD				m_wLeader;						//	리더
	WORD				m_wCount;						//	그룹원의 수
	WORD				m_wAI;							//	그룹의 AI
	WORD				m_wFormation;					//	대형
};

extern	cPOWER_INFO	g_aPowers[50];
extern	cPOWER_INFO	g_aLimitedPowers[50];

extern	char		*g_strBattleInclination[];
extern	char		*g_strMonsterGrade[];
extern	char		*g_strLinkType[];


#endif