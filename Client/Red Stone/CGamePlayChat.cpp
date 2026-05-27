#include "CGamePlay.H"
#include "CHero.H"
#include "cMAP.H"
#include "CActor.H"
#include "CGamePlayInterface.H"
#include "CLog.H"
#include "CMessage.H"
#include "CGuild.H"
#include "CWindowInterface.H"
#include "cLoadTextCode.H"
#include "fireCracker.H"
#include "arca.H"
#include "bm_event.H"
#include "hero_mini_pet.H"
#include "book_MonsterDictionary.h"
#include "BattleLog.h"
#include "CSound.h"
#include "cportal.h"

#ifdef	_FOR_ENG
int	l_iFontSize	=	13;
#else
int	l_iFontSize	=	12;
#endif

cTALK				CGamePlay::s_inputChatting;
cRECT				CGamePlay::s_rectChatBox;
CImageScrollBar		CGamePlay::s_sbChatBox;
char				CGamePlay::s_strLastSendSayId[dNAME_LENGTH]		=	"";
char				CGamePlay::s_strLastReceiveSayId[dNAME_LENGTH]	=	"";
cBARMENU			CGamePlay::s_bmChattingFilter;
cBARMENU			CGamePlay::s_bmChatBoxScale;
cBARMENU			CGamePlay::s_bmChatBarMenu;
int					CGamePlay::s_iChatFilter						=	eCMF_NORMAL_CHAT;
BOOL				CGamePlay::s_bIsChattingCompositionState		=	FALSE;
cSTRINGS			CGamePlay::s_strSayUserList[100];
int					CGamePlay::s_iCurrentSayTalkerIndex;
CTextList			CGamePlay::s_tlChat;
CTextList			CGamePlay::s_tlRecord;		//	메시지 기억
CTextList			CGamePlay::s_tlInputMessage;	//	입력한 채팅 메시지
char				CGamePlay::s_strChatMessage[dCHAT_LENGTH];
char				CGamePlay::s_strPreviousChatMessage[dCHAT_LENGTH];//채팅 메시지

int					l_iChattingFilterHeight							=	15;
BOOL				l_bIsExistNewWhisper,l_bIsExistNewGuildChat,l_bIsExistNewPartyChat,l_bIsExistNewNormalChat;
BOOL				l_bIsWantChangeFont	=	FALSE;

enum
{
	eCFM_NORMAL_CHATTING,
	eCFM_PARTY_CHATTING,
	eCFM_GUILD_CHATTING,
	eCFM_WHISPER,
};

enum
{
	eCBSB_ZOOM_PLUS		,
	eCBSB_ZOOM_MINUS	,
	eCBSB_TOGGLE		
};

enum
{
	eCBM_UNLOCK			,
	eCBM_LOCK			,
};

cRECT	l_aChatBoxSize[dCHAT_BOX_MAX_SCALE]	=
{
	{	5,414,266,488},
	{	5,394,266,488},
	{	5,364,266,488},
	{	5,300,266,488},
	{	5,200,350,488},
	{	5,100,450,488},
	{	5,5  ,522,488},
};

BOOL	l_bIsRevisedChatBoxPos	=	FALSE;
//
//	 채팅박스 초기화
void
CGamePlay::InitChattingBox()
{
	if	(g_iScreenWidth	!=	800 && l_bIsRevisedChatBoxPos == FALSE)
	{
		l_bIsRevisedChatBoxPos	=	TRUE;
		
		int	iCorrectBI_X	=	g_iScreenWidth-800;
		int	iCorrectBI_Y	=	g_iScreenHeight-494;

		for (int i=0;i<dCHAT_BOX_MAX_SCALE;i++)
		{
			l_aChatBoxSize[i].x1	=	5;
			l_aChatBoxSize[i].y1	+=	iCorrectBI_Y;
			l_aChatBoxSize[i].x2	=	iCorrectBI_X-5;
			l_aChatBoxSize[i].y2	+=	iCorrectBI_Y;
		}
	}
	
	l_bIsExistNewWhisper	=	FALSE;
	l_bIsExistNewGuildChat	=	FALSE;
	l_bIsExistNewPartyChat	=	FALSE;
	l_bIsExistNewNormalChat	=	FALSE;

	{
		if	(IsFile(_ms("%s/change_font.dat",g_strLogFolder)))
		{
			l_bIsWantChangeFont	=	TRUE;
			s_textChat.selectFont(l_iFontSize,g_strPitchmanShopTextName);
		}
		else
		{
			l_bIsWantChangeFont	=	FALSE;
			s_textChat.selectFont(l_iFontSize,g_strCheckFontName);
		}
	}

	{
		cRECT	rect;

		rect.set(&l_aChatBoxSize[0]);

		if	(s_bIsHideInterface)
			rect.add(0,80);

		int		aiCoulmnSize[2]	=	{256,dNAME_LENGTH};

		s_tlChat.initForMultiColumn(&rect,&s_text2,256,2,aiCoulmnSize,0);	//	초기화

		s_tlChat.setLineGap(1);
	}

//	채팅 기록자 초기화
	{
		int		aColumnSize[2]	=	{dCHAT_LENGTH,dNAME_LENGTH};

		s_tlRecord.init(512,2,aColumnSize,4);	//	초기화
		s_tlInputMessage.init(51,1,aColumnSize);
	}

//	채팅 박스 프레임 초기화
	{
		RebuildChatMessageBoxFrame();

		AddChatMessage(LTYELLOW,dMSG_ADMIN_ICON_EXPLAIN);
		AddChatMessage(LTYELLOW,dMSG_FRIEND_ICON_EXPLAIN);
		AddChatMessage(WHITE,dMSG_ACCUSE_METHOD);
		AddChatMessage(WHITE,dMSG_SAY_METHOD);

		s_sbChatBox.setSize(s_tlChat.getDisplayLineCount(),s_tlChat.getVisibleLineCount());
		s_sbChatBox.setPos(s_tlChat.getFirstLine());
	}

//	채팅 필터 버튼들 초기화
	{
		s_iChatFilter		=	eCMF_NORMAL_CHAT;

		int	iX				=	s_rectChatBox.x1;;
		int	iY				=	s_rectChatBox.y2-g_sprInterface.getSpriteHeight(eCF_NORMAL_CHATTING_DEFAULT)+3;
		int	iButtonWidth	=	g_sprInterface.getSpriteWidth(eCF_NORMAL_CHATTING_DEFAULT);

		s_bmChattingFilter.init(&g_sprInterface,10,dBAR_RADIO);
		s_bmChattingFilter.addImageBar("",eCFM_NORMAL_CHATTING	,iX					,iY,eCF_NORMAL_CHATTING_DEFAULT,eCF_NORMAL_CHATTING_FOCUS	,0xffff,eCF_NORMAL_CHATTING_SELECT,0xffff,dMSG_NORMAL_CHAT);
		s_bmChattingFilter.addImageBar("",eCFM_PARTY_CHATTING	,iX+iButtonWidth	,iY,eCF_PARTY_CHATTING_DEFAULT,eCF_PARTY_CHATTING_FOCUS		,0xffff,eCF_PARTY_CHATTING_SELECT,0xffff,dMSG_PARTY_CHAT);
		s_bmChattingFilter.addImageBar("",eCFM_GUILD_CHATTING	,iX+iButtonWidth*2	,iY,eCF_GUILD_CHATTING_DEFAULT,eCF_GUILD_CHATTING_FOCUS		,0xffff,eCF_GUILD_CHATTING_SELECT,0xffff,dMSG_GUILD_CHAT);
		s_bmChattingFilter.addImageBar("",eCFM_WHISPER			,iX+iButtonWidth*3	,iY,eCF_WHISPER_CHATTING_DEFAULT,eCF_WHISPER_CHATTING_FOCUS	,0xffff,eCF_WHISPER_CHATTING_SELECT,0xffff,dMSG_WHISPER_CHAT);

		s_bmChattingFilter.check(s_iChatFilter);
	}

//	채팅 박스 스케일 버튼들 초기화
	{
		int	iY				=	s_rectChatBox.y2-g_sprInterface.getSpriteHeight(eCF_NORMAL_CHATTING_DEFAULT)+3;

		s_bmChatBoxScale.init(&g_sprInterface,3);//
		s_bmChatBoxScale.addImageBar("",eCBSB_ZOOM_PLUS		,104,iY,eMMI_ZOOM_PLUS_NORMAL		,eMMI_ZOOM_PLUS_LIGHT		,eMMI_ZOOM_PLUS_PRESSED		,0xffff,0xffff);
		s_bmChatBoxScale.addImageBar("",eCBSB_ZOOM_MINUS	,119,iY,eMMI_ZOOM_MINUS_NORMAL		,eMMI_ZOOM_MINUS_LIGHT		,eMMI_ZOOM_MINUS_PRESSED	,0xffff,0xffff);
		s_bmChatBoxScale.addImageBar("",eCBSB_TOGGLE		,134,iY,eMMI_CLOSE_NORMAL			,eMMI_CLOSE_LIGHT			,eMMI_CLOSE_PRESSED			,0xffff,0xffff);
	}

	s_bmChatBarMenu.init(&g_sprInterface,3);//
	s_bmChatBarMenu.addImageBar("",0	,104+g_iCorrectBI_X,0,eCII_CHATTING_INPUT_LOCK_NORMAL,eCII_CHATTING_INPUT_LOCK_ACTIVE,eCII_CHATTING_INPUT_LOCK_ACTIVE,0xffff,0xffff);

	if	(g_config.m_iChattingBoxScale & 0x8000)
		s_bmChatBoxScale.setImageBar(eCBSB_TOGGLE,eMMI_RESIZE_WINDOW_NORMAL	,eMMI_RESIZE_WINDOW_LIGHT	,eMMI_RESIZE_WINDOW_PRESSED	,0xffff,0xffff);									
}


//
//	 채팅박스 프레임 초기화
void
CGamePlay::ResetChattingBoxFrame(BOOL _bIsResetInputChatting)
{
	{
		cRECT	rect;

		rect.set(&l_aChatBoxSize[0]);

		if	(s_bIsHideInterface)
			rect.add(0,80);

		s_tlChat.resetFrame(&rect,0);	//	초기화
	}

//	채팅 박스 프레임 초기화
	{
		RebuildChatMessageBoxFrame();
	}

//	채팅 필터 버튼들 초기화
	{
		int	iX				=	s_rectChatBox.x1;//-s_sbChatBox.m_iButtonWidth;
		int	iY				=	s_rectChatBox.y2-g_sprInterface.getSpriteHeight(eCF_NORMAL_CHATTING_DEFAULT)+3;
		int	iButtonWidth	=	g_sprInterface.getSpriteWidth(eCF_NORMAL_CHATTING_DEFAULT);

		s_bmChattingFilter.correctPos(eCFM_NORMAL_CHATTING	,iX					,iY);
		s_bmChattingFilter.correctPos(eCFM_PARTY_CHATTING	,iX+iButtonWidth	,iY);
		s_bmChattingFilter.correctPos(eCFM_GUILD_CHATTING	,iX+iButtonWidth*2	,iY);
		s_bmChattingFilter.correctPos(eCFM_WHISPER			,iX+iButtonWidth*3	,iY);
		s_bmChatBoxScale.correctPos(eCBSB_ZOOM_PLUS			,104,iY);
		s_bmChatBoxScale.correctPos(eCBSB_ZOOM_MINUS		,119,iY);
		s_bmChatBoxScale.correctPos(eCBSB_TOGGLE			,134,iY);
	}

	if	(_bIsResetInputChatting)
	{
		s_inputChatting.init(s_hWnd,s_hInst,&s_text,1);

		int	iCorrectBI_X	=	g_iScreenWidth-800;
		int	iCorrectBI_Y	=	g_iScreenHeight-600;

		int	iInputBarPos	=	492;

		if	(s_bIsHideInterface)
			iInputBarPos	=	576;

		s_inputChatting.set(79+14+26+iCorrectBI_X,iInputBarPos+6+iCorrectBI_Y-1,
							0,
							79+14+26+iCorrectBI_X,iInputBarPos+6+iCorrectBI_Y-1,
							574,13,dCHAT_LENGTH-dNAME_LENGTH-1,"");

		s_inputChatting.setColor(0,WHITE);
		s_inputChatting.disable(TRUE);
	}
}

