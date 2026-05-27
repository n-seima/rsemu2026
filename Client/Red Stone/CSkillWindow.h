#ifndef _classSkillWindow_H_
#define	_classSkillWindow_H_

#include "CGamePlay.H"

class	CSkillWindow	:	public CGamePlay
{
public:
	int				m_iSkillWindowScrollBarPos;	//	기억해 뒀다가 다시 팝업 뜨면 설정한다.
	CPos			m_posSkillWindow;
	cBARMENU		m_bmReactionSkillState,m_bmJobTab;
	int				m_iSelectJobTab;	//	선택한 직업 탭
	int				m_iLastActionTime;

					CSkillWindow();
					
	void			init();//	초기화
	void			resetButtons();//스킬 윈도우의 버튼들을 초기화
	void			open();
	void			close();//스킬 창 닫기
	int				getSkillDroppingSlot();//스킬이 슬롯
	inline	int		getFocusSkill();//포커스 스킬 구하기
	inline	void	operateScrollBar();	//	스킬 윈도우 스크롤바 처리
	inline	void	operateSkillPoint();//	스킬 포인트 처리
	void			operateDragSkill();	//	드래그 스킬 처리
	void			resetButtonsPos();
	void			operatePopupMenu();
	void			update();//스킬 인터페이스 업데이트

	void			draw();// 그리기
	int			getSkillItemtoEquipmentIndex(int _type);		// 10.01.07  아이템의 타입에 대한 장비창 인덱스 리턴
};

extern	CSkillWindow	g_gwSkill;

#define	dBORDER_OF_BOTTOM_SKILL	1000

#endif