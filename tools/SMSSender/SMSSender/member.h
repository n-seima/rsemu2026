#pragma

#include	"tchar.h"

#define	dMAX_MEMBER_COUNT	128

class	cSMS_Member
{
public:
	TCHAR	m_strName[32];
	TCHAR	m_strNumber[32];
	WORD	m_wType;
};

extern	int			g_iMemberCount;

extern	cSMS_Member	g_aMember[dMAX_MEMBER_COUNT];
