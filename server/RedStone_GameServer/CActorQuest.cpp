#include "CActor.h"
#include "CQuest.H"
#include "debugCode.H"


//
//	진행중인 퀘스트에 추가 시킨다.
BOOL
cACTOR::addProcessQuest(int _iQuestType,int _iQuestIndex,BOOL _bIsSendToPlayer)
{
	int		i;

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if (m_aProcessQuest[i].isActive		==	0			)
			continue;
		if (m_aProcessQuest[i].questKind	!=	(DWORD)_iQuestType	)
			continue;
		if (m_aProcessQuest[i].questIndex	==	(DWORD)_iQuestIndex)
			return	FALSE;
	}

	CQuestManager		*lpQM;

	if (_iQuestType	==	eQT_NORMAL)
		lpQM	=	&g_quest;
	if (_iQuestType	==	eQT_ARBEIT)
		lpQM	=	&g_questArbeit;
	if (_iQuestIndex>=	lpQM->m_iCount)
	{
		CLOG("questError","뭐셔? %d",_iQuestIndex);
		return	FALSE;
	}

	CQuestSimpleInfo	*lpQuest	=	lpQM->getQuest(_iQuestIndex);

	if (lpQuest->m_isCloseQuest)
		return	FALSE;

	if (lpQuest->m_isTestQuestInTestServer && g_bIsTestServer == FALSE)
	{
		sendRegistServerMessage(eRSM_TEST_QUEST);
		return	FALSE;
	}


	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if (m_aProcessQuest[i].isActive		!=	0			)
			continue;

		m_aProcessQuest[i].isActive			=	TRUE;
		m_aProcessQuest[i].questIndex		=	_iQuestIndex;
		m_aProcessQuest[i].questKind		=	_iQuestType;
		m_aProcessQuest[i].questProcessLevel=	0;
		m_aProcessQuest[i].questValue		=	0;
		m_aProcessQuest[i].isComplete		=	FALSE;
		m_aProcessQuest[i].countdownStatus	=	eQCTS_SLEEP;

		if	(_bIsSendToPlayer)
			sendQuestChangeInfo(i);

		sendLogstatisticsQuest(_iQuestIndex+_iQuestType*10000, 1);

		return	TRUE;
	}

	return	FALSE;
}

//
//	퀘스트 상태를 설정한다.
BOOL
cACTOR::setQuestProcessLevel(int _iQuestType,int _iQuestIndex,int _iStatus,BOOL _bIsSendToPlayer)
{
	int		i;

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if	(m_aProcessQuest[i].isActive		==	FALSE		)
			continue;
		if	(m_aProcessQuest[i].questKind	!=	(DWORD)_iQuestType	)
			continue;
		if	(m_aProcessQuest[i].questIndex	!=	(DWORD)_iQuestIndex)
			continue;

		m_aProcessQuest[i].questProcessLevel=	_iStatus;

		if	(_bIsSendToPlayer)
			sendQuestChangeInfo(i);

		return	TRUE;
	}

	return	FALSE;
}

//
//	퀘스트 수치 설정
BOOL
cACTOR::setQuestValue(int _iQuestType,int _iQuestIndex,int _iValue,BOOL _bIsSendToPlayer)
{
	int		i;

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if (m_aProcessQuest[i].isActive		==	FALSE				)
			continue;
		if (m_aProcessQuest[i].questKind	!=	(DWORD)_iQuestType	)
			continue;
		if (m_aProcessQuest[i].questIndex	!=	(DWORD)_iQuestIndex	)
			continue;

		m_aProcessQuest[i].questValue		=	_iValue;

		if (_bIsSendToPlayer)
			sendQuestChangeInfo(i);

		return	TRUE;
	}

	return	FALSE;
}

