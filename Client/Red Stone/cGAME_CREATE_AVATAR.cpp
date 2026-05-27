#include "cGAME_CREATE_AVATAR.H"
#include "CAgent.H"
#include "cJOB.H"
#include "CActor.H"
#include "CMessage.H"
#include "cLoadTextCode.H"
#include "devCode.H"

#define	dIDM_CREATE_AVATAR				1000
#define	dIDM_RETURN_TO_SELECT_AVATAR	1001

int					cGAME_CREATE_AVATAR::s_iSelectJob		=	dJOB_KNIGHT;
cSPRITE				cGAME_CREATE_AVATAR::s_sprSprite;
cANM				cGAME_CREATE_AVATAR::s_anmLight;
cGAME_UTIL::cLIGHT	cGAME_CREATE_AVATAR::s_aLight[dMAX_LIGHT];

int			cGAME_CREATE_AVATAR::s_iAvatarCount						=	0;
int			cGAME_CREATE_AVATAR::s_iTimeOfConnectTry				=	FALSE;				//	컨넥트 시도 시간

////////////////////////////////////////////////////////////////////////////////////////////////
//	업데이트
////////////////////////////////////////////////////////////////////////////////////////////////

//
//	초기화
BOOL
cGAME_CREATE_AVATAR::UpdateInit()
{
//	이미지 데이터 로딩
	{
		s_mcuData.reset();

		if (!s_sprSprite.Load("data/interface/create_avatar.sd",FALSE,NULL,&s_mcuData))	return	FALSE;
		if (!s_anmLight.Load("data/interface/light.sad",FALSE,NULL,&s_mcuData))	return	FALSE;
	}	//	이미지 데이터 로딩

	for (int i=0;i<dMAX_LIGHT;i++)
	{
		int		iAnm	=	random(s_anmLight.m_iAnmCount);
		s_aLight[i].init(iAnm,s_anmLight.GetFrameCount(iAnm),-200,-200,g_iScreenWidth+200,g_iScreenHeight+200,0.002f);
	}

//	직업 선택 버튼 초기화
	{
		int	x	=	10;
		int	y	=	10;
		int	ys	=	24;

		enum eSmall_button
		{
			ec_btn_small_normal   = 14,
			ec_btn_small_active   = 15,
			ec_btn_small_select   = 16,
			ec_btn_small_check    = 16,
			ec_btn_small_disable  = 13,
		};

		enum eButton
		{
			ec_btn_normal   = 10,
			ec_btn_active   = 11,
			ec_btn_select   = 12,
			ec_btn_check    = 12,
			ec_btn_disable  = 9,
		};

		s_bmButton.init(&s_sprSprite,20,dBAR_RADIO,eBTOM_CENTER);

		int	iY	=	121;
		int	iDY	=	31;

	//	if	(g_bIsDevNewJob)
		{
//			iY	=	119;
			iDY	=	27;
		}

		s_bmButton.addImageBar(g_pstrHeroJobName[dJOB_KNIGHT]		,dJOB_KNIGHT		, 491+g_iCorrectBI_X*9/10,iY,ec_btn_small_normal,ec_btn_small_active,ec_btn_small_select,ec_btn_small_check,ec_btn_small_disable);
		s_bmButton.addImageBar(g_pstrHeroJobName[dJOB_WARRIOR]		,dJOB_WARRIOR		, 633+g_iCorrectBI_X*9/10,iY,ec_btn_small_normal,ec_btn_small_active,ec_btn_small_select,ec_btn_small_check,ec_btn_small_disable);
		
		iY		+=	iDY;

		s_bmButton.addImageBar(g_pstrHeroJobName[dJOB_MAGIC_ARCHER]	,dJOB_MAGIC_ARCHER	, 491+g_iCorrectBI_X*9/10,iY,ec_btn_small_normal,ec_btn_small_active,ec_btn_small_select,ec_btn_small_check,ec_btn_small_disable);
		s_bmButton.addImageBar(g_pstrHeroJobName[dJOB_MAGIC_LANCER]	,dJOB_MAGIC_LANCER	, 633+g_iCorrectBI_X*9/10,iY,ec_btn_small_normal,ec_btn_small_active,ec_btn_small_select,ec_btn_small_check,ec_btn_small_disable);

		iY		+=	iDY;
		s_bmButton.addImageBar(g_pstrHeroJobName[dJOB_BEAST_TAMER]	,dJOB_BEAST_TAMER	, 491+g_iCorrectBI_X*9/10,iY,ec_btn_small_normal,ec_btn_small_active,ec_btn_small_select,ec_btn_small_check,ec_btn_small_disable);
		s_bmButton.addImageBar(g_pstrHeroJobName[dJOB_SUMMONER]		,dJOB_SUMMONER		, 633+g_iCorrectBI_X*9/10,iY,ec_btn_small_normal,ec_btn_small_active,ec_btn_small_select,ec_btn_small_check,ec_btn_small_disable);

		iY		+=	iDY;
		s_bmButton.addImageBar(g_pstrHeroJobName[dJOB_ROGUE]		,dJOB_ROGUE			, 491+g_iCorrectBI_X*9/10,iY,ec_btn_small_normal,ec_btn_small_active,ec_btn_small_select,ec_btn_small_check,ec_btn_small_disable);
		s_bmButton.addImageBar(g_pstrHeroJobName[dJOB_FIGHTER]		,dJOB_FIGHTER		, 633+g_iCorrectBI_X*9/10,iY,ec_btn_small_normal,ec_btn_small_active,ec_btn_small_select,ec_btn_small_check,ec_btn_small_disable);

		iY		+=	iDY;
		s_bmButton.addImageBar(g_pstrHeroJobName[dJOB_PRIEST]		,dJOB_PRIEST		, 491+g_iCorrectBI_X*9/10,iY,ec_btn_normal,ec_btn_active,ec_btn_select,ec_btn_check,ec_btn_disable);
		iY		+=	iDY;
		s_bmButton.addImageBar(g_pstrHeroJobName[dJOB_WIZARD]		,dJOB_WIZARD		, 491+g_iCorrectBI_X*9/10,iY,ec_btn_normal,ec_btn_active,ec_btn_select,ec_btn_check,ec_btn_disable);
		iY		+=	iDY;
		s_bmButton.addImageBar(g_pstrHeroJobName[dJOB_PRINCESS]		,dJOB_PRINCESS		, 491+g_iCorrectBI_X*9/10,iY,ec_btn_normal,ec_btn_active,ec_btn_select,ec_btn_check,ec_btn_disable);
		iY		+=	iDY;
		s_bmButton.addImageBar(g_pstrHeroJobName[dJOB_NECROMANCER]	,dJOB_NECROMANCER	, 491+g_iCorrectBI_X*9/10,iY,ec_btn_normal,ec_btn_active,ec_btn_select,ec_btn_check,ec_btn_disable);

		iY		+=	iDY;
		s_bmButton.addImageBar(g_pstrHeroJobName[dJOB_SOUL_BRINGER]	,dJOB_SOUL_BRINGER	, 491+g_iCorrectBI_X*9/10,iY,ec_btn_small_normal,ec_btn_small_active,ec_btn_small_select,ec_btn_small_check,ec_btn_small_disable);


		s_bmButton.addImageBar(g_pstrHeroJobName[dJOB_CHAMPION],dJOB_CHAMPION	, 633+g_iCorrectBI_X*9/10,iY,ec_btn_small_normal,ec_btn_small_active,ec_btn_small_select,ec_btn_small_check,ec_btn_small_disable);

		iY		+=	iDY;
		s_bmButton.addImageBar(g_pstrHeroJobName[dJOB_OPTICALIST]		,dJOB_OPTICALIST		, 491+g_iCorrectBI_X*9/10,iY,ec_btn_normal,ec_btn_active,ec_btn_select,ec_btn_check,ec_btn_disable);

		s_bmButton.setAllBarTextColor( 0, FALSE );

		if (g_bIsDevPC	==	FALSE )
			for (int i=0;i<dPLAYER_JOB_COUNT;i++)
			{
				if ( g_aPlayer1LvSetting[i].m_wStatus	==	0)
					s_bmButton.disable(i);

				if ( g_aPlayer1LvSetting[i].m_wStatus	==	2	&&	g_bIsTestServer	==	FALSE)
					s_bmButton.disable(i);
			}

		s_bmButton.check(dJOB_KNIGHT);

		s_bmMenu.init(&s_sprSprite,2);
		s_bmMenu.addImageBar("",dIDM_CREATE_AVATAR				,534+g_iCorrectBI_X*9/10,480+g_iCorrectBI_Y*9/10,0,2,3,0xffff,1);
		s_bmMenu.addImageBar("",dIDM_RETURN_TO_SELECT_AVATAR	,534+g_iCorrectBI_X*9/10,533+g_iCorrectBI_Y*9/10,4,5,6,0xffff,0xffff);

//		s_bmMenu.disable(dIDM_CREATE_AVATAR);
	}	//	직업 선택 버튼 초기화
	s_iLoadedHeroImageJob				=	-1;

//	캐릭터 이름
	{
		s_inputBox.init(s_hWnd,s_hInst,&s_text,1);
		s_inputBox.set(520+g_iCorrectBI_X*9/10,440,0,520+g_iCorrectBI_X*9/10,440,120,13,dNAME_LENGTH-2,"");
		s_inputBox.setColor(WHITE,0);
		s_inputBox.active(0,TRUE);
	}	//	캐릭터 이름

	BookingStep(dSTEP_READY,dFADE_IN);

	return	SAFETY;
}	//	cGAME_CREATE_AVATAR::UpdateInit()


