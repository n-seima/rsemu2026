#ifndef _classQUEST_
#define	_classQUEST_

#include "SFC.H"
#include "quest_common.H"

class	cACTOR;

class	CQuestItem : public CQuestItem_common
{
public:
			CQuestItem()
			{
				reset();
			}
	void	reset()
			{
				m_wSerial	=	0xffff;
			}
};

class	CQuestSimpleInfo
{
public:
	WORD	m_wQuestType,m_wQuestKind;	//	이벤트 퀘스트여?
	WORD	m_wBeginYear,m_wBeginMonth,m_wBeginDay;
	WORD	m_wExpireYear,m_wExpireMonth,m_wExpireDay;

	UINT	m_uiEnableJobMask;

	WORD	m_wMinLimitLevel,m_wMaxLimitLevel;

	WORD	m_isEnableCancel					: 1;
	WORD	m_isRemoveQuestItemByCancelQuest	: 1;
	WORD	m_isTestQuestInTestServer			: 1;
	WORD	m_isCloseQuest						: 1;
	WORD	m_bf1IsMaleJobEquipAble				: 1;	//	남성 직업
	WORD	m_bf1IsFemaleJobEquipAble			: 1;	//	여성 직업
	WORD	: 0;
	
	BOOL	isProcessAblePlayer(cACTOR *_lpPlayer);

	inline	BOOL	isEnableJob(int _iJob)
	{
		UINT	uiCheckMask	=	1<<_iJob;

		if	(m_uiEnableJobMask & uiCheckMask)
			return	TRUE;

		return	FALSE;
	}
};

class	CQuestManager
{
public:
	int					m_iCount;

	CQuestSimpleInfo	m_aQuest[dQUEST_COUNT];

	inline	CQuestSimpleInfo*	getQuest(int _iIndex)
	{
		if	(_iIndex < 0 || _iIndex >= dQUEST_COUNT)
			return	NULL;

		return	&m_aQuest[_iIndex];
	}

	BOOL				setQuestInfo(int _iIndex,CQuestItem *_lpQuest);
	void				checkCloseQuest(int _iCorrectSerial);
};

extern	CQuestManager	g_quest,g_questArbeit;

#endif
