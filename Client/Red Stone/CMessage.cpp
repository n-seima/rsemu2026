#include "cMESSAGE.H"
#include "CDebugWindow.H"
#include "CLoadTextCode.h"
#include "CGamePlay.h"

CMsgBox		g_msgBox;			//	팝업 박스

char	g_strFontName[64];
char	g_strCheckFontName[64];
char	g_strMessageBoxTitleFontName[64];
char	g_strPitchmanShopTextName[64];

char	**g_pstrCommonMessage			=	NULL;
char	**g_pstrRegistServerMessage		=	NULL;
char	**g_pstrCorrectMagicResistance	=	NULL;
char	**g_pstrTip						=	NULL;
char	**g_pstrEnableAllignment		=	NULL;	//	착용/사용 가능한 성향
char	**g_pstrEquipUseLimitContents	=	NULL;
char	**g_pstrEquipmentName			=	NULL;	//	장비 이름
char	**g_pstrGuildRank				=	NULL;
char	**g_pstrHeroJobName				=	NULL;
char	**g_pstrNpcName					=	NULL;
char	**g_pstrSladerFilter			=	NULL;
char	**g_pstrMonsterGrade			=	NULL;
char	**g_pstrMonsterType				=	NULL;
char	**g_pstrSummonBeastName			=	NULL;
char	**g_pstrNameFilter				=	NULL;
char	**g_pstrExtraItemGrade			=	NULL;
char	**g_pstrServerComment			=	NULL;
char	**g_pstrStatusTooltip			=	NULL;
char	*g_lpstrBannerType[20];
char	*g_lpstrGuildGuardianName[5];
char	*g_lpstrGuildPetName[5];
char	*g_lpstrRedStoneReceiver[3];
char	**g_pstrMonsterDicMessage		=	NULL;	//JBC	몬스토 도감 관련 텍스트.	08-07-28
char	**g_pstrGreateGodGrace			=	NULL;	//JBC	거신의 은총 관련 텍스트.	08-10-22

COperatorCommandInfo	g_aOperatorCommandInfo[100];

cNOTICE		cNOTICE::s_aMessages[dNOTICE_COUNT];
cNOTICE		cNOTICE::s_Notice;
CTextDC		*cNOTICE::s_lpText;

int		cNOTICE::s_iCount	=	0;

inline	void
cNOTICE::setMessage(char *_lpMessage,DWORD _dwColor,int _iMessageType)
{	
	ustrncpy(m_strMessage	,	_lpMessage,sizeof(m_strMessage));

	m_iMessageType	=	_iMessageType;
	m_dwColor		=	_dwColor;
	m_dwTime		=	timeGetTime();
}

void
cNOTICE::update()
{
	if (m_dwTime	==	0)	return;

	if (m_dwTime	+	dNOTICE_TIME	<=	timeGetTime())	m_dwTime	=	0;
}

void
cNOTICE::put()
{
	if (m_dwTime	==	0)	return;

	s_lpText->cPut(400,490,m_dwColor,m_strMessage);
}

void
cNOTICE::Notify(char *_lpMessage,DWORD _dwColor)
{
	s_Notice.setMessage(_lpMessage,_dwColor,dMSG_KIND_NORMAL);
}

void
cNOTICE::Update()
{
	s_Notice.update();
}

void
cNOTICE::Put()
{
	s_Notice.put();
}







//▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲▲
//	팝업 박스
//▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼▼

