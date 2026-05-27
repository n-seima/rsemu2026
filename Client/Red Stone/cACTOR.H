/*************************************************************************************************
	CActor.H

	Red Stone 캐릭터 관리 클래스

	작성  날짜 : 2000.12.24(... 그래!! 크리스마스 이브다!! 크리스마시 이브에 일하고 있다!! 떫냐? 크흑...
	작  성  자 : 이영찬
	개      요 : 모든 캐릭터,몬스터,NPC의 기본이 되는 클래스다. 당근 무쟈게 중요하다.

*************************************************************************************************/

#ifndef _classACTOR_H
#define	_classACTOR_H

#include "cNEMO.H"
#include "cJOB.H"
#include "cItem.H"
#include "CGamePlay.H"
#include "cACTION.H"
#include "CSkill.H"
#include "CImageData.H"
#include "cMAP.H"
#include "CHitEffect.H"
#include "cPACKET.H"
#include "CHero.H"
#include "CAgent.H"
#include "tdutil.h"
#include "tdspring_motion.h"
#include "screen_focus.h"

class	CActor;

//
//	Charge Animation Effect
enum
{
	eCAE_BEGIN,
	eCAE_MAIN_LOOP,
	eCAE_END,
};

//
//	Sync Info with Server
enum
{
	eSIS_ACTIVE_ACTOR,
	eSIS_SLEEP,
	eSIS_REMOVED,
};

#define	dLOOP_INFINITY								3	//	무한 루프.. -o-

#define	dWAIT_ACTOR_INFO_PERIOD						dSYNC_FPS

#define	dMAX_ENCHANTED_MAGIC_COUNT_FOR_CHARACTER	10

#define	dFORECAST_VISIBLE_ACTOR_WIDTH				600
#define	dFORECAST_VISIBLE_ACTOR_HEIGHT				400

#define	dBLOCKER_PUT_DISTANCE					40	//	블럭커 출력 거리

#define	dALIVE_DUMMY							0xfffe
#define	dDISAPEAR_DUMMY							dSYNC_FPS

#define	dEVENT_RANGE_WITH_MERCHANT				300		//	상인과 이벤트 가능 거리(이를 테면 대화나 물품 구입)
#define	dEVENT_RANGE_WITH_NPC					100		//	NPC와 이벤트 가능 거리(이를 테면 대화나 물품 구입)
#define	dRESET_RANGE_BY_MOVE_PACKET				(200*200)
#define	dREFIT_RANGE_BY_MOVE_PACKET				(50*50)
#define	dMINIMUM_HIT_RATE						5		//	최소 명중률
#define	dMAXIMUM_HIT_RATE						95		//	최고 명중률

#define	dPERIOD_OF_ASK_ACTOR_INFO_WHEN_FOCUS	(dSYNC_FPS*2)	//	포커스일때 액터 정보 요청하는 주기
#define	dREMAIN_CORPSE_TIME						(dSYNC_FPS*5)	//	5초
#define	dREMAIN_CORPSE_TIME_FOR_SUMMON_BEAST	(dSYNC_FPS*28)	//	5초
#define	dSEND_MISPLACED_ACTOR_LIST_PERIOD		(dSYNC_FPS/2)
 
#define	dRANGE_OF_CORRECT_ACTOR_POSITION		256		//	16픽셸정도 차이.
#define	dRANGE_OF_RESET_ACTOR_POSITION			1024	//	타일 하나 정도 차이난다.
#define	dMAX_PATH								40
#define	dMAX_LINK_MONSTER_COUNT					46

#define	dSTATUS_MOVE							0
#define	dSTATUS_ATTACK							1
#define	dSTATUS_AI								2
#define	dSTATUS_ARRANGE							3

#define	dMAX_ENEMY								128		//	최대 적 캐릭터 수
#define	dMAX_OWNER								128		//	최대 플레이어 캐릭터 수
#define	dHIT_EFFECT_COUNT						5

#define	dMAX_CHARACTER							128		//	최대 캐릭터 수(몬스터 포함)
#define	dMAX_PET								8		//	최대 소환수 수(데리고 다니는 애들)
#define	dMAX_PLAYER								8
#define	dMAX_ACTOR								2048

#define	dTEAM_PLAYER							0		//	플레이어
#define	dTEAM_NPC								1		//	NPC
#define	dTEAM_MONSTER							2		//	몬스터

//GGG 이펙트 타입		//비트 마스크로 쓴다.
#define dGGG_EFFECT_PLAYTYPE_NONE				0x0000
#define dGGG_EFFECT_PLAYTYPE_POINT_UP			0x0001
#define dGGG_EFFECT_PLAYTYPE_HIT				0x0002
#define dGGG_EFFECT_PLAYTYPE_HIT_BY				0x0004

#define dGGG_HIT_EFFECT_INDEX					544		
#define dGGG_STRUCK_EFFECT_INDEX				550		

enum{
	eGGG_ATTRIBUTE_NONE,
		eGGG_ATTRIBUTE_FIRE,
		eGGG_ATTRIBUTE_WATER,
		eGGG_ATTRIBUTE_WIND,
		eGGG_ATTRIBUTE_EARTH,
		eGGG_ATTRIBUTE_END
};

enum{
	eINCRECE_VALUE1_ACITECHANCE,	//발동 확률. * 100 
		eINCRECE_VALUE1,				//효과 수치 * 100 
		eINCRECE_VALUE1_EFFECT,			//효과 EX>불 : 데미지 비례 추가 공격
		eINCRECE_VALUE2_ACITECHANCE,	//발동 확률. * 100 
		eINCRECE_VALUE2,				//효과 수치  * 100
		eINCRECE_VALUE2_EFFECT,			//효과
		eINCRECE_VALUE_ETC,				//기타 등등 효과...
		eINCRECE_END
};

#define dGGG_EFFECT_NONE					0x0000
#define dGGG_EFFECT_CP_MAX_SELF				0x0001
#define dGGG_EFFECT_RETURN_FIRE_DAMAGE		0x0002
#define dGGG_EFFECT_DECREASE_MOVE_SPEED		0x0004
#define dGGG_EFFECT_DECREASE_ATTACK_SPEED	0x0008
#define dGGG_EFFECT_STUN					0x0010
#define dGGG_EFFECT_DECREASE_DEFENCE_POWER	0x0020
#define dGGG_EFFECT_ADD_DAMAGE				0x0040
#define dGGG_EFFECT_IGNORE_ONE_ATTACK_SELF	0x0080
#define dGGG_EFFECT_INCREASE_SPEED_SELF		0x0100
#define dGGG_EFFECT_RETURN_DAMAGE			0x0200

#define dAMEM_BLIND_SIZE_OLD				0
#define dAMEM_BLIND_SIZE_NEW_SMALL			1
#define dAMEM_BLIND_SIZE_NEW_MIDDLE			2
#define dAMEM_BLIND_SIZE_NEW_LARGE			3
#define dAMEM_BLIND_SIZE_ERROR				4

//
//	Actor Kind
enum
{
	eAK_PLAYER,
	eAK_NPC,
	eAK_MONSTER,
};

#define	dCLOSE_AFTER_IMAGE			0
#define	dFADE_AFTER_IMAGE			1
#define	dPUT_AFTER_IMAGE			2
#define	dPUT_AFTER_IMAGE_MANUAL		3

#define	dAFTER_IMAGE_COUNT			12

#define	dLOCK_ON_LOCATE				0x7fff

#define	dMONSTER_GRADE_UNDERING		0	//	최하위 스킬은... 하나 밖에 없고.. 하는 짓도 단순하다.
#define	dMONSTER_GRADE_THROWER		1	//	기본적으로 근접 공격 스킬이 하나 있고, 장거리 공격 스킬이 있다.
#define	dMONSTER_GRADE_MAGIC_USER	2	//	기본적으로 근접 공격 스킬이 하나 있고, 마법이 몇개 있다.
#define	dMONSTER_GRADE_MIDDLE_BOSS	3	//	중간 보스.. 다..
#define	dMONSTER_GRADE_BOSS			4	//	보스급이다.


#define	dPLAYER_UPDATE_TIME			8	//	1초에 4번
#define	dMONSTER_UPDATE_TIME		16	//	1초에 2번

#define	dDEATH_REALLY	1	//	진짜로 죽었어 -_-;;
#define	dDEATH_FAINTING	2	//	기절한거다.

#define	dHIT_EFFECT_COUNT_ON_CHARACTER	20

class	CActorSimpleArmorInfo
{
public:
	WORD			m_wBaseItem;
	WORD			m_wColorizeEffect;

	cBasicItem*		getBasicItem()
	{
		if (m_wBaseItem	==	0xffff)	return	NULL;

		return	&g_aBasicItem[m_wBaseItem];
	}

	void			reset()
	{
		m_wBaseItem			=	0xffff;
		m_wColorizeEffect	=	0xffff;
	}

	void			copy(cItem *_lpItem,int _iItemKind=0xffff)	//	_iItemKind 0 무기 ,1 방패 , 2 갑옷
	{
		reset();

		if (!_lpItem)
			return;

		m_wBaseItem			=	_lpItem->m_wBaseItem;

		if (_iItemKind	==	0)
			m_wColorizeEffect	=	_lpItem->getWeaponColorizeEffect();
	}
};

class	cACTOR_COMMON_DATA
{
public:
	WORD			m_wSerial;						//	액터 슬롯
	char			m_strName[dNAME_LENGTH];		//	이름
	WORD			m_wTeam,m_wActorKind;			//	팀
	WORD			m_wJob;							//	직업
	WORD			m_wLevel;						//	레벨

	CActorSimpleArmorInfo	m_itemWeapon,m_itemShield,m_itemArmor;		//	장비하고 있는 무기,장비하고 있는 방패
};

class	CAfterImageInfo
{
public:
	CPos			m_pos;
	BYTE			m_bAnm;
	BYTE			m_bDirect;
	BYTE			m_bFrame;
	BYTE			m_bAlphaDepth;
};

enum
{
	eSSWS_COMPLETE,
	eSSWS_WAIT_ACTOR_INFO,
	eSSWS_WAIT_MINIMUM_ACTOR_INFO,
	eSSWS_WAIT_VERY_SIMPLE_ACTOR_INFO,

	eSSWS_WANT_SYNC,	//	바로 싱크 시키길 원한다.
	eSSWS_VALID_ACTOR,
	eSSWS_OUT_OF_SIGHT,
};

//
//	Rush Status
enum
{
	eRS_NONE,		//	러쉬하고 있지 않다.
	eRS_READY,		//	걍 러쉬한다.
	eRS_READY_FOR_WHIRL_RUNNING_TYPE_RUSH,		//	걍 러쉬한다.
	eRS_WAIT_TRIGGER,		//	걍 러쉬한다.
	eRS_RUSH,
	eRS_WHIRL_RUNNING_TYPE_RUSH,
	eRS_ROLLING_LOG,//	데굴데굴 통나무
	eRS_SPIN,		//	삥글 삥글 돌면서 러쉬한다.
	eRS_FINISH,		//	마무리 동작 중이다.
	eRS_JUMP,		//	점프..
	eRS_JUMP_RUSH,		// 점프 러쉬 이다.
	eRS_JUMP_RUSH_READY,// 점프러쉬 준비..

};
//
//	Jump Status
enum
{
	eJS_NONE,		//	점프하고 있지 않다.
	eJS_READY,		//	준비
	eJS_WAIT_TRIGGER,	//	트리거 기다림
	eJS_JUMP,		//	점프!!
	eJS_FINISH		//	점프 동작 중이다.
};

