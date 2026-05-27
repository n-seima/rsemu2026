#include	"life_code_common.h"
#include	"assert.h"

BOOL
cLifeCode_Info::isLife(DWORD _dwCurTime)
{
	if	(m_bIsCheckNow		==	FALSE)
		return	TRUE;

	if	(m_dwLastWorkTime	>	_dwCurTime)	//	시계가 한바퀴 돌았다.
		return	TRUE;

	int	iSpentTime	=	_dwCurTime-m_dwLastWorkTime;

	if	(iSpentTime	>=	m_iDeathTime)
		return	FALSE;

	return	TRUE;
}

cLifeCode_Mgr::cLifeCode_Mgr()
{
	m_pLifeCode			=	NULL;
	strcpy(m_strBadStatusLog,"");
}

cLifeCode_Mgr::~cLifeCode_Mgr()
{
	pKILL(m_pLifeCode);
}

void
cLifeCode_Mgr::init(int _iCount)
{
	pKILL(m_pLifeCode);

	m_pLifeCode	=	new cLifeCode_Info [_iCount];

	m_iSize		=	_iCount;
}

void
cLifeCode_Mgr::set(int _iSlot,char *_lpstrWarningMsg,int _iDeathTime)
{
	assert(_iSlot < m_iSize && _iSlot >= 0 );

	strcpy(m_pLifeCode[_iSlot].m_strWarningLog,_lpstrWarningMsg);
	m_pLifeCode[_iSlot].m_iDeathTime	=	_iDeathTime;
}

BOOL
cLifeCode_Mgr::checkAll(DWORD _dwCurTime)
{
	for (int i=m_iSize;i;)
	{
		--i;

		if	(!m_pLifeCode[i].isLife(_dwCurTime))
		{
			strcpy(m_strBadStatusLog,m_pLifeCode[i].m_strWarningLog);

			return	FALSE;
		}
	}

	return	TRUE;
}
