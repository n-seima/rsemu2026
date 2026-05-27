#ifndef _classGAME_H
#define	_classGAME_H

#include "cNEMO.H"
#include "cMAIN.H"
#include "cACTION.H"
#include "CImageData.H"
#include "cPACKET.H"
#include "cTOOLTIP.H"
#include "CGameSaveOption.H"

enum
{
	eGAME_INITIALIZING				,	//	초기화
	eGAME_OPENING					,	//	오프닝
	eGAME_LOGO						,	//	오프닝
	eGAME_TITLE						,	//	타이틀 화면
	eGAME_CONNECT_GAME_SERVER		,	//	서버에 접속
	eGAME_OPTION					,	//	옵션
	eGAME_SELECT_AVATAR				,	//	아바타 선택
	eGAME_CREATE_AVATAR				,	//	아바타 생성
	eGAME_PLAY						,	//	게임 플레이
	eGAME_ENDDING					,	//	엔딩
	eGAME_CREDIT					,	//	크레딧
	eGAME_CLOSE						,	//	게임 끝내기
};


#define	dGAME_MODE_COUNT				20

#define	dSTEP_INIT						0
#define	dSTEP_READY						1
#define	dSTEP_MAIN						2
#define	dSTEP_FINISH					3
#define	dSTEP_CLOSE						4

#define	dSTEP_COUNT						6


#define	dSYNC_FPS						64
#define	dSYNC_FPS_IN_SERVER				16
#define	dUPDATE_PERIOD					4
#define	dMOVE_TYPE						2			//	타원형 이동

#define	dUSE_MCU						1

class cGAME_MCU
{
public:
	static	cMCU	s_mcuStatic;			//	한번 로딩하고 변경하지 않는 데이터

	static	cMCU	s_mcuData;				//	오브젝트,건물,맵,기타등등,유동적인 데이터
	static	cMCU	s_mcuFieldImage;		//	타일/오브젝트/건물(레퍼런스 s_mcuData)
	static	cMCU	s_mcuBodyImage;			//	NPC,몬스터(레퍼런스 s_mcuData)
};

#ifdef	dUSE_MCU
#define	mMcuStatic		(&s_mcuStatic)		
#define	mMcuFiledImage	(&s_mcuFieldImage)	
#define	mMcuBody		(&s_mcuBody)		
#else
#define	mMcuStatic		NULL
#define	mMcuFiledImage	NULL
#define	mMcuBody		NULL
#endif

class	CAgent;
class	CTextDC;
class	CTextMT;

class	cLoginServerIPInfo
{
public:
	char	m_strLocalName[40];
	char	m_strIP[64];
};

class cGAME_INTERFACE_STATUS
{
public:
	static	BOOL	s_isViewInterface;			//	인터페이스가 보인다.
};

class cGAME_INPDEV_STATE
{
public:
	static	BOOL	s_isHeroFocusOn;						//	주인공에게 포커스 맞추기
	static	BOOL	s_isSmoothFocusing;						//	부드럽게 포커스 맞추기 on!!

	static	int		s_iFocusAreaStack;						//	포커스 영역 저장

	static	int		s_iFocusActor;							//	포커스 캐릭터
	static	int		s_iFocusArea;							//	포커스 영역
	static	int		s_iFocusItem;							//	포커스 아이템
	static	int		s_iFocusEquipment;						//	포커스 장비
	static	int		s_iFocusFieldItem;						//	포커스 필드 아이템
	static	int		s_iFocusTrap;							//	포커스 함정
	static	int		s_iFocusSkill;							//	포커스 스킬
	static	int		s_iFocusBeltSlot;						//	포커스 벨트 슬롯

	static	inline	void	ResetAllFocus()
	{
		s_iFocusActor		=	0xffff;
		s_iFocusArea		=	0xffff;
		s_iFocusItem		=	0xffff;
		s_iFocusSkill		=	0xffff;
		s_iFocusFieldItem	=	0xffff;
		s_iFocusBeltSlot	=	0xffff;
		s_iFocusTrap		=	0xffff;
	}

	static	inline	BOOL	IsValidFocus()	//	포커스 없냐?
	{
		if (s_iFocusActor		!=	0xffff)	return	FALSE;
		if (s_iFocusArea		!=	0xffff)	return	FALSE;
		if (s_iFocusItem		!=	0xffff)	return	FALSE;
		if (s_iFocusSkill		!=	0xffff)	return	FALSE;
		if (s_iFocusFieldItem	!=	0xffff)	return	FALSE;
		if (s_iFocusTrap		!=	0xffff)	return	FALSE;

		return	TRUE;
	}

