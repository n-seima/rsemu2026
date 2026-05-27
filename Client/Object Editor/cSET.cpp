#include "cSET.H"
#include "cWINDOW.H"
#include "cBMPWND.H"
#include "resource.H"
//DialogBox( _MAIN,hINST, "SET_DETAIL_ANM",_MAIN.hWND,(DLGPROC)procDETAILANM);

//	대화상자.. 콜백 함수들..

char		anmName[40];
int			anmCount,anmType,crashType,bHalf,bShadow,bOutline,curAnm;	//	에니 설정 템플릿..
cANM		tempAnm[dMAX_ANM];

cSPIN		cSET::maxAnm;
cSPIN		cSET::maxFrame;
cSPIN		cSET::minImgRow;
cSPIN		cSET::minImgCol;
cSPIN		cSET::minFrmCol;

char		*strAnmDirect[]	=	{"방향 없음","좌우 2 방향","상하 2 방향","4 방향","8 방향","16 방향"};

cLISTVIEW	cSET::ANMLIST;

void
cSET::SetAnm()
{	DialogBox( _MAIN.hINST, "SET_ANM",_MAIN.hWND,(DLGPROC)procSETANM);
}

BOOL
cSET::SetAnmData(HWND hDlg)
{	int		i,j,k;
	BOOL	temp;

	anmType		=	cCOMBOBOX::GetSelect(GetDlgItem(hDlg,IDC_ANI_KIND));
	crashType	=	cCOMBOBOX::GetSelect(GetDlgItem(hDlg,IDC_CRASH));
	anmCount	=	GetDlgItemInt	(hDlg,IDC_MAX_ANI,&temp,FALSE);

	bHalf	=	FALSE,bShadow	=	FALSE,bOutline	=	FALSE;
	if (IsDlgButtonChecked(hDlg,IDR_FLIP		)) bHalf	=	TRUE;
	if (IsDlgButtonChecked(hDlg,IDR_SAVESHADOW	)) bShadow	=	TRUE;
	if (IsDlgButtonChecked(hDlg,IDR_SAVEOUTLINE	)) bOutline	=	TRUE;

	GetDlgItemText(hDlg,IDE_ANINAME,anmName,sizeof(anmName));

	for (i=cANM::AnmCount;i<dMAX_ANM;i++)	ANM[i].ClearData(anmType);

	for (i=0;i<cANM::AnmCount;i++)
	{	ANM[i].Set(&tempAnm[i]);

		for (j=_directCount[ANM[i].anmtype];j<dMAX_DIRECT;j++)	for (k=0;k<dMAX_FRAME;k++)	ANM[i].Sprite[j][k]	=	0;
		for (j=0;j<dMAX_DIRECT;j++)	for (k=ANM[i].FrameCount;k<dMAX_FRAME;k++)	ANM[i].Sprite[j][k]	=	0;
	}

	strcpy(cANM::ANMName,anmName);
	cANM::CrashType		=	crashType;
	cANM::bHALF			=	bHalf;
	cANM::bSAVESHADOW	=	bShadow;
	cANM::bSAVEOUTLINE	=	bOutline;
	cANM::AnmCount		=	anmCount;
	cANM::AnmType		=	anmType;

	_MAIN.Update();

	return TRUE;
}

