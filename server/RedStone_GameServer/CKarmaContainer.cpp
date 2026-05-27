#include "CKarma.h"
#include "cGame.h"
#include "cField.h"

//
//	작동!!
void
CKarmaContainer::operate(cACTOR *_lpAsker,cACTOR *_lpOwner,cFIELD *_lpField,BOOL _bIsEnterField)
{
	if	(m_wKarmaCount	<=	0)
		return;

	if	(_lpField	==	NULL)
	{
		if	(_lpOwner)
			_lpField	=	_lpOwner->m_lpField;
		else
		if	(_lpOwner)
			_lpField	=	_lpOwner->m_lpField;
	}

	for (int i=0;i<m_wKarmaCount;i++)
	{
		BOOL	bIsStopKarma	=	FALSE;

		if	(g_bIsWantPrintTriggerCondition && m_pKarma[i].m_bf1IsForEnterFieldPlayer == FALSE)
		{
			if ( m_pKarma[i].m_pstrTitle)
				printf("    %.2d '%s' 카르마 처리 시작\n",i,m_pKarma[i].m_pstrTitle);
			else
				printf("    %.2d 카르마 처리 시작\n",i);
		}

		int	iJumpKarma	=	-1;

		m_pKarma[i].operate(_lpAsker,_lpOwner,&bIsStopKarma,_lpField,_bIsEnterField,&iJumpKarma,this);

		if	(iJumpKarma	!=	-1)
		{
			if	(iJumpKarma	>=	m_wKarmaCount)
			{
				if	(g_bIsWantPrintTriggerCondition)
					printf("카르마 %d번으로 점프하라고 했지만 카르마 수가 %d개다.",iJumpKarma,m_wKarmaCount);
			}

			i	=	iJumpKarma-1;
		}

		if	(bIsStopKarma)
			break;
	}
}

//
//	카르마 정보 복사
void
CKarmaContainer::copy(CKarmaContainer *_lpKarma)
{
	reset();

	if (!_lpKarma)
		return;

	memcpy(this,_lpKarma,sizeof(CKarmaContainerBasicElement));

	setTitle(_lpKarma->m_pstrTitle);

	if (_lpKarma->m_wKarmaCount <= 0 )
		return;

	m_pKarma	=	new CKarma [_lpKarma->m_wKarmaCount];				//	선택 항목

	for(int i=0;i<_lpKarma->m_wKarmaCount;i++)
		m_pKarma[i].copy(&_lpKarma->m_pKarma[i]);
}

void
CKarmaContainer::resetToFirstStatus()
{
	for(int i=0;i<m_wKarmaCount;i++)
		m_pKarma[i].resetToFirstStatus();
}
