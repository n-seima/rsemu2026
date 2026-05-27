#ifndef _classQUEST_H
#define	_classQUEST_H

#include "SFC.h"
#include "quest_common.h"

#define	dQUEST_NAME_SIZE				64
#define	dQUEST_EXPLAIN_SIZE				2048
#define	dQUEST_STATUS_COUNT				6

#define	dQUEST_BUFFER_SIZE				20
#define	dMAX_PROCESS_QUEST_COUNT		6


class	CQuestItemSaveData : public CQuestItem_common
{
public:
};

class	CQuestItem
{
public:
	class	CQuestText
	{
	public:
		char	*m_pstrText;
		WORD	m_wExplainLength;

				CQuestText()
				{
					m_pstrText			=	NULL;
					m_wExplainLength	=	0;
				}
				~CQuestText()
				{
					pKILL(m_pstrText);
				}

		void	setText(char *_lpstrText)
		{
			pKILL(m_pstrText);

			m_wExplainLength	=	0;
			if (_lpstrText == NULL) return;

			m_wExplainLength	=	strlen(_lpstrText)+1;
			m_pstrText			=	new char [m_wExplainLength];

			strcpy(m_pstrText,_lpstrText);
		}
	};

	WORD		m_wSerial;
	char		m_strName[dQUEST_NAME_SIZE];
	CQuestText	m_pExplain[dQUEST_STATUS_COUNT];

	char		m_strQuestClientInfo[dQUEST_CLIENT_INFO_SIZE];
	char		m_strQuestValueComment[dQUEST_VALUE_COMMENT_SIZE];

	WORD		m_isEnableCancel					: 1;
	WORD		m_isRemoveQuestItemByCancelQuest	: 1;
	WORD		m_dummy								: 14;

	char*		getExplain(int _iStatus)
	{
		return	m_pExplain[_iStatus].m_pstrText;
	}

	void		setExplain(int _iStatus,char *_lpstrText)
	{
		m_pExplain[_iStatus].setText(_lpstrText);
	}
};

class	CQuest
{
public:
	int					m_iCount;
	CQuestItem			*m_pQuest;

						CQuest()
						{
							m_iCount	=	0;
							m_pQuest	=	NULL;
						}
						~CQuest()
						{
							pKILL(m_pQuest);
						}

	static	BOOL		Load();
	void				init(int _iCount);

	inline	CQuestItem*	getQuest(int _iQeustIndex)
	{
		if	(_iQeustIndex	<	0	||	_iQeustIndex	>=	m_iCount)
			return	NULL;

		return	&m_pQuest[_iQeustIndex];
	}
	CQuestItem*			getQuestByName(char *_lpstrName);
	inline	char*		getName(int _iQeustIndex)
	{
		return	m_pQuest[_iQeustIndex].m_strName;
	}

	inline	char*		getExplain(int _iQeustIndex,int _iStatus)
	{
		return	m_pQuest[_iQeustIndex].getExplain(_iStatus);
	}

	inline	void		setExplain(int _iQeustIndex,int _iStatus,char *_lpstrText)
	{
		m_pQuest[_iQeustIndex].setExplain(_iStatus,_lpstrText);
	}

	void				setQuestInfo(int _iQeustIndex,CQuestItemSaveData *_lpQuest);
};

extern	CQuest		g_quest,g_questArbeit;
extern	CQuestItem*	GetQuest(int _iKind,int _iIndex);

#endif