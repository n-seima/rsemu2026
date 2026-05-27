#ifndef _classFriendWindow_H_
#define	_classFriendWindow_H_

#include "CGamePlay.H"

class	cFriendWindow	:	public CGamePlay
{
public:
	int				m_iDisplayFriendCount,m_iFriendWindowScrollBarPos,m_iSelectFriend,m_iFocusFriend;	//	기억해 뒀다가 다시 팝업 뜨면 설정한다.
	int				m_iFocusedGroup,m_iSelectedGroup;
	CPos			m_posFriendWindow;
	char			m_strAddFriendAskerName[dNAME_LENGTH];
	char			m_strSelectFriendName[dNAME_LENGTH];
	char			m_strSelectGroupName[dNAME_LENGTH];
	BYTE			m_bISarrtyConnectionList[c_iMaxGuildMemberCount];

	void			open();//아이템 & 스테이터스 인터페이스 열기
	void			close();//스킬 창 닫기
	void			update();//스킬 인터페이스 업데이트
	void			operatePopupMenu();	//	팝업 메뉴 처리
	void			recoverOldFriendList();

	void			draw();//아이템 인터페이스 그리기

	void			drawFriendList();//	친구 리스트 그리기

	void			getFocusedFriend(int	*_lpiFocusedFriend,int *_lpiFocusedGroup);
	int				getLineCount();

	void			resetFriendList();	//	친구 리스트 초기화
	void			askAddFriend(char *_lpstrAsker);	//	친구 추가 요청
	void			askRemoveFriend(char *_lpstrAsker);	//	친구 제거 요청
};

extern	cFriendWindow	g_gwFriend;

#endif