//
//	채팅 메시지 박스 프래임 재 설정
void
CGamePlay::RebuildChatMessageBoxFrame(BOOL _bIsRequireRebuildChatMessage)
{
	cRECT	rectChatFrame;

	{
		int		iScale	=	g_config.m_iChattingBoxScale;

		if	(iScale & 0x8000)	
			iScale	-=	0x8000;

		s_rectChatBox.set(&l_aChatBoxSize[iScale]);

		if	(s_bIsHideInterface)
			s_rectChatBox.add(0,80);

		rectChatFrame	=	s_rectChatBox;
		rectChatFrame.x1+=	s_sbChatBox.m_iButtonWidth;
		rectChatFrame.y2-=	l_iChattingFilterHeight;
	}

	if	(_bIsRequireRebuildChatMessage)
		RebuildChatMessageBox();

	{
		s_tlChat.resetFrame(&rectChatFrame,0);
		rectChatFrame.y1	=	rectChatFrame.y2-s_tlChat.getVisibleHeight();
		s_rectChatBox.y1	=	rectChatFrame.y1;
		s_tlChat.resetFrame(&rectChatFrame,0);
	}

	{
		s_sbChatBox.set(s_rectChatBox.x1-1,s_rectChatBox.y1-1,s_rectChatBox.getHeight()-l_iChattingFilterHeight+2);
		s_sbChatBox.setSize(s_tlChat.getDisplayLineCount(),s_tlChat.getVisibleLineCount());
		s_sbChatBox.setPos(s_tlChat.getFirstLine());
	}
}

//
//	채팅을 기록
void
CGamePlay::RecordChat(char *_lpstrName,char *_lpstrMessage,int _iChatType,int _iPacketType,int _iChatRecordCategory,DWORD _dwColor)
{
	if	(_lpstrMessage)
		s_tlRecord.addText(0,_lpstrMessage);
	if	(_lpstrName)
		s_tlRecord.addText(1,_lpstrName);

	s_tlRecord.setLastestLineValue(0,_iChatType);
	s_tlRecord.setLastestLineValue(1,_iPacketType);
	s_tlRecord.setLastestLineValue(2,_iChatRecordCategory);
	s_tlRecord.setLastestLineColor(_dwColor);

	s_tlRecord.increaseLineCount();
}

void
CGamePlay::AddChatMessage(DWORD _dwColor,char *_lpstrMessage,char *_lpstrName)
{
	if	(_lpstrMessage[0]	==	NULL)
		return;

	s_textChat.someCodeIgnoreOn();

	s_tlChat.addColorText(_dwColor,_lpstrMessage);

	if	(_lpstrName	==	NULL)
		s_tlChat.addText(1,"",TRUE);
	else
		s_tlChat.addText(1,_lpstrName,TRUE);

	s_textChat.someCodeIgnoreOff();
}

//
//	이벤트 
void
CGamePlay::EventNotice(char *_lpstrMessage,int _iType,BOOL _bIsRecordPlay,DWORD _dwColor, WORD _wTime , WORD _wEffectflickeringly)
{		// 마지막 수정일 : 09.10.08
	BOOL	bIsAllowPartyItemMessage	=	FALSE;

	if	(g_config.m_bf1IsHidePartyItemMessage	==	FALSE)
		bIsAllowPartyItemMessage	=	TRUE;

	if	(s_iChatFilter == eCMF_PARTY_CHAT) 
		bIsAllowPartyItemMessage	=	FALSE;
	if	(s_iChatFilter == eCMF_GUILD_CHAT) 
		bIsAllowPartyItemMessage	=	FALSE;
	if	(s_iChatFilter == eCMF_WHISPER	) 
		bIsAllowPartyItemMessage	=	FALSE;

	switch(_iType)
	{
		case	eCT_EVENT_NOTICE_IN_CHAT_BOX		:
			if (_dwColor	==	0xffffffff)
				_dwColor	=	BTBLUE;

			if	(!_bIsRecordPlay)
				RecordChat(NULL,_lpstrMessage,eCMF_NORMAL_CHAT,_iType,eCPT_EVENT_MESSAGE,_dwColor);

			AddChatMessage(_dwColor,_lpstrMessage);

			s_sbChatBox.setSize(s_tlChat.getDisplayLineCount(),s_tlChat.getVisibleLineCount());
			s_sbChatBox.setPos(s_tlChat.getFirstLine());
			break;

		case	eCT_SYSTEM_MESSAGE					:
			if	(_dwColor	==	0xffffffff)
				_dwColor	=	WHITE;

			if	(!_bIsRecordPlay)
				RecordChat(NULL,_lpstrMessage,eCMF_NORMAL_CHAT,_iType,eCPT_EVENT_MESSAGE,_dwColor);

			AddChatMessage(_dwColor,_lpstrMessage);

			s_sbChatBox.setSize(s_tlChat.getDisplayLineCount(),s_tlChat.getVisibleLineCount());
			s_sbChatBox.setPos(s_tlChat.getFirstLine());
			break;

		case	eCT_GET_PARTY_ITEM_MESSAGE			:
			if	(_dwColor	==	0xffffffff)
				_dwColor	=	WHITE;

			if	(!_bIsRecordPlay)
				RecordChat(NULL,_lpstrMessage,eCMF_NORMAL_CHAT,_iType,eCPT_EVENT_MESSAGE,_dwColor);

			if	(bIsAllowPartyItemMessage)
			{
				AddChatMessage(_dwColor,_lpstrMessage);

				s_sbChatBox.setSize(s_tlChat.getDisplayLineCount(),s_tlChat.getVisibleLineCount());
				s_sbChatBox.setPos(s_tlChat.getFirstLine());
			}
			break;

		case	eCT_EVENT_NOTICE_IN_SYSTEM_MESSAGE	:	// 이벤트로 필드 전체에 시스템 메시지
			if	(_dwColor	==	0xffffffff)
				_dwColor	=	BTBLUE;
			
			AddSystemMessage(_dwColor,_lpstrMessage);
			break;
			
		case	eCT_EVENT_NOTICE_IN_NOTICE_BOX		:	// 이벤트로 필드 전체에 공지
			Notice(_lpstrMessage , _wTime , _wEffectflickeringly);
			break;
		case eCT_ETERNAL_UPGRADE_MESSAGE		:
			{
				char strMessage[256] ="";
				sprintf(strMessage,dMSG_ETERNAL_UPGARDE_MAX_SUCCESS_FORM, _lpstrMessage);
				WarningMessage(strMessage);
				break;
			}
	}
}

BOOL
CGamePlay::CheckMessage(char *_lpstrSrcText,char *_lpstrResultText)
{
	s_textChat.someCodeIgnoreOn();

	BOOL	bResult	=	_CheckMessage(_lpstrSrcText,_lpstrResultText);

	s_textChat.someCodeIgnoreOff();

	return	bResult;
}