BOOL
cACTOR::setQuestTimer(int _iType,int _iQuestIndex,int _iBeginEnd,int _iTime,BOOL _bIsSendToPlayer)
{
	int		i;

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if (m_aProcessQuest[i].isActive		==	FALSE				)
			continue;
		if (m_aProcessQuest[i].questKind	!=	(DWORD)_iType		)
			continue;
		if (m_aProcessQuest[i].questIndex	!=	(DWORD)_iQuestIndex	)
			continue;

		if (_iBeginEnd	==	0)
		{
			m_awQuestCountDownTimer[i]			=	max(_iTime,1);
			m_aProcessQuest[i].countdownStatus	=	eQCTS_PROCESS;
		}
		else
		{
			m_awQuestCountDownTimer[i]			=	0xffff;
			m_aProcessQuest[i].countdownStatus	=	eQCTS_COMPLETE;
		}

		if (_bIsSendToPlayer)
			sendQuestChangeInfo(i);

		return	TRUE;
	}

	return	FALSE;
}

//
//	퀘스트를 완료한 상태로 만든다.
BOOL
cACTOR::setQuestCountdonwTimerStatus(int _iQuestType,int _iQuestIndex,int _iStatus,BOOL _bIsSendToPlayer)
{
	int		i;

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if (m_aProcessQuest[i].isActive		==	FALSE				)
			continue;
		if (m_aProcessQuest[i].questKind	!=	(DWORD)_iQuestType	)
			continue;
		if (m_aProcessQuest[i].questIndex	!=	(DWORD)_iQuestIndex	)
			continue;

		m_aProcessQuest[i].countdownStatus	=	_iStatus;

		if (_bIsSendToPlayer)
			sendQuestChangeInfo(i);

		return	TRUE;
	}

	return	FALSE;
}

BOOL
cACTOR::checkReceiveQuestExp(int _iExp,int _iQuest,int _iQuestArbeit)
{
	int	iQuestCode	=	_iQuest;
	
	if	(_iQuestArbeit)
		iQuestCode	=	dQUEST_AND_ARBEIT_BORDER_VALUE+_iQuestArbeit;

	{
		for (int i=dCHECK_QUEST_EXP_BUFFER_SIZE-1;i>=1;i--)
		{
			m_aiLastReceiveQuestExp[i][0]	=	m_aiLastReceiveQuestExp[i-1][0];
			m_aiLastReceiveQuestExp[i][1]	=	m_aiLastReceiveQuestExp[i-1][1];
		}

		m_aiLastReceiveQuestExp[0][0]	=	_iExp;
		m_aiLastReceiveQuestExp[0][1]	=	iQuestCode;
	}

	{
		for (int iSlot=0;iSlot<dCHECK_QUEST_EXP_BUFFER_SIZE-1;iSlot++)
		{
			if	(m_aiLastReceiveQuestExp[iSlot][0]	==	0)
				break;

			for (int i=iSlot+1;i<dCHECK_QUEST_EXP_BUFFER_SIZE;i++)
			{
				if	(m_aiLastReceiveQuestExp[iSlot][0]	==	m_aiLastReceiveQuestExp[i][0] &&	//	같은 퀘스트 관련 경험치를 연속으로 받았다.
					m_aiLastReceiveQuestExp[iSlot][1]	==	m_aiLastReceiveQuestExp[i][1] )
				{
					sendWarningQuestExpSerialReceive(_iExp,iQuestCode);

					CLOG("questExpSerialReceive","[%s/%s] %d,%d",m_strId,m_strName,_iExp,iQuestCode);
				}
			}
		}
	}

	return	TRUE;
}

//
//	퀘스트를 완료한 상태로 만든다.
BOOL
cACTOR::setQuestCompleteChecker(int _iQuestType,int _iQuestIndex,BOOL _bIsComplete,BOOL _bIsSendToPlayer)
{
	int		i;

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if (m_aProcessQuest[i].isActive		==	FALSE				)
			continue;
		if (m_aProcessQuest[i].questKind	!=	(DWORD)_iQuestType	)
			continue;
		if (m_aProcessQuest[i].questIndex	!=	(DWORD)_iQuestIndex	)
			continue;

		m_aProcessQuest[i].isComplete		=	_bIsComplete;

		if	(_bIsSendToPlayer)
			sendQuestChangeInfo(i);

		return	TRUE;
	}

	return	FALSE;
}

