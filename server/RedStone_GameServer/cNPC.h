#ifndef _classNPC_H
#define	_classNPC_H

#include "SFC.H"
#include "CDialog.h"
#include "cArea.h"

#define	dMAX_LINK_MONSTER_COUNT		46
#define	dNAME_LENGTH				18

#define	dMONSTER_GENERATE_SET_POS		0
#define	dMONSTER_GENERATE_ACTIVE_AREA	1
#define dMONSTER_GENERATE_AROUND		2
#ifdef	_IS_TOOL
#define	dSYNC_FPS						64
#else
#define	dSYNC_FPS						16
#endif

//
//	맵에 저장되어 있는 액터 데이터
class	cBASE_NPC_INFO
{
public:
	int					m_iSerial;
	WORD				m_wCharacter;
	WORD				m_wActorKind;

	WORD				m_wDirect;			// 어느쪽을 보고 있을꺼냐?
	WORD				m_wIsChangeDirect;	// 서 있는 동안 방향을 바꿀거냐?
	int					m_iRegenerationCycle;// 리젠 주기

	WORD				m_wMoveArea;	// 행동 영역
	WORD				m_wPatrolRoute;	// 순찰 루트(없으면 행동 반경 안에서 얼레 벌레~)
	WORD				m_wWanderCycle;	// 0 안움직인다. 10단계로 이동 주기 설정
	WORD				m_wBattleInclination;// 전투 성향.

	WORD				m_bf1IsLeader		:	1;	//  리더
	WORD				m_bf1IsGuildCrest	:	1;	//  길드 문장
	WORD				m_bf1IsGuildObject	:	1;	//	길드 오브젝트
	WORD				m_bf1IsHideName		:	1;	//	네임바 숨기기
	WORD				:	0;		//	초기화?

	WORD				m_wWard;		// 보호해야할 인물
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
	
	BOOL				isNotMoveNpc()	{if (m_wMoveArea	==	dMAX_AREA) return	TRUE;return	FALSE;}

};	//	cBASE_NPC_INFO


#define	dMAX_BEATER				40
#define	dPARTY_FLAG				0x8000

#define	dMAX_HITTER				40	//	때린넘
#define	dMAX_NON_TARGET			16
#define	dANGRY_TIME				5
#define	dRESET_BEAT_INFO_PERIOD	(dSYNC_FPS*60*3)

class	cNPC	:	public	cBASE_NPC_INFO	//	몬스터를 포함한 플레이어가 컨트롤 할 수 없는 모든 녀석들의 정보
{
public:
	CConversationContainer	*m_pSpeech;

	WORD				m_wActorSerial;	//	필드에서 시리얼
	int					m_iLastTime;	//	마지막으로 존재 했던 초단위 시간.
	cPOINT				m_posSleep;
	BOOL				m_bIsSleep;
	BOOL				m_bWakeUpNow;

//	때린넘 정보(경험치 분배를 위해 필요)
	WORD				m_wFirstHitter,m_wFirstBeater;
	WORD				m_aBeater[dMAX_BEATER];
	DWORD				m_aDamage[dMAX_BEATER+1];
	int					m_aLastBeatTime[dMAX_BEATER];
	int					m_dwLastBeatTime;

//	때린넘 정보 반격을 위해 필요
	WORD				m_wHitterMaxLevel;
	WORD				m_aHitter[dMAX_HITTER];
	DWORD				m_aPersonalDamageSum[dMAX_HITTER];	//	캐릭터 한명 한명의 대미지 합
	int					m_aLastHitTime[dMAX_HITTER];

	char				m_strKiller[dNAME_LENGTH];
	WORD				m_wKilledBySameCharacterCount,m_wBrithCount,m_wBeaterCount;
	WORD				m_wBoostPower;
	WORD				m_awNonTarget[dMAX_NON_TARGET][2],m_wNonTargetCount;

						cNPC()
						{
							m_wKilledBySameCharacterCount	=	0;
							m_wBrithCount					=	0;
							m_strKiller[0]					=	'a';
							m_strKiller[1]					=	0;
							m_iSerial						=	0xffffffff;
							m_iLastTime						=	-0x7ffffff;
							m_pSpeech						=	NULL;
							m_wBoostPower					=	100;
							m_wBeaterCount					=	0;

							reset();
						}

						~cNPC()
						{
							KILL(m_pSpeech);
						}

//	리셋
	void				reset(BOOL _bIsSleep=FALSE);
	void				outBeater(int _iActorSerial,int _iPartySerial,BOOL _isPartyLeave);//언넘이 나간다.
	void				operateBreakParty(int _iPartySerial);//언 파티가 해체된다.
	void				killedBy(char *_strKillerName);//누군가 한테 죽었다. -o-!!
	void				calcMerit();//공훈 계산.. 죽었을때만 쓰자..
	BOOL				isNonTarget(int _iSerial);
	void				addNonTarget(int _iSerial,int _iTime);
	void				updateNonTarget();

	int					getMaxMerit();	//최대 공훈 계산
	int					getMerit(int _iIndex){return m_aDamage[_iIndex];}//	공헌도 리턴
	int					getSlugger();	//가장 큰 데미지를 많이 입힌넘 구하기..
	int					getWinningHitter();//가장 데미지를 많이 입힌넘 구하기..

	void				addDamageBySolo(int _iLevel,int _iSerial,int _iDamage,int _iFrame);//솔로가 때렸다!!
	void				addDamageByParty(int _iLevel,int _iPartySerial,int _iActorSerial,int _iDamage,int _iFrame);//파티가 때렸다!!
	void				addHitterDamage(int _iLevel,int _iSerial,int _iDamage,int _iFrame);

	BOOL				updateBeatInfo(int _iFrame);
	void				wakeUpNow(int _iX,int _iY);

	inline	BOOL		isGenerateTime(int _iSecond)//리젠 시간~~(초단위)
	{
		if	(m_wActorSerial!=	0xffff	)
			return	FALSE;

		if	(m_bWakeUpNow)
			return	TRUE;
		
		if	(m_wIsBlockToAutoRegen)
			return	FALSE;

		if	(m_bIsSleep)
			return	TRUE;

		if	(m_iLastTime+m_iRegenerationCycle	<=	_iSecond	)
			return	TRUE;

		return	FALSE;
	}

	BOOL				setFirstBeater(int _iSerial);
	BOOL				isOwnedShop(int _iSerial);	//	이런 시리얼이 붙어 있는 상점을 가지고 있냐?(대화 중에..)
	void				checkBeginBattleEvent(cACTOR *_lpOwner,cACTOR *_lpTarget);
	void				checkKilledByPlayerEvent(cACTOR *_lpOwner,cACTOR *_lpKiller);
	void				checkKillPlayerEvent(cACTOR *_lpOwner,cACTOR *_lpTarget);
	void				operateAlwaysEvent(cFIELD *_lpField);
};

#endif