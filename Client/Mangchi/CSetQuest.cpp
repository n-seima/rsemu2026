#include "resource.H"
#include "cSET.H"
#include "cMAPWND.H"
#include "cMAIN.H"
#include "cMESSAGE.H"
#include "cQUEST.H"
#include "cDRAW.H"
#include "cTEXT.H"

#define			dNEW_QUEST_NEW		0
#define			dNEW_QUEST_EDIT		1

cLISTCONTROL	cSETQUEST::s_listQuest;
cDIBWND			cSETQUEST::s_dibQuestObject;
int				cSETQUEST::s_iState;					//	수정이냐? 아님 추가냐?
int				cSETQUEST::s_iSelectQuest	=	0;
int				cSETQUEST::s_iSelectQuestType=	eQT_NORMAL;
CQuest*			cSETQUEST::s_lpQuest		=	NULL;

BOOL
cSETQUEST::SetQuest(HINSTANCE hinst,HWND hwnd)
{
	s_hInst		=	hinst;
	int	result	=	DialogBox( hinst,"QUEST",hwnd,(DLGPROC)s_procQuestBox);

	SetLogFolder();

	return	TRUE;
}	//	cSETQUEST::SetValue(HINSTANCE hinst,HWND hwnd)

//
//	퀘스트 창 초기화
void
cSETQUEST::InitQuestBox(HWND hwnd)
{
	int		i,iCount;

	HWND	hQuestList	=	GetDlgItem(hwnd,IDC_QUEST_LIST);
	HWND	hQuestObject=	GetDlgItem(hwnd,IDC_QUEST_OBJECT);
	HWND	hQuestType	=	GetDlgItem(hwnd,IDC_QUEST_TYPE);

	switch(s_iSelectQuestType)
	{
		case	eQT_NORMAL	:
			s_lpQuest	=	&g_quest;
			break;

		case	eQT_ARBEIT	:
			s_lpQuest	=	&g_questArbeit;
			break;
	}

	for (i=0;;i++)
	{
		if	(stricmp(g_strQuestKind[i],"end")==0)
			break;

		cCOMBOBOX::Add(hQuestType,g_strQuestKind[i],i);
	}

	cCOMBOBOX::Select(hQuestType,s_iSelectQuestType);

	s_iSelectQuest		=	0xffff;

	s_listQuest.Init		(hQuestList				,2	,FALSE,s_procQuestList);		//	3개의 컬럼
	s_listQuest.AddColumn	("Index"				,6*7);
	s_listQuest.AddColumn	(dMSG_QUEST_LIST		,6*39);

	s_dibQuestObject.lpszRegister	=	"QUEST_OBJECT_WND";
	s_dibQuestObject.Init	(hQuestObject,(WNDPROC)s_procObjectView);

	for (i=0,iCount=0;i<dQUEST_COUNT && iCount < s_lpQuest->m_iCount;i++)
	{
		s_listQuest.AddItem(0,_ms("%5d",i));
		s_listQuest.AddItem(1,s_lpQuest->getName(i));
		s_listQuest.IncreaseLineCounter();
		iCount++;
	}

	if (s_iSelectQuest>=s_lpQuest->m_iCount)	s_iSelectQuest	=	0;

	s_listQuest.Select(s_iSelectQuest);

	SetSelectQuest();

	DrawQuest(hwnd);
}

//
//	리스트 박스에 선택된 넘을 참조해서 선택된 퀘스트를 알아온다.
void
cSETQUEST::SetSelectQuest()
{
	char	*str	=	s_listQuest.GetSelectStr(1);			//	선택한 넘을 찾는다.

	if	(str)
		s_iSelectQuest	=	s_lpQuest->getIndex(str);		//	선택한 넘을 찾는다.(퀘스트 인덱스 설정)
	else
		s_iSelectQuest	=	0xffff;

}	//	cSETQUEST::SetSelectQuest()

