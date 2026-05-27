#include "partyDefine.h"

int
cPartyInfoForBCS::getAverageLevel()
{
	if	(m_bf3MemberCount	<=	0)
		return	0;

	int	iLevelSum	=	0;
	int	iMaxLevel	=	0;
	int	iMinLevel	=	1000;

	for	(int i=m_bf3MemberCount;i;)
	{
		--i;

		int	iLevel	=	m_aMemberList[i].m_bf10Level;

		iLevelSum	+=	iLevel;

		if	(iLevel	>	iMaxLevel)
			iMaxLevel	=	iLevel;

		if	(iLevel	<	iMinLevel)
			iMinLevel	=	iLevel;
	}

	if	(m_bf3MemberCount	>=	5)
	{
		iLevelSum	-=	(iMaxLevel+iMinLevel);

		return	iLevelSum/(m_bf3MemberCount-2);
	}

	return	iLevelSum/m_bf3MemberCount;
}

int
cPartyInfoForPL::getAverageLevel()
{
	if	(m_bf3MemberCount	<=	0)
		return	0;

	int	iLevelSum	=	0;
	int	iMaxLevel	=	0;
	int	iMinLevel	=	1000;

	for	(int i=m_bf3MemberCount;i;)
	{
		--i;

		int	iLevel	=	m_aMemberList[i].m_bf10Level;

		iLevelSum	+=	iLevel;

		if	(iLevel	>	iMaxLevel)
			iMaxLevel	=	iLevel;

		if	(iLevel	<	iMinLevel)
			iMinLevel	=	iLevel;
	}

	if	(m_bf3MemberCount	>=	5)
	{
		iLevelSum	-=	(iMaxLevel+iMinLevel);

		return	iLevelSum/(m_bf3MemberCount-2);
	}

	return	iLevelSum/m_bf3MemberCount;
}
