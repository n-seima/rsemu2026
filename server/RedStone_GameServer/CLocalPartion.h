#ifndef _classLocalPartion_h
#define _classLocalPartion_h

#include "SFC.H"

#define	dMAX_ACTOR_COUNT_IN_LOCAL_PART	200
#define	dTEAM_COUNT						3
#define	dPARTITION_COUNT				10
#define	dPARTITION_COUNT_IN_FIELD		(dPARTITION_COUNT*dPARTITION_COUNT)
#define	dLINK_PARTITION_COUNT			9

class	CLocalSmallPartionLinkInfo
{
public:
	WORD	m_wLinkPartCount;
	WORD	*m_pwLinkPart;

	CLocalSmallPartionLinkInfo()
	{
		m_pwLinkPart	=	NULL;
	}

	~CLocalSmallPartionLinkInfo()
	{
		pKILL(m_pwLinkPart);
	}

	void	init(int _iPartCount)
	{
		m_wLinkPartCount=	0;
		m_pwLinkPart	=	new WORD [_iPartCount];
	}
	void	addPart(int _iPart)
	{
		m_pwLinkPart[m_wLinkPartCount++]	=	_iPart;
	}
};

class	CLocalPartionInfo
{
public:
	cRECT			m_rect;
	int				m_aiActorCount[dTEAM_COUNT],m_aiBufferSize[dTEAM_COUNT];
	WORD			*m_apActorList[dTEAM_COUNT];

	CLocalPartionInfo()
	{
		reset();

		for (int i=0;i<dTEAM_COUNT;i++)
			m_apActorList[i]	=	NULL;

	}

	~CLocalPartionInfo()
	{
		reset();

		for (int i=0;i<dTEAM_COUNT;i++)
			pKILL(m_apActorList[i]);
	}

	void			init(int _iTeam1,int _iTeam2,int _iTeam3)
	{
		m_aiBufferSize[0]	=	_iTeam1;
		m_aiBufferSize[1]	=	_iTeam2;
		m_aiBufferSize[2]	=	_iTeam3;

		m_apActorList[0]	=	new WORD [_iTeam1];
		m_apActorList[1]	=	new WORD [_iTeam2];
		m_apActorList[2]	=	new WORD [_iTeam3];
	}

	void			reset()
	{
		memset(m_aiActorCount,0,sizeof(m_aiActorCount));
	}

	inline	BOOL	isFull(int _iTeam)
	{
		if (m_aiActorCount[_iTeam]	==	dMAX_ACTOR_COUNT_IN_LOCAL_PART)
			return	TRUE;

		return	FALSE;
	}

	inline	BOOL	addMember(int _iSerial,int _iTeam)
	{
		if (m_aiActorCount[_iTeam]	>=	m_aiBufferSize[_iTeam])
			return	FALSE;

		m_apActorList[_iTeam][m_aiActorCount[_iTeam]]	=	_iSerial;
		m_aiActorCount[_iTeam]++;

		return	TRUE;
	}

	inline	void	removeMember(int _iSerial,int _iTeam)
	{
		int	iListIndex	=	0xffff;

		for (int i=m_aiActorCount[_iTeam];i;i--)	
			if (m_apActorList[_iTeam][i-1]	==	_iSerial)
			{
				m_apActorList[_iTeam][i-1]	=	m_apActorList[_iTeam][m_aiActorCount[_iTeam]-1];
				m_aiActorCount[_iTeam]--;

				iListIndex	=	i-1;
				break;
			}
	}
};

#endif