//
//	리스트를 그려준다.
void
cSETQUEST::DrawQuest(HWND hDlg)
{
	s_listQuest.Draw();

	if	(!s_dibQuestObject.Active())
		return;

	SetSelectQuest();

	cDRAW::Fill(0);

	if (s_iSelectQuest	==	0xffff)
	{
		cTEXT::FitPut(2,2,cDRAW::WIDTH-4,cDRAW::HEIGHT-4,WHITE,"valid string");
		s_dibQuestObject.Draw();

		return;
	}
	
	int		i,iY	=	20;

	CQuestItem	*lpQuest	=	&s_lpQuest->m_aQuest[s_iSelectQuest];

	cTEXT::cPut(cDRAW::WIDTH/2,iY,GOLDYELLOW,lpQuest->m_strName);	

	iY	+=	26;

	if (lpQuest->m_isEnableCancel					)
	{
		cTEXT::Put(8,iY,LTCYAN,"※ 퀘스트 취소 가능");
		iY	+=	14;
	}
	if (lpQuest->m_isRemoveQuestItemByCancelQuest	)
	{
		cTEXT::Put(8,iY,LTCYAN,"※ 퀘스트를 취소하거나 완료하면 관련 아이템 제거");
		iY	+=	14;
	}
	if (lpQuest->m_isTestQuestInTestServer			)
	{
		cTEXT::Put(8,iY,LTCYAN,"※ 테스트 서버에서만 돌아가는 퀘스트");
		iY	+=	14;
	}

	if (lpQuest->m_isEnableCancel+lpQuest->m_isRemoveQuestItemByCancelQuest+lpQuest->m_isTestQuestInTestServer)
		iY	+=	16;

	if (lpQuest->m_strQuestClientInfo[0])
	{
		char	*lpstr		=	_ms("- %c%s%c",1,lpQuest->m_strQuestClientInfo,2);
		cTEXT::FitPut(8,iY,cDRAW::WIDTH-20,200,WHITE,"퀘스트 의뢰인 정보");
		iY	+=	16;
		cTEXT::FitPut(20,iY,cDRAW::WIDTH-32,200,WHITE,lpstr);

		int		iLineCount	=	cTEXT::GetLines(cDRAW::WIDTH-20,lpstr);

		iY	+=	iLineCount*14;
	}

	if (lpQuest->m_strQuestValueComment[0])
	{
		if (lpQuest->m_strQuestClientInfo[0])	iY	+=	10;

		char	*lpstr		=	_ms("- %c%s%c",1,lpQuest->m_strQuestValueComment,2);
		cTEXT::FitPut(8,iY,cDRAW::WIDTH-20,200,WHITE,"퀘스트 수치 표현 방법");
		iY	+=	16;
		cTEXT::FitPut(20,iY,cDRAW::WIDTH-32,200,WHITE,lpstr);

		int		iLineCount	=	cTEXT::GetLines(cDRAW::WIDTH-20,lpstr);

		iY	+=	iLineCount*14;
	}

	iY	+=	16;

	for (i=0;i<dQUEST_STATUS_COUNT;i++)
	{
		int	iExplainLength	=	strlen(lpQuest->m_astrExplain[i]);

		if (iExplainLength	<=	0)	continue;

		char	strExplain[2048];

		sprintf(strExplain," %d 단계 : %s",i+1,lpQuest->m_astrExplain[i]);

		cTEXT::FitPut(2,iY,cDRAW::WIDTH-4,cDRAW::HEIGHT-4,WHITE,strExplain);
		int		iLineCount	=	cTEXT::GetLines(cDRAW::WIDTH-4,strExplain);

		iY	+=	iLineCount*12+12;
	}

	//	그려줘~~
	s_dibQuestObject.Draw();
}	//	cSETQUEST::DrawList(HWND hDlg,BOOL value)

//
//	퀘스트를 제거한다.
void
cSETQUEST::RemoveQuest(HWND hwnd)
{
	int		iResult;
	BOOL	bIsArbeit;

	iResult		=	cMSG::YESNO("퀘스트 삭제","선택한 퀘스트를 삭제하시겠습니까?");

	if (iResult	==	IDNO)	return;

	if (s_iSelectQuestType	==	eQT_ARBEIT)	bIsArbeit	=	TRUE;

	if (s_lpQuest->remove(s_listQuest.GetSelectStr(1),bIsArbeit))
	{
		s_listQuest.SetSelectItemStr(1,"Removed Quest");
		DrawQuest(hwnd);
	}
}	//	cSETQUEST::RemoveQuest(HWND hwnd)