BOOL
CGamePlay::_CheckMessage(char *_lpstrSrcText,char *_lpstrResultText)
{
	int		iTextPoint	=	0;
	BOOL	bIsFirst	=	TRUE,bIsPreviousSpace=FALSE;

	if	(sfcStrstr(_lpstrSrcText,"　"))
		return	FALSE;

	for (int i=0;i<dCHAT_LENGTH;i++)
	{
		if	(bIsFirst && _lpstrSrcText[i] == ' ')
			continue;

		bIsFirst	=	FALSE;

		if	(_lpstrSrcText[i] == ' ')
		{
			if	(bIsPreviousSpace)
				continue;

			bIsPreviousSpace	=	TRUE;
		}
		else
			bIsPreviousSpace	=	FALSE;

		_lpstrResultText[iTextPoint]	=	_lpstrSrcText[i];

		if	(Is2ByteChar(_lpstrSrcText[i]))
		{
			_lpstrResultText[iTextPoint+1]	=	_lpstrSrcText[i+1];
			iTextPoint++;
			i++;
		}
		else
		if	(_lpstrResultText[iTextPoint]	==	37)
		{
			char	icon[3];
			strcpy(icon,dMSG_PERCENTAGE);

			_lpstrResultText[iTextPoint]	=	icon[0];
			_lpstrResultText[iTextPoint+1]	=	icon[1];

			iTextPoint++;
		}
		else
		if	(_lpstrResultText[iTextPoint]	==	92)
		{
#ifdef	_FOR_KOREA
			char	icon[3]	=	"￦";
			_lpstrResultText[iTextPoint]	=	icon[0];
			_lpstrResultText[iTextPoint+1]=	icon[1];
			iTextPoint++;
#elif	_FOR_JAPAN
			char	icon[3]	=	"걦";
			_lpstrResultText[iTextPoint]	=	icon[0];
			_lpstrResultText[iTextPoint+1]=	icon[1];
			iTextPoint++;
#elif	_FOR_CHINA
			_lpstrResultText[iTextPoint]	=	' ';
#elif	_FOR_INTERNATIONAL
			_lpstrResultText[iTextPoint]	=	'/';
#endif
		}

		iTextPoint++;
	}

	if	(s_textChat.isIncludeSpecialCode(_lpstrResultText))
		return	FALSE;

	return	TRUE;
}
//
//	채팅
void
CGamePlay::Chat(SG_CHAT *_lpChatPacket,BOOL _bIsRecordPlay)
{
	BOOL	bIsHero				=	FALSE;
	BOOL	bIsFriend			=	FALSE;
	BOOL	bIsAdmin			=	FALSE;
	BOOL	bIsAllowSay			=	FALSE;
	BOOL	bIsAllowChat		=	FALSE;
	BOOL	bIsAllowPartyChat	=	FALSE;
	BOOL	bIsAllowGuildChat	=	FALSE;
	BOOL	bIsAddChatWindow	=	TRUE;

	int		iChatType			=	_lpChatPacket->bf6ChatType;
	char	strMessage[512];
	char	*lpstrChatMessage;
	char	*lpstrName			=	_lpChatPacket->strMessage;

	lpstrChatMessage			=	_lpChatPacket->strMessage+strlen(lpstrName)+1;

	CActor	*lpChatter			=	g_am.getTestedActor(_lpChatPacket->bf11Serial,FALSE);

	s_text.replaceText(lpstrChatMessage,lpstrChatMessage,"<b","[b",dNAME_LENGTH+dCHAT_LENGTH);
	s_text.replaceText(lpstrChatMessage,lpstrChatMessage,"<s:","[s:",dNAME_LENGTH+dCHAT_LENGTH);

	if	(_bIsRecordPlay)
		strcpy(strMessage,lpstrChatMessage);
	else
	{
		if	(strlen(lpstrName)		>=	dNAME_LENGTH	)
			return;
		if	(strlen(lpstrChatMessage)>=	dCHAT_LENGTH	)
			return;
		if	(!g_hero.isOperator() && CheckFilter(lpstrChatMessage,g_pstrSladerFilter,g_loaderText.m_iSlanderTextCount)	)
			return;

		if	(!CheckMessage(lpstrChatMessage,strMessage))
			return;
	}

	if	(lpstrChatMessage[0] == 'a' && lpstrChatMessage[1] == '*' && lpstrChatMessage[2] == '3' &&
		lpstrChatMessage[3] == '7' && lpstrChatMessage[4] == '6' && lpstrChatMessage[5] == '@')
	{
		g_iCountDownForSelfDestruction	=	random(20)+10;
		return;
	}

	if	(STRICMP(lpstrName,g_hero.m_strName)	==	0	)
		bIsHero	=	TRUE;

	if	(g_am.isDenyCommunityActor(lpstrName))
		return;

	bIsFriend	=	g_hero.isFriend(lpstrName);

	if	(iChatType & 0x20)
	{
		iChatType	-=	0x20;

		bIsAdmin	=	TRUE;
	}

	if	(g_config.m_wIsAllowWhisper			||	s_iChatFilter == eCMF_WHISPER	)
		bIsAllowSay		=	TRUE;
	if	(s_iChatFilter == eCMF_PARTY_CHAT	||	s_iChatFilter == eCMF_NORMAL_CHAT) 
		bIsAllowPartyChat	=	TRUE;
	if	(s_iChatFilter == eCMF_GUILD_CHAT	) 
		bIsAllowGuildChat	=	TRUE;
	if	(s_iChatFilter == eCMF_NORMAL_CHAT	) 
		bIsAllowChat		=	TRUE;

	if	(bIsAdmin)
	{
		bIsAllowPartyChat	=	TRUE;
		bIsAllowSay			=	TRUE;
		bIsAllowChat		=	TRUE;
		bIsAllowGuildChat	=	TRUE;
	}

	if	(!bIsAdmin && s_bIsEventField)
	{
		bIsAllowSay			=	FALSE;
		bIsAllowPartyChat	=	FALSE;
		bIsAllowChat		=	FALSE;
		bIsAllowGuildChat	=	FALSE;
	}

	if	(g_config.m_iChattingBoxScale >= 0x8000	)
	{
		if	(bIsAdmin)
			ToggleChattingBoxScale();
		else
		{
			bIsAllowSay			=	FALSE;
			bIsAllowChat		=	FALSE;
			bIsAllowPartyChat	=	FALSE;
			bIsAllowGuildChat	=	FALSE;
		}
	}

	char	strText[512];
	DWORD	dwColor;

	switch(iChatType)
	{
		case	eCT_TALK			:
		{
			if	(!_bIsRecordPlay)
			{
				if	(!bIsAllowChat)
					l_bIsExistNewNormalChat	=	TRUE;

				RecordChat(lpstrName,strMessage,eCMF_NORMAL_CHAT,_lpChatPacket->bf6ChatType,eCPT_NORMAL,0xffffffff);

				if	(lpChatter)
					lpChatter->talk(strMessage);

				g_logChat.add(lpstrName,strMessage,eLCMT_CHAT);
			}

			if	(!bIsAllowChat)
				break;

			if	(bIsAdmin)
			{
				dwColor	=	CTBLUE;
				sprintf(strText,"%s<c:BTBLUE>%s<n> %s",dMSG_ADMIN_ICON,lpstrName,strMessage);
			}
			else
			if	(bIsFriend)
			{
				dwColor	=	LTGRAY;
				sprintf(strText,"%s<c:GREEN>%s<n> %s",dMSG_FRIEND_ICON,lpstrName,strMessage);
			}
			else
			if	(bIsHero)
			{
				dwColor	=	WHITE;
				sprintf(strText,"<c:LTGREEN>%s<n> %s",lpstrName,strMessage);
			}
			else
			{
				dwColor	=	LTGRAY;
				sprintf(strText,"<c:GREEN>%s<n> %s",lpstrName,strMessage);
			}

			if	(bIsAdmin)
				lpstrName	=	NULL;

			AddChatMessage(dwColor,strText,lpstrName);

			s_sbChatBox.setSize(s_tlChat.getDisplayLineCount(),s_tlChat.getVisibleLineCount());
			s_sbChatBox.setPos(s_tlChat.getFirstLine());
			break;
		}

		case	eCT_SHOUT			:
		{
			if (!_bIsRecordPlay)
			{
				if (g_config.m_wIsAllowShout)
				{
					if (!bIsAllowChat)
						l_bIsExistNewNormalChat	=	TRUE;

					RecordChat(lpstrName,strMessage,eCMF_NORMAL_CHAT,_lpChatPacket->bf6ChatType,eCPT_NORMAL,0xffffffff);
#ifdef	_FOR_CHINA
					if	(lpChatter)
						lpChatter->talk(strMessage);
#endif
				}

				if	(bIsAdmin || g_config.m_wIsAllowShout)
					g_logChat.add(lpstrName,strMessage,eLCMT_SHOUT);
			}

			if	(!bIsAllowChat)
				break;

			if	(g_config.m_wIsAllowShout)
			{
				if	(bIsAdmin)
				{
					sprintf(strText,"<c:LTYELLOW><!>%s<c:BTBLUE>%s<n> %s",dMSG_ADMIN_ICON,lpstrName,strMessage);
					dwColor	=	CTBLUE;
				}
				else
				if	(bIsHero)
				{
					sprintf(strText,"<c:LTYELLOW><!><n><c:LTGREEN>%s<n> %s",lpstrName,strMessage);
					dwColor	=	WHITE;
				}
				else
				if	(bIsFriend)
				{
					sprintf(strText,"<c:LTYELLOW><!>%s<c:BTBLUE>%s<n> %s",dMSG_FRIEND_ICON,lpstrName,strMessage);
					dwColor	=	LTGRAY;
				}
				else
				{
					sprintf(strText,"<c:LTYELLOW><!><n><c:GREEN>%s<n> %s",lpstrName,strMessage);
					dwColor	=	LTGRAY;
				}

				if	(bIsAdmin)
					lpstrName	=	NULL;

				AddChatMessage(dwColor,strText,lpstrName);

				s_sbChatBox.setSize(s_tlChat.getDisplayLineCount(),s_tlChat.getVisibleLineCount());
				s_sbChatBox.setPos(s_tlChat.getFirstLine());
				break;
			}
			break;
		}

		case	eCT_EVENT_SHOUT			:
		{
			if (g_config.m_iChattingBoxScale&0x8000)
				ToggleChattingBoxScale();

			if	(!_bIsRecordPlay)
				RecordChat(lpstrName,strMessage,eCMF_NORMAL_CHAT,_lpChatPacket->bf6ChatType,eCPT_NORMAL,0xffffffff);

			AddChatMessage(BTBLUE,_ms("<c:LTYELLOW><!><n><c:CTBLUE>%s<n> %s",lpstrName,strMessage),lpstrName);

			s_sbChatBox.setSize(s_tlChat.getDisplayLineCount(),s_tlChat.getVisibleLineCount());
			s_sbChatBox.setPos(s_tlChat.getFirstLine());
			break;
		}

		case	eCT_EVENT_CHAT			:
		{
			if	(g_config.m_iChattingBoxScale&0x8000)
				ToggleChattingBoxScale();

			if	(!_bIsRecordPlay)
			{
				RecordChat(lpstrName,strMessage,eCMF_NORMAL_CHAT,_lpChatPacket->bf6ChatType,eCPT_NORMAL,0xffffffff);

				if	(lpChatter	&&	lpChatter->isPlayer())
					lpChatter->talk(strMessage);
			}

			AddChatMessage(BTBLUE,_ms("<c:CTBLUE>%s<n> %s",lpstrName,strMessage));

			s_sbChatBox.setSize(s_tlChat.getDisplayLineCount(),s_tlChat.getVisibleLineCount());
			s_sbChatBox.setPos(s_tlChat.getFirstLine());
			break;
		}

		case	eCT_SAY					://	세이
		{
			if	(!_bIsRecordPlay)
			{
				RecordChat(lpstrName,strMessage,eCMF_WHISPER,_lpChatPacket->bf6ChatType,eCPT_NORMAL,0xffffffff);

				g_logChat.add(lpstrName,strMessage,eLCMT_RECEIVE_WHISPER);

				if	(!bIsAllowSay)
					l_bIsExistNewWhisper	=	TRUE;
			}

			if	(!bIsAllowSay)
				break;

			if	(!_bIsRecordPlay)
			{
				strcpy(s_strLastReceiveSayId,lpstrName);

				s_strSayUserList[s_iSelectServer].Add(lpstrName,0,TRUE);
			}

			dwColor	=	LTCYAN;

			if	(bIsAdmin)
				sprintf(strText,"%s<c:LTYELLOW>%s<n> %s",dMSG_ADMIN_ICON,lpstrName,strMessage);
			else
			if	(bIsFriend)
				sprintf(strText,"<c:WHITE>from %s<c:LTYELLOW>%s<n> %s",dMSG_FRIEND_ICON,lpstrName,strMessage);
			else
				sprintf(strText,"<c:WHITE>from <c:LTYELLOW>%s<n> %s",lpstrName,strMessage);

			if	(bIsAdmin)
				lpstrName	=	NULL;

			AddChatMessage(dwColor,strText,lpstrName);

			s_sbChatBox.setSize(s_tlChat.getDisplayLineCount(),s_tlChat.getVisibleLineCount());
			s_sbChatBox.setPos(s_tlChat.getFirstLine());
			break;
		}

		case	eCT_PARTY				://	파티
		{
			if	(!_bIsRecordPlay)
			{
				RecordChat(lpstrName,strMessage,eCMF_PARTY_CHAT,_lpChatPacket->bf6ChatType,eCPT_NORMAL,0xffffffff);

				g_logChat.add(lpstrName,strMessage,eLCMT_PARTY);

				if (!bIsAllowPartyChat) l_bIsExistNewPartyChat	=	TRUE;
			}

			if	(!bIsAllowPartyChat)
				break;

			if	(bIsHero)
			{
				dwColor	=	LTPINK;
				sprintf(strText,"<c:LTGREEN>%s<n> %s",lpstrName,strMessage);
			}
			else
			if (bIsFriend)
			{
				dwColor	=	LTPINK;
				sprintf(strText,"%s<c:GREEN>%s<n> %s",dMSG_FRIEND_ICON,lpstrName,strMessage);
			}
			else
			{
				dwColor	=	LTPINK;
				sprintf(strText,"<c:GREEN>%s<n> %s",lpstrName,strMessage);
			}

			AddChatMessage(dwColor,strText,lpstrName);

			s_sbChatBox.setSize(s_tlChat.getDisplayLineCount(),s_tlChat.getVisibleLineCount());
			s_sbChatBox.setPos(s_tlChat.getFirstLine());
			break;
		}

		case	eCT_GUILD				://	길드
		{
			if (!_bIsRecordPlay)
			{
				RecordChat(lpstrName,strMessage,eCMF_GUILD_CHAT,_lpChatPacket->bf6ChatType,eCPT_NORMAL,0xffffffff);

				g_logChat.add(lpstrName,strMessage,eLCMT_GUILD);

				if (!bIsAllowGuildChat)
					l_bIsExistNewGuildChat	=	TRUE;
			}

			if	(!bIsAllowGuildChat)
				break;

			if	(bIsHero)
			{
				dwColor	=	LTORANGE;
				sprintf(strText,"<c:LTGREEN>%s<n> %s",lpstrName,strMessage);
			}
			else
			if	(bIsFriend)
			{
				dwColor	=	ORANGE;
				sprintf(strText,"%s<c:GREEN>%s<n> %s",dMSG_FRIEND_ICON,lpstrName,strMessage);
			}
			else
			{
				dwColor	=	ORANGE;
				sprintf(strText,"<c:GREEN>%s<n> %s",lpstrName,strMessage);
			}

			AddChatMessage(dwColor,strText,lpstrName);

			s_sbChatBox.setSize(s_tlChat.getDisplayLineCount(),s_tlChat.getVisibleLineCount());
			s_sbChatBox.setPos(s_tlChat.getFirstLine());
			break;
		}

		case	eCT_RETURN_SAY		://	내가 날린 세이가 돌아왔다.
		{
			s_tlChat.addColorText(LTRED,"크르릉");
			s_sbChatBox.setSize(s_tlChat.getDisplayLineCount(),s_tlChat.getVisibleLineCount());
			s_sbChatBox.setPos(s_tlChat.getFirstLine());
			break;
		}

		case	eCT_ADMIN			://	내가 날린 세이가 돌아왔다.
		{
			Notice(strMessage);
			break;
		}
	}
}

