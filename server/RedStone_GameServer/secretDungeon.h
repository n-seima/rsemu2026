#ifndef	_classSecretDungeon_h
#define	_classSecretDungeon_h

#include "SFC.h"
#include "secretDungeonDefine.h"

#define	dNAME_LENGTH								18
#define	dINSTANCE_FIELD_UPKEEPTIME_IN_VALID_PLAYER	3
#define	dMAX_IF_VALUE_COUNT							50
#ifdef	_FOR_JAPAN
#define	dMAX_IF_COUNT								512
#else
#define	dMAX_IF_COUNT								256
#endif
#define	dPARTY_MEMBER_COUNT							8
#define	dINSTANCE_FIELD_DEATH_COUNTER				30
#define	dINSTANCE_FIELD_COMPLETE_COUNTER			60
#define	dENTER_PLAY_IF_PLAYER_LIST_BUFFER_SIZE		32
#define dMAX_GUILD_DUNGEON_COUNT					30

#define	dINSTANCE_FIELD_WARNING_STEP_COUNT			4

enum
{
	eESDE_SUCCESS				,
	eESDE_INCORRECT_CLIENT		,
	eESDE_CLOSE_DUNGEON			,
	eESDE_TOO_MANY_PLAYER_COUNT	,
	eESDE_DATA_SAVE_FAILED		,
};

class	cSecretDungeon	:	public	cSecretDungeonDefine
{
public:
	void					reset();
	void					copy(cSecretDungeon *_lpDungeon);
};

class	cSecretDungeonManager
{
public:
	WORD						m_wGuildPointBattleSecretDungeon;
	WORD						m_wGuildDungeon[dMAX_GUILD_DUNGEON_COUNT];

	cDen<cSecretDungeon>		m_den;

								cSecretDungeonManager();
								~cSecretDungeonManager();

	inline	void				init(int _iCount)	{m_den.init(_iCount);}
	inline	void				reset()				{m_den.reset();}
	inline	int					getCount()			{return m_den.getCount();}
	inline	int					getMaxCount()		{return	m_den.getMaxCount();}
	inline	cSecretDungeon*		getBay()			{return	m_den.getList();}
	inline	cSecretDungeon*		get(int _iIndex)	{return	m_den.get(_iIndex);}
	void						rebuild();
};

class	cInstanceFieldMemberInfo
{
public:
	WORD	m_wX,m_wY;
	WORD	m_wFloor;
	char	m_strName[dNAME_LENGTH];
};

class	cFIELD;
class	cParty;
class	cACTOR;

class	cInstanceField
{
public:
	WORD						m_wSerial;
	WORD						m_wSecretDungeonSerial;
	WORD						m_wLinkField,m_wLinkGate;
	WORD						m_wLifeTime;
	WORD						m_wFloorCount;
	cFIELD						*m_pFloor;
	WORD						m_awDungeonValue[dMAX_IF_VALUE_COUNT];
	int							m_iRealPlayerCount,m_iAlivePlayerCount;
	WORD						m_wEnterPlayerCount;
	char						m_strEnterPlayerList[dENTER_PLAY_IF_PLAYER_LIST_BUFFER_SIZE][dNAME_LENGTH];

	WORD						m_wPartySerial;
	DWORD						m_dwPartyUniqueSerial;
	int							m_iTotalGuildHonorPoint,m_iGuildHonorPoint,m_iPeneltyGuildHonorPoint;	//	길드 포인트전 포인트
	WORD						m_wGenerateMinute;
	WORD						m_wUpkeepMinuteInValidPlayerStatus;
	WORD						m_wUpkeepMinute;
	WORD						m_wDestroyCounter;
	WORD						m_wIsReadyToClose;
	WORD						m_wIsProgressEvent;
	WORD						m_wCountdownTimer;

	WORD						m_wCompleteCountdown;
	WORD						m_wFocusFloor;
	cPOINT						m_posGroundFocus;
	WORD						m_wFocusActor;
	WORD						m_wCreateTimeValue;

	cPOINT						m_posStressMinimap;
	WORD						m_wStressMinimapFloor;
	BOOL						m_bIsGuildPointBattleField;
	BOOL						m_bISGuildDungeonFiled;

