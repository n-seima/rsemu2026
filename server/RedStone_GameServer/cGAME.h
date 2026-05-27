#ifndef _classGAME_H
#define	_classGAME_H

#include	"SFC.H"
#include	"cFIELD.H"
#include	"cArea.H"
#include	"CProfiler.h"
#include	"debugCode.h"
#include	"Client.h"
#include	"cPACKET_BCSERVER.h"

#ifdef	_IS_TOOL
#define	dSYNC_FPS							64
#else
#define	dSYNC_FPS							16
#endif
//	서버 전체에서 공통적으로 쓰이는 데이터

#define	dMAX_ZONE_COUNT						20
#define	dMAX_FIELD_COUNT_IN_ZONE			50

#define	dMAX_MAP_COUNT_IN_SERVER			2048

#define	dCORRECT_ACTION_SPEED				130

class cFIELD_INFO
{
public:
	int		m_iSerial;	//	맵 고유 시리얼
	int		m_iType;	//	필드 타입
	char	m_strName[64];	//	화일 이름

	cFIELD_INFO()
	{
		m_iSerial	=	0xffff;
	}

	BOOL	isSurfaceField();
};

class cSERVER_DATA
{
public:
	int			m_iCount;	//	맵의 수
	char		m_strName[64];	//	서버 이름
	cFIELD_INFO	m_aMap[dMAX_MAP_COUNT_IN_SERVER];	//	맵 정보

				cSERVER_DATA()
				{
					m_iCount	=	0;
				}

	cFIELD_INFO*	getFieldInfo(char *_strMapName)
	{
		for (int i=0;i<m_iCount;i++)
			if (STRICMP(m_aMap[i].m_strName,_strMapName)==0)	
				return	&m_aMap[i];

		return	NULL;
	}

	cFIELD_INFO*	getFieldInfo(int _iSerial)
	{
		for (int i=0;i<m_iCount;i++)
			if (m_aMap[i].m_iSerial	==	_iSerial)	
				return	&m_aMap[i];

		return	NULL;
	}
};

#define	dDB_PACKET_BUFFER_SIZE			1024
#define	dWORLD_PACKET_BUFFER_SIZE		1024
#define	dMAX_REQUITAL_TIME_COUNT		1024			

class	cGAME
{
private:
	char				m_strBCSIP[dIP_LENGTH];
	WORD				m_awPlayerCountPerLevel[2000];

public:
	class	cRenewPremiumItemInfo
	{
	public:
		WORD	m_wSerial;
		WORD	m_wIsForOnlyYahoo;
		WORD	m_wIsForOnlyRedGem;
		WORD	m_wExtendTerm;
	};

	static	BOOL		s_isExist;

	BOOL				m_bIsRequireRebuildRedStonePointTitle;	//	붉은 보석 포인트를 새로 받았다. 기존 칭호 다시 한번 체크해 봐라
	BOOL				m_bIsUpdateSightBorderActorInfo;
	int					m_iMaxConnectedPlayerCount,m_iPlayerCount;
	int					m_iCountReserve[3];	//	nhn playnet 유저 DB에 총 3개의 공간이 있음.
	int					m_iResetTimerCounterForSync;
	DWORD				m_dwFrameCounter;
	int					m_iOncePerMinuteWorkBeginIndex,m_iOncePer10SecondWorkBeginIndex,m_iOncePerSecondWorkBeginIndex;
	int					m_iOncePerMinuteWorkLastestOperateTime,m_iOncePer10SecondWorkLastestOperateTime;
	int					m_iOXQuizField,m_iOxQuizVillage,m_iGuildBattleLobbyField;
	int					m_iWordQuizField,m_iWordQuizVillage;
	BOOL				m_bIsBookedBanishAllUser,m_bIsChangeSecond;
	WORD				m_wCheckCountForReceiveInitDataFromWorld;

	DWORD				m_dw1SecondTime;	//	게임상에서 1초 처리하는데 실제로 걸린시간.

	cACTOR				*m_pActor;
	cFIELD				*m_pField;			//	필드
	cSERVER_DATA		m_aServerList[10];		//	월드 정보
	int					m_iFieldCount,m_iServerCount;		//	필드 카운트
	int					m_iMaxActorCount,m_iActorCount,m_iRookieActor,m_iFieldGroupCount,m_iSavedActorCount;

#ifdef	_USE_HS
	int					m_iLastUpdateActorForHS;
#endif

#ifdef	_USE_NPGL
	int					m_iLastUpdateActorForNPGL;
#endif
	
	char				m_strDataFolder[256];
	CRITICAL_SECTION	m_csActorManager;	//	패킷 큐를 위한 메니져. -_-a
	cTIMER				m_timer;
	CProfiler			m_profiler;
	BOOL				m_bResetProfiler,m_bIsExportProfile,m_bIsActiveGame;
	BOOL				m_bIsResetFieldStatics,m_bIsReceiveAllData;
	BOOL				m_bIsBookingRegenNPC;
	WORD				m_wIsReadyToUrgentSave;
	int					m_iFailedClinetCount;
	BOOL				m_aExistField[2048];
	WORD				m_awCloseMapList[2048];
	int					m_iCloseMapCount;

	int					m_iTotalPlayerCountInWorld;
	int					m_aiPlayerCountInfoInWorld[10];

	int						m_iRenewPremiumItemCount,m_iLastRenewPremiumItemCounter;
	cRenewPremiumItemInfo	m_aRenewPremiumItemInfo[100];

						cGAME()
						{
							if	(s_isExist)	
								ERRMSG("no!!!!!!!!!!!","크아아!!");

							m_iRenewPremiumItemCount		=	0;
							m_iLastRenewPremiumItemCounter	=	0;
							s_isExist						=	TRUE;
							m_iFieldCount					=	0;
							m_pField						=	NULL;
							m_pActor						=	NULL;
							m_iMaxActorCount				=	0;
							m_iFieldGroupCount				=	0;
							m_iFailedClinetCount			=	0;
							m_bIsExportProfile				=	FALSE;
							m_bResetProfiler				=	FALSE;
							m_bIsUpdateSightBorderActorInfo	=	FALSE;
							m_wIsReadyToUrgentSave			=	FALSE;
							m_iMaxConnectedPlayerCount		=	0;
							m_dwFrameCounter				=	0;
							m_bIsBookingRegenNPC			=	FALSE;
							m_bIsResetFieldStatics			=	FALSE;
							m_wCheckCountForReceiveInitDataFromWorld	=	0;
							m_bIsReceiveAllData				=	FALSE;

							memset(m_aiPlayerCountInfoInWorld,0,sizeof(m_aiPlayerCountInfoInWorld));
						}