//
//	초기설정
BOOL
CMsgBox::init(CTextDC *_lpText,cSPRITE *_lpSprite,int _iBox,int _iNormalButton,int _iActiveButton,int _iPushButton,cRECT *_lprectOutput)
{
	if	(!_lpSprite)
		return	FALSE;

	memset(this,0xff,sizeof(CMsgBoxInfo));

	m_lpText				=	_lpText;
	m_lpSprite				=	_lpSprite;

	m_wType					=	0;
	m_wBoxImage				=	_iBox;
	m_wNormalButtonImage	=	_iNormalButton;
	m_wActiveButtonImage	=	_iActiveButton;
	m_wPushButtonImage		=	_iPushButton;

	WORD	*lpBoxImage		=	m_lpSprite->Get16(m_wBoxImage);

	m_iBoxWidth				=	lpBoxImage[0];
	m_iBoxHeight			=	lpBoxImage[1];	//	박스의 크기

	m_pos.x					=	(g_iScreenWidth-m_iBoxWidth)/2;
	m_pos.y					=	(g_iScreenHeight-m_iBoxHeight)*2/5;

	m_posClickedImageMargin.x	=	1;
	m_posClickedImageMargin.y	=	1;

	memcpy(&m_rectOutput,_lprectOutput,sizeof(cRECT));

	return	TRUE;
}	//	CMsgBox::initImage

//
//	초기화
BOOL
CMsgBox::init(CTextDC *_lpText,cSPRITE *_lpSprite,CMsgBoxInfo *_lpImageInfo,cRECT *_lprectOutput)
{
	if (!_lpSprite)	return	FALSE;

	memset(this,0xff,sizeof(CMsgBoxInfo));

	m_lpText				=	_lpText;
	m_lpSprite				=	_lpSprite;

	memcpy(this,_lpImageInfo,sizeof(CMsgBoxInfo));

	if (m_wBoxImage			!=	0xffff)
	{
		WORD	*lpBoxImage	=	m_lpSprite->Get16(m_wBoxImage);

		m_iBoxWidth	=	m_lpSprite->getSpriteWidth(m_wBoxImage);
		m_iBoxHeight=	m_lpSprite->getSpriteHeight(m_wBoxImage);

		m_pos.x		=	(g_iScreenWidth-m_iBoxWidth)/2;
		m_pos.y		=	(g_iScreenHeight-m_iBoxHeight)*2/5;
	}

	m_posClickedImageMargin.x	=	1;
	m_posClickedImageMargin.y	=	1;

	{
		CTooltipInfo	ttInfo;

		ttInfo.m_iLeftTop				=	m_wLeftTopImage;
		ttInfo.m_iRightTop				=	m_wRightTopImage;
		ttInfo.m_iLeftBottom			=	m_wLeftBottomImage;
		ttInfo.m_iRightBottom			=	m_wRightBottomImage;
		ttInfo.m_iTopBorder				=	m_wTopBorderImage;
		ttInfo.m_iBottomBorder			=	m_wBottomBorderImage;
		ttInfo.m_iLeftBorder			=	m_wLeftBorderImage;
		ttInfo.m_iRightBorder			=	m_wRightBorderImage;

		ttInfo.m_lpSprite				=	_lpSprite;
		ttInfo.m_lpText					=	_lpText;

		ttInfo.m_wPannelColor			=	m_wPannelColor;
		ttInfo.m_wIsHalfblendingPannel	=	FALSE;

		m_ttFrame.init(&ttInfo);
	}

	memcpy(&m_rectOutput,_lprectOutput,sizeof(cRECT));

	m_textTitle.selectFont(m_wTitleFontSize,m_strTitleFontName,m_dwTitleFontStyle);
	m_textTitle.setShadowMargin(m_wTitleTextShadowMargin);

	return	TRUE;
}

//
//	업데이트 한다.
int
CMsgBox::update(int _iX,int _iY,BYTE *_lpKeyMap,BOOL _isTouchLeftButton,BOOL _isClickLeftButton,int _iClickButton)
{
	if (!isPoped())	return dMSGBOX_NOTHING;

	if (_isTouchLeftButton)	
		_isTouchLeftButton	=	TRUE;

	if (m_bIsRookie)
	{
		_isClickLeftButton	=	FALSE;
		_isTouchLeftButton	=	FALSE;

		m_bIsRookie			=	FALSE;
	}

	if (_iClickButton	!=	0xffff)
	{	
		close();

		return	_iClickButton;
	}

	DWORD	dwReturnValue	=	m_bmButton.update(_iX,_iY,_lpKeyMap,_isTouchLeftButton,_isClickLeftButton);

	if (dwReturnValue	!=	0xffffffff)
	{
		m_iResult	=	dwReturnValue;

		close();
	}
	else	m_iResult	=	dMSGBOX_NOTHING;

	return	dMSGBOX_NOTHING;
}	//	CMsgBox::update(int _iX,int _iY,BOOL _isClickLeftButton,int _iClickButton)