	static	inline	BOOL	IsExclusiveFocus()	//	포커스 없냐?
	{
		if (s_iFocusActor		!=	0xffff)	return	TRUE;
		if (s_iFocusArea		!=	0xffff)	return	TRUE;
		if (s_iFocusItem		!=	0xffff)	return	TRUE;
		if (s_iFocusSkill		!=	0xffff)	return	TRUE;
		if (s_iFocusFieldItem	!=	0xffff)	return	TRUE;
		if (s_iFocusTrap		!=	0xffff)	return	TRUE;

		return	FALSE;
	}

	static	inline	void	PushFocusArea()
	{
		s_iFocusAreaStack	=	s_iFocusArea;
		s_iFocusArea		=	0xffff;
	}

	static	inline	void	PopFocusArea()
	{
		if (IsValidFocus())
			s_iFocusArea		=	s_iFocusAreaStack;
	}
};

class cGAME_UTIL
{
public:
	static	CTooltip	s_ttCommon,s_ttDialogFrame,s_ttFrame;
	static	CPopupMenu	s_pmCommon;
	static	BOOL		s_bIsSpeedHackUser;				//	이 녀석 스피드핵 유저다.
	static	WORD		s_wSpeedHackUserCountdown;
	static	char		s_strTimeStr[256];

	static	BOOL		CheckID(char *_strID);			//	아이디 체크
	static	BOOL		CheckName(char *_strName);		//	이름 체크
	static	void		ReadLastPlayAvatarName(char *_lpstrId,char *_lpstrName);//	마지막으로 플레이한 아바타 이름 얻어오기
	static	void		DrawLoadingText();	//	Loading 텍스트 그리기

	static	void		BeginSpeedHackChecker();
	static	void		CloseSpeedHackChecker();
	static	BOOL		CheckSpeedHack();
	static	BOOL CALLBACK	OperateSpeedHackUser(int _iType);
	static	void		EncodePassword(char *_lpstrPassword);
	static	void		DecodePassword(char *_lpstrPassword);
	static	char*		GetTimeStringHM(int _iHour,int _iMinute);	//	시간과 분을 얻는다.

	static	BOOL		ImeUpdate(int _iX,int _iY,BOOL _bIsClickedLeftButton,BOOL _bIsClickedRightButton);
	static	BOOL		ImeDraw(HDC _hDC);
	static	int			ImeMessageProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	static	void		ChangeCursorShape(HCURSOR _hCursor);

	class	cLIGHT
	{
	public:
		cSPLINE		m_spline;
		int			m_iAnm,m_iFrameCount,m_iFrame;

		void		init(int _iAnm,int _iFrameCount,int _iX1,int _iY1,int _iX2,int _iY2,float _fSplineDelta)
		{
			m_iAnm			=	_iAnm;
			m_iFrame		=	random(_iFrameCount);
			m_iFrameCount	=	_iFrameCount;
			m_spline.init(_iX1,_iY1,_iX2,_iY2,_fSplineDelta);
			m_spline.update();
		}

		inline	void	updateSpline()	{	m_spline.update();}
		inline	void	updateFrame()
		{
			m_iFrame++;
			if (m_iFrame	>=	m_iFrameCount)	m_iFrame	=	0;
		}
	};
};

class	CGameInfo
{
public:
	static	int						s_iLastSecondCount;
	static	int						s_iSecondCount;
	static	int						s_iFrameCounter;						//	프레임 카운트
	static	BOOL					s_isUpdateTimingByUpdatePeriod;			//	업데이트 주기에 의한 업데이트 타이밍이다.
	static	BOOL					s_bIs1Second;							//	1초
	static	int						s_iFrameCounterFor1SecondWork;
	static	char					s_strLastSelectServerName[dNAME_LENGTH];//	선택한 월드 서버
	static	char					s_strLoginServerAddress[64];
	static	char					s_strGameServerAddress[64];		//	게임 서버 주소(임시)
	static	int						s_iLoginServerPort;

	static	cSERVER_INFO			s_aServerList[dMAX_SERVER_COUNT];		//	월드 서버 리스트
	static	int						s_iWorldServerCount;					//	월드 서버의 수
	static	int						s_iServerGameVersion;					//	서버의 게임 버젼

