#ifndef _classJOB_H
#define	_classJOB_H

#include "cNEMO.H"
#include "cItem.H"
#include "CSkill.H"
#include "definePacketData.H"
#include "jobDefine.h"
#include "playerJobCommon.h"
#include "jobCommon.h"

enum
{
	eNPC_PLAYER,			//	플레이어
	eNPC_NORMAL,			//	보통 NPC
	eNPC_MONSTER,			//	몬스터

	eNPC_WEAPON_MERCHANT,	//	무기 상인
	eNPC_ARMOR_MERCHANT,	//	방어구 상인
	eNPC_ETC_ITEM_MERCHANT,	//	잡화 상인
	eNPC_ACCESSORY_MERCHANT,//	도구 상인
	eNPC_PITCHMAN,			//	노점상
	eNPC_BANK,
	eNPC_SKILL_MASTER,

	eNPC_NORMAL_QUEST,
	eNPC_TITLE_QUEST,
	eNPC_GUILD_QUEST,
	eNPC_MAJOR_QUEST,
	eNPC_DOUMI,
	eNPC_TELEPORTER,

	eNPC_HEALER,
	eNPC_QUEST_MANAGER,
	eNPC_SMITH,
	eNPC_FIRST_HELPER,

	eNPC_PROCESS_QUEST_CLIENT,
	eNPC_PROCESS_QUEST_REFERENCE_NPC,
	eNPC_PROCESS_QUEST_TARGET_MONSTER,

	eNPC_WEAPON_MERCHANT_KNIGHT_WARRIOR,	//	"견습기사/전사 무기상",
	eNPC_WEAPON_MERCHANT_WIZARD_WEREWOLF,	//	"마법사/늑대인간 무기상",
	eNPC_WEAPON_MERCHANT_PRIEST_FALLEN_ANGEL,	//	"사제/타락천사 무기상",
	eNPC_WEAPON_MERCHANT_ROGUE_FIGHTER,	//	"도둑/무도가 무기상",
	eNPC_WEAPON_MERCHANT_LANCER_ARCHER,	//	"마법 창검사/마법 궁수 무기상",
	eNPC_WEAPON_MERCHANT_TAMER_SUMMONER,	//	"조련사/소환사 무기상",
	eNPC_WEAPON_MERCHANT_PRINCESS_MAGICAL_GIRL,	//	"공주/변신 소녀 무기상",
	eNPC_WEAPON_MERCHANT_NECROMANCER_DEVIL,	//	"강신술사/악마 무기상",

	eNPC_REWARD_FOR_DUEL	,	//	"결투장 보상",
	eNPC_WEAPON_MERCHANT_SOUL_BRINGER,	//	"영술사 무기상",
	eNPC_GUILD_HALL_TELEPORTER,		// "길드홀 텔레포터"
	eNPC_EVENT_HELPER,		// "이벤트 도우미"
	eNPC_ADVENTURER_SOCIETY,		// "모험가 협회"
	eNPC_WEAPON_MERCHANT_OPTICALIST,	//	"광학사무기상"
};	//	NPC 종류


#define	dJOB_BORDER									100
#define	dMAX_CHARACTER_COUNT						256
#define	dMAX_LOAD_BODY								100
#define	dMAX_MONSTER_ABILITY_COUNT					10

#define	dMAX_JOB_COUNT								2048
#define	dJOB_GROUP_HERO								0
#define	dJOB_GROUP_MONSTER							1
#define	dJOB_GROUP_NPC								2

#define	dJOB_SET_HERO								0
#define	dJOB_SET_CHARACTER							1

#define	dJOB_MONSTER								200		//	이거 순전히 사용 가능 
#define	dJOB_SUMMON_BEAST							50

#define	dACT_READY									2

#define	dCHARACTER_COUNT							256
#define	dBODY_COUNT									300


#define	dJOB_VALID									0x7fff

//
//	캐릭터 정보
class	cCHARACTER
{
public:
	WORD			m_wSerial;
	WORD			m_wBaseJob;
	WORD			m_wMinLevel;
	char			m_strName[32];	//	이름
	WORD			m_wMaxLevel;
};	//	class	cCHARACTER

class	cJOB_DATA : public cNpcData_common
{
public:
};

class	cJOB		:	public cJOB_DATA,public CJobBasicDataDefine
{
public:
};	//	class	cJOB

//
//	클라이언트에서 필요한 기본 정보
class	CJobInfo
{
public:
	char				m_strName[32];		//	직업 이름1
	BYTE				m_abBodyShape[4];					//	몬스터 바디 쉐입

	WORD				m_wSerial;
	int					m_iBody;			//	몸통
	int					m_iVariation;
	WORD				m_wPalette;
	WORD				m_wPortrait;		//	포트릿
	WORD				m_wType;			//	영웅,NPC,몬스터
	WORD				m_wRank,m_wKind;

	WORD				m_bf1IsZinMonster	:	1;
	WORD				m_bf1IsExMonster	:	1;
	WORD				m_bf1IsEnableTameMonster	:	1;

	WORD				m_awOwnedSkill[dMAX_MONSTER_ABILITY_COUNT];
	
						CJobInfo()
						{
							m_wSerial	=	0xffff;
						}
	void				setData(cJOB*	_lpJob);
};

//
//	직업 메니져
class	CJobManager
{
public:
	CJobInfo		m_aJob[dMAX_JOB_COUNT];
	int				m_iCharacterDataCount;
	WORD			m_aJobList[dMAX_CHARACTER_COUNT];

	BOOL			init();//초기화
	BOOL			loadLocalBody(cNUX *_lpNux,cMCU *_lpMcu);//이 지역에서 사용하는 몸뚱아리 로딩
	void			close();	//	제거
	inline	int		getPalettIndex(int _iJob)	{return	m_aJob[_iJob].m_wPalette;}
	CJobInfo*		getJob(int _iJob)			{return	&m_aJob[_iJob];}
	CJobInfo*		getJob(char *_lpstrName);
	BOOL			readDefaultEquipmentData();
	CJobInfo*		getBaseJob(int _iJob)		
	{
		for (int i=0;i<dMAX_JOB_COUNT;i++)
		{
			if (m_aJob[i].m_iBody	==	_iJob+dJOB_MONSTER_START)
				return	&m_aJob[i];
		}

		return	NULL;
	}
};


extern	CJobManager	g_jm;

//	CTR	-	Change Transformation Result
enum
{
	eCTR_OK	,
	eCTR_IS_NOT_TRANSFORMATION_JOB	,
	eCTR_IS_NOT_TRANSFORMATION_TIME	,
	eCTR_CAN_NOT_TRANSFORMATION_IN_SITDOWN_STATUS,
	eCTR_IS_REQUIRE_TRANSFORMATION_CP,
	eCTR_REQUIRE_SECOND_JOB_SKILL,
};

enum
{
	eML_NORMAL1	=	0,
	eML_NORMAL2,
	eML_NORMAL3,
	eML_NORMAL4,
	eML_SEMI_BOSS1,
	eML_SEMI_BOSS2,
	eML_SEMI_BOSS3,
	eML_BOSS1,
	eML_BOSS2,
	eML_BOSS3,
};


#endif