						~cGAME()
						{
						}

	inline	void		off()	{m_bIsActiveGame	=	FALSE;}
	inline	BOOL		isActive(){return	m_bIsActiveGame;}

	inline	BOOL		isCloseMap(int _iIndex)
	{
		for (int i=0;i<m_iCloseMapCount;i++)
			if (m_awCloseMapList[i]	==	_iIndex)
				return	TRUE;

		return	FALSE;
	}

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃각종 초기 설정
	BOOL				init(int _iZoneIndex);
	void				buildActorBuffer();			//	액터 버퍼 설정
	void				close();					//	종료
	void				banishAllUser();			//	전부 쫏아내!!
	void				bookingBanishAllUser(BOOL isFlag = TRUE);
	BOOL				checkLastErrorSign();

	BOOL				loadServerInfo();			//	서버 인포를 읽어 들인다.(server_info.txt)
	BOOL				loadConfigData();			//	콘피그 데이터 디코딩
	BOOL				loadConfigEventData();		//	이벤트 리스트 리딩.
	BOOL				loadConfigForBattleData();	//	듀얼 펙터 디코딩
	BOOL				findDataFolder();			//	레지스트리를 읽어서 게임 데이터가 있는 폴더를 찾는다.
	BOOL				loadField();				//	필드들을 로딩한다.

	char* const			getBCSIP();					//	브로드 캐스트 서버 IP 리턴
	void				setBCSIP(char *_lpstrIP);	//	브로드 캐스트 서버 IP설정
//┃각종 초기 설정
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

	void				update();
	BOOL				checkIncorrectInfo();

	void				updateInterface();
	inline	void		updateGame();
	inline	void		operateOncePerSecondNPGLWork();
	void				operateOncePerSecondHS_Work();
	inline	void		operateOncePerSecondWork();		//	초당 한번씩 업데이트 해야될 작업들 처리
	inline	void		operateOncePerMinuteWork();		//	분당 한번씩 업데이트 해야될 작업들 처리
	inline	void		operateOncePer10SecondWork();	//	10초에 한번씩 업데이트 해야될 작업들 처리
	inline	void		increasePlayerPlayTime();
	inline	void		operateOncePerHourWork();

	void				removeSaveData(char *_lpstrId,int _iAvatarIndex,BOOL _bIsIncludeOldData=FALSE);	//	세이브 데이터를 지운다.

	void				urgentSave();
	void				recoveryUrgentSave();
	int					operteLogoutActorWorkById(char *_lpstrId);
	int					operteLogoutActorWorkByName(char *_lpstrName);
	void				rebuildAllShopInfo();	//	모든 상점의 정보를 업데이트 한다.

	BOOL				isActiveGameServer();
	void				receiveInitDataFromWorld();

	BOOL				addReceivePacket(int _iField,ALL_MSG* _lpPacket,int _iSerial);
	BOOL				bookingMoveFieldUser(cACTOR *_lpActor,char *_lpstrFieldFileName,int _iFieldIndex,int _iGate,int _iX,int _iY);

	cFIELD_INFO			*getFieldInfo(int _iFieldSerial);
	void				resetGuildHallFieldInfo();
	void				resetSiegeWarfareFieldStatus();

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃utilGame.cpp
	DWORD				m_dwThisTimeReceivePacketSizeSum;
	DWORD				m_dwThisTimeReceivePacketCount;
	DWORD				m_dwLastCheckReceivePacketSizeTime;
	DWORD				m_dwMaxReceivePacketSizePerMinute;
	DWORD				m_dwMaxReceivePacketCountPerMinute;
	DWORD				m_dwCountOfCheckReceivePacketSize;
	DWORD				m_dwSumOfReceivePacketSizePerMinute;
	DWORD				m_dwCountOfReceivePacketSizePerMinute;

	DWORD				m_dwThisTimeSendPacketSizeSum;
	DWORD				m_dwThisTimeSendPacketCount;
	DWORD				m_dwLastCheckSendPacketSizeTime;
	DWORD				m_dwMaxSendPacketSizePerMinute;
	DWORD				m_dwMaxSendPacketCountPerMinute;
	DWORD				m_dwCountOfCheckSendPacketSize;
	DWORD				m_dwSumOfSendPacketSizePerMinute;
	DWORD				m_dwCountOfSendPacketSizePerMinute;

	void				operateReceivePacketDataStaticsWork(int _iSize);
	void				operateSendPacketDataStaticsWork(int _iSize);
	int					checkPacketSize(int _iMaxSize,int _iSize,char *_lpstrName);
	void				checkPacketSize();

	void				makeStaticsOfPacketFlowing();
	void				exportUserPlayerInfoInField();
	void				resetUserPlayStaticsInField();

	void				exportProfilerInfomation(char *_lpstrFileName=NULL);//	프로파일러 정보 출력
	void				exportPacketCounterInfomation();					//	패킷 카운터 정보 출력
	void				beginProfileByID(DWORD _dwID,char *_lpstrName=NULL);
	void				beginProfile(char *_lpstrName);
	void				beginProfile(int _iIndex);
	void				endProfileByID(DWORD _dwID);
	void				endProfile(char *_lpstrName);
	void				endProfile(int _iIndex);

//┃utilGame.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSendPacketToClient.cpp
//┃클라이언트에 패킷 전송
	void				sendDisconnectByHackShieldPacket(CClient *_lpClient);
	void				notice(char *_lpstrNotice);
	void				sendPacketToAllPlayer(char *_lpPacket);	//	패킷을 모든 플레이어에게 전송
	void				sendPacketToAllOperator(char *_lpPacket,int _iLevel=-1);	
	void				sendEtcWorkPacketToAllPlayer(int _iWork,int _iValue1=0,int _iValue2=0,int _iValue3=0,int _iValue4=0,int _iValue5=0,int _iValue6=0);
//┃클라이언트에 패킷 전송
//┃CSendPacketToClient.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃receive_packet_from_bcs.cpp
//┃클라이언트에 패킷 전송
	CRITICAL_SECTION	m_csReceiveBCSPacket;	//	패킷 큐를 위한 메니져. -_-a
	int					m_iReceivedBCSPacketCount;
	DWORD				m_dwMaxBCSPacketSize;
	char				*m_apBCSPacketBuffer[dDB_PACKET_BUFFER_SIZE];

