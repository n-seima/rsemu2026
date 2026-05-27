#ifndef _ClassEVENT_H
#define _ClassEVENT_H

#include "SFC.H"
#include "CItem.H"
#include "definePacketData.H"
#include "eventDefine.h"

#define	dOX_QUIZ_SPARE_COUNT	3	//	3초정도 더 기다린다.

enum
{
	eSIM_SET,
	eSIM_PERCENTAGE,
};
enum
{// 마지막 수정일 : 09.10.12

	eEVENT_UNANIMOUS,		// 이벤트 이구동성
	eEVENT_CHRISTMAS,		// 이벤트 크리스마스.

};
#define	dMAX_SALE_ITEM_EVENT_ITEM_COUNT	100

class	cEventWord	:	public cEventWordDefine
{
public:
	BOOL			IsEventActive()	{return m_bIsActiveWordCompareEvent;}
	BOOL			IsSameWordCompareEventWord(char* _strWord);
	BOOL			CheckWordEventAppointment();
};

class	CSaleItemInfo
{
public:
	WORD	m_wItemIndex;
	DWORD	m_dwPrice;
	WORD	m_wMethod;
};

class	CItemSaleEvent
{
public:
	CTimeInfo			m_tiBeginTime,m_tiEndTime;
	int					m_iItemCount;
	BOOL				m_bIsActive;
	CSaleItemInfo		m_aItemInfo[dMAX_SALE_ITEM_EVENT_ITEM_COUNT];

	void				readData();
	void				setBeginTime(int _iYear,int _iMonth,int _iDay,int _iHour,int _iMinute);
	void				reset();
	void				addItem(int _iItem,int _iGold,int _iType);
	void				operateItemByItemSaleEvent(cItem *_lpItem);
	BOOL				isItemSaleTerm();			//	지금 아이템 세일 기간이냐?
	CSaleItemInfo*		getSaleItem(int _iItem);	//	판매 아이템 찾기
	int					getItemPriceByItemSaleEvent(cItem *_lpItem,DWORD _dwPrice);	//	아이템 가격 얻기
	void				update();	//	업데이트
};

class	CEventDropItemInfo
{
public:
	WORD	m_wItemIndex;
	int		m_iDropwChance;
};


#define	dMAX_DROP_ITEM_EVENT_ITEM_COUNT	100

class	CItemDropEvent
{
public:
	CTimeInfo			m_tiBeginTime,m_tiEndTime;
	int					m_iItemCount;
	BOOL				m_bIsActive;
	CEventDropItemInfo	m_aItemInfo[dMAX_DROP_ITEM_EVENT_ITEM_COUNT];

	void				readData();
	void				reset();
	void				addItem(int _iItem,int _iGold);
	BOOL				isActiveTerm();			//	지금 아이템 세일 기간이냐?
	void				update();	//	업데이트
	int					getDropItem();
};


class	CPlusDropGoldEvent
{
public:
	CTimeInfo			m_tiBeginTime,m_tiEndTime;
	BOOL				m_bIsActive;
	int					m_iGold;

	void				readData();
	void				reset();
	BOOL				isEventTerm();			//	지금 아이템 세일 기간이냐?
	int					getPlusGold();	//	
	void				update();	//	업데이트
	int					getDropItem();
};

class	COXQuiz
{
public:
	CTimeInfo		m_tiBeginTime;
	char			m_strQuestion[512];
	WORD			m_wQuizGateField;
	WORD			m_wLimitPlayerCount;
	WORD			m_wSolution,m_wCountDown,m_wIsWaitFinishEvent,m_wQuestionCounter,m_wIsTest,m_wStatus;
	DWORD			m_dwLastAnnounceTime,m_dwLastDecreaseCountTime,m_dwFinishEventTime;

	void			reset();
	void			update();
	void			setData(int _iYear,int _iMonth,int _iDay,int _iHour,int _iMinute,int _iGateVillage,int _iLimitPlayerCount,BOOL _bIsTest);
	void			setSolution(int _iSolution);
	void			setQuestion(char *_lpstrQuestion,int _iSolution,int _iCount);
	void			cancelOXQuiz();
	void			endEvent();
};

//
//	붉은 보석 포인트 순위
class	cRedStonePointRank
{
public:
	enum
	{
		eMaxRedStonePointRankCount	=	200,
	};
	class	cUser
	{
	public:
		WORD	m_wRank,m_wTitleLevel;
		DWORD	m_dwNameHashCode;
		char	m_strName[dNAME_LENGTH];
	};