//
//	Special Action Skill
enum
{
	eSAS_NONE,				//	러쉬하고 있지 않다.
	eSAS_SCIMATAR_CUTTING,	//	걍 러쉬한다.
	eSAS_RUSH,				//	걍 러쉬한다.
	eSAS_JUMP,				//	점프
	eSAS_BOUNCING_LINEAR,	//	바운싱 리니어
	eSAS_BIG_BULLET,
};

class	CActroAnmInfo
{
public:
	WORD	m_wFrameCounter;
	WORD	m_wMaxFrame;
	WORD	m_wFPS;
	WORD	m_wAnm;
	WORD	m_wDirect;
	WORD	m_wFrame;
	WORD	m_wAlphaDepth;
	WORD	m_bf1IsDamageFrame  : 1;
	WORD	m_bf1IsTriggerFrame : 1;
	WORD	m_bf14Body			: 14;

	void	reset();
	void	init(CActor *_lpActor,int _iAnm,int _iAlphaDepth);
	BOOL	updateFrame();
};

class	CBitInfo
{
public:

	WORD	m_wTargetSerial;
	DWORD	m_dwNameHashCode;
	CPos	m_pos;
	WORD	m_wState;
	WORD	m_wAngle;
	void	reset()
	{
			m_wTargetSerial		= 0xffff;
			m_dwNameHashCode	= 0xffffffff;
			m_pos.x				= 0;
			m_pos.y				= 0;
			m_wState			= 0;
			m_wAngle			= 0;

	}
	void	set(WORD _wSerialInField, DWORD _dwNameHashCode,CPos _pos, WORD	_wAngle)
	{

			m_wTargetSerial		= _wSerialInField;
			m_dwNameHashCode	= _dwNameHashCode;
			m_pos				= _pos;
			m_wAngle			= _wAngle;
	}

	BOOL	isEqual(WORD _wSerialInField, DWORD _dwNameHashCode)
	{
		if(m_wTargetSerial == _wSerialInField && m_dwNameHashCode == _dwNameHashCode)
			return TRUE;

		return FALSE;
	}
	
	
};


class	CBitInfoManager	
{
public:

	CBitInfo	m_bitInfo[dMAX_BIT_STICK_ACTOR];				// 비트 정보
	int			m_iCount;

	void	reset()
	{
		m_iCount	=	0;

		for(int i=0;i<dMAX_BIT_STICK_ACTOR;++i)
		{
			m_bitInfo[i].reset();
		}
	}

	BOOL	addBit(WORD _wSerialInField, DWORD _dwNameHashCode,CPos _pos, WORD _wAngle)
	{
	
		if(m_iCount >= dMAX_BIT_STICK_ACTOR)
			return	FALSE;
		
		m_bitInfo[m_iCount].set(_wSerialInField,_dwNameHashCode,_pos,_wAngle);
		++m_iCount;

		return TRUE;
		
	}

	void	removeBit(WORD _wSerialInField, DWORD _dwNameHashCode, int _iCount)
	{
		int iCount	=	0;
		for(int i=0;i<m_iCount;++i)
		{
			if(m_bitInfo[i].isEqual( _wSerialInField, _dwNameHashCode))
			{
				removeBit(i);
				--i;
				++iCount;
				if(iCount>=_iCount)
					break;
			}
		}
	}

	void	removeBit(WORD _wIndex)
	{
		memcpy(&m_bitInfo[_wIndex], &m_bitInfo[m_iCount-1], sizeof(CBitInfo));
		m_bitInfo[m_iCount-1].reset();
		--m_iCount;
	}

	
	int		getCount(WORD _wSerialInField, DWORD _dwNameHashCode)
	{
		int	iCount	=	0;
		for(int i=0;i<m_iCount;++i)
		{
			if(m_bitInfo[i].isEqual( _wSerialInField, _dwNameHashCode))
			{
				++iCount;
			}
			
		}
		return iCount;
	}

};


class	cACTOR_DATA	:	public cACTOR_COMMON_DATA
{
public:
	enum
	{
		eMaxChanedDamageCount	=	20,
	};

	class	cChanedDamageInfo
	{
	public:
		WORD		m_wCount;
		WORD		m_wCurrentCount;
		WORD		m_wGap,m_wSkill;
		WORD		m_wCaster;
		DWORD		m_dwLastDamagedFrame;

		CHitInfo	m_aInfo[eMaxChanedDamageCount];

		BOOL		isValid()
		{
			if	(m_wCount	==	0)
				return	TRUE;

			return	FALSE;
		}

		void	reset()
		{
			m_wCount	=	0;
		}
	};

	class	cChanedDamageInfoStack
	{
	public:
		cChanedDamageInfo	m_aData[eMaxChanedDamageCount];

		void				reset();
		cChanedDamageInfo*	add(int _iSkill,int _iShotCount,int _iGap,int _iCaster);
	};

	char			m_strTalk[128];
	WORD			m_wTalkTime;
	WORD			m_wIsWarningMessage;
	BOOL			m_bIsTransOther;
	WORD			m_wIsReceivedBasicData;
	WORD			m_wIsFixedActor;			//	위치가 고정된 캐릭터
	WORD			m_wIsHideShield;			//	방패가 안보이는 상태
	WORD			m_wHorizonScale,m_wVerticalScale;

////////////////////////////////////////////////////////////
//	서버와 싱크 관련
////////////////////////////////////////////////////////////
	WORD			m_wOldSyncStatusWithServer,m_wSyncStatusWithServer;
	WORD			m_wWaitActorInfoTime;		//	재 요청까지 남은 시간

////////////////////////////////////////////////////////////
//	기본 정보
////////////////////////////////////////////////////////////
	int				m_iFirstAidHP;
	int				m_iHP,m_iMaxHP;				//	HP
	WORD			m_wRealTeam;				//	진짜 팀(유혹을 당했던 어쨌던..)
	char			m_isRunning;				//	뛰는 중이다.
	char			m_isSkillControl;			//	기술쪽에서 캐릭터의 움직임을 컨트롤하는 상태
	WORD			m_wBody,m_wBodyVariation;		//	몸뚱아리
	WORD			m_wTrueJob;					//	몸뚱아리
	WORD			m_wGuildSerial;

//	죽음에 관련된 상태
	WORD			m_wCorpseTime;				//	일정 시간이 지나면 시체가 사라진다.
	WORD			m_wWhiteFadeStep;
	WORD			m_wFadeInStep;
	WORD			m_wNpcActorSerial;

////////////////////////////////////////////////////////////
//	펫 관련 정보
////////////////////////////////////////////////////////////
	WORD			m_wGuildPetType,m_wMiniPetType,m_wMiniPetType2;
	WORD			m_wIsPet,m_wIsRideTamer,m_wIsReleaseSummonBeast;					//	펫이다.
	WORD			m_wTamer,m_wCommand;
	WORD			m_wPetWaitCommand;
	WORD			m_wRideDog;
	char			m_strTamerName[dNAME_LENGTH];
	CPos			m_posPet,m_posPet2;
////////////////////////////////////////////////////////////
//	변신 정보
////////////////////////////////////////////////////////////
	CActorSimpleArmorInfo	m_itemTransformationWeapon,m_itemTransformationShield,m_itemTransformationArmor;		//	장비하고 있는 무기,장비하고 있는 방패
	WORD			m_wTransformationJob;		//	변신할 직업
	WORD			m_wRollingLogImage;


////////////////////////////////////////////////////////////
//	여러가지 상태
////////////////////////////////////////////////////////////
	short			m_sConfuseTime;				//	혼란 상태
	WORD			m_wIsDontHitAction;
	WORD			m_wMagicCarpetType;		//	매직 카펫 라이더
	WORD			m_wDuelBuffType;
	WORD			m_wGGGBuffType;
	WORD			m_wPitchmanShopSerial;
	WORD			m_wShutInMagicBoxImage;
	WORD			m_wReleaseMagicBoxAlpha;
	WORD			m_wCrushMagicBoxFrame;

	DWORD			m_bf1IsQuizEventLoser	:	1;
	DWORD			m_bf1IsQuizEventWinner	:	1;
	DWORD			m_bf1IsHide				:	1;
	DWORD			m_bf1IsPlayShutInMagicBoxAnm	:	1;
	DWORD			m_bf1IsTalker			:	1;	//	대화 NPC?
	DWORD			m_bf1IsCopyActor		:	1;	//	복제 캐릭
	DWORD			m_bf1IsMarionette		:	1;	//	마리오네뜨
	DWORD			m_bf1IsBoss				:	1;
	DWORD			m_bf3HitGGGType			:	3;	//	때린 엑터의 GGG
	DWORD			m_bf1IsGGGPointHit		:	1;	//	스킬업 이펙트 .	Effect출력용.
	DWORD			m_bf1IsGGGNormalHit		:	1;	//	전투 버프 타격시 .	Effect출력용.
	DWORD			m_bf1IsGGGHitBy			:	1;	//	전투 버프 피격시 .	Effect출력용.
	DWORD			m_bf1IsSoundOfLedersBellsLeader	:	1;	//	인도자의 종소리 울린 사람
	DWORD			m_bf1IsHideName			:	1;	//	이름 숨기기
	DWORD			m_bf4RebirthCount				:	4;	//	전생 횟수

////////////////////////////////////////////////////////////
//	비트정보
////////////////////////////////////////////////////////////
	CBitInfoManager		m_bitInfoManager;

////////////////////////////////////////////////////////////
//	기타정보
////////////////////////////////////////////////////////////

//	게임 내부 캐릭터 상태
	BOOL			m_isDontUpdate;				//	update를 하지 않는다.
	BOOL			m_isExclusiveAction;		//	독점적 행동(다른 행동이나 이동등을 할 수 없다.)
	char			m_isCasting;				//	캐스팅 중이다.
	WORD			m_wBlockingRigidityTime;		//	방어시 경직 시간
	int				m_iPostureOfDefense;		//	방어 상태. 나 기타등등..
	WORD			m_wIsAddPaint,m_wIsPutName;

///////////////////////////////////////////////////////////////
//	출력 관련
///////////////////////////////////////////////////////////////
	WORD			m_wParallelBunshineCount,m_wParallelBunshineAngleGap;
	CAfterImageInfo	m_aAfterImageList[dAFTER_IMAGE_COUNT];
	WORD			m_wAfterImageDelayPeriod;	//	캐릭터 잔상 딜레이 간격.
	WORD			m_wAfterImageDelayCounter;	//	캐릭터 잔상 딜레이 간격.
	char			m_cAfterImageStatus;
	char			m_cAfterImageCount;			//	캐릭터 잔상 수
	char			m_cAfterImageDepth;			//	캐릭터 잔상 수
	char			m_cAfterImageDisappearSpeed;//	캐릭터 잔상이 사라지는 속도

	char			m_isPutDust;				//	먼지를 뿌려주는 상태
	char			m_cParallelCount,m_cParallelAnm;	//	분신의 수,분신의 에니메이션