//
//	퀘스트 타입 변경
void
cSETQUEST::ChangeQuestType(HWND _hDlg)		// 09.08.10 수정
{
	HWND	hQuestType	=	GetDlgItem(_hDlg,IDC_QUEST_TYPE);

	int	iSelectType		=	cCOMBOBOX::GetData(hQuestType);

	if (iSelectType		==	s_iSelectQuestType)	return;

	s_iSelectQuestType	=	iSelectType;

	switch(s_iSelectQuestType)
	{
		case	eQT_NORMAL	:
			s_lpQuest	=	&g_quest;
			break;

		case	eQT_ARBEIT	:
			s_lpQuest	=	&g_questArbeit;
			break;
	}
	char strFilter[256] = "";
	GetDlgItemText(_hDlg,IDC_QUEST_FILTER,strFilter,256);

	if(strFilter != NULL){
		FilterQuest(_hDlg);
		return;
	}

	int	i,iCount;

	s_listQuest.ResetItems();

	for (i=0,iCount=0;i<dQUEST_COUNT && iCount < s_lpQuest->m_iCount;i++)
	{
		s_listQuest.AddItem(0,_ms("%5d",i));
		s_listQuest.AddItem(1,s_lpQuest->getName(i));
		s_listQuest.IncreaseLineCounter();
		iCount++;
	}

	s_listQuest.Select(0);

	DrawQuest(_hDlg);

}

void
cSETQUEST::FilterQuest(HWND _hDlg)		// 09.08.10
{

	s_listQuest.ResetItems();
		
	char strFilter[256] = "";
	GetDlgItemText(_hDlg,IDC_QUEST_FILTER,strFilter,256);
	int iFilterNameLen	= strlen(strFilter);
	
	int	iCount =0;

	for(int i=0;i<dQUEST_COUNT && iCount < s_lpQuest->m_iCount; ++i)
	{
		
		int iItemNameLen	= strlen(s_lpQuest->getName(i));
		int iLenGap			= iItemNameLen - iFilterNameLen;
		
		if(iLenGap<0)
			continue;
		
		for(int iName=0;iName<=iLenGap;++iName)
		{
			if(strnicmp(strFilter,&s_lpQuest->getName(i)[iName], iFilterNameLen) == 0 )
			{
				s_listQuest.AddItem(0,_ms("%5d",i));
				s_listQuest.AddItem(1,s_lpQuest->getName(i));
				
				s_listQuest.IncreaseLineCounter();
				++iCount;
				break;
			}
		}
	}

	s_listQuest.Select(0);

	DrawQuest(_hDlg);
}


LRESULT CALLBACK
cSETQUEST::s_procQuestBox(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	switch(uMsg)
	{
		case WM_INITDIALOG	:
			InitQuestBox(hwnd);
			break;

		case WM_CLOSE		:
			EndDialog(hwnd,IDCANCEL);
			break;

		case WM_COMMAND :
			switch( LOWORD( wParam ) )
            {

				case	IDC_QUEST_TYPE		:
				{
					ChangeQuestType(hwnd);
					break;
				}

				case	IDC_NEW				:
				{
					s_iState	=	dNEW_QUEST_NEW;
					NewQuest(s_hInst,hwnd);
					break;
				}

				case	IDC_EDIT			:
				{
					if (!s_listQuest.GetSelectStr(1))	break;
					s_iState	=	dNEW_QUEST_EDIT;
					NewQuest(s_hInst,hwnd);
					break;
				}

				case	IDC_REMOVE			:
				{
					RemoveQuest(hwnd);
					break;
				}

				case	IDOK				:
				{
					EndDialog(hwnd,IDOK);
					break;
				}
				
				case	IDCANCEL			:
					{
						EndDialog(hwnd,IDCANCEL);
						break;
					}
				case	IDC_QUEST_FILTER	:		// 09.08.10
					{
						if	(HIWORD(wParam) != EN_CHANGE)
							break;
						FilterQuest(hwnd);
						
						break;
					}
			}
			break;
	}

	return (0L);
}

LRESULT CALLBACK
cSETQUEST::s_procObjectView(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_dibQuestObject.Active();
			s_dibQuestObject.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}

		default					:	return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}


//
//
LRESULT CALLBACK 
cSETQUEST::s_procQuestList(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)		// 09.08.11	
{
	switch(message)
	{
	case WM_LBUTTONDOWN	:
		{
			s_iSelectQuest	=	s_listQuest.GetSelectData();
			DrawQuest(hDlg);
			SetFocus(hDlg);
			break;
		}
	case WM_MOUSEWHEEL :
		{
			int zDelta = ((short)HIWORD(wParam));
			
			
			if	(zDelta>=0)
			{
				s_listQuest.VScroll(SB_LINEUP);
			}
			else{
				s_listQuest.VScroll(SB_LINEDOWN);
			}
			DrawQuest(hDlg);
			break;
		}
	}

	return (0L);
}	//	cSET_ITEM_PREFIX::s_procItemPrefixListEvent(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)

/////////////////////////////////////////////////////////////////////////////////////
//	퀘스트 추가
/////////////////////////////////////////////////////////////////////////////////////