	static	int						s_iConnectType;							//	접속 이유
	static	BOOL					s_isEnterenceGame;						//	게임에 들어가는거다.


	static	int						s_iSelectAvatar;						//	선택한 아바타
	static	char					s_strSelectAvatarName[dNAME_LENGTH];	//	선택한 아바타 이름
	static	cAVATAR_INFO			s_aAvatarList[dMAX_AVATAR_COUNT];		//	자신의 아바타 정보

	static	char					s_strLastID[dID_LENGTH];				//	로그인 한 아이뒤
	static	char					s_strLastPassword[64];		//	로그인 한 아이뒤
	static	char					s_strSelectServerName[dNAME_LENGTH];
	static	char					s_strLastSelectAvatarName[dNAME_LENGTH];
	static	char					s_strGuildHallOwnerGuildName[dGUILD_NAME_LENGTH];
	static	char					s_strMessage[256];

	static	char					s_strBookingToMoveFieldFileName[64];
	static	char					s_strBookingToMoveFieldServeAddress[20];
	static	int						s_iMySerial;							//	나 자신의 시리얼

	static	BOOL					s_bIsVillage,s_bIsGuildBattleField,s_bIsSiegeWarfareField,s_bIsHideInterface,s_bIsBlockUseSkill;
	static	BOOL					s_bIsObserverMode,s_bIsOXQuizField,s_bIsWordQuizField,s_bIsEventField,s_bIsGuildHall;
	static	BOOL					s_bIsGuildPointBattleField,s_bIsBossZone;
	static	int						s_iMinimapDisplayLevel,s_iRemainGuildStategyPoint;	//	미니맵 표시 레벨
	static	int						s_iFieldSerial;	//	필드 시리얼

	static	int						s_iLoginServerLocalCount;
	static	cLoginServerIPInfo		s_aLoginServerIPList[20];



//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃네트워크 상에서 일어난 이벤트들 정리
	static	int						s_aNetworkEvent[10];							//	갖가지 이벤트를 처리하기 위한 10개
	inline	static	void			ResetNetworkEvent()	{memset(s_aNetworkEvent,0,sizeof(s_aNetworkEvent));}

//	로그인 결과 관련
	inline	static	void			SetLogInResult(int _iResult)			{s_aNetworkEvent[0]	=	_iResult;}
	inline	static	int				GetLogInResult()						{return s_aNetworkEvent[0];}

//	OTP로그인 결과 관련
	inline	static	void			SetOTPLogInResult(int _iResult)			{s_aNetworkEvent[1]	=	_iResult;}
	inline	static	int				GetOTPLogInResult()						{return s_aNetworkEvent[1];}

//	아바타 생성 결과 관련
	inline	static	void			SetCreateAvatarResult(int _iResult)		{s_aNetworkEvent[0]=_iResult;}
	inline	static	int				GetCreateAvatarResult()					{return s_aNetworkEvent[0];}
//	아바타 제거 결과 관련
	inline	static	void			SetRemoveAvatarResult(int _iResult)		{s_aNetworkEvent[0]	=	_iResult;}
	inline	static	int				GetRemoveAvatarResult()					{return s_aNetworkEvent[0];}

//	아바타 선택 결과 관련
	inline	static	void			SetSelectAvatarResult(int _iResult)		{s_aNetworkEvent[0]	=	_iResult;}
	inline	static	int				GetSelectAvatarResult()					{return s_aNetworkEvent[0];}

//	게임 연결 결과
	inline	static	void			SetConnectResult(int _iResult)			{s_aNetworkEvent[0]	=	_iResult;}
	inline	static	int				GetConnectResult()						{return s_aNetworkEvent[0];}

//	게임 참가 결과
	inline	static	void			SetJoinResult(int _iResult)				{s_aNetworkEvent[0]	=	_iResult;}
	inline	static	int				GetJoinResult()							{return s_aNetworkEvent[0];}
//┃네트워크 상에서 일어난 이벤트들 정리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
};

class	CMsgBox;

enum
{
	eCLIENT_LANGUAGE_KOREAN,
	eCLIENT_LANGUAGE_CHINA,
	eCLIENT_LANGUAGE_TAIWAN,
	eCLIENT_LANGUAGE_ENGLISH,
	eCLIENT_LANGUAGE_JAPAN,
};

