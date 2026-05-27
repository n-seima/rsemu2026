#ifndef _classGuildMarkComposer_H_
#define	_classGuildMarkComposer_H_

#include "CGamePlay.H"
#include "CGuild.H"

//
//	Guild Mark Compower Work Tab
enum
{
	eGMCWT_SELECT_ICON,
	eGMCWT_SELECT_ICON_COLOR,
	eGMCWT_SELECT_BASE_SHAPE,
	eGMCWT_SELECT_BASE_COLOR_PARTITION,
	eGMCWT_SELECT_BASE_COLOR,
	eGMCWT_SELECT_BASE_OUTLINE_COLOR,
};

class	CGuildMarkComposer :	public CGamePlay
{
public:
	CPos					m_pos;
	cBARMENU				m_bmButton;
	int						m_iSelectWorkTab,m_iFocusWorkTab,m_iSelectIconShapePart;
	CGuildMarkInfo			m_guildMarkInfo;
	int						m_iFocusIcon,m_iFocusIconColorPattern,m_iFocusBaseShape,m_iFocusBaseColorPartition,m_iFocusBaseColorPattern,m_iFocusBaseOutLineColorPattern;

							CGuildMarkComposer();

	BOOL					isOpened();	//	열렸냐?

	void					open();		//	열기
	void					update();	//	업데이트
	void					close();	//창 닫기

	void					draw();		//그리기
	void					drawMarkIconList();	//	아이콘 리스트
	void					drawMarkIconColorPatternList();	//	아이콘 컬러 패턴 리스트
	void					drawMarkBaseShapeList();	//	베이스 모양 리스트
	void					drawMarkBaseShapeColorPartitionList();	//	베이스 모양 컬러 분할형태 리스트
	void					drawMarkBaseShapeColorPatternList();	//	베이스 모양 컬러 패턴 리스트
	void					drawMarkBaseShapeOutLineColorPatternList();	//	베이스 모양 외곽선 컬러 패턴 리스트

	int						getFocusWorkTab();	//	현재 포커스가 있는 워크 탭
	int						getFocusIconShapeTab();	//	
	int						getFocusIcon();	//	포커스 가 있는 아이콘
	int						getFocusIconColorPattern();	//	
	int						getFocusBaseShape();
	int						getFocusBaseColorPartition();
	int						getFocusBaseColorPattern();
	int						getFocusBaseOutLineColorPattern();

	void					changeTab(int _iTab);

};

extern	CGuildMarkComposer	g_gwGuildMarkComposer;

#endif