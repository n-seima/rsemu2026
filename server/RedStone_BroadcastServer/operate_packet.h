#ifndef	_operate_packet_h
#define	_operate_packet_h

#include	"Client.h"
#include	"cPACKET_GAMESERVER.h"
#include	"cPACKET_BCSERVER.h"
#include	"cSOCKET.h"

class	cBinaryLog
{
	char	m_strLogName[512];
	char	m_strLogFileName[512];
	int		m_iLogFileCounter;
	int		m_iMaxLogFileSize;
	UINT	m_uiCurrentFilePoint;

	CRITICAL_SECTION	m_cs;
	cFILE	m_file;

public:

	cBinaryLog();
	~cBinaryLog();
	BOOL	init(char *_lpstrFileName,int _iMaxSize=4096*1000);
	BOOL	add(void *_lpData,int _iSize);
};

extern cBinaryLog	g_binaryLog;

extern cSOCKET	* g_cpAvatarDBC;

//	클라이언트에서 받은 패킷들 처리
namespace	nsReceiveFromClient
{
void	LogInClient(CClient *_lpClient,CB_LOGIN *_lpPacket);	//	로그인
void	UpdateUserAllInfoPacket(CClient *_lpClient,CB_UPDATE_USER_ALLINFO *_lpPacket);		//	모든 정보 변경
void	ChangeUserSkillSetting(CClient *_lpClient,CB_CHANGE_USER_SKILL_SETTING *_lpPacket);	//	스킬 설정 변경
void	ChangeFriendGroupName(CClient *_lpClient,CB_CHANGE_FRIEND_GROUP_NAME *_lpPacket);	//	친구 그룹 이름 변경
void	RemoveFriend(CClient *_lpClient,CB_REMOVE_FRIEND *_lpPacket);	//	친구 제거
void	ChangeFriendGroup(CClient *_lpClient,CB_CHANGE_FRIEND_GROUP	*_lpPacket);	//	친구 그룹 변경

void	PacketDeliver(CClient *_lpClient,GB_PACKET_DELIVER *_lpPacket);
void	PartyPacketDeliver(CClient *_lpClient,GB_PARTY_PACKET_DELIVER *_lpPacket);
void	GuildChat(CClient *_lpClient,GB_GUILD_CHAT *_lpPacket);	//	길드 채팅
void	PartyChat(CClient *_lpClient,GB_PARTY_CHAT *_lpPacket);	//	길드 채팅
void	AddFriend(CClient *_lpClient,GB_ADD_FRIEND *_lpPacket);	//	친구 추가
void	ForcedRemoveFriend(CClient *_lpClient,GB_FORCED_REMOVE_FRIEND *_lpPacket);	//	강제로 친구 목록에서 친구 제거
void	AskItemInfo(CClient *_lpClient,GB_ASK_ITEM_INFO *_lpPacket);
void	UpdateInfo(CClient *_lpClient,GB_UPDATE_INFO *_lpPacket);	//	정보 업데이트

void	AskOpenPartyList(CClient *_lpClient,CB_ASK_OPEN_PARTY_LIST *_lpPacket);
void	AskPartyProstpectus(CClient *_lpClient,CB_ASK_PARTY_PROSPECTUS *_lpPacket);
void	ChangePartyProstpectus(CClient *_lpClient,CB_CHANGE_PARTY_PROSPECTUS *_lpPacket);
void	OperateGuildWorkBus(CClient *_lpClient,GB_GUILD_WORK_BUS *_lpPacket);
};

namespace	nsReceiveFromWorld
{
void	UpdatePartyList(WB_PARTY_LIST *_lpPacket);
void	UpdatePartyInfo(WB_UPDATE_PARTY_INFO *_lpPacket);
};

namespace	nsSendToWorld
{
void	AskPartyList();
void	AskPartyInfo(int _iPartySerial);
};

//	DB로 패킷 전송
namespace	nsSendToDB
{
void	SavePersonalData(CClient *_lpClient, BOOL _isSave = 0);
};

//	클라이언트로 패킷 전송
namespace	nsSendToClient
{
void	ChangeFriendOnlineStatus(CClient *_lpClient);	//	친구 목록의 온라인 상태 갱신
void	SendGuildChat(CClient *_lpClient,char *_lpstrMessage);	//	길드 채팅 전송
void	SendPartyChat(CClient *_lpClient,char *_lpstrMessage);
void	SendMessage(CClient *_lpClient,int _iMessage);
void	SendConnectionInfoToMyFriends(CClient* _lpClient, BOOL _bConnection);		// 내 친구들에게 나의 접속정보를 보낸다.
};


namespace	nsSendToGS
{
void	AskClientInfo(CClient *_lpServer,int _iSerial);
};

extern	void	FillChattingBuffer(SG_CHAT *_lpstrPacket,char *_lpstrName,char *_lpstrMessage);
extern	void	CheckString(char * _str,int _size, char * _where,int _line);	//	지정된 string이 정상적인지를 확인합니다. 

#endif