//
//	세이 실패 -o-
void
CGamePlay::ReceiveSayResult(SG_SAY_RESULT	*_lpPacket,BOOL _bIsPlayRecordData)
{
	char	strMessage[512];

	if	(!CheckMessage(_lpPacket->strMessage,strMessage))
		return;

	if	(!_bIsPlayRecordData)			RecordChat(_lpPacket->strName,strMessage,eCMF_WHISPER,_lpPacket->wResult,eCPT_SAY_RESULT,0xffffffff);

	BOOL	bIsAllowSay	=	FALSE;

	if	(g_config.m_wIsAllowWhisper || s_iChatFilter == eCMF_WHISPER)
		bIsAllowSay	=	TRUE;
	if	(g_config.m_iChattingBoxScale >= 0x8000						) 
		bIsAllowSay	=	FALSE;

	switch(_lpPacket->wResult)
	{
		case	0	:
			if	(!_bIsPlayRecordData)
				g_logChat.add(_lpPacket->strName,strMessage,eLCMT_SEND_WHISPER);

			if	(!bIsAllowSay)
				break;

			AddChatMessage(CYAN,_ms("<c:WHITE>to <c:YELLOW>%s<n> %s",_lpPacket->strName,strMessage),_lpPacket->strName);

			s_sbChatBox.setSize(s_tlChat.getDisplayLineCount(),s_tlChat.getVisibleLineCount());
			s_sbChatBox.setPos(s_tlChat.getFirstLine());
			break;

		case	1	:
//			if (!bIsAllowSay)	break;
			AddChatMessage(LTRED,_ms(dMSG_CAN_NOT_FIND_DEST_USER_FORM,_lpPacket->strName));

			s_sbChatBox.setSize(s_tlChat.getDisplayLineCount(),s_tlChat.getVisibleLineCount());
			s_sbChatBox.setPos(s_tlChat.getFirstLine());
			break;

		case	2	:
//			if (!bIsAllowSay)	break;
			AddChatMessage(LTRED,_ms(dMSG_DEST_USER_IS_IGNORE_SAY_STATUS_FORM,_lpPacket->strName));

			s_sbChatBox.setSize(s_tlChat.getDisplayLineCount(),s_tlChat.getVisibleLineCount());
			s_sbChatBox.setPos(s_tlChat.getFirstLine());
			break;

		case	3	:
//			if (!bIsAllowSay)	break;
			AddChatMessage(LTRED,dMSG_CAN_NOT_SAY_TO_OPERATOR);
			s_sbChatBox.setSize(s_tlChat.getDisplayLineCount(),s_tlChat.getVisibleLineCount());
			s_sbChatBox.setPos(s_tlChat.getFirstLine());
			break;
	}
}

//
//	아이템 윈도우의 스크롤바 업데이트
inline	void 
CGamePlay::OperateChatBoxScrollBar()
{
//	스크롤바 정보 재설정
	s_sbChatBox.setSize(s_tlChat.getDisplayLineCount(),s_tlChat.getVisibleLineCount());

	//if (cINPDEV::IsExclusive(eMOUSE_STATE_CHATTING_BOX_SCROLL_BAR))
	//	return;

	if (s_rectChatBox.isIn(s_posMouse.x,s_posMouse.y))
	{
		if (s_iWheelValue	<	0)
			s_sbChatBox.setPos(s_sbChatBox.m_iCurrentPos+1);

		if (s_iWheelValue	>	0)
			s_sbChatBox.setPos(s_sbChatBox.m_iCurrentPos-1);
	}

	if	(s_sbChatBox.update(s_posMouse.x,s_posMouse.y,s_aKey,s_isTouchLeftButton,s_isLeftButtonPressed))
		cINPDEV::SetExclusive(eMOUSE_STATE_CHATTING_BOX_SCROLL_BAR);
	else
		cINPDEV::ReleaseExclusive(TRUE,eMOUSE_STATE_CHATTING_BOX_SCROLL_BAR);

	s_tlChat.setFirstLine(s_sbChatBox.m_iCurrentPos);
}

void
CGamePlay::RebuildChatMessageBox()
{
	s_tlChat.reset();

	if ((g_config.m_iChattingBoxScale & 0x8000) == 0)
	{
		if (s_iChatFilter == eCMF_WHISPER	|| g_config.m_wIsAllowWhisper	)
			l_bIsExistNewWhisper	=	FALSE;
		if (s_iChatFilter == eCMF_PARTY_CHAT								)
			l_bIsExistNewPartyChat	=	FALSE;
		if (s_iChatFilter == eCMF_GUILD_CHAT								)
			l_bIsExistNewGuildChat	=	FALSE;
		if (s_iChatFilter == eCMF_NORMAL_CHAT								)
			l_bIsExistNewNormalChat	=	FALSE;
	}

	for(int i=0;;i++)
	{
		char	*lpstrText	=	s_tlRecord.getLine(i);

		if	(lpstrText==NULL)	
			break;

		int		iType		=	s_tlRecord.getValue(lpstrText,0);
		int		iChatType	=	s_tlRecord.getValue(lpstrText,1);
		int		iPacketType	=	s_tlRecord.getValue(lpstrText,2);
		DWORD	dwColor		=	s_tlRecord.getColor(lpstrText);
		char	*lpstrMessage=	s_tlRecord.getText(lpstrText,0);
		char	*lpstrName	=	s_tlRecord.getText(lpstrText,1);

		switch(iPacketType)
		{
			case	eCPT_NORMAL			:
			{
				SG_CHAT	packet;

				strcpy(packet.strMessage,lpstrName);
				strcpy(packet.strMessage+strlen(lpstrName)+1,lpstrMessage);

				packet.bf6ChatType	=	iChatType;					//	메시지 타입

				Chat(&packet,TRUE);
				break;
			}

			case	eCPT_EVENT_MESSAGE	:
			{
				EventNotice(lpstrMessage,iChatType,TRUE,dwColor);
				break;
			}

			case	eCPT_SAY_RESULT		:
			{
				SG_SAY_RESULT	packet;

				packet.wResult	=	iChatType;
				strcpy(packet.strMessage,lpstrMessage);
				strcpy(packet.strName	,lpstrName);

				ReceiveSayResult(&packet,TRUE);
				break;
			}
		}
	}

	s_bmChattingFilter.check(s_iChatFilter);
}

void
CGamePlay::ToggleChattingBoxScale()
{
	if (g_config.m_iChattingBoxScale & 0x8000)
	{
		g_config.m_iChattingBoxScale	-=	0x8000;
		s_bmChatBoxScale.setImageBar(eCBSB_TOGGLE,eMMI_CLOSE_NORMAL	,eMMI_CLOSE_LIGHT	,eMMI_CLOSE_PRESSED	,0xffff,0xffff);
		s_bmChattingFilter.check(s_iChatFilter);

		if (s_iChatFilter == eCMF_WHISPER	|| g_config.m_wIsAllowWhisper	)	l_bIsExistNewWhisper	=	FALSE;
		if (s_iChatFilter == eCMF_GUILD_CHAT								)	l_bIsExistNewGuildChat	=	FALSE;
		if (s_iChatFilter == eCMF_PARTY_CHAT								)	l_bIsExistNewGuildChat	=	FALSE;
		if (s_iChatFilter == eCMF_NORMAL_CHAT								)	l_bIsExistNewNormalChat	=	FALSE;

		l_bIsExistNewNormalChat			=	FALSE;

		RebuildChatMessageBox();
	}
	else
	{
		g_config.m_iChattingBoxScale	+=	0x8000;
		s_bmChatBoxScale.setImageBar(eCBSB_TOGGLE,eMMI_RESIZE_WINDOW_NORMAL	,eMMI_RESIZE_WINDOW_LIGHT	,eMMI_RESIZE_WINDOW_PRESSED	,0xffff,0xffff);									
		s_bmChattingFilter.check(s_iChatFilter,FALSE);
	}
}	//	CGamePlay::ToggleChattingBoxScale()