	WORD			m_aHitEffect[dHIT_EFFECT_COUNT_ON_CHARACTER];			//	타격 효과
	WORD			m_wAuraImage;
	LONGLONG		m_dwEnchantedMask;
	uEnchantedImage	m_enchantedImage;
	WORD			m_wTheOtherSelfCount;
	WORD			m_wRevealTime;
	WORD			m_wDoppelgangerCount;		// 분신 개수...
/////////////////////////////////////////////////////////////////////////////
//	캐릭터 액션/에니메이션 관련
/////////////////////////////////////////////////////////////////////////////
	WORD			m_wChargeImage,m_wChargeFrameCounter;
	BYTE			m_bChargeAnm,m_bChargeFrame;
	char			m_isTriggerFrame,m_isDamageFrame;		//	트리거 프레임이다. 대미지 프레임이다.
	int				m_iFPS;									//	현재 에니메이션의 FPS와 PPS
	int				m_iDirectCount,m_iFrameCount;			//	현재 에니메이션의 방향수
	WORD			m_wPltIndex;							//	팰레트
	WORD			m_wLoopState;							//	현재 액션이 루프다.
	WORD			m_wRemainReplayCount;					//	현재 액션에니메이션의 남은 반복 수치.
	int				m_iAnm,m_iDirect,m_iAngle,m_iFrame,m_iFrameCounter;	//	액션,방향,프레임,FrameCounter
	int				m_iMiniPetFPS,m_iMiniPetAnm,m_iMiniPetFrame,m_iMiniPetFrameCount,m_iMiniPetFrameCounter;	//	액션,방향,프레임,FrameCounter
	int				m_iMiniPetFPS2,m_iMiniPetAnm2,m_iMiniPetFrame2,m_iMiniPetFrameCount2,m_iMiniPetFrameCounter2;	//	액션,방향,프레임,FrameCounter

	WORD			m_wBookingDirect;
	WORD			m_wOverlapAnm,m_wOverlapAnm2;			//	오버랩되는 에니메이션
	int				m_iViewAngle;							//	눈에 보이는 각도
	WORD			m_wStopDirect;							//	정지해 있는 방향
	char			m_isActionFinished;						//	방금 액션이 끝났다.
	WORD			m_wActionSpeed;							//	액션 속도 100%기준

/////////////////////////////////////////////////////////////////////////////
//	오우섬 포트리스를 위한 정보
/////////////////////////////////////////////////////////////////////////////
	WORD			m_wAwesomeFortressAction;
	CActroAnmInfo	m_awesomeFortressActionInfo;

/////////////////////////////////////////////////////////////////////////////
//	분신 연타 정보
/////////////////////////////////////////////////////////////////////////////
	WORD			m_wContinuousHitOfDoppelgangerAction[dAFTER_IMAGE_COUNT];
	CActroAnmInfo	m_continuousHitOfDoppelgangerActionInfo[dAFTER_IMAGE_COUNT];
/////////////////////////////////////////////////////////////////////////////
//	위치 정보/길찾기 정보
/////////////////////////////////////////////////////////////////////////////
	c4POINT			m_pos,m_posDest;					//	현재 위치,목표위치,타일단위 위치(맵 타일 말고.. 맵 정보타일)
	CPos			m_posTarget,m_aPath[dSYNC_FPS];			//	프레임 수 만큼 맞춰 준다.
	int				m_iPathIndex;						//	현재 이동 인덱스
	int				m_iPPS;								//	이동 속도
	WORD			m_wMoveSpeed;						//	이동 속도 100%기준

	BOOL			m_isMove;							//	현재 이동중이다.
	BOOL			m_isFindStopPosition;				//	정지할 곳을 찾아 헤메는-o-중이다.
	BOOL			m_isBlockedWay;						//	막힌길이라니까!!

/////////////////////////////////////////////////////////////////////////////
//	스킬 관련
/////////////////////////////////////////////////////////////////////////////
	cATTACK_INFO	m_attackInfo;
	cAbility		m_abilityUse,m_abilityRush;
	WORD			m_wUseSkill;						//	현재 사용중인 기술 Skill Manager index
	char			m_isInvincibleSkill;				//	지금 사용하는 기술이 사용하는동안 무적이 되는 기술이다.
	WORD			m_wIsFreezeAtTriggerFrame;			//	트리거 프레임에서 고정된다.
	WORD			m_wTargetMark;

//	연속 타격 공격
	BYTE			m_isGetCPByContinuousHitFirstAttack,m_bContinuousHitCounter;
	BYTE			m_bIllusionAttackFrame;

//	몸 주위에 옵션
	WORD			m_wIsCastBlocker,m_wBlockerAngle,m_wBlockerShape,m_wSpearBlockerShape,m_wBlockerRigidityTime;	//	
	WORD			m_wShakeTime;						//	몸이 흔들거려!!
	//	블록커 - 캐스터(m_wBlockerCaster - 자기자신,m_wBlockerTime),수혜자(m_wBlockerCaster - 캐스터)
	WORD			m_wMachineSkill,m_wMachineAngle,m_wMachineFrame;	//	머쉰~~
	WORD			m_wIntervalShooter;								//	인터벌 슈터
//	점프
	WORD			m_wJumpTargetRange;									//	타겟과의 거리
	WORD			m_wJumpHeight;
	WORD			m_wJumpMaxRange;

	short			m_sLevitateHeight;

//	특수한 액션이 있는 스킬 사용중
	WORD			m_wUseSpecialActionSkill;
//	러쉬
	WORD			m_wRushStatus;							//	러쉬? 중이다.
	WORD			m_wWaitRushTriggerTime;
	WORD			m_wRushResult;
	BOOL			m_bIsJumpRush;							//	점프 러쉬 인가?!
	WORD			m_wRushAcceleration;					// 러쉬 가속도
	int				m_iHeightJumpRush;						//	점프러쉬 높이
	WORD			m_wIncreaseHeightJumpRush;				//	점프증가값
	DWORD			m_dwRushFrameCounter;					//  러쉬 프레임 카운터....
	WORD			m_wJumpRushLastActionFrame;				//	점프러쉬 최종액션프레임
	int				m_iJumpRushRange;	
//	점프
	WORD			m_wJumpStatus;							//	러쉬? 중이다.
	WORD			m_wJumpResult;
	WORD			m_wJumpMaxRangeSave;					//JBC  08-07-31
	BOOL			m_bIsJumpAttack;
	BOOL			m_bIsEffectJump;				//	공중점프 이팩트인가?
	WORD			m_wEffectJumpSpeed;					//	공중 점프속도
	WORD			m_wIncreaseHeightEffectJump;					//	공중 점프 증가 높이
	int				m_iHeightEffectJump;			//		공중점프 높이.
	WORD			m_wJumpFrameCount;				//	점프프레임
//	언덕 스킬
	WORD			m_wHillSkill;						//	언덕
	int				m_iHillHP;							//	체력... -_-

//	디스플래이스먼트
	WORD			m_wLordActor,m_wSlaveActor;			//	늑대인간의 디스플레이스먼트에서 주인
	WORD			m_wIsFeignDeath;					//	죽은척..?
	CPos			m_posFeignDeath;

//	바이트 행잉
	WORD			m_wBiteTarget,m_wBiter;

//	기타
	WORD			m_wDummyStatus;
	WORD			m_wOperatorLevel;					//	이사람 운영자 레벨
	WORD			m_wIsEventMob;
	WORD			m_wSageBookColor;

//	족쇄 스킬
	WORD			m_wShakleSkill,m_wShakleShakeTime;
	WORD			m_wShakleAnm,m_wShakleFrameCounter,m_wShakleFrame;
	WORD			m_bf1IsFreezeShakle	:	1;

//  분신 공격 (부가효과)

	WORD			m_wAttackDoppelgangerCount		;
	int				m_iAttackDoppelgangerDamage		;
	WORD			m_wActionPerionAttackDoppelganger	;
	float			m_fAttackDoppelgangerFrame;
	WORD			m_wAttackDoppelgangerAnm;
	BOOL			m_bIsHitAttackDoppelganger;

	the_fallowing_motion	m_flying_bow;

	cChanedDamageInfoStack	m_chainedDamage;
	cATTACK_INFO	m_attackInfoCHOD;		// 분신연타공격 공격정보
	WORD			m_wItemEffect;			//	아이템으로 인해 얻은 이팩트 효과
	inline	void	reset()
	{
		memset(this,0,sizeof(cACTOR_DATA));

		m_itemWeapon.reset();
		m_itemShield.reset();	//	장비하고 있는 무기,장비하고 있는 방패
		m_itemArmor.reset();
		m_awesomeFortressActionInfo.reset();
		for(int i=0;i<dAFTER_IMAGE_COUNT;++i)
			m_continuousHitOfDoppelgangerActionInfo[i].reset();
		m_enchantedImage.reset();
		m_chainedDamage.reset();

		m_wNpcActorSerial		=	0xffff;
		m_wSageBookColor		=	0;
		m_wSerial				=	0xffff;	//	액터 슬롯
		m_wTamer				=	0xffff;
		m_wRideDog				=	0xffff;
		m_wAwesomeFortressAction=	0xffff;
		memset(m_wContinuousHitOfDoppelgangerAction, 0xffff, sizeof(WORD) * dAFTER_IMAGE_COUNT);

		m_attackInfo.m_wTarget	=	0xffff;
		m_attackInfo.reset();
		m_wStopDirect			=	0xffff;
		m_wBlockerAngle			=	0xffff;
		m_wDummyStatus			=	0xffff;
		m_wChargeImage			=	0xffff;
		m_wAuraImage			=	0xffff;
		m_wOverlapAnm			=	0xffff;
		m_wOverlapAnm2			=	0xffff;
		m_wTargetMark			=	0xffff;
		m_wBiteTarget			=	0xffff;
		m_wBiter				=	0xffff;
		m_wIsCastBlocker		=	FALSE;
		m_wHorizonScale			=	100;
		m_wVerticalScale		=	100;
		m_wShakleSkill			=	0xffff;
		
		m_wPitchmanShopSerial	=	0xffff;
		m_wShutInMagicBoxImage	=	0xffff;
		m_wReleaseMagicBoxAlpha	=	0xffff;
		m_wCrushMagicBoxFrame	=	0xffff;
		m_wRollingLogImage		=	0xffff;
		m_bf1IsTalker			=	TRUE;
		m_posPet.x				=	0xffff;
		m_posPet.y				=	0xffff;
		m_posPet2.x				=	0xffff;
		m_posPet2.y				=	0xffff;
		m_bitInfoManager.reset();
		m_wItemEffect			=	0xffff;
	}

};

#define	dDEFAULT_BALOON_TALK_TIME		(5*dSYNC_FPS)
#define	dDEFAULT_BALOON_WARNING_TIME	(3*dSYNC_FPS)

class	CActor	:	public cACTOR_DATA , public CGamePlay
{
public:
	static	cRECT	s_rectHeroSight;

					CActor();

	int				getBodyIndex();
	inline	BOOL	isHighOperator()
	{
		if (m_wOperatorLevel >= dMASTER_OPERATOR_BOTTOM_LEVEL)
			return	TRUE;

		return	FALSE;
	}