void
cSET::initSETANM(HWND hDlg)
{	int i;
	for (i=0;i<dMAX_ANM;i++)	tempAnm[i].Set(&ANM[i]);

	strcpy(anmName,cANM::ANMName);
	anmType		=	cANM::AnmType;
	crashType	=	cANM::CrashType;
	bHalf		=	cANM::bHALF;
	bShadow		=	cANM::bSAVESHADOW;
	bOutline	=	cANM::bSAVEOUTLINE;
	anmCount	=	cANM::AnmCount;

	//	전체 설정..
	for (i=0;i<6;i++)	cCOMBOBOX::Add(GetDlgItem(hDlg,IDC_ANI_KIND),_ms("%s 에니메이션",strAnmType[i]));
	for (i=0;i<5;i++)	cCOMBOBOX::Add(GetDlgItem(hDlg,IDC_CRASH),_ms("%s",strCrashType[i]));
	cCOMBOBOX::Select(GetDlgItem(hDlg,IDC_ANI_KIND),cANM::AnmType);
	cCOMBOBOX::Select(GetDlgItem(hDlg,IDC_CRASH),cANM::CrashType);

	SetDlgItemText	(hDlg,IDE_ANINAME,cANM::ANMName);

	CheckDlgButton	(hDlg,IDR_FLIP			,cANM::bHALF		);
	CheckDlgButton	(hDlg,IDR_SAVESHADOW	,cANM::bSAVESHADOW	);
	CheckDlgButton	(hDlg,IDR_SAVEOUTLINE	,cANM::bSAVEOUTLINE	);

	maxAnm.Init		(hDlg,_MAIN.hINST,IDM_MAX_ANM,GetDlgItem(hDlg,IDC_MAX_ANI),1,dMAX_ANM,cANM::AnmCount);

	ANMLIST.Init	(GetDlgItem(hDlg,IDL_ANM_LIST),_MAIN.hINST,LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT|LVS_EX_ONECLICKACTIVATE);
	ANMLIST.AddColumn(IDC_INDEX,40 ,"번호");
	ANMLIST.AddColumn(IDC_ANIMATION,140,"에니메이션 명칭");
	ANMLIST.AddColumn(IDC_DIRECT,80 ,"방향");
	ANMLIST.AddColumn(IDC_MAX_FRAME,66 ,"총 프레임");

	for (i=0;i<cANM::AnmCount;i++)
	{	ANMLIST.AddItem(i,IDC_INDEX,_ms("%3d",i));
		ANMLIST.SetItem(i,IDC_ANIMATION,ANM[i].Name);
		ANMLIST.SetItem(i,IDC_DIRECT,strAnmDirect[ANM[i].anmtype]);
		ANMLIST.SetItem(i,IDC_MAX_FRAME,_ms("%d",ANM[i].FrameCount));
	}
}

void
cSET::loadANMSET(HWND hDlg)
{	int i;

	cANM::LoadAniSet();
	_MAIN.Update();

	for (i=0;i<dMAX_ANM;i++)	tempAnm[i].Set(&ANM[i]);

	strcpy(anmName,cANM::ANMName);
	anmType		=	cANM::AnmType;
	crashType	=	cANM::CrashType;
	bHalf		=	cANM::bHALF;
	bShadow		=	cANM::bSAVESHADOW;
	bOutline	=	cANM::bSAVEOUTLINE;

	//	전체 설정..
	cCOMBOBOX::Select(GetDlgItem(hDlg,IDC_ANI_KIND),cANM::AnmType);
	cCOMBOBOX::Select(GetDlgItem(hDlg,IDC_CRASH),cANM::CrashType);

	SetDlgItemText	(hDlg,IDE_ANINAME,cANM::ANMName);

	CheckDlgButton	(hDlg,IDR_FLIP			,cANM::bHALF		);
	CheckDlgButton	(hDlg,IDR_SAVESHADOW	,cANM::bSAVESHADOW	);
	CheckDlgButton	(hDlg,IDR_SAVEOUTLINE	,cANM::bSAVEOUTLINE	);

	maxAnm.setValue	(cANM::AnmCount,FALSE);
	SetDlgItemInt 	(hDlg,IDC_MAX_ANI,cANM::AnmCount,FALSE);

	if (anmCount	>	cANM::AnmCount)	for (i=anmCount-1;i>=cANM::AnmCount;i--) ANMLIST.DelItem(i);

	for (i=0;i<cANM::AnmCount;i++)
	{	ANMLIST.SetText(i,IDC_ANIMATION,tempAnm[i].Name);
		ANMLIST.SetText(i,IDC_DIRECT,strAnmDirect[tempAnm[i].anmtype]);
		ANMLIST.SetText(i,IDC_MAX_FRAME,_ms("%d",tempAnm[i].FrameCount));
	}

	for (i=anmCount;i<cANM::AnmCount;i++)
	{	ANMLIST.AddItem(i,IDC_INDEX,_ms("%3d",i));
		ANMLIST.SetItem(i,IDC_ANIMATION,tempAnm[i].Name);
		ANMLIST.SetItem(i,IDC_DIRECT,strAnmDirect[tempAnm[i].anmtype]);
		ANMLIST.SetItem(i,IDC_MAX_FRAME,_ms("%d",tempAnm[i].FrameCount));
	}

	anmCount	=	cANM::AnmCount;
}

