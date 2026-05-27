#include "cMAP.H"
#include "cDRAW.H"
#include "cMAPWND.H"
#include "cTEXT.H"
#include "cBUILDING.H"
#include "cMESSAGE.H"
#include "cPROJECT.H"
#include "cTOOLWND.H"
#include "CShop.H"
#include "doorDefine.H"
#include "secretDungeon.h"

void
cMAP::exportExpReward(cFILE *_lpFile)
{
	int		i;

	for	(i=0;i<m_karma.m_wKarmaCount;i++)
	{
		CKarma	*lpKarma	=	&m_karma.m_pKarma[i];

		for	(int	iReaction=0;iReaction<lpKarma->m_wReactionCount;iReaction++)
		{
			CKarmaItem	*lpReaction	=	&lpKarma->m_pReaction[iReaction];

			if	(lpReaction->m_wItem	==	eREACTION_GET_EXPERIENCE)
				_lpFile->writeStreamCR("%s field_karma \"%s\" %d",m_strFName,lpKarma->m_pstrTitle,lpReaction->m_aValue[2]);
		}
	}
	
	for	(i=0;i<dMAX_ACTOR_COUNT;i++)
	{
		cACTOR	*lpActor	=	m_setActor.getActor(i);

		if	(!lpActor	||	lpActor->m_pCC	==	NULL)
			continue;

		for	(int	iCC=0;iCC<lpActor->m_pCC->m_wConversationCount;iCC++)
		{
			CConversation	*lpConversation	=	&lpActor->m_pCC->m_pConversation[iCC];

			for	(int	iContents=0;iContents<lpConversation->m_wContentsCount;iContents++)
			{
				CKarmaContainer	*lpkc	=	&lpConversation->m_pContents[iContents];

				for	(int	iKarma=0;iKarma<lpkc->m_wKarmaCount;iKarma++)
				{
					CKarma	*lpKarma	=	&lpkc->m_pKarma[iKarma];

					for	(int	iReaction=0;iReaction<lpKarma->m_wReactionCount;iReaction++)
					{
						CKarmaItem	*lpReaction	=	&lpKarma->m_pReaction[iReaction];

						if	(lpReaction->m_wItem	==	eREACTION_GET_EXPERIENCE)
							_lpFile->writeStreamCR("%s actor \"%s\" [iCC %d,iContents %d,iKarma %d,iReaction %d] %d",m_strFName,lpActor->m_strName,iCC,iContents,iKarma,iReaction,lpReaction->m_aValue[2]);
					}
				}
			}
		}
	}

	for	(i=0;i<dMAX_CHARACTER;i++)
	{
		cCHARACTER	*lpCharacter	=	m_setCharacter.getCharacter(i);

		if	(!lpCharacter)
			continue;

		for (int iEvent=0;iEvent<eMECC_COUNT;iEvent++)
		{
			if	(!lpCharacter->m_pEvent[iEvent])
				continue;

			CKarmaContainer	*lpkc	=	lpCharacter->m_pEvent[iEvent];

			for	(int	iKarma=0;iKarma<lpkc->m_wKarmaCount;iKarma++)
			{
				CKarma	*lpKarma	=	&lpkc->m_pKarma[iKarma];

				for	(int	iReaction=0;iReaction<lpKarma->m_wReactionCount;iReaction++)
				{
					CKarmaItem	*lpReaction	=	&lpKarma->m_pReaction[iReaction];

					if	(lpReaction->m_wItem	==	eREACTION_GET_EXPERIENCE)
						_lpFile->writeStreamCR("%s character \"%s\" [iEvent %d,iKarma %d,iReaction  %d] %d",m_strFName,lpCharacter->m_strName,iEvent,iKarma,iReaction,lpReaction->m_aValue[2]);
				}
			}
		}
	}
}

WORD
cMAP::GetReactionIndex(CKarmaItem	*_lpReaction)
{
	for (int i = 0 ; i < 200 ; i++ )
	{
		if(305 == g_aReaction[i].m_wSerial)
			return i;
	}

	return NULL;
}

void
cMAP::exportKarma(cFILE *_lpFile)
{
	int		i;
	
	for	(i=0;i<m_karma.m_wKarmaCount;i++)
	{
		CKarma	*lpKarma	=	&m_karma.m_pKarma[i];
		
		for	(int	iReaction=0;iReaction<lpKarma->m_wReactionCount;iReaction++)
		{
			CKarmaItem	*lpReaction	=	&lpKarma->m_pReaction[iReaction];
			
			_lpFile->writeStreamCR("%s field_karma \"%s\" %s",m_strFName,lpKarma->m_pstrTitle,
				g_aReaction[GetReactionIndex(lpReaction)].m_strName);
		}
	}
	
	for	(i=0;i<dMAX_ACTOR_COUNT;i++)
	{
		cACTOR	*lpActor	=	m_setActor.getActor(i);
		
		if	(!lpActor	||	lpActor->m_pCC	==	NULL)
			continue;
		
		for	(int	iCC=0;iCC<lpActor->m_pCC->m_wConversationCount;iCC++)
		{
			CConversation	*lpConversation	=	&lpActor->m_pCC->m_pConversation[iCC];
			
			for	(int	iContents=0;iContents<lpConversation->m_wContentsCount;iContents++)
			{
				CKarmaContainer	*lpkc	=	&lpConversation->m_pContents[iContents];
				
				for	(int	iKarma=0;iKarma<lpkc->m_wKarmaCount;iKarma++)
				{
					CKarma	*lpKarma	=	&lpkc->m_pKarma[iKarma];
					
					for	(int	iReaction=0;iReaction<lpKarma->m_wReactionCount;iReaction++)
					{
						CKarmaItem	*lpReaction	=	&lpKarma->m_pReaction[iReaction];
							
						_lpFile->writeStreamCR("%s actor \"%s\" %s",m_strFName,lpActor->m_strName ,
							g_aReaction[GetReactionIndex(lpReaction)].m_strName);
					}
				}
			}
		}
	}
	
	for	(i=0;i<dMAX_CHARACTER;i++)
	{
		cCHARACTER	*lpCharacter	=	m_setCharacter.getCharacter(i);
		
		if	(!lpCharacter)
			continue;
		
		for (int iEvent=0;iEvent<eMECC_COUNT;iEvent++)
		{
			if	(!lpCharacter->m_pEvent[iEvent])
				continue;
			
			CKarmaContainer	*lpkc	=	lpCharacter->m_pEvent[iEvent];
			
			for	(int	iKarma=0;iKarma<lpkc->m_wKarmaCount;iKarma++)
			{
				CKarma	*lpKarma	=	&lpkc->m_pKarma[iKarma];
				
				for	(int	iReaction=0;iReaction<lpKarma->m_wReactionCount;iReaction++)
				{
					CKarmaItem	*lpReaction	=	&lpKarma->m_pReaction[iReaction];
										
					_lpFile->writeStreamCR("%s character \"%s\" %s",m_strFName,lpCharacter->m_strName,
						g_aReaction[GetReactionIndex(lpReaction)].m_strName);
				}
			}
		}
	}
}