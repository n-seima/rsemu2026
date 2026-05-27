#ifndef __ACTORINFO_H
#define __ACTORINFO_H

#include "cPACKET_BASE.h"

#define	dEQUIP_WEAPON				0			//	무기 1
#define	dEQUIP_ARMOR				1			//	갑옷
#define	dEQUIP_GLOVE				2			//	장갑
#define	dEQUIP_HELM					3			//	헬멧 종류
#define	dEQUIP_EARING				4			//	귀거리
#define	dEQUIP_NECKLACE				5			//	목걸이
#define	dEQUIP_BELT					6			//	벨트
#define	dEQUIP_SHOES				7			//	신발
#define	dEQUIP_RING1				8			//	반지
#define	dEQUIP_RING2				9			//	반지
#define	dEQUIP_RING3				10			//	반지
#define	dEQUIP_RING4				11			//	반지
#define	dEQUIP_RING5				12			//	반지
#define	dEQUIP_RING6				13			//	반지
#define	dEQUIP_RING7				14			//	반지
#define	dEQUIP_RING8				15			//	반지
#define	dEQUIP_WEAPON_2				16		//	무기 1
#define	dEQUIP_ARMOR_2				17			//	갑옷
#define	dEQUIP_GLOVE_2				18			//	장갑
#define	dEQUIP_HELM_2				19			//	헬멧 종류
#define	dEQUIP_EARING_2				20			//	귀거리
#define	dEQUIP_NECKLACE_2			21			//	목걸이
#define	dEQUIP_BELT_2				22			//	벨트
#define	dEQUIP_SHOES_2				23			//	신발
#define	dEQUIP_RING1_2				24			//	반지
#define	dEQUIP_RING2_2				25			//	반지
#define	dEQUIP_RING3_2				26			//	반지
#define	dEQUIP_RING4_2				26			//	반지
#define	dEQUIP_RING5_2				27			//	반지
#define	dEQUIP_RING6_2				28			//	반지
#define	dEQUIP_RING7_2				30			//	반지
#define	dEQUIP_RING8_2				31			//	반지
#define	dEQUIP_SHIELD				32			//	방패/탄환
#define	dEQUIP_BELT_SLOT1			33			//	벨트 슬롯 1
#define	dEQUIP_BELT_SLOT2			34			//	벨트 슬롯 2
#define	dEQUIP_BELT_SLOT3			35			//	벨트 슬롯 3
#define	dEQUIP_BELT_SLOT4			36			//	벨트 슬롯 4
#define	dEQUIP_BELT_SLOT5			37			//	벨트 슬롯 5

#define	dMAX_AVATAR				4			//	아이디 하나당 가질 수 있는 아바타의 수
#define dAVATAR_EMPTY			"EMPTY"		//	H 비어있는 아바타 공간

#define	dEQUIPMENT_PART_COUNT	38
#define	dOWN_ITEM_COUNT			40
#define	dABILITY_COUNT			52

typedef class	_cAVATAR_INFO
{
public:
	char					m_strName[dNAME_LENGTH];		//	H 이름 - EMPTY - 대소문자 구분 없구
	WORD					m_wJob;				//	직업
	WORD					m_wLevel;			//	레벨
	WORD					m_aEquip[2][3];		//	H 장비 하고 있는 갑옷, 무기 , 방패
	WORD					m_wLastVillage;		//	마지막에 로그 아웃한 마을 - 서버에서 준다.

	void					init(char *_strName,int _iCharacter)
	{
		strcpy(m_strName,_strName);
		m_wJob			=	_iCharacter*2;
		m_wLevel		=	1;
		memset(m_aEquip,0xff,sizeof(m_aEquip));
		m_wLastVillage	=	0;
	}
}cAVATAR_INFO;	//	class	cAVATAR_INFO

//////////////////////////////////////////////////////////////
//	아바타 정보 받기
//////////////////////////////////////////////////////////////

