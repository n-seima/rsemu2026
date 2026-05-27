#include "resource.H"
#include "cSET.H"
#include "cMAPWND.H"
#include "cMAIN.H"
#include "cMESSAGE.H"


cLISTCONTROL	cSETVALUE::s_ValueList;
HWND			cSETVALUE::s_hTab;
int				cSETVALUE::s_iSelectTab	=	dVALUE_LOCAL;

BOOL
cSETVALUE::SetValue(HINSTANCE hinst,HWND hwnd)
{
	s_hInst		=	hinst;
	int	result	=	DialogBox( hinst,"SET_VALUE",hwnd,(DLGPROC)s_procListBox);

	SetLogFolder();

	return	TRUE;
}	//	cSETVALUE::SetValue(HINSTANCE hinst,HWND hwnd)


void
cSETVALUE::InitListBox(HWND hwnd)
{
	HWND	hName		=	GetDlgItem(hwnd,IDC_VALUE_NAME);
	HWND	hValueList	=	GetDlgItem(hwnd,IDC_VALUE_LIST);

	SetFocus(hName);		//	이름 받는 곳에 포커스를 맞춘다.

	SetDlgItemInt(hwnd,IDC_DEFAULT_VALUE,0,FALSE);

	TC_ITEM item;

	s_hTab			=	GetDlgItem(hwnd,IDC_TAB);

	item.mask		=	TCIF_TEXT | TCIF_PARAM;
	item.pszText	=	dMSG_LOCAL_KARMA_VALUE;
	item.lParam		=	0;
	TabCtrl_InsertItem( s_hTab,0, &item );

	item.mask		=	TCIF_TEXT | TCIF_PARAM;
	item.pszText	=	dMSG_GLOBAL_KARMA_VALUE;
	item.lParam		=	1;
	TabCtrl_InsertItem( s_hTab,1, &item );

	TabCtrl_SetCurSel		(s_hTab					, s_iSelectTab);		//	선택한 탭..

	s_ValueList.Init		(hValueList				,2);		//	3개의 컬럼
	s_ValueList.AddColumn	(dMSG_VALUE_NAME		,6*34);
	s_ValueList.AddColumn	(dMSG_VALUE_DEFUALT		,6*12);

	s_iSelectTab	=	-1;
	ChangeTab(hwnd);

	DrawList(hwnd);
}

//
//	리스트를 그려준다.
//
void
cSETVALUE::DrawList(HWND hDlg,BOOL value)
{
	s_ValueList.Draw();

	if (value)
	{
//		if (s_FileList.GetSelect(0))
//				SetWindowText(GetDlgItem(s_hFileListWnd,IDC_FILE_NAME),s_FileList.GetSelect(0));
//		else	SetWindowText(GetDlgItem(s_hFileListWnd,IDC_FILE_NAME),"");
	}

}	//	cSETVALUE::DrawList(HWND hDlg,BOOL value)


//
//	탭을 바꾼다.
//
void
cSETVALUE::ChangeTab(HWND hdlg)
{
	int iSelectTab	=	TabCtrl_GetCurSel(s_hTab);

	if (iSelectTab	==	s_iSelectTab)	return;

	s_iSelectTab	=	iSelectTab;

	s_ValueList.ResetItems();

	cVALUE	*lpValue;

	if (s_iSelectTab==	dVALUE_LOCAL)
	{
		lpValue	=	&g_lpActmap->m_Value;

		SetWindowText(GetDlgItem(hdlg,IDC_EXPLAIN),dMSG_LOCAL_KARMA_VALUE_EXPLAIN);
	}
	else
	{
		lpValue	=	&g_ProjectValue;

		SetWindowText(GetDlgItem(hdlg,IDC_EXPLAIN),dMSG_GLOBAL_KARMA_VALUE_EXPLAIN);
	}

	for (int i=0,iCount=0;i < dVALUE_COUNT && iCount < lpValue->m_iCount;i++)
	{
		if (lpValue->GetValue(i)	!=	dVALID_VALUE)
		{
			s_ValueList.AddItem(0,lpValue->GetName(i));

			s_ValueList.AddItem(1,_ms("%d",lpValue->GetValue(i)));

			s_ValueList.IncreaseLineCounter();

			iCount++;
		}
	}

	DrawList(hdlg);
}	//	cSETVALUE::ChangeTab(HWND hdlg)

