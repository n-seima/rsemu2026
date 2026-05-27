#include "cDROPDOWNLIST.h"



//┌---------------------------------------------------------------┐
//│ 드롭다운리스트를 초기화한다.                                  │
//│                                                               │
//│ int _iPutPosX       :  출력할 X 좌표                          │
//│ int _iPutPosY       :  출력할 Y 좌표                          │
//│ int _iWidth         :  출력할 넓이                            │
//│ CTextDC* _text      :  폰트객체                               │
//│ CTooltip* _tooltip  :  틀을 그려줄 툴팁객체 -_-;              │
//│ cSPRITE *_sprite    :  툴팁옆에 출력할 아이콘의 스프라이트    │
//│ WORD _Normal        :  일반상태 스프라이트 인덱스             │
//│ WORD _Push          :  클릭상태 스프라이트 인덱스             │
//│ WORD _Over          :  오버상태 스프라이트 인덱스             │
//└---------------------------------------------------------------┘
void
cDROPDOWNLIST::init(int _iPutPosX, int _iPutPosY, int _iWidth, CTextDC* _text, CTooltip* _tooltip, cSPRITE *_sprite, WORD _Normal, WORD _Push, WORD _Over)
{
	m_vtTextList.clear();
	m_text = _text;
	m_tooltip = _tooltip;
	m_sprite = _sprite;
	m_wNormal = _Normal;
	m_wPush = _Push;
	m_wOver = _Over;
	m_iSpriteWidth = m_sprite->getSpriteWidth(_Normal)+2;
	m_iSelectList = -1;
	m_iSelectList = 0;

	m_iMenuState = eDD_NORMAL;
	m_bDropDownState = FALSE;
	m_iLinePixel = m_text->getFontHeight()+1;
	m_iLinegap = 4;

	m_rtPutArea.set(_iPutPosX, _iPutPosY, _iPutPosX+_iWidth, _iPutPosY+_text->getFontHeight()+8);
}



//┌---------------------------------------------------------------┐
//│ 출력                                                          │
//└---------------------------------------------------------------┘
void
cDROPDOWNLIST::draw()
{
	int iIndex = m_wNormal;
	if (m_iMenuState == eDD_OVER)
		iIndex = m_wOver;
	if (m_iMenuState == eDD_PUSH)
		iIndex = m_wPush;
	if (m_bDropDownState)
		iIndex = m_wPush;


	cRECT rtRealPut = {m_rtPutArea.x1, m_rtPutArea.y1, m_rtPutArea.x2, m_rtPutArea.y2};

	// 활성화상태라면 리스트만큼 틀크기를 늘려준다.
	if (m_bDropDownState)
	{
		rtRealPut.y2 += (m_vtTextList.size()-1)*(m_iLinePixel+m_iLinegap);
	}

	// 포커스가 간 리스트텍스트에 포인트를 준다.
	if (m_bDropDownState)
	{
		int iY = rtRealPut.y1+2;
		for(int i=0 ; i<m_vtTextList.size() ; i++)
		{
			if (m_iFocusList == i)
			{
				cDRAW::FillAB(0xffff, rtRealPut.x1+1, iY, rtRealPut.x1+rtRealPut.getWidth()-2, iY+m_iLinePixel+m_iLinegap, 20);
				break;
			}
			iY += (m_iLinePixel+m_iLinegap);
		}
	}

	// 틀 출력
	m_tooltip->drawBox(&rtRealPut, 0x0000);

	// 화살표 아이콘 출력
	if (!m_bDropDownState)
		m_sprite->Put(m_rtPutArea.x2-m_iSpriteWidth, m_rtPutArea.y1+3, iIndex);

	if (!m_bDropDownState)
	{
		m_text->cPut(rtRealPut.x1, rtRealPut.y1+4, rtRealPut.getWidth()-m_iSpriteWidth, m_iLinePixel, 0, WHITE, dFITTEXT_WIDTH_CENTER, m_vtTextList[m_iSelectList]);
	}

	// 활성화상태 목록을 출력해준다.
	if (m_bDropDownState)
	{
		int iY = rtRealPut.y1+2;
		for(int i=0 ; i<m_vtTextList.size() ; i++)
		{
			m_text->cPut(rtRealPut.x1, iY+2, rtRealPut.getWidth(), m_iLinePixel, 0, WHITE, dFITTEXT_WIDTH_CENTER, m_vtTextList[i]);
			//cDRAW::FillAB(0xffff, rtRealPut.x1, iY, rtRealPut.x1+rtRealPut.getWidth(), iY+m_iLinePixel, 20);
			iY += (m_iLinePixel+m_iLinegap);
		}
	}
}


//┌---------------------------------------------------------------┐
//│ 필터를 출력한다.                                              │
//│                                                               │
//│ int _iMouseX        :  마우스 X 좌표                          │
//│ int _iMouseY        :  마우스 Y 좌표                          │
//│ BOOL _bMouseTouch   :  눌렀을때!                              │
//│ BOOL _bMouseClick   :  떼었을때! (안쓰지만 나중을 위해..)     │
//└---------------------------------------------------------------┘

void
cDROPDOWNLIST::update(int _iMouseX, int _iMouseY, BOOL _bMouseTouch, BOOL _bMouseClick)
{
	// 마우스 이벤트 처리
	if (m_rtPutArea.isIn(_iMouseX, _iMouseY))
		m_iMenuState = eDD_OVER;
	if (!m_rtPutArea.isIn(_iMouseX, _iMouseY))
		m_iMenuState = eDD_NORMAL;
	if (m_iMenuState == eDD_OVER && _bMouseTouch)
	{
		m_iMenuState = eDD_PUSH;
		m_bDropDownState = 1-m_bDropDownState;
		m_iSelectList = m_iFocusList;
	}
	if (m_bDropDownState && _bMouseTouch && !m_rtPutArea.isIn(_iMouseX, _iMouseY))
	{
		m_iMenuState = eDD_NORMAL;
		m_bDropDownState = 1-m_bDropDownState;
		m_iSelectList = m_iFocusList;
	}

	// 메뉴 활성화 상태일때 처리
	if (m_bDropDownState)
	{
		int iY = m_rtPutArea.y1;
		for(int i=0 ; i<m_vtTextList.size() ; i++)
		{			
			cRECT rtArea = {m_rtPutArea.x1, iY, m_rtPutArea.x2, iY+(m_iLinePixel+m_iLinegap)};
			if (rtArea.isIn(_iMouseX, _iMouseY))
				m_iFocusList = i;
			iY += (m_iLinePixel+m_iLinegap);
		}
	}
}
