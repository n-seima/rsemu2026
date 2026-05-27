#ifndef _classGAME_PLAY_H_
#define	_classGAME_PLAY_H_

#include "CGame.H"
#include "CAgent.H"
#include "CDialog.h"
#include "CShop.h"
#include "CParty.h"
#include "CTrade.h"
#include "CShake.h"
#include "effectManager.H"

extern	int		g_iFieldScreenHeight;

#define	dPREPARATION_FOR_GAME_STATUS_INIT							0
#define	dPREPARATION_FOR_GAME_STATUS_DATA_LOADING					1
#define	dPREPARATION_FOR_GAME_STATUS_WAIT_JOIN_RESULT				2
#define	dPREPARATION_FOR_GAME_STATUS_GUILD_DATA_LOADING				3
#define	dPREPARATION_FOR_GAME_STATUS_GUILD_DATA_LOADING_WAIT		4
#define	dPREPARATION_FOR_GAME_FINISH								5			

#define	dDATA_LOADING_STATUS_PROCESS					0
#define	dDATA_LOADING_STATUS_COMPLETE					1
#define	dDATA_LOADING_STATUS_FAILED						2

#define	dCHAT_BOX_MAX_SCALE								7
#define	dMAX_KARMA_DRAW_IMAGE					10
//
//	Chat Message Filter
enum
{
	eCMF_NORMAL_CHAT,
	eCMF_PARTY_CHAT,
	eCMF_GUILD_CHAT,
	eCMF_WHISPER,
};

enum
{
	eMMOPTION_QUEST_NPC,	
	eMMOPTION_QUEST_FIELD,
	eMMOPTION_QUEST_MONSTER,
};

enum
{
	eSM_CATEGORY_SCREEN,
	eSM_CATEGORY_SOUND,
	eSM_CATEGORY_COMMUNITY,
};

enum	
{	// 카르마 이미지 출력 그리기 방법
	eDM_NONE,					//	없음
	eDM_LOOP_UP_AND_DOWN,		//	상하 반복
	eDM_LOOP_LEFT_AND_RIGHT,	//	좌우 반복
};


//
//	Chat Packet Type
enum
{
	eCPT_NORMAL,
	eCPT_EVENT_MESSAGE,
	eCPT_SAY_RESULT,
};

//
//	Popup Interface Window
enum
{
	ePIW_NONE					,
	ePIW_INVENTORY				,
	ePIW_STATUS					,
	ePIW_SKILL					,
	ePIW_QUEST					,
	ePIW_PARTY					,
	ePIW_GUILD					,
	ePIW_FRIEND					,
	ePIW_SYSTEM					,
	ePIW_INVENTORY_AND_STATUS	,
	ePIW_SHOP					,
	ePIW_PITCHMAN_SHOP			,
	ePIW_REPAIR_ITEM			,
	ePIW_SELECT_VILLAGE			,
	ePIW_TRADE_BOX				,
	ePIW_BANK					,
	ePIW_READY_TO_MAKE_GUILD	,
	ePIW_SHOPPING_CART			,
	ePIW_GUILD_BATTLE_SCHEDULER	,
	ePIW_GUILD_HALL_LIST		,
	ePIW_GUILD_HONOR_POINT_LIST		,
	ePIW_GUILD_RELATED_PLACE_LIST	,
	ePIW_GUILD_FEED_PET_WINDOW	,
	ePIW_GUILD_SCRAMBLE_SCHEDULER	,
	ePIW_GUILD_MARK_COMPOSER	,
	ePIW_GUILD_INVENTORY		,
	ePIW_DEBUG					,
	ePIW_WORLD_MAP				,
	ePIW_HELP_SCREEN			,
	ePIW_CARROT_SHOP			,
	ePIW_SELECT_ITEM_IN_PACK	,
	ePIW_REMOVE_PREFIX			,
	ePIW_MERGE_ITEM				,
	ePIW_JUDGE_TO_ITEM			,
	ePIW_PUT_BANNER				,
	ePIW_BOTTOMLESS_BOX			,
	ePIW_MAIN_QUEST_EVENT		,
	ePIW_BOOK					,
	ePIW_BOOK_MONSTER_DICTIONARY	,
	ePIW_COMMON_LIST			,
	ePIW_COMMON_UI				,
	ePIW_UPGRADE_SHOP,				// 강화창  09.08.25
	ePIW_COPY_OR_CUT_PREFIX			,	// 접두사 복사창 
};

#define	dMAX_NOTICE_MESSAGE_SIZE						256

enum
{
eDROPPING_ITEM_AREA_NONE,
eDROPPING_ITEM_AREA_FIELD,
eDROPPING_ITEM_AREA_EQUIPMENT_FRAME,
eDROPPING_ITEM_AREA_SHOP,
eDROPPING_ITEM_AREA_INVENTORY,
eDROPPING_ITEM_AREA_BELT,
eDROPPING_ITEM_AREA_TRADE_BOX,
eDROPPING_ITEM_AREA_BANK,
eDROPPING_ITEM_AREA_GUILD_INVENTORY,
eDROPPING_ITEM_AREA_INVENTORY_WHEN_OPEN_GUILD_INVENTORY,
eDROPPING_ITEM_AREA_INVENTORY_WHEN_BANK_TRANSACTION,
eDROPPING_ITEM_AREA_INVENTORY_WHEN_CART_TRANSACTION,
eDROPPING_ITEM_AREA_PITCHMAN_SHOP,
eDROPPING_ITEM_AREA_EXTRA_INVENTORY,
eDROPPING_ITEM_AREA_BOTTOMLESS_BOX,
eDROPPING_ITEM_AREA_FEED_PET,
eDROPPING_ITEM_AREA_MINIPET,
eDROPPING_ITEM_AREA_UPGRADE_SHOP,		// 강화창에 드래그 했따~!09.09.01
eDROPPING_ITEM_AREA_SKILL,			// 10.01.07 추가  스킬창
};