	void				initBCSPacketReceiver();
	void				closeBCSPacketReceiver();

	void				operateBCSPacket();
	void				addBCSPacket(char *_lpPacketData);

	void				operateRegClientResult(BG_REG_CLIENT_RESULT *_lpPacket);
	void				operateRegClientSerial(BG_REG_CLIENT_SERIAL *_lpPacket);
	void				operateAskClientInfo(BG_ASK_CLIENT_INFO *_lpPacket);
	void				operateAskItemInfo(BG_ASK_ITEM_INFO *_lpPacket);
//┃클라이언트에 패킷 전송
//┃receive_packet_from_bcs.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃send_packet_to_bcs.cpp
//┃클라이언트에 패킷 전송
	void				sendBPGetItemInfo(cACTOR *_lpActor,char *_lpstrTargetName);
	void				sendBPActorInfo(cACTOR *_lpActor);	//	유저 정버 전송
	void				sendBPUpdatePlayerInfo(cACTOR *_lpActor);	//	클라이언트 정보 갱신
	void				sendBPAddFriend(cACTOR *_lpActor1,cACTOR *_lpActor2,char *_lpstrTargetName=NULL,int _iJob=0);	//	친구 추가
	void				sendBPRemoveFriend(char *_lpstrOwner,char *_lpstrRemoveFriendName);
	void				sendBPGuildChat(cACTOR *_lpActor,char *_lpstrMessage);
	BOOL				sendBPPartyChat(cACTOR *_lpActor,char *_lpstrMessage);
	BOOL				sendBPGuildPacket(void *_lpPacket,int _iGuild);
	
	BOOL				deliverPartyPacket(cACTOR *_lpOwner,void *_lpPacket,int _iField=0xffff);
						//	같은 필드의 파티원에게 패킷을 전달 할 경우 _iField를 세팅해 준다.
	BOOL				deliverPacket(cACTOR *_lpOwner,void *_lpPacket,int _iClientCount,int _iField,WORD *_lpwClientList);
	BOOL				deliverPacket(cACTOR *_lpOwner,void *_lpPacket,int _iTargetClient);
//┃클라이언트에 패킷 전송
//┃send_packet_to_bcs.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSendPacketToWorld.cpp
//┃월드에 패킷 전송
	void				sendWPSimpleWork(int _iWork,int _iValue1=0,int _iValue2=0,int _iValue3=0,int _iValue4=0,int _iValue5=0,int _iValue6=0);	//	단순 작업
	void				sendWPConfirmEnterField(cACTOR *_lpActor);
	void				sendWPPlayerCount(int _iServerIndex,int _iPlayerCount);
	void				sendWPIncreasePackItemCount(int _iPackSerial,int _iItemIndex);
	void				sendWPChangePartyMemberInfo(cParty *_lpParty,char *_lpstrName,int _iLevel,int _iJob,int _iPSC,int _iGDPC);
	void				sendWPAskDuelRecord(cACTOR *_lpActor);
	void				sendWPRemoveFriendInTargetList(char *_lpstrClient,char *_lpstrTarget);
	void				sendWPRemoveFriendInTargetListResult(char *_lpstrClient,char *_lpstrTarget,int _iResult);
	void				sendWPEventMessage(char *_lpstrText,int _iChatType);
	void				sendWPCalling(cACTOR *_lpCaster,int _iRange,int _iSkill);
	void				sendWPIsCommunityDenyStatus(char *_lpstrSender,char *_lpstrReceiver);	//	이사람 커뮤니티 거부 상태다.
	void				sendWPQuizVillage(int _iOXQuizVillage,int _iWordQuizVillage);	//	OX 퀴즈 마을이다.
	void				sendWPUpdateGuildExpInServer(int _iGuildSerial,DWORD _dwExp);

	void				sendPartyMessage(CClient *_lpClient,int _iMessage);
	void				sendWPUserLogoutForMoveField(cACTOR *_lpActor);
	void				sendWPUserLogoutById(char *_lpstrId,char *_lpstrIP);

	void				sendWPChangePartyStatus(cParty *_lpParty,BOOL _bIsChangeSetting=TRUE);
	void				sendWPCreateParty(cACTOR *_lpLeader,cACTOR *_lpMember);	//	월드에 파티 생성 메시지 보내기
	void				sendWPCreateUnionParty(cACTOR *_lpLeader,cACTOR *_lpMember);	//월드에 연합 파티 생성 메시지 보내기.
	void				sendWPJoinParty(cParty *_lpParty,cACTOR *_lpActor,BOOL _bIsReconnect);
	void				sendWPJoinPartyByApplication(char *_lpstrPartyName,int _iPartySerial,cACTOR *_lpActor);
	
	void				sendWPChangePartyName(cParty *_lpParty,char *_lpstrName);	//	월드에 파티이름 변경 메시지를 전송한다.

	void				sendWPAskPartyInfo(int _iPartySerial,char *_lpstrPartyName=NULL);
	void				sendWPUpkeepPlayerData(cACTOR *_lpActor);
	void				sendWPPartyWork(int _iPartySerial,int _iWork,char *_lpstrMemberName);	//	파티 탈퇴
	void				sendWPDisconnectPartyMember(int _iPartySerial,char *_lpstrPartyName,char *_lpstrMemberName);	//	파티 탈퇴
	void				sendWPEnterIFParty(int _iPartySerial,char *_lpstrPartyName,int _iGateField,int _iGateArea);
	void				sendWPAddItem(cACTOR *_lpActor,cItem *_lpItem);
	void				sendWPFindUser(char *_lpstrClientName,char *_lpstrTargetName,int _iField,int _iWork);
	BOOL				sendWPMoveField(cACTOR *_lpActor,char *_lpstrMapname,int _iField,int _iGateIndex,int _iX=-1,int _iY=-1,BOOL _bIsInnerMove=FALSE,BOOL _bIsMoveNow=FALSE);
	void				sendWPMoveFieldPartyMember(char *_lpstrMapname,char *_lpstrName,int _iDestField,int _iZoneSerial,BOOL _bIsLeave,int _iDestInstanceField=0,int _iDestInstanceFieldFloor=0);
	void				sendWPAskItemUniqueSerial(cACTOR *_lpActor);
	void				sendWPSetGuildHallRoenGoodWillInfo(CGuild *_lpGuild,int _wGoodWill);

