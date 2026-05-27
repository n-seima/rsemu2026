// questList.cpp : implementation file
//

#include "stdafx.h"
#include "admintool.h"
#include "questList.h"
#include "quest.h"
#include "CharacterInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cQuestList dialog


cQuestList::cQuestList(CWnd* pParent /*=NULL*/)
	: CDialog(cQuestList::IDD, pParent)
{
	//{{AFX_DATA_INIT(cQuestList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void cQuestList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cQuestList)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(cQuestList, CDialog)
	//{{AFX_MSG_MAP(cQuestList)
	ON_BN_CLICKED(IDC_CANCEL_QUEST, OnCancelQuest)
	ON_BN_CLICKED(IDC_MAIN_QUEST, OnMainQuest)
	ON_BN_CLICKED(IDC_ARBEIT, OnArbeit)
	ON_BN_CLICKED(IDC_CHANGE_VALUE, OnChangeValue)
	ON_BN_CLICKED(IDC_CANCEL_QUEST_COMPLETE_INFO, OnCancelQuestCompleteInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cQuestList message handlers

BOOL cQuestList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_listQuest.init(GetDlgItem(IDC_QUEST_LIST)->m_hWnd,0,LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
	::SendMessage(GetDlgItem(IDC_QUEST_LIST)->m_hWnd,WM_SETFONT,(WPARAM)GetStockObject( DEFAULT_GUI_FONT ), 0 );

	m_listQuest.addColumn(0,46,"Type");
	m_listQuest.addColumn(1,220,"Name");
	m_listQuest.addColumn(2,46,"Step");
	m_listQuest.addColumn(3,56,"Value");
	
	m_listCompleteQuest.init(GetDlgItem(IDC_COMPLETE_QUEST_LIST)->m_hWnd,0,LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
	::SendMessage(GetDlgItem(IDC_COMPLETE_QUEST_LIST)->m_hWnd,WM_SETFONT,(WPARAM)GetStockObject( DEFAULT_GUI_FONT ), 0 );

	m_listCompleteQuest.addColumn(0,388,"Name");

	m_listMainQuestValue.init(GetDlgItem(IDC_MAIN_QUEST_VALUE_LIST)->m_hWnd,0,LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
	::SendMessage(GetDlgItem(IDC_MAIN_QUEST_VALUE_LIST)->m_hWnd,WM_SETFONT,(WPARAM)GetStockObject( DEFAULT_GUI_FONT ), 0 );

	m_listMainQuestValue.addColumn(0,28,"ID");
	m_listMainQuestValue.addColumn(1,160,"Value");

	m_iQuestType		=	0;

	CheckDlgButton(IDC_MAIN_QUEST,TRUE);

	updateList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void
cQuestList::setPlayer(CAvatarData 	*_lpPlayer)
{
	m_lpPlayer	=	_lpPlayer;
}

void cQuestList::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void cQuestList::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void cQuestList::OnCancelQuest() 
{
	BOOL	bIsSuccess;
	int		iSelectedPack	=	m_listQuest.getSelectedItemLParam(&bIsSuccess);

	if (bIsSuccess	==	FALSE)
	{
		MessageBox("please select quest","");
		return;
	}

	CQuest	*lpQuestManager;

	if (m_aProcessQuest[iSelectedPack].questKind	==	eQT_NORMAL	)
		lpQuestManager	=	&g_quest;
	if (m_aProcessQuest[iSelectedPack].questKind	==	eQT_ARBEIT	)
		lpQuestManager	=	&g_questArbeit;

	int			iQuestIndex	=	m_aProcessQuest[iSelectedPack].questIndex;
	CQuestItem	*lpQuest	=	lpQuestManager->getQuest(iQuestIndex);

	if (cMSG::YESNO("Remove Quest","Do you really cancel this quest?\n\n%s",lpQuest->m_strName)	==	IDNO)
		return;

	m_aProcessQuest[iSelectedPack].isActive			=	FALSE;
	m_aProcessQuest[iSelectedPack].questKind		=	0;
	m_aProcessQuest[iSelectedPack].questProcessLevel=	0;
	m_aProcessQuest[iSelectedPack].questValue		=	0;
	m_aProcessQuest[iSelectedPack].questIndex		=	0;

	updateList();
}

void
cQuestList::updateList()
{		// 마지막 수정일 :09.10.22
	m_listQuest.reset();

	int		iCount	=	0,i;
	CQuest	*lpQuestManager;

	m_listMainQuestValue.reset();

	for	(i=0;i<10;i++)
	{
		m_listMainQuestValue.addItem(i,0,_ms("%d",i));
		m_listMainQuestValue.setItem(i,1,_ms("%d",m_awMainQuestValue[i]));
	}

	for (i=0;i<dMAX_PROCESS_QUEST_COUNT;i++)
	{
		if (m_aProcessQuest[i].isActive	==	FALSE		)	
			continue;

		if (m_aProcessQuest[i].questKind	==	eQT_NORMAL	)
			lpQuestManager	=	&g_quest;
		else
		if (m_aProcessQuest[i].questKind	==	eQT_ARBEIT	)
			lpQuestManager	=	&g_questArbeit;
		else
		{
			m_aProcessQuest[i].isActive		=	FALSE;
			continue;
		}

		int			iQuestIndex	=	m_aProcessQuest[i].questIndex;
		CQuestItem	*lpQuest	=	lpQuestManager->getQuest(iQuestIndex);

		if (m_aProcessQuest[i].questKind	==	eQT_NORMAL	)
			m_listQuest.addItem(iCount,0,"Major",i);
		else
			m_listQuest.addItem(iCount,0,"Arbeit",i);

		m_listQuest.setItem(iCount,1,lpQuest->m_strName);
		m_listQuest.setItem(iCount,2,_ms("%d",m_aProcessQuest[i].questProcessLevel));
		m_listQuest.setItem(iCount,3,_ms("%d",m_aProcessQuest[i].questValue));

		iCount++;
	}

	BYTE	*lpCompleteBuffer;

	if	(m_iQuestType	==	eQT_NORMAL	)
	{
		lpQuestManager	=	&g_quest;
		lpCompleteBuffer=	m_aCompleteQuest;
	}
	if	(m_iQuestType	==	eQT_ARBEIT	)
	{
		lpQuestManager	=	&g_questArbeit;
		lpCompleteBuffer=	m_aCompleteArbeit;
	}

	m_listCompleteQuest.reset();

	for (i=0;i<lpQuestManager->m_iCount;i++)
	{
		BOOL	bIsCompleteQuest	=	FALSE;

		if	(m_iQuestType	==	eQT_NORMAL)
		{
			int	iSlot	=	i/8;

			if	(iSlot	<	dQUEST_BUFFER_SIZE)
			{
				if	(m_aCompleteQuest[iSlot]&(1<<(i%8)))
					bIsCompleteQuest	=	TRUE;
			}
			else
			{
				iSlot	-=	dQUEST_BUFFER_SIZE;

				if	(m_aCompleteQuest2[iSlot]&(1<<(i%8)))
					bIsCompleteQuest	=	TRUE;
			}
		}
		else
		if	(m_iQuestType	==	eQT_ARBEIT)		// 아르바이트일때...
		{
			int	iSlot	=	i/8;

			if	(iSlot	>=	dARBEIT_BUFFER_SIZE)		// 버퍼사이즈가 넘어가면 .. 추가안함~!
			{
				iSlot	-=	dARBEIT_BUFFER_SIZE;

				if	(m_aCompleteArbeit2[iSlot]&(1<<(i%8)))
					bIsCompleteQuest	=	TRUE;
			}
			else
			{
				if	(m_aCompleteArbeit[iSlot]&(1<<(i%8)))
					bIsCompleteQuest	=	TRUE;
			}
		}

		if	(bIsCompleteQuest	==	0)
			continue;

		CQuestItem	*lpQuest	=	lpQuestManager->getQuest(i);

		m_listCompleteQuest.addItem(iCount,0,lpQuest->m_strName,i);
	}
}

void cQuestList::OnMainQuest() 
{
	m_iQuestType	=	eQT_NORMAL;

	updateList();
}

void cQuestList::OnArbeit() 
{
	m_iQuestType	=	eQT_ARBEIT;

	updateList();
}

void cQuestList::OnChangeValue() 
{
	BOOL	bResult;
	int	iIndex	=	m_listMainQuestValue.getSelectedItemLParam(&bResult);

	if	(!bResult)
		return;

	int	iValue	=	GetDlgItemInt(IDC_MAIN_QUEST_VALUE);

	if	(iValue	<	0	||	iValue	>	0xffff)
		return;

	m_awMainQuestValue[iIndex]	=	iValue;

	updateList();
}

void cQuestList::OnCancelQuestCompleteInfo() 
{
	BOOL	bResult;
	int		iSelected	=	m_listCompleteQuest.getSelectedItemLParam(&bResult);

	if	(!bResult)
		return;

	if	(m_iQuestType	==	eQT_NORMAL)
	{
		int	iSlot	=	iSelected/8;

		if	(iSlot	<	dQUEST_BUFFER_SIZE)
		{
			if	(m_aCompleteQuest[iSlot]&(1<<(iSelected%8)))
				m_aCompleteQuest[iSlot] -= (1<<(iSelected%8));
		}
		else
		{
			iSlot	-=	dQUEST_BUFFER_SIZE;

			if	(m_aCompleteQuest2[iSlot]&(1<<(iSelected%8)))
				m_aCompleteQuest2[iSlot]	-=	(1<<(iSelected%8));
		}
	}
	else
	{
		int	iSlot	=	iSelected/8;

		if	(iSlot	>=	dARBEIT_BUFFER_SIZE)
		{
			iSlot	-=	dARBEIT_BUFFER_SIZE;

			if	(m_aCompleteArbeit2[iSlot]	&	(1<<(iSelected%8)))
				m_aCompleteArbeit2[iSlot]	-=	(1<<(iSelected%8));
		}
		else
		{
			if	(m_aCompleteArbeit[iSlot]	&	(1<<(iSelected%8)))
				m_aCompleteArbeit[iSlot]	-=	(1<<(iSelected%8));
		}
	}

	updateList();
}