//
//	스킬 슬롯
enum
{
eSKILL_SLOT_LEFT,
eSKILL_SLOT_RIGHT,
eSKILL_SLOT_SUB,
eSKILL_SLOT_SUPPORT_MAGIC,
eSKILL_SLOT_QUICK1,
eSKILL_SLOT_QUICK2,
eSKILL_SLOT_QUICK3,
eSKILL_SLOT_QUICK4,
eSKILL_SLOT_QUICK5,
eSKILL_SLOT_QUICK6,
eSKILL_SLOT_QUICK7,
eSKILL_SLOT_QUICK8,
eSKILL_SLOT_QUICK9,
eSKILL_SLOT_QUICK10
};

enum
{
	eFIELD_TITLE_APPEARENCE,
	eFIELD_TITLE_STAY,
	eFIELD_TITLE_DISAPPEAR,
	eFIELD_TITLE_SLEEP,
};

enum			//JBC CGamePlayMinimap.cpp 에서 월드맵 단축키를 위해 이동 시킴 08-07-03
{
	eMMTB_ZOOM_PLUS,
	eMMTB_ZOOM_MINUS,
	eMMTB_TOGGLE,
	eMMTB_QUESTION_MARK,
	eMMTB_WORLD_MAP,
	eMMTB_ZOOM_MIN,			// 09.06.24 미니맵최소 
	eMMTB_ZOOM_MAX,			// 09.06.24 미니맵최대
// insu add
	eMMTB_QUESTLIST,
	eMMTB_FOGMAP,
	eMMTB_OPTION,	// insu add
// insu add end
};

class CProposeWorkFieldMask
{
public:
	DWORD	isProposeToParty			: 1;
	DWORD	isProposeToTrade			: 1;
	DWORD	isReadyToAddFriend			: 1;
	DWORD	isReadyUseItemToItem		: 1;
	DWORD	isReadyUseItemToCharacter	: 1;
	DWORD	isReadyAskJoinGuild			: 1;
	DWORD	isReadySelectControlMonster	: 1;
	DWORD	isReadyUseItemToCorpse		: 1;
	DWORD	isProposeToUnionParty		: 1;
	DWORD	isReadyUseItemToEnemy		: 1;
};

class	cKarmaDrawImage
{
public:
	WORD	m_wIndex;
	WORD	m_wTime;
	CPos	m_pos;
	CPos	m_posCurrent;
	WORD	m_wDrawingMethod;
	WORD	m_wDistance;
	WORD	m_wFrameCounter;

	cKarmaDrawImage() : 
	m_wIndex			(0xffff),
	m_wTime				(0xffff),
	m_wDrawingMethod	(eDM_NONE),
	m_wDistance			(0),
	m_wFrameCounter		(0)
	{
		m_pos.Set(0,0);
		m_posCurrent.Set(0,0);
	}

	virtual ~cKarmaDrawImage(){}

	BOOL	reset()
	{
		m_wIndex			=	0xffff;
		m_wTime				=	0xffff;
		m_pos.Set( 0,0 );
		m_posCurrent.Set( 0,0 );
		m_wDrawingMethod	=	eDM_NONE;
		m_wDistance			=	0;
		m_wFrameCounter		=	0;
		return TRUE;
	}

};

class	cEventActiveManager
{
public:
	DWORD			m_adwEventActiveBuffer[dEVENT_TIME_BUFFER_SIZE];
	int				m_iCount;

	void	reset()
	{
		memset(m_adwEventActiveBuffer,0,sizeof(m_adwEventActiveBuffer));
		m_iCount	=	0;
	}
};


typedef	union
{
	CProposeWorkFieldMask	m_work;
	DWORD					m_dwValue;
} tuPropose;

typedef struct _SKILLITEM {
	WORD	IconShape;
	BOOL	NotUseItem;
	BOOL	DxItem;
	BOOL	UmItem;
} SKILLITEM, *LPSKILLITEM;


class	CFieldTitle	:	public CGame
{
public:
	char			m_strTitle[64];
	int				m_iState,m_iStayTime;
	int				m_iBarAlphaValue;
	int				m_iPutPosY;				// 09.09.02  y축..
	CAccelValue		m_avPosX;

					CFieldTitle()
					{
						m_iState	=	eFIELD_TITLE_SLEEP;
					}

	void			init(char *_lpstrTitle, int _iY = 5);// 09.09.02  y축..
	void			draw();
	void			update();
};

////////////////////////////////////////////////////////////////////////
//	게임 로고
////////////////////////////////////////////////////////////////////////
class	CGamePlay		:	public CGame
{
	static	BOOL			_CheckMessage(char *_lpstrSrcText,char *_lpstrResultText);
public:
	static	CFieldTitle		s_fieldTitle;
	static	CFieldTitle		s_UpgradeFieldTitle;				// 09.09.02 강화 타이틀보이기.!
	static	CShop			s_shop;

	static	BOOL			s_bIsFullScreenInterface,s_bIsOpenEventWindow;

	static	BOOL			s_bIsSecretDungeon;
	static	BOOL			s_bIsMoveFieldAtSameServer;	//	같은 서버에서 이동하는거다.
	static	BOOL			s_bIsFailedToJoinGame;
	static	BOOL			s_bIsMoveField,s_bIsProgressEvent;
	static	int				s_iLoadingImage;
	static	BOOL			s_bIsIgnoreAllFocus;
	static	BOOL			s_bIsBookedDisableChattingBox;

