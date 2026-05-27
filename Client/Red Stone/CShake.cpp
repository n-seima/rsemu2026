#include "CShake.H"
#include "CActor.H"
#include "cMAP.H"
#include "CHero.H"

CShake	g_shaker;

//
//	¿
BOOL
CShakeInfo::update(DWORD _dwCurrentTime)
{
	if (m_dwBeginTime+m_dwUpkeepTime < _dwCurrentTime || m_dwBeginTime > _dwCurrentTime)
		return	TRUE;

	return	FALSE;
}	//	CShakeInfo::update()

CShake::CShake()
{
	m_iCount	=	0;
	m_iNewSerial=	0;
}	//	CShake::CShake()

//
//	¾·
void
CShake::close()
{
	m_iCount	=	0;
	m_iNewSerial=	0;
}	//	CShake::close()

//
//	¤ ¸¹
BOOL
CShake::add(int x,int y,int iIntensity,DWORD dwUpkeepTime,int iRange)
{
	if(g_config.m_bf1IsShakeOff)
		return FALSE;

	if (m_iCount	>=	dSHAKE_COUNT	)	return FALSE;

	m_aShake[m_iNewSerial].m_pos.x			=	x;
	m_aShake[m_iNewSerial].m_pos.y			=	y;
	m_aShake[m_iNewSerial].m_wSerial		=	m_iNewSerial;
	m_aShake[m_iNewSerial].m_iSI			=	iIntensity;
	m_aShake[m_iNewSerial].m_iRange			=	iRange;
	m_aShake[m_iNewSerial].m_dwUpkeepTime	=	dwUpkeepTime;
	m_aShake[m_iNewSerial].m_dwBeginTime	=	timeGetTime();

	for (int i=m_iNewSerial+1;i<dSHAKE_COUNT;i++)
	{
		if	(m_aShake[i].m_wSerial	==	0xffff)
		{
			m_iNewSerial	=	i;
			m_iCount++;

			return TRUE;
		}
	}

	return FALSE;
}

//
//	°
void
CShake::remove(int index)
{
	if	(m_aShake[index].m_wSerial	==	0xffff)
		return;

	m_aShake[index].m_wSerial	=	0xffff;
	m_iCount--;

	if	(index < m_iNewSerial)
		m_iNewSerial	=	index;
}

//
//
int
CShake::getMySeismicIntensity()
{
	int	x	=	g_am.m_posFocus.x;
	int	y	=	g_am.m_posFocus.y;

	int		i,iMaxSI	=	0,iIndex=-1;

	for (i=0;i<dSHAKE_COUNT; i++)
	{
		if	(m_aShake[i].m_wSerial	==	0xffff)
			continue;

		int	iCurrentRange	=	GetOvalRange(x,y,m_aShake[i].m_pos.x,m_aShake[i].m_pos.y);
		int	iRange			=	m_aShake[i].m_iRange*m_aShake[i].m_iRange;

		if (iCurrentRange	<	iRange || iRange == 0)
			if (iMaxSI	<	m_aShake[i].m_iSI)
			{
				iMaxSI	=	m_aShake[i].m_iSI;
				iIndex	=	i;
			}
	}

	return iIndex;
}

//
//	¾÷Æ®°
void
CShake::update()
{	
	DWORD	dwCurrentTime	=	timeGetTime();

	for (int i=0;i<dSHAKE_COUNT; i++)
	{
		if	(m_aShake[i].m_wSerial	==	0xffff)
			continue;

		if	(m_aShake[i].update(dwCurrentTime))
			remove(i);
	}

	int	iSI	=	getMySeismicIntensity();

	m_dwUpdateCount++;

	if	(iSI	>	-1)
	{
		CShakeInfo	*lpShake	=	&m_aShake[iSI];
		CPos		pos;

		int	iAngle	=	90;

		if	(m_dwUpdateCount%2	==	0)
			iAngle	=	(iAngle+180)%360;

		pos.Set(0,0);

		GetAnglePos(&pos,iAngle,lpShake->m_iSI);

		g_canvas.shake(pos.x,pos.y);
	}
}