BOOL
cSETQUEST::NewQuest(HINSTANCE hinst,HWND hwnd)
{
	int	result	=	DialogBox( hinst,"QUEST_NEW",hwnd,(DLGPROC)s_procAddQuest);

	return	TRUE;
}

//
//	퀘스트 창 초기화
void
cSETQUEST::InitNewQuestBox(HWND hwnd)
{
	if	(g_hEditFont)
	{
		HWND	hDialogText;

		int		aiID[]	=	
		{
			IDC_BEGIN_NPC,IDC_QUEST_NAME,IDC_QUEST_CLIENT_INFO,IDC_QUEST_VALUE_COMMENT,
			IDC_LEVEL_1,IDC_LEVEL_2,IDC_LEVEL_3,IDC_LEVEL_4,IDC_LEVEL_5,IDC_LEVEL_6,-1
		};

		for (int i=0;aiID[i]!=-1;i++)
		{
			hDialogText	=	GetDlgItem(hwnd,aiID[i]);
			SendMessage(hDialogText,WM_SETFONT,(WPARAM)g_hEditFont, 0 );
		}
	}

//	SendMessage(hwnd,WM_SETFONT,(WPARAM)GetStockObject( DEFAULT_GUI_FONT ), 0 );

//	접두사 타입
	HWND	hQuestKind	=	GetDlgItem(hwnd,IDC_QUEST_KIND);

	{
		int		iIndex		=	0;

		while(1)
		{
			if (stricmp(g_astrQuestCategory[iIndex],"end")==0)
				break;

			cCOMBOBOX::Add(hQuestKind,g_astrQuestCategory[iIndex],iIndex);

			iIndex++;
		}
	}	//	접두사 타입

	if (s_iState	==	dNEW_QUEST_EDIT)
	{
		int		index			=	s_lpQuest->getIndex(s_listQuest.GetSelectStr(1));

		CQuestItem	*lpQuest	=	s_lpQuest->getQuest(index);

		SetDlgItemText(hwnd,IDC_QUEST_NAME			,lpQuest->m_strName);
		SetDlgItemText(hwnd,IDC_QUEST_CLIENT_INFO	,lpQuest->m_strQuestClientInfo);
		SetDlgItemText(hwnd,IDC_QUEST_VALUE_COMMENT	,lpQuest->m_strQuestValueComment);
		
		SetDlgItemText(hwnd,IDC_BEGIN_NPC			,lpQuest->m_strBeginNPCName);
		SetDlgItemInt(hwnd,IDC_BEGIN_NPC_FIELD		,lpQuest->m_wBeginNPC_Field,FALSE);
		SetDlgItemInt(hwnd,IDC_MIN_PROPRIETY_LEVEL	,lpQuest->m_wMinProprietyLevel,FALSE);
		SetDlgItemInt(hwnd,IDC_MAX_PROPRIETY_LEVEL	,lpQuest->m_wMaxProprietyLevel,FALSE);
		SetDlgItemInt(hwnd,IDC_MIN_LIMIT_LEVEL		,lpQuest->m_wMinLimitLevel,FALSE);
		SetDlgItemInt(hwnd,IDC_MAX_LIMIT_LEVEL		,lpQuest->m_wMaxLimitLevel,FALSE);

		SetDlgItemInt(hwnd,IDC_BEGIN_YEAR			,lpQuest->m_wBeginYear,FALSE);
		SetDlgItemInt(hwnd,IDC_BEGIN_MONTH			,lpQuest->m_wBeginMonth,FALSE);
		SetDlgItemInt(hwnd,IDC_BEGIN_DAY			,lpQuest->m_wBeginDay,FALSE);
		SetDlgItemInt(hwnd,IDC_EXPIRE_YEAR			,lpQuest->m_wExpireYear,FALSE);
		SetDlgItemInt(hwnd,IDC_EXPIRE_MONTH			,lpQuest->m_wExpireMonth,FALSE);
		SetDlgItemInt(hwnd,IDC_EXPIRE_DAY			,lpQuest->m_wExpireDay,FALSE);

		CheckDlgButton(hwnd,IDC_ENABLE_CANCEL_QUEST			,lpQuest->m_isEnableCancel);
		CheckDlgButton(hwnd,IDC_REMOVE_ITEM_BY_CANCEL_QUEST	,lpQuest->m_isRemoveQuestItemByCancelQuest);
		CheckDlgButton(hwnd,IDC_TEST_QUEST					,lpQuest->m_isTestQuestInTestServer);

		for (int i=0;i<dQUEST_STATUS_COUNT;i++)
			SetDlgItemText(hwnd,IDC_LEVEL_1	+i	,s_lpQuest->m_aQuest[index].m_astrExplain[i]);

		SetWindowText(GetDlgItem(hwnd,IDOK		),dMSG_REVISE_QUEST);
		SetWindowText(GetDlgItem(hwnd,IDCANCEL	),dMSG_REVISE_QUEST_CANCEL);

		cCOMBOBOX::SelectByData(hQuestKind,lpQuest->m_wQuestKind);

//	사용/착용 가능 직업
		{
			for (int i=0;;i++)
			{
				int	iID	=	g_aiEnableJobIDCList[i];

				if	(iID	==	-1)
					break;

				if (lpQuest->isEnableJob(i))
					CheckDlgButton(hwnd,iID,TRUE);
				else
					CheckDlgButton(hwnd,iID,FALSE);
			}

			if (lpQuest->m_bf1IsMaleJobEquipAble)
				CheckDlgButton(hwnd,IDC_MALE,TRUE);
			if (lpQuest->m_bf1IsFemaleJobEquipAble)
				CheckDlgButton(hwnd,IDC_FEMALE,TRUE);
		}
		if (lpQuest->m_bf1IsDisableQuest)
			CheckDlgButton(hwnd, IDC_DISABLE_QUEST, TRUE);
		if (lpQuest->m_bf6ChapterNumber)
			SetDlgItemInt(hwnd,IDC_CHAPTER_NUMBER, lpQuest->m_bf6ChapterNumber, FALSE);
		if (lpQuest->m_bf6PartNumber)
			SetDlgItemInt(hwnd,IDC_PART_NUMBER, lpQuest->m_bf6PartNumber, FALSE);
		if (lpQuest->m_bf3Quarter)
			SetDlgItemInt(hwnd,IDC_QUARTER, lpQuest->m_bf3Quarter, FALSE);
	}
	else
	{
		SetDlgItemInt(hwnd,IDC_BEGIN_NPC_FIELD		,65535,FALSE);

		SetDlgItemInt(hwnd,IDC_MIN_PROPRIETY_LEVEL	,0,FALSE);
		SetDlgItemInt(hwnd,IDC_MAX_PROPRIETY_LEVEL	,0,FALSE);
		SetDlgItemInt(hwnd,IDC_MIN_LIMIT_LEVEL		,0,FALSE);
		SetDlgItemInt(hwnd,IDC_MAX_LIMIT_LEVEL		,0,FALSE);
	
		SetDlgItemInt(hwnd,IDC_BEGIN_YEAR	,0,FALSE);
		SetDlgItemInt(hwnd,IDC_BEGIN_MONTH	,0,FALSE);
		SetDlgItemInt(hwnd,IDC_BEGIN_DAY	,0,FALSE);
		SetDlgItemInt(hwnd,IDC_EXPIRE_YEAR	,0,FALSE);
		SetDlgItemInt(hwnd,IDC_EXPIRE_MONTH	,0,FALSE);
		SetDlgItemInt(hwnd,IDC_EXPIRE_DAY	,0,FALSE);

		cCOMBOBOX::SelectByData(hQuestKind,0);

//	사용/착용 가능 직업
		{
			for (int i=0;;i++)
			{
				int	iID	=	g_aiEnableJobIDCList[i];

				if	(iID==	-1)
					break;

				CheckDlgButton(hwnd,iID,TRUE);
			}

			CheckDlgButton(hwnd,IDC_MALE,TRUE);
			CheckDlgButton(hwnd,IDC_FEMALE,TRUE);
		}

	}

	SetFocus(GetDlgItem(hwnd,IDC_QUEST_NAME));
}

