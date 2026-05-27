#include	"cFIELD.H"
#include	"cGAME.H"
#include	"Client.H"
#include	"debugCode.H"
#include	"CGuild.H"

BOOL
cFIELD::checkLocalPartPlayerForMonster(int _iPart)
{
	if	(m_bIsGuildBattleField)
		return	m_pLocalPartition[_iPart].m_aiActorCount[0]+m_pLocalPartition[_iPart].m_aiActorCount[1];

	return	m_pLocalPartition[_iPart].m_aiActorCount[0];
}

void
cFIELD::changeSmallLocalPart(int _iSerial,int _iTeam,int _iOldPart,int _iNewPart)
{
	removeActorAtLocalPart(_iSerial,_iTeam,_iOldPart);
	addActorAtLocalPart(_iSerial,_iTeam,_iNewPart);
}

void
cFIELD::addActorAtLocalPart(int _iIndex,int _iTeam,int _iSmallPart)
{
	int i;

	for (i=m_pPartLinkInfo[_iSmallPart].m_wLinkPartCount;i;i--)
	{
		int	iLocalPart	=	m_pPartLinkInfo[_iSmallPart].m_pwLinkPart[i-1];

		m_pLocalPartition[iLocalPart].addMember(_iIndex,_iTeam);
	}
}

void
cFIELD::removeActorAtLocalPart(int _iIndex,int _iTeam,int _iSmallPart)
{
	int	i;

	for (i=m_pPartLinkInfo[_iSmallPart].m_wLinkPartCount;i;i--)
	{
		int	iLocalPart	=	m_pPartLinkInfo[_iSmallPart].m_pwLinkPart[i-1];

		m_pLocalPartition[iLocalPart].removeMember(_iIndex,_iTeam);
	}
}