	WORD		m_wCount;
	cUser		m_aList[eMaxRedStonePointRankCount];

	void		init();
	void		reset();

	void		setCount(int _iCount);

	int			getTitleLevel(char *_lpstrName);
	void		setRank(int _iIndex,int _iRank,char *_lpstrName);

protected:
	cUser*		_get(char *_lpstrName);
};

class	cWordQuiz
{
public:
	CTimeInfo		m_tiBeginTime;
	char			m_strQuestion[512];
	char			m_strSolution[32];
	WORD			m_wQuizGateField;
	WORD			m_wLimitPlayerCount;
	WORD			m_wCountDown,m_wIsWaitFinishEvent,m_wQuestionCounter,m_wIsTest,m_wStatus,m_wWinnerCount;
	DWORD			m_dwLastAnnounceTime,m_dwLastDecreaseCountTime,m_dwFinishEventTime;
	char			m_strWordEventName[128];

	void			reset();
	void			update();
	void			setData(int _iYear,int _iMonth,int _iDay,int _iHour,int _iMinute,int _iGateVillage,int _iLimitPlayerCount,BOOL _bIsTest, char* _strEventName);
	void			setSolution(char *_lpstrSolution);
	void			setQuestion(char *_lpstrQuestion,char *_lpstrSolution,int _iCount);
	void			cancelQuiz();
	void			endEvent();
	int				getCurrentScore();
	void			increaseCorrectAnswerCount();
};

class	cGoldRush
{
public:
	BOOL			m_bIsForcedSetting;
	UTime			m_begin,m_end;
	WORD			m_wBoostExperience,m_wCorrectItemDropChance;
	char			m_strBeginComment[256],m_strEndComment[256];

	WORD			m_wIsSendBeginNotice,m_wIsSendEndNotice;

	BOOL			init();
	void			update();
	void			updateInfo(UTime *_lpBegin,UTime *_lpEnd,int _iBoostExperience,int _iCorrectItemDropChance,char *_lpstrBeginComment,char *_lpstrEndComment);
};

class	CEventTimeManager
{
public:
	int					m_iCount;
	cEventTimeDefine	m_aEventTimeList[dMAX_EVENT_TIME_COUNT];
	DWORD				m_adwEventTimeActiveBuffer[dEVENT_TIME_BUFFER_SIZE];

	CEventTimeManager();

	void	updateData(int _iCount,cEventTimeDefine *_lpData);
	BOOL	isEventTime(int _iEventIndex);
	BOOL	update();
};

class	CLottoEventManager
{
public:
	int					m_iCount;
	cLottoItemDropEvent	m_aEventList[dLOTTO_EVENT_COUNT];

	CLottoEventManager();

	void	update();
	void	updateData(int _iCount,cLottoItemDropEvent *_lpData);
	void	receiveLottoBox(cACTOR *_lpPlayer,int _iPlayerLevel,int _iMonsterLevel);
};

class	CEventManager
{
public:
	BOOL				m_bIsReceiveGoldRushInfo;
	COXQuiz				m_oxQuiz;
	CItemSaleEvent		m_itemSale;
	CPlusDropGoldEvent	m_plusDropGold;
	CItemDropEvent		m_itemDrop;
	cGoldRush			m_goldRush;
	cWordQuiz			m_wordQuiz;
	cRedStonePointRank	m_redStonePointRank;
	int					m_aiDBControlledEventValue[c_iDBControlledEvnetCount];
	cEventWord			m_wordCompareEvent;

	char				m_strSoundOfLeadersBellOwner[dNAME_LENGTH];
	char				m_strSoundOfLeadersBellMessage[128];
	WORD				m_wIsDisplayNameWithSoundOfLeadersBell;
	int					m_iUpkeepSecondForSoundOfLeadersBell;

	BOOL				checkTwoMouthOneSoundEventTime(int _iPreMinute);
	BOOL				checkGoldRushEventTime(int _iPreMinute);

	inline BOOL			isWordCompareEventTime()			{return m_wordCompareEvent.IsEventActive();}
	inline BOOL			isEventWordCompare(char* _strWord)	{return !STRICMP(_strWord , m_wordCompareEvent.m_strEventWord);}

	void				init();
	void				update();
	void				readEventData();	//	이벤트 정보 읽어오기
	void				operateWordEventPlayer(cACTOR* _lpChatter);

