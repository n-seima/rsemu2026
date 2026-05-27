#ifndef	_classCHARACTER_H
#define	_classCHARACTER_H

#include "cJOB.H"
#include "cKARMA.H"

#define	dMAX_CHARACTER					256
#define	dCHARACTER_SELF					511
#define	dCHARACTER_ALL					512
#define	dCHARACTER_TARGET_PLAYER		513
#define	dCHARACTER_TARGET_PARTY			514
#define	dCHARACTER_PLAYER				515

//
//	Event Check Condition 
enum
{
	eMECC_KILLED_BY_PLAYER,
	eMECC_BEGIN_BATTLE,
	eMECC_KILL_PLAYER,
	eMECC_ALWAYS,
	eMECC_COUNT,
};

class	CCharacterBaseInfo_old_before090717
{
public:
	WORD			m_wSerial;
	WORD			m_wBaseJob;
	WORD			m_wMinLevel;
	char			m_strName[20];	//	이름
	WORD			m_wCorrectExp;
	WORD			m_wCorrectHP;
	WORD			m_wCorrectAP;
	WORD			m_wCorrectFinalDamage;
	WORD			m_wScaleX,m_wScaleY;
	WORD			m_wMaxLevel;

	DWORD			m_bf1IsInvincible			:	1;
	DWORD			m_bf1IsNotDropGold			:	1;
	DWORD			m_bf1IsNotDropItem			:	1;
	DWORD			m_bf1IsNotExp				:	1;
	DWORD			m_bf8CorrectUseSkillChance	:	8;
	DWORD			m_bf1IsKeyMonster			:	1;
	DWORD			m_bf8Skin					:	8;

	DWORD			m_bf1IsImmuneStun			:	1;
	DWORD			m_bf1IsImmuneStone			:	1;
	DWORD			m_bf1IsImmuneCharming		:	1;
	DWORD			m_bf1IsImmuneFreeze			:	1;
	DWORD			m_bf1IsImmuneKnockBack		:	1;
	DWORD			m_bf1IsReviseIntelligence	:	1;

	DWORD			m_bf1IsImmunePhysicalDamage	:	1;
	DWORD			m_bf1IsImmuneMagicDamage	:	1;

	DWORD			m_bf1IsImmuneConfuse		:	1;
	DWORD			m_bf1IsSetBoss				:	1;		//31
};

class	CCharacterBaseInfo
{
public:
	WORD			m_wSerial;
	WORD			m_wBaseJob;
	WORD			m_wMinLevel;
	char			m_strName[20];	//	이름
	WORD			m_wCorrectExp;
	WORD			m_wCorrectHP;
	WORD			m_wCorrectAP;
	WORD			m_wCorrectFinalDamage;
	WORD			m_wScaleX,m_wScaleY;
	WORD			m_wMaxLevel;

	DWORD			m_bf1IsInvincible			:	1;
	DWORD			m_bf1IsNotDropGold			:	1;
	DWORD			m_bf1IsNotDropItem			:	1;
	DWORD			m_bf1IsNotExp				:	1;
	DWORD			m_bf8CorrectUseSkillChance	:	8;
	DWORD			m_bf1IsKeyMonster			:	1;
	DWORD			m_bf8Skin					:	8;

	DWORD			m_bf1IsImmuneStun			:	1;
	DWORD			m_bf1IsImmuneStone			:	1;
	DWORD			m_bf1IsImmuneCharming		:	1;
	DWORD			m_bf1IsImmuneFreeze			:	1;
	DWORD			m_bf1IsImmuneKnockBack		:	1;
	DWORD			m_bf1IsReviseIntelligence	:	1;

	DWORD			m_bf1IsImmunePhysicalDamage	:	1;
	DWORD			m_bf1IsImmuneMagicDamage	:	1;

	DWORD			m_bf1IsImmuneConfuse		:	1;
	DWORD			m_bf1IsSetBoss				:	1;		//31
	DWORD			m_bf1IsBlockToTame			:	1;		//32

	short			m_asAbsorbMagicDamage[7][2];	//	6개 속성별 발동 확률과 흡수량. 0 발동 확률 1 흡수량
};

class	cCHARACTER	: public CCharacterBaseInfo
{
public:
	CKarmaContainer	*m_pEvent[eMECC_COUNT];	//	이벤트