//
//	값을 추가한다.
BOOL
cSETQUEST::AddQuest(HWND hwnd)
{
	CQuestItem	quest;
	BOOL		temp;

	if (GetDlgItemText(hwnd,IDC_QUEST_NAME	,quest.m_strName	,dQUEST_NAME_SIZE		)	==	0)
	{
		SetFocus(GetDlgItem(hwnd,IDC_QUEST_NAME));	//	포커스를 해당 윈도우로 맞춰준다.
		return	ERRMSG(dMSG_ADD_QUEST_ERROR,dMSG_VALID_QUEST_NAME);
	}

	GetDlgItemText(hwnd,IDC_QUEST_CLIENT_INFO	,quest.m_strQuestClientInfo	,dQUEST_CLIENT_INFO_SIZE);

	quest.m_wBeginYear		=	GetDlgItemInt(hwnd,IDC_BEGIN_YEAR	,&temp,FALSE);
	quest.m_wBeginMonth		=	GetDlgItemInt(hwnd,IDC_BEGIN_MONTH	,&temp,FALSE);
	quest.m_wBeginDay		=	GetDlgItemInt(hwnd,IDC_BEGIN_DAY	,&temp,FALSE);
	quest.m_wExpireYear		=	GetDlgItemInt(hwnd,IDC_EXPIRE_YEAR	,&temp,FALSE);
	quest.m_wExpireMonth	=	GetDlgItemInt(hwnd,IDC_EXPIRE_MONTH	,&temp,FALSE);
	quest.m_wExpireDay		=	GetDlgItemInt(hwnd,IDC_EXPIRE_DAY	,&temp,FALSE);
	quest.m_wQuestKind		=	cCOMBOBOX::GetData(GetDlgItem(hwnd,IDC_QUEST_KIND));

	quest.m_isEnableCancel						=	IsDlgButtonChecked(hwnd,IDC_ENABLE_CANCEL_QUEST			);
	quest.m_isRemoveQuestItemByCancelQuest		=	IsDlgButtonChecked(hwnd,IDC_REMOVE_ITEM_BY_CANCEL_QUEST	);
	quest.m_isTestQuestInTestServer				=	IsDlgButtonChecked(hwnd,IDC_TEST_QUEST					);

	GetDlgItemText(hwnd,IDC_BEGIN_NPC	,quest.m_strBeginNPCName	,dNAME_LENGTH-2);

	quest.m_wBeginNPC_Field		=	GetDlgItemInt(hwnd,IDC_BEGIN_NPC_FIELD			,&temp,FALSE);
	quest.m_wMinProprietyLevel	=	GetDlgItemInt(hwnd,IDC_MIN_PROPRIETY_LEVEL		,&temp,FALSE);
	quest.m_wMaxProprietyLevel	=	GetDlgItemInt(hwnd,IDC_MAX_PROPRIETY_LEVEL		,&temp,FALSE);
	quest.m_wMinLimitLevel		=	GetDlgItemInt(hwnd,IDC_MIN_LIMIT_LEVEL			,&temp,FALSE);
	quest.m_wMaxLimitLevel		=	GetDlgItemInt(hwnd,IDC_MAX_LIMIT_LEVEL			,&temp,FALSE);

//	직업
	{
		quest.m_uiEnableJobMask	=	0;

		for (int i=0;;i++)
		{
			int	iID	=	g_aiEnableJobIDCList[i];

			if	(iID==	-1)
				break;

			if	(IsDlgButtonChecked(hwnd,iID))
				quest.m_uiEnableJobMask	+=	(1<<i);
		}

		if	(IsDlgButtonChecked(hwnd,IDC_MALE))	
			quest.m_bf1IsMaleJobEquipAble	=	1;
		else
			quest.m_bf1IsMaleJobEquipAble	=	0;

		if	(IsDlgButtonChecked(hwnd,IDC_FEMALE))
			quest.m_bf1IsFemaleJobEquipAble	=	1;
		else
			quest.m_bf1IsFemaleJobEquipAble	=	0;
	}

	for (int i=0;i<dQUEST_STATUS_COUNT;i++)
		GetDlgItemText(hwnd,IDC_LEVEL_1	+i	,quest.m_astrExplain[i],dQUEST_EXPLAIN_SIZE-1);

	quest.m_wQuestType	=	eQT_LIVE;

	if	(s_lpQuest->add(&quest))		//	퀘스트를 추가한다.
	{
		s_listQuest.AddItem(0,_ms("%5d",s_lpQuest->m_iCount-1));
		s_listQuest.AddItem(1,quest.m_strName);
		s_listQuest.IncreaseLineCounter();

		DrawQuest(hwnd);

		SetDlgItemText(hwnd,IDC_QUEST_NAME	,"");
		
		for (int i=0;i<dQUEST_STATUS_COUNT;i++)
			SetDlgItemText(hwnd,IDC_LEVEL_1	+i	,"");

		SetFocus(GetDlgItem(hwnd,IDC_QUEST_NAME));

		return	TRUE;
	}

	return	FALSE;
}