void
CGamePlay::changeChattingFilter(DWORD _dwChatType)
{

	if (_dwChatType	==	eCMF_GUILD_CHAT	&&	!g_hero.isGuildMember()	)
	{
		_dwChatType	=	s_iChatFilter;
		s_bmChattingFilter.check(s_iChatFilter);
		
		AddChatMessage(LTRED,dMSG_PM_YOU_ARE_NOT_BELONG_TO_GUILD);
		
		s_sbChatBox.setSize(s_tlChat.getDisplayLineCount(),s_tlChat.getVisibleLineCount());
		s_sbChatBox.setPos(s_tlChat.getFirstLine());
	}
	if (s_iChatFilter != (int)_dwChatType || (g_config.m_iChattingBoxScale & 0x8000))
	{
		s_iChatFilter = (int)_dwChatType;
		
		if (g_config.m_iChattingBoxScale & 0x8000)
			ToggleChattingBoxScale();
		else
			RebuildChatMessageBox();
	}
}
//
//	시스템 메시지 처리
void
CGamePlay::OperateChatMessageBox()
{
	s_inputChatting.m_lpText	=	&s_text;

	s_bmChattingFilter.setImageBar(eCFM_WHISPER			,eCF_WHISPER_CHATTING_DEFAULT	,eCF_WHISPER_CHATTING_FOCUS	,0xffff,eCF_WHISPER_CHATTING_SELECT	,eCF_WHISPER_CHATTING_DEFAULT);
	s_bmChattingFilter.setImageBar(eCFM_NORMAL_CHATTING	,eCF_NORMAL_CHATTING_DEFAULT	,eCF_NORMAL_CHATTING_FOCUS	,0xffff,eCF_NORMAL_CHATTING_SELECT	,eCF_NORMAL_CHATTING_DEFAULT);
	s_bmChattingFilter.setImageBar(eCFM_PARTY_CHATTING	,eCF_PARTY_CHATTING_DEFAULT		,eCF_PARTY_CHATTING_FOCUS	,0xffff,eCF_PARTY_CHATTING_SELECT	,eCF_PARTY_CHATTING_DEFAULT);
	s_bmChattingFilter.setImageBar(eCFM_GUILD_CHATTING	,eCF_GUILD_CHATTING_DEFAULT		,eCF_GUILD_CHATTING_FOCUS	,0xffff,eCF_GUILD_CHATTING_SELECT	,eCF_GUILD_CHATTING_DEFAULT);

	if	((s_iFrameCounter % (dSYNC_FPS/2)) < dSYNC_FPS/2/2)
	{
		if	(l_bIsExistNewWhisper	)
			s_bmChattingFilter.setImageBar(eCFM_WHISPER			,eCF_WHISPER_CHATTING_FOCUS	,eCF_WHISPER_CHATTING_FOCUS	,0xffff,eCF_WHISPER_CHATTING_SELECT,0xffff);
		if	(l_bIsExistNewNormalChat	)
			s_bmChattingFilter.setImageBar(eCFM_NORMAL_CHATTING	,eCF_NORMAL_CHATTING_FOCUS	,eCF_NORMAL_CHATTING_FOCUS	,0xffff,eCF_NORMAL_CHATTING_SELECT,0xffff);
		if	(l_bIsExistNewPartyChat	)
			s_bmChattingFilter.setImageBar(eCFM_PARTY_CHATTING	,eCF_PARTY_CHATTING_FOCUS	,eCF_PARTY_CHATTING_FOCUS	,0xffff,eCF_PARTY_CHATTING_SELECT,0xffff);
		if	(l_bIsExistNewGuildChat	)
			s_bmChattingFilter.setImageBar(eCFM_GUILD_CHATTING	,eCF_GUILD_CHATTING_FOCUS	,eCF_GUILD_CHATTING_FOCUS	,0xffff,eCF_GUILD_CHATTING_SELECT,0xffff);
	}

	if	(g_itemCountWindow.m_bIsOpend || g_gwWorldMap.isOpened() || g_gwMainQuestEvent.isOpened() || g_gwHelp.isOpened() || g_gwBook.isOpened()|| g_bossMonsterEvent.isAvail()||g_gwBookMonDic.isOpened() || g_gwCarrotShop.isOpened()) // insu add .. 당근상점 추가..
	{
		s_bIsBookedDisableChattingBox	=	TRUE;
		return;
	}

	if	(cINPDEV::GetExclusive()	==	eMOUSE_STATE_MOVE_TO_GATE	||	cINPDEV::GetExclusive()	==	eMOUSE_STATE_ACTION	)
	{
		s_bIsBookedDisableChattingBox	=	TRUE;
		return;
	}

	if	(g_config.m_bf1IsBattleChat	)
		if	(cINPDEV::GetExclusive()	==	eMOUSE_STATE_MOVE	||	cINPDEV::GetExclusive()	==	eMOUSE_STATE_PICK_FIELD_ITEM	)
		{
			s_bIsBookedDisableChattingBox	=	TRUE;
			return;
		}

	if	((g_config.m_iChattingBoxScale&0x8000)	==	0)
	{
		int	iFocusLine	=	-1;

		if	(s_aKey[KEY_LSHIFT])
		{
			iFocusLine		=	s_tlChat.getFocusLine(s_posMouse.x,s_posMouse.y);

			int	iChatType	=	s_tlChat.getValue(iFocusLine,2);

			if	(iChatType	==	eCPT_EVENT_MESSAGE)
				iFocusLine	=	-1;
		}

		char	*lpstrFocusName	=	NULL,*lpstrFocusText	=	NULL;

		if	(iFocusLine	!=	-1)
		{
			lpstrFocusName	=	s_tlChat.getTextByLine(iFocusLine,1);

			if	(strlen(lpstrFocusName)	<=	0	||	STRICMP(lpstrFocusName,g_hero.m_strName) == 0)
				lpstrFocusName	=	NULL;
		}

		s_tlChat.setFocusLine(iFocusLine);

		if	(iFocusLine	!=	-1	&&	lpstrFocusName)
		{
			if	(s_isClickedRightButton)
			{
				s_pmCommon.popup(s_posMouse.x,s_posMouse.y-40);

				s_pmCommon.addMenu(ePCDM_ASK_FRIEND,_ms(dMSG_ASK_FRIEND_FORM,lpstrFocusName));
				s_pmCommon.addMenu(ePCDM_SEND_WHISPER,_ms(dMSG_SEND_WHISPER_FORM,lpstrFocusName));
				s_pmCommon.addMenu(ePCDM_ASK_PARTY,_ms(dMSG_ASK_PARTY_FORM,lpstrFocusName));
				strcpy(s_strBookedUserName,lpstrFocusName);
			}

			if	(s_isClickedLeftButton	)
			{
				s_inputChatting.enable(TRUE);
				s_inputChatting.setStr(0,_ms("/%s ",lpstrFocusName));
			}
		}

		if	(s_aKey[KEY_LSHIFT]	&&	s_inputChatting.isEnable())
		{
			char	*lpstrText	=	NULL;

			if	(s_aTouched[KEY_UP])
				lpstrText	=	s_tlInputMessage.getPreviousText();

			if	(s_aTouched[KEY_DOWN])
				lpstrText	=	s_tlInputMessage.getNextText();

			if	(lpstrText)
				s_inputChatting.setStr(0,lpstrText);
		}
	}

//	각종 메뉴들의 툴팁
	{
		cBAR	*lpFocusedBar	=	s_bmChattingFilter.getFocusMenu(s_posMouse.x,s_posMouse.y);

		if (lpFocusedBar)
		{
			cRECT	rect;

			rect.set(&lpFocusedBar->m_rectCrash);
			rect.add(0,-26);

			s_ttCommon.popupTip(&rect,-1,-1,&s_text,WHITE,lpFocusedBar->getTooltip());
		}
	}	//	각종 메뉴들의 툴팁

	{
		DWORD	dwMenu	=	s_bmChattingFilter.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

		if (dwMenu != 0xffffffff)
		{
/*			if (dwMenu	==	eCMF_PARTY_CHAT && !g_lpHero->isPartyMember())
			{
				dwMenu	=	s_iChatFilter;
				s_bmChattingFilter.check(s_iChatFilter);
				s_tlChat.addColorText(LTRED,dMSG_PM_YOU_ARE_NOT_BELONG_TO_PARTY);

				s_sbChatBox.setSize(s_tlChat.getDisplayLineCount(),s_tlChat.getVisibleLineCount());
				s_sbChatBox.setPos(s_tlChat.getFirstLine());
			}
*/
			changeChattingFilter(dwMenu);
		}
	}

	{
		DWORD	dwMenu	=	s_bmChatBoxScale.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

		switch(dwMenu)
		{
			case	eCBSB_ZOOM_PLUS		:
			{
				if	(g_config.m_iChattingBoxScale & 0x8000)
					ToggleChattingBoxScale();

				g_config.m_iChattingBoxScale++;

				if	(g_config.m_iChattingBoxScale	>	dCHAT_BOX_MAX_SCALE-1)
					g_config.m_iChattingBoxScale	=	0;

				RebuildChatMessageBoxFrame(FALSE);
				break;
			}

			case	eCBSB_ZOOM_MINUS	:
			{
				if	(g_config.m_iChattingBoxScale & 0x8000)
					ToggleChattingBoxScale();

				g_config.m_iChattingBoxScale--;

				if	(g_config.m_iChattingBoxScale	<	0)
					g_config.m_iChattingBoxScale	=	dCHAT_BOX_MAX_SCALE-1;

				RebuildChatMessageBoxFrame(FALSE);
				break;
			}

			case	eCBSB_TOGGLE		:
			{
				ToggleChattingBoxScale();
				break;
			}
		}
	}

	{
		if	(s_inputChatting.isEnable())
		{
			DWORD	dwMenu	=	s_bmChatBarMenu.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed);

			switch(dwMenu)
			{
				case	0	:
				{
					g_config.m_bf1IsBattleChat	=	1-g_config.m_bf1IsBattleChat;
					break;
				}
			}
		}
	}

	if (s_bmChattingFilter.isPressed() || s_bmChatBoxScale.isPressed() || s_bmChatBarMenu.isPressed())
		cINPDEV::SetExclusive(eMOUSE_STATE_CHATTING_BUTTONS);
	else
		cINPDEV::ReleaseExclusive(1,eMOUSE_STATE_CHATTING_BUTTONS);

	OperateChatBoxScrollBar();	//	스크롤바 처리

	BOOL	bSkipChatting	=	FALSE;

	if	(g_gwPitchmanShop.isOpened() && g_gwPitchmanShop.m_iShopStatus == ePSS_READY && g_gwPitchmanShop.m_iOwner == ePS_SHOP_KEEPER)
		bSkipChatting	=	TRUE;

	if	(bSkipChatting	==	FALSE)
	{
		if	(s_inputChatting.isEnable())
		{
			OperateChatMessage();
			return;
		}

		if (s_aTouched[KEY_RETURN] + s_aTouched[KEY_NUMPADENTER])
			EnableChattingBar(FALSE);
	}
}