	BYTE						m_abIsAnnouncedWarning[dINSTANCE_FIELD_WARNING_STEP_COUNT];

								cInstanceField();
								~cInstanceField();

	BOOL						init(int _iDungeonSerial,int _iSerial,cParty *_lpParty,int _iLinkField,int _iLinkGate);
	void						copy(cInstanceField *_lpField);
	void						reset();
	void						close();
	void						addDungeonValue(int _iValueIndex,int _iValue);
	void						setDungeonValue(int _iValueIndex,int _iValue);
	int							getDungeonValue(int _iValueIndex);
	BOOL						update();
	void						banishAllPlayer();
	void						readyToClose();
	cParty*						getParty();
	int							getFloorIndex(char *_lpstrName);

	void						beginCountdownTimer(int _iTime);
	void						extensionCountdownTimer(int _iTime);
	void						reductionCountdownTimer(int _iTime);
	void						stopCountdownTimer();

	void						sendCountdownRemainTime();
	void						sendPacketToAllPlayer(char *_lpData,int _iFloor=-1);
	void						sendMessage(int _iMessage,int _iValue1=0,int _iValue2=0,int _iFloor=-1);
	void						sendMessageForChangeFloorActor(cACTOR *_lpActor);

	void						setEvent(BOOL _bIsEnd,BOOL _bIsDisableBattle);
	void						setGroundFocus(int _iX,int _iY,int _iFloor);
	void						setCharacterFocus(int _iActorSerial,int _iFloor);
	void						releaseEventFocus();

	void						stressMinimap(int _iX,int _iY,int _iFloor);
	void						releaseStressMinimap();
	void						complete(int _iMethod=0,int _iRemainTime=0 , BOOL _bISGuildDungeon = FALSE , BOOL _bISGuildPointBattle = FALSE);
	void						extendUpkeepTime(int _iMinute);
	void						dissolutionParty();
	BOOL						addEnterPlayerLog(char *_lpstrName,BOOL *_lpbIsFirst);
	void						removeEnterPlayerLog(char *_lpstrName);

	void						addGuildHonorPoint(int _iPoint);
};

enum
{
	eCSDR_FAILED,
	eCSDR_SUCCESS,
	eCSDR_FULL,
	eCSDR_ALREADY_USE_SECRET_DUNGEON,
	eCSDR_LIMIT_LEVEL,
};

class	cInstanceFieldManager
{
public:
	int						m_iCount;
	int						m_iRookie;
	int						m_iRealPlayerCount;
	
	cInstanceField			m_aInstanceField[dMAX_IF_COUNT];

							cInstanceFieldManager();
							~cInstanceFieldManager();

	cFIELD*					getFloor(int _iIndex,int _iFloor,DWORD	_dwPartyUniqueSerial=0xffffffff,int _iCreateTimeValue=-1);
	cInstanceField*			get(int _iIndex);
	cInstanceField*			getRookieField();
	cInstanceField*			getByUniquePartySerial(DWORD _dwUniqueSerial);
	void					update();
	int						create(int _iDungeonSerial,cParty *_lpParty,int _iLinkField,int _iLinkGate);
	void					close(int _iSerial);
	void					close();
	int						getAvailSecretDungeonCount(int _iDungeonSerial);
	BOOL					isFullSecretDungeon(int _iDungeonSerial,BOOL _bIsIncludeActiveKey=FALSE);
	cACTOR*					getPlayerByName(char *_lpstrName);
	cACTOR*					getPlayerByID(char *_lpstrID,DWORD _dwIDHashCode);

	void					updateGame();
	void					operateStoredPacket();
	void					operateOncePerSecondWork();
	void					operateOncePer10SecondWork();
	void					operateOncePerMinuteWork();
	void					operateOncePerHourWork();

	void					sendPacketToAllPlayer(char *_lpData);
	void					sendPacketToAllOperator(char *_lpData,int _iLevel);
	void					addSendEventMessage(char *_lpstrMessage,int _iType);
	void					resetReceivePacket();
	int						getRealPlayerCount();
	void					increasePlayerPlayTime();
};

extern	cSecretDungeonManager	*g_pSdManager;
extern	cInstanceFieldManager	*g_pIfManager;

#endif