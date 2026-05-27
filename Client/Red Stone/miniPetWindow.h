#ifndef	_classMiniPetWindow_h
#define	_classMiniPetWindow_h

#include "CGamePlay.H"
#include "miniPet.H"

class	cMiniPetStatus	:	public CGamePlay
{
public:
	int				m_iFocusMiniPet;
	int				m_iSelectMiniPet,m_iFocusSkill;
	int				m_aiMultiSelect[c_iMiniPetCount];
	int				m_iSelectMinipetCount;
	int				m_iFeedItem;
	int				m_iFeedType;

	BOOL			m_bIsChangeSkillWork,m_bIsWaitResult;
	cMiniPet		*m_lpSelectMiniPet;

	cRECT			m_rect;
	cRECT			m_rectCombiSkill,m_rectReversibleFrame;

	BOOL			isIncludeActiveMiniPetInSelectedList();
	BOOL			addSelectMiniPet(int _iIndex);
	int				getFirstSelectMiniPet();
	BOOL			isSelectMiniPet(int _iIndex);
	BOOL			selectMiniPet(int _iIndex,BOOL _bIsAdd=FALSE);

	void			open();
	void			close();
	void			update();
	void			modifyInfo();
	void			receiveResult()	//	서버로 보낸 작푳E요청의 결과를 받았다.
	{
		m_bIsWaitResult	=	FALSE;
	}

	void			draw();

	int				getFocusMiniPet();
	int				getFocusSkill();
	void			resetButtons();
	BOOL			isNeedOnOffButtonSkill(CSkill* _lpSkill);
	void			resetButtonsPos();
	void			feedItem();
};

extern	cMiniPetStatus	g_gwMiniPetStatus;

#endif