//
//	값을 추가한다.
//
void
cSETVALUE::RemoveValue(HWND hwnd)
{
	HWND	hName	=	GetDlgItem(hwnd,IDC_VALUE_NAME);
	HWND	hValue	=	GetDlgItem(hwnd,IDC_DEFAULT_VALUE);

//	s_ValueList.m_iSelect
	cVALUE	*lpValue;

	if (s_iSelectTab==	dVALUE_LOCAL)
			lpValue	=	&g_lpActmap->m_Value;
	else	lpValue	=	&g_ProjectValue;

	if (lpValue->Remove(s_ValueList.GetSelectStr(0)))
	{
		if (s_iSelectTab==	dVALUE_LOCAL)	g_lpActmap->LostVirgin();

		s_ValueList.RemoveSelectItem();
		DrawList(hwnd);

		SetDlgItemText(hwnd,IDC_VALUE_NAME,"");
		SetDlgItemInt(hwnd,IDC_DEFAULT_VALUE,0,TRUE);

		SetFocus(hName);
	}
}

//
//	값을 추가한다.
//
void
cSETVALUE::AddValue(HWND hwnd)
{
	HWND	hName	=	GetDlgItem(hwnd,IDC_VALUE_NAME);
	HWND	hValue	=	GetDlgItem(hwnd,IDC_DEFAULT_VALUE);

	char	strName[32];
	BOOL	isSuccess;
	int		value;

	value	=	GetDlgItemText(hwnd,IDC_VALUE_NAME,strName,sizeof(strName));

	if(value==	0)
	{
		SetFocus(hName);
		ERRMSG(dMSG_ERROR_ADD_VALUE,dMSG_CAN_NOT_READ_VALUE_NAME);
		return;
	}

	value	=	GetDlgItemInt(hwnd,IDC_DEFAULT_VALUE,&isSuccess,TRUE);

	if (!isSuccess)
	{
		SetFocus(hName);
		ERRMSG(dMSG_ERROR_ADD_VALUE,dMSG_CAN_NOT_READ_VALUE);
		return;
	}

	if (s_iSelectTab	==	dVALUE_LOCAL)
	{
		SetFocus(hName);

		if (g_lpActmap->m_Value.Add(strName,value)	==	0xffff)	return;

		g_lpActmap->LostVirgin();	//	... 헉.. --;;
	}
	else
	{
		SetFocus(hName);

		if (g_ProjectValue.Add(strName,value)		==	0xffff)	return;
	}

	s_ValueList.AddItem(0,strName);
	s_ValueList.AddItem(1,_ms("%d",value));

	s_ValueList.Sort(0);

	s_ValueList.IncreaseLineCounter();

	SetDlgItemText(hwnd,IDC_VALUE_NAME,"");
	SetDlgItemInt(hwnd,IDC_DEFAULT_VALUE,0,TRUE);

	SetFocus(hName);

	DrawList(hwnd);

}	//	cSETVALUE::AddValue(HWND hwnd)

LRESULT CALLBACK
cSETVALUE::s_procListBox(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	switch(uMsg)
	{	case WM_INITDIALOG	:
			InitListBox(hwnd);
			break;

		case WM_CLOSE		:
			EndDialog(hwnd,IDCANCEL);
			break;

		case WM_NOTIFY		:
		{
			if (((NMHDR*)lParam)->code ==	TCN_SELCHANGE)
				cSETVALUE::ChangeTab(hwnd);
			break;
		}

		case WM_COMMAND :
			switch( LOWORD( wParam ) )
            {
				case	IDOK				:
				{
					AddValue(hwnd);
					break;
				}

				case	IDC_ADD				:
				{
					break;
				}

				case	IDC_REMOVE			:
				{
					RemoveValue(hwnd);
					break;
				}

				case	IDCANCEL			:
				{
					EndDialog(hwnd,IDOK);
					break;
				}
			}
			break;
	}

	return (0L);
}