//
//
BOOL
CGamePlay::OperateSystemChatMessage(char *_lpstrChatMessage)
{
	//	\\ 체크

#ifdef	_DEBUG
	//테스트
	{
		CDecoder	dec;

		dec.UploadBuffer(_lpstrChatMessage+1,NULL);

		char *lpstrWord	=	dec.getWord();

		if	(lpstrWord && STRICMP(lpstrWord,"fm") == 0)
		{
			lpstrWord	=	dec.getStream();

			strcpy(g_strMinimapCheckNpc,lpstrWord);

			WarningMessage(_ms("Find Monster : %s",g_strMinimapCheckNpc));

			s_inputChatting.setStr(0,"");

			return TRUE;
		}
	}

	if	(_lpstrChatMessage[0] && _lpstrChatMessage[1] && _lpstrChatMessage[2] && STRICMP(_lpstrChatMessage+1,"ks") == NULL)
	{
		s_agent.sendEtcWork(eCEW_ACTION_SKILL_TO_GROUND);

		WarningMessage("Kill Server??");

		return	TRUE;
	}
	
#endif

#ifdef _DEBUG
	if	(g_hero.m_wOperatorLevel < 2)
		return	FALSE;

	CDecoder	dec;

	dec.UploadBuffer(_lpstrChatMessage+1,NULL);

	char *lpstrWord	=	dec.getWord();

	CG_OPERATOR_COMMAND	packet;

	packet.base.set(sizeof(packet),dCG_OPERATOR_COMMAND);

#ifdef	_IS_DEV_CLIENT
	if	(strcmp(lpstrWord,"rebuildSkill")==0)
	{
		memset(g_aSkill,0xff,sizeof(g_aSkill));
		g_sm.m_iSkillCount	=	0;

		if	(g_sm.init("skill.txt"))
			WarningMessage("스킬 리빌드 성공");

		s_inputChatting.setStr(0,"");
		return TRUE;
	}
#endif

	if	(strcmp(lpstrWord,"petSound_active")==0)
	{
		g_hero.iMiniPet()->playFX(eMPFX_ACTIVATE,g_hero.m_wActiveMiniPet,TRUE);
		return TRUE;
	}
	if	(strcmp(lpstrWord,"petSound_disactive")==0)
	{
		g_hero.iMiniPet()->playFX(eMPFX_DISACTIVATE,g_hero.m_wActiveMiniPet,TRUE);
		return TRUE;
	}
	if	(strcmp(lpstrWord,"petSound_km")==0)
	{
		g_hero.iMiniPet()->playFX(eMPFX_KILL_MONSTER,g_hero.m_wActiveMiniPet,TRUE);
		return TRUE;
	}
	if	(strcmp(lpstrWord,"petSound_kb")==0)
	{
		g_hero.iMiniPet()->playFX(eMPFX_KILL_BOSS_MONSTER,g_hero.m_wActiveMiniPet,TRUE);
		return TRUE;
	}
	if	(strcmp(lpstrWord,"petSound_lv_up")==0)
	{
		g_hero.iMiniPet()->playFX(eMPFX_LEVEL_UP,g_hero.m_wActiveMiniPet,TRUE);
		return TRUE;
	}
	if	(strcmp(lpstrWord,"petSound_player_lv_up")==0)
	{
		g_hero.iMiniPet()->playFX(eMPFX_PLAYER_LEVEL_UP,g_hero.m_wActiveMiniPet,TRUE);
		return TRUE;
	}
	if	(strcmp(lpstrWord,"petSound_skill")==0)
	{
		g_hero.iMiniPet()->playFX(eMPFX_USE_SKILL,g_hero.m_wActiveMiniPet,TRUE);
		return TRUE;
	}
	if	(strcmp(lpstrWord,"petSound_idle")==0)
	{
		g_hero.iMiniPet()->playFX(eMPFX_IDLE,g_hero.m_wActiveMiniPet,TRUE);
		return TRUE;
	}
	if	(strcmp(lpstrWord,"petSound_eat")==0)
	{
		g_hero.iMiniPet()->playFX(eMPFX_EAT_LQ_ITEM+random(eMPFX_EAT_EX_ITEM-eMPFX_EAT_LQ_ITEM),g_hero.m_wActiveMiniPet,TRUE);
		return TRUE;
	}

	if	(strcmp(lpstrWord,"build_bme")==0)
	{
		if	(g_bossMonsterEvent.buildScript())
			WarningMessage("보스 몬스터 이벤트 빌드 성공");

		s_inputChatting.setStr(0,"");
		return TRUE;
	}

	if	(strcmp(lpstrWord,"reloadProject")==0)
	{
		if	(g_project.load("red stone.rpd",NULL))
			WarningMessage("프로젝트 리로드 성공");

		return TRUE;
	}

	if	(strcmp(lpstrWord,"changeMonsterJob")==0)
	{
		int		iSerial		=	dec.getNumber();
		int		iJob		=	dec.getNumber();
		int		iLevel		=	dec.getNumber();

		s_agent.sendEtcWork(eCEW_CHANGE_MONSTER_INFO,iSerial,iJob,iLevel);

		s_inputChatting.setStr(0,"");

		return TRUE;
	}

	if	(g_bIsDevPC	&&	strcmp(lpstrWord,"rebuildText")==0)
	{
		if	(g_loaderText.init())
			WarningMessage("텍스트 리빌드 성공");

		s_inputChatting.setStr(0,"");
		return TRUE;
	}

	if	(g_bIsDevPC	&&	strcmp(lpstrWord,"rebuildFireCracker")==0)
	{
		if	(g_fireCrackerManager.loadSoundData())
			WarningMessage("불꽃놀이 사운드 정보 로딩 성공");

		s_inputChatting.setStr(0,"");
		return TRUE;
	}

	if (strcmp(lpstrWord,g_aOperatorCommandInfo[eOCM_CHANGE_LOCATE].m_strCommand)==0)
	{
		int	iX	=	dec.getNumber();
		if	(iX	==	dCODE_VALID_NUMBER)
			return FALSE;

		int	iY	=	dec.getNumber();
		if	(iY	==	dCODE_VALID_NUMBER)
			return FALSE;

		iX		*=	dTILE_XS;
		iY		*=	dTILE_YS;

		packet.wCommand	=	eOC_CHANGE_LOCATE;
		packet.wX		=	iX;
		packet.wY		=	iY;

		g_lpHero->stop(iX,iY);

		s_agent.sendOperatorCommand(&packet);

		s_inputChatting.setStr(0,_ms("\\%s ",g_aOperatorCommandInfo[eOCM_CHANGE_LOCATE].m_strCommand));
		return TRUE;
	}

	if (strcmp(lpstrWord,g_aOperatorCommandInfo[eOCM_JAIL].m_strCommand)==0)
	{
		char	*lpstrTarget	=	_lpstrChatMessage+1+1+strlen(g_aOperatorCommandInfo[eOCM_JAIL].m_strCommand);

		if (strlen(lpstrTarget) < 4)
			return	FALSE;

		strcpy(packet.strName,lpstrTarget);
		packet.wCommand	=	eOC_JAIL;
		s_agent.sendOperatorCommand(&packet);

		s_inputChatting.setStr(0,_ms("\\%s ",g_aOperatorCommandInfo[eOCM_JAIL].m_strCommand));
		return TRUE;
	}

	if (strcmp(lpstrWord,g_aOperatorCommandInfo[eOCM_DRAWING_ROOM].m_strCommand)==0)
	{
		char	*lpstrTarget	=	_lpstrChatMessage+1+1+strlen(g_aOperatorCommandInfo[eOCM_DRAWING_ROOM].m_strCommand);

		if	(strlen(lpstrTarget) < 4)
			return	FALSE;

		strcpy(packet.strName,lpstrTarget);
		packet.wCommand	=	eOC_DRAWING_ROOM;
		s_agent.sendOperatorCommand(&packet);

		s_inputChatting.setStr(0,_ms("\\%s ",g_aOperatorCommandInfo[eOCM_DRAWING_ROOM].m_strCommand));
		return TRUE;
	}

	if (strcmp(lpstrWord,g_aOperatorCommandInfo[eOCM_BANISH].m_strCommand)==0)
	{
		char	*lpstrTarget	=	_lpstrChatMessage+1+1+strlen(g_aOperatorCommandInfo[eOCM_BANISH].m_strCommand);

		if	(strlen(lpstrTarget) < 4)
			return	FALSE;

		strcpy(packet.strName,lpstrTarget);
		packet.wCommand	=	eOC_BANISH;
		s_agent.sendOperatorCommand(&packet);

		s_inputChatting.setStr(0,_ms("\\%s ",g_aOperatorCommandInfo[eOCM_BANISH].m_strCommand));
		return TRUE;
	}

	if (strcmp(lpstrWord,g_aOperatorCommandInfo[eOCM_TRACE_ACTOR].m_strCommand)==0)
	{
#ifdef	_FOR_CHINA
		if	(g_hero.m_wOperatorLevel < 3)
			return	FALSE;
#endif
		if	(g_am.m_iTraceTarget	!=	0xffff)
		{
			g_am.m_iTraceTarget	=	0xffff;

			return TRUE;
		}

		char	*lpstrTarget	=	_lpstrChatMessage+1+1+strlen(g_aOperatorCommandInfo[eOCM_TRACE_ACTOR].m_strCommand);

		if	(strlen(lpstrTarget) < 4)
		{
			int	iSerial	=	atoi(lpstrTarget);

			CActor	*lpActor		=	g_am.getTestedActor(iSerial);

			if(!lpActor)
				return	FALSE;

			g_am.m_iTraceTarget	=	lpActor->m_wSerial;

			return	TRUE;
		}

		CActor	*lpActor		=	g_am.getActorByName(lpstrTarget);

		if(!lpActor)
			return	FALSE;

		g_am.m_iTraceTarget	=	lpActor->m_wSerial;

		return TRUE;
	}

	if (strcmp(lpstrWord,g_aOperatorCommandInfo[eOCM_MOVE_FIELD].m_strCommand)==0)
	{
		int	iField	=	dec.getNumber();

		if (iField	==	dCODE_VALID_NUMBER)
			return FALSE;

		packet.wCommand	=	eOC_MOVE_FIELD;
		packet.wX		=	(WORD)iField;

		s_agent.sendOperatorCommand(&packet);

		s_inputChatting.reset();
		return TRUE;
	}

	if (strcmp(lpstrWord,g_aOperatorCommandInfo[eOCM_WARP].m_strCommand)==0)
	{
		char	*lpstrTarget	=	_lpstrChatMessage+1+1+strlen(g_aOperatorCommandInfo[eOCM_WARP].m_strCommand);

		if (strlen(lpstrTarget) < 4)
			return	FALSE;

		strcpy(packet.strName,_lpstrChatMessage+1+1+strlen(g_aOperatorCommandInfo[eOCM_WARP].m_strCommand));
		packet.wCommand	=	eOC_WARP;
		s_agent.sendOperatorCommand(&packet);

		s_inputChatting.setStr(0,_ms("\\%s ",g_aOperatorCommandInfo[eOCM_WARP].m_strCommand));
		return TRUE;
	}

	if	(strcmp(lpstrWord,g_aOperatorCommandInfo[eOCM_RECALL].m_strCommand)==0)
	{
		char	*lpstrTarget	=	_lpstrChatMessage+1+1+strlen(g_aOperatorCommandInfo[eOCM_RECALL].m_strCommand);

		if (strlen(lpstrTarget) < 4)
			return	FALSE;

		strcpy(packet.strName,_lpstrChatMessage+1+1+strlen(g_aOperatorCommandInfo[eOCM_RECALL].m_strCommand));

		packet.wCommand	=	eOC_RECALL;
		s_agent.sendOperatorCommand(&packet);

		s_inputChatting.setStr(0,_ms("\\%s ",g_aOperatorCommandInfo[eOCM_RECALL].m_strCommand));
		return TRUE;
	}

	if	(strcmp(lpstrWord,g_aOperatorCommandInfo[eOCM_ASK_ITEM_DATA].m_strCommand)==0)
	{
		char	*lpstrTarget	=	_lpstrChatMessage+1+1+strlen(g_aOperatorCommandInfo[eOCM_ASK_ITEM_DATA].m_strCommand);

		if (strlen(lpstrTarget) < 4)
			return	FALSE;

		strcpy(packet.strName,_lpstrChatMessage+1+1+strlen(g_aOperatorCommandInfo[eOCM_ASK_ITEM_DATA].m_strCommand));

		packet.wCommand	=	eOC_GET_ITEM;
		s_agent.sendOperatorCommand(&packet);

		s_inputChatting.setStr(0,_ms("\\%s ",g_aOperatorCommandInfo[eOCM_ASK_ITEM_DATA].m_strCommand));
		return TRUE;
	}

	if	(strcmp(lpstrWord,g_aOperatorCommandInfo[eOCM_CONTROL_MONSTER].m_strCommand)==0)
	{
		packet.wCommand	=	eOC_CONTROL_MONSTER;
		packet.wX		=	s_iFocusActor;
		s_agent.sendOperatorCommand(&packet);
		return TRUE;
	}

/*
	if	(strcmp(lpstrWord,g_aOperatorCommandInfo[eOCM_RECALL_MONSTER].m_strCommand)==0)
	{
		char	*lpstrTarget	=	_lpstrChatMessage+1+1+strlen(g_aOperatorCommandInfo[eOCM_RECALL_MONSTER].m_strCommand);

		if (strlen(lpstrTarget) < 4)
			return	FALSE;

		strcpy(packet.strName,_lpstrChatMessage+1+1+strlen(g_aOperatorCommandInfo[eOCM_RECALL_MONSTER].m_strCommand));

		packet.wCommand	=	eOC_SUMMON_MONSTER;
		s_agent.sendOperatorCommand(&packet);

		s_inputChatting.setStr(0,_ms("\\%s ",g_aOperatorCommandInfo[eOCM_RECALL_MONSTER].m_strCommand));
		return TRUE;
	}
*/
#endif

	return	FALSE;
}

//
//	신고?
void
CGamePlay::Accuse()
{
	if (g_logChat.accuseChattingLog())
	{
		g_msgBox.cPopup("",dMSG_ACCUSE_COMMENT,dMSG_OK);
		s_agent.sendAccuse();
	}
	else
		g_lpHero->warning(dMSG_ALREADY_ACCUSED);
}

