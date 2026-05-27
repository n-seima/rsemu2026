#include "cFIELD.H"
#include "cACTOR.H"
#include "cGame.H"

//
//	범위 안에 파티원 치료
void
cFIELD::cureRangedPartyMember(cACTOR *_lpCaster,cAbility *_lpAbility,int _iHealPoint,int _iX,int _iY,int _iCureRange,int _iParty)
{
	int		i,iTeam		=	_lpCaster->m_wTeam;
	CSkill	*lpSkill	=	_lpAbility->getSkill();

	for (i=m_aiActorCount[iTeam];i;i--)
	{
		cACTOR	*lpActor	=	getActor(m_awActorList[iTeam][i-1]);

		if	(!lpActor)
			continue;

		int	iRange	=	GetOvalRange(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y);

		if	(iRange	>	_iCureRange	)
			continue;

		if	(isMissileBlockedLine(_iX,_iY,lpActor->m_pos.x,lpActor->m_pos.y))
			continue;		

		if	(lpActor->isPet() || lpActor->isSummonBeast())
		{
			cACTOR	*lpTamer	=	lpActor->getTamer();
			
			if	(!lpTamer)
				continue;

			if	(lpTamer->getPartySerial()	!=	_iParty		)
				continue;
		}
		else
			if	(lpActor->getPartySerial()	!=	_iParty		)
				continue;

		if	(lpActor->cure(_lpCaster,_lpAbility,_iHealPoint)	)
		{
			int	iHealPoint	=	_iHealPoint;

			if	(g_bIsDuelServer)
				iHealPoint	=	_iHealPoint	*g_iCorrectHealEfficientUnderDuelBattle/100;

			sendCureActorMessage(lpActor->m_wSerialInField,lpSkill->m_wSerial,iHealPoint);
		}
	}
}