	//JBC 이동중 도착점 X표시 08-06-30
	inline void		DrawCross(int _iX , int _iY)
	{
		cDRAW::Line(0xffff, _iX - 10 , _iY - 10, _iX + 10, _iY + 10);
		cDRAW::Line(0xffff, _iX - 10 , _iY + 10, _iX + 10, _iY - 10);
	}

	BOOL			isIncorrectName();	//	잘못된 이름
	BOOL			set(int x,int y){return	TRUE;};//맵에서 로딩한 캐릭터 데이터를 세팅한다.
	void			setDataByHero(CHero *_lpPlayer);//영웅데이터로 데이터 설정
	BOOL			setDataByAvatar(cAVATAR_INFO *_lpAvatar);//아바타 정보로 액터 데이터 설정.. -_-a

	void			changeMinipetType(int _iType1,int _iType2);
	void			buildByMinimumActorInfo(CMinimumActorInfo*);//난입한 플레이어/몬스터
	void			buildBySimplePlayerInfo(CSimplePlayerInfo *_lpInfo);
	void			buildByVerySimplePlayerInfo(CVerySimplePlayerInfo *_lpInfo,BOOL _bIsAskActorInfoResult);
	void			buildBySimpleMonsterInfo(CSimpleMonsterInfo *_lpInfo);
	void			buildByVerySimpleMonsterInfo(CVerySimpleMonsterInfo *_lpInfo,BOOL _bIsAskActorInfoResult);
	void			buildBySimpleNpcInfo(CSimpleNpcInfo *_lpInfo);
	void			buildByVerySimpleNpcInfo(CVerySimpleNpcInfo *_lpInfo,BOOL _bIsAskActorInfoResult);

	void			reset();									//	모든 데이터를 초기화 한다.
	BOOL			resetForOutOfSightActor();
	BOOL			remove();

	void			setReservedAction();
	void			blockingAction(CActor *_lpAttacker,int _iRigidityTime);	//	블럭킹 액션 취하기
	void			hitAction();
	
	void			setAnm(int anm,BOOL _bIsForeced=FALSE);
	void			setReplayCount(int _ReplayCount);
	void			setMiniPetAnm(int anm,int _iIndex);
	void			changeShakleAnm(int anm);
	void			setDirect(int _iDirect,BOOL _bIsIgnoreStunStatus=FALSE);
	void			bookingDirect(int _iDirect);
	void			setFpsByWeaponSpeed(int _iCorrectSpeed=100);	//	무기 속도에 맞춰서 속도를 보정한다.
	void			setFpsByTPA(int _iTPA,int _iCorrectSpeed=100);	//	TPA(Time Per Action)에 맞춰서 FPS를 설정한다.

//////////////////////////////////////////////////////////////////////////////////////////
//	보정
//////////////////////////////////////////////////////////////////////////////////////////
	void			resetCorrectDataByAidMagic();
	void			resetAllCorrectData();	//	캐릭터의 모든 보정 데이터를 초기화 한다.

	void			updateStatusData();
	void			computePowerData();		//	캐릭터의 보정 데이터를 계산한다.
	void			reduceHP(int _iDamage,BOOL _bIsCheckSync = TRUE,cAbility *_lpAbility = NULL);	//	체력 줄어듦

	inline	void	changeSyncWithServerStatus(int _iStatus)
	{
		m_wOldSyncStatusWithServer	=	m_wSyncStatusWithServer;
		m_wSyncStatusWithServer		=	_iStatus;
	}
	inline	BOOL			isTransformationMonster()
	{
		if(m_enchantedImage.m_bf1IsTransformationHunter || m_enchantedImage.m_bf1IsTransformationThief || m_enchantedImage.m_bf1IsTransformationMagician || m_enchantedImage.m_bf1IsTransformationReptile)
			return TRUE;

		return FALSE;
	}
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃이펙트 처리
//┃CActorEffect.cpp
	void			addHitEffect(int _iDx,int _iDy,int _iEffect,int _iDirect=0,int _iAnm=0xffff,int _iShakeValue=0 , BOOL _isHalfFaram = FALSE);
	void			addSkillHitEffect(int _iEffect);
	void			addEffect(int _iEffectOutputPos,int _iImage,int _iAnm=0xffff,int _iXScale=100,int _iYScale=100);
	void			addHitEffectByEvent(int _iDamageType,int _iDamage);
	void			addHitEffectAndInfo(int _iEffect,int _iPhysicalDamage,int _iMagicDamage,int _iEffectOutputPos=eHEOP_HIT_ZONE,int _iAnm=0xffff);
	void			addHealEffectAndInfo(int _iEffect,int _iHealPoint,int _iEffectOutputPos=eHEOP_HIT_ZONE);
	void			addHitInfo(int _iInfo,int _iEffectOutputPos=eHEOP_HIT_ZONE);
	void			getHitPointCorrectPos(int _iPoint,CPos *_lpPos);

	void			addSkillEffect(int _iSkill,int _iImage=0xffff);	//	스킬 효과 발생
	void			addAuraImage(int _iSkill);	//	오라 이펙트 발사!!-_-?
	void			attatchLevelUpEffect();
	void			attatchSkillLevelUpEffect();

	void			setChargeEffect(int _iImage);
	void			increaseChargeEffectAnimation();
	void			resetChargeEffect();
	void			cancelChargeEffect();
	void			changeSageBookColor(int _iBookColor);
//┃CActorEffect.cpp
//┃이펙트 처리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃이벤트 처리
//┃CActorEvent.cpp
	void			talk(char *_lpText,int _iTime=dDEFAULT_BALOON_TALK_TIME);	//	말하기
	void			warning(char *_lpText,int _iTime=dDEFAULT_BALOON_WARNING_TIME);	//	말하기
	
//┃CActorEvent.cpp
//┃이벤트 처리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃출력
//┃cACTOR_PUT.cpp


	void			makeHealEffect(int _iSkill,int _iHealPoint);	//	힐 효과 추가
	void			makeAbsorbDamageEffect(int _iHealPoint);	//	대미지 흡수 효과 추가
	inline void		setPalette();				//	캐릭터의 팔레트 설정
	inline void		setWeaponPalette();			//	무기의 팔레트 설정
	inline void		setShieldPalette();			//	방패의 팔레트 설정

	void			setSageBookPalette();

	void			putImageSyncWithActor(int	_iImage);

	cANM*			getWeaponAnm();
	inline void		putWeapon(int x,int y,cANM *_lpWeapon, int _iDirect,WORD _wAnm, int _iAlphaDepth=32);
	inline void		putBackWeapon(int x,int y, int _iDirect,WORD _wAnm,BOOL _bIsWantIgnorePos=FALSE);
	inline void		putFrontWeapon(int x,int y, int _iDirect,WORD _wAnm,BOOL _bIsWantIgnorePos=FALSE);
	inline void		putBackWeaponAB(int x,int y, int _iDirect,WORD _wAnm,int _iAlpha,BOOL _bIsWantIgnorePos=FALSE);
	inline void		putFrontWeaponAB(int x,int y, int _iDirect,WORD _wAnm,int _iAlpha,BOOL _bIsWantIgnorePos=FALSE);
	inline void		putBackWeaponHB(int x,int y, int _iDirect,WORD _wAnm,BOOL _bIsWantIgnorePos=FALSE);
	inline void		putFrontWeaponHB(int x,int y, int _iDirect,WORD _wAnm,BOOL _bIsWantIgnorePos=FALSE);

	inline void		putShield(int x,int y,int _iEquimentShape, int _iDirect,WORD _wAnm, int _iAlphaDepth=32);
	inline void		putBackShield(int x,int y, int _iDirect,WORD _wAnm);
	inline void		putFrontShield(int x,int y, int _iDirect,WORD _wAnm);
	inline void		putBackShieldAB(int x,int y, int _iDirect,WORD _wAnm,int _iAlpha);
	inline void		putFrontShieldAB(int x,int y, int _iDirect,WORD _wAnm,int _iAlpha);
	inline void		putBackShieldHB(int x,int y, int _iDirect,WORD _wAnm);
	inline void		putFrontShieldHB(int x,int y, int _iDirect,WORD _wAnm);

	inline void		putEquipmentBack(int x,int y, int _iDirect,WORD _wAnm);	//	장비를 캐릭터 보다 뒤에 찍는다.(북
	inline void		putEquipmentFront(int x,int y, int _iDirect,WORD _wAnm);	//	장비를 캐릭터 보다 앞에 찍는다.(남

	inline void		putBlockerShadow(int x,int y);
	inline void		putShadow(int x,int y);			//	그림자.
	inline void		putAfterImage(BOOL front);		//	잔상을 찍는다.
	inline void		putOverLap(int x,int y);		//	오버랩 이미지를 찍어준다.

	inline void		putActorSimpleInfo(int x,int y);
	inline void		putBackAdditionalEffect(int _iX,int _iY);
	inline void		putFrontAdditionalEffect(int _iX,int _iY);
	inline void		putAdditionalEffect(int x,int y,BOOL _bIsFront);

	inline void		putChargeImage(int _iX,int _iY,BOOL _bIsFront);
	inline void		putHitEffect(int x,int y);
	inline void		putAllEnchantedImage(int x,int y,BOOL _bIsFront);
			void	putAttatchedFlatImage(int x,int y,BOOL _bIsFront);
	
	inline void		putEnchantedImage(int x,int y,int _iMask,BOOL _bIsFront);
	void			putCounter(int _iImage,int _iTime,int _iX,int _iY);


	inline BOOL		putDummy();							//	화면에 더미를 찍어준다.
	inline void		putBlocker(int x,int y,BOOL front);
	inline void		putMachine(int x,int y);
	inline void		putDebugInfo1(int x,int y);
	inline void		putDebugInfo2(int x,int y);
	inline void		putName(int x,int y,BOOL _bIsOnlyGauge=FALSE);//이름 출력

	void			putBody(int _iX,int _iY);//	찍기
	inline void		putBodyHalf(int _iX,int _iY,int _iFrame, int _iDirect);//반투명으로 찍기
	inline void		putGGGBuffType(int _iX,int _iY);//은총 이펙트를 출력 해준다..
	void			put();//화면에 찍어준다.
	void			putShakle(int _iX,int _iY);
	void			putTheOtherSelf(int _iX,int _iY,BOOL _bIsFront);
	void			putFeignDeath();
	void			putHill(int _iX,int _iY);
	void			putForModel(int _iX,int _iY,BOOL _bIsFocus);//	모델을 위한 출력?
	void			putBottomImage();	//	바닥에 깔리는 이미지 출력
	void			putCastImage(int x,int y);	//	바닥에 깔리는 이미지 출력
	void			putDialog(int _iX,int _iY);	//	풍선대화 출력

	void			addPaint();															//	화면에 찍어준다.
	void			drawPetMark();

	inline	void	setExclusiveAction(BOOL _isSetting)		{m_isExclusiveAction	=	_isSetting;}
	inline	BOOL	isExclusiveAction()						{return	m_isExclusiveAction;}

	inline	void	setPostureOfDefense(int _iPosture)		{m_iPostureOfDefense	=	_iPosture;}
	inline	int		getPostureOfDefense()					{return m_iPostureOfDefense;}