//
//	값을 추가한다.
BOOL
cSETQUEST::ReviseQuest(HWND hwnd)
{
	CQuestItem	quest;
	BOOL		temp;

	if (GetDlgItemText(hwnd,IDC_QUEST_NAME	,quest.m_strName		,dQUEST_NAME_SIZE		)	==	0)
		return	ERRMSG(dMSG_ADD_QUEST_ERROR,dMSG_VALID_QUEST_NAME);

	for (int i=0;i<dQUEST_STATUS_COUNT;i++)
		GetDlgItemText(hwnd,IDC_LEVEL_1	+i	,quest.m_astrExplain[i],dQUEST_EXPLAIN_SIZE-1);

	GetDlgItemText(hwnd,IDC_QUEST_CLIENT_INFO	,quest.m_strQuestClientInfo	,dQUEST_CLIENT_INFO_SIZE);
	GetDlgItemText(hwnd,IDC_QUEST_VALUE_COMMENT	,quest.m_strQuestValueComment	,dQUEST_VALUE_COMMENT_SIZE);
	
	quest.m_wBeginYear		=	GetDlgItemInt(hwnd,IDC_BEGIN_YEAR	,&temp,FALSE);
	quest.m_wBeginMonth		=	GetDlgItemInt(hwnd,IDC_BEGIN_MONTH	,&temp,FALSE);
	quest.m_wBeginDay		=	GetDlgItemInt(hwnd,IDC_BEGIN_DAY	,&temp,FALSE);
	quest.m_wExpireYear		=	GetDlgItemInt(hwnd,IDC_EXPIRE_YEAR	,&temp,FALSE);
	quest.m_wExpireMonth	=	GetDlgItemInt(hwnd,IDC_EXPIRE_MONTH	,&temp,FALSE);
	quest.m_wExpireDay		=	GetDlgItemInt(hwnd,IDC_EXPIRE_DAY	,&temp,FALSE);
	quest.m_wQuestKind		=	cCOMBOBOX::GetData(GetDlgItem(hwnd,IDC_QUEST_KIND));

	GetDlgItemText(hwnd,IDC_BEGIN_NPC	,quest.m_strBeginNPCName	,dNAME_LENGTH-2);

	quest.m_wBeginNPC_Field		=	GetDlgItemInt(hwnd,IDC_BEGIN_NPC_FIELD			,&temp,FALSE);
	quest.m_wMinProprietyLevel	=	GetDlgItemInt(hwnd,IDC_MIN_PROPRIETY_LEVEL		,&temp,FALSE);
	quest.m_wMaxProprietyLevel	=	GetDlgItemInt(hwnd,IDC_MAX_PROPRIETY_LEVEL		,&temp,FALSE);
	quest.m_wMinLimitLevel		=	GetDlgItemInt(hwnd,IDC_MIN_LIMIT_LEVEL			,&temp,FALSE);
	quest.m_wMaxLimitLevel		=	GetDlgItemInt(hwnd,IDC_MAX_LIMIT_LEVEL			,&temp,FALSE);

//	직업
	{
		quest.m_uiEnableJobMask	=	0;

		for (int i=0;;i++)
		{
			int	iID	=	g_aiEnableJobIDCList[i];

			if	(iID==	-1)
				break;

			if	(IsDlgButtonChecked(hwnd,iID))
				quest.m_uiEnableJobMask	+=	(1<<i);
		}

		if	(IsDlgButtonChecked(hwnd,IDC_MALE))
			quest.m_bf1IsMaleJobEquipAble	=	1;
		else
			quest.m_bf1IsMaleJobEquipAble	=	0;

		if	(IsDlgButtonChecked(hwnd,IDC_FEMALE))
			quest.m_bf1IsFemaleJobEquipAble	=	1;
		else
			quest.m_bf1IsFemaleJobEquipAble	=	0;
	}

	quest.m_isEnableCancel						=	IsDlgButtonChecked(hwnd,IDC_ENABLE_CANCEL_QUEST				);
	quest.m_isRemoveQuestItemByCancelQuest		=	IsDlgButtonChecked(hwnd,IDC_REMOVE_ITEM_BY_CANCEL_QUEST		);
	quest.m_isTestQuestInTestServer				=	IsDlgButtonChecked(hwnd,IDC_TEST_QUEST						);
	quest.m_bf1IsDisableQuest					=	IsDlgButtonChecked(hwnd, IDC_DISABLE_QUEST);
	quest.m_bf6ChapterNumber					=	GetDlgItemInt(hwnd,IDC_CHAPTER_NUMBER	,&temp,FALSE);
	quest.m_bf6PartNumber						=	GetDlgItemInt(hwnd,IDC_PART_NUMBER	,&temp,FALSE);
	quest.m_bf3Quarter							=	GetDlgItemInt(hwnd,IDC_QUARTER	,&temp,FALSE);

	int index		=	s_lpQuest->getIndex(s_listQuest.GetSelectStr(1));

	int	existQuest	=	s_lpQuest->getIndex(quest.m_strName);

	if (existQuest	!=	0xffff)
		if (existQuest	!=	index)
			return	ERRMSG(dMSG_ADD_QUEST_ERROR,dMSG_EXIST_SAME_NAME_QUEST);

	s_lpQuest->m_aQuest[index].copy(&quest);

	s_listQuest.SetSelectItemStr(1,quest.m_strName);

	DrawQuest(hwnd);

	return	TRUE;
}	//	cSETQUEST::AddQuest(HWND hwnd)

