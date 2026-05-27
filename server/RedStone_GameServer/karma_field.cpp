#include	"cFIELD.H"
#include	"cGAME.H"
#include	"instance_guild_battle_manager.H"
#include	<time.H>
#include	<math.H>
#include	"CPitchmanShop.h"
#include	"debugCode.h"
#include	"secretDungeon.h"

void
cFIELD::operateKarma()
{
	int		i,iCount;
	cACTOR	*lpAsker	=	NULL;

	if	(m_iUniqueSerial	>=	dINSTANCE_FIELD_FLAG)
		for (i=0;i<m_aiActorCount[0];i++)
		{
			cACTOR	*lpActor	=	getActor(m_awActorList[0][i]);

			if (lpActor)
			{
				lpAsker	=	lpActor;

				break;
			}
		}

	BOOL	bOutputOld	=	g_bIsWantPrintTriggerCondition;
		
	g_bIsWantPrintTriggerCondition	=	m_bIsWantPrintTimeKarmaInfo;

	if	(m_kc.m_wKarmaCount)
	{
		if	(m_bIsWantPrintTimeKarmaInfo)
			printf("operate field %d karma\n==================================\n",m_iUniqueSerial);

		m_kc.operate(lpAsker,NULL,this);
	}

	for (i=0,iCount=0;i<dMAX_NPC_COUNT && iCount<m_iSavedActorCount;i++)
	{
		cNPC	*lpMonster	=	m_apNpcGenerateData[i];

		if (lpMonster	==	NULL)
			continue;

		iCount++;

		if (lpMonster->m_wActorKind		!=	eAK_MONSTER)
			continue;

		lpMonster->operateAlwaysEvent(this);
	}

	g_bIsWantPrintTriggerCondition	=	bOutputOld;

	if	(m_bIsWantPrintTimeKarmaInfo)
		printf("==================================\n");
}