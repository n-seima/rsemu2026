#include "cField.h"
#include "cGame.h"

void
cFIELD::resetUserPlayStatics()
{
	memset(m_adwUniquePlayerNameHashCodeList,0xff,sizeof(m_adwUniquePlayerNameHashCodeList));
	m_llExpSum	=	0;

	memcpy(&m_beginTime,&g_currentTime,sizeof(g_currentTime));
}

int
cFIELD::getUniqueUserCount()
{
	int	iCount	=	0;

	for	(int i=10000;i;)
	{
		--i;

		if	(m_adwUniquePlayerNameHashCodeList[i]	==	0xffffffff)
			break;

		iCount++;
	}

	return	iCount;
}