	static	DWORD			s_dwReceiveServerTime;
	static	CTimeInfo		s_currentTime,s_serverTime;

	static	tuPropose		s_propose;
	static	int				s_iBookedChangeGuildTax;
	static	char			s_strBookedUserName[dNAME_LENGTH];
	static	int				s_iPlayerMovePacketPeriod;
  	static	SKILLITEM		m_sQuickSlotItem[10][10][2];
	static	DWORD			s_dwRank12AgeTime;		// 12세등급아이콘 출력시간.
	static	DWORD			s_dwLoginTime;			// 게임로그인시간.
	static	cKarmaDrawImage		m_karmaDrawImage[dMAX_KARMA_DRAW_IMAGE];		// 카르마로 요청온 이미지 

	static	cEventActiveManager		s_eventActiveManager;
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃게임 초기화
//┃CGamePlay.cpp
	static	cTHREAD			s_thLoading;//로딩 쓰레드
	static	int				s_iPreparationsForGameStatus,s_iDataLoadingStatus;
	static	BOOL			s_bIsChattingCompositionState;

	static	BOOL			Run();
	static	BOOL			Draw();

	static	BOOL			UpdateInit();
	static	BOOL			UpdateReady();
	static	BOOL			UpdateFinish();
	static	BOOL			UpdateClose();

	static	void			DrawInit();
	static	void			DrawReady();
	static	void			DrawFinish();
	static	void			DrawClose();

	static	void			InitGamePlay();
	static	void			BookingMoveField(char *_strIP,char *_strMapName);//필드 이동
	static	void			MoveField();//필드 이동
	static	void			LoadData(void* _lpData);//데이터 로딩
	static	BOOL			LoadField(char *_lpstrMapName);//필드 로딩
	static	void			FailedToJoinGame(char *_lpstrReason);	//	게임 참가 실패

	static	BOOL			IsOpenedEventWindow();
//┃CGamePlay.cpp
//┃게임 초기화
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃인터페이스
//┃CGamePlyUtility.cpp
	static	char				s_strCurrentPlayBgm[256];
	static	int					s_iPlayBgmIndex;
	static	int					s_iAskItemPackNameCount;
	static	WORD				s_awAskItemPackNameList[32];
	static	DWORD				s_dwLastItemPackNameAskTime;
	static	DWORD				s_dwResultPopupwindow;//JBC 월드맵 단축키 사용을 위해 선언한 변수. 08-07-03

	static	WORD				s_wAskArcaInfoCount,s_awAskArcaInfoList[32];
	static	DWORD				s_dwLastArcaInfoAskTime;

	static	BOOL				PlayBgm(char *_lpstrFileName=NULL);
	static	void				PlayBgm(int _iIndex,BOOL _bIsMapSerial=TRUE);
	static	char*				GetMoneyString(LONGLONG _llMoney);
	static	void				ChangeCursorShape(HCURSOR _hCursor);
	static	void				AddAskItemPackName(int _iIndex);
	static	void				SendAskItemPackName();
	static	void				AddAskArcaInfo(int _iIndex);
	static	void				SendAskArcaInfo();
//┃CGamePlyUtility.cpp
//┃게임 초기화
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃인터페이스
//┃gpWarning.cpp
	static	char				s_strWarningMessage[256];
	static	DWORD				s_dwBeginWarningMessageTime;
	
	static	void				WarningMessage(char *_lpstrMessage);
	static	void				DrawWarningMessage();
	static	BOOL				CheckBCS(BOOL _bIsRequireWarning=FALSE);
//┃gpWarning.cpp
//┃인터페이스
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃인터페이스
//┃CGameInterface.cpp
	static	cRECT				s_rectInterfaceSize;	// insu add .. 10.9.15
	static	CTextList			s_tlSystem;	//	시스템 메시지

	static	cBARMENU			s_bmToolButton,s_bmBottomMenu,s_bmRadioBox,s_bmButton,s_bmButtonI2 ,s_bmPlusState;//하단 메뉴
	static	cBARMENU			s_bmCategory;	// 시스템메뉴 카테고리버튼(화면 사운드 커뮤니티)
	static	int					s_iCategoryIndex;
	static	cBARMENU			s_bmMiddleWindowButton;//중간 윈도우 버튼

	static	int					s_iBloodStoneWidth;		//체력 보석의 가로 사이즈
	static	int					s_iPopupInterface;		//현재 팝업 되어 있는 인터페이스

	static	cRECT				s_rectTooltip;			//	툴팁 영역

	static	int					s_iDragEquipment;	//	드래그 중인 장비
	static	int					s_iDragItem;		//	드래그 중인 아이템
	static	int					s_iDragSkill;		//	드래그 중인 기술
	static	CImageScrollBar		s_sbPopupWindow;
	static	CImageScrollBar		s_sbMiddleWindow;
	static	BOOL				s_bDragSkillItem;		// 10.01.07  스킬창에서 드래그한 아이템
	static	BOOL				s_bIsCanNotControlInterface;

	static	BOOL				ClosePopupWindow();	//	팝업 윈도우 닫기
	static	void				InitPopupWindowCloseButton();
	static	void				UpdatePopupWindowCloseButton();
	static	void				DrawPopupWindowCloseButton();

	static	void				LoadConfigData();
	static	void				SaveConfigData();
	static	void				openInterface(int iInterface, BOOL _bIsClose, BOOL _bIsCanNotControl);
//┃CGameInterface.cpp
//┃인터페이스
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃메인 플레이
//┃cGAME_PLAY_MAIN.cpp
	static	void				Init();				//	초기화
	static	void				Reset();			//	리셋
	static	void				UpdateDebugMenu();


	static	void				OperateMessageBox();