BOOL
cGAME_CREATE_AVATAR::UpdateReady()
{
	BookingStep(dSTEP_MAIN);

	return	SAFETY;
}	//	cGAME_CREATE_AVATAR::UpdateReady()

//
//	패킷 처리
BOOL
cGAME_CREATE_AVATAR::OperatePacket()
{
//	패킷 처리
	if	(s_agent.isDisconnected())
	{
		g_msgBox.cPopup(NULL,s_agent.m_strDisconnectText,dMSG_OK);
		g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
		s_agent.reset();

		return	FALSE;
	}

	if	(!s_agent.packetHandler())
		return	FALSE;

	if (s_agent.isReceivedPacket(dSL_RESULT_CREATE_AVATAR))
	{
		switch(GetCreateAvatarResult())
		{
			case	dRESULT_CREATE_AVATAR_SUCESS				:			//	성공
			{
				g_msgBox.cPopup(NULL,dMSG_SUCCESS_CREATE_AVATAR,dMSG_OK);
				g_msgBox.setButtonId(dIDM_RETURN_TO_SELECT_AVATAR);
				break;
			}

			case	dRESULT_CREATE_AVATAR_EXIST_SAME_NAME		:			//	이미 존재하는 이름이다.
				s_inputBox.active(0,TRUE);
				g_msgBox.cPopup(NULL,dMSG_ALREADY_EXIST_SAME_NAME_CHARACTER,dMSG_OK);
				break;

			case	dRESULT_CREATE_AVATAR_AVATAR_FULL			:			//	아바타 수가 넘친다.
				g_msgBox.cPopup(NULL,dMSG_CAN_NOT_CREATE_MORE_CHARACTER,dMSG_OK);
				g_msgBox.setButtonId(dIDM_RETURN_TO_SELECT_AVATAR);
				break;

			case	dRESULT_CREATE_AVATAR_FAILED				:			//	알수 없는 이유로 실패
				g_msgBox.cPopup(NULL,dMSG_FAILED_UNKNOWN_REASON,dMSG_OK);
				break;

			case	dRESULT_CREATE_FAILED_BY_INCORRECT_CHAR		:
				g_msgBox.cPopup(NULL,dMSG_FAILED_UNKNOWN_REASON,dMSG_OK);
				break;
		}
	}

	return	FALSE;
}

