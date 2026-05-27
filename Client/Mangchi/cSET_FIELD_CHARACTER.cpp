#include "cSET.H"
#include "resource.H"
#include "CSetDialog.H"
#include "cSetDialogNpcData.h"					//	----------------------------------	추가
#include "CSetDialogMonster.h"					//	09.11.23 수정
HWND					cSET_FIELD_CHARACTER::s_hWnd;
cACTOR*					cSET_FIELD_CHARACTER::s_lpEditActor;
int						cSET_FIELD_CHARACTER::s_iEditActor;
BOOL					cSET_FIELD_CHARACTER::s_isEditMultiActor;
CConversationContainer	cSET_FIELD_CHARACTER::g_backupConversationContainer;

//
//	
BOOL
cSET_FIELD_CHARACTER::Init(HWND _hWnd,cACTOR *_lpActor,BOOL _isMulti)
{
	s_lpEditActor		=	_lpActor;
	s_isEditMultiActor	=	_isMulti;

	g_backupConversationContainer.reset();

	if	(s_lpEditActor->m_pCC)
		g_backupConversationContainer.copy(s_lpEditActor->m_pCC);

	DialogBox( s_hInst, "SET_FIELD_CHARACTER",_hWnd,(DLGPROC)DlgProc);

	return	TRUE;
}