	static	inline	void		OperateProposeWorks();//	프로포즈 관련 작업 처리
	static	void				OperateSystemMessageAboutHero();
	static	void				OperateEventWork();
	static	void				HideTooltip();
	static	BOOL				CheckIncorrectStatus();
	static	void				OperateMenuWindow();

	static	inline	void		OperatePacket();	//	패킷 처리
	static	inline	void		OperateKeyboard();	//	키보드 처리
	static	inline	void		OperatePopupMenu();	//	팝업 메뉴 처리
	static	inline	void		UpdateCursorShape();

	static	BOOL				UpdateMain();
	static	void				ExitGame(int _iType);	
	static	void				ExitGame();
	static	BOOL				addKarmaDrawImage(WORD	_wIndex,  int _iX, int _iY, WORD	_wTime, WORD _wDrawingMethod, WORD _wDistance);
	static	BOOL				updateKarmaDrawImage();
	static	BOOL				offKarmaDrawImage(WORD	_wIndex,  int _iX, int _iY);
	static	BOOL				isExistKarmaDrawImage();
	static	BOOL				resetKarmaDrawImage();
	static	void				changeEventObject(BOOL	_bIsEvent);
	static	void				operateMoveByKey();
//┃cGAME_PLAY_MAIN.cpp
//┃메인 플레이
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃채팅 메시지 처리
//┃CGamePlayChat.cpp
	static	char				s_strLastSendSayId[dNAME_LENGTH];	//	마지막으로 내가 보낸 사람
	static	char				s_strLastReceiveSayId[dNAME_LENGTH];//	마지막으로 받은 메시지
	static	CTextList			s_tlChat,s_tlRecord;//	채팅
	static	CTextList			s_tlInputMessage;	//	입력한 채팅 메시지
	static	char				s_strPreviousChatMessage[dCHAT_LENGTH];//채팅 메시지
	static	char				s_strChatMessage[dCHAT_LENGTH];//채팅 메시지

	static	cTALK				s_inputChatting;
	static	cRECT				s_rectChatBox;
	static	CImageScrollBar		s_sbChatBox;
	static	cBARMENU			s_bmChattingFilter,s_bmChatBoxScale,s_bmChatBarMenu;
	static	int					s_iChatFilter;
	static	int					s_iCurrentSayTalkerIndex;
	static	cSTRINGS			s_strSayUserList[100];

	static	void				InitChattingBox();
	static	void				ResetChattingBoxFrame(BOOL _bIsResetInputChatting=FALSE);
	static	BOOL				CheckMessage(char *_lpstrSrcText,char *_lpstrResultText);
	static	void				Chat(SG_CHAT *_lpChatPacket,BOOL _bIsRecordPlay=FALSE);
	static	void				EventNotice(char *_lpstrMessage,int _iType,BOOL _bIsRecordPlay,DWORD _dwColor , WORD _wTime  = 30, WORD _wEffectflickeringly = TRUE);
	static	void				RecordChat(char *_lpstrName,char *_lpstrMessage,int _iChatType,int _iPacketType,int _iChatRecordCategory,DWORD _dwColor);
	static	void				AddChatMessage(DWORD _dwColor,char *_lpstrMessage,char *_lpstrName=NULL);

	static	void				ReceiveSayResult(SG_SAY_RESULT *_lpPacket,BOOL _bIsPlayRecordData=FALSE);
	static	inline	void		OperateChatBoxScrollBar();//	채팅 박스 스크롤바 처리
	static	void				OperateChatMessageBox();	//	시스템 메시지 처리
	static	void				ToggleChattingBoxScale();

	static	void				RebuildChatMessageBox();
	static	void				RebuildChatMessageBoxFrame(BOOL _bIsRequireRebuildChatMessage=TRUE);

	static	void				DrawChatMessageBox();	//	그리기

	static	BOOL				OperateSystemChatMessage(char *_lpstrChatMessage);	
	static	BOOL				OperateCommunityChatMessage(char *_lpstrChatMessage);
	static	BOOL				OperateCommandChatMessage(char *_lpstrChatMessage);
	static	void				Accuse();
	static	void				OperateChatMessage();

	static	char*				GetNextWhisperName();
	static	char*				GetPreviousWhisperName();
	static	void				changeChattingFilter(DWORD _dwChatType);
//┃CGamePlayChat.cpp
//┃채팅 메시지 처리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃공지 메시지 처리
//┃CGamePlayNotice.cpp
	static	int					s_iNoticeTime;
	static	int					s_iAlpha;
	static	int					s_iLineGap;
	static	char				s_strNotice[dMAX_NOTICE_MESSAGE_SIZE];
	static	WORD				s_wNoticeTimeForOutPut;
	static	WORD				s_wEffectflickeringly;

	static	void				InitNoticeBox();
	static	void				Notice(char *_lpstrMessage , WORD _wTime = 30, WORD _wEffectflickeringly = TRUE,int _iAlpha=-1,int _iLineGap=-1);
	static	void				DrawNotice();	//	그리기

	static	void				DrawBoostExpAndGoldEventInfo();
//┃CGamePlayNotice.cpp
//┃공지 메시지 처리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃시스템 메시지 처리
//┃cGAME_PLAY_SYSTEM_MESSAGE.cpp
	static	int					s_iCountDownForDecreaseSystemMessageLine;

	static	void				InitSystemMessageBox();//	시스템 메시지 박스 초기화
	static	void				AddSystemMessage(DWORD color,char *_strText,...);	//	메시지 추가
	static	void				OperateSystemMessageBox();	//	시스템 메시지 처리
	static	void				DrawSystemMessageBox();	//	그리기
//┃cGAME_PLAY_SYSTEM_MESSAGE.cpp
//┃시스템 메시지 처리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃그리기
//┃CGamePlayDraw.cpp
	static	void			DrawMain();

