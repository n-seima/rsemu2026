#ifdef _DEBUG

#ifndef _classDebugWindow_H_
#define	_classDebugWindow_H_

#include "CGamePlay.H"

#define	dMAX_DEBUG_CONTENTS_LIST_COUNT	2048

enum
{
	eOCM_MOVE_FIELD		,
	eOCM_RECALL			,
	eOCM_WARP			,
	eOCM_BANISH			,
	eOCM_JAIL			,
	eOCM_DRAWING_ROOM	,
	eOCM_CONTROL_MONSTER,
	eOCM_SHOW_TILE_INFO	,
	eOCM_CHANGE_LEVEL	,
	eOCM_CHANGE_OWN_GOLD,
	eOCM_RECOVE_HP_CP	,
	eOCM_CHANGE_LOCATE	,
	eOCM_TRACE_ACTOR	,
	eOCM_COSTUME		,
	eOCM_REGEN_FIELD_MOB,
	eOCM_SPRINKLE_GIFT	,
	eOCM_HIDING			,
	eOCM_HIDE_NAME_AND_HIDE_DAMAGE,
	eOCM_ASK_ITEM_DATA,
	eOCM_MOVIE_MODE,
	eOCM_SUMMON_EVENT_MONSTER,
	eOCM_RECALL_MONSTER,

};

class	CDebugWindow:	public CGamePlay
{
public:
	class	cContentsInfo
	{
	public:
		WORD		m_wSerial;
		WORD		m_wValue;
		DWORD		m_dwColor;
		char		m_strText[128];

		void		copy(cContentsInfo*);
		void		reset();
	};

	int					m_iFocusedLine,m_iSelectContents;
	int					m_iMenuStep,m_iFieldCount;	//	기억해 뒀다가 다시 팝업 뜨면 설정한다.
	int					m_iDisplayContentsCount,m_iListScrollBarPos;
	int					m_iSelectCostume;
	cRECT				m_rect;
	cBARMENU			m_bmMain,m_bmSub;
	cDen<cContentsInfo>	m_den;

	void			open();
	void			close();

	void			changeTab(int _iTab);

	void			update();
	void			updateMainTab();
	void			updateFieldListTab();

	void			draw();		//	스테이터스 윈도우 그려주기
	void			drawMainTab();
	void			drawListTab();

	int				getFocusedContents();


	BOOL			isOpened();
};

extern	CDebugWindow	g_gwDebug;
extern	BOOL			g_bIsHideDamageAndHideName;
extern	BOOL			g_bIsMovieMode;	//	동영상촬영 모드
#endif

#endif