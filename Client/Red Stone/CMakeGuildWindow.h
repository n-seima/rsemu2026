#ifndef _classMakeGuildWindow_H_
#define	_classMakeGuildWindow_H_

#include "CGamePlay.H"

class	CMakeGuildWindow	:	public CGamePlay
{
public:
	char		m_strGuildName[dGUILD_NAME_LENGTH];
	char		m_strMemberList[dPARTY_MAXPLAYER][dNAME_LENGTH];
	int			m_iFocusMember,m_iSelectSubMaster;	//	기억해 뒀다가 다시 팝업 뜨면 설정한다.

	cTALK		m_inputGuildName;

	CPos		m_pos;

	void		open(char *_lpstrMemberList);	//	열기

	void		close();//스킬 창 닫기
	void		draw();//아이템 인터페이스 그리기
	void		update();//스킬 인터페이스 업데이트
	int			getFocusedMember();
	BOOL		isOpened();
};

extern	CMakeGuildWindow	g_gwMakeGuild;

#endif