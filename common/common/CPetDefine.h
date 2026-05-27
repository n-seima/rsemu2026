#ifndef	_classPetDefine_H
#define	_classPetDefine_H

#include "SFC.H"

#define	dPET_NAME_LENGTH			10
#define	dOWN_PET_COUNT				2
#define	dPET_LOYALTY_UPDATE_PERIOD	30
#define	dPET_MINIMUM_SCALE			70

//
//	Tamed Monster Duty;
enum
{
	eTMD_READY,
	eTMD_ATTACK_ONE,
	eTMD_ATTACK_ALL,
	eTMD_PATROL,
	eTMD_DEFENSE_POSITION,
};

#pragma pack(1)

class	cPetAndBook_common
{
public:
	char	m_strName[dPET_NAME_LENGTH];
	WORD	m_wBreedingTime;	//	델꾸 다닌 시간
	WORD	m_bf14Loyalty	:	14;	//	충성도
	WORD	m_bf2TamerSlot	:	2;	//	주인 슬롯
};

class	cPetDefine : public cPetAndBook_common
{
public:
	DWORD	m_bf10Level		:	10;
	DWORD	m_bf22CurrentHP	:	22;
	WORD	m_bf10Job		:	10;	//	합쳐서 48비트(6바이트)
	WORD	m_bf5BoostCount	:	5;
	WORD	m_bf1Job		:	1;

	DWORD	m_dwExperience;		//	경험치

	int		getJobIndex()
	{
		int	iJobIndex	=	m_bf10Job;

		if	(m_bf1Job)
			iJobIndex	+=	1024;

		return	iJobIndex;
	}
	void	setJobIndex(int _iJobIndex)
	{
		m_bf10Job	=	_iJobIndex%1024;
		m_bf1Job	=	_iJobIndex/1024;
	}
	void	reset()
	{
		memset(this,0,sizeof(cPetDefine));
	}

	char*	getName();
	char*	getPetCode();
	char*	getBookCode(int _iBookIndex,unsigned int _uiSerial);
};

class	cBreedingRecordBookExInfo
{
public:
	DWORD	m_bf31Experience:	31;	//	경험치 31비트
	DWORD	m_bf1IsNewBRB	:	1;	//	이게 0 이勇 涌날 책쳄다.
};

class	cBreedingRecordBookDefine : public cPetAndBook_common
{
public:
	DWORD	m_bf10Level		:	10;
	DWORD	m_bf11TamerLevel:	11;	//	주인 레벨
	DWORD	m_bf4TamerSlot	:	4;	//	주인 슬롯
	DWORD	m_bf7Spare		:	7;	//	7비트 남음

	WORD	m_bf10Job		:	10;	//	합쳐서 48비트(6바이트)
	WORD	m_bf5BoostCount	:	5;
	WORD	m_bf1Job		:	1;

	DWORD	m_dwExperience;		//	경험치

	unsigned int	buildByPetInfo(cPetDefine *_lpPetInfo,int _iAvatarIndex,int _iLevel);
	int		getAvatarIndex();
	BOOL	isAvailInfo(unsigned int _uiExValue);

	int		getAvatarLevel();
	void	makePetInfo(cPetDefine *_lpPetInfo,unsigned int _uiExValue);
};

#pragma pack()

#endif