	static	void			DrawNormalGameScreen();
	static	void			DrawFullScreenInterface();
	static	void			DrawInterfaceWindow();
	static	void			DrawInfo();
	static	void			DrawInfoForDuelObserver();
	static	void			DrawInfoForGuildBattle();
	static	void			DrawProposeWorks();
	static	void			DrawImportantInfo();
	static	void			DrawDragObject();
	static	void			DrawChattingBar();
#ifdef dDEBUG_MODE
	static	void			DrawDebugInformation();
#endif
	static	BOOL			drawKarmaDrawImage();
//┃CGamePlayDraw.cpp
//┃그리기
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃하단 인터페이스 처리
//┃CGameBottomInterface.cpp
	static	cRECT			s_arectSkillSlot[],s_arectBeltSlot[];

	static	void			DrawBottomInterface();//하단 인터페이스 그려주기
	static	void			operateScreenEffect();//화면 이펙트 처리.
	static	void			InitBottomInterface();//하단 인터페이스 초기화
	static	BOOL			UpdateBottomInterface();//하단 인터페이스 업데이트
	static	void			OperateBottomInterfaceTooltip();//하단 인터페이스의 슬롯들 툴팁
//┃CGameBottomInterface.cpp
//┃하단 인터페이스 처리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃스테이터스 창
//┃CStatusWindow.cpp
	static	CPos			s_posStatusWindow;		//	인벤토리 윈도우 위치
	static	int				s_iStatusWindowScrollBarPos,s_iFocusTitle,s_iLastActionTime,s_iSelectTitle,s_iUseTitle;
	static	BOOL			s_bIsTitleScrollBar;	// 스테이터스 스크롤 그리기 여부		10.02.18
	static	CImageScrollBar	s_sbStatusWindow;		// 스테이터스 스크롤					10.02.18
	static	cBARMENU		s_bmStatusButton;	// 칭호타입 버튼	10.02.22	

	static	void			CloseStatusWindow();	//	스테이터스 창 닫기
	static	void			OpenStatusWindow();		//	스테이터스 창 열기
	static	void			OperateLevelPoint();	//	레벨 포인트 처리

	static	void			UpdateStatusWindow();	//	스테이터스 윈도우 업데이트
	static	void			DrawStatusWindow();		//	스테이터스 윈도우 그려주기
	// 10.02.18
	static	int				GetTitleImageIndex(int _iIndex);	// 칭호 이미지 인덱스 얻어오기
	static	int				CheckTitleIndex();					// 칭호 체크
	static	void			SetTitleType(int _iType);			// 칭호타입 변경
	static	void			UpdateTitleType();					// 칭호 업데이트
	static	void			OperateTitleScrollBar();			// 칭호 스크롤바
//┃CStatusWindow.cpp
//┃스테이터스 창
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃인벤토리 창
//┃CInventoryWindow.cpp
	static	CPos			s_posInventoryWindow;	//	인벤토리 윈도우 위치
	static	int				s_iBookedWorkItem;
	static	cBARMENU		s_bmCarrotShop;

	static	void			OpenInventoryWindow();	//	인벤토리 창 열기
	static	void			SetInventoryWindowPos();//	인벤토리 창 위치 정하기
	static	void			CloseInventoryWindow();	//	인벤토리 창 닫기

	static	inline	void	OperateInventoryWindowPopupMenu();
	static	void			OperateInventoryWindow();
	static	void			OperateDragItem(BOOL _bIsCheckExclusiveMode=TRUE);							//	드래그 중인 아이템 처리
	static	void			OperateDropItem(int _iItemSlot);			//	아이템 버리기
	static	void			UpdateInventoryWindow();

	static	int				GetFocusItemInInventoryWindow();			//	포커스 아이템
	static	int				GetFocusItemSlot();							//	포키스 아이템 슬롯
	static	int				GetItemDroppingArea();						//	아이템을 떨군 위치
	static	int				GetFocusEquipmentInInventoryWindow(BOOL _isWantOnlyAvailItem=TRUE);		//	초점이 간 장비

	static	int				GetFocusBeltSlot();							//	초점이 간 벨트 슬롯 얻어오기
	static	BOOL			IsInvisibleItem(int _iSlot);

	static	void			PutItem(cItem *_lpItem,int _iX,int _iY,BOOL _bIsPutCount=FALSE,BOOL _bIsSmallSize=FALSE,int _iTwinkleFrame=-1,BOOL bIsRed=FALSE);	//	마법  아이템 심볼 그려줘
	static	void			DrawInventoryWindow();						//	인벤토리 윈도우 그리기
	static	void			DrawExtraInventoryWindow();
//┃CInventoryWindow.cpp
//┃인벤토리 창
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃시스템 메뉴
//┃CSystemWindow.cpp
	static	CPos			s_posSystemMenu;
	static	int				s_iFocusedSystemMenu;

	static	void			setSystemMenu();
	static	void			PopupSystemMenu();//아이템 & 스테이터스 인터페이스 열기
	static	void			CloseSystemMenu();//창 닫아
	static	void			DrawSystemMenu();//스테이터스 인터페이스 그리기
	static	void			UpdateSystemMenu();

	static	void			AskExitGame(int _iWhere,BOOL _bIsNow=FALSE,BOOL _bIsByServer=FALSE);
	static	void			AskUsePerfectResurrectionScroll();
	static	void			AskUseResurrectionScroll();
//┃CSystemWindow.cpp
//┃시스템 메뉴
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃대화
//┃CGamePlayMinimap.cpp
	static	cRECT				s_rectMinimapFrame,s_rectMapFrame,s_rectTitle,s_rectBorderFrame;
	static	cBARMENU			s_bmMinimap, s_bmAddMinimap;	// insu add .. s_bmAddMinimap