BOOL
CMsgBox::cPopup(char *_strTitle,char *_strMessage,char *_strButton1,char *_strButton2,char *_strButton3, int _iButton1Key, int _iButton2Key, int _iButton3Key)
{
	if	(!_popup(_strTitle,_strMessage,_strButton1,_strButton2,_strButton3, _iButton1Key, _iButton2Key, _iButton3Key))
		return	FALSE;

	m_iAlignText	=	dFITTEXT_HEIGHT_CENTER;

	return	TRUE;
}

//
//	버튼 3개짜리 팝업박스
BOOL
CMsgBox::_popup(char *_strTitle,char *_strMessage,char *_strButton1,char *_strButton2,char *_strButton3,int _iButton1Key, int _iButton2Key, int _iButton3Key)
{
	int		i;

	m_dwWaitPacket	=	0xffffffff;
	m_rectCurrent.set(&m_rectOutput);
	m_iTextNodeCount=	0;

//	버튼 설정
	{
		m_iButtonCount	=	0;

		if	(_strButton1)
		{
			strncpy(m_strButton[m_iButtonCount++],_strButton1,sizeof(m_strButton[0]));

			if	(_strButton2)
			{	
				strncpy(m_strButton[m_iButtonCount++],_strButton2,sizeof(m_strButton[0]));

				if	(_strButton3)
					strncpy(m_strButton[m_iButtonCount++],_strButton3,sizeof(m_strButton[0]));
			}
		}

		m_bmButton.init(m_lpSprite,m_iButtonCount,FALSE);

		int	iX				=	m_rectOutput.x1+m_rectOutput.getWidth()/2;
		int	iY				=	m_rectOutput.y2;
		int	iButtonWidth	=	m_lpSprite->getSpriteWidth(m_wNormalButtonImage);
		int	iButtonMargin	=	m_wButtonMargin;

		if	(m_iButtonCount	>=	3)	
			iButtonMargin	/=	2;

		iX	=	m_rectOutput.x1+m_rectOutput.getWidth()/2;
		iY	=	m_rectOutput.y2+m_wBottomFrameMargin+m_lpSprite->getSpriteHeight(m_wBottomBarImage)/2-m_lpSprite->getSpriteHeight(m_wNormalButtonImage)/2+m_wBarMenuVerticalMargin;

		int	iBottomBarWidth	=	m_rectOutput.getWidth()-m_wTextBoxHorizonMargin*2;
		int	iSize			=	m_iButtonCount*(iButtonWidth+iButtonMargin) - iBottomBarWidth;

		if	(iSize > 0)
		{
			m_rectCurrent.x1-=	iSize/2;
			m_rectCurrent.x2+=	iSize/2;
		}

		int	iButtonKey[dMAX_MSG_BOX_BUTTON_COUNT] = {_iButton1Key,_iButton2Key,_iButton3Key};

		for (i=0;i<m_iButtonCount;i++)
		{
			m_bmButton.addImageBar(	m_strButton[i],dMSGBOX_BUTTON1+i,iX-(iButtonWidth+iButtonMargin)*m_iButtonCount/2+iButtonMargin/2+(iButtonWidth+iButtonMargin)*i,iY,
									m_wNormalButtonImage,m_wActiveButtonImage,m_wPushButtonImage);

			if(iButtonKey[i] != 0xffffffff)
				m_bmButton.setKey(dMSGBOX_BUTTON1+i,iButtonKey[i]);
		}

		m_bmButton.setClickTextAllBarMargin(m_posClickedImageMargin.x,m_posClickedImageMargin.y);
		m_bmButton.setAllBarTextColor(m_dwBarTextColor,m_wIsPutBarTextShadow);
	}	//	버튼 설정

//	타이틀바 설정
	{
		if	(_strTitle)
			strncpy(m_strTitle,_strTitle,sizeof(m_strTitle));
		else
			strcpy(m_strTitle,"");
	}	//	타이틀바 설정

//	메시지 복사
	strncpy(m_strMessage,_strMessage,sizeof(m_strMessage));

	m_isPoped		=	TRUE;
	m_isExclusive	=	TRUE;
	m_iAlignText	=	0;
	m_bIsRookie		=	TRUE;
	m_lpCurrentTextDC=	NULL;

	m_iIconShape	=	eMBIS_IMPORTANT;


	return	TRUE;
}