//	서버에 저장된 직업의 기본 데이터
typedef class	_cAVATAR_BASIC_DATA
{
public:
	int				m_iLevel;							//	레벨
	DWORD			m_dwExperience;						//	경험치

	int				m_iHP;								//	현재 체력			10.00	10,000.00  
	int				m_iMaxHP;							//	최대 체력			10.00	10,000.00  
	WORD			m_wCorrectHP;						//	체력 보정(최대 체력 계산식에서 사용)
	int				m_iCP;								//	현재   파워 게이지   1.00	 5,000.00  
	int				m_iMaxCP;							//	맥시멈 파워 게이지	10.00	10,000.00  
	WORD			m_wCorrectCP;						//	CP보정(최대 CP 계산식에서 사용)

	short			m_sStrength;						//	힘			5 1,000
	short			m_sAgility;							//	민첩성		5 1,000
	short			m_sConstitution;					//	건강		5 1,000
	short			m_sWisdom;							//	지혜		5 1,000
	short			m_sIntelligence;					//	지식		5 1,000
	short			m_sCharisma;						//	카리스마	5 1,000
	short			m_sLuck;							//	운			5 1,000
	short			m_sSight;							//	시야

	short			m_sMinDamage;						//	최소 데미지
	short			m_sMaxDamage;						//	최대 데미지
	short			m_sDefensivePower;					//	방어력

	short			m_sAllignment;						//	성향 -100.00~100.00(absoulte evil,evil,neutral,good,absoulte good)

	short			m_sFireResistance;					//	불 저항력 0% 100.00%  
	short			m_sWaterResistance;					//	물 저항력 0% 100.00%  
	short			m_sWindResistance;					//	바람 저항력 0% 100.00%  
	short			m_sEarthResistance;					//	대지 저항력 0% 100.00%  
	short			m_sLightResistance;					//	빛 저항력 0% 100.00%
	short			m_sDarkResistance;					//	어둠 저항력 0% 100.00%

	short			m_sBlindResistance;					//	장님 저항
	short			m_sPoisonResistance;				//	중독 저항
	short			m_sSleepResistance;					//	잠 저항
	short			m_sColdResistance;					//	콜드 저항
	short			m_sFreezeResistance;				//	프리즈 저항
	short			m_sStunResistance;					//	경직 저항
	short			m_sStoneResistance;					//	석화 저항
	short			m_sConfuseResistance;				//	혼란 저항
	short			m_sCharmingResistance;				//	챠밍 저항

	short			m_sBadStatusResistance;				//	상태 이상공격에 대한 저항 0% 100.00%
	short			m_sDeclinePowerResistance;			//	능력치 저하계에 대한 저항 0% 100.00%
	short			m_sCurseResistance;					//	저주에 대한 저항 0% 100.00%
	short			m_sAllStatusResistance;				//	전체 저항 0% 100.00%
}cAVATAR_BASIC_DATA;	//	class	cAVATAR_BASIC_DATA

//
//	서버에 저장된 어빌러티 데이터
typedef class	_cABILITY_SERVER_DATA
{
public:
	WORD					m_wSkill;	//	베이스 스킬
	WORD					m_wLevel;	//	스킬 레벨
	DWORD					m_dwExp;	//	스킬 경험치
}cABILITY_SERVER_DATA;	//	class	cABILITY_SERVER_DATA

//
//	서버에 저장된 아이템 데이터
typedef class	_cITEM_SERVER_DATA
{
public:
	WORD					m_wBaseItem;				//	아이템 인덱스
	BYTE					m_bCount;					//	숫자
	BYTE					m_bDurability;				//	내구력
	BYTE					m_isSealed;					//	봉인 되어 있다.
	BYTE					m_bUpgradeCount;
	WORD					m_aAddEffect[3];			//	추가 효과 3개 효과
}cITEM_SERVER_DATA;	//	cITEM_SERVER_DATA

//	게임에 참가 했을때 서버로 부터 받는 게임 정보
typedef struct	_SMSG_AVATAR_DATA
{
	cMSG_BASE_TYPE			base;

	WORD					wSerial;						//	자신의 시리얼
	char					strName[dNAME_LENGTH];			//	이름
	WORD					wTeam;							//	팀
	WORD					wCharacter;						//	캐릭터 번호
	WORD					wJob;							//	현재 직업(직업이 둘이니까.. -_-)
	WORD					wXPos,wYPos;					//	자신의 위치 - 상대방에게 넘겨준다. 저장X
	cAVATAR_BASIC_DATA		basicData;
	cABILITY_SERVER_DATA	aAbility[dABILITY_COUNT];		//	어빌러티
	cITEM_SERVER_DATA		aItems[dOWN_ITEM_COUNT];		//	가지고 있는 아이템들
	cITEM_SERVER_DATA		aEquip[dEQUIPMENT_PART_COUNT];	//	착용하고 있는 장비들
}SMSG_AVATAR_DATA;

//	캐릭터 정보,게임에 들어가자 마자 혹은 클라이언트에서 요청할때 날려준다.
typedef struct	_SMSG_ACTOR_INFO
{
	WORD					wSerial;			//	자신의 시리얼
	char					strName[32];		//	이름

	WORD					wLevel;				//	레벨
	WORD					wHP,wMaxHP;			//	레벨
	WORD					wTeam;				//	팀
	WORD					wCharacter;			//	캐릭터 번호
	WORD					wJob;				//	현재 직업
	WORD					wXPos,wYPos;		//	위치

	cITEM_SERVER_DATA		itemWeapon,itemShield,itemArmor,itemBullet;	//	착용 장비
	WORD					wActionSpeed,wMoveSpeed;	//	액션 속도와 이동 속도
	WORD					wMoveToX,wMoveToY;			//	여기로 이동 중이다.
}SMSG_ACTOR_INFO;



#endif	//	__ACTORINFO_H