//
//
BOOL
cSET_FIELD_CHARACTER::SetData(HWND _hDlg)
{
	SetDlgItemText(_hDlg,IDC_NAME,s_lpEditActor->m_strName);

//	NPC 타입
	{
		HWND	hTypeList	=	GetDlgItem(_hDlg,IDC_COMBO_TYPE);

		if	(s_lpEditActor->m_wType	==	0xffff)
			cCOMBOBOX::Add(hTypeList," ",0xffff);

		int	iIndex	=	0;

		while(1)
		{
			if	(stricmp(g_strNpcCategorty[iIndex],"end")==0)
				break;

			cCOMBOBOX::Add(hTypeList,g_strNpcCategorty[iIndex],iIndex);

			iIndex++;
		}

		cCOMBOBOX::SelectByData(hTypeList,s_lpEditActor->m_wType);
	}	//	NPC 타입

//	리젠 주기 설정
	{
		int	iRegenHour,iRegenMinute,iRegenSecond;

		if	(s_lpEditActor->m_dwRegenerationCycle	!=	0xffffffff)
		{
			iRegenHour		=	s_lpEditActor->m_dwRegenerationCycle/60/60;
			iRegenMinute	=	(s_lpEditActor->m_dwRegenerationCycle/60)%60;
			iRegenSecond	=	s_lpEditActor->m_dwRegenerationCycle%60;

			SetDlgItemInt(_hDlg,IDC_REGEN_CYCLE_HOUR,iRegenHour,FALSE);
			SetDlgItemInt(_hDlg,IDC_REGEN_CYCLE_MINUTE,iRegenMinute,FALSE);
			SetDlgItemInt(_hDlg,IDC_REGEN_CYCLE_SECOND,iRegenSecond,FALSE);
		}
	}	//	리젠 주기 설정

//	방황-_-주기
	{
		int	iActiveMinute,iActiveSecond;

		if (s_lpEditActor->m_wWanderCycle	!=	0xffff)
		{
			iActiveMinute	=	s_lpEditActor->m_wWanderCycle/60;
			iActiveSecond	=	s_lpEditActor->m_wWanderCycle%60;

			SetDlgItemInt(_hDlg,IDC_WANDER_MINUTE,iActiveMinute,FALSE);
			SetDlgItemInt(_hDlg,IDC_WANDER_SECOND,iActiveSecond,FALSE);
		}
	}	//	방황-_-주기

//	직시 방향
	{
		HWND	hDirect			=	GetDlgItem(_hDlg,IDC_GAZE_DIRECT);
		HWND	hChangeDirect	=	GetDlgItem(_hDlg,IDC_CHANGE_DIRECT);

		if (s_lpEditActor->m_wDirect	==	0xffff)	cCOMBOBOX::Add(hDirect," ",0xffff);

		for (int i=0;i<9;i++)	cCOMBOBOX::Add(hDirect,g_str8Direct[i],i);

		cCOMBOBOX::SelectByData(hDirect,s_lpEditActor->m_wDirect);

		if (s_lpEditActor->m_isChangeDirect	==	0xffff	)	CheckDlgButton(_hDlg,IDC_CHANGE_DIRECT,BST_INDETERMINATE);
		if (s_lpEditActor->m_isChangeDirect	==	TRUE	)	CheckDlgButton(_hDlg,IDC_CHANGE_DIRECT,BST_CHECKED);
		if (s_lpEditActor->m_isChangeDirect	==	FALSE	)	CheckDlgButton(_hDlg,IDC_CHANGE_DIRECT,BST_UNCHECKED);
	}	//	직시 방향

//	생성 패턴
	{
		if	(s_lpEditActor->m_wGenerateMethod==	0xffff							)
			CheckDlgButton(_hDlg,IDC_GENERATE_METHOD,BST_INDETERMINATE);
		if	(s_lpEditActor->m_wGenerateMethod==	dMONSTER_GENERATE_ACTIVE_AREA	)
			CheckDlgButton(_hDlg,IDC_GENERATE_METHOD,BST_CHECKED);
		if	(s_lpEditActor->m_wGenerateMethod==	dMONSTER_GENERATE_SET_POS		)
			CheckDlgButton(_hDlg,IDC_GENERATE_METHOD,BST_UNCHECKED);
		
		if	(s_lpEditActor->m_wIsBlockToAutoRegen==	0xffff							)
			CheckDlgButton(_hDlg,IDC_BLOCK_TO_AUTO_REGEN,BST_INDETERMINATE);
		if	(s_lpEditActor->m_wIsBlockToAutoRegen	==	TRUE)
			CheckDlgButton(_hDlg,IDC_BLOCK_TO_AUTO_REGEN,BST_CHECKED);
		if	(s_lpEditActor->m_wIsBlockToAutoRegen	==	FALSE)
			CheckDlgButton(_hDlg,IDC_BLOCK_TO_AUTO_REGEN,BST_UNCHECKED);
		// insu add
		if	(s_lpEditActor->m_wGenerateMethod == dMONSTER_GENERATE_AROUND)
		{
			ShowGenerateMethodWindow(_hDlg, true);
			CheckDlgButton(_hDlg,IDC_GENERATE_AROUND_METHOD,BST_CHECKED);
			// 나머지값 얻어서 셋팅
			SetDlgItemText(_hDlg, IDC_GENERATE_METHOD_POS_X, _ms("%d", s_lpEditActor->m_wGenerateAroundPosX1));
			SetDlgItemText(_hDlg, IDC_GENERATE_METHOD_POS_Y, _ms("%d", s_lpEditActor->m_wGenerateAroundPosY1));
			SetDlgItemText(_hDlg, IDC_GENERATE_METHOD_WIDTH, _ms("%d", s_lpEditActor->m_wGenerateAroundPosX2));
			SetDlgItemText(_hDlg, IDC_GENERATE_METHOD_HEIGHT, _ms("%d", s_lpEditActor->m_wGenerateAroundPosY2));
		}
		if	(s_lpEditActor->m_wGenerateMethod == dMONSTER_GENERATE_SET_POS)
			CheckDlgButton(_hDlg,IDC_GENERATE_AROUND_METHOD,BST_UNCHECKED);
		// insu add end
	}//	생성 패턴

//	행동 영역
	{
		HWND	hArea	=	GetDlgItem(_hDlg,IDC_WANDER_AREA);

		if (s_lpEditActor->m_wMoveArea	==	0xffff)	cCOMBOBOX::Add(hArea," ",0xffff);

		cCOMBOBOX::Add(hArea,dMSG_NOT_MOVE,dMAX_AREA);
		cCOMBOBOX::Add(hArea,dMSG_WHOLE_AREA,0);

		g_lpActmap->m_pArea->setFirstArea();

		while(1)
		{
			cAREA_INFO	*lpArea	=	g_lpActmap->m_pArea->getNextArea();

			if (!lpArea)	break;

			if (lpArea->isMonsterDen())
				cCOMBOBOX::Add(hArea,lpArea->m_strName,lpArea->m_wSerial);
		}

		cCOMBOBOX::SelectByData(hArea,s_lpEditActor->m_wMoveArea);
	}	//	행동 영역

//	순찰 루트
	{
		HWND	hPatrolRoute	=	GetDlgItem(_hDlg,IDC_PATROL_ROUTE);

		if (s_lpEditActor->m_wPatrolRoute	==	0xffff)	cCOMBOBOX::Add(hPatrolRoute," ",0xffff);

		cCOMBOBOX::SelectByData(hPatrolRoute,s_lpEditActor->m_wPatrolRoute);
	}	//	순찰 루트

//	전투 성향
	{
		HWND	hBattleInclination	=	GetDlgItem(_hDlg,IDC_BATTLE_INCLINATION);

		if (s_lpEditActor->m_wBattleInclination	==	0xffff)	cCOMBOBOX::Add(hBattleInclination," ",0xffff);

		for (int i=0;i<4;i++)	cCOMBOBOX::Add(hBattleInclination,g_strBattleInclination[i],i);

		cCOMBOBOX::SelectByData(hBattleInclination,s_lpEditActor->m_wBattleInclination);
	}	//	전투 성향

	if	(s_lpEditActor->m_bf1IsGuildCrest)
		CheckDlgButton(_hDlg,IDC_IS_GUILD_CREST,TRUE);
	if	(s_lpEditActor->m_bf1IsGuildObject)
		CheckDlgButton(_hDlg,IDC_IS_GUILD_OBJECT,TRUE);

	CheckDlgButton(_hDlg,IDC_IS_HIDE_NAME,s_lpEditActor->m_bf1IsHideName);
	
//	링크 몬스터 관련
	if (s_lpEditActor->m_wLinkMonsterCount	>=	1	&&	!s_isEditMultiActor)
	{
//	링크 방법
		if	(s_lpEditActor->m_wLinkType	==	dONE_WAY_LINK			)
			CheckDlgButton(_hDlg,IDR_ONE_WAY_LINK,TRUE);
		else if (s_lpEditActor->m_wLinkType	==	dTWO_WAY_LINK)
			CheckDlgButton(_hDlg,IDR_TWO_WAY_LINK,TRUE);

//	리더와 요인
		if	(s_lpEditActor->m_bf1IsLeader	)
			CheckDlgButton(_hDlg,IDC_SET_LEADER,TRUE);
		if	(s_lpEditActor->m_isWard		)
			CheckDlgButton(_hDlg,IDC_WARD,TRUE);

//	링크된 녀석들 리스트
		HWND	hList	=	GetDlgItem(_hDlg,IDC_LINK_LIST);

		for (int i=0;i<s_lpEditActor->m_wLinkMonsterCount;i++)
		{
			int	iIndex		=	s_lpEditActor->m_awLinkMonster[i];

			cACTOR	*lpActor=	&g_lpActmap->m_setActor.m_aActor[iIndex];

			if (lpActor->m_isWard	&&	lpActor->m_bf1IsLeader)
			{
				cLISTBOX::Add(hList,_ms("%s[%d]<%s/%s>",lpActor->getName(),iIndex,dMSG_LEADER,dMSG_WARD));
			}
			else
			if (lpActor->m_isWard)
			{
				cLISTBOX::Add(hList,_ms("%s[%d]<%s>",lpActor->getName(),iIndex,dMSG_WARD));
			}
			else
			if (lpActor->m_bf1IsLeader)
			{
				cLISTBOX::Add(hList,_ms("%s[%d]<%s>",lpActor->getName(),iIndex,dMSG_LEADER));
			}
			else
			{
				cLISTBOX::Add(hList,_ms("%s[%d]",lpActor->getName(),iIndex));
			}
		}
	}
	else
	{
		EnableWindow(GetDlgItem(_hDlg,IDC_SET_LEADER),FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDC_WARD),FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDR_ONE_WAY_LINK),FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDR_TWO_WAY_LINK),FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDC_LINK_LIST),FALSE);
		EnableWindow(GetDlgItem(_hDlg,IDC_STATIC_LINK),FALSE);
	}	//	링크 몬스터 관련

	return	TRUE;
}	//	cSET_FIELD_CHARACTER::SetData(_hDlg _hDlg)

