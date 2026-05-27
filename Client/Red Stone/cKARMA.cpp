#include "cKARMA.H"

cKARMA_SET		g_ProjectKarma,g_LocalKarma;

void
cKARMA::Reset()
{
	m_iSerial				=	0xffff;						//	카르마 시리얼
	m_iExcuteCount			=	0;							//	몇번이나 실행하냐?
	m_isActive				=	0;							//	살아있냐?
	m_isInfinity			=	FALSE;						//	무한 반복
	m_isAndTrigger			=	TRUE;						//	조건이 엔드냐?
	m_isExclusiveReaction	=	FALSE;						//	단독 실행
	m_iReactionCount		=	0;							//	조건의 수
	m_iTriggerCount			=	0;							//	반응의 수

	_Free(m_pTrigger);
	_Free(m_pReaction);
}

void
cKARMA::MakeBuffer()
{
	m_pTrigger	=	_New(cKARMA_TRIGGER,m_iTriggerCount,NULL,"Trigger");
	m_pReaction	=	_New(cKARMA_REACTION,m_iReactionCount,NULL,"Reaction");
}

void
cKARMA_GROUP::Reset()
{
	m_iSerial		=	0xffff;					//	카르마 그룹 시리얼
	m_isEnable		=	FALSE;					//	활성화 상태냐?
	m_iKarmaCount	=	0;						//	카르마의 수

	for (int i=0;i<dKARMA_COUNT;i++)	m_aKarma[i].Reset();
}

void
cKARMA_SET::Reset()
{
	m_iGroupCount	=	0;

	for (int i=0;i<dKARMA_GROUP_COUNT;i++)	m_aKarmaGroup[i].Reset();
}