class CGame	:	public cGAME_MCU ,public  cGAME_INTERFACE_STATUS,public cMAIN,public cGAME_INPDEV_STATE,public cINPDEV,
				public cGAME_UTIL,public CGameInfo
{
public:
	static  char			s_strHTTP_IP[256];
	static	cTIMER			s_timer;	//	프레임 스킵을 위한 타이머.
//	static	cCURSOR			s_cursor;
	static	COGGPlayer		s_oggPlayer;
	static	CSound			s_soundPlayer;

//	이미지 데이터 - 시작
	static	cBARMENU		s_bmMenu,s_bmButton,s_bmCheckBox;
	static	cTALK			s_inputBox,s_inputBox2;			//	텍스트 입력
	static	CStream			s_logStream;

//	이미지 데이터 - 끝

	static	int				s_iClientGameType,s_iGameVersion,s_iLanguage;
	static	BOOL			s_isOperatedMsgbox;					//	메시지 박스 작업을 했다.
	static	int				s_iFadeStep;
	static	int				s_iSelectServer;

	static	DWORD			s_dwPasswordAuthenticationCode;

	static	int				s_iDetailMode;						//	게임모드 안에서 세부적인 분류
	static	int				s_iMode,s_iPreMode,s_iBookingMode;	//	게임모드
	static	int				s_iStep,s_iBookingStep;				//	게임모드 안에서 세부 스텝
	static	c4POINT			s_posCursor;					//	픽셸단위 커서 좌표와 타일 단위 커서 좌표
	static	BOOL			s_isSnapShot;						//	스크린샷을 잡아라.

	static	CTextDC			s_textForMsgbox,s_text,s_text2,s_textChat,s_sText,s_bText;						//	일반 폰트와 작은 폰트
	static	CTextDC			s_textBold16,s_textLarge50,s_textLarge32,s_textBold14,s_text14,s_textBold13,s_text13;
	static	CTextDC			s_textBold13M,s_textBold14M,s_textBold16M,s_textBold18M,s_textBold24M;
	static	CTextDC			s_text13M;
	static	CTextDC			s_text13MA90;
	static	CTextDC			s_textBoldLarge30,s_textBold18;
	static	CAgent			s_agent;

	static	BOOL			s_bIsFirstLogin;		// 처음 로그인했다..

	static	BOOL			Init();								//	초기화
	static	BOOL			Update();							//	업데이트
	static	BOOL			UpdateMsgbox();						//	팝업알림 상자 업데이트
	static	BOOL			Draw();								//	그리기
	static	void			TryDisactiveApp();

	static	cANM			s_anmHero,s_anmHero2;
	static	int				s_iLoadedHeroImageJob;

	static	void			DrawHeroPicture(int _iX,int _iY,int _iJob);

	static	void			UpdateBookedWork();					//	예약된 작업 업데이트
	static	void			BookingMode(int _iMode,int _iStep=0xffff,int _iFade=dFADE_NONE)	;//	게임 모드 예약
	static	void			BookingStep(int _iStep,int _iFade=dFADE_NONE);
	static	void			SetStep(int _iStep);//	스텝 예약

	static	BOOL			Close();
	static	char*			GetWorldName(int _iWorldIndex);
};

////////////////////////////////////////////////////////////////////////
//		기타 전역변수
////////////////////////////////////////////////////////////////////////

#ifdef	dDEBUG_MODE

#define	dDEBUG_GRID			1000
#define	dDEBUG_INFORMATION	1001
#define	dDEBUG_BLOCKED_TILE	1002
#define	dDEBUG_CRASH_BOX	1003
#define	dDEBUG_ACTOR_INFO	1004
#define	dDEBUG_RANGE_INFO	1005

#endif

