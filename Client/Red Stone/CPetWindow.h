#ifndef _classPetWindow_H_
#define	_classPetWindow_H_

#include "CGamePlay.H"

class	CPetWindow	:	public CGamePlay
{
public:
	CPos			m_posPetWindow;
	WORD			m_wSelectPetIndex;	//	선택한 직업 탭
	WORD			m_wSelectSummonBeast;	//	선택한 직업 탭
	int				m_iResetPetNameItemIndex;

					CPetWindow();
					
	void			open();//	열기
	void			close();//	닫기
	void			draw();	//	그리기
	void			update();//	업데이트
};

extern	CPetWindow	g_gwPet;

#endif