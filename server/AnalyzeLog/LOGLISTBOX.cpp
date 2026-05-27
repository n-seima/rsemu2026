// LOGLISTBOX.cpp : implementation file
//

#include "stdafx.h"
#include "AnalyzeLog.h"
#include "AnalyzeLogDlg.h"
#include "LOGLISTBOX.h"
#include "LinkedList.h"
#include "cITEM.h"
#include "CItemPrefix.h"
#include "SFC.h"
#include "CMessage.h"
#include "exportLog.h"
#include "duel_filter.h"
#include "log_filtering.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	ID_MENU_SERCH_DUEL_LOG_BY_ID		31212218	//	선택 시간대에 선택한 ID와 관련된 모든 결투 정보 
#define	ID_MENU_SERCH_DUEL_LOG_BY_SERIAL	31212219	//	선택 시간대에 선택한 Serial과 관련된 모든 결투 정보 
#define	ID_MENU_SERCH_DUEL_LOG_BY_IP		31212220	//	선택 시간대에 선택한 IP와 관련된 모든 결투 정보 

unsigned char * g_data;
int				g_size;
int				g_count;


////////////////////////////////////////////////////////////////////////////
// CLOGLISTBOX
void
__cdecl CLIPOUT(char *mainmsg,...)
{
	HANDLE		hMem;
	LPVOID		lpData;
	va_list		argptr;
	char		strMsgBuffer[1024];

	va_start(argptr, mainmsg);
	vsprintf(strMsgBuffer,mainmsg,argptr);
	va_end	(argptr);

	OpenClipboard(NULL);
	EmptyClipboard();

	hMem	=	GlobalAlloc(GHND,1024);
	lpData	=	GlobalLock(hMem);

	strcpy((char *)lpData,_ms("%s\n",strMsgBuffer));
	GlobalUnlock(hMem);
	SetClipboardData(CF_TEXT,hMem);
	CloseClipboard();
}

void
exportItemInfoForClipboard(char *_lpstrText,CItemDefine *_lpItem)
{
	cITEM	*lpItem		=	(cITEM	*)_lpItem;

	if (_lpItem->m_wBaseItem == 0)
		sprintf(_lpstrText,"GD [%d]\n",_lpItem->m_dwSerial);
	else
	{
		if (lpItem->getIncorrectPrefixCount())
		{
			sprintf(_lpstrText,"%s IT [%d:%u] [%d,%d] [%d,%d] <%d:%d,%d><%d:%d,%d><%d:%d,%d> %d       [%.2d/%.2d/%.2d %.2d:%.2d] incorrect!!!!!",
				((cITEM *)_lpItem)->getName(),_lpItem->m_wBaseItem,_lpItem->m_dwSerial,_lpItem->m_bCount,_lpItem->m_bDurability,
				_lpItem->m_aOption[0],_lpItem->m_aOption[1],
				_lpItem->m_aPrefix[0].m_wPrefix,_lpItem->m_aPrefix[0].m_aValue[0],_lpItem->m_aPrefix[0].m_aValue[1],
				_lpItem->m_aPrefix[1].m_wPrefix,_lpItem->m_aPrefix[1].m_aValue[0],_lpItem->m_aPrefix[1].m_aValue[1],
				_lpItem->m_aPrefix[2].m_wPrefix,_lpItem->m_aPrefix[2].m_aValue[0],_lpItem->m_aPrefix[2].m_aValue[1],
				_lpItem->getBitFieldValue(),
				_lpItem->m_year,_lpItem->m_month,_lpItem->m_day,_lpItem->m_hour,_lpItem->m_minute);
		}
		else
		{
			sprintf(_lpstrText,"%s IT [%d:%u] [%d,%d] [%d,%d] <%d:%d,%d><%d:%d,%d><%d:%d,%d> %d       [%.2d/%.2d/%.2d %.2d:%.2d]",
				((cITEM *)_lpItem)->getName(),_lpItem->m_wBaseItem,_lpItem->m_dwSerial,_lpItem->m_bCount,_lpItem->m_bDurability,
				_lpItem->m_aOption[0],_lpItem->m_aOption[1],
				_lpItem->m_aPrefix[0].m_wPrefix,_lpItem->m_aPrefix[0].m_aValue[0],_lpItem->m_aPrefix[0].m_aValue[1],
				_lpItem->m_aPrefix[1].m_wPrefix,_lpItem->m_aPrefix[1].m_aValue[0],_lpItem->m_aPrefix[1].m_aValue[1],
				_lpItem->m_aPrefix[2].m_wPrefix,_lpItem->m_aPrefix[2].m_aValue[0],_lpItem->m_aPrefix[2].m_aValue[1],
				_lpItem->getBitFieldValue(),
				_lpItem->m_year,_lpItem->m_month,_lpItem->m_day,_lpItem->m_hour,_lpItem->m_minute);
		}

	}
}

