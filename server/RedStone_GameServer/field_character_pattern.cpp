#include "cFIELD.h"

cCharacterPattern*
cFIELD::getCharacterPattern(int _iPattern)
{
	if	(!m_pCPP)
		return	NULL;

	return	m_pCPP->m_apPatterns[_iPattern];
}