	void				sendWPChangeGuildInventoryPermitWithdrawItemCount(int _iGuild,int _iStorage,int _iRank,int _iCount);
	void				sendWPChangeGuildInventoryPermitWithdrawGold(int _iGuild,int _iRank,int _iGold);
	void				sendWPChangeGuildInventoryStorageSize(int _iGuild,int _iCount1,int _iCount2,int _iCount3,int _iCount4);

	void				sendWPSetGuildHallLevel(CGuild	*_lpGuild,int _iLevel);
	void				sendWPCreateGuild(cACTOR *_lpMaster,char *_lpstrGuildName,char *_lpstrSubMasterName);
	void				sendWPJoinGuild(char *_lpstrGuildName,char *_lpstrMasterName,cACTOR *_lpAsker,int _iRank);
	void				sendWPIncreaseGuildSkillLevel(int _iGuild,int _iSkill);
	void				sendWPUpdateGuildSkillStatus(int _iGuild,int _iSkill,BOOL _bIsDisable);	//	길드 스킬의 상태 변경

	void				sendWPAddRelatedPlaceOfGuild(int _iGuild,int _iFieldSerial,int _iGold,BOOL _bIsAdd,char *_lpstrName);	//	길드 연고지 추가 제거
	void				sendWPRecordGuildHonorPoint(int _iGuild,int _iGuildHonorPoint);
	void				sendWPStoreGuildInventoryItem(int _iGuild,int _iGuildInventorySlot,cItem *_lpItem,char *_lpstrMemberName,int _iInventorySlot);	//	길드에 인벤에 아이템 넣기
	void				sendWPWithdrawGuildInventoryItem(int _iGuild,int _iGuildInventorySlot,char *_lpstrName,cItem *_lpItem);	//	길드 인벤에서 아이템 꺼내기
	void				sendWPStoreGuildInventoryGold(int _iGuild,int _iGold,char *_lpstrMemberName);	//	길드 인벤에 골드 넣기
	void				sendWPWithdrawGuildInventoryGold(int _iGuild,int _iGold,char *_lpstrName);//	길드 인벤에서 골드 꺼내기
	void				sendWPChangeGuildInventoryItemPlace(int _iGuild,int _iSlot1,int _iSlot2,char *_lpstrName);	//	길드 인벤 안에 아이템 위치 변경

	void				sendWPAskDeleteGuild(cACTOR *_lpActor,char *_lpstrGuildName,int _iGuildSerial);
	void				sendWPChangeGuildNotice(cACTOR *_lpActor,char *_lpstrNotice);
	void				sendWPChangeGuildExpTax(cACTOR *_lpActor,int _iTax);
	void				sendWPChangeGuildExpTax(CGuild *_lpGuild,int _iTax);
	void				sendWPMakeGuildLevel(CGuild *_lpGuild,int _iLevel);
	void				sendWPDismissSubGuildMaster(cACTOR *_lpActor,char *_lpstrName);
	void				sendWPAppointmentSubGuildMaster(cACTOR *_lpActor,char *_lpstrName);
	void				sendWPDismissGuildCongress(cACTOR *_lpActor,char *_lpstrName);
	void				sendWPAppointmentGuildCongress(cACTOR *_lpActor,char *_lpstrName);
	void				sendWPBanishGuildMember(cACTOR *_lpActor,char *_lpstrName);
	void				sendWPLeaveGuild(cACTOR *_lpActor);
	void				sendWPCheckGuildData(WORD *_lpwCheckGuild,int *_lpiCheckSum,int _iCount,DWORD _dwTodayCheckSum);
	void				sendWPChangeGuildMaster(cACTOR *_lpActor,char *_lpstrName);
	void				sendWPRegistRegular(cACTOR *_lpActor,char *_lpstrName);
	void				sendWPChangeGuildRank(CGuild	*_lpGuild,char *_lpstrMemberName,int _iNewRank);
	void				sendWPAskGuildInfo(int _iSerial);
	void				sendWPAskGuildBattleSchedule();	//	길드전 스케쥴 요청
	void				sendWPRequestGuildBattle(cACTOR *_lpActor,int _iTimeOrder,int _iFieldOrder);	//	길드전 예약 요청

	void				sendWPRemoveGuildGold(int _iGuild,int _iGold);
	void				sendWPAddGuildGold(int _iGuild,int _iGold);

	void				sendWPCancelNewGuildBattle(cACTOR *_lpActor,int _iTimeOrder);	//	길드전 취소 요청
	void				sendWPCancelGuildBattle(cACTOR *_lpActor,int _iTimeOrder,int _iFieldOrder);	//	길드전 취소 요청
	void				sendWPCancelGuildBattle(int _iGuild,int _iTimeOrder,int _iFieldOrder);	//	길드전 취소 요청
	void				sendWPAskGuildBattleInfo(int _iTimeOrder,int _iFieldOrder);	//	특정 시간에 길드전 정보를 요청한다.
	void				sendWPGuildBattleResult(int _iGuild1,int _iGuild2,BOOL _bIsWinGuild1,BOOL _bIsWinGuild2);	//	길드전 결과
	void				sendWPSelectGuildMark(int _iSerial,CGuildMarkInfo *_lpMarkInfo,BOOL _bIsUseGuildMarkPack);	//	길드 마크 선택
	void				sendWPUpdateGuildMemberLevel(int _iSerial,char *_lpstrName,int _iLevel);

	void				sendSaveDataToWorld(CClient *_lpClient);
	void				sendWPCheckPartyInfoByActor(cACTOR *_lpActor);
	
	void				sendWPBoostPowerByEvent(int _iWork,char *_lpstrName);
	void				sendWPJoinPartyApplication(cACTOR *_lpAsker,char *_lpstrLeaderName);	//	파티 참가 신청
	void				sendWPJoinPartyApplicationAnswer(char *_lpstrAnswerName,char *_lpstrAskerName,int _iAnswer,cParty *_lpParty);	//	파티 참가 신청에 대한 응답

	void				SendWPCreateUnionParty(WORD _wAskerPartySerial , WORD _wReplayerPartySerial);

	void				sendWPGuildMemberJoinParty(cACTOR *_lpAsker,char *_lpstrTargetName);	// 길드멤버파티요청
	void				sendWPGuildMemberJoinPartyAnswer(char *_lpstrAnswerName,char *_lpstrAskerName,int _iAnswer);	//	길드멤버파티요청에 대한 응답

//┃월드에 패킷 전송
//┃CSendPacketToWorld.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CGameLog.cpp
//┃로그 전송
	void				sendQuizEventWinnerLog(int _iType,char *_lpstrWinners,int _iCount,int _iReviveLoserBattleCount);	//	OX 퀴즈 승자 로그 남기기