enum
{
	eMBI_RETURN_TO_TITLE			=	1010,
	eMBI_CANCEL_TO_ENTER_GAME			,
	eMBI_ACCEPT_JOIN_PARTY_REQUEST		,
	eMBI_ACCEPT_JOIN_PARTY_APPLICATION			,
	eMBI_DECLINE_JOIN_PARTY_REQUEST		,
	eMBI_DECLINE_JOIN_PARTY_APPLICATION	,
	eMBI_LEAVE_PARTY					,
	eMBI_BANISH_MEMBER					,
	eMBI_CHANGE_PARTY_LEADER			,
	eMBI_CANCEL							,
	eMBI_ACCEPT_TRADE_REQUEST			,
	eMBI_DECLINE_TRADE_REQUEST			,
	eMBI_REMOVE_FRIEND					,
	eMBI_ACCEPT_ADD_FRIEND_REQUEST		,
	eMBI_DECLINE_ADD_FRIEND_REQUEST		,
	eMBI_CANCEL_SELECT_QUEST			,
	eMBI_CHANGE_PREFIX_BY_20031117		,
	eMBI_BANK_TRANSACTION_PERMIT		,
	eMBI_ACCEPT_JOIN_GUILD_REQUEST		,
	eMBI_DECLINE_JOIN_GUILD_REQUEST		,
	eMBI_EXIT_TO_WINDOW					,
	eMBI_CHANGE_GUILD_EXP_TAX			,
	eMBI_CHANGE_GUILD_NOTICE			,
	eMBI_DISMISS_SUB_GUILD_MASTER		,
	eMBI_APPOINTMENT_SUB_GUILD_MASTER	,
	eMBI_APPOINTMENT_GUILD_CONGRESS		,
	eMBI_DISMISS_GUILD_CONGRESS			,
	eMBI_LEAVE_GUILD					,
	eMBI_DISSOLUTION_GUILD				,
	eMBI_BANISH_GUILD_MEMBER			,
	eMBI_CHANGE_GUILD_MASTER			,
	eMBI_REGIST_REGULAR_MEMBER			,	
	eMBI_REPAIR_ITEM					,	
	eMBI_MAKE_GUILD						,
	eMBI_SELECT_GUILD_MARK				,
	eMBI_ENTER_TOWN_PORTAL				,
	eMBI_OPEN_SHOPPING_CART				,
	eMBI_OPEN_PITCHMAN_SHOP				,
	eMBI_STOP_PITCHMAN_SHOP				,
	eMBI_BUY_PITCHMAN_SHOP_ITEM			,
	eMBI_RECOVER_DEATH_PENELTY			,

	eMBI_CALL_MAGIC_CARPET				,

	eMBI_DISPLAY_NAME_WITH_SOUND_OF_LEADERS_BELL	,
	eMBI_NOT_DISPLAY_NAME_WITH_SOUND_OF_LEADERS_BELL	,
	eMBI_USE_SOUND_OF_LEADERS_BELL	,
	
	eMBI_RETURN_TO_THE_VILLAGE_BY_REVIVE,
	eMBI_RETURN_TO_THE_VILLAGE_BY_END_OF_GUILD_BATTLE	,
	eMBI_RETURN_TO_THE_VILLAGE_BY_END_OF_OX_QUIZ_EVENT	,
	eMBI_RETURN_TO_THE_VILLAGE_BY_ITEM	,

	eMBI_CANCEL_TO_MOVE_FIELD			,
	eMBI_REALLY_CANCEL_TO_MOVE_FIELD	,
	eMBI_CANCEL_TO_CANCEL_TO_MOVE_FIELD	,
	eMBI_PERMIT_TRADE					,

	eMBI_PERMIT_CART_WORK				,
	eMBI_DESTROY_CART_ITEM				,
	eMBI_DESTROY_ITEM					,
	eMBI_USE_ITEM						,
	eMBI_CHANGE_MINIPET_NAME			,
	eMBI_MIX_MINIPET					,
	eMBI_RESTRAINT_MINIPET				,
	eMBI_USE_POLISHER					,
	eMBI_USE_MASSIVE_POLISHER			,

	eMBI_ACCEPT_ENCHANT_ITEM			,
	eMBI_MOVE_TO_SELECT_GUILD_HALL		,
	eMBI_MOVE_TO_SELECT_RELATED_PLACE	,

	eMBI_REQUEST_GUILD_BATTLE			,
	eMBI_REQUEST_GUILD_SCRAMBLE			,
	eMBI_CANCEL_GUILD_SCRAMBLE			,
	eMBI_CANCEL_GUILD_BATTLE			,
	eMBI_RETURN_TO_THE_VILLAGE_BY_GUILD_BATTLE_END,
	eMBI_PARTING_WITH_PET,
	eMBI_PARTING_WITH_MINIPET,
	eMBI_RESET_PET_NAME	,
	
	eMBI_USE_BLOCK_LIGHT_POCKET	,
	eMBI_RELEASE_SUMMON_BEAST,
	eMBI_WITHDRAW_MAGIC_BAG,

	eMBI_BUY_CARROT_SHOP_ITEM,

	eMBI_DOOR_DISARM_TRAP,
	eMBI_UNLOCK_DOOR_BY_USE_KEY,
	eMBI_UNLOCK_ARCA_BY_USE_KEY,