//
//	메시지 박스 처리
BOOL
cGAME_CREATE_AVATAR::OperateMessageBox()
{
	if (!s_isOperatedMsgbox)
		return	FALSE;

	if	(g_msgBox.isNothing())
		return	TRUE;

	switch(g_msgBox.getResult())
	{
		case	eMBI_RETURN_TO_TITLE	:		//	로그인 화면으로 돌아가기
		case	eMBI_RETURN_TO_TITLE_BY_CANCEL	:
			s_agent.reset();
			BookingMode(eGAME_TITLE,dSTEP_FINISH,dFADE_OUT);
			s_inputBox.close();
			break;

		case	dIDM_RETURN_TO_SELECT_AVATAR	:
			BookingMode(eGAME_SELECT_AVATAR,dSTEP_FINISH,dFADE_OUT);
			s_inputBox.close();
			break;
	}

	return	TRUE;
}

//
//	빛-o-을 업데이트
void
cGAME_CREATE_AVATAR::UpdateLight()
{
	for (int i=0;i<dMAX_LIGHT;i++)
	{
		s_aLight[i].updateSpline();
		s_aLight[i].updateFrame();
	}
}	//	cGAME_SELECT_AVATAR::UpdateLight()

//
//	업데이트
BOOL
cGAME_CREATE_AVATAR::UpdateMain()
{
	char	strName[dNAME_LENGTH];

	OperatePacket();
	UpdateLight();

	if (OperateMessageBox())	return	TRUE;	//	메시지 박스 메시지 처리

//	캐릭터 이름 업데이트
	{
		s_inputBox.update(s_posMouse.x,s_posMouse.y,s_isClickedLeftButton,s_aTouched[KEY_TAB]);

		s_inputBox.getStr(0,strName,dNAME_LENGTH);

		if (CheckName(strName))	s_bmMenu.enable(dIDM_CREATE_AVATAR);
		else					s_bmMenu.disable(dIDM_CREATE_AVATAR);
	}

	s_bmButton.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isTouchLeftButton,TRUE);

	s_iSelectJob			=	s_bmButton.getCheckedMenu();

	DWORD	dwSelectMenu	=	s_bmMenu.update(s_posMouse.x,s_posMouse.y,s_aTouched,s_isTouchLeftButton,s_isLeftButtonPressed,TRUE);

	switch(dwSelectMenu)
	{
		case	dIDM_CREATE_AVATAR				:	//	아바타 생성
		{
			if (CheckFilter(strName,g_pstrSladerFilter,g_loaderText.m_iSlanderTextCount) || CheckFilter(strName,g_pstrNameFilter,g_loaderText.m_iNameFilterCount))
			{
				g_msgBox.cPopup(NULL,dMSG_CAN_NOT_MAKE_NAME_BY_SLANDER_NAME,dMSG_OK);
				break;
			}

			int	iSelectJob	=	s_bmButton.getCheckedMenu();
			s_agent.sendCreateAvatar(strName,iSelectJob);
			g_msgBox.cPopup(NULL,dMSG_WAIT_RESULT_OF_CREATE_AVATAR,dMSG_CANCEL);
			g_msgBox.setButtonId(eMBI_RETURN_TO_TITLE_BY_CANCEL);
			g_msgBox.setWaitPacket(dSL_RESULT_CREATE_AVATAR);
			break;
		}

		case	dIDM_RETURN_TO_SELECT_AVATAR	:
			BookingMode(eGAME_SELECT_AVATAR,dSTEP_FINISH,dFADE_OUT);
			s_inputBox.close();
			break;
	}

	return	SAFETY;
}	//	cGAME_CREATE_AVATAR::UpdateMain()

