#include "CKarma.H"
#include "cMAP.H"


CKarmaContainer	*g_lpAKC				=	NULL;
int				g_iSelectKarma			=	0;
CKarma			g_EditKarma;								//	현재 편집중인 카르마
int				CKarma::s_iNewKarma		=	0;
char			g_strGroupName[128];
char			g_strKarmaName[128];
char			g_astrWorldValueTxt[256][30];

//
//	카르마를 리셋한다.
void
CKarma::reset(BOOL full)
{
	m_wSerial				=	0xffff;				//	카르마 시리얼
	m_isWantAndCondition	=	TRUE;
	m_isWantNotComplete		=	FALSE;
	m_wReactionCount		=	0;
	m_wTriggerCount			=	0;					//	조건의 수,반응의 수
	m_wActivateChance		=	100;
	m_bf1IsDisable			=	FALSE;
	m_bf1IsForEnterFieldPlayer=	FALSE;
	
	m_wActiveCount			=	0;
	m_wActivePeriod			=	0;

	pKILL(m_pTrigger);
	pKILL(m_pReaction);
	pKILL(m_pstrTitle);

	if	(full)
	{
		m_pTrigger	=	new	CKarmaItem [dTRIGGER_COUNT];
		m_pReaction	=	new	CKarmaItem[dREACTION_COUNT];
	}
}	//	CKarma::Reset(BOOL full)

//
//	카르마를 리셋한다.
void
CKarma::reset(int iTriggerCount,int iReactionCount)
{
	reset();

	m_wTriggerCount	=	iTriggerCount;
	m_wReactionCount=	iReactionCount;

	if (iTriggerCount	>	0)	m_pTrigger	=	new	CKarmaItem [iTriggerCount];
	if (iReactionCount	>	0)	m_pReaction	=	new	CKarmaItem [iReactionCount];
}	//	CKarma::Reset(int iTriggerCount,int iReactionCount)

//
//	카르마를 리셋한다.
void
CKarma::allocBuffer(int iTriggerCount,int iReactionCount)
{
	if (iTriggerCount	!=	-1)	m_wTriggerCount	=	iTriggerCount;
	if (iReactionCount	!=	-1)	m_wReactionCount=	iReactionCount;

	pKILL(m_pTrigger);
	pKILL(m_pReaction);

	if (m_wTriggerCount	>	0)	m_pTrigger	=	new	CKarmaItem [m_wTriggerCount];
	if (m_wReactionCount>	0)	m_pReaction	=	new	CKarmaItem [m_wReactionCount];
}	//	CKarma::Reset(int iTriggerCount,int iReactionCount)

//
//	카르마를 셋팅한다.(템플릿..)
void
CKarma::copy(CKarma *lpKarma,BOOL full)
{
	if (full)
	{
		reset(dTRIGGER_COUNT,dREACTION_COUNT);

		m_wTriggerCount	=	lpKarma->m_wTriggerCount;
		m_wReactionCount=	lpKarma->m_wReactionCount;
	}
	else
		reset(lpKarma->m_wTriggerCount,lpKarma->m_wReactionCount);

	memcpy(this,lpKarma,sizeof(CKarmaBasicInfo));	//	수치들

	setTitle(lpKarma->m_pstrTitle);

	int	i;

	for (i=0;i<m_wTriggerCount	;i++)
		m_pTrigger[i].copy(&lpKarma->m_pTrigger[i]);
	for (i=0;i<m_wReactionCount	;i++)
		m_pReaction[i].copy(&lpKarma->m_pReaction[i]);
}

//
//	리액션 제거			//	리액션 ㅔ거
BOOL
CKarma::RemoveReaction(int index,int count)
{
	int	i;

	if (index >= m_wReactionCount)	return	FALSE;
	if (m_wReactionCount	<=	0)	return	FALSE;

	for (i=index;i<m_wReactionCount-count;i++)	m_pReaction[i].copy(&m_pReaction[i+count]);

	m_wReactionCount-=count;

	return	TRUE;
}	//	CKarma::RemoveReaction(int index)

//
//	트리거 제거
BOOL
CKarma::RemoveTrigger(int index,int count)
{
	int	i;

	if (index >= m_wTriggerCount)	return	FALSE;
	if (m_wTriggerCount	<=	0	)	return	FALSE;

	for (i=index;i<m_wTriggerCount-count;i++)	m_pTrigger[i].copy(&m_pTrigger[i+count]);

	m_wTriggerCount-=count;

	m_pTrigger[m_wTriggerCount].Reset();

	return	TRUE;
}	//	CKarma::RemoveTrigger(int index)

//
//	반응들을 붙여 넣는다.
void
CKarma::PasteReaction(int pos,CKarma *lpClipKarma)
{
	int	i,iCount,iBufferSize;
	CKarmaItem	*pReaction;								//	임시 버퍼

	iCount		=	lpClipKarma->m_wReactionCount;	//	붙여넣을 반응의 수

	if (pos	==	0xffff)	//	맨 끝에 붙인다.
	{
		pos	=	m_wReactionCount;

		for (i=pos;i<pos+iCount		;i++)	m_pReaction[i].copy(&lpClipKarma->m_pReaction[i-pos]);

		m_wReactionCount	+=	iCount;

		return;
	}

	iBufferSize	=	m_wReactionCount-pos;

	pReaction	=	new CKarmaItem [iBufferSize];			//	붙여 넣을 수 만큼 임시 버퍼를 만든다.

	for (i=pos;i<pos+iBufferSize;i++)	pReaction[i-pos].copy(&m_pReaction[i]);	//	임시 버퍼에 복사해 둔다.

	for (i=pos;i<pos+iCount		;i++)	m_pReaction[i].copy(&lpClipKarma->m_pReaction[i-pos]);

	pos	+=	iCount;

	for (i=pos;i<pos+iBufferSize;i++)	m_pReaction[i].copy(&pReaction[i-pos]);

	pKILL(pReaction);

	m_wReactionCount	+=	iCount;
}	//	CKarma::PasteReaction(int pos,CKarma *lpClipKarma)

