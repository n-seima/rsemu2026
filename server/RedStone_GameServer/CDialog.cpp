#include "CDialog.h"
#include "cACTOR.h"
#include "CGame.h"

//
//	초기화
BOOL
CConversation::init()
{
	if (m_wTriggerCount	>	0)	m_pTrigger	=	new CKarmaItem		[m_wTriggerCount];		//	선택 항목
	if (m_wContentsCount>	0)	m_pContents	=	new CKarmaContainer [m_wContentsCount];		//	선택 항목
	if (m_wSpeechLength	>	0)	m_pstrSpeech=	new char			[m_wSpeechLength];

	return	TRUE;
}

//
//	초기화
BOOL
CConversation::init(int _iTriggerCount,int _iContentsCount)
{
	reset();

	if (_iTriggerCount	>	0)	m_pTrigger	=	new CKarmaItem		[_iTriggerCount];		//	선택 항목
	if (_iContentsCount	>	0)	m_pContents	=	new CKarmaContainer [_iContentsCount];		//	선택 항목
	
	m_wContentsCount	=	_iContentsCount;
	m_wTriggerCount		=	_iTriggerCount;

	return	TRUE;
}

//
//	리셋
void
CConversation::reset()
{
	m_wSerial		=	0xffff;	//	시리얼
	m_wSpeechLength	=	0;		//	대사 길이
	m_wContentsCount=	0;		//	선택 항목 수
	m_wTriggerCount	=	0;

	pKILL(m_pstrSpeech);	//	대사
	pKILL(m_pContents);	//	선택 항목
	pKILL(m_pTrigger);	//	선택 항목
}

//
//	자폭 -o-
void
CConversation::destroy()
{
	reset();
}

//
//	제목 얻어오기(대사에서 일정길이 잘라옴)
char*
CConversation::getTitle()
{
	return	m_pstrSpeech;
}

//
//	대사 설정
void
CConversation::setSpeech(char *_lpstrSpeech)
{
	pKILL(m_pstrSpeech);

	if (_lpstrSpeech	== NULL)
	{	m_wSpeechLength	=	0;
		return;
	}

	m_wSpeechLength	=	strlen(_lpstrSpeech)+1;
	m_pstrSpeech	=	new char [m_wSpeechLength];

	strcpy(m_pstrSpeech,_lpstrSpeech);
}

//
//	대화 리턴
BOOL
CConversation::getSpeech(CSpeech* _lpReturnBuffer)
{
	int	iTextPoint	=	0,i;

	if (m_wSpeechLength	==	0)
	{
		_lpReturnBuffer->m_wLength	=	0;	//	대사는 없고 반응만 있다.

		return	TRUE;
	}

	_lpReturnBuffer->m_wContentsCount	=	m_wContentsCount;

	strcpy(_lpReturnBuffer->m_strText,m_pstrSpeech);

	iTextPoint		=	m_wSpeechLength;

	for (i=0;i<m_wContentsCount;i++)
	{
		strcpy(_lpReturnBuffer->m_strText+iTextPoint,m_pContents[i].m_pstrTitle);
		iTextPoint	+=	m_pContents[i].m_wTitleLength;
	}

	_lpReturnBuffer->m_wLength	=	iTextPoint;

	return	TRUE;
}