	void				sendOXQuizWinnerLog(char *_lpstrWinners,int _iCount,int _iReviveLoserBattleCount);	//	OX 퀴즈 승자 로그 남기기
	void				sendQuizComerListLog(char *_lpstrWinners,int _iCount,int _iType);	//	O퀴즈 참가자 로그 남기기
	void				sendQuizLoserListLog(char *_lpstrLoserList,int _iCount,int _iQuestionCounter,int _iType);	//	OX 퀴즈 탈락자 로그
	void				sendOXQuizQuestionLog(char *_lpstrQuestion,int _iSolution,int _iQuestionCounter);	//	OX 퀴즈 문제

	void				sendWordQuizQuestionLog(char *_lpstrQuestion,char *_lpstrSolution,int _iQuestionCounter);

	void				sendJoinGuildLog(char *_lpstrGuildName,char *_lpstrPlayerName);
	void				sendLeaveGuildLog(char *_lpstrGuildName,char *_lpstrPlayerName);

	void				sendBookingGuildBattleLog(char *_lpstrGuildName,int _iTimeOrder,int _iFieldOrder);
	void				sendCancelGuildBattleLog(char *_lpstrGuildName);
	void				sendEngageGuildBattleLog(char *_lpstrGuildName1,char *_lpstrGuildName2,int _iTimeOrder,int _iFieldOrder);


	void				sendErrorOfGuildBattleLog(int _iError);
	void				sendBeginGuildBattleLog(CGuild *_lpGuild1,CGuild *_lpGuild2,int _iCount1,int _iCount2);
	void				sendResultOfGuildBattleLog(char *_lpstrGuildName1,char *_lpstrGuildName2,int _iVP1,int _iVP2);
	void				sendPreResultOfGuildBattleLog(char *_lpstrGuildName1,char *_lpstrGuildName2,int _iGuild1KP,int _iGuild2KP,int _iCount1,int _iCount2,char *_lpstrGuild1MemberName,char *_lpstrGuild2MemberName);
	void				sendKillPlayerGuildBattleLog(char *_lpstrGuildName1,char *_lpstrGuildName2,char *_lpstrKiller,char *_lpstrTarget,int _iAddKP,int _iGuild1KP,int _iGuild2KP,int _iKillerTeam);
	void				sendExterminateGuildBattleLog(char *_lpstrGuildName1,char *_lpstrGuildName2,int _iKillerTeam);
	void				sendGuildMasterGraceLog(CGuild *_lpGuild , int _iPlayTime , cACTOR *_lpActor);
	void				sendPlayGuildDungeonLog(CGuild *_lpGuild,cACTOR *_lpActor,int _iFloor);
	void				sendTransGuildToGVGLog(CGuild *_lpGuild,cACTOR *_lpActor,char *_lpstrTransGuildName);
	void				sendGuildRoenGoodWillLog(CGuild *_lpGuild, cACTOR *_lpActor, int LastData , int NewData);
//┃로그 전송
//┃CGameLog.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CSendPacketToDB.cpp
//┃DB 서버에 패킷 보내기
	BOOL				sendUseCarrotLog(cACTOR *_lpActor,int _iPremiumItemIndex,int _iCount,int _iCarrotCount,BOOL _bIsIDPublicItem,
										char *_lpstrItemName=NULL,char *_lpstrReceiverID=NULL,char *_lpstrEtcBuffer=NULL);
	BOOL				sendGetCarrotCount(cACTOR *_lpActor);
//┃DB 서버에 패킷 보내기
//┃CSendPacketToDB.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CReceivePacketFromDB.cpp
//┃DB 서버에서 온 패킷 처리
	CRITICAL_SECTION	m_csReceiveDBPacket;	//	패킷 큐를 위한 메니져. -_-a
	int					m_iReceivedDBPacketCount;
	DWORD				m_dwMaxDBPacketSize;
	char				*m_apDBPacketBuffer[dDB_PACKET_BUFFER_SIZE];

	void				initDBPacketReceiver();
	void				closeDBPacketReceiver();

	void				operateDBPacket();
	void				addDBPacket(char *_lpPacketData);

	void				operateKillAvatar(GDMSG_KILLAVATAR* _lpPacket);
	void				operateSaveDataToFileResult(DWMSG_SAVEAVA_TO_FILE* _lpPacketData);
	void				operateLogoutResult(DGMSG_LOGOUTAVATAR* _lpPacketData);
	void				operateBankInfo(DWMSG_BANKOPEN *_lpPacket);

	void				operateProvisionRank(DGMSG_PROVISION_RANK_RET *_lpPacket);
	void				operateGetCarrotCount(DGMSG_GET_CARROT_COUNT_RESULT	*_lpPacket);
	void				operateUseCarrotResult(DGMSG_USE_CARROT_RESULT *_lpPacket);
	void				operateBankSaveResult(DWMSG_SAVEBANK* _lpPacketData);
	void				operatePremiumItemList(DGMSG_BUYLIST * _lpPacketData);
	void				operateWithdrawPremiumItemResult(DGMSG_USEBUY* _lpPacketData);
//┃DB 서버에서 온 패킷 처리
//┃CReceivePacketFromDB.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CReceivePartyPacketFromWorld.cpp
	void				operateJoinPartyApplication(WORLDtoSERVERS_JOIN_PARTY_APPLICATION *_lpPacket);
	void				operateJoinPartyApplicationAnswer(WORLDtoSERVERS_JOIN_PARTY_APPLICATION_ANSWER *_lpPacket);
	void				operateChangePartyStatus(WORLDtoSERVERS_CHANGE_PARTY_STATUS *_lpPacket);
	void				operateMoveFieldPartyMemberPacket(WORLDtoSERVERS_MOVE_FIELD_PARTY_MEMBER *_lpPacket);
	void				operateUpdatePartyInfoPacket(WORLDtoSERVERS_UPDATE_PARTYINFO *_lpPacket);
	void				operateUpdateUnionPartyInfoPacket(WORLDtoSERVERS_UPDATE_UNIONPARTYINFO *lpPacket);
	void				operateValidPartyPacket(WORLDtoSERVERS_VALID_PARTY *_lpPacket);
	void				operateFailedToCreatePartyPacket(WORLDtoSERVERS_FAILED_TO_CREATE_PARTY *_lpPacket);
	void				operateDissolutionPartyPacket(WORLDtoSERVERS_DISSOLUTION_PARTY *_lpPacket);
	void				operatePartyListPacket(WORLDtoSERVERS_PARTY_LIST	*_lpPacket);
	void				operateJoinPartyPacket(WORLDtoSERVERS_JOIN_PARTY	*_lpPacket);
	void				operatePartyWorkPacket(WORLDtoSERVERS_PARTY_WORK	*_lpPacket);
	void				operateDisconnectPartyMemberPacket(WORLDtoSERVERS_DISCONNECT_PARTY_MEMBER	*_lpPacket);
	void				operatePartyInfoPacket(WORLDtoSERVERS_UPDATE_PARTYINFO	* _lpPacket);
	void				operateChangePartyNamePacket(WORLDtoSERVERS_RENAME_PARTYNAME	*_lpPacket);
	void				operateChangePartyMemberInfoPacket(WORLDtoSERVERS_CHANGE_PARTY_MEMBER_INFO * _lpPacket);
	void				operatePartyChat(WORLDtoSERVERS_PARTY_CHAT	* _lpPacket);

