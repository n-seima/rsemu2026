#include "CDialog.h"
#include "cACTOR.h"

char					g_strCurrentSpeechList[dMAX_CONVERSATION_COUNT][64];
CConversationContainer	g_clipConversationContainer;
CConversationContainer	g_clipConversation;

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃대화

//
//	초기화
BOOL
CConversation::init()
{
	pKILL(m_pTrigger);
	pKILL(m_pContents);
	pKILL(m_pstrSpeech);
	
	if (m_wTriggerCount	>	0)	m_pTrigger	=	new CKarmaItem [m_wTriggerCount];		//	선택 항목
	if (m_wContentsCount>	0)	m_pContents	=	new CKarmaContainer [m_wContentsCount];		//	선택 항목
	if (m_wSpeechLength	>	0)	m_pstrSpeech=	new char [m_wSpeechLength];

	return	TRUE;
}	//	CConversation::init()

//
//	문장 메모리 할당
void
CConversation::allocSpeechMemory()
{
	pKILL(m_pstrSpeech);
	if (m_wSpeechLength	>	0)	m_pstrSpeech=	new char [m_wSpeechLength];
}	//	CConversation::allocSpeechMemory()

//
//	트리거 메모리 할당
void
CConversation::allocTriggerMemory()
{
	pKILL(m_pTrigger);
	if (m_wTriggerCount	>	0)	m_pTrigger	=	new CKarmaItem [m_wTriggerCount];		//	선택 항목
}	//	CConversation::allocTriggerMemory()

//
//	선택문 메모리 할당
void
CConversation::allocContentsMemory()
{
	pKILL(m_pContents);
	if (m_wContentsCount>	0)	m_pContents	=	new CKarmaContainer [m_wContentsCount];		//	선택 항목
}	//	CConversation::allocContentsMemory()

//
//	초기화
BOOL
CConversation::init(int _iTriggerCount,int _iContentsCount)
{
	reset();

	if (_iTriggerCount	>	0)
	{
		m_pTrigger			=	new CKarmaItem [_iTriggerCount];		//	선택 항목
		m_wTriggerCount		=	_iTriggerCount;
	}

	if (_iContentsCount	>	0)
	{
		m_pContents			=	new CKarmaContainer [_iContentsCount];		//	선택 항목
		m_wContentsCount	=	_iContentsCount;
	}

	return	TRUE;
}	//	CConversation::init(int _iContentsCount)

//
//	리셋
void
CConversation::reset()
{
	m_wSerial		=	0xffff;	//	시리얼
	m_wSpeechLength	=	0;		//	대사 길이
	m_wContentsCount=	0;		//	선택 항목 수
	m_wTriggerCount	=	0;
	m_wIsNoSpeech	=	FALSE;
	m_bf7ActivateTriggerCount=	0;

	m_isWantAndCondition	=	TRUE;
	m_isWantNotComplete		=	FALSE;
	m_spareValue			=	FALSE;

	pKILL(m_pstrSpeech);	//	대사
	pKILL(m_pContents);	//	선택 항목
	pKILL(m_pTrigger);	//	선택 항목
}	//	CConversation::reset()

//
//	자폭 -o-
void
CConversation::destroy()
{
	reset();
}	//	CConversation::destroy()

//
//	제목 얻어오기(대사에서 일정길이 잘라옴)
char*
CConversation::getTitle()
{
	return	m_pstrSpeech;
}	//	CConversation::getTitle()

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
}	//	CConversation::setSpeech(char *_lpstrSpeech)

//
//	트리거 추가
BOOL
CConversation::addTrigger(CKarmaItem *_lpKarma)
{
	m_pTrigger[m_wTriggerCount].copy(_lpKarma);

	m_wTriggerCount++;

	return	TRUE;
}	//	CConversation::addTrigger(CKarmaItem *_lpKarma)

//
//	트리거 제거
BOOL
CConversation::removeTrigger(int _iIndex)
{
	if (_iIndex	>=	m_wTriggerCount)
		return	FALSE;

	m_pTrigger[_iIndex].Reset();

	for (int i=_iIndex;i<m_wTriggerCount-1;i++)	m_pTrigger[i].copy(&m_pTrigger[i+1]);

	m_pTrigger[m_wTriggerCount-1].Reset();

	m_wTriggerCount--;

	return	TRUE;
}	//	CConversation::removeTrigger(int _iIndex)

//
//	항목 추가
BOOL
CConversation::addContent(CKarmaContainer *_lpKc)
{
	m_pContents[m_wContentsCount].copy(_lpKc);
	m_pContents[m_wContentsCount].m_wSerial	=	m_wContentsCount;

	m_wContentsCount++;

	return	TRUE;
}	//	CConversation::addContent(CKarmaContainer *_lpKc)

