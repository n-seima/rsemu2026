#include "cKARMA_VALUE.H"

cVALUE		g_ProjectValue,g_LocalValue;

void
cVALUE::Reset()
{
	m_iCount	=	0;
	memset(m_aValue,0,sizeof(m_aValue));
}