	void				operateUpdateUnionPartyDissolutionPacket(SG_UNION_PARTY_WORK *_lpPacket);
	void				operateUpdateUnionPartyCreateResultPacket(SG_UNION_PARTY_WORK *_lpPacket);

	void				operateGuildMemberJoinParty(WORLDtoSERVERS_GUILD_MEMBER_JOIN_PARTY *_lpPacket);
	void				operateGuildMemberJoinPartyAnswer(WORLDtoSERVERS_GUILD_MEMBER_JOIN_PARTY_ANSWER *_lpPacket);
//┃CReceivePartyPacketFromWorld.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃CReceivePacketFromWorld.cpp
//┃월드 서버에서 온 패킷 처리
	CRITICAL_SECTION	m_csReceiveWorldPacket;	//	패킷 큐를 위한 메니져. -_-a
	int					m_iReceivedWorldPacketCount;
	DWORD				m_dwMaxWorldPacketSize;
	char				*m_apWorldPacketBuffer[dWORLD_PACKET_BUFFER_SIZE];

	void				initWorldPacketReceiver();
	void				closeWorldPacketReceiver();
	void				operateWorldPacket();
	void				addWorldPacket(char *_lpPacketData);

	void				operateChangeDay();
	
	void				operateSimpleWork(WORLDtoSERVERS_SIMPLE_WORK *_lpPacket);
	void				operateDuelRankerInfo(WORLDtoSERVERS_DUEL_RANKER_INFO *_lpPacket);
	void				operateDuelTeamInfo(WORLDtoSERVERS_DUEL_TEAM *_lpPacket);
	void				operateDuelRecord(WORLDtoSERVERS_DUEL_RECORD *_lpPacket);
	void				operateDuelMatchInfo(WORLDtoSERVERS_DUEL_MATCH_INFO *_lpPacket);
	void				operateTransAvatarToGVGResult(WORLDtoSERVERS_TRANS_AVATAR_TO_GVG_RESULT *_lpPacket);
	void				operateTransGuildToGVGResult(WORLDtoSERVERS_TRANS_GUILD_TO_GVG_RESULT *_lpPacket);
	void				operateW2G_GVG_AVATAR_ORG_DATA(W2G_GVG_AVATAR_ORG_DATA *_lpPacket);
	void				operateW2G_ASK_GVG_AVATAR_ORG_DATA_RESULT(W2G_ASK_GVG_AVATAR_ORG_DATA_RESULT *_lpPacket);
	void				operateBoostPowerByEvent(WORLDtoSERVERS_BOOST_POWER_BY_EVENT *_lpPacket);
	void				operateSoundOfLeadersBell(WORLDtoSERVERS_SOUND_OF_LEADERS_BELL *_lpPacket);
	void				operateAccountInfo(WORLDtoSERVERS_ACCOUNT_INFO *_lpPacket);
	void				operateDisconnectServerPacket(WORLDtoSERVERS_DISCONNECT_SERVER *_lpPacket);
	void				operateGRInfo(WORLDtoSERVERS_GR_INFO *_lpPacket);
	void				operateItemPackInfo(WORLDtoSERVERS_ITEM_PACK_INFO *_lpPacket);
	void				operateItemPackListSerial(WORLDtoSERVERS_ITEM_PACK_LIST_SERIAL *_lpPacket);
	void				operateResetDropPackItemCount(WORLDtoSERVERS_RESET_DROP_PACK_ITEM_COUNT *_lpPacket);
	void				operateUpdatePackItemCount(WORLDtoSERVERS_UPDATE_PACK_ITEM_COUNT *_lpPacket);
	void				operateCarrotShopInfo(WORLDtoSERVERS_CARROT_SHOP_INFO *_lpPacket);
	void				operateConfirmEnterField(WORLDtoSERVERS_CONFIRM_ENTER_FIELD *_lpPacket);

	void				operateCreateItemResultPacket(WORLDtoSERVERS_CREATE_ITEM_RESULT *_lpPacket);
	void				operateAddItemResultPacket(WORLDtoSERVERS_ADD_ITEM_RESULT *_lpPacket);
	void				operateUserFindResultPacket(WORLDtoSERVERS_USER_FIND *_lpPacket);
	void				operateUserRecallPacket(WORLDtoSERVERS_USER_RECALL* _lpPacket);
	void				operateReceiveItemUniqueSerial(WORLDtoSERVERS_ASK_ITEM_UNIQUE_SERIAL* _lpPacket);

	void				operateEventMessage(WORLDtoSERVERS_EVENT_MESSAGE* _lpPacket);
	void				operateResultIncreaseDBEventValue(WORLDtoSERVERS_RESULT_INCREASE_DB_EVENT_VALUE* _lpPacket);
	void				operateMoveFieldResult(WORLDtoSERVERS_USER_MOVEMAP* _lpPacket);
	void				operateCheckUserMoveMapResult(SERVERStoWORLD_USER_MOVEMAP* _lpPacket);
	void				operateWorldLogoutForMoveServerResult(WORLDtoSERVERS_USER_LOGOUT* _lpPacket);
	void				operateCreateGuildResult(WORLDtoSERVERS_USER_CREATEGUILD *_lpPacket);
	
