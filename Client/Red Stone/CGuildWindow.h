#ifndef _classGuildWindow_H_
#define	_classGuildWindow_H_

#include "CGamePlay.H"
#include "CGuild.H"

class	CGuildWindow	:	public CGamePlay
{
public:
	CPos		m_pos;
	cBARMENU	m_bmTab,m_bmMenu;
	int			m_iSelectTab,m_iFocusedSkill,m_iFocusedMember,m_iSelectMember;
	int			m_iGuildSkillWindowScrollBarPos,m_iGuildMemberListWindowScrollBarPos;
	char		m_strJoinGuildAsker[dNAME_LENGTH];
	WORD		m_wJoinGuildAskerSerial;

	void		open();		//열기
	void		close();	//창 닫기
	void		changeTab(int _iTab=-1,BOOL _bIsCheckSameWork=TRUE);
	void		reset();
	
	void		resetSkillPlusButtons();	//	스킬 업 버튼들 초기화
	void		resetSkillPlusButtonsPos();	//	스킬 업 버튼들 위치 조절

	void		draw();		//그리기
	void		drawGuildInfo();		//	길드 정보 출력
	void		drawGuildSkill();		//	길드 스킬
	void		drawGuildItem();		//	길드 아이템
	void		drawGuildManageTool();	//	길드 관리 툴
	void		drawGuildMemberList();	//	길드 멤버 리스트

	int			getFocusedSkill();
	int			getFocusedMember();
	void		selectMemberListPopupMenu(int _iMenu);
	void		operateGuildMemberListPopupMenu();

	void		update();	//업데이트
	void		updateGuildInfo();
	void		updateGuildSkill();
	void		updateGuildItem();
	void		updateMemberList();
	void		updateManageTool();

	void		askJoinGuild(char *_lpAskerName,int _iAskerSerial,int _iLevel,int _iJob);

};

extern	CGuildWindow	g_gwGuild;

#endif