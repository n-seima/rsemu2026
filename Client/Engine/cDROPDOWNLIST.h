#ifndef _classDROPDOWNLIST_H_
#define	_classDROPDOWNLIST_H_

#include <vector>
#include "SFC.H"
#include "cIMAGE.H"
#include "cTOOLTIP.h"

using namespace std;

//┌---------------------------------------------------------------┐
//│ 드롭다운 형식의 메뉴 컨트롤                                   │
//│ 거의 대부분 가져다 쓰는 ...                                   │
//└---------------------------------------------------------------┘
enum
{
	eDD_NORMAL,
	eDD_PUSH,
	eDD_OVER,
};

class cDROPDOWNLIST
{
public:	
	cRECT			m_rtPutArea;		// 드롭다운메뉴가 출력될 영역
	CTextDC*		m_text;				// 폰트
	CTooltip*		m_tooltip;
	cSPRITE*		m_sprite;			// 화살표를 출력할 스프라이트
	WORD			m_wNormal, m_wPush, m_wOver;	// 화살표 스프라이트 번호
	int				m_iSpriteWidth;		// 화살표 스프라이트 넓이
	int				m_iMenuState;		// 메뉴 상태 (NORMAL, PUSH, OVER)
	BOOL			m_bDropDownState;	// 메뉴 활성화 상태
	int				m_iLinePixel;		// 한줄표시할 픽셀
	vector<char*>	m_vtTextList;		// 드롭리스트의 텍스트들
	int				m_iLinegap;			// 한줄표시 후 공백
	int				m_iSelectList;		// 선택한 리스트 값
	int				m_iFocusList;		// 포커스가 위치한 리스트값

	void			init(int _iPutPosX, int _iPutPosY, int _iWidth, CTextDC* _text, CTooltip* _tooltip, cSPRITE *_sprite, WORD _Normal, WORD _Push, WORD _Over);
	void			draw();
	void			update(int _iMouseX, int _iMouseY, BOOL _bMouseTouch, BOOL _bMouseClick);

	inline void		addTextList(char* str) {m_vtTextList.push_back(str);}
	inline BOOL		isActive() {return m_bDropDownState;}
	inline int		getSelectList() {return m_iSelectList;}
};


#endif