//
//	항목 제거
BOOL
CConversation::removeContent(int _iIndex)
{
	if (_iIndex	>=	m_wContentsCount)	return	FALSE;

	m_pContents[_iIndex].reset();

	for (int i=_iIndex;i<m_wContentsCount-1;i++)	m_pContents[i].copy(&m_pContents[i+1]);

	m_pContents[m_wContentsCount-1].reset();

	m_wContentsCount--;

	return	TRUE;
}	//	CConversation::removeContent(int _iIndex)

//
//	복사
void
CConversation::copy(CConversation *_lpConversation,BOOL _isForEdit)
{
	int	i;

	if	(!_isForEdit)
		init(_lpConversation->m_wTriggerCount,_lpConversation->m_wContentsCount);

	m_wContentsCount	=	_lpConversation->m_wContentsCount;
	m_wTriggerCount		=	_lpConversation->m_wTriggerCount;

	m_isWantAndCondition=	_lpConversation->m_isWantAndCondition;
	m_isWantNotComplete	=	_lpConversation->m_isWantNotComplete;
	m_spareValue		=	_lpConversation->m_spareValue;
	m_bf7ActivateTriggerCount=	_lpConversation->m_bf7ActivateTriggerCount;
	m_wIsNoSpeech		=	_lpConversation->m_wIsNoSpeech;		//	대화 없음

	setSpeech(_lpConversation->m_pstrSpeech);

	for (i=0;i<m_wContentsCount;i++)
		m_pContents[i].copy(&_lpConversation->m_pContents[i]);
	for (i=0;i<m_wTriggerCount;i++)
		m_pTrigger[i].copy(&_lpConversation->m_pTrigger[i]);

	m_wSerial	=	0;
}
//┃대화
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃대화 컨테이너
CConversationContainer::CConversationContainer()
{
	m_wSerial		=	0xffff;
	m_pConversation	=	NULL;
	reset();
}	//	CConversationContainer::CConversationContainer()

CConversationContainer::~CConversationContainer()
{
	destroy();
}	//	CConversationContainer::~CConversationContainer()

//
//	초기화
BOOL
CConversationContainer::init()
{
	destroy();

	return	TRUE;
}	//	CConversationContainer::init()

//
//	초기화
BOOL
CConversationContainer::init(int _iConversationCount,BOOL _bIsAllocBuffer)
{
	destroy();

	m_pConversation		=	new CConversation	[_iConversationCount];

	if (_bIsAllocBuffer	==	FALSE)
		m_wConversationCount=	_iConversationCount;

	return	TRUE;
}

//
//	리셋
void
CConversationContainer::reset()
{
	m_wSerial				=	0xffff;
	m_wConversationCount	=	0;
	m_wDialogBoxSize		=	0;
	m_wIsRandomChoiceDialg	=	FALSE;

	pKILL(m_pConversation);
}

//
//	자폭 -o-
void
CConversationContainer::destroy()
{
	reset();
}	//	CConversationContainer::destroy()

//
//	오프너? 추가
BOOL
CConversationContainer::addSpeech(CConversation *_lpcNewSpeech)
{
	m_pConversation[m_wConversationCount].copy(_lpcNewSpeech);
	m_pConversation[m_wConversationCount].m_wSerial	=	m_wConversationCount;

	m_wConversationCount++;

	return	TRUE;
}

//
//	오프너? 제거
BOOL
CConversationContainer::removeSpeech(int _iIndex)
{
	if (_iIndex	>=	m_wConversationCount)	return	FALSE;

	m_pConversation[_iIndex].reset();

	for (int i=_iIndex;i<m_wConversationCount-1;i++)	m_pConversation[i].copy(&m_pConversation[i+1]);

	m_pConversation[m_wConversationCount-1].reset();

	m_wConversationCount--;

	return	TRUE;
}

//
//	복제
void
CConversationContainer::copy(CConversationContainer *_lpConversationContanier,BOOL _isForEdit)
{
	int	i;

	if (!_isForEdit)
		init(_lpConversationContanier->m_wConversationCount);

	m_wConversationCount	=	_lpConversationContanier->m_wConversationCount;
	m_wDialogBoxSize		=	_lpConversationContanier->m_wDialogBoxSize;
	m_wIsRandomChoiceDialg	=	_lpConversationContanier->m_wIsRandomChoiceDialg;

	for (i=0;i<m_wConversationCount	;i++)
		m_pConversation[i].copy(&_lpConversationContanier->m_pConversation[i]);

	m_wSerial				=	0;
}

