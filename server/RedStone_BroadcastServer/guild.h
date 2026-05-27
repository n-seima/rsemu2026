#ifndef	_class_community_server_guild_h
#define	_class_community_server_guild_h

#include	"SFC.H"
#include	"definePacketData.H"
#include	"CGuildDefine.H"


class	cGuildInfo
{
public:
	WORD				m_wSerial;
	WORD				m_wActiveMemberCount;
	WORD				m_awMemberSerial[dGUILD_MAX_PLAYER];

	BOOL				m_bIsChangeConnetState;
	BYTE				m_bMemberDisplayIndex[dGUILD_MAX_PLAYER];
	BYTE				m_bMemberConnetionState[dGUILD_MAX_PLAYER];
	DWORD				m_dwGuildMemberListUpdateGap;	//	X ms초마다 갱신한다.	길드멤버 
						cGuildInfo()
						{
							m_dwGuildMemberListUpdateGap = 0;
							reset();
						}

	BOOL				addMember(int _iSerial , int _iDisplayIndex);
	void				removeMember(int _iSerial);

	void				reset();
	void				copy(cGuildInfo *_lpInfo);

	void				sendGuildMemberConnectionList();
	void				sendPacketToClient(void *_lpPacket);
	BOOL				changeConnetionState(int _iIndex , BYTE _ConnetionState);
};

class	cGuildManager
{
public:
	static	BOOL		s_bIsExist;

	cDen<cGuildInfo>	m_denGuild;
	CRITICAL_SECTION	m_csGuildWork;

						cGuildManager();
						~cGuildManager();

	cGuildInfo*			get(int _iSerial);
	void				reset();

	void				addGuildMember(int _iGuild,int _iClient,int _GuildDisplayIndex);
	void				removeGuildMember(int _iGuild,int _iClient);
	void				sendPacketToClient(int _iGuild,void *_lpPacket);
	BYTE*				getGuildMemberOnlineStateArry(int _iGuild);
};

extern	cGuildManager	g_guildManager;


#endif