	static	void				InitMinimap();
	static	void				DrawMinimap();	//	미니맵 그리기
	static	BOOL				GetMMCPosToReal(CPos *_lpPos=NULL);
	static	void				BuildMinimapFrameInfo();
	static	void				UpdateMinimap();//	미니맵 
	static	void				UpdateMinimapQuestList();
//┃CGamePlayMinimap.cpp
//┃미니맵
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃트레이드 윈도우
//┃CGameTradeWindow.cpp
	static	CTradeManager		s_tradeManager;
	static	cItem				*s_lpInventoryWindowEquipmentList,*s_lpInventoryWindowItemList;
	static	int					*s_lpGold,s_iReadyToUseItem;

	static	char				s_strRequestTradeAskerName[dNAME_LENGTH];
	static	cRECT				s_rectTradeBox;
	static	int					s_iTradeItemSlot,s_iDragTradeItem,s_iChangeTradeItemSlot,s_iSetGoldValueCount;
	static	int					s_iMateLevel;

	static	void				OpenTradeBox(CActor *_lpMate,int _iTradeBoxSerial,int _iTargetLevel);	//	트레이드 박스 열기
	static	void				CloseTradeBox();	//	상점 닫기
	static	void				DrawTradeBox();		//	그리기
	static	void				UpdateTradeBox();	//	업데이트
	static	void				OperateDragItemInTradeWindow();
	static	int					GetFocusItemInTradeWindow();	//	거래창에 포커스가 간 아이템 찾기
	static	inline	BOOL		IsOpenTradeBox()	{return	s_tradeManager.m_bIsActive;}
	static	BOOL				GetTradeItem(int _iSlot,cItem* _lpTradeItem);

//━━	트레이드관련 메시지 전송
	static	void				AskAddTradeItem(int _iInventorySlot);

//━━	트레이드관련 메시지 받아서 처리
	static	void				RequestTrade(SG_REQUEST_TRADE *_lpPacket);
	static	void				AddTradeItemByMe(int _iSlot,int _iCount);
	static	void				AddTradeItemByTradeMate(cItem *_lpItem);
	static	void				ChangeTradeItemDataByMe(int _iTradeSlot,int _iSlot,int _iCount);
	static	void				ChangeTradeItemDataByTradeMate(int _iTradeSlot,cItem *_lpItem);
	static	void				SetTradeGold(int _iTrader,int _iGold);
	static	void				RemoveTradeItem(int _iTrader,int _iTradeBoxSlot);
	static	void				ReadyTrade(int _iTrader);
	static	void				PermitTrade(int _iTrader);
	static	void				Trade(SG_TRADE *_lpPacket);
	static	BOOL				EmptyTradeItem(SG_TRADE *_lpPacket);
	static	BOOL				ReceiveTradeItem(SG_TRADE *_lpPacket);
//┃CGameTradeWindow.cpp
//┃트레이드 윈도우
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃패킷 처리
//┃gpPacket.cpp
	static	void				OperateItemUseResult(int _iResult);
	static	void				OperateFieldTrapWork(SG_CLOSE_RANGE_FIELD_TRAP_WORK	*_lpPacket);
	static	void				OperateDoorWork(SG_DOOR_MESSAGE *_lpPacket);
	static	void				OperateSiegeWarfareNotice(SG_SIEGE_WARFARE_NOTICE	*_lpPacket);

	static	void				OperateArcaWork(SG_CLOSE_RANGE_ARCA_WORK	*_lpPacket);

	static	void				OperateSecretDungeonMessage(SG_SECRET_DUNGEON_MESSAGE *_lpPacket);
	static	void				OperateEtcWork(SG_ETC_WORK *_lpPacket);
	static	void				OperateSetAnm(SG_SET_ANM* _lpPacket);
	static	void				OperateGuildBattleKillNotice(SG_GUILD_BATTLE_KILL_NOTICE* _lpPacket);
	static	void				OperateGuildBattleInfo(SG_GUILD_BATTLE_INFO* _lpPacket);
	static	void				OperateUpgradeSuccess(int _iRemoveItemSlot, int _iUpgradePenaltyItemSerial,int _iUpgradeItemSlot, int _iUpgradeItemNormalSlot);		// 09.09.02
	static	void				OperateUpgradeFailed(int _iRemoveItemSlot, int _iUpgradePenaltyItemSerial,int _iUpgradeItemSlot, int _iUpgradeItemNormalSlot);		// 09.09.02

	static	void				operateSummonEventMonster();	//	이벤트 몬스터 소환

//┃gpPacket.cpp
//┃패킷 처리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃파티
//┃GamePlayParty.cpp
	static	int					s_iPartyCount,s_iSelectParty,s_iFirstPartyIndex,s_iLastPartyIndex;
	static	CPatyInfo			s_partyInfo;
	//static  CUnionPartyInfo		s_unionPartyInfo;
	static	char				s_strPartyJoinAskerName[dNAME_LENGTH];

	static	void				AskJoinUnionParty(SG_ASK_JOIN_UNION_PARTY *_lpAskerInfo);
	static	void				ReplayJoinUnionParty(SG_UNION_PARTY_WORK *_lpAskerInfo);
	static	void				OperateUnionPartyMemberInfo(SG_UNION_PARTY_MEMBER_INFO *_info);
	static	void				OperateUnionPartyInfo(SG_UNION_PARTY_INFO *_info);
	static	void				OperateUnionPartyDissolution(SG_UNION_PARTY_WORK *_lpDissolutionInfo);