	void				operateChangeGuildInventoryPermitWithdrawGold(WORLDtoSERVERS_CHANGE_GI_PERMIT_WITHDRAW_GOLD *_lpPacket);
	void				operateChangeGuildInventoryPermitWithdrawItem(WORLDtoSERVERS_CHANGE_GI_PERMIT_WITHDRAW_ITEM *_lpPacket);
	void				operateChangeGuildInventoryStorageSize(WORLDtoSERVERS_CHANGE_GI_STORAGE_SIZE *_lpPacket);

	void				operateRecordGuildHonorPoint(WORLDtoSERVERS_RECORD_GUILD_HONOR_POINT *_lpPacket);
	void				operateAddRelatedPlaceOfGuild(WORLDtoSERVERS_ADD_RELATED_PLACE_OF_GUILD *_lpPacket);
	void				operateStoreGuildInventoryItem(WORLDtoSERVERS_STORE_GUILD_INVENTORY_ITEM *_lpPacket);
	void				opreateWithdrawGuildInventoryItem(WORLDtoSERVERS_WITHDRAW_GUILD_INVENTORY_ITEM *_lpPacket);
	void				opreateStoreGuildInventoryGold(WORLDtoSERVERS_STORE_GUILD_INVENTORY_GOLD *_lpPacket);
	void				opreateWithdrawGuildInventoryGold(WORLDtoSERVERS_WITHDRAW_GUILD_INVENTORY_GOLD *_lpPacket);
	void				opreateChangeGuildInventoryItemPlace(WORLDtoSERVERS_CHANGE_GUILD_INVENTORY_ITEM_PLACE *_lpPacket);

	void				operateSetGuildHallRoenGoodWill(WORLDtoSERVERS_SET_GUILD_HALL_ROEN_GOOD_WILL *_lpPacket);

	void				operateSetGuildHallLevel(WORLDtoSERVERS_SET_GUILD_HALL_LEVEL *_lpPacket);
	void				operateReceiveGuildList(WORLDtoSERVERS_USER_LISTGUILD *_lpPacket);
	void				operateReceiveGuildMemberList(WORLDtoSERVERS_USER_GUILDINFO* _lpPacket);
	void				operateFinishReceiveGuildInfo(WORLDtoSERVERS_LOGIN* _lpPacket);
	void				operateReceiveRenewPremiumItemInfo(WORLDtoSERVERS_RENEW_PREMIUM_ITEM* _lpPacket);

	void				operateCreateGuild(WORLDtoSERVERS_GUILD_CREATE* _lpPacket);
	void				operateRemoveGuild(WORLDtoSERVERS_DELETEGUILD* _lpPacket);
	void				operateAddGuildMember(WORLDtoSERVERS_GUILD_JOIN* _lpPacket);
	void				operateRemoveGuildMember(WORLDtoSERVERS_GUILD_EXIT* _lpPacket);
	void				operateJoinGuildResult(WORLDtoSERVERS_USER_JOINGUILD* _lpPacket);
	void				operateUpdateGuildExp(WORLDtoSERVERS_GUILD_UPDATE_EXP* _lpPacket);

	void				operateUpdateGuildNotice(WORLDtoSERVERS_GUILD_NOTICE* _lpPacket);
	void				operateChangeGuildExpTaxRate(WORLDtoSERVERS_GUILD_EXPRATE* _lpPacket);
	void				operateUpdateGuildHallInfo(WORLDtoSERVERS_UPDATE_GUILD_HALL_INFO *_lpPacket);	
	void				operateChangeRankInGuild(WORLDtoSERVERS_USER_CHANGERANK* _lpPacket);
	void				operateLeaveGuild(WORLDtoSERVERS_USER_EXITGUILD* _lpPacket);
	void				operateChangeRankResult(WORLDtoSERVERS_USER_CHANGERANK_RESULT* _lpPacket);
	void				operateUpdateGuildSkillLevel(WORLDtoSERVERS_UPDATE_GUILD_SKILL_LEVEL * _lpPacket);//길드 스킬레벨 업데이트	
	void				operateGuildSkillLevelIncreaseFailure(WORLDtoSERVERS_GUILD_SKILL_LEVEL_INCREASE_FAILURE * _lpPacket);//길드 스킬레벨 증가 실패
	
	void				receiveAppointmentQuizEvent(WORLDtoSERVERS_APPOINTMENT_QUIZ_EVENT	*_lpPacket);
	void				receiveWorldValueInfoAll(WORLDtoSERVERS_WORLD_VALUE_INFO_ALL	*_lpPacket);
	void				receiveWorldValueInfo(WORLDtoSERVERS_WORLD_VALUE_INFO	*_lpPacket);
	void				receiveEndQuizEvent(WORLDtoSERVERS_END_QUIZ_EVENT *_lpPacket);
	void				receiveEventWordInfo(WORLDtoSERVER_EVENT_WORD_INFO *_lpPacket);
	void				receiveCancelQuizEvent(WORLDtoSERVERS_CANCEL_QUIZ_EVENT *_lpPacket);
	void				receiveProduceOxQuiz(WORLDtoSERVERS_PRODUCE_OX_QUIZ *_lpPacket);
	void				receiveLottoEventData(WORLDtoSERVERS_LOTTO_EVENT_DATA *_lpPacket);
	void				receiveEventTimeData(SERVERStoWORLD_MODIFY_EVENT_TIME	*_lpPacket);
	void				receiveOXQuizLoserReviveBattle();	//	OX 퀴즈 패자 부활전

	void				receiveChangeWordQuizRound(WORLDtoSERVERS_CHANGE_WORD_QUIZ_ROUND *_lpPacket);
	void				receiveProduceWordQuiz(WORLDtoSERVERS_PRODUCE_WORD_QUIZ *_lpPacket);

	void				receiveCalling(WORLDtoSERVERS_CALLING* _lpPacket);
	void				receiveBeginTestGuildBattle(WORLDtoSERVERS_BEGIN_TEST_GUILD_BATTLE* _lpPacket);

	void				receiveRemoveFriendInTargetList(WORLDtoSERVERS_REMOVE_FRIEND_IN_TARGET_LIST* _lpPacket);
	void				receiveRemoveFriendInTargetListResult(WORLDtoSERVERS_REMOVE_FRIEND_IN_TARGET_LIST_RESULT* _lpPacket);