void
cSET::resetData()
{	int i;

	for (i=0;i<anmCount;i++)
	{	tempAnm[i].anmtype		=	anmType;
		tempAnm[i].directCount	=	_directCount[anmType];

		ANMLIST.SetText(i,IDC_DIRECT,strAnmDirect[tempAnm[i].anmtype]);
	}
}

void
cSET::resetCount(int ac)
{	int i;

	if (ac == anmCount ) return;
	if (ac < anmCount )	for (i=anmCount-1;i>=ac;i--) ANMLIST.DelItem(i);

	for (i=ac;i<dMAX_ANM;i++)
	{	tempAnm[i].anmtype		=	anmType;
		tempAnm[i].directCount	=	_directCount[anmType];
	}

	for (i=anmCount;i<ac;i++)
	{	tempAnm[i].anmtype		=	anmType;
		tempAnm[i].directCount	=	_directCount[anmType];

		ANMLIST.AddItem(i,IDC_INDEX,_ms("%3d",i));
		ANMLIST.SetItem(i,IDC_ANIMATION,tempAnm[i].Name);
		ANMLIST.SetItem(i,IDC_DIRECT,strAnmDirect[tempAnm[i].anmtype]);
		ANMLIST.SetItem(i,IDC_MAX_FRAME,_ms("%d",tempAnm[i].FrameCount));
	}
	anmCount	=	ac;
}