//
//	퀘스트를 완료한 상태로 만든다.
BOOL
cACTOR::setQuestComplete(int _iQuestType,int _iQuestIndex,BOOL _bIsSendToPlayer ,BOOL _bIsHideComplateMessage)
{
	if	(_iQuestType	==	eQT_NORMAL)
	{
		int	iSlot	=	_iQuestIndex/8;

		if	(iSlot	>=	dQUEST_BUFFER_SIZE)
		{
			iSlot	-=	dQUEST_BUFFER_SIZE;

			m_aCompleteQuest2[iSlot] |= (1<<(_iQuestIndex%8));
		}
		else
			m_aCompleteQuest[iSlot] |= (1<<(_iQuestIndex%8));

		if	(!cancelQuest(_iQuestType,_iQuestIndex,_bIsSendToPlayer,TRUE, FALSE))
			return	FALSE;

		sendEtcWork(eEW_QUEST_COMPLETE,_iQuestType,_iQuestIndex , _bIsHideComplateMessage);
		sendCompleteQuest(_iQuestType,_iQuestIndex);

		return	TRUE;
	}

	int		iSlot		=	_iQuestIndex/8;
	BYTE	*lpBuffer	=	m_aCompleteArbeit;

	if	(iSlot	>=	dARBEIT_BUFFER_SIZE)
	{
		lpBuffer		=	m_aCompleteArbeit2;
		iSlot			-=	dARBEIT_BUFFER_SIZE;
	}

	lpBuffer[iSlot] |= (1<<(_iQuestIndex%8));

	if	(!cancelQuest(_iQuestType,_iQuestIndex,_bIsSendToPlayer,TRUE , FALSE))
		return	FALSE;

	sendEtcWork(eEW_QUEST_COMPLETE,_iQuestType,_iQuestIndex , _bIsHideComplateMessage);
	sendCompleteQuest(_iQuestType,_iQuestIndex);

	return	TRUE;
}

//
//	퀘스트를 취소한다.
BOOL
cACTOR::cancelQuest(int _iQuestType,int _iQuestIndex,BOOL _bIsSendToPlayer,BOOL _bIsForQuestComplete, BOOL _bIsSendStatistics)
{
	int		i;

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if	(m_aProcessQuest[i].isActive	==	FALSE				)
			continue;
		if	(m_aProcessQuest[i].questKind	!=	(DWORD)_iQuestType	)
			continue;
		if	(m_aProcessQuest[i].questIndex	!=	(DWORD)_iQuestIndex	)
			continue;

		CQuestSimpleInfo	*lpQuest	=	getQuestSimpleInfo(i);

		if	(!lpQuest)
			return	FALSE;

		if	(lpQuest->m_isRemoveQuestItemByCancelQuest)
			removeQuestItem(m_aProcessQuest[i].questKind,m_aProcessQuest[i].questIndex);

		m_aProcessQuest[i].isActive			=	FALSE;
		m_aProcessQuest[i].questKind		=	0;
		m_aProcessQuest[i].questProcessLevel=	0;
		m_aProcessQuest[i].questValue		=	0;
		m_aProcessQuest[i].questIndex		=	0;

		if	(_bIsSendToPlayer)
			sendQuestChangeInfo(i,_bIsForQuestComplete);

		if (_bIsSendStatistics)
			sendLogstatisticsQuest(_iQuestIndex+_iQuestType*10000 , 2);

		return	TRUE;
	}

	return	TRUE;
}