					cCHARACTER()
					{
						memset(this,0,sizeof(CCharacterBaseInfo));

						m_wSerial	=	0xffff;

						for (int i=0;i<eMECC_COUNT;i++)
							m_pEvent[i]		=	NULL;
					}

	cJOB			*getJob();
	BOOL			isExistEvent();
	inline	void	reset()		
	{
		memset(this,0,sizeof(CCharacterBaseInfo));

		for (int i=0;i<eMECC_COUNT;i++)
			KILL(m_pEvent[i]);

		m_wSerial					=	0xffff;
		m_wBaseJob					=	0xffff;
		m_wMinLevel					=	1;
		m_wMaxLevel					=	1;
		m_wScaleX					=	100;
		m_wScaleY					=	100;
		m_wCorrectFinalDamage		=	10000;

		m_wCorrectExp				=	100;
		m_wCorrectHP				=	100;
		m_wCorrectAP				=	100;

		m_bf1IsInvincible			=	0;
		m_bf1IsNotDropGold			=	0;
		m_bf1IsNotDropItem			=	0;
		m_bf1IsNotExp				=	0;
		m_bf8CorrectUseSkillChance	=	0;

		m_bf1IsImmuneConfuse		=	0;
		m_bf1IsSetBoss				=	0;

		strcpy(m_strName,"");
	}
	int				getBody(){return getJob()->m_wBody;}
	char*			getJobName(){return getJob()->m_strName;}
	void			copy(cCHARACTER *_lpCharacter);

	inline	int		getAvailConditionCount()
	{
		int	iCount	=	0;

		for (int i=0;i<eMECC_COUNT;i++)
			if (m_pEvent[i])
				iCount++;

		return	iCount;
	}
};


//
//	직업 세트
class	cCHARACTER_SET
{
public:
	cCHARACTER			m_aCharacter[dMAX_CHARACTER];
	int					m_iCount,m_iSelectCharacter;
	int					m_iAutoIndex,m_iAutoIndexCount;

						cCHARACTER_SET();

	BOOL				add(cCHARACTER *_lpCharacter);
//	BOOL				add(int _iJob,int _iMinLevel,int _iMaxLevel,char *_strName);
	BOOL				remove(int index);
	BOOL				removeSelectCharacter();
	void				reset();
	void				setSelectCharacter();		//	선택된 직업이 없다면 젤 처음에 있는넘으러 선택한다.

	cCHARACTER			*getSelectCharacter();		//	선택된 직업을 리턴
	cCHARACTER			*getCharacter(int index);	//	index번째 직업 리턴
	cJOB				*getJob(int index);

	int					getIndex(int count);
	int					getIndex(char *name);
	char				*getName(int index);


	int					getSelectCharacterJob();
	int					getSelectCharacterBody();
	int					getBody();
	int					getBody(int index);			//	몸뚱아리 얻어 오기

	int					getDataSize();				//	동시에 로드해야할 직업들의 스프라이트 데이터 사이즈를 구한다.
	int					getBodySize();				//	선택된 직업의 몸통 사이즈
	int					getBodySize(int index);		//	index번째 직업의 몸통 사이즈

	void				resetIndex()			{	m_iAutoIndex	=	0,m_iAutoIndexCount	=	0;}
	BOOL				reBuild();					//	쭈욱~~ 훑어서... 사라진 직업을 캐릭터로 가지고 있는건 지워버린다.

	cCHARACTER			*getNextCharacter();			//	직업을 하나씩 얻어온다.
	inline	BOOL		isAvailCharacter(int index)	//	index직업을 하나씩 얻어온다.
						{
							if (m_aCharacter[index].m_wSerial	==	0xffff)	return	FALSE;

							return	TRUE;
						}

	int					getAvailIndex()
						{
							for (int i=0;i<dJOB_COUNT;i++)
								if (m_aCharacter[i].m_wSerial	==	0xffff)	return	i;

							return	0xffff;
						}
};

extern	char		*g_strMonsterEventCheckCondition[];
extern	char		*g_astrNpcSkin[];

extern	int			GetCorrectExpByLevelGap(int iSharedExp,int _iPlayerLevel);

#endif