//
//	가지고 있는 트리거 조건이 만족되냐?
BOOL
CConversation::isCompleteTrigger(cACTOR *_lpAsker,cACTOR *_lpOwner)
{
	int	i,iCompleteTriggerCount=0;

	if (m_wTriggerCount	<=	0)
		return	TRUE;

	if (g_bIsWantPrintTriggerCondition)
	{
		char	*strCondition[]	=	{"or","and"};
		char	*strNot[]		=	{"만족","불만족"};

		printf("        트리거 체크 시작 - 조건 %s/%s \n",strCondition[m_isWantAndCondition],strNot[m_isWantNotComplete]);
	}

	cFIELD	*lpField	=	NULL;

	if	(_lpAsker)
		lpField	=	_lpAsker->m_lpField;

	int	iRequireCompleteTriggerCount	=	1;

	if	(m_isWantAndCondition)
		iRequireCompleteTriggerCount	=	m_wTriggerCount;
	else
	if	(m_bf7ActivateTriggerCount)
	{
		iRequireCompleteTriggerCount	=	m_bf7ActivateTriggerCount;
		iRequireCompleteTriggerCount	=	min(iRequireCompleteTriggerCount,m_wTriggerCount);
	}

	for (i=0;i<m_wTriggerCount;i++)
	{
		BOOL	isComplete		=	m_pTrigger[i].isCompleteTrigger(_lpAsker,_lpOwner,lpField);

		if	(isComplete	==	FALSE	&&	(m_pTrigger[i].m_bf1IsAbsolute || m_isWantAndCondition== TRUE ))	//	절대 트리거 체크
		{
			if	(m_isWantNotComplete)	//	실패 해야 성공하는 카르마
				return	TRUE;

			return	FALSE;
		}

		iCompleteTriggerCount	+=	isComplete;
	}

	if	(m_isWantNotComplete)
	{
		if	(iCompleteTriggerCount	>=	iRequireCompleteTriggerCount)
			return	FALSE;

		return	TRUE;
	}

	if	(iCompleteTriggerCount	>=	iRequireCompleteTriggerCount)
		return	TRUE;

	return	FALSE;
}