	inline void		putBodyEffect(int _iX,int _iY,int _iFrame, int _iDirect, int _iEffect, int _iAnm = 0xffff, BOOL _bIsUseEnchantedMask = TRUE);// 몸통의 이팩트효과
	void			putBit(int _iX,int _iY);//	비트찍기
//┃cACTOR_PUT.cpp
//┃출력
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃액터 정보
//┃CActorInfo.cpp
	inline	BOOL	isDummy()	{if (m_wDummyStatus == 0xffff) return FALSE;return TRUE;}
	BOOL			isInvisivle();
	BOOL			isBlur();
	char*			getName();

	CActiveSkill*	getActiveSkill();	//	현재 사용중인 스킬
	BOOL			isCastAbleSkill(cAbility *_lpAbility,BOOL _bIsCheck=FALSE);	//	이 스킬이 이녀석한테 사용 가능한 스킬이냐?
	BOOL			isInAttackRange(int _iX,int _iY,int _iRange);//	사정거리 안?
	BOOL			isPartyMember();	//	이녀석 울 파티 멤버냐?
	BOOL			isPartyLeader();	//	이녀석 울 파티 리더냐?
	BOOL			isUnionPartyLeader();	//	이녀석 울 파티 리더냐?
	BOOL			isControlMonster();
	BOOL			isFocusActor();
	BOOL			isEngageTarget();
	BOOL			isPetEngageTarget();
	BOOL			isVisibleName();
	BOOL			isRequireAddPaint();

	CJobInfo*		getJob();

	int				getHillHeight();
	int				getHeight(BOOL _bIsIncludeLevitateHeight=TRUE);