CLOGLISTBOX::CLOGLISTBOX()
{
}

CLOGLISTBOX::~CLOGLISTBOX()
{
}

BEGIN_MESSAGE_MAP(CLOGLISTBOX, CListBox)
	//{{AFX_MSG_MAP(CLOGLISTBOX)
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLOGLISTBOX message handlers

void CLOGLISTBOX::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMenu	menu;
	POINT	pos;
	int		iIndex		=	GetCurSel();
	CLinkedList * list	=	NULL;

	if	(g_lpMainDlg->m_dwCurListCount>0 && g_lpMainDlg->m_dwSearchListCount==0)
		list= g_lpMainDlg->allData;
	else
	if	(g_lpMainDlg->m_dwSearchListCount>0)
		list= g_lpMainDlg->searchData;

	ALL_LOGMSG * msg	=	NULL;

	menu.CreatePopupMenu();

	menu.AppendMenu(MF_STRING,(UINT)ID_COPY_CLIP,"Copy To Clipboard");
	menu.AppendMenu(MF_SEPARATOR,(UINT)0,"");
	menu.AppendMenu(MF_STRING,(UINT)ID_MENU_COPYID,"Copy ID");
	menu.AppendMenu(MF_STRING,(UINT)ID_MENU_COPYNAME,"Copy Name");
	menu.AppendMenu(MF_STRING,(UINT)ID_MENU_COPYSERIAL,"Copy Item Serial");
	menu.AppendMenu(MF_STRING,(UINT)ID_MENU_COPYIP,"Copy IP");
	menu.AppendMenu(MF_SEPARATOR,(UINT)0,"");
	menu.AppendMenu(MF_STRING,(UINT)ID_MENU_DELETE,"Delete Selected Items");

	if	(list)
	{
		msg	=	list->Get(iIndex);

		if	(msg)
		{
			if	(msg->base.wType	==	dLOG_SYSTEM)
			{
				LOG_SYSTEM	*lpLog	=	(LOG_SYSTEM	*)&msg->mLOG_SYSTEM;

				if	(lpLog->wWork	>=	eSLT_BEGIN_DUEL	&&	lpLog->wWork	<=	eSLT_DISCONNECT_IN_DUEL_FIELD)
				{
					menu.AppendMenu(MF_SEPARATOR,(UINT)0,"");
					menu.AppendMenu(MF_STRING,(UINT)ID_MENU_SERCH_DUEL_LOG_BY_ID,"Search All Duel Log By Player ID");
					menu.AppendMenu(MF_STRING,(UINT)ID_MENU_SERCH_DUEL_LOG_BY_SERIAL,"Search All Duel Log By Duel Serial");
					menu.AppendMenu(MF_STRING,(UINT)ID_MENU_SERCH_DUEL_LOG_BY_IP,"Search All Duel Log By IP");
				}
			}
		}
	}

	menu.AppendMenu(MF_SEPARATOR,(UINT)0,"");
	menu.AppendMenu(MF_STRING,(UINT)ID_MENU_TXT_PRINT,"Save to Text File");

	UINT	nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;

	::GetCursorPos(&pos);

	int		nRetValue	=	menu.TrackPopupMenu(nFlagsForMenu, pos.x, pos.y+16, this);

	nRetValue	=	nRetValue;

	switch(nRetValue)
	{
		case	ID_MENU_SERCH_DUEL_LOG_BY_ID		:	//	선택 시간대에 선택한 ID와 관련된 모든 결투 정보 
		{
			if	(!list)
				break;

			char	*lpstrId	=	msg->mLOG_SYSTEM.strId;
			CNode	*pNode		=	list->m_pFirstNode;

			g_duelFilter.resetDuelSerialList();

			while(pNode)
			{
				LOG_S_BEGIN_DUEL *lpLog	=	(LOG_S_BEGIN_DUEL *)&pNode->data;

				if	(stricmp(lpLog->strId,lpstrId)	==	0)
					g_duelFilter.addSearchDuelSerial(lpLog->iDuelSerial);

				pNode = pNode->pNext;
			}

			g_lpMainDlg->searchSettingDuelLog();
			break;
		}

		case	ID_MENU_SERCH_DUEL_LOG_BY_SERIAL	:	//	선택 시간대에 선택한 Serial과 관련된 모든 결투 정보 
		{
			if	(!msg)
				break;

			LOG_S_BEGIN_DUEL *lpLog	=	(LOG_S_BEGIN_DUEL *)msg;

			g_duelFilter.resetDuelSerialList();
			g_duelFilter.addSearchDuelSerial(lpLog->iDuelSerial);
			g_lpMainDlg->searchSettingDuelLog();
			break;
		}

		case	ID_MENU_SERCH_DUEL_LOG_BY_IP		:	//	선택 시간대에 선택한 IP와 관련된 모든 결투 정보 
		{
			if	(!list)
				break;

			char	*lpstrIP	=	msg->mLOG_SYSTEM.strIP;
			CNode	*pNode		= list->m_pFirstNode;

			g_duelFilter.resetDuelSerialList();
			while(pNode)
			{
				LOG_S_BEGIN_DUEL *lpLog	=	(LOG_S_BEGIN_DUEL *)&pNode->data;

				if	(stricmp(lpLog->strIP,lpstrIP)	==	0)
					g_duelFilter.addSearchDuelSerial(lpLog->iDuelSerial);

				pNode = pNode->pNext;
			}
			g_lpMainDlg->searchSettingDuelLog();
			break;
		}

		case ID_MENU_COPYID:
		{
			if	(msg == NULL)
				break;

			switch(msg->base.wType)
			{
				case	dLOG_ITEM_TRADE	:
					g_lpMainDlg->SetDlgItemText(IDC_ID,msg->mLOG_ITEM_TRADE.strSenderId);
					break;

				case	dLOG_SYSTEM		:
					g_lpMainDlg->SetDlgItemText(IDC_ID,msg->mLOG_SYSTEM.strId);
					break;

				case	dLOG_PITCHMAN_SHOP_TRADE	:
				case	dLOG_AVA_CREATE	:
				case	dLOG_AVA_DELETE	:
				case	dLOG_AVA_LOGIN	:
				case	dLOG_AVA_LOGOUT	:
				case	dLOG_ITEM_DROP	:
				case	dLOG_ITEM_PICK	:
				case	dLOG_ITEM_SELL	:
				case	dLOG_ITEM_DEL	:
				case	dLOG_ITEM_BANK_IN	:
				case	dLOG_ITEM_BANK_OUT	:
				case	dLOG_ITEM_WITHDRAW_PREMIUM_ITEM	:
				case	dLOG_ITEM_CREATE_PREMIUM_ITEM	:
				case	dLOG_CREATE_ITEM	:
				case	dLOG_USE_ITEM		:
				case	dLOG_EXPIRED_ITEM	:
				case	dLOG_DOUBLE_ITEM	:
				case	dLOG_ENCHANT_ITEM	:
					g_lpMainDlg->SetDlgItemText(IDC_ID,msg->mLOG_ITEM_DROP.strId);
					break;

				case	dLOG_AVA_SAVE_LOG:
				{
					LOG_AVA_SAVE_LOG	*lpPacket	=	(LOG_AVA_SAVE_LOG	*)msg;

					char	strUserId[dID_LENGTH],strUserName[dNAME_LENGTH];

					GetSaveLogIDAndName2(lpPacket->strIdAndName,strUserId,strUserName);
					g_lpMainDlg->SetDlgItemText(IDC_ID,strUserId);
					break;
				}
				default	:
				{
					cMSG::Put("error!","invalid id info log");
					break;
				}
			}
			break;
		}

		case ID_MENU_COPYNAME:
		{
			if	(msg == NULL)
				break;

			switch(msg->base.wType)
			{
				case	dLOG_ITEM_TRADE	:
					g_lpMainDlg->SetDlgItemText(IDC_NAME,msg->mLOG_ITEM_TRADE.strSenderName);
					break;

				case	dLOG_SYSTEM		:
					g_lpMainDlg->SetDlgItemText(IDC_NAME,msg->mLOG_SYSTEM.strName);
					break;

				case	dLOG_PITCHMAN_SHOP_TRADE	:
				case	dLOG_AVA_CREATE	:
				case	dLOG_AVA_DELETE	:
				case	dLOG_AVA_LOGIN	:
				case	dLOG_AVA_LOGOUT	:
				case	dLOG_ITEM_DROP	:
				case	dLOG_ITEM_PICK	:
				case	dLOG_ITEM_SELL	:
				case	dLOG_ITEM_DEL	:
				case	dLOG_ITEM_BANK_IN	:
				case	dLOG_ITEM_BANK_OUT	:
				case	dLOG_ITEM_WITHDRAW_PREMIUM_ITEM	:
				case	dLOG_ITEM_CREATE_PREMIUM_ITEM	:
				case	dLOG_CREATE_ITEM	:
				case	dLOG_USE_ITEM		:
				case	dLOG_EXPIRED_ITEM	:
				case	dLOG_DOUBLE_ITEM	:
				case	dLOG_ENCHANT_ITEM	:
					g_lpMainDlg->SetDlgItemText(IDC_NAME,msg->mLOG_ITEM_DROP.strName);
					break;

				case	dLOG_AVA_SAVE_LOG:
				{
					LOG_AVA_SAVE_LOG	*lpPacket	=	(LOG_AVA_SAVE_LOG	*)msg;

					char	strUserId[dID_LENGTH],strUserName[dNAME_LENGTH];

					GetSaveLogIDAndName2(lpPacket->strIdAndName,strUserId,strUserName);
					g_lpMainDlg->SetDlgItemText(IDC_NAME,strUserName);
					break;
				}
				default	:
				{
					cMSG::Put("error!","invalid info log");
					break;
				}
			}
			break;
		}

		case ID_MENU_COPYSERIAL:
		{
			if	(msg == NULL)
				break;
			char	strValue[30];

			switch(msg->base.wType)
			{
				case	dLOG_ITEM_DROP	:
				case	dLOG_ITEM_PICK	:
				case	dLOG_ITEM_SELL	:
				case	dLOG_USE_ITEM		:
				case	dLOG_CREATE_ITEM	:
				case	dLOG_EXPIRED_ITEM	:
				case	dLOG_DOUBLE_ITEM	:
				case	dLOG_ENCHANT_ITEM	:
				case	dLOG_PITCHMAN_SHOP_TRADE	:
				case	dLOG_ITEM_DEL	:
				case	dLOG_ITEM_CREATE_PREMIUM_ITEM	:
					g_lpMainDlg->SetDlgItemText(IDC_ITEMSERIAL,itoa(msg->mLOG_ITEM_DROP.cItem.m_dwSerial,strValue,10));
					break;

				default	:
				{
					cMSG::Put("error!","invalid info log");
					break;
				}
			}
			break;
		}

		case ID_MENU_COPYIP:
		{
			if	(msg	== NULL)
				break;

			switch(msg->base.wType)
			{
				case	dLOG_ITEM_TRADE	:
					g_lpMainDlg->SetDlgItemText(IDC_IP,msg->mLOG_ITEM_TRADE.strSenderIP);
					break;

				case	dLOG_SYSTEM		:
					g_lpMainDlg->SetDlgItemText(IDC_IP,msg->mLOG_SYSTEM.strIP);
					break;

				case	dLOG_PITCHMAN_SHOP_TRADE	:
				case	dLOG_AVA_CREATE	:
				case	dLOG_AVA_DELETE	:
				case	dLOG_AVA_LOGIN	:
				case	dLOG_AVA_LOGOUT	:
				case	dLOG_ITEM_DROP	:
				case	dLOG_ITEM_PICK	:
				case	dLOG_ITEM_SELL	:
				case	dLOG_ITEM_DEL	:
				case	dLOG_ITEM_BANK_IN	:
				case	dLOG_ITEM_BANK_OUT	:
				case	dLOG_ITEM_WITHDRAW_PREMIUM_ITEM	:
				case	dLOG_ITEM_CREATE_PREMIUM_ITEM	:
				case	dLOG_CREATE_ITEM	:
				case	dLOG_USE_ITEM		:
				case	dLOG_EXPIRED_ITEM	:
				case	dLOG_DOUBLE_ITEM	:
				case	dLOG_ENCHANT_ITEM	:
					g_lpMainDlg->SetDlgItemText(IDC_IP,msg->mLOG_ITEM_DROP.strIP);
					break;

				default	:
				{
					cMSG::Put("error!","invalid info log");
					break;
				}
			}

			break;
		}

		case ID_COPY_CLIP:
		{
			int iSelectedCount = GetSelCount();
			if(iSelectedCount<=0)	break;

			int iIndex = GetCurSel();

			CLinkedList * list = NULL;

			if	(g_lpMainDlg->m_dwCurListCount>0 && g_lpMainDlg->m_dwSearchListCount==0)
				list= g_lpMainDlg->allData;
			else
			if	(g_lpMainDlg->m_dwSearchListCount>0)
				list= g_lpMainDlg->searchData;

			if	(list == NULL)
				break;

			ALL_LOGMSG * msg = list->Get(iIndex);

			if(iSelectedCount==1)
			{
				char	txt[1024];		
				CItemDefine		item;	
				item.reset();
				memcpy(&item,&msg->mLOG_ITEM_DROP.cItem,sizeof(msg->mLOG_ITEM_DROP.cItem));
				exportItemInfoForClipboard(txt,&item);
				CLIPOUT(txt);
			}else
			{
			}
			break;
		}

		case IDA_DELETE:
		case ID_MENU_DELETE:
		{
			int iSelectedCount = GetSelCount();
			if(iSelectedCount<=0)	break;

			int iIndex = GetCurSel();
			CLinkedList * list = NULL;
			if(g_lpMainDlg->m_dwCurListCount>0 && g_lpMainDlg->m_dwSearchListCount==0)	list= g_lpMainDlg->allData;
			else if(g_lpMainDlg->m_dwSearchListCount>0)						list= g_lpMainDlg->searchData;
			if(list == NULL)	break;
			ALL_LOGMSG * msg = list->Get(iIndex);

			if(iSelectedCount==1)
			{
				int iCurIndex = this->GetCurSel();
				DeleteString(iCurIndex);
				list->DeleteNode(iCurIndex);
				if	(g_lpMainDlg->m_dwCurListCount>0 && g_lpMainDlg->m_dwSearchListCount==0)
					--g_lpMainDlg->m_dwCurListCount;
				else
				if	(g_lpMainDlg->m_dwSearchListCount>0)
					--g_lpMainDlg->m_dwSearchListCount;
			}else
			{
				int	iCurIndex = 0;
				int nCount = GetSelCount();
				int* pList = new int[nCount];
				GetSelItems(nCount, pList);
				for(int i = 0; i<nCount  ; i++)
				{
					DeleteString(pList[0]);
					list->DeleteNode(pList[0]);

					if	(g_lpMainDlg->m_dwCurListCount>0 && g_lpMainDlg->m_dwSearchListCount==0)
						--g_lpMainDlg->m_dwCurListCount;
					else
					if	(g_lpMainDlg->m_dwSearchListCount>0)
						--g_lpMainDlg->m_dwSearchListCount;
				}
			}
			break;
		}

		case ID_MENU_TXT_PRINT:
		{
			if	(g_lpMainDlg->m_filter.m_iFilter	==	-1)
			{
				cMSG::Error("error!!","invalid log data");
				break;
			}
			BOOL	bIsWantExportComment	=	FALSE;

			bIsWantExportComment	=	TRUE;

			char	strfn[1024];

			sprintf(strfn,"%s\\log.txt",g_lpMainDlg->m_strExportLogFolder);

			CFileDialog dlgFileSave(FALSE,"txt",strfn, OFN_EXPLORER, "RedStone Log TEXT File(*.txt)|*.txt||",NULL);

			if	(dlgFileSave.DoModal()==IDOK)
				strcpy(strfn,dlgFileSave.GetPathName());
			else
				break;

			strcpy(g_lpMainDlg->m_strExportLogFolder,_exportFolderName(strfn));
			g_lpMainDlg->saveCofig();

			FILE *fp = fopen(strfn,"a+");

			if	(!fp)
			{
				MessageBox("ERROR TXT FILE SAVE","File Create FAIL!!");
				break;
			}

			//	필터 코드 저장
			fprintf(fp,g_lpMainDlg->getEditFilterCode());
			fprintf(fp,"\n");

			int count = GetCount();

			if	(count<= 0 )
				break;

			CLinkedList * lpLogList = NULL;

			if	(g_lpMainDlg->m_dwCurListCount>0 && g_lpMainDlg->m_dwSearchListCount==0)
				lpLogList	=	g_lpMainDlg->allData;
			else
			if	(g_lpMainDlg->m_dwSearchListCount>0)
				lpLogList	=	g_lpMainDlg->searchData;

			if	(lpLogList == NULL)
				break;

			char	strPrinterTitle[1024];

			time_t tTime;
			struct tm *tmTime;
			tTime = time(NULL);
			tmTime = localtime(&tTime);

			sprintf(strPrinterTitle,"red stone log file : %02d/%02d/%02d %02d:%02d\n\n",tmTime->tm_year%100,tmTime->tm_mon+1,tmTime->tm_mday,tmTime->tm_hour,tmTime->tm_min);
			
			char	strPrint[1024],strComment[1024];

			fprintf(fp,strPrinterTitle);

			for (int i=0;i<g_lpMainDlg->m_iLogNodeCountForSort;i++)
			{
				CNode	*lpLogNode	=	g_lpMainDlg->m_aLogNodeForSort[i].m_lpNode;

				strComment[0]		=	NULL;

				if	(!makeFullLogText(&lpLogNode->data,strPrint,strComment,fp))
				{
					lpLogNode		=	lpLogNode->pNext;
					continue;
				}

				fprintf(fp,strPrint);

				if	(strComment[0])
				{
					fprintf(fp,strComment);
					fprintf(fp,"\n");
				}

				fflush(fp);
			}

			fclose(fp);

			if	(cMSG::YESNO("Export complete!!","do you want open this log file?")	==	IDYES)
				ShellExecute(NULL,"open",strfn,NULL,NULL,SW_SHOWNORMAL);

			break;
		}
	}

	CListBox::OnRButtonUp(nFlags, point);
}

LRESULT CLOGLISTBOX::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CListBox::WindowProc(message, wParam, lParam);
}

void CLOGLISTBOX::OnRButtonDown(UINT nFlags, CPoint point) 
{
	BOOL	bOutSide	=	FALSE;

	int		iSelect		=	ItemFromPoint(point,bOutSide);
	int		iCurSel		=	GetCurSel();

	SetSel(iCurSel,FALSE);
	SetSel(iSelect);

	CListBox::OnRButtonDown(nFlags, point);
}