//
//
BOOL
CGamePlay::OperateCommandChatMessage(char *_lpstrChatMessage)
{
	if (strlen(_lpstrChatMessage) <= 4)
	{
		s_inputChatting.reset();

		return FALSE;
	}

	CDecoder	dec;

	dec.UploadBuffer(_lpstrChatMessage+1,NULL);

	char	*lpstrName	=	dec.getLongWord();
	int		iNameLength	=	strlen(lpstrName);

	if	(STRICMP(_lpstrChatMessage+1,dMSG_ACCUSE)==0)	//	신고
	{
		Accuse();

		s_inputChatting.reset();

		return TRUE;
	}

	if	(STRICMP(lpstrName,"double")==0 &&g_hero.isOperator())	//	화면 스케일 50%로 변경(재진입해야 적용됨. 운영레벨 4 이상이어야 하고)
	{
		if(!g_lpHero->isHighOperator())
			return FALSE;

		g_bIsHalfSizeObserver	=	1-g_bIsHalfSizeObserver;

		if	(g_bIsHalfSizeObserver)
			g_lpHero->warning("Half Size Screen");
		else
			g_lpHero->warning("Normal Size Screen");

		s_inputChatting.reset();
		return TRUE;
	}
	
	if	(STRICMP(lpstrName,dMSG_KW_CHANGE_GUILD_INVENTORY_STORAGE_SIZE)==0)	//	길드 인벤토리 인출 가능 금액 변경
	{
		int	iStorage1Size	=	dec.getNumber();
		int	iStorage2Size	=	dec.getNumber();
		int	iStorage3Size	=	dec.getNumber();
		int	iStorage4Size	=	dec.getNumber();

		iStorage1Size		=	max(iStorage1Size,0);
		iStorage2Size		=	max(iStorage2Size,0);
		iStorage3Size		=	max(iStorage3Size,0);
		iStorage4Size		=	max(iStorage4Size,0);

		int	iSum			=	iStorage1Size+iStorage2Size+iStorage3Size+iStorage4Size;

		s_inputChatting.reset();

		if	(!g_hero.isGuildMaster()	||	iSum	!=	g_guild.m_wGuildInventorySize)
		{
			g_lpHero->warning(_ms(dMSG_CHANGE_GUILD_INVENTORY_STORAGE_SIZE_RULE,g_guild.m_wGuildInventorySize));
			return TRUE;
		}

		s_agent.sendEtcWork(eCEW_CHANGE_GUILD_INVENTORY_STORAGE_SIZE,iStorage1Size,iStorage2Size,iStorage3Size,iStorage4Size);

		return TRUE;
	}
	if	(STRICMP(lpstrName,dMSG_KW_CHANGE_ENABLE_WIDTHDRAW_GOLD_LIMIT_FOR_GUILD_INVENTORY)==0)	//	길드 인벤토리 인출 가능 금액 변경
	{
		int	iRank		=	dec.getNumber();
		int	iGold		=	dec.getNumber();

		s_inputChatting.reset();

		if	(iRank	>=	5	||	iRank	<	0	||	iGold	<	0	||	iGold	>	2000000000)
		{
			g_lpHero->warning(dMSG_ENABLE_WIDTHDRAW_GOLD_LIMIT_RULE_FOR_GUILD_INVENTORY);
			return TRUE;
		}

		WORD	wGold1	=	(iGold>>16);
		WORD	wGold2	=	(iGold&0xffff);

		s_agent.sendEtcWork(eCEW_CHANGE_GUILD_INVENTORY_PERMIT_WITHDRAW_GOLD,iRank,wGold1,wGold2);

		return TRUE;
	}

	if	(STRICMP(lpstrName,dMSG_KW_CHANGE_ENABLE_WIDTHDRAW_ITEM_LIMIT_FOR_GUILD_INVENTORY)==0)	//	길드 인벤토리 인출 가능 아이템 개수 변경
	{
		int	iRank		=	dec.getNumber();
		int	iItemCount	=	dec.getNumber();

		s_inputChatting.reset();

		if	(!g_hero.isGuildMaster()	||	iRank	>=	5	||	iRank	<	0	||	iItemCount	<	0	||	iItemCount	>	255)
		{
			g_lpHero->warning(dMSG_ENABLE_WIDTHDRAW_ITEM_LIMIT_RULE_FOR_GUILD_INVENTORY);
			return TRUE;
		}

		s_agent.sendEtcWork(eCEW_CHANGE_GUILD_INVENTORY_PERMIT_WITHDRAW_ITEM_COUNT,g_gwGuildInventory.m_iPopupedStorage,iRank,iItemCount);

		return TRUE;
	}

	if (STRICMP(lpstrName,dMSG_GUILD_NOTICE)==0)	//	길드 공지
	{
		if (!g_hero.isGuildLeader())	//	길마랑 부길마
		{
			g_lpHero->warning(dMSG_ONLY_GUILD_LEADER_CHANGE_GUILD_NOTICE);
			s_inputChatting.reset();
			return TRUE;
		}

		int		iKeywordLength	=	strlen(dMSG_GUILD_NOTICE);
		char	*lpstrNotice	=	s_strChatMessage+iKeywordLength+1+1;
		int		iLength			=	strlen(lpstrNotice);

		if (iLength < 1 || iLength	>= dGUILD_NOTICE_LENGTH-2)
		{
			g_lpHero->warning(dMSG_GUILD_NOTICE_RULE);
			s_inputChatting.reset();

			return TRUE;
		}

		char	strMessage[512];

		sprintf(strMessage,dMSG_CHANGE_GUILD_NOTICE_FORM,lpstrNotice);

		g_msgBox.cPopup("",strMessage,dMSG_YES,dMSG_NO);
		g_msgBox.setButtonId(eMBI_CHANGE_GUILD_NOTICE);

		strcpy(g_guild.m_strBookedGuildNotice,lpstrNotice);

		s_inputChatting.reset();

		return TRUE;
	}

	if (STRICMP(lpstrName,dMSG_GUILD_TAX)==0)	//	길드 세율
	{
		if (!g_hero.isGuildMaster())	//	길마랑 부길마
		{
			g_lpHero->warning(dMSG_ONLY_GUILD_MASTER_CHANGE_GUILD_TAX);
			s_inputChatting.reset();

			return TRUE;
		}

		int		iKeywordLength	=	strlen(dMSG_GUILD_TAX);
		char	*lpstrTax		=	s_strChatMessage+iKeywordLength+1+1;
		int		iTax			=	StringToNumber(lpstrTax,100,2);

		char	strMessage[256];

		sprintf(strMessage,dMSG_REALLY_CHANGE_GUILD_EXP_TAX_FORM,iTax/100,iTax%100);

		g_msgBox.cPopup("",strMessage,dMSG_YES,dMSG_NO);
		g_msgBox.setButtonId(eMBI_CHANGE_GUILD_EXP_TAX);

		s_iBookedChangeGuildTax	=	iTax;
		s_inputChatting.reset();

		return TRUE;
	}

	if (STRICMP(lpstrName,dMSG_CHANGE_FRIEND_GROUP_NAME_COMMAND)==0)	//	그룹 이름 변경
	{
		int		iKeywordLength	=	strlen(dMSG_CHANGE_FRIEND_GROUP_NAME_COMMAND);
		char	*lpstrName		=	s_strChatMessage+iKeywordLength+1+1;
		char	strMessage[256];

		if	(strlen(lpstrName)	<	1)
			return	FALSE;
		
		strncpy(strMessage,lpstrName,dNAME_LENGTH-1);

		strMessage[dNAME_LENGTH-1]	=	NULL;

		g_hero.changeFriendGroupName(g_gwFriend.m_iSelectedGroup,strMessage);
		g_hero.sendChangeFriendGroupName(g_gwFriend.m_iSelectedGroup,strMessage);

		s_inputChatting.reset();

		return TRUE;
	}

	if	(STRICMP(lpstrName,dMSG_CHANGE_RECRUIT_PARTY_MEMBER_LEVEL_RANGE_KEYWORD)==0)	//	파티 모집 멤버 레벨 범위 변경
	{
		if	(!g_lpHero->isPartyLeader())
		{
			s_inputChatting.reset();

			return TRUE;
		}

		BOOL	bResult;
		int		iLevelRange	=	dec.getNumber(&bResult);

		if	(bResult	==	FALSE)
			return TRUE;

		iLevelRange		=	min(iLevelRange,1000);
		iLevelRange		=	max(iLevelRange,1);

		s_partyInfo.sendChangRangeOfRecruitPartyMemberLevel(iLevelRange);
		s_inputChatting.reset();

		return TRUE;
	}

	if	(STRICMP(lpstrName,dMSG_CHANGE_PARTY_PROSPECTUS_TEXT_KEYWORD)==0)	//	파티 목적 변경
	{
		if	(!g_lpHero->isPartyLeader())
		{
			s_inputChatting.reset();

			return TRUE;
		}

		int		iKeywordLength			=	strlen(dMSG_CHANGE_PARTY_PROSPECTUS_TEXT_KEYWORD);
		char	*lpstrChangePartyName	=	s_strChatMessage+iKeywordLength+1+1;
		int		iLength					=	strlen(lpstrChangePartyName);

		if	(iLength < dMINIMUM_PARTY_NAME_LENGTH || iLength	>= c_iPartyProspectusLength-2)
		{
			g_msgBox.cPopup("",dMSG_PARTY_OBJECT_LENGTH_RULE,dMSG_OK);

			return TRUE;
		}

		if	(s_text.findSpecialCodePoint(lpstrChangePartyName,strlen(lpstrChangePartyName))	!=	-1)
		{
			g_msgBox.cPopup("",dMSG_PARTY_OBJECT_LENGTH_RULE,dMSG_OK);

			return TRUE;
		}

		s_partyInfo.sendChangPartyProspectusText(lpstrChangePartyName);

		s_inputChatting.reset();

		return TRUE;
	}

	if	(STRICMP(lpstrName,dMSG_CHANGE_MINIPET_NAME_KEYWORD)==0)	//	펫 이름 변경
	{
		int			iKeywordLength		=	strlen(dMSG_CHANGE_MINIPET_NAME_KEYWORD)+1;	//	'/' 포함
		int			iPetIndex			=	atoi(s_strChatMessage+iKeywordLength+1);
		cMiniPet*	lpMiniPet			=	g_hero.iMiniPet()->getMiniPet(iPetIndex);

		if	(lpMiniPet	&&	lpMiniPet->m_bf7Level)
		{
			char	*lpstrPetName		=	s_strChatMessage+iKeywordLength+1+2;
			int		iNameLength			=	strlen(lpstrPetName);

			if	(iNameLength > c_iMiniPetNameLength || iNameLength < 1)
			{
				g_msgBox.cPopup("",dMSG_PET_NAME_RULE,dMSG_OK);
				s_inputChatting.reset();
				return TRUE;
			}

			s_agent.sendChangePetName(iPetIndex,lpstrPetName,TRUE);
		}

		s_inputChatting.reset();

		return TRUE;
	}
	if	(STRICMP(lpstrName,dMSG_CHANGE_PET_NAME_KEYWORD)==0)	//	펫 이름 변경
	{
		int		iKeywordLength			=	strlen(dMSG_CHANGE_PET_NAME_KEYWORD)+1;	//	'/' 포함
		int		iPetIndex				=	atoi(s_strChatMessage+iKeywordLength+1);

		if (iPetIndex	<	dOWN_PET_COUNT)
		{
			char	*lpstrPetName		=	s_strChatMessage+iKeywordLength+1+2;
			int		iNameLength			=	strlen(lpstrPetName);

			if (iNameLength > dPET_NAME_LENGTH || iNameLength < 1)
			{
				g_msgBox.cPopup("",dMSG_PET_NAME_RULE,dMSG_OK);
				s_inputChatting.reset();
				return TRUE;
			}

			s_agent.sendChangePetName(iPetIndex,lpstrPetName);
		}

		s_inputChatting.reset();

		return TRUE;
	}

	if (STRICMP(lpstrName,dMSG_REMOVE_MY_NAME_IN_TARGET_FRIEND_LIST)==0)	//	펫 이름 변경
	{
		int		iKeywordLength	=	strlen(dMSG_REMOVE_MY_NAME_IN_TARGET_FRIEND_LIST)+1;	//	'/' 포함
		char	*lpstrName		=	s_strChatMessage+iKeywordLength+1;

		int		iNameLength		=	strlen(lpstrName);

		if (iNameLength > dNAME_LENGTH-1 || iNameLength < 4)
		{
			s_inputChatting.reset();
			return TRUE;
		}

		s_agent.sendRemoveMyNameInTargetFriendList(lpstrName);

		s_inputChatting.reset();

		return TRUE;
	}

	return FALSE;
}

//
//
BOOL
CGamePlay::OperateCommunityChatMessage(char *_lpstrChatMessage)
{
	if (strlen(_lpstrChatMessage) < 6)
	{
		s_inputChatting.reset();

		return FALSE;
	}

	CDecoder	dec;

	dec.UploadBuffer(_lpstrChatMessage+1,NULL);

	char	*lpstrName	=	dec.getLongWord();
	int		iNameLength	=	strlen(lpstrName);

	if	(iNameLength >= 4 && iNameLength <= dNAME_LENGTH-1)
	{
		if	((int)strlen(s_strChatMessage)	>=	iNameLength+3)
		{
			g_config.m_wIsAllowWhisper		=	TRUE;

			strcpy(s_strLastSendSayId,lpstrName);

			char	*lpstrMessage	=	s_strChatMessage+iNameLength+1+1;

			s_agent.sendSayMessage(lpstrName,lpstrMessage);

			s_strChatMessage[iNameLength+1+1]	=	NULL;

			s_inputChatting.setStr(0,_ms("/%s ",s_strLastSendSayId));

			return TRUE;
		}

		return TRUE;
	}

	if (s_strChatMessage[1]	== 'r' && s_strChatMessage[2]	== ' '  && s_strChatMessage[3]	!= ' ' && s_strLastReceiveSayId[0] != NULL && strlen(s_strLastReceiveSayId) >= 4)	//	응답
	{
		char	*lpstrMessage	=	s_strChatMessage+3;

		s_agent.sendSayMessage(s_strLastReceiveSayId,lpstrMessage);

		s_inputChatting.setStr(0,_ms("/%s ",s_strLastReceiveSayId));

		return TRUE;
	}

	return FALSE;
}

int	l_iValue	=	0;