LRESULT CALLBACK
cSETQUEST::s_procAddQuest(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	switch(uMsg)
	{	case WM_INITDIALOG	:
			InitNewQuestBox(hwnd);
			break;

		case WM_CLOSE		:
			EndDialog(hwnd,IDCANCEL);
			break;

		case WM_COMMAND :
			switch( LOWORD( wParam ) )
            {
				case	IDOK				:
				{
					if (s_iState	==	dNEW_QUEST_EDIT)
					{
						if(ReviseQuest(hwnd))	EndDialog(hwnd,IDCANCEL);

						break;
					}

					if (AddQuest(hwnd))	EndDialog(hwnd,IDCANCEL);

					break;
				}

				case	IDCANCEL			:
				{
					EndDialog(hwnd,IDCANCEL);
					break;
				}

				case	IDC_CHECK_ALL		:
				{
					for (int i=0;;i++)
					{
						int	iID	=	g_aiEnableJobIDCList[i];

						if	(iID==	-1)
							break;
						CheckDlgButton(hwnd,iID,TRUE);
					}
					break;
				}

				case	IDC_DECHECK_ALL		:
				{
					for (int i=0;;i++)
					{
						int	iID	=	g_aiEnableJobIDCList[i];

						if	(iID==	-1)
							break;
						CheckDlgButton(hwnd,iID,FALSE);
					}
					break;
				}
				case	IDC_HELP_SCRIPT		:
				{
					DialogBox( s_hInst,"QUEST_SCRIPT_HELP",hwnd,(DLGPROC)s_procQuestHelpBox);

					break;
				}
			}
			break;
	}

	return (0L);
}

