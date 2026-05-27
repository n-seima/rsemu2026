#include "CTitle.h"
#include "SFC.h"

CTitleManager	g_titleManager;

//
//	타이틀 데이터 로딩
BOOL
CTitleManager::init()
{
	cFILE	file;

	if (!file.Open("data/title.dat","rb"))
		return	ERRMSG("error in CTitleManager::init()","can not find 'data/title.dat' file");

	file.Read(&m_iTitleCount,4);
	file.Read(m_aTitles,sizeof(m_aTitles));

	return	TRUE;
}

//
//	_iLevel 타이틀을 위해 필요한 레벨
int
CTitle::getRequireLevel(int _iLevel)
{
	if (m_wComputeRequireLevelMethod	==	eCRLM_LEVEL_VALUE	)	return	m_wRequireLevelValue;
	if (m_wComputeRequireLevelMethod	==	eCRLM_MULTIPLE_VALUE)	return	_iLevel*m_wRequireLevelValue;
	if (m_wComputeRequireLevelMethod	==	eCRLM_SQUARE_VALUE	)	return	_iLevel^m_wRequireLevelValue;

	return	0xffff;	
}

//
//	타이틀이 적용되는 레벨 구하기
int
CTitle::getEnableLevel(int _iTitleLevel,int _iOwnersLevel)
{
	int	iLevel;

	for (iLevel=_iTitleLevel;iLevel>=1;iLevel--)
		if (_iOwnersLevel >= getRequireLevel(iLevel))
			return	iLevel;

	return	0;
}