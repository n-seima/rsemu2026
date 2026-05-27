#ifndef _classQUEST_H
#define	_classQUEST_H

#include "cNEMO.h"
#include "quest_common.h"

#pragma	pack(2)

const	int		c_iHeavyQuestDataSize	=	dQUEST_EXPLAIN_SIZE*dQUEST_STATUS_COUNT+dQUEST_NAME_SIZE+2;

class	CQuestItemSaveData	:	public CQuestItem_common
{
public:
};

class	CQuestItem
{
public:
	class	cTargetInfo
	{
		public:
			WORD	m_wField;
			WORD	m_wNpcSerial;
			char	*m_pStrName;

					cTargetInfo()
					{
						m_wField	=	0xffff;
						m_wNpcSerial=	0xffff;
						m_pStrName	=	NULL;
					}
					~cTargetInfo()
					{
						m_wField	=	0xffff;
						pKILL(m_pStrName);
					}

		void		setName(char *_lpstrName)
		{
			pKILL(m_pStrName);

			if	(_lpstrName == NULL)
				return;

			if	(IsNumberText(_lpstrName))
			{
				m_wNpcSerial	=	atoi(_lpstrName);

				return;
			}

			m_wNpcSerial	=	0xffff;

			int	iLength		=	strlen(_lpstrName)+1;
			m_pStrName		=	new char [iLength];

			strcpy(m_pStrName,_lpstrName);
		}

		void		copy(cTargetInfo *_lpData)
		{
			m_wField	=	_lpData->m_wField;
			setName(_lpData->m_pStrName);
		}
	};

	class	cQuestTextForDecode
	{
	public:
		WORD			m_wLinkFieldCount,m_wRequireItemCount;
		WORD			m_wLinkNpcCount,m_wTargetMonsterCount;

		WORD			m_awLinkField[256];
		WORD			m_awRequireItemList[256][2];

		cTargetInfo		m_aNpcList[256];
		cTargetInfo		m_aTargetMonsterList[256];

						cQuestTextForDecode()
						{
							memset(this,0,sizeof(cQuestTextForDecode)-sizeof(m_aNpcList)*2);
						}
						~cQuestTextForDecode(){};
	};

	class	cQuestInfo
	{
	public:
		WORD			m_wLinkFieldCount,m_wRequireItemCount;
		WORD			m_wLinkNpcCount,m_wTargetMonsterCount;
		WORD			m_wClientField;

		WORD			*m_pLinkField;
		WORD			*m_pRequireItemList;

		cTargetInfo		*m_pNpcList;
		cTargetInfo		*m_pTargetMonsterList;

		char			*m_pstrQuestValue;
		char			*m_pstrChecker;
		char			*m_pstrClientName;
		char			*m_pstrText;
		char			*m_pstrCountdown;

		WORD			m_wExplainLength;
		WORD			m_wClientNpcSerial;

		WORD			m_bf1IsClearClient	:	1;
		WORD			m_bf1IsClearValue	:	1;

						cQuestInfo()
						{
							reset();
						}

						~cQuestInfo()
						{
							pKILL(m_pLinkField);
							pKILL(m_pRequireItemList);

							pKILL(m_pstrClientName);
							pKILL(m_pstrQuestValue);
							pKILL(m_pstrCountdown);
							pKILL(m_pstrChecker);
							pKILL(m_pstrText);

							pKILL(m_pNpcList);
							pKILL(m_pTargetMonsterList);
						};

		void			reset()
		{
			memset(this,0,sizeof(cQuestInfo));

			m_wClientField		=	0xffff;
			m_wClientNpcSerial	=	0xffff;
			m_pLinkField		=	NULL;
			m_pRequireItemList	=	NULL;

			m_pNpcList			=	NULL;
			m_pTargetMonsterList=	NULL;
			m_pstrClientName	=	NULL;

			m_pstrQuestValue	=	NULL;
			m_pstrCountdown		=	NULL;
			m_pstrChecker		=	NULL;

			m_pstrText			=	NULL;
		}
		void			setText(char *_lpstrText)
		{
			pKILL(m_pstrText);

			m_wExplainLength	=	0;

			if (_lpstrText == NULL)
				return;

			m_wExplainLength	=	strlen(_lpstrText)+1;
			m_pstrText			=	new char [m_wExplainLength];

			strcpy(m_pstrText,_lpstrText);
		}

		void			setCountdownComment(char *_lpstrText)
		{
			pKILL(m_pstrCountdown);

			if (_lpstrText == NULL)
				return;

			int	iLength			=	strlen(_lpstrText)+1;
			m_pstrCountdown		=	new char [iLength];

			strcpy(m_pstrCountdown,_lpstrText);
		}