	int					getRedDevilFactor();
	BOOL				christMasEvent(cACTOR* _lpChatter);		// 크리스마스 이벤트 09.10.12
	void				setDBControlledEventValue(int *_lpiValues);
	int					getDBControlledEventValue(int _iIndex)
	{
		return	m_aiDBControlledEventValue[_iIndex];
	}
	void				changeDBControlledEventValue(int _iEvent,int _iValue);
	int					setRedStonCount(int _iHeavenRedStoneCount,int _iHellRedStoneCount,int _iRedDevilRedStoneCount);

	inline	void		updateGoldRushInfo(UTime *_lpBegin,UTime *_lpEnd,int _iBoostExperience,int _iCorrectItemDropChance,char *_lpstrBeginComment,char *_lpstrEndComment)
	{
		m_bIsReceiveGoldRushInfo	=	TRUE;
		m_goldRush.updateInfo(_lpBegin,_lpEnd,_iBoostExperience,_iCorrectItemDropChance,_lpstrBeginComment,_lpstrEndComment);
	}

	inline	const	COXQuiz*	getOXQuiz()
	{
		return	&m_oxQuiz;
	}

	inline	cWordQuiz*	const	getWordQuiz()
	{
		return	&m_wordQuiz;
	}

	inline	cRedStonePointRank*	getRedStoneRankManager()
	{
		return	&m_redStonePointRank;
	}


	inline	int			getOXQuizStatus()
	{
		return	m_oxQuiz.m_wStatus;
	}
	inline	char*		getOXQuizQuestion()
	{
		return	m_oxQuiz.m_strQuestion;
	}
	inline	int			getOXQuizSolution()
	{
		return	m_oxQuiz.m_wSolution;
	}
	inline	int			getOXQuizCount()
	{
		return	m_oxQuiz.m_wCountDown;
	}
	inline	int			getOXQuizQuestionCounter()
	{
		return	m_oxQuiz.m_wQuestionCounter;
	}
	void				setOXQuizData(int _iYear,int _iMonth,int _iDay,int _iHour,int _iMinute,int _iGateVillage,int _iLimitPlayerCount,BOOL _bIsTest)
	{
		m_oxQuiz.setData(_iYear,_iMonth,_iDay,_iHour,_iMinute,_iGateVillage,_iLimitPlayerCount,_bIsTest);
	}

	void				setOXQuizSolution(int _iSolution)
	{
		m_oxQuiz.setSolution(_iSolution);
	}

	void				setOXQuizQuestion(char *_lpstrQuestion,int _iSolution,int _iCount)
	{
		m_oxQuiz.setQuestion(_lpstrQuestion,_iSolution,_iCount);
	}

	void				cancelOXQuiz()
	{
		m_oxQuiz.cancelOXQuiz();
	}

	void				endOXQuizEvent()
	{
		m_oxQuiz.endEvent();
	}


	void				operateItemByItemSaleEvent(cItem *_lpItem)
	{
		m_itemSale.operateItemByItemSaleEvent(_lpItem);
	}

	int					getItemPriceByItemSaleEvent(cItem *_lpItem,DWORD _dwPrice)
	{
		return	m_itemSale.getItemPriceByItemSaleEvent(_lpItem,_dwPrice);
	}

	int					getPlusDropGold()//	
	{
		return	m_plusDropGold.getPlusGold();
	}

	int					getEventDropItem()
	{
		return	m_itemDrop.getDropItem();
	}
};

extern	CEventManager		g_eventManager;
extern	CEventTimeManager	g_eventTimeManager;
extern	CLottoEventManager	g_lottoEventManager;

extern	int	g_iBoostExpByGoldRush,g_iCorrectItemDropChanceByGoldRush;
extern	int	g_iBoostExpByTreasureMapEventCount,g_iBoostGoldByTreasureMapEventCount;
extern	int	g_iBoostExpSecondByTreasureMapEventInServer,g_iBoostGoldSecondByTreasureMapEventInServer;

extern	int	g_iHeavenRedStoneCount,g_iHellRedStoneCount,g_iRedDevilRedStoneCount,g_iRedDevilFactor;

extern	int	g_iBoostExpByLeadersSoundofBell,g_iBoostItemDropChanceByLeadersSoundofBell;

extern	int		g_iCurrentSecondTime,g_iPasteSecondTime;

extern	BOOL	g_bIsCompleteReceiveLottoEventData,g_bIsCompleteReceiveEventTimeData;

#endif