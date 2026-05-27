#include "CQuest.h"
#include "CItem.h"
#include "CGamePlay.h"

CQuest	g_quest;
CQuest	g_questArbeit;

CQuest::~CQuest()
{
	pKILL(m_pQuest);
}

//
//	초기화
void
CQuest::init(int _iCount)
{
	m_iCount	=	_iCount;
	pKILL(m_pQuest);

	m_pQuest	=	new CQuestItem [m_iCount];
}

CQuestItem*
CQuest::getQuestByName(char *_lpstrName)
{
	for (int i=0;i<m_iCount;i++)
	{
		if (STRICMP(m_pQuest[i].m_strName,_lpstrName) == 0)
			return	&m_pQuest[i];
	}

	return	NULL;
}

void
CQuest::setQuestInfo(int _iQeustIndex,CQuestItemSaveData *_lpQuestSaveData)
{
	CQuestItem	*lpQuest=	&m_pQuest[_iQeustIndex];

	lpQuest->m_wSerial	=	_iQeustIndex;

	strcpy(lpQuest->m_strName	,_lpQuestSaveData->m_strName);

	memcpy(&lpQuest->m_wQuestType,&_lpQuestSaveData->m_wQuestType,sizeof(CQuestItemSaveData)-c_iHeavyQuestDataSize-sizeof(_lpQuestSaveData->m_aSpareBuffer));

	if	(IsNumberText(lpQuest->m_strBeginNPCName))
		lpQuest->m_wBeginNpcSerial	=	atoi(lpQuest->m_strBeginNPCName);
	else
		lpQuest->m_wBeginNpcSerial	=	0xffff;
}

CQuestItem*
CQuest::getProprietyQuest(int _iLevel,int _iField,int _iJob,int *_lpiLastProprietyQuestIndex,char *_lpstrNpcName,int _iNpcActorSerial)
{
	for (int i=*_lpiLastProprietyQuestIndex;i<m_iCount;i++)
	{
		CQuestItem	*lpQuest	=	&m_pQuest[i];

		if	(lpQuest->m_wBeginNPC_Field	!=	0xffff)
			if	(lpQuest->m_wBeginNPC_Field	!=	_iField)
				continue;

		if	(lpQuest->m_wBeginNpcSerial	!=	0xffff)
		{
			if	(lpQuest->m_wBeginNpcSerial	!=	_iNpcActorSerial)
				continue;
		}
		else
		if	(!_lpstrNpcName	||	STRICMP(_lpstrNpcName,lpQuest->m_strBeginNPCName)!=0)
			continue;

		if	(lpQuest->m_wBeginYear)
		{
			int	iCurValue	=	CGamePlay::s_serverTime.m_wYear*31*12+CGamePlay::s_serverTime.m_wMonth*31+CGamePlay::s_serverTime.m_wDay;
			int	iBeginValue	=	lpQuest->m_wBeginYear*31*12+lpQuest->m_wBeginMonth*31+lpQuest->m_wBeginDay;
			int	iExpValue	=	lpQuest->m_wExpireYear*31*12+lpQuest->m_wExpireMonth*31+lpQuest->m_wExpireDay;

			if	(iCurValue<	iBeginValue)
				continue;
			if	(iCurValue>	iExpValue)
				continue;
		}

		if	(lpQuest->m_wMinProprietyLevel	!=	0)	//	해당 퀘스트를 해결하기에 적정 레벨이 아니다.
			if	(_iLevel	<	lpQuest->m_wMinProprietyLevel	||	_iLevel	>	lpQuest->m_wMaxProprietyLevel)
				continue;

		if	(!lpQuest->isEnableJob(_iJob))
			continue;

		*_lpiLastProprietyQuestIndex	=	i+1;

		return	lpQuest;
	}

	return	NULL;
}

CQuestItem*
CQuest::GetQuest(int _iQuestType,int _iQuestIndex)
{
	if	(_iQuestType	==	eQT_NORMAL)
		return	g_quest.getQuest(_iQuestIndex);

	return	g_questArbeit.getQuest(_iQuestIndex);
}