	BOOL			isStunedStatus(BOOL _bIsIgnoreNightmare=FALSE);
	BOOL			isBlockToMove();
	inline	BOOL	isRockBounding()
	{
		if (m_dwEnchantedMask & dAMEM_ROCK_BOUNDING)
			
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL	isStoned()
	{
		if (m_dwEnchantedMask & dAMEM_STONED)	return	TRUE;

		return	FALSE;
	}
	inline	BOOL	isStuned()
	{
		if (m_dwEnchantedMask & dAMEM_STUN)
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL	isParalysis()
	{
		if (m_dwEnchantedMask & dAMEM_PARALYSIS)
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL	isCharmed()
	{
		if (m_dwEnchantedMask & dAMEM_CHARMING)	
			return	TRUE;

		return	FALSE;
	}
	inline	void	releaseStoned()
	{
		if (m_dwEnchantedMask & dAMEM_STONED)
			m_dwEnchantedMask -= dAMEM_STONED;
	}

	inline	BOOL	isFreeze()
	{
		if (m_dwEnchantedMask & dAMEM_FREEZE)	
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL	isCold()
	{
		if (m_dwEnchantedMask & dAMEM_COLD)	
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL	isConfuse()
	{
		if (m_dwEnchantedMask & dAMEM_CONFUSE)	
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL	isBoost()
	{
		if (m_dwEnchantedMask & dAMEM_BOOST)	
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL			isIgnoreAttackByGGG()
	{
		if (m_dwEnchantedMask & dAMEM_IGNORE_ATTACK_BY_GGG)
			return	TRUE;
		
		return FALSE;
	}

	inline	BOOL	isInvincible()
	{
		if (m_dwEnchantedMask & dAMEM_INVINCIBLE)	
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL	isBlind()
	{
		if (m_dwEnchantedMask & dAMEM_BLIND)
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL	isNewBlindVerySmall()
	{
		if (m_dwEnchantedMask & dAMEM_NEW_BLIND_VERY_SMALL)
			return	TRUE;
		
		return	FALSE;
	}

	inline	BOOL	isNewBlindSmall()
	{
		if (m_dwEnchantedMask & dAMEM_NEW_BLIND_SMALL)
			return	TRUE;
		
		return	FALSE;
	}

	inline	BOOL	isNewBlindMiddle()
	{
		if (m_dwEnchantedMask & dAMEM_NEW_BLIND_MIDDLE)
			return	TRUE;
		
		return	FALSE;
	}

	inline	BOOL	isNewBlindLarge()
	{
		if (m_dwEnchantedMask & dAMEM_NEW_BLIND_LARGE)
			return	TRUE;
		
		return	FALSE;
	}

	inline	BOOL	isNewBlindVeryLarge()
	{
		if (m_dwEnchantedMask & dAMEM_NEW_BLIND_VERY_LARGE)
			return	TRUE;
		
		return	FALSE;
	}

	inline	int		getMaskSize()
	{
		if (m_dwEnchantedMask & dAMEM_BLIND)
			return	0;

		if (m_dwEnchantedMask & dAMEM_NEW_BLIND_VERY_SMALL)
			return	1;

		if (m_dwEnchantedMask & dAMEM_NEW_BLIND_SMALL)
			return	2;

		if (m_dwEnchantedMask & dAMEM_NEW_BLIND_MIDDLE)
			return	3;

		if (m_dwEnchantedMask & dAMEM_NEW_BLIND_LARGE)
			return	4;

		if (m_dwEnchantedMask & dAMEM_NEW_BLIND_VERY_LARGE)
			return	5;
		
		return	6;
	}
	

	inline	BOOL	isBerserk()
	{
		if (m_dwEnchantedMask & dAMEM_BERSERK)
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL	isDisplacement()
	{
		if (m_dwEnchantedMask & dAMEM_DISPLACEMENT)	
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL	isNightMare()
	{
		if (m_dwEnchantedMask & dAMEM_NIGHTMARE)
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL	isSleep()
	{
		if (m_dwEnchantedMask & dAMEM_SLEEP)
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL	isEnchantedFountaionBarrier()
	{
		if (m_dwEnchantedMask & dAMEM_FOUNTAIN_BARRIER)
			return	TRUE;
		return	FALSE;
	}

	inline	BOOL	isEnchantedMisticFog()
	{
		if (m_dwEnchantedMask & dAMEM_MISTIC_FOG)
			return	TRUE;
		return	FALSE;
	}

	inline	BOOL	isEnchantedTornadoShield()
	{
		if (m_dwEnchantedMask & dAMEM_TORNADO_SHIELD)
			return	TRUE;
		return	FALSE;
	}

	inline	BOOL	isEnchantedRockBounding()
	{
		if (m_dwEnchantedMask & dAMEM_ROCK_BOUNDING)
			return	TRUE;
		return	FALSE;
	}

	inline	BOOL	isEnchantedHotEnchanting()
	{
		if (m_dwEnchantedMask & dAMEM_HOT_ENCHANTING)
			return	TRUE;
		return	FALSE;
	}
	inline	BOOL	isEnchantedLevitate()
	{
		if (m_dwEnchantedMask & dAMEM_LEVITATE)	
			return	TRUE;
		return	FALSE;
	}

	inline	BOOL			getKind()
	{
		if (isPlayer())
			return	eJT_HUMAN;

		if (getJob())
			return	getJob()->m_wKind;

		return	eJT_HUMAN;
	}
	inline	BOOL	isMadness()
	{
		if (m_dwEnchantedMask & dAMEM_BLACK)	
			return	TRUE;

		return	FALSE;

	}	

	inline	BOOL			isUndead()
	{
		if (getKind()	==	eJT_UNDEAD || m_enchantedImage.m_bf1IsMakeTypeUndead)	
			return	TRUE;
		return	FALSE;
	}
	inline	BOOL			isDemon()	{if (getKind()	==	eJT_DEVIL)	return	TRUE;return	FALSE;}
	inline	BOOL			isHuman()	{if (getKind()	==	eJT_HUMAN)	return	TRUE;return	FALSE;}
	inline	BOOL			isAnimal()	{if (getKind()	==	eJT_ANIMAL)	return	TRUE;return	FALSE;}
	inline	BOOL			isHolyBeast(){if (getKind()==	eJT_HOLY_BEAST)return	TRUE;return	FALSE;}

//┃CActorInfo.cpp
//┃액터 정보
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//////////////////////////////////////////////////////////////////////////////////////////
//	정보 구함
//////////////////////////////////////////////////////////////////////////////////////////
	BOOL			isPickAbleItem(int _iFieldItem);		//	필드에 떨어진 아이템이 지금 줏을 수 있는 아이템이냐?
	int				getAttackPower(int _iAddSkillDamagePercentage,int _iAddSkillDamage);	//	공격력을 구한다.
	int				getDefensivePower();													//	방어력을 구한다.

	inline	void	setShakeTime(int _iTime)	{m_wShakeTime	=	max(_iTime,m_wShakeTime);}

	inline	BOOL	isDeath()	{if (m_wCorpseTime)	return	TRUE;return	FALSE;}				//	죽었냐?
	inline	BOOL	isCorpse()	//	시체냐? (쓰러지는 중이 아니고 바닥에 쓰러져 있다.
	{
		if (m_wCorpseTime == 0)	
			return	FALSE;

		if (m_iFrame	>=	m_iFrameCount-1)
			return	TRUE;

		return	FALSE;
	}										//	죽었냐?
	inline	int		isOurForce(int _iTeam){if(m_wTeam==_iTeam) return TRUE;return FALSE;} //	_iTeam이 아군이냐?
	inline	BOOL	isSitdown()		{if (m_iAnm	==	dACT_SITDOWN)	return	TRUE;return	FALSE;}	//	앉아 있냐?
	inline	BOOL	isOwnTeam()		{if (m_wTeam == g_hero.m_wTeam)	return	TRUE;return	FALSE;}//	같은 팀이다.
	inline	BOOL	isMonster()		{if (m_wActorKind	==	eAK_MONSTER	)	return	TRUE;return	FALSE;}	//	플레이어 편이다.(몬스터가 아니다 -_-)
	inline	BOOL	isPlayer()		{if (m_wActorKind	==	eAK_PLAYER	)	return	TRUE;return	FALSE;}	//	영웅이다.
	inline	BOOL	isHero()		{if (m_wSerial		==	g_hero.m_iSerial)	return	TRUE;return	FALSE;}
	inline	BOOL	isSlave()		{if (m_wLordActor	==	g_hero.m_iSerial)	return	TRUE;return	FALSE;}
	inline	BOOL	isNpc()			{if (m_wActorKind	==	eAK_NPC	||	m_wActorKind	>	eAK_MONSTER)	return	TRUE;return	FALSE;}
	inline	BOOL	isMerchant()	{if (m_wActorKind	>=	eNPC_WEAPON_MERCHANT	||m_wActorKind<=eNPC_BANK)	return	TRUE;return	FALSE;}
	inline	BOOL	isEventNpc()	{if (m_wActorKind	>=	eNPC_WEAPON_MERCHANT	)	return	TRUE;return	FALSE;}
	inline	BOOL	isPet()			{return	m_wIsPet;}
	inline	BOOL	isSummonBeast()	{if (m_wJob	>=	dJOB_KELBY_1 && m_wJob	<=	dJOB_HEDGER_3)return	TRUE;return	FALSE;}

	inline	int		getAnotherJob();											//	세컨드 직업을 구한다.
	cANM*			getBody();
	inline	cRECT*	getSelectBox()	{return	&getBody()->m_rectSelect;}		//	선택 박스를 구한다.
	inline	cRECT*	getCrashBox()	{return	&getBody()->m_rectCrash;}		//	크래쉬 박스를 구한다.
	BOOL			isFocus(int _iX,int _iY);	//	포커스냐?

	int				getViewEnemy();												//	시야에 보이는 적을 찾는ㄴ다.
	int				getDirect(int x1,int y1,int x2,int y2,BOOL setAngle);		//	방향을 구한다.
	int				getDirect(int x,int y,BOOL setAngle);
	int				getDirect(int angle,BOOL setAngle);

	inline	int		getRange(CActor *_lpActor);				//	actor 와의 거리를 구한다.
	inline	int		getRange(CActor *_lpActor,int _iAdd);	//	actor 와의 거리를 구한다.
	BOOL			isTouchAbleTarget(CActor *_lpActor,int _iRange,int _iMinRange=0);
	BOOL			isTouchAblePos(int _iX,int _iY,int _iRange);	//	충돌 박스를 고려해서 _iX,_iY까지가 _iRange 안에 있는지 체크한다.

	inline	int		getCrashActor(c4POINT *pos);			//	pos에서 충돌이 일어나는 캐릭터 구함

	inline	BOOL	isApproach(int index);					//	다음 프레임에서 index캐릭터와 가까워진다. 
	BOOL			isViewActor(int serial,int *range);		//	시야 안에 있는 녀석이냐?

	void			getReleasePos(CPos *pos,int _iAnm=-1,int _iDirect=-1);				//	액션 프레임에서 마법이나 기타 이펙트 릴리즈 포인트 구함

	inline	int		getTargetHitHeight(){return	-getBody()->m_rectCrash.y1-getBody()->m_rectCrash.getHeight()/2;}
	void			getHitPoint(CPos *_lpPos);			//	타격 포인트.. 미사일 종류가 날아갈때 타겟으로 잡아야할 부분

	inline	BOOL	isDamageFrame()		{return m_isDamageFrame;	}		//	뎀쥐 프레임?
	inline	BOOL	isTriggerFrame()	{return m_isTriggerFrame;	}		//	트리거 프렘??


	inline	BOOL	isVisibleShield();						//	방패가 보이냐?
	inline	BOOL	isVisibleWeapon();						//	무기를 장비하고 있냐?
	BOOL			isEquippedShield();						//	방패를 장비하고 있냐?
	BOOL			isEquippedWeapon();						//	무기를 장비하고 있냐?
	BOOL			isEquippedSpareWeapon();				//	여분의 무기를 장비하고 있냐?

	int				findSuitableAbility(int target,int range);	//	target을 range에서 공격하기에 적합한 스킬을 찾자.

	int				getTarget(int count,int *lpShots,int range);	//	range안에 count개의 적을 찾아서 리턴한다.
	inline	CActor	*getTarget();								//	현재 공격하고 있는 적을 구함

//	캐릭터 에니메이션 정보
	inline	int		getTriggerFrame(int _iAnm)	{return	getBody()->getTriggerFrame(_iAnm);}
	inline	int		getDamageFrame(int _iAnm)	{return	getBody()->getDamageFrame(_iAnm);}

	int				getBodyWidth(BOOL _bIsWantApplyScale=TRUE);
	int				getBodyHeight(BOOL _bIsWantApplyScale=TRUE);
	int				getBodySize(BOOL _bIsWantApplyScale=TRUE);

	inline	int		getFrameCount(int anm)	{return	getBody()->GetFrameCount(anm);}
	inline	int		getDirectCount(int anm)	{return	getBody()->GetDirectCount(anm);}
	inline	int		getFPS(int anm)			{return	getBody()->GetFPS(anm);}
	inline	int		getPPS(int anm)			{return	getBody()->GetPPS(anm);}
	inline	int		getLoopBeginFrame()		{return	getBody()->m_pAnmData[m_iAnm].m_iLoopBegin;}
	inline	int		getLoopEndFrame()		{return	getBody()->m_pAnmData[m_iAnm].m_iLoopEnd;}

//////////////////////////////////////////////////////////////////////////////////////////
//	이동 관련 
//////////////////////////////////////////////////////////////////////////////////////////
	BOOL			moveUpdate();

	BOOL			makePath(int x,int y);// x,y까지 길을 만든다.
	BOOL			findOtherWay(int x,int y);//makePath로 길을 찾고, 막혔을때 이넘으로 한번더 찾는다.. x,y까지 길을 만든다.
	void			getNextPos(c4POINT *pos);//	다음 프레임에서의 캐릭터 위치 정보
	BOOL			moveTo(int x,int y,BOOL _isFindAnyWay,BOOL _isFindStopPosition,BOOL _bIsThrust=FALSE);//이동
	inline	BOOL	moveDestPos(){return	moveTo(m_posDest.x,m_posDest.y,TRUE,FALSE);}//	목표 위치로 이동
	void			setDestPos(int _iX,int _iY);//목적지 위치
	void			makeAfterImage(CPos *pos,int anm,int direct,int frame,int angle,int pps,BOOL reverse=FALSE,BOOL dust=FALSE);	//	잔상의 위치를 설정한다.

	BOOL			traceActor(int actor,BOOL _isFindDetailPath=FALSE);		//	특정 캐릭터에게 이동(길찾기를 한다.
	void			stop(BOOL _bIsCancelUsedSkill=TRUE);
	void			stopAction();//액션취소
	inline	void	stop(int _iX,int _iY)//_iX,_iY에 정지
	{
		setPos(_iX,_iY);
		stop();
	}
	void			stopMove();

	//	위치 보정 관련
	CPos*			findMovablePos();				//	현재 위치에서 다음 타일의 중심으로 이동 할 수 없을때, 다음 타일에서
													//	이동 가능한 지점을 찾아 준다.
	BOOL			findBestPos(int *x,int *y);		//	이동 할 수 없는 지역을 선택했을때 주변에 이동 가능한 영역을 찾는다.
	BOOL			findBestPos(c4POINT *lpPos);	//	해당 위치를 검사하고 충돌이 생기면 적당한 위치를 찾아 준다.
	void			secureArea();
	void			setPos(int x,int y);

	inline	void	setPos()
	{
		m_pos.mx		=	m_pos.x>>dTILE_XSIZE_SHIFT;
		m_pos.my		=	m_pos.y>>dTILE_YSIZE_SHIFT;

		m_posDest.x		=	m_pos.x;
		m_posDest.y		=	m_pos.y;
		m_posDest.mx	=	m_pos.mx;
		m_posDest.my	=	m_pos.my;
	}	//	CActor::setPos()



//////////////////////////////////////////////////////////////////////////////////////////
//	데이터 설정
//////////////////////////////////////////////////////////////////////////////////////////

//	어빌러티 관련
	inline	BOOL		reduceCorpseTime();							//	시체-_-로 남아 있는 시간줄임

	inline	void		setStopDirect(int _iDirect)	{m_wStopDirect	=	_iDirect;}	//	정지시의 방향 설정

	inline	void		setBlockingRigidityTime(int	_iTime)	{	m_wBlockingRigidityTime	=	_iTime;	}
	inline	int			getBlockingRigidityTime()			{	return	m_wBlockingRigidityTime;		}
	inline	BOOL		updateBlockingRigidityTime();	//	블럭킹 경직 시간 업데이트
	inline	void		operateGGGEffectByHit(CActor* _lpTarget)
	{
		int		iEffect = 0 ;
		
		//은총 전투 버프 피격 이펙트.				//반사한 애가 이펙트를 뿌린다.
		if (m_bf1IsGGGHitBy)
		{
			m_bf1IsGGGHitBy = FALSE;
			
			switch (m_bf3HitGGGType)
			{
			case 1:		//FIRE 
				iEffect = dGGG_STRUCK_EFFECT_INDEX;	
				break;
			case 2:		//WATER
				iEffect = dGGG_STRUCK_EFFECT_INDEX + 1;	
				break;
			case 3:		//WIND
				iEffect = dGGG_STRUCK_EFFECT_INDEX + 2;	
				break;
			case 4:		//EARTH 
				iEffect = dGGG_STRUCK_EFFECT_INDEX + 3;	
				break;
			}
		}
		
		if (iEffect)
			_lpTarget->addHitEffect(0,0,iEffect);
	}
	inline	void		operateGGGEffectHit(CActor* _lpTarget)
	{
		int		iEffect = 0 ;
		
		//은총 전투 버프 타격 이펙트.
		if (m_bf1IsGGGNormalHit)
		{
			m_bf1IsGGGNormalHit = FALSE;
			
			switch (m_bf3HitGGGType)
			{
			case 1:		//FIRE 
				iEffect = dGGG_HIT_EFFECT_INDEX;	
				break;
			case 2:		//WATER
				iEffect = dGGG_HIT_EFFECT_INDEX + 1;	
				break;
			case 3:		//WIND
				iEffect = dGGG_HIT_EFFECT_INDEX + 2;	
				break;
			case 4:		//EARTH 
				iEffect = dGGG_HIT_EFFECT_INDEX + 3;	
				break;
			}
		}
		
		if (iEffect)
			_lpTarget->addHitEffect(0,0,iEffect);
	}
	inline  void	operateGGGEffectPointHit(CActor* _lpTarget)
	{
		int		iEffect = 0 ;

		//전투 스킬 버프 스킬포인트 시간이 있을때 버프.
		if (m_bf1IsGGGPointHit)
		{
			m_bf1IsGGGPointHit = FALSE;
			
			switch (m_bf3HitGGGType)
			{
			case 1:		//FIRE 
				iEffect = dGGG_HIT_EFFECT_INDEX;	
				break;
			case 2:		//WATER
				iEffect = dGGG_HIT_EFFECT_INDEX + 1;	
				break;
			case 3:		//WIND
				iEffect = dGGG_HIT_EFFECT_INDEX + 2;	
				break;
			case 4:		//EARTH 
				iEffect = dGGG_HIT_EFFECT_INDEX + 3;	
				break;
			}
		}
		
		if (iEffect)
			_lpTarget->addHitEffect(0,0,iEffect);
	}
	inline	void		operateBookedHit();	//	예약된 히트

	inline	void		setActionFinish(BOOL _isSetting)	{	m_isActionFinished	=	_isSetting;	}
	inline	BOOL		isActionFinished()					{	return	m_isActionFinished;			}

	inline	void		setInvincibleSkill(BOOL _isSetting)	{	m_isInvincibleSkill	=	_isSetting;}
	inline	BOOL		isInvincibleSkill()					{	return	m_isInvincibleSkill;}


//	잔상 효과
	void				afterImageOn(int _iEffect=0xffff,int _iCount=dAFTER_IMAGE_COUNT,int _iDepth=20,int _iReduce=2,int _iDelayPeriod=0);
	inline	void		afterImageOff()	{m_cAfterImageStatus		=	dFADE_AFTER_IMAGE;}	//	잔상 효과를 끈다.

	void				addAfterImage(int _iSlot,CPos *_lpPos,int _iAnm,int _iDirect,int _iFrame,int _iAlphaDepth);
	void				resetAfterImage();													//	잔상효과 초기화
	void				updateAfterImage();													//	잔상효과 업데이트

	inline	void		increaseFrame()	{m_iFrame++;if (m_iFrame >= m_iFrameCount)	m_iFrame=0;}

	BOOL				updateDummy();															//	데이터 갱신
	BOOL				update();															//	데이터 갱신
	void				syncAppearance();

	BOOL				updateFrame();
	void				updateMiniPetFrame();
	void				updateMiniPetPos(CPos* _lpPos, int _iAngle);
	void				updateShakleFrame();

	void				setLastFrame();
	inline void			updateChargeEffect();	//	충전효과 업데이트
	inline int			syncInfoWithServer();
	void				addToAskInfoActorList();		//	액터 정보 요청
	void				addToObscurityActorList();	//	매우 간단한 캐릭터 정보를 얻기위한 리스트에 추가
	void				addToMisplacedActorList();			//	액터 정보 요청
	BOOL				updateForModel();					//	모델을 위한 업데이트
	void				updateActionStatus();
	inline	void		updateLoopingAction();
	void				operateDoppellgangerAttack();		// 분신공격 부가효과

	void				updateEffect();					//	타격 효과 갱신
	inline	BOOL		isCanNotAttack();				//	공격할 수 없는 타겟이다.
	inline	BOOL		isUsedPos(int mx,int my)
	{
		if (g_map.m_aBlockInfo[mx+my*g_map.m_iWidth])
			return	TRUE;
		return	FALSE;
	}	//	mx,my에 뭔가가 있다.
	inline	int			checkBlockedWay(int mx,int my);		//	mx,my에서 뭔가와 충돌을 일으킨다.
	inline	BOOL		isBlockedWay(int _iMx,int _iMy,int _iDestMx,int _iDestMy);		//	mx,my에서 뭔가와 충돌을 일으킨다.
	inline	BOOL		isBlockedLine(int _iX1,int _iY1,int _iX2,int _iY2);

	inline	BOOL		isBlocked(int _iMx,int _iMy)			//	막힌 지역?
	{
		if (g_map.m_aBlockInfo[_iMx+_iMy*g_map.m_iWidth])
			return	TRUE;

		return	FALSE;
	}
	inline	BOOL	isMoveAblePos(int mx,int my);	//	캐릭터의 몸통 사이즈에 맞춰 바닥에 체크


	void			addBit(WORD _wSerialInField, DWORD	_dwNameHashCode);
	void			updateBit();
	void			removeBit(WORD _wSerialInField, DWORD	_dwNameHashCode, int _iCount);
//////////////////////////////////////////////////////////////////////////////////////////
//	스킬 제어 - 스킬이 캐릭터의 움직임을 제어 한다.
//////////////////////////////////////////////////////////////////////////////////////////

//	점프 관련  코드는 cSKILL_JUMP 에 들어가 있다. 
	void			jump();		//	점프시에 공통적으로 업데이트 돼야 할것들
	void			jumpFly();

//	러쉬 관련  코드는 cSKILL_RUSH 에 들어가 있다. 
	BOOL			rush(int _iX,int _iY,cAbility *_lpAbility,WORD _wResult,int _iFrame);													//	장애물 다 무시하고 러쉬!!
	BOOL			rushCrash(c4POINT *end,c4POINT *returnPos);					//	러쉬하면서 충돌한다
	BOOL			rushCrash(CPos *begin,CPos *end,c4POINT *returnPos);	//	러쉬하면서 걸리는넘 팬다. --;;
	BOOL			makeRushLine(int x,int y);								//	러쉬로 이동할 길을 닦아 놓는다.
	void			makeRushLine();
	void			getNextRushPos(c4POINT *pos);							//	러쉬시에 다음 프레임에서의 캐릭터 위치 정보
	int				rushCrashCheck(c4POINT *pos,c4POINT *returnPos);		//	러쉬중에 충돌 캐릭터 체크
	int				findRushVictim();										//	바운싱 리니어에서 다음 희생자(?)를 찾는다.

//	라운딩 스킬(빙글 빙글 도는..) 관련 코드는 cSKILL_ROUND에 들어가 있다.
//	BOOL			skillControlRounding(cSM *lpSm);


//////////////////////////////////////////////////////////////////////////////////////////
//	... 뭐라고 분류 해야 하나.. -_-a
//////////////////////////////////////////////////////////////////////////////////////////
	void			kill(BOOL _isRemoveOurforce	=	FALSE,BOOL _bIsReleaseSummon=FALSE);			//	죽었다. 죽어서 사라짐
	void			targetDeath();	//	타겟이 죽었다.
	void			targetRemove();	//	타겟이 제거 되었다.


//////////////////////////////////////////////////////////////////////////////////////////
//	행동 관련 
//////////////////////////////////////////////////////////////////////////////////////////
	void			action(int _iX,int _iY,int _iAction,int _iAngle=0xffff);	//	동작을 취한다.
	void			miniPetAction(int _iLocate);
	void			sitDown(int _iX,int _iY,BOOL _isSitDown);//	앉자

	void			randomDirect();												//	임의의 방향으로 바꾼다.
	void			readyTransformation(SG_TRANSFORMATION *_lpPacket);	//	변신준비
	void			transformation();	//	변신
	BOOL			checkEquipmentShape();

//////////////////////////////////////////////////////////////////////////////////////////
//	타격 관련
//////////////////////////////////////////////////////////////////////////////////////////

	void			strikeBasicAttackDamageToObject();
	void			strikeBasicAttackDamage();
	void			strike(CActor *_lpTarget,cAbility *_lpAbility,CHitInfo *lpHitInfo,int _iDirect=0,BOOL _isGetCP=TRUE,BOOL _bIsPlayHitSound=TRUE);//	예약된 대미지를 입힌다.
	void			strikeDamage(CActor *_lpTarget,cAbility *_lpAbility,int _iDamage);	//	_iDamage 먹임
	void			updateChainedDamage(cChanedDamageInfo	*_lpInfo);
	void			updateChainedDamage();
	BOOL			defense(CActor	*lpAttacker,cAbility *lpAbility);
	int				getFreeHitIndex();																	//	비어 있는 hit index를 구한다.
	void			hitDummy(CActor	*lpAttacker,cAbility *abil,int _iDirect=0);							//	맞았다.
	void			hit(CActor	*lpAttacker,cAbility *_lpAbility,CHitInfo *_lpHitInfo,int _iDirect=0,BOOL _bIsPlaySound=TRUE,BOOL _bIsMirrorTower=FALSE);//	맞았다.
	void			hitChainedDamage(int _iSkill,int _iShotCount,int _iMissCount,int _iGap,BOOL _bIsAttackToShakle,
									int _iPhysicalDamage,int _iMagicDamage,int _iMinPhysicalDamage,int _iMinMagicDamage,
									int _iCriticalCount,int _iDoubleCriticalCount,int _iInstanceKillCount,
									int _iBlockingCount,int _iNoActionBlockCount,int _iDancingBlockCount,int _iCaster, int _iHardBlowCount);

	void			hitMultipleDamage(CHitInfo *_lpHitInfo,int _iCount,int _iSkill,BOOL _bIsDamageSum);
	void			hitThornDamage(CActor *_lpAttacker,int _iDamage,int _iRemainCP, BOOL _isDisplayMagicDamage = FALSE);//	가시에 찔렸다.
	

//////////////////////////////////////////////////////////////////////////////////////////
//	이동 관련 
//////////////////////////////////////////////////////////////////////////////////////////
	void			thrust(int _iX,int _iY,int _iSkill,BOOL _bIsBlocked, BOOL _bIsChangeTargetDirect = FALSE);
	void			chaseActor(int serial);								//	특정 캐릭터를 따라 감

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃스킬 관련
//┃CActorSkill.cpp
	void			floatMachine(int _iWeapon);				//	머신 띄우기
	void			releasMachine();						//	머신 제거
	void			receiveAidMagic(int _iSkill,int _iType,int _iValue);	//	보조 마법 캐스팅
	BOOL			updateRushSkill();
	BOOL			updateScimitarCuttingSkill();
	BOOL			updateJumpSkill();
	void			stopBouncingLinear();
	BOOL			updateBouncingLinear();
	BOOL			updateBigBullet();
	BOOL			jump(int _iX,int _iY,int _iSkill);

	void			operateSkillEffectForCaster(cAbility *_lpAbility);	//	스킬 기타 정보 적용
	void			operateSkillEffectForTarget(cAbility *_lpAbility);	//	스킬 기타 정보 적용
	void			castIcyStalagmite(SG_ICY_STALAGMITE	*_lpPacket);
	void			releaseStalagmite();
	void			shutInMagicBox();
	void			crushMagicBox(BOOL _bIsDeath);
//┃CActorSkill.cpp
//┃스킬 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃상태이상관련
//┃CActorWrongStatus.cpp
	void			revive();
	void			cure(int _iSkill,int _iHealPoint);	//	힐
	void			absorbDamage(int _iHealPoint);	//	힐
	
	void			cureStrangeStatus();
	void			cureDeclinePowerStatus();
	void			cureCurseStatus();
	void			cureWrongStatus();
//┃CActorWrongStatus.cpp
//┃상태이상관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃상태이상관련
//┃CActorStatus.cpp
	void			operateRecoverHP();
	void			increaseHP(int _iHP);
//┃CActorStatus.cpp
//┃상태이상관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃액션 관련
	BOOL			attackToObject(int _iTarget,int _iTargetType,cAbility *_lpAbility);
	BOOL			attackToActor(CActor *_lpTarget,cAbility *_lpAbility,int _iRange);	//	특정 캐릭터를 공격(찾아가서 때린다.)
	BOOL			attackToGroup(CActor *_lpTarget,cAbility *_lpAbility,int _iRange,WORD *_lpwGroup,int _iTargetCount,int _iContinuousShotCount,int _iCriticalCount,int _iMissCount,int _iDoubleCriticalCount, int _iHardBlowCount);
	BOOL			attackToActorByContinuousAttack(SG_CONTINUOUS_HIT_BY_VALUE	*_lpPacket);	//	연속공격 스킬로 공격!!
	BOOL			landMarkerAttack(SG_LAND_MARKER *_lpPacket);	//	랜드 마커 공격
	BOOL			castIntervalShooter(SG_CAST_INTERVAL_SHOOTER *_lpPacket);	//	인터벌 슈터
	BOOL			attackToActorByBunshine(SG_BUNSHINE_ATTACK *_lpPacket);	//	연속공격 스킬로 공격!!
	BOOL			attackToActorByScimitarCutting(SG_SCIMITAR_CUTTING *_lpPacket);	//	연속공격 스킬로 공격!!
	BOOL			operateAwesomeFortressAttack(SG_WIDE_AREA_DAMAGE *_lpPacket);	//	오우섬 포트리스
	BOOL			operateChainLightningSkill(SG_WIDE_AREA_DAMAGE *_lpPacket);	//	체인 라이트닝
	BOOL			operateBouncingLinear(SG_WIDE_AREA_DAMAGE *_lpPacket);	//	바운싱 리니어
	BOOL			operateBigBullet(SG_WIDE_AREA_DAMAGE *_lpPacket);	//	거대 탄환
	BOOL			operateWideAreaDamage(SG_WIDE_AREA_DAMAGE *_lpPacket);	//	광역 대미지
	BOOL			operateBouncingMissileSkill(SG_WIDE_AREA_DAMAGE *_lpPacket);	//	바운싱미사일스킬
	BOOL			operateWideMissileSkill(SG_WIDE_AREA_DAMAGE *_lpPacket);	//	범위미사일스킬	
	
	void			setContinuousAttackDamage(DWORD _dwPhysicalDamage,DWORD _dwMagicDamage,int _iCount,int _iMissCount,int _iCriticalCount,int _iDoubleCriticalCount,BOOL _bIsAttackToShakle,
												int _iMinPhysicalDamage,int _iMinMagicDamage,int _iBlockingCount,int _iDancingBlockingCount,int _iNoActionBlockingCount,CHitInfo *_lpHitInfoList=NULL, int _iHardBlowCount = 0);
	BOOL			attackToGround(int _iX,int _iY,cAbility *_lpAbility,int _iRange);			//	특정 장소로 이동하면서 시야에 들어오는 적 공격
	BOOL			quickAction(cAbility *_lpAbility);	//	퀵 액션을 사용한다.
	BOOL			reserveAttack(int target,int ability);

	void			dodgeAction(CActor *_lpTarget,int _iSpentCP,int _iDodgeAbility);	//	회피 액션

	inline	void	bookingAction(int action);
	inline	void	changeAction(int anm,int booking);


	BOOL			operateContinuousHitOfDoppelgangerAttack(SG_ACTION_TO_GROUP *_lpPacket);	//	분신연속공격
//┃액션 관련
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃서버에서 받은 패킷 처리
	void			operateMovePacket(SG_MOVE *_lpMovePacket);//	이동을 한다.
	void			operateStopPacket(SG_STOP *_lpStopPacket);	
	void			operateThrustPacket(SG_THRUST *_lpPacket);	
	void			operateMoveResult(SG_MOVE_RESULT *_lpMovePacket);//	이동결과를 처리한다.
//┃서버에서 받은 패킷 처리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
};

extern	CActor			*g_lpHero;
extern	CActor			*g_lpControlActor;
extern	CActor			g_aActor[dMAX_ACTOR];		//	액터들..
extern	BOOL			g_bObserverMode;

class	CActorManager	:	public CGamePlay
{
public:
	cScreenFocus	m_screenFocus;
	CPos			m_posGroundFocus,m_posStress;
	WORD			m_wFocusActor;
	cRECT			m_rectScreen;
	int				m_iActorCount,m_aActorList[dMAX_ACTOR];	//	캐릭터 리스트,캐릭터 수
	int				m_iLastAskActorInfoTime;	//	마지막으로 액터 정보를 요청한 시간.
	CPos			m_posScreenDelta,m_posFocus;	//	화면과 영웅의 위치 차이
	int				m_iScreenFollowTime;//	화면이 영웅을 쫒기 시작한 시간..
	int				m_iTraceTarget,m_iBookedTarget;
	int				m_iIsHideTimathFeelerTime;

	CDetectingEvilTargetInfo	m_aDetetingActorList[dMAX_DETECTING_EVIL_TARGET_COUNT];
	int				m_iDetectingActorCount,m_iDetectingActorTime;//	화면이 영웅을 쫒기 시작한 시간..
	char			m_astrDenyCommunityActor[256][dNAME_LENGTH];
	int				m_iDenyCommunityActorCount,m_iSmoothScrollTime;
	
	WORD			m_wObscurityActorCount,m_wWaitObscurityActorInfoResultTime,m_aObscurityActorList[dMAX_REQUEST_ACTOR_DATA_COUNT];
	WORD			m_wAskInfoActorCount,m_wWaitAskActorInfoResultTime,m_aAskInfoActorList[dMAX_REQUEST_ACTOR_DATA_COUNT];
	WORD			m_wMisplacedActorCount,m_wWaitMisplacedActorResultTime,m_aMisplacedActorList[dMAX_MISPLACED_ACTOR_COUNT];//서버에서 이벤트 발생했다고 했는데.. 실제로 없는 캐릭
	WORD			m_wAskActorEnchantEffectInfoCount,m_wAskActorEnchantEffectInfoResultTime,m_awAskActorEnchantEffectInfoList[dMAX_MISPLACED_ACTOR_COUNT][2];//서버에서 이벤트 발생했다고 했는데.. 실제로 없는 캐릭

	WORD			m_wCurrentIndex;

	WORD			m_wOXQuizLoserCount,m_wOXQuizSurvivorCount;
	WORD			m_wOXQuizLoserCountInServer,m_wOXQuizSurvivorCountInServer;
	WORD			m_wOXQuizWinnerCount,m_wOXQuizBattleCount;

	WORD			m_wTalker;//서버에서 이벤트 발생했다고 했는데.. 실제로 없는 캐릭

					CActorManager();

	void			init();									//	최초의 초기화
	void			reset();								//	리셋
	void			close();								//	클로즈
	void			update();
	void			setFocus();								//	선택된 캐릭터에 포커스를 맞춘다.
	void			getFocusPos(CPos *_lpPos);
	void			setGroundFocus(int _iX,int _iY,int _iTime);
	BOOL			MoveObserverPos();
	void			setCharacterFocus(int _iActor);
	void			releaseKarmaFocus();

	//JBC			옵져버 모드를 위한 변수& 함수 08-07-15
	CPos			m_posObserber;
	int				m_iObserberTime , m_iObserberWaitTime;		//카르마에의한 옵져버 모드의 스피드와 시간.
	float			m_fObserberRemainX , m_fObserberRemainY , m_fObserberSpeedX , m_fObserberSpeedY;
	void			ChangeObserverMode(BOOL _isByKarma = FALSE , int _iX = 0 , int _iY = 0, int _iTime = 0, int _iTimeForWait = 0);
	void			OperateObserverKeyboard();
	void			OperateObserverMouse();
	void			SetCorrectPos(CPos* _Pos);
	void			SetViewPort();
	void			SetViewPortLargeMargin();
	inline CPos		GetObserverPos()	{return m_posObserber;}
	void			setObserverMode();

	void			setHero(CHero *_lpPlayer);//플레이어 데이터 설정
	void			rebuildActorList();						//	액터 리스트 업데이트
	void			findFocusActor();						//	마우스가 가르키고 있는넘을 찾아라!!
	void			findFocusCorpse();						//	마우스가 가르키고 있는... 시체를-o- 찾아라!!
	int				getViewActors(cINDEX *_lpIndexes,cRECT *_lpRectViewport);//_lpRectViewport안에 보이는 넘들
	BOOL			addToAskInfoActorList(int _iActorSerial);		//	잘못된 액터 데이터 리스트 추가
	BOOL			addObscurityActor(int _iActorSerial);		//	매우 간단한 캐릭터 정보를 얻기위한 리스트에 추가
	BOOL			addMisplacedActor(int _iActorSerial);//	위치 업데이트가 필요한 액터들
	BOOL			addAskActorEnchantEffectInfo(int _iActorSerial,int _iWork);
	void			sendAskInfoActorList();					//	서버에 잘못된 액터 데이터의 교정을 요구한다.
	void			sendObscurityActorList();
	void			sendMisplacedActorList();				//	위치가 맞지 않는 액터 리스트
	void			sendAskActorEnchantInfoList();

	char*			getNextPlayerName();
	char*			getPreviousPlayerName();

	void			addDenyCommunityActor(char *_lpstrName);
	BOOL			isDenyCommunityActor(char *_lpstrName);
	void			resetDenyCommunityActorList();

	int				getScreenActorList(CActor	**_alpActorList,int _iBufferSize);

	BOOL			checkDeathStatus(CActor *_lpActor);		//	
	CActor*			getActorByName(char *_lpstrName);
	inline	CActor*	getActor(int _iSerial){return &g_aActor[_iSerial];}	//	걍  리턴
	inline	CActor*	getTestedActor(int _iSerial,BOOL _bIsAddValidActorList=TRUE)	//	이것저것 확인한후 리턴
	{
		if (_iSerial < 0 || _iSerial >= dMAX_ACTOR)	return NULL;

		if (g_aActor[_iSerial].m_wSerial==	0xffff	||	g_aActor[_iSerial].m_wSyncStatusWithServer	==	eSSWS_VALID_ACTOR)
		{
			if (_bIsAddValidActorList)
				addObscurityActor(_iSerial);
			return	NULL;
		}

		return	&g_aActor[_iSerial];
	}	//	CActorManager::getTestedActor(int _iSerial)

	void			killActor(int _iSerial,BOOL _bIsInstanceKill,int _iDeathPeneltyTime,BOOL _bIsReleaseSummon);//죽여!!
	void			removeActor(int _iSerial,BOOL _bIsHide=FALSE);//캐릭터 제거
	void			resetActor(int _iSerial);//	캐릭터 초기화
	void			resetQuizEventStatus();//	캐릭터 초기화
	void			reviveOXQuizLoser();	//	OX 퀴즈 패자부활
	void			receiveOXQuizWinnerList(WORD *_lpwWinnerList,int _iCount);

	BOOL			joinActor(CSimplePlayerInfo *_lpActor,BOOL _bIsDummy=FALSE,BOOL _bIsCopy=FALSE);//플레이어 참가
	BOOL			joinMonster(CSimpleMonsterInfo *_lpSimpleActorInfo,BOOL _bIsDummy=FALSE,BOOL _bIsCopy=FALSE);

	BOOL			receiveMinimumActorInfoList(SG_MINIMUM_ACTOR_INFO_LIST *_lpPacketSgExistActor);//기존에 액터들을 추가한다.

	BOOL			receiveSimpleActorInfoList(SG_SIMPLE_ACTOR_INFO_LIST *_lpPacket);
	BOOL			receiveVerySimpleActorInfoList(SG_VERY_SIMPLE_ACTOR_INFO_LIST	*_lpPacket);

	BOOL			receiveSimpleActorInfo(SG_SIMPLE_ACTOR_INFO *_lpPacket);
	BOOL			receiveVerySimpleActorInfo(SG_VERY_SIMPLE_ACTOR_INFO *_lpPacket);

	BOOL			receiveOperatorList(SG_OPERATOR_LIST *_lpPacket);	//	운영자 리스트 받음
	BOOL			operateReceivedRefitActorPostionPacket(SG_REFIT_ACTORS_POSITION *_lpPacket);//	액터들의 위치를 재설정 한다.
	BOOL			operateReceivedOuterActorPostionPacket(SG_OUTER_ACTORS_POS_UPDATE *_lpPacket);//	액터들의 위치를 재설정 한다.

	BOOL			updateInpdev();

	inline	BOOL	OperateFalse(int _iState);
	BOOL			updateActor();
	BOOL			updateFieldItem();
	BOOL			updateArea();
	BOOL			updateTrap();

	inline	BOOL	updateMove();

	void			attatchItemUseEffect(int _iWho,cItem *_lpItem);	//	_iWho에게 _lpItem 사용
	void			changeAppearance(int _iSerial,SG_CHANGE_APPEARANCE *_lpPacket);	//	외관이 변했다.
	void			drawDeathActors(cRECT *_lpRectViewport);	//	죽은 넘들을 그려준다.
	
	inline	void	put(int _iSerial){g_aActor[_iSerial].put();}//	캐릭터를 찍는다.
	inline	void	putDummy(int _iSerial){g_aActor[_iSerial].putDummy();}
	inline	void	addPaint(int _iSerial){g_aActor[_iSerial].addPaint();}
};

extern	CActorManager	g_am;						//	액터 메니져
extern	BOOL			g_isSmoothFocusing;			//	주인공에 포커스를 맞출때 스무스~ 하게 맞춘다.
extern	BOOL			g_isHeroFocusOn;			//	포커스 맞추기 on


#include	"CActorInline.h"

#endif