//
//	마무리 준비
BOOL
cGAME_CREATE_AVATAR::UpdateFinish()
{
	if (g_canvas.isCompleteFadeOut())	BookingStep(dSTEP_CLOSE);

	return	SAFETY;
}	//	cGAME_CREATE_AVATAR::UpdateFinish()

//
//	닫기
BOOL
cGAME_CREATE_AVATAR::UpdateClose()
{
	s_bmButton.close();
	s_inputBox.close();
	s_sprSprite.close();
	s_anmLight.Reset();

	return	SAFETY;
}	//	cGAME_CREATE_AVATAR::UpdateClose()


////////////////////////////////////////////////////////////////////////////////////////////////
//	화면에 출력
////////////////////////////////////////////////////////////////////////////////////////////////

void
cGAME_CREATE_AVATAR::DrawInit()
{
	DrawMain();
}

void
cGAME_CREATE_AVATAR::DrawReady()
{
	DrawMain();
}

void
cGAME_CREATE_AVATAR::DrawLight()
{
	for (int i=0;i<dMAX_LIGHT;i++)
		cDRAW::Fill(_LTYELLOW,	s_aLight[i].m_spline.m_pos.x,s_aLight[i].m_spline.m_pos.y,
								s_aLight[i].m_spline.m_pos.x+2,s_aLight[i].m_spline.m_pos.y+2);
//		s_anmLight.PutReg(s_aLight[i].m_spline.m_pos.x,s_aLight[i].m_spline.m_pos.y,s_aLight[i].m_iAnm,0,s_aLight[i].m_iFrame);
}