//
//	데이터 수집-_-?
BOOL
cSET_FIELD_CHARACTER::GetData(HWND _hDlg)
{
	GetDlgItemText(_hDlg,IDC_NAME,s_lpEditActor->m_strName,dNAME_LENGTH);

//	NPC 타입
	{
		HWND	hTypeList		=	GetDlgItem(_hDlg,IDC_COMBO_TYPE);
		s_lpEditActor->m_wType	=	cCOMBOBOX::GetData(hTypeList);
	}	//	NPC 타입

//	리젠 주기 설정
	{
		int		iRegenHour,iRegenMinute,iRegenSecond;
		BOOL	temp;

		iRegenHour		=	GetDlgItemInt(_hDlg,IDC_REGEN_CYCLE_HOUR,&temp,FALSE);
		if (!temp)	iRegenHour	=	0xffff;

		iRegenMinute	=	GetDlgItemInt(_hDlg,IDC_REGEN_CYCLE_MINUTE,&temp,FALSE);
		if (!temp)	iRegenMinute=	0xffff;

		iRegenSecond	=	GetDlgItemInt(_hDlg,IDC_REGEN_CYCLE_SECOND,&temp,FALSE);
		if (!temp)	iRegenSecond=	0xffff;

		if (iRegenHour	!=	0xffff	||	iRegenMinute	!=	0xffff	||	iRegenSecond	!=	0xffff)
		{
			if (iRegenHour	==	0xffff)	iRegenHour	=	0;
			if (iRegenMinute==	0xffff)	iRegenMinute=	0;
			if (iRegenSecond==	0xffff)	iRegenSecond=	0;

			s_lpEditActor->m_dwRegenerationCycle	=	iRegenHour*60*60 + iRegenMinute*60 + iRegenSecond;
		}
	}	//	리젠 주기 설정

//	방황-_-주기
	{
		int		iActiveMinute,iActiveSecond;
		BOOL	temp;

		iActiveMinute	=	GetDlgItemInt(_hDlg,IDC_WANDER_MINUTE,&temp,FALSE);
		if (!temp)	iActiveMinute	=	0xffff;
		iActiveSecond	=	GetDlgItemInt(_hDlg,IDC_WANDER_SECOND,&temp,FALSE);
		if (!temp)	iActiveSecond	=	0xffff;

		if (iActiveMinute!=0xffff	||	iActiveSecond!=0xffff)
		{
			if (iActiveMinute	==	0xffff)	iActiveMinute	=	0;
			if (iActiveSecond	==	0xffff)	iActiveSecond	=	0;

			s_lpEditActor->m_wWanderCycle	=	iActiveMinute*60 + iActiveSecond;
		}
	}	//	방황-_-주기

//	직시 방향
	{
		HWND	hDirect			=	GetDlgItem(_hDlg,IDC_GAZE_DIRECT);

		s_lpEditActor->m_wDirect	=	cCOMBOBOX::GetData(hDirect);

		switch(IsDlgButtonChecked(_hDlg,IDC_CHANGE_DIRECT))
		{
			case	BST_CHECKED			:
				s_lpEditActor->m_isChangeDirect	=	TRUE;
				break;
			case	BST_UNCHECKED		:
				s_lpEditActor->m_isChangeDirect	=	FALSE;
				break;
		}
	}	//	직시 방향

//	생성 패턴
	{
		switch(IsDlgButtonChecked(_hDlg,IDC_BLOCK_TO_AUTO_REGEN))
		{
			case	BST_CHECKED			:
				s_lpEditActor->m_wIsBlockToAutoRegen=	TRUE;
				break;
			case	BST_UNCHECKED		:
				s_lpEditActor->m_wIsBlockToAutoRegen=	FALSE;
				break;
		}

		switch(IsDlgButtonChecked(_hDlg,IDC_GENERATE_METHOD))
		{
			case	BST_CHECKED			:
				s_lpEditActor->m_wGenerateMethod=	dMONSTER_GENERATE_ACTIVE_AREA;
				break;
			case	BST_UNCHECKED		:
				s_lpEditActor->m_wGenerateMethod=	dMONSTER_GENERATE_SET_POS;
				break;
		}
		// insu add
		switch(IsDlgButtonChecked(_hDlg,IDC_GENERATE_AROUND_METHOD))
		{
			case	BST_CHECKED			:
				s_lpEditActor->m_wGenerateMethod = dMONSTER_GENERATE_AROUND;
				s_lpEditActor->m_wGenerateAroundPosX1 = GetDlgItemInt(_hDlg,IDC_GENERATE_METHOD_POS_X, NULL, FALSE);
				s_lpEditActor->m_wGenerateAroundPosY1 = GetDlgItemInt(_hDlg,IDC_GENERATE_METHOD_POS_Y, NULL, FALSE);
				s_lpEditActor->m_wGenerateAroundPosX2 = GetDlgItemInt(_hDlg,IDC_GENERATE_METHOD_WIDTH, NULL, FALSE);
				s_lpEditActor->m_wGenerateAroundPosY2 = GetDlgItemInt(_hDlg,IDC_GENERATE_METHOD_HEIGHT, NULL, FALSE);
				break;
			case	BST_UNCHECKED		:
				if (s_lpEditActor->m_wGenerateMethod != dMONSTER_GENERATE_ACTIVE_AREA)
					s_lpEditActor->m_wGenerateMethod = dMONSTER_GENERATE_SET_POS;
				break;
		}
		// insu add end
	}	//	직시 방향

	if	(IsDlgButtonChecked(_hDlg,IDC_IS_GUILD_CREST))
		s_lpEditActor->m_bf1IsGuildCrest	=	TRUE;
	else
		s_lpEditActor->m_bf1IsGuildCrest	=	FALSE;

	if	(IsDlgButtonChecked(_hDlg,IDC_IS_GUILD_OBJECT))
		s_lpEditActor->m_bf1IsGuildObject	=	TRUE;
	else
		s_lpEditActor->m_bf1IsGuildObject	=	FALSE;

	if	(IsDlgButtonChecked(_hDlg,IDC_IS_HIDE_NAME))
		s_lpEditActor->m_bf1IsHideName	=	TRUE;
	else
		s_lpEditActor->m_bf1IsHideName	=	FALSE;

//	행동 영역
	{
		HWND	hArea			=	GetDlgItem(_hDlg,IDC_WANDER_AREA);

		s_lpEditActor->m_wMoveArea	=	cCOMBOBOX::GetData(hArea);
	}	//	행동 영역

//	순찰 루트
	{
		HWND	hPatrolRoute	=	GetDlgItem(_hDlg,IDC_PATROL_ROUTE);

		s_lpEditActor->m_wPatrolRoute	=	cCOMBOBOX::GetData(hPatrolRoute);
	}	//	순찰 루트

//	전투 성향
	{
		HWND	hBattleInclination	=	GetDlgItem(_hDlg,IDC_BATTLE_INCLINATION);

		s_lpEditActor->m_wBattleInclination	=	cCOMBOBOX::GetData(hBattleInclination);
	}	//	전투 성향


	if	(s_lpEditActor->m_wLinkMonsterCount	>=	1	&&	!s_isEditMultiActor)
	{
	//	나 리더-_-?
		if	(IsDlgButtonChecked(_hDlg,IDC_IS_GUILD_CREST))
			s_lpEditActor->m_bf1IsGuildCrest	=	TRUE;
		else
			s_lpEditActor->m_bf1IsGuildCrest	=	FALSE;

		if	(IsDlgButtonChecked(_hDlg,IDC_IS_GUILD_OBJECT))
			s_lpEditActor->m_bf1IsGuildObject	=	TRUE;
		else
			s_lpEditActor->m_bf1IsGuildObject	=	FALSE;

		if	(IsDlgButtonChecked(_hDlg,IDC_IS_HIDE_NAME))
			s_lpEditActor->m_bf1IsHideName	=	TRUE;
		else
			s_lpEditActor->m_bf1IsHideName	=	FALSE;

	//	나 리더-_-?
		if	(IsDlgButtonChecked(_hDlg,IDC_SET_LEADER))
			g_lpActmap->m_setActor.setLeader(s_lpEditActor->m_iSerial,TRUE);	//	리더 설정
		else
			g_lpActmap->m_setActor.setLeader(s_lpEditActor->m_iSerial,FALSE);

	//	나 요인-_-?
		if	(IsDlgButtonChecked(_hDlg,IDC_WARD))
			g_lpActmap->m_setActor.setWard(s_lpEditActor->m_iSerial,TRUE);	//	요인 설정
		else
			g_lpActmap->m_setActor.setWard(s_lpEditActor->m_iSerial,FALSE);

	//	링크 방법
		if	(IsDlgButtonChecked(_hDlg,IDR_ONE_WAY_LINK))
			g_lpActmap->m_setActor.setLinkType(s_lpEditActor->m_iSerial,dONE_WAY_LINK);		//	링크 방법
		else
			g_lpActmap->m_setActor.setLinkType(s_lpEditActor->m_iSerial,dTWO_WAY_LINK);		//	링크 방법
	}

	g_lpActmap->LostVirgin();

	return	TRUE;
}

