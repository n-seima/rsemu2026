#include "pattern.h"

cCharacterPattern::cCharacterPattern()
{
	init();
}

cCharacterPattern::~cCharacterPattern()
{
	close();
}

void
cCharacterPattern::init()
{
	m_pPattern	=	NULL;
}

void
cCharacterPattern::close()
{
	pKILL(m_pPattern);
}


cCharacterPatternPack::cCharacterPatternPack()
{
	init();
}

cCharacterPatternPack::~cCharacterPatternPack()
{
	close();
}

void
cCharacterPatternPack::init()
{
	m_wCount	=	0;

	for	(int i=c_iCharacterPatternCountInField;i;)
	{
		--i;

		m_apPatterns[i]	=	NULL;
	}
}

void
cCharacterPatternPack::close()
{
	m_wCount	=	0;

	for	(int i=c_iCharacterPatternCountInField;i;)
	{
		--i;

		KILL(m_apPatterns[i]);
	}
}