//
//	메인루프 그려주기!!
void
cGAME_CREATE_AVATAR::DrawMain()
{
	enum eSpr
	{
		ec_spr_new_character = 8,
		ec_spr_num1 = 17,
		ec_spr_num2 = 18,
		ec_spr_name_bar = 19,
	};
	cDRAW::Clear();

	DrawLight();
	DrawHeroPicture(0,0,s_iSelectJob);

	int	iDeltaWidth	=	(g_iScreenWidth - 445+g_iCorrectBI_X*9/10)*(32-g_canvas.getFadeScreenValue())/32;

	cRECT	rectBorder;
	rectBorder.set(480+g_iCorrectBI_X*9/10,82,785+g_iCorrectBI_X*9/10,395);
	rectBorder.add(iDeltaWidth,0);
	s_ttFrame.drawBox(&rectBorder);
	rectBorder.set(480+g_iCorrectBI_X*9/10,400,785+g_iCorrectBI_X*9/10,471);
	rectBorder.add(iDeltaWidth,0);
	s_ttFrame.drawBox(&rectBorder);

	s_sprSprite.Put(480+iDeltaWidth+g_iCorrectBI_X*9/10,0,ec_spr_new_character);
	s_sprSprite.Put(495+iDeltaWidth+g_iCorrectBI_X*9/10,93,ec_spr_num1);
	s_sprSprite.Put(492+iDeltaWidth+g_iCorrectBI_X*9/10,410,ec_spr_num2);
	s_sprSprite.Put(511+iDeltaWidth+g_iCorrectBI_X*9/10,456,ec_spr_name_bar );

	s_bmButton.setCorrectPos(iDeltaWidth,0);
	s_bmMenu.setCorrectPos(iDeltaWidth,0);

	s_bmButton.draw();
	s_bmMenu.draw();
	s_inputBox.draw(WHITE);

	s_text14.put(512+iDeltaWidth+g_iCorrectBI_X*9/10,101,WHITE,dMSG_CREATE_CHARACTER_CHOICE_JOB);
	s_text14.put(512+iDeltaWidth+g_iCorrectBI_X*9/10,416,WHITE,dMSG_CREATE_CHARACTER_NAME);
	
	if	(timeGetTime() % 2000 > 500)
		s_textBold14.cPut(g_iScreenWidth/2,582+g_iCorrectBI_Y,WHITE,dMSG_BAD_NAME_NOTICE);
}
void
cGAME_CREATE_AVATAR::DrawFinish()
{
	DrawMain();
}

void
cGAME_CREATE_AVATAR::DrawClose()
{
	cDRAW::Clear();
}



BOOL
cGAME_CREATE_AVATAR::Run()
{
	switch(s_iStep)
	{
		case	dSTEP_INIT	:	return	UpdateInit();
		case	dSTEP_READY	:	return	UpdateReady();
		case	dSTEP_MAIN	:	return	UpdateMain();
		case	dSTEP_FINISH:	return	UpdateFinish();
		case	dSTEP_CLOSE	:	return	UpdateClose();
	}

	return	FALSE;
}

BOOL
cGAME_CREATE_AVATAR::Draw()
{
	switch(s_iStep)
	{
		case	dSTEP_INIT	:DrawInit();break;
		case	dSTEP_READY	:DrawReady();break;
		case	dSTEP_MAIN	:DrawMain();break;
		case	dSTEP_FINISH:DrawFinish();break;
		case	dSTEP_CLOSE	:DrawClose();break;
	}

	return	TRUE;
}