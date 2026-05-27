#ifndef _classPartyManager_H
#define	_classPartyManager_H

#include	"SFC.H"
#include	"definePacketData.H"
#include	"partyDefine.H"

class	cPartyInfo	:	public cPartyInfoForBCS
{
public:
	WORD				m_wActiveMemberCount;
	WORD				m_wRecruitMemberLevelRange;
	DWORD				m_dwPreferenceJobMask;	// 직업추가로 WORD -> DWORD 전환
	char				m_strPartyProspectus[c_iPartyProspectusLength];
	WORD				m_awMemberSerial[dPARTY_MEMBER_COUNT];

						cPartyInfo()
						{
							reset();
						}

	BOOL				addMember(int _iSerial);
	void				removeMember(int _iSerial);
	void				reset();
	void				copy(cPartyInfo *_lpInfo);
	void				copyAdvancedInfo(cPartyInfo *_lpInfo);
	void				resetAdvancedInfo();

	void				changePartyProspectusText(char *_lpstrObject);
	BOOL				isPartyLeader(char *_lpstrName);
	BOOL				isJoinAbleLevelParty(int _iLevel);
	void				checkInvalidMember();

	void				getInfoForPL(cPartyInfoForPL *_lpInfo);
	void				sendPacketToClient(void *_lpClient,int _iFieldSerial=0xffff);
};

class	cPartyManager
{
public:
	static	BOOL		s_bIsExist;

	BOOL				m_bIsReceivePartyInfo;
	BOOL				m_bIsProcessUpdataPartyList;

	cDen<cPartyInfo>	m_denParty;
	CRITICAL_SECTION	m_csPartyWork;

						cPartyManager();
						~cPartyManager();

	cPartyInfo*			get(int _iSerial);
	void				reset();
	void				beginReceivePartyList();	//	파티 리스트 전부를 받기 시작
	void				endReceivePartyList();		//	파티 리스트 전부를 받기 끝
	void				updatePartyInfo(cPartyInfo *_lpInfo,BOOL _bIsByUpdateListPacket=FALSE);

	BOOL				addPartyMember(int _iParty,void *_lpClient);
	void				removePartyMember(int _iParty,int _iClient);
};

extern	cPartyManager	g_partyManager;

#endif