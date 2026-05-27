#ifndef _classEasySkillTree_H_
#define	_classEasySkillTree_H_

#include "CGamePlay.H"

class	cEasySkillTree	:	public CGamePlay
{
public:
	int				m_iObjectAbility;
	int				m_iSkillWindowScrollBarPos;	//	기억해 뒀다가 다시 팝업 뜨면 설정한다.
	CPos			m_pos;
	cBARMENU		m_bmPlus;
	int				m_iSelectJobTab;	//	선택한 직업 탭
	int				m_iLastActionTime;
	int				m_iFocusSkill,m_iRequireAbilityCount,m_iBookedAbility;
	cAbility		m_aRequireAbility[dABILITY_COUNT];

					cEasySkillTree();
					
	void			init();//	초기화
	void			resetButtons();//스킬 윈도우의 버튼들을 초기화
	void			open(int _iObjectAbility=-1);
	void			close();//스킬 창 닫기
	int				getFocusAbility();//	스킬 포인트 처리
	void			operateSkillPoint();//	스킬 포인트 처리
	void			resetButtonsPos();
	void			update();//스킬 인터페이스 업데이트
	void			getRequireAbility(int _iAbility);
	void			addRequireAbility(int _iAbility,int _iLevel);
	int				getRequireAbilityLevel(int _iAbility);

	void			draw();// 그리기
};

extern	cEasySkillTree	g_gwEasySkillTree;

#define	dBORDER_OF_BOTTOM_SKILL	1000

#endif