	eMBI_ARCA_DISARM_TRAP			,
	eMBI_ARCA_UNLOCK				,

	eMBI_DISARM_FIELD_TRAP			,

	eMBI_RETURN_TO_TITLE_BY_CANCEL	,
	eMBI_RETURN_TO_TITLE_BY_LOGOUT	,
	eMBI_EXIT_TO_WINDOW_BY_FOCED	,
	eMBI_RETURN_TO_TITLE_BY_FOCED	,
	eMBI_EXIT_TO_WINDOW_BY_NGPL		,

	eMBI_REMOVE_SELECT_PREFIX		,
	eMBI_MERGE_SELECT_ITEM			,
	eMBI_JUDGE_TO_SELECT_ITEM		,
	eMBI_USE_GATE_GLOVE				,

	eMBI_MOVE_TO_USE_GAME_CARROT_AGREEMENT_PAGE,
	eMBI_USE_COUPLE_RING			,
	eMBI_USE_TITLE			,

	eMBI_USE_BOTTOMLESS_BOX			,
	eMBI_BUY_PIGEON_SHOP_ITEM		,
	eMBI_REMOVE_TITLE				,
	eMBI_FEED_ITEM_TO_GUILD_PET_OR_GUILD_GUARDIAN,

	eMBI_MAKE_GUILD_MARK			,

	eMBI_TRY_GUILD_POINT_BATTLE		,
	eMBI_RETURN_TO_GUILD_HALL_BY_END_BATTLE		,
	eMBI_RETURN_TO_THE_GUILD_HALL_BY_END_GUILD_POINT_BATTLE	,

	eMBI_ADD_RELATED_PLACE_OF_GUILD	,
	eMBI_FEED_ITEM_TO_MINIPET		,

	eMBI_COPY_ITEM,
	eMBI_USE_ITEM_TO_ITEM,

	eMBI_CANCEL_TRY_DUEL,
	eMBI_USE_AWAKEN_ITME50,
	eMBI_USE_AWAKEN_ITME100,

	eMBI_ACCEPT_JOIN_UNION_PARTY_REQUEST,
	eMBI_DECLINE_JOIN_UNION_PARTY_REQUEST,
	
	eMBI_START_GUILD_DUNGEN,
	eMBI_ACTIVE_GUILD_MASTER_GRACE,
	eMBI_FEED_ALL_ITEM_TO_MINIPET		,
	eMBI_BUY_GOLD_BAR,			//  메세지박스 금괴 ID~~~~~+_+	09.08.19
	eMBI_UPGRADE_ETERNAL_ITEM,		// 이터널 아이템 업그레이드 ~~~ 09.08.26
	eMBI_DISJOINTING_ITEM,				// 아이템 분해 창 메세지~~ 09.09.07
	eMBI_USE_SHOP,			// 상점 이용하기
	eMBI_USE_BANK,			// 은행 이용하기
	eMBI_COPY_SELECT_PREFIX,	// 복사하기
	eMBI_CUT_SELECT_PREFIX,		//	잘라내기

	eMBI_ACCEPT_GUILD_MEMBER_JOIN_PARTY_REQUEST		,	// 길드 멤버의 파티 요청을 수락한당.
	eMBI_DECLINE_GUILD_MEMBER_JOIN_PARTY_REQUEST	,	// 길드 멤버의 파티 요청을 거절한당.
	eMBI_MINIPET_FEED			,						//	미니펫 먹이
	eMBI_RESET_MINIPET_SKILL			,				//	미니펫 스킬초기화
	eMBI_SET_MINIPET_TYPE			,					//	미니펫 타입 변경
	eMBI_TRANS_AVATAR_TO_GVG_SERVER,
	eMBI_TRANS_GUILD_TO_GVG_SERVER	,

};

extern	BOOL		g_bIsTestServer,g_bIsDuelServer;
extern	BOOL		g_bIsInnerTestServer;
extern	BOOL		g_bIsTestClient;
extern	BOOL		g_bIsPutSceneWhenLoading;
extern	BOOL		g_bIsObserverModeForDuel;
extern	BOOL		g_bIsHalfSizeObserver;
extern	int			g_iCountDownForSelfDestruction;
extern	int			g_iWorldServerType;

extern	int			g_iCorrectBI_X;
extern	int			g_iCorrectBI_Y;

extern	CProfiler	g_profiler;

extern	char		g_strLaunchFolder[MAX_PATH];	//	레지스트리 말고 클라이언트 실행 폴더

#endif