	static	void				ResetPartyInfo();	//	파티창 초기화
	static	void				ChangePartyStatus(SG_CHANGE_PARTY_STATUS *_lpPacket);
	static	void				SetPartyInfo(SG_PARTY_INFO *_lpPartyInfo);
	static	void				AskJoinParty(SG_ASK_JOIN_PARTY *_lpAskerInfo);
	static	void				JoinPartyMember(SG_JOIN_PARTY_MEMBER *_lpMemberInfo);
	static	void				OperatePartyWork(SG_PARTY_WORK *_lpMemberInfo);
	static	void				ChangePartyName(char *_lpstrPartyName);//	파티 이름 변경
	static	void				DissolutionParty();//	파티 해체 메시지
	static	void				UpdatePartyMemberInfo(SG_UPDATE_PARTY_MEMBER_INFO *_lpPartyInfo);
	static	void				UpdatePartyMemberHP(SG_UPDATE_PARTY_MEMBER_HP *_lpPartyInfo);
	static	void				UpdateMemberNameBarAtField();	//	필드에 멤버 네임 바를 업데이트
	static	void				DrawPartyMemberNameAtField();
	static	void				DrawMemberNameAtField(int _iX , int _iY,int _iMemberIndex);
	static	void				DrawActorNameBarAtField(int _iX,int _iY,char *_lpstrName,int _iHPRate,int _iSerial,int _bIsDisConnected,DWORD _dwTextColor,int _iJob=-1);
	static	void				DrawActorNameBarAtFieldForUnionParty(int _iX,int _iY,char *_lpstrName,int _iHPRate,BOOL _bIsNotSameField,int _bIsDisConnected,DWORD _dwTextColor);
	static	void				DrawDuelActorNameBar(int _iX,int _iY,int _iWidth,int _iFillRate,int _iBarIndex,char *_lpstrText,DWORD _dwTextColor,BOOL _bIsLeft);

	static	void				DrawPetNameAtField(int _iY,int _iPetIndex);
	static	void				DrawSummonBeastNameAtField(int _iY,int _iPetIndex);

//┃GamePlayParty.cpp
//┃파티
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃파티
//┃GamePlayInterface.cpp
	static	void				EnableChattingBar(BOOL _bIsEnable,int _iTextLength=-1);
//┃GamePlayInterface.cpp
//┃파티
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃은총

	static int					GetGGGSkillUpTableValue(int _attribute , int _IncreceType);
	static int					GetGGGSkillUpTimeValue(int _Level);

//┃은총
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

};

class	COperatorCommandInfo
{
public:
	WORD	m_wCommand;
	char	m_strComment[64];
	char	m_strCommand[20];
};

extern	COperatorCommandInfo	g_aOperatorCommandInfo[100];

//
//	Bottom Interface Menu
enum
{
	eBIM_RUN				,
	eBIM_WALK				,
	eBIM_TRANS				,
	eBIM_QUICK_SKILL1		,
	eBIM_QUICK_SKILL2		,
	eBIM_QUICK_SKILL3		,
	eBIM_QUICK_SKILL4		,
	eBIM_QUICK_SKILL5		,
	eBIM_QUICK_SKILL6		,
	eBIM_QUICK_SKILL7		,
	eBIM_QUICK_SKILL8		,
	eBIM_QUICK_SKILL9		,
	eBIM_QUICK_SKILL10		,
	eBIM_QUICK_SKILL_UP		,
	eBIM_QUICK_SKILL_DOWN	,
	eBIM_LEFT_SKILL1		,
	eBIM_RIGHT_SKILL1		,
	eBIM_LEFT_SKILL2		,
	eBIM_RIGHT_SKILL2		,

	eBIM_QUICK_ITEM1		,
	eBIM_QUICK_ITEM2		,
	eBIM_QUICK_ITEM3		,
	eBIM_QUICK_ITEM4		,
	eBIM_QUICK_ITEM5		,
	eBIM_QUICK_ITEM_RELOAD	,

	eBIM_MENU_STATUS		,
	eBIM_MENU_INVENTORY		,
	eBIM_MENU_SKILL			,
	eBIM_MENU_PARTY			,
	eBIM_MENU_QUEST			,
	eBIM_MENU_GUILD			,
	eBIM_MENU_FRIEND		,
	eBIM_MENU_SYSTEM		,
	eBIM_SITDOWN			,
	eBIM_STAND_UP			,
};

//
//	Tool Button Menu
enum
{
	eTBM_CLOSE_WINDOW,	//	윈도우 닫기
};

#define	dITEM_COUNT_IN_INVENTORY_WINDOW	22


enum
{
	ePM_ITEM_DROP			,	//	아이템 버리기
	ePM_ITEM_USE			,	//	아이템 사용하기
	ePM_USE_GATE_GLOVE		,
	ePM_PUT_BANNER			,
	ePM_PARTY_JOIN_REQUEST	,	//	파티 참가 요청
	ePM_BAN_PARTY			,	//	파티에서 쫏아내기
	ePM_RETURN_TO_VILLAGE	,
	ePM_CALL_MAGIC_CARPET	,
	ePM_REMEMBER_THIS_PLACE_TO_SLOT1,
	ePM_REMEMBER_THIS_PLACE_TO_SLOT2,
	ePM_WARP_TO_WAYPOINT_SLOT1,
	ePM_WARP_TO_WAYPOINT_SLOT2,
	ePM_DESTROY_CART_ITEM,
	ePM_DESTROY_ITEM,