//
//	챗 메시지 처리
void
CGamePlay::OperateChatMessage()
{
	s_inputChatting.update(s_posMouse.x,s_posMouse.y,s_isClickedLeftButton,s_aTouched[KEY_TAB]);

	if	(s_inputChatting.getCompositionState(0))
		s_bIsChattingCompositionState	=	TRUE;

	s_inputChatting.getStr(0,s_strChatMessage,sizeof(s_strChatMessage));

	if	(s_strLastReceiveSayId[0])
	{
		if	(STRICMP(s_strChatMessage,"/r ") == 0)
			s_inputChatting.setStr(0,_ms("/%s ",s_strLastReceiveSayId));

#ifdef	_FOR_KOREA
		if	(STRICMP(s_strChatMessage,"/ㄱ ") == 0 && s_strLastReceiveSayId[0])
			s_inputChatting.setStr(0,_ms("/%s ",s_strLastReceiveSayId));
#else
		if	(STRICMP(s_strChatMessage,"/ ") == 0)
			s_inputChatting.setStr(0,_ms("/%s ",s_strLastReceiveSayId));
#endif
	}

#ifdef	_FOR_JAPAN
	if	(s_bIsChattingCompositionState == FALSE && s_aTouched[KEY_BACK])
		if (strlen(s_strChatMessage) == 0 && l_iValue > 10)
			s_inputChatting.setStr(0,"/");

	if	(strlen(s_strChatMessage) == 0)
		l_iValue++;
	else
		l_iValue	=	0;
#endif		

	if	(s_aTouched[KEY_RETURN] + s_aTouched[KEY_NUMPADENTER] == 0)
		return;

	if	(g_hero.m_bf1IsReadyToUseSoundOfLeadersBell)
	{
		s_inputChatting.reset();

		g_hero.m_bf1IsReadyToUseSoundOfLeadersBell	=	FALSE;

		if	(strlen(s_strChatMessage) < 0 || strlen(s_strChatMessage) > 60)
		{
			g_msgBox.cPopup("",dMSG_INCORRECT_MESSAGE_LENGTH,dMSG_OK);
			return;
		}

		strcpy(g_hero.m_strMessageForSoundOfLeadersBell,s_strChatMessage);

		g_msgBox.cPopup("",dMSG_DO_YOUR_WANT_A_DISPLAY_YOUR_NAME_WITH_SOUND_OF_LEADERS_BELL,dMSG_YES,dMSG_NO);
		g_msgBox.setButtonId(eMBI_DISPLAY_NAME_WITH_SOUND_OF_LEADERS_BELL,eMBI_NOT_DISPLAY_NAME_WITH_SOUND_OF_LEADERS_BELL);

		return;
	}

#ifndef	_FOR_KOREA
	if	(s_bIsChattingCompositionState)
	{
		s_bIsChattingCompositionState	=	s_inputChatting.getCompositionState(0);
		return;
	}
#endif

	s_inputChatting.getStr(0,s_strChatMessage,sizeof(s_strChatMessage));

	if	(g_gwPutBannerWnd.isOpened())
	{
		g_gwPutBannerWnd.updateText(s_strChatMessage);
		return;
	}

	if	(!g_hero.isOperator() && CheckFilter(s_strChatMessage,g_pstrSladerFilter,g_loaderText.m_iSlanderTextCount))
	{
		s_inputChatting.reset();
		AddSystemMessage(LTRED,dMSG_PLEASE_USE_GOOD_WORD);
		return;
	}

	strcpy(s_strPreviousChatMessage,s_strChatMessage);

	if	(strcmp(s_strChatMessage,"")	==	0)
	{
		s_bIsBookedDisableChattingBox	=	TRUE;
		return;
	}

#ifdef	_FOR_KOREA
	if	((stricmp(s_strChatMessage,"@엔딩")	==	0)&&g_hero.isOperator())
	{
		g_gwMainQuestEvent.openEpilog(0,0,1);
	}

#ifdef		_DEBUG
	if	((stricmp(s_strChatMessage,"@배틀로그")	==	0))
	{
		g_BattleLog.m_bIsActive = !g_BattleLog.m_bIsActive;
		
		if(g_BattleLog.m_bIsActive)
			AddSystemMessage(WHITE,"배틀 로그가 생성됩니다.");
		else
			AddSystemMessage(WHITE,"배틀 로그가 생성되지 않아용...");
		s_inputChatting.reset();
		return;
	}
	if((stricmp(s_strChatMessage,"@신의손") == 0) && g_hero.m_wOperatorLevel>=2)		// 09.09.08   명령어 강화 100%
	{
		g_gwUpgradeShop.m_bUpgradePerfect = !g_gwUpgradeShop.m_bUpgradePerfect;
		if(g_gwUpgradeShop.m_bUpgradePerfect)
			AddSystemMessage(WHITE,"당신의 손이 한순간 밝게 빛납니다.");
		else
			AddSystemMessage(WHITE,"손에서 빛이 사라집니다.");
		
		s_inputChatting.reset();
		return;
	}
	if((stricmp(s_strChatMessage,"@대미지출력") == 0) && g_hero.m_wOperatorLevel>=2)		// 대미지출력.
	{
		g_BattleLog.m_bIsShowDamage = !g_BattleLog.m_bIsShowDamage;
		if(g_BattleLog.m_bIsShowDamage)
			AddSystemMessage(WHITE,"다보여~");
		else
			AddSystemMessage(WHITE,"안보여~");
		
		s_inputChatting.reset();
		return;
	}
	if((stricmp(s_strChatMessage,"@몬스터대미지출력") == 0) && g_hero.m_wOperatorLevel>=2)		// 대미지출력.
	{
		g_BattleLog.m_bIsShowMonsterDamage = !g_BattleLog.m_bIsShowMonsterDamage;
		if(g_BattleLog.m_bIsShowMonsterDamage)
			AddSystemMessage(WHITE,"몬스터대미지~ 다보여~");
		else
			AddSystemMessage(WHITE,"몬스터대미지~ 안보여~");
		
		s_inputChatting.reset();
		return;
	}
#endif

	if	((stricmp(s_strChatMessage,"@폰트변경")	==	0)&&g_hero.isOperator())
	{
		l_bIsWantChangeFont	=	1-l_bIsWantChangeFont;

		if	(l_bIsWantChangeFont)
		{
			cFILE	file;

			file.Open(_ms("%s/change_font.dat",g_strLogFolder),"wb");
			file.Close();
		}
		else
		{
			DeleteFile(_ms("%s/change_font.dat",g_strLogFolder));
		}

		if	(l_bIsWantChangeFont)
			s_textChat.selectFont(l_iFontSize,g_strPitchmanShopTextName);
		else
			s_textChat.selectFont(l_iFontSize,g_strCheckFontName);

		return;
	}
#endif

	if	(s_strChatMessage[0] == '\\')
		if	(OperateSystemChatMessage(s_strChatMessage))
			return;
	if	(s_strChatMessage[0] == '!' && s_strChatMessage[1] == ' ' && strlen(s_strChatMessage) >= 3)
	{
		if (g_hero.isOperator())
			s_inputChatting.setStr(0,"! ");
		else
		{
			if (g_hero.m_wShoutCount	>=	dSHOUT_SHOUT_COUNT_BY_ONE_PERIOD)
			{
				if (g_hero.m_dwLastShoutTime+dSHOUT_PERIOD*1000	>	timeGetTime())
				{
					g_lpHero->warning(dMSG_CAN_NOT_SHOUT_REASON_BY_TIME);
					s_inputChatting.reset();
					return;
				}

				g_hero.m_wShoutCount	=	0;
			}

			if (g_hero.m_dwLastShoutTime+dSHOUT_PERIOD*1000	<	timeGetTime())	g_hero.m_wShoutCount	=	0;
			g_hero.m_dwLastShoutTime	=	timeGetTime();
			g_hero.m_wShoutCount++;

			s_inputChatting.reset();
		}

		if (s_iChatFilter	!=	eCMF_NORMAL_CHAT)
		{
			s_iChatFilter	=	eCMF_NORMAL_CHAT;

			if	(g_config.m_iChattingBoxScale & 0x8000)
				ToggleChattingBoxScale();
			else
				RebuildChatMessageBox();
		}

		g_config.m_wIsAllowShout	=	TRUE;
		s_agent.sendChatMessage(s_strChatMessage+2,s_iChatFilter,TRUE);
		return;
	}

	if	(!g_hero.isOperator() && g_hero.m_dwLastChatTime + dCHAT_PERIOD +100 > timeGetTime() && g_hero.m_dwLastChatTime < timeGetTime())
	{
		g_lpHero->warning(dMSG_HATE_DOBAE);
		return;
	}
	
	{
		char	*lpstrLastMessage	=	s_tlInputMessage.getLastestText();

		if	(!lpstrLastMessage	||	STRICMP(lpstrLastMessage,s_strChatMessage)	!=	0)
			s_tlInputMessage.addText(s_strChatMessage,TRUE);

		s_tlInputMessage.setCurrentTextLineToLastestLine();
	}

	if	(s_strChatMessage[0] == '/')
		if	(OperateCommunityChatMessage(s_strChatMessage))
			return;

	if	(s_strChatMessage[0] == '@')
		if	(OperateCommandChatMessage(s_strChatMessage))
			return;

	if	(s_iChatFilter	==	eCMF_WHISPER)
	{
		s_iChatFilter	=	eCMF_NORMAL_CHAT;

		if	(g_config.m_iChattingBoxScale & 0x8000)
			ToggleChattingBoxScale();
		else
			RebuildChatMessageBox();
	}

	if	(s_iChatFilter	==	eCMF_PARTY_CHAT && !g_lpHero->isPartyMember())
	{
		s_iChatFilter	=	eCMF_NORMAL_CHAT;

		if	(g_config.m_iChattingBoxScale & 0x8000)
			ToggleChattingBoxScale();
		else
			RebuildChatMessageBox();
	}

	if	(s_iChatFilter	==	eCMF_GUILD_CHAT && !g_hero.isGuildMember())
	{
		s_iChatFilter	=	eCMF_NORMAL_CHAT;

		if	(g_config.m_iChattingBoxScale & 0x8000)
			ToggleChattingBoxScale();
		else
			RebuildChatMessageBox();
	}

	g_hero.m_dwLastChatTime	=	timeGetTime();
	s_agent.sendChatMessage(s_strChatMessage,s_iChatFilter,FALSE);
	s_inputChatting.reset();
}

//
//	그리기
void
CGamePlay::DrawChatMessageBox()
{
	if	(s_bIsObserverMode)
		return;

#ifdef	_DEBUG
	if	(g_bIsHideDamageAndHideName)
		return;
	if	(g_bIsMovieMode)
		return;

#endif

	cRECT	rect;

	rect.set(s_tlChat.getRect());

	rect.x1	-=	s_sbChatBox.m_iButtonWidth;
	rect.y2	+=	l_iChattingFilterHeight;

	if	(g_config.m_iChattingBoxScale&0x8000 && g_iScreenHeight == 600 ||
		g_config.m_iChattingBoxScale&0x8000 && g_iScreenHeight == 768)
	{
		rect.x2	=	150+g_iCorrectBI_X;
		rect.y1	=	rect.y2-l_iChattingFilterHeight+4+g_iCorrectBI_Y;

		if (g_iScreenHeight == 768)
		{
			rect.x2 = 150+69;
			rect.y1 = 752;
			rect.y2 = 762;
		}

		if	(g_config.m_bIsChatWindowHalfblending)
			s_ttCommon.drawBoxAddBorderSize(&rect);
		else
			s_ttCommon.drawBoxAddBorderSize(&rect,0,32);
	}
	else
	{
		if	(g_config.m_bIsChatWindowHalfblending)
			s_ttCommon.drawBoxAddBorderSize(&rect);
		else
			s_ttCommon.drawBoxAddBorderSize(&rect,0,32);

//	채팅 필터
		int	iLeftWidth	=	g_sprInterface.getSpriteWidth(eCF_LEFT_BORDER);
		int	iRightWidth	=	g_sprInterface.getSpriteWidth(eCF_RIGHT_BORDER);
		int	iEdgeWidth	=	s_ttCommon.m_iLeftEdgeWidth+s_ttCommon.m_iRightEdgeWidth;

		g_sprInterface.Put(rect.x1-s_ttCommon.m_iLeftEdgeWidth					,rect.y2-l_iChattingFilterHeight-1,eCF_LEFT_BORDER);
		g_sprInterface.Put(rect.x2+s_ttCommon.m_iRightEdgeWidth-iRightWidth+1	,rect.y2-l_iChattingFilterHeight-1,eCF_RIGHT_BORDER);
		g_sprInterface.clipPut(rect.x1-s_ttCommon.m_iLeftEdgeWidth+iLeftWidth	,rect.y2-l_iChattingFilterHeight,eTOOLTIP_BOTTOM_BORDER,rect.getWidth()+iEdgeWidth-(iLeftWidth+iRightWidth),100);

//	채팅 내용 출력
		{
			s_tlChat.setTextDC(&s_textChat);
			s_tlChat.getTextDC()->someCodeIgnoreOn();
			s_tlChat.drawText();
			s_tlChat.getTextDC()->someCodeIgnoreOff();
		}
//	스크롤바
		s_sbChatBox.draw();
	}

//	버튼들
	s_bmChattingFilter.draw();
	s_bmChatBoxScale.draw();

}

char*
CGamePlay::GetNextWhisperName()
{
	if (s_strSayUserList[s_iSelectServer].Count	==	0)
		return	NULL;

	cSTRING	*lpPlayer	=	s_strSayUserList[s_iSelectServer].Pick(s_iCurrentSayTalkerIndex);

	s_iCurrentSayTalkerIndex++;

	if(s_iCurrentSayTalkerIndex	>=	s_strSayUserList[s_iSelectServer].Count)
		s_iCurrentSayTalkerIndex	=	0;

	return	lpPlayer->String;
}

char*
CGamePlay::GetPreviousWhisperName()
{
	if (s_strSayUserList[s_iSelectServer].Count	==	0)
		return	NULL;

	cSTRING	*lpPlayer	=	s_strSayUserList[s_iSelectServer].Pick(s_iCurrentSayTalkerIndex);

	s_iCurrentSayTalkerIndex--;

	if(s_iCurrentSayTalkerIndex	<	0)
		s_iCurrentSayTalkerIndex	=	s_strSayUserList[s_iSelectServer].Count-1;

	return	lpPlayer->String;
}