//
//	퀘스트를 취소한다.
BOOL
cACTOR::cancelQuestByClient(int _iQuestSlot,int _iQuestIndex)
{
	if	(_iQuestSlot	>=	dMAX_PROCESS_QUEST_COUNT)
		return	FALSE;

	tsProcessQuestField	*lpQuestInfo	=	&m_aProcessQuest[_iQuestSlot];

	if	(lpQuestInfo->isActive	==	FALSE					)
		return	FALSE;
	if	(lpQuestInfo->questIndex	!=	(DWORD)_iQuestIndex	)
		return	FALSE;

	CQuestSimpleInfo	*lpQuest	=	getQuestSimpleInfo(_iQuestSlot);

	if	(!lpQuest || !lpQuest->m_isEnableCancel)
		return	FALSE;

	if	(lpQuest->m_isRemoveQuestItemByCancelQuest)
		removeQuestItem(lpQuestInfo->questKind,lpQuestInfo->questIndex);

	sendLogstatisticsQuest(_iQuestIndex+lpQuestInfo->questKind*10000 , 2);

	lpQuestInfo->isActive			=	FALSE;
	lpQuestInfo->questKind			=	0;
	lpQuestInfo->questProcessLevel	=	0;
	lpQuestInfo->questValue			=	0;
	lpQuestInfo->questIndex			=	0;

	return	TRUE;
}

//
//	퀘스트 수치 조절
BOOL
cACTOR::controlQuestValue(int _iQuestType,int _iQuestIndex,int _iMethod,int _iValue,BOOL _bIsSendToPlayer,int _iLimitValue)
{
	int		i;
	int		iResultValue	=	0;

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if	(m_aProcessQuest[i].isActive		==	FALSE				)
			continue;
		if	(m_aProcessQuest[i].questKind	!=	(DWORD)_iQuestType	)
			continue;
		if	(m_aProcessQuest[i].questIndex	!=	(DWORD)_iQuestIndex	)
			continue;

		if	(_iMethod	==	0)
			iResultValue	=	m_aProcessQuest[i].questValue	+	_iValue;
		else
		if	(_iMethod	==	1)
		{
			if	(m_aProcessQuest[i].questValue>=(DWORD)_iValue)	
				iResultValue	=	m_aProcessQuest[i].questValue-(DWORD)_iValue;
			else
				iResultValue	=	0;
		}

		if	(_iLimitValue)
			iResultValue	=	min(iResultValue,_iLimitValue);

		m_aProcessQuest[i].questValue	=	iResultValue;

		if	(_bIsSendToPlayer)
			sendQuestChangeInfo(i);

		return	TRUE;
	}

	return	FALSE;
}

//
//	이거 완료한 퀘스트냐?
BOOL
cACTOR::isCompleteQuest(int _iQuestType,int _iQuestIndex)
{
	if	(_iQuestType	==	eQT_NORMAL)
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

	int		iSlot		=	_iQuestIndex/8;
	BYTE	*lpBuffer	=	m_aCompleteArbeit;

	if	(iSlot	>=	dARBEIT_BUFFER_SIZE)
	{
		lpBuffer		=	m_aCompleteArbeit2;
		iSlot			-=	dARBEIT_BUFFER_SIZE;
	}

	if	(lpBuffer[iSlot]&(1<<(_iQuestIndex%8)))
		return	TRUE;

	return	FALSE;
}

BOOL
cACTOR::isProcessAbleQuest(int _iQuestType,int _iQuestIndex)
{
	if	(isCompleteQuest(_iQuestType,_iQuestIndex))	//	이미 완료한 퀘스트다.
		return	FALSE;

	if	(isOwnQuest(_iQuestType,_iQuestIndex))	//	수행중인 퀘스트다.
		return	FALSE;

	if	(getProcessQuestCount()	>=	dMAX_PROCESS_QUEST_COUNT)	//	퀘스트 받을 자리가 없다.
		return	FALSE;

	CQuestSimpleInfo	*lpQuest;

	if (_iQuestType	==	eQT_NORMAL)	
		lpQuest	=	g_quest.getQuest(_iQuestIndex);
	if (_iQuestType	==	eQT_ARBEIT)	
		lpQuest	=	g_questArbeit.getQuest(_iQuestIndex);

	if (!lpQuest)
		return	FALSE;

	if (!lpQuest->isProcessAblePlayer(this))	//	레벨 제한이나 직업 제한에 걸린다.
		return	FALSE;

	return	TRUE;
}

