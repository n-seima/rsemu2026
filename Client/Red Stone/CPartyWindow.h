#ifndef _classPartyWindow_H_
#define	_classPartyWindow_H_

#include "CGamePlay.H"

enum
{
	ePBMS_NOTHING,
	ePBMS_PARTY_LIST,
	ePBMS_PARTY_INFO,
	ePBMS_PARTY_UNION,
};

enum
{
	ePBM_LEAVE_PARTY,
	ePBM_ASK_JOIN_PARTY,
	ePBM_ASK_BANISH_MEMBER,
	ePBM_ASK_NEXT_PARTY_LIST,
	ePBM_ASK_PREVIOUS_PARTY_LIST,
	ePBM_CHANGE_PARTY_LEADER,
	ePBM_SHOW_PARTY_INFO,
	ePBM_ASK_PARTY_UNION,
};
enum
{
	ePS_PARTY_STATUS,
	ePS_GOLD_SHARING_METHOD,
	ePS_ITEM_SHARING_METHOD,
	ePS_PARTY_OBJECT,
	ePS_CHANGE_RECRUIT_PARTY_MEMBER_LEVEL_RANGE,
	ePS_CHANGE_PARTY_PROSPECTUS_TEXT,				//	파티 모집 요강 변경
	ePS_PARTY_PREFERENCE_JOB_CHECK_BOX	=	100,
};

class	CPartyWindow	:	public CGamePlay
{
public:
	CPos			m_posPartyWindow;
	cBARMENU		m_bmPartyMenuButton;
	int				m_iPartyBarmenuStatus;
	int				m_iLastReceiveParty;

	int				m_iSkillWindowScrollBarPos;	//	기억해 뒀다가 다시 팝업 뜨면 설정한다.
	CPos			m_posSkillWindow;
	cBARMENU		m_bmReactionSkillState,m_bmJobTab;
	int				m_iFocusPartyStatus;	//	선택한 직업 탭
	BOOL			m_bIsOpenOld;

					CPartyWindow();

	void			open();//	열기
	void			close();	//	파티창 닫기
	void			drawMemberName(int _iX,int _iY,int _iMemberIndex,DWORD _dwTextColor);	//	캐릭터 이름 출력
	void			draw();	//	파티창 그리기
	void			drawPartyInfo();	//	파티창 그리기
	void			updateBarbutton();	//	버턴 업데이트.
	void			update();//	파티창 업데이터
	void			updateBarmenu();
	BOOL			isIn();
	BOOL			isInRequestJoinPartyUserList();

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃requestJoinPartyUserListWindow.cpp
	void			drawRequestJoinPartyUserList();
	void			updateRequestJoinPartyUserList();
//┃requestJoinPartyUserListWindow.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃partyListWindow.cpp
	void			receivePartyList(int _iPartyCount,cPartyInfoForPL *_lpList);
	int				getFocusParty();
	void			drawPartyList();
//┃partyListWindow.cpp
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
};

extern	CPartyWindow	g_gwParty;

#endif