	void				receiveGuildBattleSchedule(WORLDtoSERVERS_GUILD_BATTLE_SCHEDULE* _lpPacket);	//	길드전 스케쥴을 모두 받았다.	
	void				receiveGuildBattleInfo(WORLDtoSERVERS_UPDATE_GUILD_BATTLE_INFO* _lpPacket);	//	길드전 정보를 받았다.
	void				receiveGuildInfo(WORLDtoSERVERS_UPDATE_GUILD_INFO* _lpPacket);	//	길드 정보 업데이트
	void				receiveValidGuild(WORLDtoSERVERS_VALID_GUILD* _lpPacket);
	void				receiveGuildBattleResult(WORLDtoSERVERS_GUILD_BATTLE_RESULT * _lpPacket);	//	길드전 결과 받음
	void				receiveSelectGuildMark(WORLDtoSERVERS_SELECT_GUILD_MARK *_lpPacket);	//	길드 마크 선택
	void				receiveUpdateGuildMemberLevel(WORLDtoSERVERS_UPDATE_GUILD_MEMBER_LEVEL* _lpPacket);

	void				operateSayResult(WORLDtoSERVERS_USER_SAY_RESULT *_lpPacket);
	void				operateSayMessage(WORLDtoSERVERS_USER_SAY *_lpPacket);
	void				operateIsDenyCommunityTarget(WORLDtoSERVERS_IS_DENY_COMMUNITY_TARGET *_lpPacket);
	void				operateCreateShopItemResult(WORLDtoSERVERS_CREATE_SELL_ITEM_RESULT *_lpPacket);


//┃월드 서버에서 온 패킷 처리
//┃CReceivePacketFromWorld.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃ACTOR 관리
//┃CGameActor.cpp
	int					bookingJoinPlayer(CPlayerSaveData *_lpPlayerData,CUpkeepPlayerData *_lpUpkeepData,cActorStatusUpkeep *_lpUpkeepStatus,int _iClientSerial,BOOL _bIsFirstConnect);	//	플레이어 참가 예약

	cACTOR*				getFreeActor();				//	빈 자리 찾기
	BOOL				removeActor(int _iSerial);	//	시리얼
	cACTOR*				getPlayer(char *_lpstrName);
	cACTOR*				getPlayerByID(char *_lpstrID);
	
	inline	cACTOR*		getActor(int _iIndex)
	{
		if (_iIndex >= 0 && _iIndex < m_iMaxActorCount	)
		{
			if (m_pActor[_iIndex].m_iZoneSerial	!=	_iIndex	)	return	NULL;

			return	&m_pActor[_iIndex];
		}

		return	NULL;
	}
//┃CGameActor.cpp
//┃ACTOR 관리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃ACTOR 행동
	BOOL				moveTo(int _iX,int _iY,int _iDestX,int _iDestY,int _iSerial);
	BOOL				stop(int _iX,int _iY,int _iSerial);
//┃ACTOR 행동
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃필드 관리
	inline	cFIELD*		getField(char *_strMapName)	//_strMapName이란 이름을 가진 필드를 얻는다.
	{
		for	(int i=0;i<m_iFieldCount;i++)
			if	(STRICMP(m_pField[i].m_strFileName,_strMapName)==0)
				return	&m_pField[i];

		return	NULL;
	}
	
	inline	cFIELD*		getOXQuizField()	//_strMapName이란 이름을 가진 필드를 얻는다.
	{
		return	getFieldByUniqueSerial(m_iOXQuizField);
	}
	inline	cFIELD*		getWordQuizField()
	{
		return	getFieldByUniqueSerial(m_iWordQuizField);
	}
	
	int					findEmptyDuelField(int _iDuelMatchType);
	cFIELD*				getField(int _iIndex);	//_iIndex번째 필드를 리턴한다.
	cFIELD*				getFieldByUniqueSerial(int _iSerial,DWORD _dwPartyUniqueSerial=0xffffffff,int _iInstanceFieldSerial=-1,int _iInstanceFieldFloor=-1,BOOL _bIsForLoadGuildHall=FALSE,int _iCreateTimeValue=-1);//고유시리얼로 필드를 얻는다.
//┃필드 관리
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃폴더 이동
	BOOL				setHeroBodyFolder();
	BOOL				setEffectDataFolder();
	BOOL				setMonsterBodyFolder();
	BOOL				setNpcBodyFolder();
	BOOL				setMapFolder();
//┃폴더 이동
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	static	void		EmergencyWork(void *lpInfo);
};

extern	cGAME		g_game;
extern	CTimeInfo	g_currentTime;
extern	int			g_iCurrentTimeMinuteValue,g_iCurrentTimeSecondValue;

extern	int			g_iExperienceFactor;	//	경험치 팩터
extern	int			g_iItemFactor;	//	경험치 팩터
extern	BOOL		g_bIsOpenedTestServer;
extern	BOOL		g_bIsUseRealPrice;
extern	BOOL		g_bIsDuelServer;
extern	int			g_iServerIndex;
extern	int			g_iWorldServerIndex;
extern	int			g_iWorldServerType;
extern	BOOL		g_bIsChangeMinute,g_bIsChangeHour,g_bIsChangeDay;
extern	BOOL		g_bIsWantPrintTriggerCondition;
extern	DWORD		g_dwCurrentTime;
extern	int			g_iSeasonVariable;

#define dSEASON_VARIABLE_PVP_JOB_FACTOR 									1		//결투장 직업 보정 
#define dSEASON_VARIABLE_PVP_REFORM 										1		//결투장 세부 규칙 변경(운굴림,이속 증가 등)
#define dSEASON_VARIABLE_MINIPET_POWER_UP_2 								1		//미니펫 강화 2차
#define dSEASON_VARIABLE_GUILD_REFORM										3		//길드 개편
#define dSEASON_VARIABLE_PARTY_UNION										100		//파티 연합

extern	float		g_afDuelDamageFactor[dPLAYER_JOB_COUNT + 1];

extern	int			g_iLastTimeByTimeGetTime;
extern	int			g_iCounterBySecond,g_iCurrentMoveNumber;
extern	BOOL		g_bIsDisplayTick;
extern	char		g_strServerInfoFileName[64];
extern	int			g_iSpecialMagicOptionDropChance;
extern	BOOL		g_bGameClose;
extern	BOOL		g_bIsUseHackShield;


extern	int			g_iXtrapMapFileCount;
extern	char		*g_pXtrapMapFile;

extern	bool		BeginXtrap(int _iCount);
extern	void		EndXtrap();

extern	LONGLONG	g_allGettingGold[eGET_GOLD_TYPE_COUNT],g_allSpentGold[eSPENT_GOLD_TYPE_COUNT];

#endif