void
CMsgBox::drawFrame(cRECT *_lpRect,BOOL _bIsDrawShadow,char *_lpstrTitle)
{
	if (_bIsDrawShadow)
	{
		cDRAW::FillHB(0,_lpRect->x2+m_ttFrame.m_iRightBorderWidth,_lpRect->y1+m_wShadowMargin,_lpRect->x2+m_wShadowMargin+m_ttFrame.m_iRightBorderWidth,_lpRect->y2+m_wShadowMargin);
		cDRAW::FillHB(0,_lpRect->x1+m_wShadowMargin-m_ttFrame.m_iLeftBorderWidth,_lpRect->y2+m_ttFrame.m_iBottomBorderHeight,_lpRect->x2+m_ttFrame.m_iRightBorderWidth,_lpRect->y2+m_wShadowMargin+m_ttFrame.m_iBottomBorderHeight);
	}

	m_ttFrame.drawBox(_lpRect,0,32,TRUE);

	_lpRect->x1	-=	m_wLeftFrameMargin;
	_lpRect->x2	+=	m_wRightFrameMargin;
	_lpRect->y1	-=	m_wTopFrameMargin;
	_lpRect->y2	+=	m_wBottomFrameMargin;
	
	m_lpSprite->putClipedImage(_lpRect->x1,_lpRect->y1,m_wTitleBarImage,_lpRect->getWidth());
	m_lpSprite->putClipedImage(_lpRect->x1,_lpRect->y2-m_lpSprite->getSpriteHeight(m_wBottomBarImage),m_wBottomBarImage,_lpRect->getWidth());

	cRECT	rect;

	rect.x1	=	_lpRect->x1;
	rect.x2	=	_lpRect->x1+_lpRect->getWidth()-m_lpSprite->getSpriteWidth(m_wTitleBarLeftImage);
	rect.y1	=	_lpRect->y1;
	rect.y2	=	_lpRect->y1+_lpRect->getHeight()-m_lpSprite->getSpriteHeight(m_wBottomBarImage)-1;

	m_lpSprite->Put(rect.x1,rect.y1,m_wTitleBarLeftImage);
	m_lpSprite->Put(rect.x2,rect.y1,m_wTitleBarRightImage);

	m_lpSprite->Put(rect.x1,rect.y2,m_wBottomBarLeftImage);
	m_lpSprite->Put(rect.x2,rect.y2,m_wBottomBarRightImage);

	if	(_lpstrTitle)
		m_textTitle.cPut(_lpRect->x1+_lpRect->getWidth()/2,_lpRect->y1+m_wTitleTextVerticalMargin+m_lpSprite->getSpriteHeight(m_wTitleBarImage)/2-m_textTitle.m_iFontHeight/2,WHITE,_lpstrTitle);
}

