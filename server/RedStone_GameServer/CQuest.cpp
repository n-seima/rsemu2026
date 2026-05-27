#include "CQuest.h"
#include "CProject.h"
#include "cACTOR.h"

CQuestManager	g_quest,g_questArbeit;

//
//	퀘스트 정보 설정
BOOL
CQuestManager::setQuestInfo(int _iIndex,CQuestItem *_lpQuest)
{
	CQuestSimpleInfo	*lpQuestSimpleInfo	=	getQuest(_iIndex);

	if (!lpQuestSimpleInfo)
		return	FALSE;

	lpQuestSimpleInfo->m_wQuestType							=	_lpQuest->m_wQuestType						;	//	이벤트 퀘스트여?
	lpQuestSimpleInfo->m_wQuestKind							=	_lpQuest->m_wQuestKind						;
	lpQuestSimpleInfo->m_wBeginYear							=	_lpQuest->m_wBeginYear						;
	lpQuestSimpleInfo->m_wBeginMonth						=	_lpQuest->m_wBeginMonth						;
	lpQuestSimpleInfo->m_wBeginDay							=	_lpQuest->m_wBeginDay						;	
	lpQuestSimpleInfo->m_wExpireYear						=	_lpQuest->m_wExpireYear						;	
	lpQuestSimpleInfo->m_wExpireMonth						=	_lpQuest->m_wExpireMonth					;	
	lpQuestSimpleInfo->m_wExpireDay							=	_lpQuest->m_wExpireDay						;

	lpQuestSimpleInfo->m_isEnableCancel						=	_lpQuest->m_isEnableCancel					;
	lpQuestSimpleInfo->m_isRemoveQuestItemByCancelQuest		=	_lpQuest->m_isRemoveQuestItemByCancelQuest	;
	lpQuestSimpleInfo->m_isTestQuestInTestServer			=	_lpQuest->m_isTestQuestInTestServer			;
	lpQuestSimpleInfo->m_isCloseQuest						=	FALSE;
	
	lpQuestSimpleInfo->m_uiEnableJobMask					=	_lpQuest->m_uiEnableJobMask;
	lpQuestSimpleInfo->m_bf1IsMaleJobEquipAble				=	_lpQuest->m_bf1IsMaleJobEquipAble;
	lpQuestSimpleInfo->m_bf1IsFemaleJobEquipAble			=	_lpQuest->m_bf1IsFemaleJobEquipAble;

	lpQuestSimpleInfo->m_wMinLimitLevel	=	_lpQuest->m_wMinLimitLevel;
	lpQuestSimpleInfo->m_wMaxLimitLevel	=	_lpQuest->m_wMaxLimitLevel;

	return	TRUE;	
}

//
//	퀘스트 정보 설정
void
CQuestManager::checkCloseQuest(int _iCorrectSerial)
{
	for (int i=0;i<dQUEST_COUNT;i++)
	{
		CQuestSimpleInfo	*lpQuest	=	&m_aQuest[i];

		lpQuest->m_isCloseQuest	=	g_project.isCloseQuest(i+_iCorrectSerial);
	}
}

BOOL
CQuestSimpleInfo::isProcessAblePlayer(cACTOR *_lpPlayer)
{
	if	(isEnableJob(_lpPlayer->m_wJob)	==	FALSE)
		return	FALSE;

	if	(!m_bf1IsMaleJobEquipAble)	
		if	(c_aIsMale[_lpPlayer->m_wJob])
			return	FALSE;		//	남자는 착용 불가

	if	(!m_bf1IsFemaleJobEquipAble)	
		if	(!c_aIsMale[_lpPlayer->m_wJob])	
			return	FALSE;		//	여자는 착용 불가

	if	(m_wMaxLimitLevel	==	0)
		return	TRUE;

	if	(_lpPlayer->m_iLevel	<	m_wMinLimitLevel)
		return	FALSE;

	if	(_lpPlayer->m_iLevel	>	m_wMaxLimitLevel)
		return	FALSE;

	return TRUE;
}