void
cSETQUEST::InitQuestHelpBox(HWND hwnd)
{
char	*strText	=
" 1. 모든 텍스트는 따옴표 대신 ' 기호로 둘러싼다\xd\xa \
2. '' 와 같은 경우는 해당 값은 리셋한다.\xd\xa \
3. 필드 시리얼이 -1 인 경우 해당 필드는 무시한다.\xd\xa \
4. 마지막에는 반드시 _codeEnd를 넣는다.\xd\xa \
\xd\xa \
field 1,10,13;\xd\xa \
client 0 '초키' // 고도 브룬넨 슈티키의 초키\xd\xa \
client -1 '건방진 초키' // 건방진 초키\xd\xa \
npc 0 '에린' // 대화대상 : 고도 브룬넨 슈티키의 에린\xd\xa \
npc -1 '에린' // 대화대상 :  에린\xd\xa \
monster 10 '병든 코볼트'; // 동부 플라톤 대로의 병든 코볼트\xd\xa \
monster -1 '병든 코볼트'; // 병든 코볼트\xd\xa \
countDown '배달까지 남은 시간'; // 퀘스트 카운트 다운 타이머 설명(ex 배달까지 남은 시간 : 30분 3초) \xd\xa \
item 2 '녹슨칼' 2;// 필요 아이템 : 녹슨칼 2개\xd\xa \
value '녹슨칼 %d개 수집' \xd\xa \
checker '브룬넨슈티크의 감자에게로 돌아가자'\xd\xa \
clearValue; // 디폴트 퀘스트 수치 초기화\xd\xa \
clearClient; // 디폴트 클라이언트 정보 초기화\xd\xa \
\
_codeEnd";

	SetDlgItemText(hwnd,IDC_TEXT,strText);
}

LRESULT CALLBACK
cSETQUEST::s_procQuestHelpBox(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG	:
			InitQuestHelpBox(hwnd);
			break;

		case WM_CLOSE		:
			EndDialog(hwnd,IDCANCEL);
			break;

		case WM_COMMAND :
			switch( LOWORD( wParam ) )
            {
				case	IDCANCEL	:
					EndDialog(hwnd,IDOK);
					break;
			}
			break;
	}

	return (0L);
}