//
//	그려준다.
void
CMsgBox::draw()
{
	if	(!isPoped())
		return;

	if	(m_isFreeze)
		cDRAW::Clear();

//	m_rectCurrent.set(g_iScreenWidth/2-140,g_iScreenHeight/20*7,g_iScreenWidth/2+140,g_iScreenHeight/20*11);

	cRECT	rect;

	rect.set(&m_rectCurrent);

	rect.x1	-=	m_wTextBoxHorizonMargin;//_lpSprite->getSpriteWidth(m_wTitleBarLeftImage);
	rect.x2	+=	m_wTextBoxHorizonMargin;//m_lpSprite->getSpriteWidth(m_wTitleBarRightImage);
	rect.y1	-=	m_lpSprite->getSpriteHeight(m_wTitleBarImage);
	rect.y2	+=	m_lpSprite->getSpriteHeight(m_wBottomBarImage);

	drawFrame(&rect,TRUE,m_strTitle);

	rect.set(&m_rectCurrent);

	if	(m_iTextNodeCount)
		drawNodeText();
	else
	{
		CTextDC	*lpTextDC	=	m_lpText;

		if	(m_lpCurrentTextDC)
			lpTextDC	=	m_lpCurrentTextDC;

		if	(m_iIconShape	!=	eMBIS_NONE)
		{
			rect.x1	+=	m_lpSprite->getSpriteWidth(m_wIconImportant)+6;

			if	(m_iIconShape	==	eMBIS_IMPORTANT)
				m_lpSprite->Put(m_rectCurrent.x1,m_rectCurrent.y1+m_rectCurrent.getHeight()/2-m_lpSprite->getSpriteHeight(m_wIconImportant)/2,m_wIconImportant);
			else
			if	(m_iIconShape	!=	eMBIS_WARNING)
				m_lpSprite->Put(m_rectCurrent.x1,m_rectCurrent.y1+m_rectCurrent.getHeight()/2-m_lpSprite->getSpriteHeight(m_wIconImportant)/2,m_wIconWarning);
		}

		if	(m_iAlignText)
			lpTextDC->cPut(rect.x1,rect.y1,rect.getWidth(),rect.getHeight(),0,WHITE,m_iAlignText,m_strMessage);	//	메인 텍스트
		else
			lpTextDC->put(rect.x1,rect.y1,rect.getWidth(),rect.getHeight(),0,WHITE,m_strMessage);	//	메인 텍스트
	}

	m_bmButton.draw();	//	버튼 그리기
}	//	CMsgBox::draw()

//
//	버튼의 아이디를 변경한다.
void
CMsgBox::setButtonId(int _iId1,int _iId2,int _iId3)
{
	m_bmButton.setMenuId(0,_iId1);

	if	(_iId2	!=	0xffffffff)	
		m_bmButton.setMenuId(1,_iId2);

	if	(_iId3	!=	0xffffffff)	
		m_bmButton.setMenuId(2,_iId3);
}	//	CMsgBox::setButtonId(int _iId1,int _iId2,int _iId3)

//
//	욕설 필터
inline	BOOL	IsIncludeString(char *_lpstrText,char *_lpstrWord)
{
	int	iWordLength	=	strlen(_lpstrWord);

	for (int i=0;i<iWordLength;i++)
	{
		if	(_lpstrText[i] ==	NULL			)
			return	FALSE;
		if	(_lpstrText[i] !=	_lpstrWord[i]	)
			return	FALSE;
	}

	return	TRUE;
}

BOOL	sfcStrstr(char *_lpstrText,char *_lpstrWord)
{
	int	iLength		=	strlen(_lpstrText),i;
	int	iWordLength	=	strlen(_lpstrWord);

	for (i=0;i<iLength;)
	{
		if	(IsIncludeString(_lpstrText+i,_lpstrWord))
			return	TRUE;

		if	(Is2ByteChar(_lpstrText[i]))
			i++;
		i++;
	}

	return	FALSE;
}

BOOL	CheckFilter(char *_lpstrText,char **_lpFilter,int _iFilterCount)
{
	char	strText[512];
	int		i;
	int		iLength	=	strlen(_lpstrText),iPoint=0;

	for (i=0;i<iLength;i++)
		if (_lpstrText[i]	!=	' ')
		{
			strText[iPoint]	=	_lpstrText[i];

			if (Is2ByteChar(_lpstrText[i]))
			{
				iPoint++;
				i++;
				strText[iPoint]	=	_lpstrText[i];
			}
			else
			if (strText[iPoint]	>=	'A' && strText[iPoint]	<=	'Z' )
				strText[iPoint]	+=	('a'-'A');

			iPoint++;
		}

	strText[iPoint]	=	NULL;

	for (i=0;i<_iFilterCount;i++)
	{
#ifdef	_FOR_INTER
		if	(stricmp(_lpFilter[i],"ho")	==	0)
			continue;
#endif
		if	(sfcStrstr(strText,_lpFilter[i]))
			return	TRUE;
	}

	return	FALSE;
}