//
//	트리거들을 붙여 넣는다.
void
CKarma::PasteTrigger(int pos,CKarma *lpClipKarma)
{
	int	i,iCount,iBufferSize;
	CKarmaItem	*pTrigger;								//	임시 버퍼

	iCount		=	lpClipKarma->m_wTriggerCount;	//	붙여넣을 반응의 수

	if (pos	==	0xffff)	//	맨 끝에 붙인다.
	{
		pos	=	m_wTriggerCount;

		for (i=pos;i<pos+iCount		;i++)	m_pTrigger[i].copy(&lpClipKarma->m_pTrigger[i-pos]);

		m_wTriggerCount	+=	iCount;

		return;
	}

	iBufferSize	=	m_wTriggerCount-pos;

	pTrigger	=	new CKarmaItem	[iBufferSize];			//	붙여 넣을 수 만큼 임시 버퍼를 만든다.

	for (i=pos;i<pos+iBufferSize;i++)	pTrigger[i-pos].copy(&m_pTrigger[i]);	//	임시 버퍼에 복사해 둔다.

	for (i=pos;i<pos+iCount		;i++)	m_pTrigger[i].copy(&lpClipKarma->m_pTrigger[i-pos]);

	pos	+=	iCount;

	for (i=pos;i<pos+iBufferSize;i++)	m_pTrigger[i].copy(&pTrigger[i-pos]);

	pKILL(pTrigger);

	m_wTriggerCount	+=	iCount;
}	//	CKarma::PasteTrigger(int pos,CKarma *lpClipKarma)


///////////////////////////////////////////////////////////////////////////////////////
//	정적 멤버들
///////////////////////////////////////////////////////////////////////////////////////

//
//	모든 카르마를 리셋한다.
//
void
CKarma::RESET()
{
	g_EditKarma.reset();
}	//	CKarma::RESET()


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
////	카르마 그룹
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

//
//	추가
int
CKarmaContainer::add(CKarma *_lpKarma)
{
	int	iSerial	=	m_wKarmaCount;

	m_pKarma[m_wKarmaCount].copy(_lpKarma);
	m_pKarma[m_wKarmaCount].m_wSerial	=	m_wKarmaCount;

	m_wKarmaCount++;

	return	iSerial;
}

//
//	카르마 정보 복사
void
CKarmaContainer::copy(CKarmaContainer *_lpKarma,BOOL _bIsForEdit)
{
	reset();

	if	(_bIsForEdit)
		allocBufferForEdit();

	if	(!_lpKarma)
		return;

	memcpy(this,_lpKarma,sizeof(CKarmaContainerBasicElement));

	setTitle(_lpKarma->m_pstrTitle);

	if (_lpKarma->m_wKarmaCount <= 0 ) return;

	if (_bIsForEdit	==	FALSE)	m_pKarma	=	new CKarma [_lpKarma->m_wKarmaCount];				//	선택 항목

	for(int i=0;i<_lpKarma->m_wKarmaCount;i++)
	{
		m_pKarma[i].copy(&_lpKarma->m_pKarma[i],_bIsForEdit);
	}
}

void
CKarmaContainer::allocBuffer(int _iSize)
{
	setTitle(" ");

	pKILL(m_pKarma);

	m_pKarma	=	new CKarma [_iSize];
}

//
//	편집을 위해 버퍼 할당
void
CKarmaContainer::allocBufferForEdit()
{
	pKILL(m_pKarma);

	m_pKarma	=	new CKarma [dKARMA_COUNT];

	for (int i=0;i<dKARMA_COUNT;i++)
	{
		m_pKarma[i].reset(dTRIGGER_COUNT,dREACTION_COUNT);
		m_pKarma[i].m_wTriggerCount	=	0;
		m_pKarma[i].m_wReactionCount=	0;
	}
}

void
CKarmaContainer::reset()
{
	m_wSerial		=	0xffff;
	m_wTitleLength	=	0;	//	대사 길이
	m_wKarmaCount	=	0;	//	이 대화가 처음에 시작될 조건 수

	pKILL(m_pstrTitle);	//	선택문
	pKILL(m_pKarma);				//	선택 항목
}

//
//	제거
BOOL
CKarmaContainer::remove(int _iIndex)
{
	if	(_iIndex	>=	m_wKarmaCount)
		return	FALSE;	//	이 대화가 처음에 시작될 조건 수

	m_pKarma[_iIndex].reset();

	for (int i=_iIndex;i<m_wKarmaCount-1;i++)
		m_pKarma[i].copy(&m_pKarma[i+1]);

	m_wKarmaCount--;

	return	FALSE;	
}