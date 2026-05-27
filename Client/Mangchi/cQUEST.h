/*************************************************************************************
  개			  요 :  붉은 보석 퀘스트 시스템... 이라기 보다는 그냥 퀘스트 설명해 놓
						은 데이터다. 퀘스트 시스템이야... 카르마들 조합이지 머.. --

  제한			사항 :	... 머 그런거 까지.. *^o^* .... 역시 미쳐가는 걸까.. ;;
						
  필요	  라이브러리 :	도서관은... 별루.... ... 왜 이러지.. --

  작성			날짜 :	2001.06.14

  기              타 : 토욜날 미팅이다. 쿠하하하!!!

*************************************************************************************/

#ifndef	_classQUEST_H
#define	_classQUEST_H

#include	"SFC.H"
#include	"playerJobCommon.H"
#include	"quest_common.H"

#define	dQUEST_CLIENT_POS_EXPLAIN_SIZE	64

class	CQuestItem20031021
{
public:
	WORD	m_wSerial;
	char	m_strName[dQUEST_NAME_SIZE];
	char	m_astrExplain[dQUEST_STATUS_COUNT][dQUEST_EXPLAIN_SIZE];

	WORD	m_wQuestType;	//	이벤트 퀘스트여?
	WORD	m_wBeginYear,m_wBeginMonth,m_wBeginDay;
	WORD	m_wExpireYear,m_wExpireMonth,m_wExpireDay;
};

class	CQuestItem20031113 : public CQuestItem20031021
{
public:
	char	m_strClientName[dNAME_LENGTH];
	char	m_strClientLocate[dQUEST_CLIENT_POS_EXPLAIN_SIZE];
};

class	CQuestItem20031113_2 : public CQuestItem20031113
{
public:
	char	m_strQuestValueComment[dQUEST_VALUE_COMMENT_SIZE];
};

class	CQuestItem20031115_Base
{
public:
	WORD	m_wSerial;
	char	m_strName[dQUEST_NAME_SIZE];
	char	m_astrExplain[dQUEST_STATUS_COUNT][dQUEST_EXPLAIN_SIZE];

	WORD	m_wQuestType;	//	이벤트 퀘스트여?
	WORD	m_wBeginYear,m_wBeginMonth,m_wBeginDay;
	WORD	m_wExpireYear,m_wExpireMonth,m_wExpireDay;

	char	m_strQuestClientInfo[dQUEST_CLIENT_INFO_SIZE];
	WORD	m_wQuestKind;
	char	m_strBeginNPCName[dNAME_LENGTH];
	WORD	m_wBeginNPC_Field;
	BYTE	m_aEnableJob[18];
	WORD	m_wMinProprietyLevel,m_wMaxProprietyLevel;
	WORD	m_wMinLimitLevel,m_wMaxLimitLevel;
	char	m_strQuestValueComment[dQUEST_VALUE_COMMENT_SIZE];
};

class	CQuestItem20031115 : public CQuestItem20031115_Base
{
public:
	WORD	m_isEnableCancel					: 1;
	WORD	m_isRemoveQuestItemByCancelQuest	: 1;
	WORD	m_isTestQuestInTestServer			: 1;
	WORD	m_dummy								: 13;
};

class	CQuestItem : public CQuestItem_common
{
public:

			CQuestItem()
			{
				reset();
			}

	inline	BOOL	isEnableJob(int _iJob)
	{
		UINT	uiCheckMask	=	1<<_iJob;

		if	(m_uiEnableJobMask & uiCheckMask)
			return	TRUE;

		return	FALSE;
	}

	void	setExplain(int _iIndex,char *_lpstrExplain)
			{
				ustrncpy(m_astrExplain[_iIndex],_lpstrExplain,dQUEST_EXPLAIN_SIZE-2);
			}

	void	copy(CQuestItem *_lpQuest)
			{
				WORD	wSerial	=	m_wSerial;

				memcpy(this,_lpQuest,sizeof(CQuestItem));

				m_wSerial	=	wSerial;
			}

	void	reset()
			{
				memset(this,0,sizeof(CQuestItem));
				m_wSerial			=	0xffff;
				m_wBeginNPC_Field	=	0xffff;

				for (int iJob=0;iJob<dPLAYER_JOB_COUNT;iJob++)
					m_uiEnableJobMask	+=	(1<<iJob);

				m_bf1IsMaleJobEquipAble			=	TRUE;	//	남성 직업 가능
				m_bf1IsFemaleJobEquipAble		=	TRUE;	//	여성 직업 가능

				strcpy(m_strName,"valid quest");
			}
};

class	CQuest
{
public:
	int			m_iCount;
	int			m_iSelectQuest;

	CQuestItem	m_aQuest[dQUEST_COUNT];

				CQuest()
				{	m_iCount		=	0;
					m_iSelectQuest	=	0;
				}


	void		reset();
	void		initEventQuest();

	int			getIndex(char *_lpstrName);
	char		*getName(int _iIndex);
	BOOL		add(CQuestItem *_lpQuest);
	BOOL		remove(int _iIndex,BOOL _bIsArbeit=FALSE);
	BOOL		remove(char *_lpstrName,BOOL _bIsArbeit=FALSE);
	CQuestItem*	getQuest(int _iIndex)
	{
		if (_iIndex < 0 || _iIndex >= m_iCount)	return	NULL;
		
		return &m_aQuest[_iIndex];
	}
	void		rebuild();
};

extern	CQuest			g_quest,g_questArbeit;

extern	char			*g_strQuestKind[];

#endif