//
//	대화 추가
BOOL
cSET_FIELD_CHARACTER::AddDialog(HWND _lphWnd,BOOL _isEdit)
{
	CConversationContainer*	lpData	=	CSetDialog::Init(_lphWnd,s_lpEditActor->getName(),s_lpEditActor->m_pCC);

	if (!lpData)
		return	FALSE;

	if (!s_lpEditActor->m_pCC)
		s_lpEditActor->m_pCC	=	new CConversationContainer();

	s_lpEditActor->m_pCC->copy(lpData);

	return	TRUE;
}
void
cSET_FIELD_CHARACTER::ShowGenerateMethodWindow(HWND _hDlg, BOOL _bShow)
{
	DWORD dwState = (_bShow)? SW_SHOW : SW_HIDE;
	ShowWindow(GetDlgItem(_hDlg,IDC_GENERATE_METHOD_POS_X), dwState);
	ShowWindow(GetDlgItem(_hDlg,IDC_GENERATE_METHOD_POS_Y), dwState);
	ShowWindow(GetDlgItem(_hDlg,IDC_STATIC_GENERATE_METHOD), dwState);
	ShowWindow(GetDlgItem(_hDlg,IDC_GENERATE_METHOD_WIDTH), dwState);
	ShowWindow(GetDlgItem(_hDlg,IDC_GENERATE_METHOD_HEIGHT), dwState);
}