BOOL
cACTOR::isCheckedQuestCompleteChecker(int _iQuestType,int _iQuestIndex)
{
	int		i;

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if (m_aProcessQuest[i].isActive		==	FALSE				)
			continue;
		if (m_aProcessQuest[i].questKind	!=	(DWORD)_iQuestType	)
			continue;
		if (m_aProcessQuest[i].questIndex	!=	(DWORD)_iQuestIndex	)
			continue;

		return	m_aProcessQuest[i].isComplete;
	}

	return	FALSE;
}

int
cACTOR::getQuestCountdownTimerStatus(int _iQuestType,int _iQuestIndex)
{
	int		i;

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if (m_aProcessQuest[i].isActive		==	FALSE				)
			continue;
		if (m_aProcessQuest[i].questKind	!=	(DWORD)_iQuestType	)
			continue;
		if (m_aProcessQuest[i].questIndex	!=	(DWORD)_iQuestIndex	)
			continue;

		return	m_aProcessQuest[i].countdownStatus;
	}

	return	eQCTS_SLEEP;
}

//
//	이거 갖고 있는 퀘스트냐?
BOOL
cACTOR::isOwnQuest(int _iQuestType,int _iQuestIndex)
{
	int	i;

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if (m_aProcessQuest[i].isActive		==	FALSE				)
			continue;
		if (m_aProcessQuest[i].questKind	!=	(DWORD)_iQuestType	)
			continue;
		if (m_aProcessQuest[i].questIndex	==	(DWORD)_iQuestIndex	)
			return	TRUE;
	}

	return	FALSE;
}

//
//	진행중인 퀘스트 수
int
cACTOR::getProcessQuestCount(int _iQuestKind)
{
	int	i,iQuestCount=0;

	CQuestSimpleInfo	*lpQuest		=	NULL;

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if	(m_aProcessQuest[i].isActive		==	FALSE		)
			continue;

		if	(_iQuestKind	!=	0)
		{
			tsProcessQuestField	*lpQuestInfo	=	&m_aProcessQuest[i];

			if (lpQuestInfo->questKind	==	eQT_NORMAL)
				lpQuest	=	g_quest.getQuest(lpQuestInfo->questIndex);
			else
			if (lpQuestInfo->questKind	==	eQT_ARBEIT)
				lpQuest	=	g_questArbeit.getQuest(lpQuestInfo->questIndex);

			if	(lpQuest->m_wQuestKind	!=	_iQuestKind-1)
				continue;
		}

		iQuestCount++;
	}

	return	iQuestCount;
}

//
//	퀘스트 진행 레벨 얻기
int
cACTOR::getQuestProcessLevel(int _iQuestType,int _iQuestIndex)
{
	int		i;

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if (m_aProcessQuest[i].isActive		==	FALSE				)
			continue;
		if (m_aProcessQuest[i].questKind	!=	(DWORD)_iQuestType	)
			continue;
		if (m_aProcessQuest[i].questIndex	!=	(DWORD)_iQuestIndex	)
			continue;

		return	m_aProcessQuest[i].questProcessLevel;
	}

	return	0xffff;
}

//
//	퀘스트 수치 얻기
int
cACTOR::getQuestValue(int _iQuestType,int _iQuestIndex)
{
	int		i;

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if (m_aProcessQuest[i].isActive		==	FALSE				)
			continue;
		if (m_aProcessQuest[i].questKind	!=	(DWORD)_iQuestType	)
			continue;
		if (m_aProcessQuest[i].questIndex	!=	(DWORD)_iQuestIndex	)
			continue;

		return	m_aProcessQuest[i].questValue;
	}

	return	0xffff;
}


