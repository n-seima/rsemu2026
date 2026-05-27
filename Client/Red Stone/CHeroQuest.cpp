#include "CHero.h"
#include "CWindowInterface.H"

//
//
tsProcessQuestField*
CHero::getQuestInfo(int _iSlotIndex)
{
	if (_iSlotIndex	< 0 || _iSlotIndex >= dMAX_PROCESS_QUEST_COUNT)
		return	NULL;

	return	&m_aProcessQuest[_iSlotIndex];
}

//
//
CQuestItem*
CHero::getQuest(int _iSlotIndex)
{
	tsProcessQuestField	*lpQuestInfo	=	getQuestInfo(_iSlotIndex);

	if (!lpQuestInfo || lpQuestInfo->isActive == FALSE)
		return	NULL;

	if (lpQuestInfo->questKind	==	eQT_NORMAL)
		return	g_quest.getQuest(lpQuestInfo->questIndex);

	if (lpQuestInfo->questKind	==	eQT_ARBEIT)
		return	g_questArbeit.getQuest(lpQuestInfo->questIndex);

	return	NULL;
}

//
//	퀘스트 취소
BOOL
CHero::cancelQuest(int _iSlotIndex)
{
	tsProcessQuestField	*lpQuestInfo	=	getQuestInfo(_iSlotIndex);

	if (!lpQuestInfo || lpQuestInfo->isActive == FALSE)	
		return	NULL;

	CQuestItem	*lpQuest	=	getQuest(_iSlotIndex);

	if (!lpQuest)	
		return	FALSE;

	if (lpQuest->m_isRemoveQuestItemByCancelQuest)
		removeQuestItem(lpQuestInfo->questKind,lpQuestInfo->questIndex);

	lpQuestInfo->isActive	=	FALSE;

	m_awQuestCountDownTimer[_iSlotIndex] = 0xffff;

	return	FALSE;
}

void
CHero::setChapter(int _iChapter,int _iHighQuest)
{
	m_wProcessChapterIndex	=	_iChapter;
	m_wProcessHighQuestIndex=	_iHighQuest;

	if	(m_wProcessHighQuestIndex	==	0)
		g_gwMainQuestEvent.open(m_wProcessChapterIndex);
}