void
CMsgBox::setSize(int _iWidth,int _iHeight)
{
	if	(_iWidth)
	{
		int	iCenter			=	m_rectCurrent.x1+(m_rectCurrent.x2-m_rectCurrent.x1)/2;
		m_rectCurrent.x1	=	iCenter-_iWidth/2;
		m_rectCurrent.x2	=	iCenter+_iWidth/2;
	}
	if	(_iHeight)
	{
		int	iCenter			=	m_rectCurrent.y1+(m_rectCurrent.y2-m_rectCurrent.y1)/2;
		m_rectCurrent.y1	=	iCenter-_iHeight/2;
		m_rectCurrent.y2	=	iCenter+_iHeight/2;

		int	iX				=	m_rectCurrent.x1+m_rectCurrent.getWidth()/2;
		int	iY				=	m_rectCurrent.y2+m_wBottomFrameMargin+m_lpSprite->getSpriteHeight(m_wBottomBarImage)/2-
								m_lpSprite->getSpriteHeight(m_wNormalButtonImage)/2+m_wBarMenuVerticalMargin;

		int	iButtonWidth	=	m_lpSprite->getSpriteWidth(m_wNormalButtonImage);
		int	iButtonMargin	=	m_wButtonMargin;
		int	iBottomBarWidth	=	m_rectCurrent.getWidth()-m_wTextBoxHorizonMargin*2;

		int	iSize			=	m_iButtonCount*(iButtonWidth+iButtonMargin) - iBottomBarWidth;

		for (int i=0;i<m_iButtonCount;i++)
			m_bmButton.m_pMenu[i].move(iX-(iButtonWidth+iButtonMargin)*m_iButtonCount/2+iButtonMargin/2+(iButtonWidth+iButtonMargin)*i,iY);
	}
}

bool
CMsgBox::addTextNode(int _iX,int _iY,int _iPutMethod,DWORD _dwColor,char *_lpstrText)
{
	if	(m_iTextNodeCount	>=	c_iMaxTextNodeCount)
		return	false;

	cTextNode	*lpNode	=	&m_aTextNode[m_iTextNodeCount];

	lpNode->set(_iX,_iY,_iPutMethod,_dwColor,_lpstrText);

	m_iTextNodeCount++;

	return	true;
}

void
CMsgBox::drawNodeText()
{
	CTextDC	*lpTextDC	=	m_lpText;

	if	(m_lpCurrentTextDC)
		lpTextDC	=	m_lpCurrentTextDC;

	for (int i=0;i<m_iTextNodeCount;i++)
	{
		cTextNode	*lpNode	=	&m_aTextNode[i];

		int	iX	=	m_rectCurrent.x1+lpNode->x;
		int	iY	=	m_rectCurrent.y1+lpNode->y;

		switch(lpNode->m_wPutMethod)
		{
			case	dFITTEXT_LEFT	:
				lpTextDC->put(iX,iY,lpNode->m_dwTextColor,lpNode->m_pstrText);
				break;
			case	dFITTEXT_WIDTH_CENTER	:
			case	dFITTEXT_HEIGHT_CENTER	:
			case	dFITTEXT_CENTER	:
				lpTextDC->cPut(iX,iY,lpNode->m_dwTextColor,lpNode->m_pstrText);
				break;
			case	dFITTEXT_RIGHT	:
				lpTextDC->putRF(iX,iY,lpNode->m_dwTextColor,lpNode->m_pstrText);
				break;
		}
	}
}

void
CMsgBox::close()
{
	m_isFreeze			=	FALSE;
	m_isPoped			=	FALSE;
	m_isExclusive		=	FALSE;
	m_iTextNodeCount	=	0;
}