		void			setValueComment(char *_lpstrText)
		{
			pKILL(m_pstrQuestValue);

			if (_lpstrText == NULL)
				return;

			int	iLength			=	strlen(_lpstrText)+1;
			m_pstrQuestValue	=	new char [iLength];

			strcpy(m_pstrQuestValue,_lpstrText);
		}
		void			setCheckerComment(char *_lpstrText)
		{
			pKILL(m_pstrChecker);

			if (_lpstrText == NULL)
				return;

			int	iLength			=	strlen(_lpstrText)+1;
			m_pstrChecker		=	new char [iLength];

			strcpy(m_pstrChecker,_lpstrText);
		}
		void			setClientName(char *_lpstrText)
		{
			pKILL(m_pstrClientName);

			if (_lpstrText == NULL)
				return;

			if	(IsNumberText(_lpstrText))
				m_wClientNpcSerial	=	atoi(_lpstrText);
			else
			{
				m_wClientNpcSerial	=	0xffff;
				int	iLength			=	strlen(_lpstrText)+1;
				m_pstrClientName	=	new char [iLength];

				strcpy(m_pstrClientName,_lpstrText);
			}
		}
		
		void			copy(cQuestTextForDecode *_lpQuestText);
	};

	cQuestInfo*			getInfo(int _iLevel)
	{
		return	&m_aInfo[_iLevel];
	}

	WORD				m_wSerial;
	char				m_strName[dQUEST_NAME_SIZE];
	cQuestInfo			m_aInfo[dQUEST_STATUS_COUNT];

	WORD				m_wQuestType;	//	이벤트 퀘스트여?
	WORD				m_wBeginYear,m_wBeginMonth,m_wBeginDay;
	WORD				m_wExpireYear,m_wExpireMonth,m_wExpireDay;

	char				m_strQuestClientInfo[dQUEST_CLIENT_INFO_SIZE];
	WORD				m_wQuestKind;
	char				m_strBeginNPCName[dNAME_LENGTH];
	WORD				m_wBeginNPC_Field;

	WORD				m_wMinProprietyLevel,m_wMaxProprietyLevel;
	WORD				m_wMinLimitLevel,m_wMaxLimitLevel;
	char				m_strQuestValueComment[dQUEST_VALUE_COMMENT_SIZE];

	UINT				m_uiEnableJobMask;

	WORD				m_bf1IsMaleJobEquipAble				:	1;	//	남성 직업 가능
	WORD				m_bf1IsFemaleJobEquipAble			:	1;	//	여성 직업 가능
	WORD				m_isEnableCancel					:	1;
	WORD				m_isRemoveQuestItemByCancelQuest	:	1;
	WORD				m_isTestQuestInTestServer			:	1;
	WORD				m_bf1IsDisableQuest					:	1;	// 6 전체목록에서 표시안함
	WORD				m_bf6ChapterNumber					:   6;  // 12 챕터번호 // c_iMaxChapterCount
	WORD				: 0;

	WORD				m_bf6PartNumber						:   6; //  6 파트번호 // c_iMaxHighQuestCount
	WORD				m_bf3Quarter						:	3; //  9 분기퀘스트
	WORD				: 0;

	WORD				m_wBeginNpcSerial;
	

	char*				getValueString(int _iStatus);
	char*				getClientName(int _iStatus);
	int					getClientNpcSerial(int _iStatus);
	int					getClientField(int _iStatus);
	BOOL				isNpcAboutProcessQuest(char *_lpstrName,int _iProcessLevel,int _iField,int _iNpcActorSerial);
	BOOL				isMonsterAboutProcessQuest(char *_lpstrName,int _iProcessLevel,int _iField,int _iNpcActorSerial);

	char*				getExplain(int _iStatus,BOOL _bIsChecker);
	BOOL				setExplain(int _iStatus,char *_lpstrText);
	BOOL				isEnableJob(int _iJob);
	char*				getQuestName(){return m_strName;}
};

#pragma	pack()

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
						~CQuest();

	void				init(int _iCount);
	inline	int			GetQuestCount() {return m_iCount;}
	inline	CQuestItem*	getQuest(int _iQeustIndex)
	{
		return	&m_pQuest[_iQeustIndex];
	}
	CQuestItem*			getQuestByName(char *_lpstrName);
	inline	char*		getName(int _iQeustIndex)
	{
		return	m_pQuest[_iQeustIndex].m_strName;
	}

	inline	char*		getExplain(int _iQeustIndex,int _iStatus,BOOL _bIsChecker)
	{
		return	m_pQuest[_iQeustIndex].getExplain(_iStatus,_bIsChecker);
	}

	inline	void		setExplain(int _iQeustIndex,int _iStatus,char *_lpstrText)
	{
		m_pQuest[_iQeustIndex].setExplain(_iStatus,_lpstrText);
	}

	void				setQuestInfo(int _iQeustIndex,CQuestItemSaveData *_lpQuest);

	CQuestItem*			getProprietyQuest(int _iLevel,int _iField,int _iJob,int *_lpiLastProprietyQuestIndex,char *_lpstrNpcName,int _iNpcActorSerial);

	static	CQuestItem*	GetQuest(int _iQuestType,int _iQuestIndex);

};

extern	CQuest	g_quest,g_questArbeit;

#endif