LRESULT CALLBACK 
cSET::procSETANM(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{	switch(uMsg)
	{	case WM_INITDIALOG	:
			cSET::initSETANM(hWnd);
			break;

		case WM_NOTIFY		:
			if (wParam == IDL_ANM_LIST)
			{	NM_LISTVIEW *pNM = (NM_LISTVIEW *)lParam;
				if (pNM->hdr.code == LVN_ITEMACTIVATE)
				{	curAnm=pNM->iItem;
					SetDetailAnm(hWnd);
				}
				if (pNM->hdr.code == LVN_COLUMNCLICK)	if (pNM->iSubItem == IDC_DIRECT)	resetData();
			}
			break;

		case WM_VSCROLL :
			cSPIN::setValue((HWND)lParam,HIWORD( wParam ));
			break;

		case WM_COMMAND :
			if ( HIWORD( wParam ) == EN_CHANGE )
				if (LOWORD(wParam) == IDC_MAX_ANI)
				{	int value;
					BOOL temp;
					value=GetDlgItemInt(hWnd,LOWORD( wParam ),&temp,FALSE);
					value=max(1,value);
					value=min(dMAX_ANM,value);
					maxAnm.setValue(value,FALSE);
					break;
				}

			switch(LOWORD(wParam))
			{	case	IDOK			:
					if (SetAnmData(hWnd))	EndDialog(hWnd,IDOK);
					break;

				case	IDCANCEL		:
					EndDialog(hWnd,IDCANCEL);
					break;

				case	IDC_ANI_KIND	:
				{	if (HIWORD(wParam)==CBN_SELCHANGE)
					{	int value;
						value	=	cCOMBOBOX::GetSelect(GetDlgItem(hWnd,IDC_ANI_KIND));
						if (value == dANI_CUSTOM )	
							if (value != anmType)
							{	anmType	=	value;
								resetData();
							}
						anmType	=	value;
						break;
					}
					break;
				}

				case	IDB_APPLY		:
				{	BOOL	temp;
					int		value;
					value	=	GetDlgItemInt	(hWnd,IDC_MAX_ANI,&temp,FALSE);
					resetCount(value);
					break;
				}

				case	IDB_LOAD		:
					loadANMSET(hWnd);
					break;

				case	IDB_SAVE		:
					if (cMSG::YESNO("에니메이션 셋팅 데이터 저장","저장 하기 전에 지금의 상태를 적용해야 합니다. 계속 하시겠습니까?")==IDNO) break;
					if (SetAnmData(hWnd))	cANM::SaveAniSet();
					break;

				default	:
					break;
			}
			break;
			
		default :	return (FALSE);
	}
	return (TRUE);
}

void
cSET::SetDetailAnm(HWND hwnd)
{	DialogBox( _MAIN.hINST, "SET_DETAIL_ANM",hwnd,(DLGPROC)procDETAILANM);
}

void
cSET::initDETAILANM(HWND hDlg)
{	int i;

	SetDlgItemText	(hDlg,IDE_ANINAME,tempAnm[curAnm].Name);
	maxFrame.Init(hDlg,_MAIN.hINST,IDM_MAX_FRAME,GetDlgItem(hDlg,IDE_MAX_FRAME),1,dMAX_FRAME,tempAnm[curAnm].FrameCount);
	CheckDlgButton	(hDlg,IDR_SINGLE+tempAnm[curAnm].anmtype,TRUE);
	if (anmType==dANI_CUSTOM)
		for (i=0;i<6;i++) EnableWindow(GetDlgItem(hDlg,IDR_SINGLE+i),FALSE);
}

void
cSET::SetDetailAnmData(HWND hDlg)
{	int i;
	BOOL temp;

	if (anmType!= dANI_CUSTOM)
		for (i=IDR_SINGLE;i<=IDR_16WAY;i++)	
			if (IsDlgButtonChecked(hDlg,i)) tempAnm[curAnm].anmtype	=	i-IDR_SINGLE;

	GetDlgItemText	(hDlg,IDE_ANINAME,tempAnm[curAnm].Name,sizeof(tempAnm[curAnm].Name));
	tempAnm[curAnm].FrameCount	=	GetDlgItemInt	(hDlg,IDE_MAX_FRAME,&temp,FALSE);
	tempAnm[curAnm].directCount	=	_directCount[tempAnm[curAnm].anmtype];

	ANMLIST.SetText(curAnm,1,tempAnm[curAnm].Name);
	ANMLIST.SetItem(curAnm,2,strAnmDirect[tempAnm[curAnm].anmtype]);
	ANMLIST.SetItem(curAnm,3,_ms("%d",tempAnm[curAnm].FrameCount));
}

LRESULT CALLBACK 
cSET::procDETAILANM(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{	switch(uMsg)
	{	case WM_INITDIALOG	:
			cSET::initDETAILANM(hWnd);
			break;

		case WM_VSCROLL :
			cSPIN::setValue((HWND)lParam,HIWORD( wParam ));
			break;

		case WM_COMMAND :
			if ( HIWORD( wParam ) == EN_CHANGE )
				if (LOWORD(wParam) == IDE_MAX_FRAME)
				{	int value;
					BOOL temp;
					value=GetDlgItemInt(hWnd,LOWORD( wParam ),&temp,FALSE);
					value=max(0,value);
					value=min(dMAX_FRAME,value);
					maxFrame.setValue(value,FALSE);
					break;
				}
	
			switch(wParam )
			{	case	IDOK		:
					SetDetailAnmData(hWnd);
					EndDialog(hWnd,IDOK);
					break;

				case	IDCANCEL	:
					EndDialog(hWnd,IDCANCEL);
					break;

				default	:
					break;
			}
			break;
			
		
		default :	return (FALSE);
	}
	return (TRUE);
}

//	툴 셋팅..

DWORD	colImg,colWork,colFrm,colOut,colCross;

void
cSET::SetTool()
{	DialogBox( _MAIN.hINST, "SET_TOOL",_MAIN.hWND,(DLGPROC)procSETTOOL);
}

void
cSET::initSETTOOL(HWND hDlg)
{	BYTE r,g,b;
	r		=	(_cfg.imgColor >>	10)	&31	,g	=	(_cfg.imgColor >>	5 )	&31	,b	=	_cfg.imgColor	&31;
	r		=	r*255/31					,g	=	g*255/31					,b	=	b*255/31;
	colImg	=	RGB(r,g,b);
	r		=	(_cfg.frmColor >>	10)	&31	,g	=	(_cfg.frmColor >>	5 )	&31	,b	=	 _cfg.frmColor	&31;
	r		=	r*255/31					,g	=	g*255/31					,b	=	b*255/31;
	colFrm	=	RGB(r,g,b);
	r		=	(_cfg.workColor >>	10)	&31	,g	=	(_cfg.workColor >>	5 )	&31	,b	=	 _cfg.workColor&31;
	r		=	r*255/31					,g	=	g*255/31					,b	=	b*255/31;
	colWork	=	RGB(r,g,b);
	r		=	(_cfg.outColor >>	10)	&31	,g	=	(_cfg.outColor >>	5 )	&31	,b	=	 _cfg.outColor &31;
	r		=	r*255/31					,g	=	g*255/31					,b	=	b*255/31;
	colOut	=	RGB(r,g,b);
	r		=	(_cfg.crossColor >>	10)	&31	,g	=	(_cfg.crossColor >>	5 )	&31	,b	=	 _cfg.crossColor &31;
	r		=	r*255/31					,g	=	g*255/31					,b	=	b*255/31;
	colCross=	RGB(r,g,b);

	CheckDlgButton	(hDlg,IDC_SHADOW	,_cfg.bDRAWSHADOW	);
	CheckDlgButton	(hDlg,IDC_OUTLINE	,_cfg.bDRAWOUTLINE	);
	CheckDlgButton	(hDlg,IDC_WALLPAPER	,_cfg.bDRAWBACK	);
	CheckDlgButton	(hDlg,IDC_PRE_IMAGE	,_cfg.bDRAWPREFRAME);
	CheckDlgButton	(hDlg,IDC_DRAW_CRASH,_cfg.bDRAWCRASHBOX);
	CheckDlgButton	(hDlg,IDC_SPRITE_INFO,_cfg.bDRAWSPRITEINFO);
	CheckDlgButton	(hDlg,IDC_OPTION_BAR,_cfg.bOPTIONBAR);
	CheckDlgButton	(hDlg,IDC_EFFECT_BAR,_cfg.bEFFECTBAR);
	CheckDlgButton	(hDlg,IDC_PUT_INDEX	,_cfg.bPUTINDEX);

	minImgCol.Init	(hDlg,_MAIN.hINST,IDM_MINIMGCOL,GetDlgItem(hDlg,IDE_MINIMGCOL),1,20,_cfg.imgMinCol);
	minImgRow.Init	(hDlg,_MAIN.hINST,IDM_MINIMGROW,GetDlgItem(hDlg,IDE_MINIMGROW),1,20,_cfg.imgMinRow);
	minFrmCol.Init	(hDlg,_MAIN.hINST,IDM_MINFRMCOL,GetDlgItem(hDlg,IDE_MINFRMCOL),1,20,_cfg.frmMinCol);
	CheckDlgButton	(hDlg,IDR_FRONT+_cfg.putCROSSBAR	,TRUE);

}

void
cSET::SetTool(HWND hDlg)
{	BOOL temp;

	_cfg.bDRAWSHADOW	=	IsDlgButtonChecked	(hDlg,IDC_SHADOW	);
	_cfg.bDRAWOUTLINE	=	IsDlgButtonChecked	(hDlg,IDC_OUTLINE	);
	_cfg.bDRAWBACK		=	IsDlgButtonChecked	(hDlg,IDC_WALLPAPER	);
	_cfg.bDRAWPREFRAME	=	IsDlgButtonChecked	(hDlg,IDC_PRE_IMAGE	);
	_cfg.bDRAWCRASHBOX	=	IsDlgButtonChecked	(hDlg,IDC_DRAW_CRASH);
	_cfg.bDRAWSPRITEINFO=	IsDlgButtonChecked	(hDlg,IDC_SPRITE_INFO);
	_cfg.bPUTINDEX		=	IsDlgButtonChecked	(hDlg,IDC_PUT_INDEX	);
	_cfg.bOPTIONBAR		=	IsDlgButtonChecked	(hDlg,IDC_OPTION_BAR);
	_cfg.bEFFECTBAR		=	IsDlgButtonChecked	(hDlg,IDC_EFFECT_BAR);

	if (IsDlgButtonChecked	(hDlg,IDR_FRONT))	_cfg.putCROSSBAR	=	dPUT_FRONT;
	if (IsDlgButtonChecked	(hDlg,IDR_BACK ))	_cfg.putCROSSBAR	=	dPUT_BACK ;
	if (IsDlgButtonChecked	(hDlg,IDR_HALF ))	_cfg.putCROSSBAR	=	dPUT_HALF ;
	if (IsDlgButtonChecked	(hDlg,IDR_VALID))	_cfg.putCROSSBAR	=	dPUT_VALID;

	_cfg.imgMinCol		=	GetDlgItemInt	(hDlg,IDE_MINIMGCOL,&temp,FALSE);
	_cfg.imgMinRow		=	GetDlgItemInt	(hDlg,IDE_MINIMGROW,&temp,FALSE);
	_cfg.frmMinCol		=	GetDlgItemInt	(hDlg,IDE_MINFRMCOL,&temp,FALSE);

	_cfg.imgMinCol		=	max(1,_cfg.imgMinCol);
	_cfg.imgMinCol		=	min(40,_cfg.imgMinCol);
	_cfg.imgMinRow		=	max(1,_cfg.imgMinRow);
	_cfg.imgMinRow		=	min(40,_cfg.imgMinRow);
	_cfg.frmMinCol		=	max(1,_cfg.frmMinCol);
	_cfg.frmMinCol		=	min(40,_cfg.frmMinCol);


	DWORD r,g,b;

	r	=	(colImg >>	16)	&255,g	=	(colImg >> 8  )	&255,b	=	colImg	&	255;
	r	=	r*31/255			,g	=	g*31/255			,b	=	b*31/255;
	_cfg.imgColor	=	(WORD)((b<<10) | (g<<5) | r);

	r	=	(colFrm >>	16)	&255,g	=	(colFrm >> 8  )	&255,b	=	colFrm	&	255;
	r	=	r*31/255			,g	=	g*31/255			,b	=	b*31/255;
	_cfg.frmColor	=	(WORD)((b<<10) | (g<<5) | r);

	r	=	(colWork >>	16)	&255,g	=	(colWork>> 8  )&255	,b	=	colWork	&	255;
	r	=	r*31/255			,g	=	g*31/255			,b	=	b*31/255;
	_cfg.workColor	=	(WORD)((b<<10) | (g<<5) | r);

	r	=	(colOut >>	16)	&255,g	=	(colOut >> 8  )	&255,b	=	colOut	&	255;
	r	=	r*31/255			,g	=	g*31/255			,b	=	b*31/255;
	_cfg.outColor	=	(WORD)((b<<10) | (g<<5) | r);

	r	=	(colCross >>16)	&255,g	=	(colCross >> 8 )&255,b	=	colCross&	255;
	r	=	r*31/255			,g	=	g*31/255			,b	=	b*31/255;
	_cfg.crossColor	=	(WORD)((b<<10) | (g<<5) | r);

	_MAIN.Draw();
}

void
cSET::drawColorBox(int index,LPARAM lParam)
{	DWORD color;

	switch(index)
	{	case	IDC_WORKCOLOR	:	color	=	colWork	;break;
		case	IDC_IMGCOLOR	:	color	=	colImg	;break;
		case	IDC_FRMCOLOR	:	color	=	colFrm	;break;
		case	IDC_OUTCOLOR	:	color	=	colOut	;break;
		case	IDC_CROSSCOLOR	:	color	=	colCross;break;
	}

	DRAWITEMSTRUCT	*item	=	(DRAWITEMSTRUCT *)lParam;
	RECT			rect;
	HWND			hBUTTON	=	item->hwndItem;
	HDC				hDC		=	item->hDC;
	HBRUSH			hCOLOR	=	CreateSolidBrush(color);

	GetClientRect(hBUTTON,&rect);

	FillRect(hDC,&rect,hCOLOR);

	DeleteObject(hCOLOR);
}

void
cSET::chooseColor(int index,HWND hwnd)
{	CHOOSECOLOR	cc;


	memset( &cc, 0, sizeof( CHOOSECOLOR ) );
	cc.lStructSize  = sizeof( CHOOSECOLOR );
	cc.hwndOwner    = hwnd;
	cc.lpCustColors	= _cfg.dwCustColors;
	switch(index)
	{	case	IDC_WORKCOLOR	:	cc.rgbResult	=	colWork	;break;
		case	IDC_IMGCOLOR	:	cc.rgbResult	=	colImg	;break;
		case	IDC_FRMCOLOR	:	cc.rgbResult	=	colFrm	;break;
		case	IDC_OUTCOLOR	:	cc.rgbResult	=	colOut	;break;
		case	IDC_CROSSCOLOR	:	cc.rgbResult	=	colCross;break;
	}
	cc.Flags        = CC_RGBINIT;

	if ( ChooseColor( &cc ) )
	{	switch(index)
		{	case	IDC_WORKCOLOR	:	colWork	=	cc.rgbResult;break;
			case	IDC_IMGCOLOR	:	colImg	=	cc.rgbResult;break;
			case	IDC_FRMCOLOR	:	colFrm	=	cc.rgbResult;break;
			case	IDC_OUTCOLOR	:	colOut	=	cc.rgbResult;break;
			case	IDC_CROSSCOLOR	:	colCross=	cc.rgbResult;break;
		}
		InvalidateRect(hwnd,NULL,TRUE);
	}
}

LRESULT CALLBACK	
cSET::procSETTOOL(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{	switch(uMsg)
	{	case WM_INITDIALOG	:
			cSET::initSETTOOL(hWnd);
			break;

		case WM_DRAWITEM	:
			switch(LOWORD(wParam) )
			{	case	IDC_WORKCOLOR	:
				case	IDC_IMGCOLOR	:
				case	IDC_FRMCOLOR	:
				case	IDC_OUTCOLOR	:
				case	IDC_CROSSCOLOR	:
					drawColorBox(LOWORD(wParam),lParam);
					break;
			}
			break;

		case WM_VSCROLL :
			cSPIN::setValue((HWND)lParam,HIWORD( wParam ));
			break;

		case WM_COMMAND :
			if ( HIWORD( wParam ) == EN_CHANGE )
				switch(LOWORD(wParam))
				{	case	IDE_MINIMGCOL	:
					case	IDE_MINIMGROW	:
					case	IDE_MINFRMCOL	:
					{	int value;
						BOOL temp;
						value=GetDlgItemInt(hWnd,LOWORD( wParam ),&temp,FALSE);
						value=max(1,value);
						value=min(40,value);
						maxAnm.setValue(value,FALSE);
						break;
					}
				}

			switch(LOWORD(wParam))
			{	case	IDOK			:
					SetTool(hWnd);
					EndDialog(hWnd,IDOK);
					break;

				case	IDCANCEL		:
					EndDialog(hWnd,IDCANCEL);
					break;

				case	IDC_WORKCOLOR	:
				case	IDC_IMGCOLOR	:
				case	IDC_FRMCOLOR	:
				case	IDC_OUTCOLOR	:
				case	IDC_CROSSCOLOR	:
					chooseColor(LOWORD(wParam),hWnd);
					break;

				default	:
					break;
			}
			break;
			
		
		default :	return (FALSE);
	}
	return (TRUE);
}


/////////////////////////////////////////////////////////////////////
//	Program
/////////////////////////////////////////////////////////////////////


//	툴 셋팅..

int			cSET::_iImageType			=	0;
int			cSET::_iShadowType			=	1;
int			cSET::_iFileType			=	0;
BOOL		cSET::_isIncludeSubfolder	=	0;
char		cSET::_strSaveFolder[512];
char		cSET::_strImageFolder[512];

void
cSET::Program()
{	DialogBox( _MAIN.hINST, "PROGRAM",_MAIN.hWND,(DLGPROC)procPROGRAM);
}


void
cSET::initPROGRAM(HWND hDlg)
{
	CheckRadioButton(hDlg,IDC_IMAGE_PCX,IDC_IMAGE_TGA,IDC_IMAGE_PCX+_iImageType);
	CheckRadioButton(hDlg,IDC_SHADOW_PCX,IDC_SHADOW_TGA,IDC_SHADOW_PCX+_iShadowType);

	CheckDlgButton	(hDlg,IDC_INCLUDE_SUB	,_isIncludeSubfolder);
	SetDlgItemText	(hDlg,IDC_SAVE_FOLDER	,_strSaveFolder);

	SetDlgItemText	(hDlg,IDC_SOURCE_FOLDER	,_strImageFolder);
}

void
cSET::launchPROGRAM(HWND hDlg)
{
	int			iImageType,iShadowType;
	BOOL		isIncludeSubfolder;
	char		strSaveFolder[512],strSourceFolder[512];

	if (IsDlgButtonChecked(hDlg,IDC_IMAGE_PCX))	iImageType	=	0;
	if (IsDlgButtonChecked(hDlg,IDC_IMAGE_BMP))	iImageType	=	1;
	if (IsDlgButtonChecked(hDlg,IDC_IMAGE_TGA))	iImageType	=	2;
	if (IsDlgButtonChecked(hDlg,IDC_SHADOW_PCX))iShadowType	=	0;
	if (IsDlgButtonChecked(hDlg,IDC_SHADOW_BMP))iShadowType	=	1;
	if (IsDlgButtonChecked(hDlg,IDC_SHADOW_TGA))iShadowType	=	2;

	GetDlgItemText(hDlg,IDC_SAVE_FOLDER,strSaveFolder,sizeof(strSaveFolder));
	GetDlgItemText(hDlg,IDC_SOURCE_FOLDER,strSourceFolder,sizeof(strSourceFolder));

	isIncludeSubfolder	=	IsDlgButtonChecked(hDlg,IDC_INCLUDE_SUB	);

	if (!IsFolder(strSaveFolder))
	{
		cMSG::Error("프로그램 에러!!","세이브 폴더를 찾을 수 없습니다.");
		return;
	}

	if (!IsFolder(strSourceFolder))
	{
		cMSG::Error("프로그램 에러!!","이미지 폴더를 찾을 수 없습니다.");
		return;
	}

	_iImageType			=	iImageType;
	_iShadowType		=	iShadowType;
	_isIncludeSubfolder	=	isIncludeSubfolder;

	strcpy(_strSaveFolder,strSaveFolder);
	strcpy(_strImageFolder,strSourceFolder);

	SetCurrentDirectory(_strSaveFolder);
	
	cSTRINGS	*lpRso	=	cFOLDER::GetFiles("*.rso");
	cSTRINGS	*lpRfo	=	cFOLDER::GetFiles("*.rfo");

	if (lpRso)
	{
		cSTRING		*lpFile	=	lpRso->Pick(0);

		for (int i=0;i<lpRso->Count;i++)
		{
			DeleteFile(lpFile->String);
			lpFile	=	lpFile->pNEXT;
		}
	}

	if (lpRfo)
	{
		cSTRING		*lpFile	=	lpRfo->Pick(0);

		for (int i=0;i<lpRfo->Count;i++)
		{
			DeleteFile(lpFile->String);
			lpFile	=	lpFile->pNEXT;
		}
	}

	KILL(lpRso);
	KILL(lpRfo);

	_BMPWND.Program(_iImageType,_iShadowType,_isIncludeSubfolder,_strImageFolder,_strSaveFolder);
}


LRESULT CALLBACK
cSET::procPROGRAM(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{	switch(uMsg)
	{	case WM_INITDIALOG	:
			initPROGRAM(hWnd);
			break;

		case WM_COMMAND :
			switch(LOWORD(wParam))
			{	case	IDOK			:
					launchPROGRAM(hWnd);
					break;

				case	IDCANCEL		:
					EndDialog(hWnd,IDCANCEL);
					break;

				case	IDC_IMAGE_PCX	:
				case	IDC_IMAGE_BMP	:
				case	IDC_IMAGE_TGA	:
					CheckRadioButton(hWnd,IDC_IMAGE_PCX,IDC_IMAGE_TGA,LOWORD(wParam));
					break;
				case	IDC_SHADOW_PCX	:
				case	IDC_SHADOW_BMP	:
				case	IDC_SHADOW_TGA	:
					CheckRadioButton(hWnd,IDC_SHADOW_PCX,IDC_SHADOW_TGA,LOWORD(wParam));
					break;

				case	IDC_GET_FOLDER	:
				{	char	*folder	=	cFOLDER::Tree(hWnd,TRUE,"화일을 저장할 폴더를 선택해 주세요.",_strSaveFolder);

					if (folder)	SetDlgItemText(hWnd,IDC_SAVE_FOLDER,folder);
					break;
				}

				case	IDC_GET_SOURCE_FOLDER	:
				{	char	*folder	=	cFOLDER::Tree(hWnd,TRUE,"이미지 화일이 있는 폴더를 선택해 주세요.",_strImageFolder);
					if (folder)	SetDlgItemText(hWnd,IDC_SOURCE_FOLDER,folder);
					break;
				}

				default	:
					break;
			}
			break;
			
		
		default :	return (FALSE);
	}
	return (TRUE);
}