//
//	이런 상점을 언급하냐?
BOOL
CConversation::isExistShop(int _iSerial)
{
	int	i,j;

	for (i=0;i<m_wContentsCount;i++)
	{
		CKarmaContainer	*lpkcSpeech	=	&m_pContents[i];

		for (int iKarma=0;iKarma<lpkcSpeech->m_wKarmaCount;iKarma++)
		{
			CKarma	*lpSelectSpeech	=	&lpkcSpeech->m_pKarma[iKarma];

			for (j=0;j<lpSelectSpeech->m_wReactionCount;j++)
			{
				CKarmaItem	*lpReaction	=	&lpSelectSpeech->m_pReaction[j];

				if (lpReaction->m_wItem	==	eREACTION_SHOP)
				{
					if (lpReaction->m_aValue[0]	==	_iSerial)
						return	TRUE;
				}
			}
		}
	}

	return	FALSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////	대화 컨테이너
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//
//
CConversationContainer::CConversationContainer()
{
	m_wSerial		=	0xffff;
	m_pConversation	=	NULL;
}

//
//
CConversationContainer::~CConversationContainer()
{
	destroy();
}

//
//	초기화
BOOL
CConversationContainer::init()
{
	destroy();

	return	TRUE;
}

//
//	초기화
BOOL
CConversationContainer::init(int _iConversationCount)
{
	destroy();

	m_pConversation		=	new CConversation	[_iConversationCount];

	m_wConversationCount=	_iConversationCount;

	return	TRUE;
}

//
//	리셋
void
CConversationContainer::reset()
{
	m_wIsEventArea			=	FALSE;
	m_wSerial				=	0xffff;
	m_wConversationCount	=	0;

	pKILL(m_pConversation);
}

//
//	자폭 -o-
void
CConversationContainer::destroy()
{
	reset();
}

//
//	첫번째 대사를 찾아서 실행
BOOL
CConversationContainer::operateFirstSpeech(cACTOR *_lpAsker,cACTOR *_lpOwner,WORD _wEventArea)
{
	CSpeech	speech;
	int		i;
	BOOL	bIsNoSpeech		=	FALSE;

	speech.m_wDialogBoxSize				=	m_wDialogBoxSize;
	_lpAsker->m_wTalkNpc				=	_lpOwner->m_wSerialInField;
	_lpAsker->m_wSelectConversation		=	0xffff;
	_lpAsker->m_wEventArea				=	_wEventArea;

	if (g_bIsWantPrintTriggerCondition)
		printf("    첫번째 대화 시작 조건 체크\n");

	if (m_wIsRandomChoiceDialg)
	{
		int				iSpeech			=	random(m_wConversationCount);
		CConversation	*lpSpeech		=	&m_pConversation[iSpeech];
		
		if(!lpSpeech)		// 09.08.13
			return FALSE;

		if (lpSpeech->m_wIsNoSpeech)
			return	operateSelectSpeechReaction(_lpAsker,_lpOwner,iSpeech,0);

		speech.m_wConversationIndex		=	iSpeech;
		lpSpeech->getSpeech(&speech);
	}
	else
	for (i=0;i<m_wConversationCount;i++)
	{
		CConversation	*lpSpeech	=	&m_pConversation[i];

		if (lpSpeech->isCompleteTrigger(_lpAsker,_lpOwner))
		{
			if (m_pConversation[i].m_wIsNoSpeech)
				return	operateSelectSpeechReaction(_lpAsker,_lpOwner,i,0);

			speech.m_wConversationIndex	=	i;
			m_pConversation[i].getSpeech(&speech);
			break;
		}
	}

	if (speech.m_wConversationIndex	==	0xffff)
	{
		speech.m_wConversationIndex	=	0;
		m_pConversation[0].getSpeech(&speech);

		if (m_pConversation[0].m_wIsNoSpeech)
			return	operateSelectSpeechReaction(_lpAsker,_lpOwner,0,0);
	}

	return	_lpOwner->sendSpeech(_lpAsker,&speech);
}

//
//대사 선택 결과
BOOL
CConversationContainer::operateSelectSpeechReaction(cACTOR *_lpAsker,cACTOR *_lpOwner,int _iConversationIndex,int _iSelectSpeech)
{
	g_debugSign.m_iTempStep	=	5;

	if	(_iConversationIndex	>=	m_wConversationCount ||	_iConversationIndex	<	0)
		return	FALSE;

	CConversation	*lpSpeech	=	&m_pConversation[_iConversationIndex];

	if	(_iSelectSpeech	>=	lpSpeech->m_wContentsCount	||	_iSelectSpeech	<	0)
		return	FALSE;

	CKarmaContainer	*lpContents	=	&lpSpeech->m_pContents[_iSelectSpeech];

	if	(lpContents->m_wKarmaCount	==	0)
	{
		g_debugSign.m_iTempStep	=	6;

		_lpAsker->closeTalk();

		return	FALSE;
	}

	if	(g_bIsWantPrintTriggerCondition)
		printf("    %.2d 번째 대화(선택 문장 %.2d번)\n",_iConversationIndex,_iSelectSpeech);

	g_debugSign.m_iTempStep	=	7;

	for (int i=0;i<lpContents->m_wKarmaCount;i++)
	{
		BOOL	bIsStopWork	=	FALSE;

		if	(g_bIsWantPrintTriggerCondition)
		{
			if	(lpContents->m_pKarma[i].m_pstrTitle)
				printf("      %.2d '%s' 카르마 처리 시작\n",i,lpContents->m_pKarma[i].m_pstrTitle);
			else
				printf("      %.2d 카르마 처리 시작\n",i);
		}

		if	(m_wIsEventArea	==	FALSE)
		{
			lpContents->m_pKarma[i].m_wRemainCount	=	0xffff;
			lpContents->m_pKarma[i].m_bf1IsActive	=	TRUE;
		}

		int	iJumpKarma	=	-1;

		lpContents->m_pKarma[i].operate(_lpAsker,_lpOwner,&bIsStopWork,_lpAsker->m_lpField,FALSE,&iJumpKarma,lpContents);

		if	(iJumpKarma	!=	-1)
		{
			if	(iJumpKarma	>=	lpContents->m_wKarmaCount)
			{
				if	(g_bIsWantPrintTriggerCondition)
					printf("카르마 %d번으로 점프하라고 했지만 카르마 수가 %d개다.",iJumpKarma,lpContents->m_wKarmaCount);
			}

			i	=	iJumpKarma-1;
		}

		if	(bIsStopWork)
			break;
	}

	g_debugSign.m_iTempStep	=	8;

	return	TRUE;
}

//
//	이런 상점을 언급하냐?
BOOL
CConversationContainer::isExistShop(int _iSerial)
{
	int	i;

	for (i=0;i<m_wConversationCount;i++)
	{
		CConversation	*lpSpeech	=	&m_pConversation[i];

		if (lpSpeech->isExistShop(_iSerial))	return	TRUE;
	}

	return	FALSE;
}