	ePM_REGIST_REGULAR_GUILD_MEMBER	,
	ePM_APPOINTMENT_SUB_GUILD_MASTER,
	ePM_DISMISS_PARLIAMENT			,
	ePM_APPOINTMENT_GUILD_MASTER	,
	ePM_DISMISS_SUB_GUILD_MASTER	,
	ePM_BANISH_MEMBER				,

//	Player Right Menu
	ePM_PRM_TRADE,
	ePM_PRM_ASK_JOIN_PARTY,
	ePM_PRM_SAY,
	ePM_PRM_ADD_FRIEND,
	ePM_PRM_DENY_COMMUNITY,
	ePM_PRM_NO_MANNER_POINT	,

	ePM_SKILL_TREE,

	ePM_OPEN_DOOR,
	ePM_ATTACK_DOOR_LOCK,
	ePM_UNLOCK_DOOR_BY_SKILL,
	ePM_UNLOCK_DOOR_BY_KEY,
	ePM_DISARM_DOOR_TRAP,
	ePM_ATTACK_ARCA	,	//	상자 공격
	ePM_ATTACK_DOOR	,	//	상자 공격
	ePM_USE_COUPLE_RING,

//	Party Status Button Menu
	ePSBM_PO_HUNT			,	//	사냥
	ePSBM_PO_EXPLORATION	,	//	탐험
	ePSBM_PO_QUEST			,	//	퀘스트 해결

	ePSBM_CHANGE_RECRUIT_PARTY_MEMBER_RANGE	,	//	사냥
	ePSBM_PS_RAISE_PARTY_MEMBER,
	ePSBM_PS_DO_NOT_RAISE_PARTY_MEMBER,

	ePSBM_PGSM_FREE			,
	ePSBM_PGSM_EQUALITY		,
	ePSBM_PGSM_ORDER		,

	ePSBM_PISM_FREE			,
	ePSBM_PISM_RANDOM		,
	ePSBM_PISM_ORDER		,
	ePSBM_PISM_FIRST_JOB_RANDOM,
	ePSBM_PISM_FIRST_JOB_ORDER,

	eJPRUL_ACCEPT_JOIN_PARTY_REQUEST,
	eJPRUL_REMOVE_USER_FROM_JOIN_PARTY_USER_LIST,
	eJPRUL_RESET_JOIN_PARTY_USER_LIST,

	ePCDM_ASK_FRIEND,
	ePCDM_SEND_WHISPER,
	ePCDM_ASK_PARTY,

	ePM_USE_SET_ISPI_STRENGTH		,
	ePM_USE_SET_ISPI_AGILITY		,
	ePM_USE_SET_ISPI_CONSTITUTION	,
	ePM_USE_SET_ISPI_CHARISMA		,
	ePM_USE_SET_ISPI_INTELLIGENCE	,
	ePM_USE_SET_ISPI_WISDOM			,
	ePM_USE_SET_ISPI_LUCK			,
	ePM_USE_BOTTOMLESS_BOX			,
	ePM_READ_BOOK					,
	ePM_REMOVE_TITLE				,

	ePM_RENT_GUILD_PET_OBITIAN		,
	ePM_RENT_GUILD_PET_DASH_BLADE	,
	ePM_RENT_GUILD_PET_ASTRO_BOW	,
	ePM_RENT_GUILD_PET_CRISTAL_WATER,
	ePM_RETRIEVAL_GUILD_PET			,

	ePM_MINIPET_PLACE_TO_LEFT	,
	ePM_MINIPET_PLACE_TO_RIGHT	,
	ePM_MINIPET_PLACE_TO_UP		,
	ePM_MINIPET_PLACE_TO_DOWN	,

	ePM_NETCAFE_GATE_GLOVE		,

	ePM_UNION_PARTY				,
	ePM_REMEMBER_THIS_PLACE_TO_SLOT3,
	ePM_WARP_TO_WAYPOINT_SLOT3,

	ePM_CHANGE_FRIEND_GROUP_NAME	,
	ePM_USE_SHOP,				// 상점 이용하기
	ePM_USE_BANK,				// 은행 이용하기
	ePM_CHANGE_FRIEND_GROUP			=	1000,
	ePM_CHANGE_FRIEND_GROUP_END		=	1010,

	ePM_SUMMON_MAGIC_CARPET			=	2000,


	ePM_CHANGE_MONSTER_JOB_BEGIN	=	4000,	//	항상 마지막에 둘것
	ePM_CHANGE_MONSTER_JOB_END		=	6000,	//	항상 마지막에 둘것

	ePM_TAME_MONSTER_BEGIN			=	7000,	//	항상 마지막에 둘것
	ePM_TAME_MONSTER_END			=	10000,	//	항상 마지막에 둘것

	ePM_CHANGE_ENABLE_WIDTHDRAW_GOLD_LIMIT_FOR_GUILD_INVENTORY_BEGIN	=	6000,	//	항상 마지막에 둘것
	ePM_CHANGE_ENABLE_WIDTHDRAW_GOLD_LIMIT_FOR_GUILD_INVENTORY_END		=	6100,	//	항상 마지막에 둘것
	ePM_CHANGE_ENABLE_WIDTHDRAW_ITEM_LIMIT_FOR_GUILD_INVENTORY_BEGIN	=	6200,	//	항상 마지막에 둘것
	ePM_CHANGE_ENABLE_WIDTHDRAW_ITEM_LIMIT_FOR_GUILD_INVENTORY_END		=	6300,	//	항상 마지막에 둘것
	ePM_CHANGE_GUILD_INVENTORY_STORAGE_SIZE	,	//	길드 인벤토리 창고 사이즈 변경


	ePM_PRM_ASK_GUILD_MEMBER_JOIN_PARTY,		//	길드멤버파티초대

};	//	팝업 메뉴

extern	int		g_iHeavenRedStoneCount,g_iHellRedStoneCount,g_iRedDevilRedStoneCount;
extern	BOOL	g_bIsWaitGetGVGAvatarOrgData;

#endif