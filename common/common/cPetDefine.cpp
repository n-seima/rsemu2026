#include	"CPetDefine.h"

char*
cPetDefine::getName()
{
	static	char	strName[32];

	memcpy(strName,m_strName,10);

	strName[10]	=	NULL;

	return	strName;
}

char*
cPetDefine::getPetCode()
{
	static	char	strPetCode[256];

	UINT	uiExp	=	m_dwExperience;
	int		iTamerSlot	=	m_bf2TamerSlot;
	
	if	(uiExp & 0x80000000)
	{
		uiExp		-=	0x80000000;
		iTamerSlot	=	m_bf22CurrentHP;
	}

	sprintf(strPetCode,"[\"%s\"] [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d] [%d]",
		getName(),m_wBreedingTime,m_bf14Loyalty,iTamerSlot,m_bf10Level,m_bf22CurrentHP,
		m_bf10Job,m_bf5BoostCount,m_bf1Job,uiExp);

	return	strPetCode;
}

char*
cPetDefine::getBookCode(int _iBookIndex,unsigned int _uiSerial)
{
	cBreedingRecordBookDefine	*lpBRBook	=	(cBreedingRecordBookDefine*)this;

	int	iTamerLevel=0,iTamerSlot=0;

	if	(lpBRBook->isAvailInfo(_uiSerial))
	{
		iTamerLevel	=	lpBRBook->m_bf11TamerLevel;
		iTamerSlot	=	lpBRBook->m_bf4TamerSlot;
	}
	else
	{
		iTamerSlot	=	m_bf2TamerSlot;
	}

	static	char	strBookCode[256];

	sprintf(strBookCode,"#br [%d:%d] name \"%s\" avatarIndex [%d] code [%d,%d,%d,%d,%d,%d,%d]",
			_iBookIndex,_uiSerial,getName(),iTamerSlot,
			m_wBreedingTime,m_bf14Loyalty,m_bf10Level,0,getJobIndex(),m_bf5BoostCount,iTamerLevel);

	return	strBookCode;
}

int
cBreedingRecordBookDefine::getAvatarIndex()
{
	return	m_bf4TamerSlot;
}

int
cBreedingRecordBookDefine::getAvatarLevel()
{
	return	m_bf11TamerLevel;
}

int
cBreedingRecordBookDefine::isAvailInfo(unsigned int _uiExValue)
{
	cBreedingRecordBookExInfo	exInfo;

	memcpy(&exInfo,&_uiExValue,4);

	return	exInfo.m_bf1IsNewBRB;
}

unsigned	int
cBreedingRecordBookDefine::buildByPetInfo(cPetDefine *_lpPetInfo,int _iAvatarIndex,int _iLevel)
{
	memcpy(this,_lpPetInfo,sizeof(cPetAndBook_common));

	m_bf10Level					=	_lpPetInfo->m_bf10Level;
	m_bf11TamerLevel			=	_iLevel;	//	주인 레벨/10
	m_bf4TamerSlot				=	_iAvatarIndex;
	m_bf7Spare					=	0;
	
	m_bf10Job					=	_lpPetInfo->m_bf10Job;
	m_bf5BoostCount				=	_lpPetInfo->m_bf5BoostCount;
	m_bf1Job					=	_lpPetInfo->m_bf1Job;

	unsigned	int	uiExpValue	=	_lpPetInfo->m_dwExperience;
	uiExpValue					|=	0x80000000;

	return	uiExpValue;
}

void
cBreedingRecordBookDefine::makePetInfo(cPetDefine *_lpPetInfo,unsigned int _uiExValue)
{
	memcpy(_lpPetInfo,this,sizeof(cPetAndBook_common));

	if	(_uiExValue	|	0x80000000)
		_uiExValue	-=	0x80000000;

	_lpPetInfo->m_bf2TamerSlot	=	0;

	_lpPetInfo->m_bf10Level		=	m_bf10Level;
	_lpPetInfo->m_bf22CurrentHP	=	0x3fffff;
	_lpPetInfo->m_bf10Job		=	m_bf10Job;
	_lpPetInfo->m_bf5BoostCount	=	m_bf5BoostCount;
	_lpPetInfo->m_bf1Job		=	m_bf1Job;
	_lpPetInfo->m_dwExperience	=	_uiExValue;
}