//
//	진행중인 퀘스트의 수
int
CHero::getProcessQeustCount()
{
	int	iCount	=	0;

	for (int i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
		if (m_aProcessQuest[i].isActive)	iCount++;

	return	iCount;
}

BOOL
CHero::SaveCompleteQuest(int _iQuestType,int _iQuestIndex)
{
	if (_iQuestType	==	eQT_NORMAL)
	{
		int	iSlot	=	_iQuestIndex/8;

		if	(iSlot	<	dQUEST_BUFFER_SIZE)
		{
			m_aCompleteQuest[iSlot] = m_aCompleteQuest[iSlot]|(1<<(_iQuestIndex%8));
		}
		else
		{
			iSlot	-=	dQUEST_BUFFER_SIZE;

			m_aCompleteQuest2[iSlot] = m_aCompleteQuest2[iSlot]|(1<<(_iQuestIndex%8));
		}

		return	FALSE;
	}

	int	iSlot	=	_iQuestIndex/8;

	if	(iSlot	>=	dARBEIT_BUFFER_SIZE)
	{
		iSlot	-=	dARBEIT_BUFFER_SIZE;

		m_aCompleteArbeit2[iSlot] = m_aCompleteArbeit2[iSlot]|(1<<(_iQuestIndex%8));
	}
	else
	{
		m_aCompleteArbeit[iSlot] = m_aCompleteArbeit[iSlot]|(1<<(_iQuestIndex%8));
	}

	return	FALSE;
}

BOOL
CHero::isProcessQuest(int _iQuestType, int _iQuestIndex)
{
	for(int i=0 ; i <dMAX_PROCESS_QUEST_COUNT ; i++)
	{
		if (g_hero.m_aProcessQuest[i].questKind == _iQuestType && g_hero.m_aProcessQuest[i].questIndex == _iQuestIndex && g_hero.m_aProcessQuest[i].isActive)
		{
			return TRUE;
		}
	}
	return FALSE;
}

//
//	이거 완료한 퀘스트냐?
BOOL
CHero::isCompleteQuest(int _iQuestType,int _iQuestIndex)
{
	if (_iQuestType	==	eQT_NORMAL)
	{
		int	iSlot	=	_iQuestIndex/8;

		if	(iSlot	<	dQUEST_BUFFER_SIZE)
		{
			if	(m_aCompleteQuest[iSlot]&(1<<(_iQuestIndex%8)))
				return	TRUE;
		}
		else
		{
			iSlot	-=	dQUEST_BUFFER_SIZE;

			if	(m_aCompleteQuest2[iSlot]&(1<<(_iQuestIndex%8)))
				return	TRUE;
		}

		return	FALSE;
	}

	int	iSlot	=	_iQuestIndex/8;

	if	(iSlot	>=	dARBEIT_BUFFER_SIZE)
	{
		iSlot	-=	dARBEIT_BUFFER_SIZE;

		if	(m_aCompleteArbeit2[iSlot]&(1<<(_iQuestIndex%8)))
			return	TRUE;
	}
	else
	{
		if	(m_aCompleteArbeit[iSlot]&(1<<(_iQuestIndex%8)))
			return	TRUE;
	}

	return	FALSE;
}

CQuestItem*
CHero::getProprietyQuest(int *_lpiLastProprietyQuestIndex,int _iQuestType,char *_lpstrNpcName,int _iNpcActorSerial)
{
	CQuest		*lpQuest;
	CQuestItem	*lpQuestItem;

	if (_iQuestType	==	eQT_ARBEIT)
		lpQuest		=	&g_questArbeit;
	else
	if (_iQuestType	==	eQT_NORMAL)
		lpQuest		=	&g_quest;

	while(1)
	{
		lpQuestItem	=	lpQuest->getProprietyQuest(m_iLevel,m_wCurrentField,m_wJob,_lpiLastProprietyQuestIndex,_lpstrNpcName,_iNpcActorSerial);

		if	(!lpQuestItem)
			return	NULL;

		if	(isCompleteQuest(_iQuestType,lpQuestItem->m_wSerial))
			continue;

		return	lpQuestItem;
	}

	return	NULL;	
}

CQuestItem*
CHero::getProprietyQuest(char *_lpstrNpcName,int _iNpcActorSerial)
{
	int			iQuestType;
	CQuest		*lpQuest;
	CQuestItem	*lpQuestItem;

	for (iQuestType=eQT_NORMAL;iQuestType<=eQT_ARBEIT;iQuestType++)
	{
		if (iQuestType	==	eQT_ARBEIT)
			lpQuest		=	&g_questArbeit;
		else
		if (iQuestType	==	eQT_NORMAL)
			lpQuest		=	&g_quest;

		int		iProprietyQuestIndex	=	0;

		while(1)
		{
			lpQuestItem	=	lpQuest->getProprietyQuest(m_iLevel,m_wCurrentField,m_wJob,&iProprietyQuestIndex,_lpstrNpcName,_iNpcActorSerial);

			if	(!lpQuestItem)
				break;

			if	(isCompleteQuest(iQuestType,lpQuestItem->m_wSerial))
				continue;

			return	lpQuestItem;
		}
	}

	return	NULL;	
}

BOOL
CHero::isProcessQeustClient(char *_lpstrNpcName,int _iNpcActorSerial)
{
	int	i;

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		CQuestItem*	lpQuest	=	getQuest(i);

		if	(!lpQuest)
			continue;

		if	(lpQuest->m_wBeginNpcSerial	==	_iNpcActorSerial)
			return	TRUE;
		if	(STRICMP(lpQuest->m_strBeginNPCName,_lpstrNpcName)	==	0)
			return	TRUE;

		char	*lpstrClient	=	lpQuest->getClientName(m_aProcessQuest[i].questProcessLevel);
		int		iClientField	=	lpQuest->getClientField(m_aProcessQuest[i].questProcessLevel);
		int		iClientNpcSerial=	lpQuest->getClientNpcSerial(m_aProcessQuest[i].questProcessLevel);

		if	(iClientField	!=	m_wCurrentField)
			continue;
		
		if	(iClientNpcSerial	!=	0xffff)
		{
			if	(iClientNpcSerial ==	_iNpcActorSerial)
				return	TRUE;
			
			return	FALSE;
		}

		if	(lpstrClient && STRICMP(lpstrClient,_lpstrNpcName) == 0)
			return	TRUE;
	}

	return	FALSE;
}

BOOL
CHero::isNpcAboutProcessQuest(char *_lpstrNpcName,int _iNpcActorSerial)
{
	int	i;

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		CQuestItem*	lpQuest	=	getQuest(i);

		if	(!lpQuest)
			continue;

		if	(lpQuest->isNpcAboutProcessQuest(_lpstrNpcName,m_aProcessQuest[i].questProcessLevel,m_wCurrentField,_iNpcActorSerial))
			return	TRUE;
	}

	return	FALSE;
}

BOOL
CHero::isMonsterAboutProcessQuest(char *_lpstrNpcName,int _iNpcActorSerial)
{
	int	i;

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		CQuestItem*	lpQuest	=	getQuest(i);

		if (!lpQuest)
			continue;

		if (lpQuest->isMonsterAboutProcessQuest(_lpstrNpcName,m_aProcessQuest[i].questProcessLevel,m_wCurrentField,_iNpcActorSerial))
			return	TRUE;
	}

	return	FALSE;
}