CQuestSimpleInfo*
cACTOR::getQuestSimpleInfo(int _iQuestSlot)
{
	if	(_iQuestSlot < 0 || _iQuestSlot >=	dMAX_PROCESS_QUEST_COUNT)	return	NULL;

	CQuestSimpleInfo	*lpQuest		=	NULL;
	tsProcessQuestField	*lpQuestInfo	=	&m_aProcessQuest[_iQuestSlot];
		
	if (lpQuestInfo->isActive	==	FALSE		)
		return	NULL;
	if (lpQuestInfo->questKind	==	eQT_NORMAL)
		lpQuest	=	g_quest.getQuest(lpQuestInfo->questIndex);
	if (lpQuestInfo->questKind	==	eQT_ARBEIT)
		lpQuest	=	g_questArbeit.getQuest(lpQuestInfo->questIndex);

	return	lpQuest;
}

//
//	진행중인 퀘스트 얻기
tsProcessQuestField*
cACTOR::getProcessQuest(int _iQuestType,int _iQuestIndex)
{
	int		i;

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if	(m_aProcessQuest[i].isActive	==	FALSE				)
			continue;
		if	(m_aProcessQuest[i].questKind	!=	(DWORD)_iQuestType	)
			continue;
		if	(m_aProcessQuest[i].questIndex	!=	(DWORD)_iQuestIndex	)
			continue;

		return	&m_aProcessQuest[i];
	}

	return	NULL;
}

void
cACTOR::setChapter(int _iChapter,int _iHighQuest)
{
	m_wProcessChapterIndex	=	_iChapter;
	m_wProcessHighQuestIndex=	_iHighQuest;

	sendEtcWork(eEW_SET_CHAPTER,m_wProcessChapterIndex,m_wProcessHighQuestIndex);
	
	if	(m_wProcessHighQuestIndex	==	0)
		m_bf1IsProcessEvent			=	TRUE;
}

void
cACTOR::cancelChapter()
{
	m_wProcessChapterIndex	=	0xffff;
	m_wProcessHighQuestIndex=	0xffff;

	sendEtcWork(eEW_SET_CHAPTER,0xffff,0xffff);
}

int
cACTOR::getMainQuestValue(int _iValueIndex)
{
	return	m_awMainQuestValue[_iValueIndex];
}

void
cACTOR::setMainQuestValue(int _iValueIndex,int _iValue)
{
	m_awMainQuestValue[_iValueIndex]	=	min(_iValue,0xffff);
}

void
cACTOR::addMainQuestValue(int _iValueIndex,int _iValue)
{
	int	iValue	=	m_awMainQuestValue[_iValueIndex]+_iValue;

	if	(iValue	<	0)
		iValue	=	0;

	m_awMainQuestValue[_iValueIndex]	=	min(iValue,0xffff);
}

int
cACTOR::getSwitchStatus(int _iSwitchIndex)
{
	int	iCheckValue	=	(1<<(_iSwitchIndex%8));

	if	((m_abMainQuestSwitchStatus[_iSwitchIndex/8]&iCheckValue) == iCheckValue)
		return	TRUE;

	return	FALSE;
}

void
cACTOR::setMainQuestSwitch(int _iSwitchIndex,BOOL _bIsOn)
{
	int	iCheckValue	=	(1<<(_iSwitchIndex%8));

	if	(_bIsOn)
		m_abMainQuestSwitchStatus[_iSwitchIndex/8] |= iCheckValue;
	else
	{
		if	(m_abMainQuestSwitchStatus[_iSwitchIndex/8]&iCheckValue)
			m_abMainQuestSwitchStatus[_iSwitchIndex/8]	-=	iCheckValue;
	}
}

void
cACTOR::resetMainQuestValue()
{
	memset(m_awMainQuestValue,0,sizeof(m_awMainQuestValue));
}

void
cACTOR::resetMainQuestSwitch()
{
	memset(m_abMainQuestSwitchStatus,0,sizeof(m_abMainQuestSwitchStatus));
}