//
//상점과 관련된 컨텐츠 구하기
CKarmaItem*
CConversationContainer::findShopContents(int _iShopIndex)
{
	for (int i=0;i<m_wConversationCount;i++)
	{
		CConversation	*lpSpeech	=	&m_pConversation[i];

		for (int j=0;j<lpSpeech->m_wContentsCount;j++)
		{
			CKarmaContainer	*lpContent	=	&lpSpeech->m_pContents[j];

			for (int k=0;k<lpContent->m_wKarmaCount;k++)
			{
				CKarma *lpSelectReaction	=	&lpContent->m_pKarma[k];

				for (int l=0;l<lpSelectReaction->m_wReactionCount;l++)
				{
					CKarmaItem	*lpReaction	=	&lpSelectReaction->m_pReaction[l];

					if (lpReaction->m_wItem	==	eREACTION_SHOP)
					{
						if (lpReaction->m_aValue[0]	==	_iShopIndex	||	_iShopIndex	==	0xffff)
							return	lpReaction;
					}
				}
			}
		}
	}

	return	NULL;
}

//
//상점과 관련된 컨텐츠 구하기
CKarmaItem*
CConversationContainer::findContents(int _iContents)
{
	for (int i=0;i<m_wConversationCount;i++)
	{
		CConversation	*lpSpeech	=	&m_pConversation[i];

		for (int j=0;j<lpSpeech->m_wContentsCount;j++)
		{
			CKarmaContainer	*lpContent	=	&lpSpeech->m_pContents[j];

			for (int k=0;k<lpContent->m_wKarmaCount;k++)
			{
				CKarma *lpSelectReaction	=	&lpContent->m_pKarma[k];

				for (int l=0;l<lpSelectReaction->m_wReactionCount;l++)
				{
					CKarmaItem	*lpReaction	=	&lpSelectReaction->m_pReaction[l];

					if (lpReaction->m_wItem	==	_iContents)
					{
						return	lpReaction;
					}
				}
			}
		}
	}

	return	NULL;
}


//
// 특정 대화를 사용하는가?
CKarmaItem*
CConversationContainer::findSpeech(int _iIndex)
{
	for (int i=0;i<m_wConversationCount;i++)
	{
		CConversation	*lpSpeech	=	&m_pConversation[i];

		for (int j=0;j<lpSpeech->m_wContentsCount;j++)
		{
			CKarmaContainer	*lpContent	=	&lpSpeech->m_pContents[j];

			for (int k=0;k<lpContent->m_wKarmaCount;k++)
			{
				CKarma *lpSelectReaction	=	&lpContent->m_pKarma[k];

				for (int l=0;l<lpSelectReaction->m_wReactionCount;l++)
				{
					CKarmaItem	*lpReaction	=	&lpSelectReaction->m_pReaction[l];

					if (lpReaction->m_wItem	==	eREACTION_SELECT_DIALOG)
					{
						if (lpReaction->m_aValue[0]	==	_iIndex	||	_iIndex	==	0xffff)
							return	lpReaction;
					}
				}
			}
		}
	}

	return	NULL;
}

BOOL
CConversationContainer::checkIncorrectKarma(cACTOR *_lpActor)
{
	int	iIncorrectKarma	=	0;

	for (int i=0;i<m_wConversationCount;i++)
	{
		CConversation	*lpSpeech	=	&m_pConversation[i];

		for (int j=0;j<lpSpeech->m_wContentsCount;j++)
		{
			CKarmaContainer	*lpContent	=	&lpSpeech->m_pContents[j];

			for (int k=0;k<lpContent->m_wKarmaCount;k++)
			{
				CKarma *lpSelectReaction	=	&lpContent->m_pKarma[k];

				for (int l=0;l<lpSelectReaction->m_wReactionCount;l++)
				{
					CKarmaItem	*lpReaction	=	&lpSelectReaction->m_pReaction[l];

					if	(lpReaction->m_wItem	==	eREACTION_SELECT_DIALOG)
						if	(lpReaction->m_aValue[0]	>=	m_wConversationCount)
						{
							iIncorrectKarma++;
							if	(_lpActor)
								cMSG::Error("카르마 체크","%s의 %d번째 대화 %d번째 선택문 %d번째 카르마 %d번째 리액션에 대화 선택의 대상이 없습니다.",
										_lpActor->m_strName,i,j,k,l);
						}

					if	(lpReaction->m_wItem	==	eREACTION_ADD_ITEM)
					{
						if	(lpReaction->m_aValue[0]	>=	dBASIC_ITEM_COUNT	||	g_aBasicItem[lpReaction->m_aValue[0]].m_iSerial	==	dVALID_ITEM)
						{
							iIncorrectKarma++;
							if	(_lpActor)
								cMSG::Error("카르마 체크","%s의 %d번째 대화 %d번째 선택문 %d번째 카르마 %d번째 리액션의 추가/제거 아이템이 비정상적입니다.",
										_lpActor->m_strName,i,j,k,l);
						}
					}
				}
			}
		}
	}

	return	iIncorrectKarma;
}
//┃대화 컨테이너
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━