//
//
LRESULT CALLBACK
cSET_FIELD_CHARACTER::DlgProc(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
{ // 마지막 수정일 : 09.10.30
	switch(message)
	{
		case WM_INITDIALOG	:
		{
			ShowGenerateMethodWindow(hDlg, false);
			SetData(hDlg);
			break;
		}

		case WM_CLOSE		:
			EndDialog(hDlg,IDCANCEL);
			break;

		case WM_COMMAND :
			switch( LOWORD( wParam ) )
            {
				case	IDC_COPY_DIALOG		:
				{
					if (s_lpEditActor->m_pCC)
						g_clipConversationContainer.copy(s_lpEditActor->m_pCC);
					else
						cMSG::Put("에러!","해당 캐릭터는 설정된 대화가 없습니다.");

					break;
				}
				case	IDC_GENERATE_AROUND_METHOD:
					{
						switch(IsDlgButtonChecked(hDlg,IDC_GENERATE_AROUND_METHOD))
						{						
						case	BST_CHECKED			:
							CheckDlgButton(hDlg,IDC_GENERATE_AROUND_METHOD,BST_UNCHECKED);
							ShowGenerateMethodWindow(hDlg, false);
							break;
							
						case	BST_UNCHECKED		:
							CheckDlgButton(hDlg,IDC_GENERATE_AROUND_METHOD,BST_CHECKED);
							CheckDlgButton(hDlg,IDC_GENERATE_METHOD,BST_UNCHECKED);
							ShowGenerateMethodWindow(hDlg, true);
							break;
							
						case	BST_INDETERMINATE	:
							CheckDlgButton(hDlg,IDC_GENERATE_AROUND_METHOD,BST_CHECKED);
							CheckDlgButton(hDlg,IDC_GENERATE_METHOD,BST_UNCHECKED);
							ShowGenerateMethodWindow(hDlg, true);
							break;
						}
						break;
					}
				
				case	IDC_PASTE_DIALOG	:
				{
					if (g_clipConversationContainer.m_wConversationCount	==	0)
						cMSG::Put("에러!","복사해 놓은 대화 세트가 없습니다.");
					else
					{
						if (s_lpEditActor->m_pCC && s_lpEditActor->m_pCC->m_wConversationCount)
						{
							if(cMSG::YESNO("","이미 대화가 있는 캐릭터 입니다. 기존의 대화에 붙여 넣으시겠습니까?")==IDNO)
								break;

							CConversationContainer	temp;
							
							temp.copy(s_lpEditActor->m_pCC);

							s_lpEditActor->m_pCC->reset();
							s_lpEditActor->m_pCC->init(g_clipConversationContainer.m_wConversationCount+temp.m_wConversationCount,TRUE);

							int i;
							for (i=0;i<g_clipConversationContainer.m_wConversationCount;i++)
								s_lpEditActor->m_pCC->addSpeech(&g_clipConversationContainer.m_pConversation[i]);
							for (i=0;i<temp.m_wConversationCount;i++)
								s_lpEditActor->m_pCC->addSpeech(&temp.m_pConversation[i]);
						}
						else
						{
							if (!s_lpEditActor->m_pCC)
								s_lpEditActor->m_pCC	=	new CConversationContainer();

							s_lpEditActor->m_pCC->copy(&g_clipConversationContainer);

							break;
						}
					}
					break;
				}

				case	IDC_REMOVE_AND_PASTE_DIALOG	:
				{
					if (g_clipConversationContainer.m_wConversationCount	==	0)
						cMSG::Put("에러!","복사해 놓은 대화 세트가 없습니다.");
					else
					{
						if (s_lpEditActor->m_pCC && s_lpEditActor->m_pCC->m_wConversationCount)
						{
							if(cMSG::YESNO("","이미 대화가 있는 캐릭터 입니다. 기존의 대화를 제거 하고 붙여 넣으시겠습니까?")==IDNO)
								break;
						}

						if (!s_lpEditActor->m_pCC)
							s_lpEditActor->m_pCC	=	new CConversationContainer();

						s_lpEditActor->m_pCC->copy(&g_clipConversationContainer);
					}
					break;
				}

				case	IDOK				:
				{
					if (GetData(hDlg))	EndDialog(hDlg,IDOK);
					cSetDialogNpcData::ResetNpcListBox(	cSetDialogNpcData::s_iSelOldList);
					CSetDialogMonster::ResetMonsterListBox(	CSetDialogMonster::s_iSelOldList);
					CSetDialogMonster::ResetMonsterInfoBox(	CSetDialogMonster::s_iSelOldInfo);
					break;
				}

				case	IDCANCEL			:
				{
					if (g_backupConversationContainer.m_wConversationCount)
					{
						KILL(s_lpEditActor->m_pCC);
						s_lpEditActor->m_pCC	=	new CConversationContainer();

						s_lpEditActor->m_pCC->copy(&g_backupConversationContainer);
					}
					else
						KILL(s_lpEditActor->m_pCC);

					EndDialog(hDlg,IDCANCEL);
					break;
				}

				case	IDC_DIALOG			:
				{
					AddDialog(hDlg);
					break;
				}

				case	IDC_REMOVE_DIALOG	:
				{
					if(cMSG::YESNO("","정말로 대화를 지울껴?")==IDNO)
						break;
				
					KILL(s_lpEditActor->m_pCC);

					break;
				}

				case	IDR_TWO_WAY_LINK	:
					CheckDlgButton(hDlg,IDR_ONE_WAY_LINK,FALSE);
					CheckDlgButton(hDlg,IDR_TWO_WAY_LINK,TRUE);
					break;

				case	IDR_ONE_WAY_LINK	:
					CheckDlgButton(hDlg,IDR_ONE_WAY_LINK,TRUE);
					CheckDlgButton(hDlg,IDR_TWO_WAY_LINK,FALSE);
					break;

				case	IDR_NPC				:
					CheckDlgButton(hDlg,IDR_NPC,TRUE);
					CheckDlgButton(hDlg,IDR_MONSTER,FALSE);
					break;

				case	IDR_MONSTER			:
					CheckDlgButton(hDlg,IDR_NPC,FALSE);
					CheckDlgButton(hDlg,IDR_MONSTER,TRUE);
					break;

				case	IDC_GENERATE_METHOD	:
					switch(IsDlgButtonChecked(hDlg,IDC_GENERATE_METHOD))
					{
						case	BST_CHECKED			:
							CheckDlgButton(hDlg,IDC_GENERATE_METHOD,BST_UNCHECKED);
							break;

						case	BST_UNCHECKED		:
							CheckDlgButton(hDlg,IDC_GENERATE_METHOD,BST_CHECKED);
							CheckDlgButton(hDlg,IDC_GENERATE_AROUND_METHOD,BST_UNCHECKED); // insu add
							ShowGenerateMethodWindow(hDlg, false);	// insu add
							break;

						case	BST_INDETERMINATE	:
							CheckDlgButton(hDlg,IDC_GENERATE_METHOD,BST_CHECKED);
							CheckDlgButton(hDlg,IDC_GENERATE_AROUND_METHOD,BST_UNCHECKED);	// insu add
							ShowGenerateMethodWindow(hDlg, false);	// insu add
							break;
					}
					break;

				case	IDC_BLOCK_TO_AUTO_REGEN	:
					switch(IsDlgButtonChecked(hDlg,IDC_BLOCK_TO_AUTO_REGEN))
					{
						case	BST_CHECKED			:
							CheckDlgButton(hDlg,IDC_BLOCK_TO_AUTO_REGEN,BST_UNCHECKED);
							break;

						case	BST_UNCHECKED		:
							CheckDlgButton(hDlg,IDC_BLOCK_TO_AUTO_REGEN,BST_CHECKED);
							break;

						case	BST_INDETERMINATE	:
							CheckDlgButton(hDlg,IDC_BLOCK_TO_AUTO_REGEN,BST_CHECKED);
							break;
					}
					break;

				case	IDC_CHANGE_DIRECT	:
					switch(IsDlgButtonChecked(hDlg,IDC_CHANGE_DIRECT))
					{
						case	BST_CHECKED			:
							CheckDlgButton(hDlg,IDC_CHANGE_DIRECT,BST_UNCHECKED);
							break;

						case	BST_UNCHECKED		:
							CheckDlgButton(hDlg,IDC_CHANGE_DIRECT,BST_CHECKED);
							break;

						case	BST_INDETERMINATE	:
							CheckDlgButton(hDlg,IDC_CHANGE_DIRECT,BST_CHECKED);
							break;
					}
					break;
			}
			break;
	}

	return (0L);
}