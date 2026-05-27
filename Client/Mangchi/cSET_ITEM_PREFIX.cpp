#include "cMAIN.H"
#include "cMESSAGE.H"
#include "cSET.H"
#include "cDRAW.H"
#include "cITEM_PREFIX.H"
#include "cITEM_PREFIX_EFFECT.H"
#include "cTEXT.H"
#include "resource.h"

cKEYWORD_ITEM	l_aPrefixOutputEffect[256]	=	
{
"Primary physical damage", ePOE_PHYSICAL_DAMAGE_1TH,
"Physical Damage Secondary", ePOE_PHYSICAL_DAMAGE_2TH,
"Physical Damage 3rd", ePOE_PHYSICAL_DAMAGE_3TH,
"First fire damage", ePOE_FIRE_DAMAGE_1TH,
"Secondary fire damage", ePOE_FIRE_DAMAGE_2TH,
"Fire Damage 3rd", ePOE_FIRE_DAMAGE_3TH,
"Primary water damage", ePOE_WATER_DAMAGE_1TH,
"Secondary water damage", ePOE_WATER_DAMAGE_2TH,
"Water Damage 3rd", ePOE_WATER_DAMAGE_3TH,
"Primary wind damage", ePOE_WIND_DAMAGE_1TH,
"Secondary wind damage", ePOE_WIND_DAMAGE_2TH,
"Wind Damage 3rd", ePOE_WIND_DAMAGE_3TH,
"Primary Earth Damage", ePOE_EARTH_DAMAGE_1TH,
"Secondary Earth Damage", ePOE_EARTH_DAMAGE_2TH,
"Earth Damage 3rd", ePOE_EARTH_DAMAGE_3TH,
"Primary light damage", ePOE_LIGHT_DAMAGE_1TH,
"Secondary light damage", ePOE_LIGHT_DAMAGE_2TH,
"Light Damage 3rd", ePOE_LIGHT_DAMAGE_3TH,
"Primary darkness damage", ePOE_DARK_DAMAGE_1TH,
"Second Dark Damage", ePOE_DARK_DAMAGE_2TH,
"Dark Damage 3rd", ePOE_DARK_DAMAGE_3TH,
"Defense 1st", ePOE_DEFENSIVE_POWER_1TH,
"Second Defense", ePOE_DEFENSIVE_POWER_2TH,
"Defense 3rd", ePOE_DEFENSIVE_POWER_3TH,
"Basic ability level 1", ePOE_BASIC_POWER_1TH,
"Basic Ability Level 2", ePOE_BASIC_POWER_2TH,
"Basic ability level 3", ePOE_BASIC_POWER_3TH,
"1st health increase", ePOE_INCREASE_HP_1TH,
"Second health increase", ePOE_INCREASE_HP_2TH,
"Health Increase 3rd", ePOE_INCREASE_HP_3TH,
"CP Increase 1st", ePOE_INCREASE_CP_1TH,
"CP Increase 2nd", ePOE_INCREASE_CP_2TH,
"CP Increase 3rd", ePOE_INCREASE_CP_3TH,
"Elemental Resistance Primary", ePOE_ELEMENTS_RESISTANCE_1TH,
"Elemental Resistance Secondary", ePOE_ELEMENTS_RESISTANCE_2TH,
"Elemental Resistance 3rd", ePOE_ELEMENTS_RESISTANCE_3TH,
"Outlier Resistance 1st", ePOE_STRANGE_STATUS_RESISTANCE_1TH,
"Second Resistance", ePOE_STRANGE_STATUS_RESISTANCE_2TH,
"Outlier Resistance 3rd", ePOE_STRANGE_STATUS_RESISTANCE_3TH,
"Thorn 1st", ePOE_THORN_1TH,
"Thorns 2nd", ePOE_THORN_2TH,
"Thorn 3rd", ePOE_THORN_3TH,
	"end",				dCODE_END,
};

HWND				cSET_ITEM_PREFIX::s_hWnd;
int					cSET_ITEM_PREFIX::s_iSelectPrefix	=	0;
int					cSET_ITEM_PREFIX::s_iFilter			=	0xffff;
cSCROLL_BAR			cSET_ITEM_PREFIX::s_sbDetailInfo;
cITEM_PREFIX*		cSET_ITEM_PREFIX::s_lpSelectPrefix;
cITEM_PREFIX		cSET_ITEM_PREFIX::s_clipPrefix;
cITEM_PREFIX		cSET_ITEM_PREFIX::s_makePrefix;
cLISTCONTROL		cSET_ITEM_PREFIX::s_listPrefix;	//	접두사 리스트

cDIBWND				cSET_ITEM_PREFIX::s_dibPrefixDetailInfo;	//	접두사의 세부 정보

//
//	기본 아이템 목록 설정
BOOL
cSET_ITEM_PREFIX::Init(HWND hwnd)
{
	int	result	=	DialogBox( MAIN.hINST,"SET_ITEM_PREFIX_LIST",hwnd,(DLGPROC)s_procItemPrefixList);

//	cPROJECT::SaveItem();

	s_makePrefix.reset();

	return	result;
}	//	cSET_ITEM_PREFIX::Init(HWND hwnd)

//
//	기본 아이템 목록 설정
void
cSET_ITEM_PREFIX::InitItemPrefixListBox(HWND _hWnd)
{
	s_hWnd	=	_hWnd;
	//	아이템 리스트를 그리고
	HWND	hComboItemPrefixType	=	GetDlgItem(_hWnd,IDC_ITEM_PREFIX_TYPE);
	HWND	hPrefixListWnd			=	GetDlgItem(_hWnd,IDC_PREFIX_LIST);
	HWND	hItemPrefixExplainWnd	=	GetDlgItem(_hWnd,IDC_PREFIX_EXPLAIN);

//	접두사 타입
	{
		int		iIndex				=	0;

		cCOMBOBOX::Add(hComboItemPrefixType,dMSG_ALL ,0xffff);

		while(1)
		{
			if (g_aItemPrefixType[iIndex].m_wType	==	0xffff)	break;

			cCOMBOBOX::Add(hComboItemPrefixType,g_aItemPrefixType[iIndex].m_strName,g_aItemPrefixType[iIndex].m_wType);

			iIndex++;
		}

		cCOMBOBOX::SelectByData(hComboItemPrefixType,s_iFilter);
	}	//	접두사 타입

//	접두사 리스트
	{
		s_listPrefix.Init(hPrefixListWnd	,3,FALSE,s_procItemPrefixListEvent);		//	3개의 컬럼
		s_listPrefix.AddColumn("Identification code", 6 * 11);
		s_listPrefix.AddColumn("Primary prefix", 6 * 20);
		s_listPrefix.AddColumn("Prefix type", 6 * 32);
	}	//	접두사 리스트

	ResetPrefixListBox(_hWnd);

	s_dibPrefixDetailInfo.Init(hItemPrefixExplainWnd,(WNDPROC)s_procPrefixInfoWnd);

	Draw(_hWnd);
}

//
//	기본 아이템 목록 설정
void
cSET_ITEM_PREFIX::ResetPrefixListBox(HWND hwnd)
{
	s_listPrefix.ResetItems();

	int	iIndex,iCount;

	for (iIndex=0,iCount=0;iIndex<dMAX_ITEM_PREFIX_COUNT && iCount < g_itemPrefix.getCount();iIndex++)
	{
		cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(iIndex);

		if	(!lpPrefix)
			continue;

		iCount++;

		if	(s_iFilter	!=	0xffff	&&	lpPrefix->m_wType	!=	s_iFilter)
			continue;

		s_listPrefix.AddItem(0,_ms("%.5d[%d]",lpPrefix->m_wDiscernmentCode,lpPrefix->m_wSerial),lpPrefix->m_wSerial);
//		s_listPrefix.AddItem(0,_ms("%.5d",lpPrefix->m_wSerial),lpPrefix->m_wSerial);
		s_listPrefix.AddItem(1,lpPrefix->m_str1stPrefix,lpPrefix->m_wSerial);
		s_listPrefix.AddItem(2,g_aItemPrefixType[lpPrefix->m_wType].m_strName,lpPrefix->m_wSerial);
		s_listPrefix.IncreaseLineCounter();
	}

	s_listPrefix.Sort(0);

	if (!s_listPrefix.selectByData(0,s_iSelectPrefix))
	{
		if (!s_listPrefix.Select(0))	s_iSelectPrefix	=	0xffff;
		else							s_iSelectPrefix	=	s_listPrefix.GetData(0,0);
	}
}

//
//	접두사 리스트를 그려준다.
void
cSET_ITEM_PREFIX::DrawPrefixList(HWND _hWwnd)
{
	s_listPrefix.Draw();
}

//
//	접두사 정보 그려주기
void
cSET_ITEM_PREFIX::DrawPrefixInfo(HWND _hWwnd)
{
	s_dibPrefixDetailInfo.Active();

	cDRAW::Fill(0);

	cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(s_iSelectPrefix);

	if (!lpPrefix)
	{
		s_dibPrefixDetailInfo.Draw();
		return;
	}

	int iX=5,iY=5;

	cTEXT::PushShadow(FALSE);

	cTEXT::Put(iX,iY,WHITE,_ms(" Prefix: %s/%s",lpPrefix->m_str1stPrefix,lpPrefix->m_str2stPrefix));

	if (lpPrefix->m_aValue[0][0])
	{
		if (lpPrefix->m_aValue[0][0]==lpPrefix->m_aValue[0][1])
				cTEXT::Put(iX,iY+=16,WHITE,_ms("Number1 : %d",lpPrefix->m_aValue[0][0]));
		else	cTEXT::Put(iX,iY+=16,WHITE,_ms("Number 1 : %d~%d",lpPrefix->m_aValue[0][0],lpPrefix->m_aValue[0][1]));
	}

	if (lpPrefix->m_aValue[1][1])
	{
		if (lpPrefix->m_aValue[1][0]==lpPrefix->m_aValue[1][1])
				cTEXT::Put(iX,iY+=16,WHITE,_ms("Number2 : %d",lpPrefix->m_aValue[1][0]));
		else	cTEXT::Put(iX,iY+=16,WHITE,_ms(" Number 2 : %d~%d",lpPrefix->m_aValue[1][0],lpPrefix->m_aValue[1][1]));
	}
	if (lpPrefix->m_sValue3)		cTEXT::Put(iX,iY+=16,WHITE,_ms("Number3 : %d",lpPrefix->m_sValue3));

	if (lpPrefix->m_wPrefixForm==0)	cTEXT::Put(iX,iY+=16,WHITE," Prefix form: noun");
	else							cTEXT::Put(iX,iY+=16,WHITE," Prefix form: adjective");
	cTEXT::Put(iX,iY+=16,WHITE,_ms(" Drop level: %d",lpPrefix->m_wDropLevel));

	char	*strEnable[]	=	{"×","○"};
	cTEXT::Put(iX,iY+=16,WHITE,_ms("Possible creation locations: 1st %s, 2nd %s, 3rd %s",
		strEnable[lpPrefix->m_aEnableSet[0]],strEnable[lpPrefix->m_aEnableSet[1]],strEnable[lpPrefix->m_aEnableSet[2]]));


	switch(lpPrefix->m_wComputePriceMethod)
	{
		case	0	:
			if (lpPrefix->m_wPriceFactor	==	0)
					cTEXT::Put(iX,iY+=16,WHITE,_ms(" Price : +%d",lpPrefix->m_dwPrice));
			else	cTEXT::Put(iX,iY+=16,WHITE,_ms(" Price : *%d",lpPrefix->m_dwPrice));
			break;

		case	1	:
			if (lpPrefix->m_wPriceFactor	==	0)
					cTEXT::Put(iX,iY+=16,WHITE,_ms(" Price : +[%d~%d]",lpPrefix->m_aValue[0][0],lpPrefix->m_aValue[0][1]) );
			else	cTEXT::Put(iX,iY+=16,WHITE,_ms(" Price : *[%d~%d]",lpPrefix->m_aValue[0][0],lpPrefix->m_aValue[0][1]) );
			break;

		case	2	:
			if (lpPrefix->m_wPriceFactor	==	0)
					cTEXT::Put(iX,iY+=16,WHITE,_ms(" Price calculation formula: +[%d~%d]*%d",lpPrefix->m_aValue[0][0],lpPrefix->m_aValue[0] [1],lpPrefix->m_dwPrice));
			else	cTEXT::Put(iX,iY+=16,WHITE,_ms(" Price calculation formula: *[%d~%d]*%d",lpPrefix->m_aValue[0][0],lpPrefix->m_aValue[0] [1],lpPrefix->m_dwPrice));
			break;

		case	3	:
			if (lpPrefix->m_wPriceFactor	==	0)
					cTEXT::Put(iX,iY+=16,WHITE,_ms("Price calculation formula: +[%d~%d]/%d",lpPrefix->m_aValue[0][0],lpPrefix->m_aValue[0] [1],lpPrefix->m_dwPrice));
			else	cTEXT::Put(iX,iY+=16,WHITE,_ms("Price calculation formula: *[%d~%d]/%d",lpPrefix->m_aValue[0][0],lpPrefix->m_aValue[0] [1],lpPrefix->m_dwPrice));
			break;

		case	4	:
			if (lpPrefix->m_wPriceFactor	==	0)
					cTEXT::Put(iX,iY+=16,WHITE,_ms(" Price calculation formula: +[%d~%d]*%d/100",lpPrefix->m_aValue[0][0],lpPrefix->m_aValue[ 0][1],lpPrefix->m_dwPrice)) ;
			else	cTEXT::Put(iX,iY+=16,WHITE,_ms(" Price calculation formula: *[%d~%d]*%d/100",lpPrefix->m_aValue[0][0],lpPrefix->m_aValue[ 0][1],lpPrefix->m_dwPrice)) ;
			break;

		case	5	:
			if (lpPrefix->m_wPriceFactor	==	0)
					cTEXT::Put(iX,iY+=16,WHITE,_ms(" Price calculation formula: +%d*[%d~%d]/[%d~%d]",lpPrefix->m_dwPrice,lpPrefix->m_aValue[ 1][0],lpPrefix->m_aValue[1 ][1],lpPrefix->m_dwPrice,lpPrefix->m_aValue[0][0],lpPrefix->m_aValue[0][1]));
			else	cTEXT::Put(iX,iY+=16,WHITE,_ms(" Price calculation formula: *%d*[%d~%d]/[%d~%d]",lpPrefix->m_dwPrice,lpPrefix->m_aValue[ 1][0],lpPrefix->m_aValue[1 ][1],lpPrefix->m_dwPrice,lpPrefix->m_aValue[0][0],lpPrefix->m_aValue[0][1]));
			break;

	}

	iY+=16;

	cITEM_EFFECT effect;

	effect.reset();
	effect.m_wEffect	=	lpPrefix->m_wType;
	effect.m_aValue[2]	=	lpPrefix->m_sValue3;

	char	strExplain[256];
	char	strValue1[256];
	char	strValue2[256];

	if (lpPrefix->m_aValue[0][0]==lpPrefix->m_aValue[0][1])
			strcpy(strValue1,_ms("%d",lpPrefix->m_aValue[0][0]));
	else	strcpy(strValue1,_ms("%d~%d",lpPrefix->m_aValue[0][0],lpPrefix->m_aValue[0][1]));

	if (lpPrefix->m_aValue[1][0]==lpPrefix->m_aValue[1][1])
			strcpy(strValue2,_ms("%d",lpPrefix->m_aValue[1][0]));
	else	strcpy(strValue2,_ms("%d~%d",lpPrefix->m_aValue[1][0],lpPrefix->m_aValue[1][1]));

	if (lpPrefix->m_wType	==	eIP_JOB_SKILL_LEVEL)	effect.m_aValue[1]	=	lpPrefix->m_aValue[1][0];

	strcpy(strExplain,cPREFIX_EFFECT::GetComment(&effect,TRUE,strValue1,strValue2));

	cTEXT::FitPut(iX,iY+=16,cDRAW::WIDTH-20,100,WHITE,strExplain);

	cTEXT::PopShadow();
	s_dibPrefixDetailInfo.Draw();
}

//
//	접두사 리스트를 그려준다.
void
cSET_ITEM_PREFIX::Draw(HWND _hWnd)
{
	DrawPrefixList(_hWnd);
	DrawPrefixInfo(_hWnd);

	if (s_clipPrefix.m_wSerial	!=	0xffff	&&	(s_clipPrefix.m_wType	==	s_iFilter	||	s_iFilter	==	0xffff))
			EnableWindow(GetDlgItem(_hWnd,IDC_PASTE),TRUE);
	else	EnableWindow(GetDlgItem(_hWnd,IDC_PASTE),FALSE);

	cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(s_iSelectPrefix);

	if (!lpPrefix)
	{
		EnableWindow(GetDlgItem(_hWnd,IDC_EDIT),FALSE);
		EnableWindow(GetDlgItem(_hWnd,IDC_REMOVE),FALSE);
		EnableWindow(GetDlgItem(_hWnd,IDC_COPY),FALSE);
	}
	else
	{
		EnableWindow(GetDlgItem(_hWnd,IDC_EDIT),TRUE);
		EnableWindow(GetDlgItem(_hWnd,IDC_REMOVE),TRUE);
		EnableWindow(GetDlgItem(_hWnd,IDC_COPY),TRUE);
	}
}

BOOL
cSET_ITEM_PREFIX::DecodeEffectScriptFile(HWND _hDlg)
{
	char	strFn[512]	=	"";
	char	strFolder[512] = "";

	g_itemPrefix.resetColorizeEffect();

	if (!cFILE::LoadBox(strFn, "Text file {*.txt}\0*.txt\0\0", "txt", NULL, "Load prefix script data file"))
	{
		SetLogFolder();
		return	FALSE;
	}

	if (!DecodeEffectScriptFile(strFn))	return	FALSE;

	ResetPrefixListBox(_hDlg);

	Draw(_hDlg);

	return	TRUE;
}

BOOL
cSET_ITEM_PREFIX::DecodeEffectScriptFile(char *_strFile)
{
	cDECODER	decoder;

	if (!decoder.Upload(_strFile,NULL))	return	FALSE;

	int	iCount	=	g_itemPrefix.getCount(),iOverlappedCount=0;

	while(1)
	{
		char	*strOneLine	=	decoder.GetStream(13);

		if (!strOneLine)	break;

		if (DecodeEffectScript(strOneLine))
		{	
			//	식별 코드/접두사 중복 확인
			{
				cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.findOverlappedData(s_makePrefix.m_str1stPrefix,s_makePrefix.m_str2stPrefix,s_makePrefix.m_wDiscernmentCode,s_iEditPrefix);

				if (!lpPrefix)	g_itemPrefix.add(&s_makePrefix);
				else
				{
					ERRMSG("Duplicate identification code or prefix!!","%s/%s\n\nDiscernment code %d",lpPrefix->m_str1stPrefix,lpPrefix->m_str2stPrefix,lpPrefix->m_wDiscernmentCode);
					iOverlappedCount++;
				}
			}	//	식별 코드/접두사 중복 확인
		}
	}

	cMSG::Put("Data file analysis results","%d prefixes were added.\n\n%d prefixes were overlapped.",g_itemPrefix.getCount()-iCount,iOverlappedCount);

	return	TRUE;
}

BOOL
cSET_ITEM_PREFIX::DecodeEffectScript(char *_strScript)
{
	cDECODER	decoder;

	if (strlen(_strScript)	<=	20)	return	FALSE;

	s_makePrefix.reset();

	decoder.UploadBuffer(_strScript,l_aPrefixOutputEffect);

	char			*lpStream;
	int				iValidCount		=	0;
	int				iGrade			=	0;
	int				iPrefixType		=	0xffff;
	BOOL			isPercentage	=	FALSE;
	cITEM_PREFIX	*lpEditPrefix	=	NULL;

	while(1)
	{	
		lpStream	=	decoder.GetStream('	');

		iGrade++;

		if (!lpStream)
		{
			iValidCount++;
		
			if (iValidCount	>=	20)	break;

			continue;
		}

		int	iIndex	=	iGrade;

		switch(iIndex)
		{
			case	2	:
			{
				int	iDiscernmentCode=	atoi(lpStream);
				lpEditPrefix		=	g_itemPrefix.getIndexByDiscernmentCode(iDiscernmentCode);

				if (!lpEditPrefix)
				{
					ERRMSG("Error!!","Strange prefix identification code!!!!!");
				}
				break;
			}
			case	4	:
			{
				int	iValue	=	decoder.AnalCode(lpStream);

				if (iValue < 255)
					lpEditPrefix->m_bWeaponEffect	=	iValue;
				else
					lpEditPrefix->m_bWeaponEffect	=	255;
				break;
			}
			case	5	:
			{
				int	iValue	=	decoder.AnalCode(lpStream);

				if (iValue < 255)
					lpEditPrefix->m_bArmorEffect	=	iValue;
				else
					lpEditPrefix->m_bArmorEffect	=	255;
				break;
			}
		}
	}

	if (iPrefixType	==	0xffff)	return	FALSE;

	return	TRUE;
}

//
//	화일 분성
BOOL
cSET_ITEM_PREFIX::DecodeFile(HWND _hDlg)
{
	char	strFn[512]	=	"";
	char	strFolder[512] = "";

	if (!cFILE::LoadBox(strFn, "Text file {*.txt}\0*.txt\0\0", "txt", NULL, "Load prefix script data file"))
	{
		SetLogFolder();
		return	FALSE;
	}

	if (g_itemPrefix.getCount() > 0)
		if (cMSG::YESNO(_ms("Decoding the file '%s'.",strFn),"Do you want to initialize the existing prefix data before decoding?")==IDYES)
			g_itemPrefix.reset();

	if (!DecodeFile(strFn))	return	FALSE;

	ResetPrefixListBox(_hDlg);

	Draw(_hDlg);

	return	TRUE;
}

//
//	화일 분성
BOOL
cSET_ITEM_PREFIX::DecodeFile(char *_strFile)
{
	cDECODER	decoder;

	if (!decoder.Upload(_strFile,NULL))	return	FALSE;

	int	iCount	=	g_itemPrefix.getCount(),iOverlappedCount=0;

	while(1)
	{
		char	*strOneLine	=	decoder.GetStream(13);

		if (!strOneLine)	break;

		if (DecodeScript(strOneLine))
		{	

			//	식별 코드/접두사 중복 확인
			{
				cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.findOverlappedData(s_makePrefix.m_str1stPrefix,s_makePrefix.m_str2stPrefix,s_makePrefix.m_wDiscernmentCode,s_iEditPrefix);

				if (!lpPrefix)	g_itemPrefix.add(&s_makePrefix);
				else
				{
					ERRMSG("Duplicate identification code or prefix!!","%s/%s\n\nDiscernment code %d",lpPrefix->m_str1stPrefix,lpPrefix->m_str2stPrefix,lpPrefix->m_wDiscernmentCode);
					iOverlappedCount++;
				}
			}	//	식별 코드/접두사 중복 확인
		}
	}

	cMSG::Put("Data file analysis results","%d prefixes were added.\n\n%d prefixes were overlapped.",g_itemPrefix.getCount()-iCount,iOverlappedCount);

	return	TRUE;
}


//
//	스크립트 문장(?)을 해석 한다.
BOOL
cSET_ITEM_PREFIX::DecodeScript(char *_strScript)
{
	cDECODER	decoder;

	if (strlen(_strScript)	<=	80)
		return	FALSE;

	s_makePrefix.reset();

	decoder.UploadBuffer(_strScript,l_aPrefixOutputEffect);

	char	*lpStream;
	int		iValidCount	=	0;
	int		iGrade		=	0;
	int		iPrefixType	=	0xffff;
	BOOL	isPercentage=	FALSE;

	while(1)
	{	
		lpStream	=	decoder.GetStream('	');

		iGrade++;

		if (!lpStream)
		{
			iValidCount++;
		
			if (iValidCount	>=	20)	break;

			continue;
		}

		int	iIndex	=	iGrade-2;

		if (g_itemPrefix.getCount()	>=	500)
			iIndex	=	iIndex;

		if (iGrade	==	1)
		{
			if (strstr(lpStream, "Deluxe Prefix"))
				s_makePrefix.m_wIsDXPrefix = TRUE;
			if (strstr(lpStream, "Ultimate Prefix"))
				s_makePrefix.m_wIsUltimatePrefix	=	TRUE;
		}

		switch(iIndex)
		{
			case	0	:
			{
				iPrefixType	=	g_itemPrefix.getPrefixType(lpStream);

				if (iPrefixType	==	0xffff)
					return	cMSG::Error("What? Unregistered prefix type... -_-a", "'%s'\n\nPlease contact the creator-_-a~", lpStream);

				s_makePrefix.m_wType	=	iPrefixType;
				break;
			}

			case	1	:
			{
				s_makePrefix.m_wDiscernmentCode	=	atoi(lpStream);
				break;
			}

			case	2	:
				strcpy(s_makePrefix.m_str1stPrefix,lpStream);
				break;

			case	3	:
				strcpy(s_makePrefix.m_str2stPrefix,lpStream);
				break;

			case	4	:
				if (stricmp("×",lpStream)==0)	s_makePrefix.m_aEnableSet[0]	=	FALSE;
				else							s_makePrefix.m_aEnableSet[0]	=	TRUE;
				break;

			case	5	:
				if (stricmp("×",lpStream)==0)	s_makePrefix.m_aEnableSet[1]	=	FALSE;
				else							s_makePrefix.m_aEnableSet[1]	=	TRUE;
				break;

			case	6	:
				if (stricmp("×",lpStream)==0)	s_makePrefix.m_aEnableSet[2]	=	FALSE;
				else							s_makePrefix.m_aEnableSet[2]	=	TRUE;
				break;

			case	7	:
				if (stricmp("noun",lpStream)==0)	s_makePrefix.m_wPrefixForm		=	0;
				else								s_makePrefix.m_wPrefixForm		=	1;
				break;

//			case	8	:
//				cMSG::Put("shape",lpStream);
//				break;

			case	9	:
			{
				for (int i=0;i<+(int)strlen(lpStream);i++)
					if (lpStream[i]	==	'%')	isPercentage	=	TRUE;

				s_makePrefix.m_aValue[0][0]	=	atoi(lpStream);
				break;
			}

			case	10	:
			{
				s_makePrefix.m_aValue[0][1]	=	atoi(lpStream);
				break;
			}


			case	11	:
			{
				char	*strJobName[]	=
				{
"Apprentice Engineer World",
"warrior world",
"Wizard World",
"Werewolf World",
"priestly world",
"Fallen angel world",
"The Thieves' World",
"Martial arts family",
"Magic Spear Weaponry",
"Magic Archer World",
"The Four Seasons of Trainers",
"Four Seasons of Summoning",
"Princess World",
"Transforming Girl World",
"Spiritualist World",
"Devil World",
"Four Spiritual Worlds",
"projection world",
					"end",
				};

				s_makePrefix.m_aValue[1][0]	=	atoi(lpStream);

				for (int i=0;i<dPLAYER_JOB_COUNT;i++)
					if (stricmp(strJobName[i],lpStream)==0)
					{
						s_makePrefix.m_aValue[1][0]	=	i;
						s_makePrefix.m_aValue[1][1]	=	i;
					}
				break;
			}

			case	12	:
			{
				s_makePrefix.m_aValue[1][1]	=	atoi(lpStream);
				break;
			}

			case	13	:
			{
				s_makePrefix.m_sValue3		=	atoi(lpStream);
				break;
			}

			case	14	:
			{
				break;
			}

			case	15	:
			{
				s_makePrefix.m_wDropLevel	=	atoi(lpStream);
				break;
			}

			case	16	:
			{
				if (stricmp("＋",lpStream)==0)	s_makePrefix.m_wPriceFactor	=	0;
				else							s_makePrefix.m_wPriceFactor	=	1;
				break;
			}

			case	17	:
			{
				int	iValue	=	0;

				if (strstr(lpStream,"수치"))
				{
					if (isPercentage)
					{
						lpStream	=	strstr(lpStream,"*");
						s_makePrefix.m_dwPrice				=	atoi(lpStream+1);
						s_makePrefix.m_wComputePriceMethod	=	4;
					}
					else
					{
						if (strstr(lpStream,"*"))
						{
							if (strstr(lpStream,"/"))
							{
								s_makePrefix.m_dwPrice				=	atoi(lpStream);
								s_makePrefix.m_wComputePriceMethod	=	5;
							}
							else
							{
								lpStream	=	strstr(lpStream,"*");

								s_makePrefix.m_dwPrice				=	atoi(lpStream+1);
								s_makePrefix.m_wComputePriceMethod	=	2;
							}
						}
						else
						if (strstr(lpStream,"/"))
						{
							lpStream	=	strstr(lpStream,"/");

							s_makePrefix.m_dwPrice				=	atoi(lpStream+1);
							s_makePrefix.m_wComputePriceMethod	=	3;
						}
						else	s_makePrefix.m_wComputePriceMethod	=	1;
					}
				}
				else
				{
					s_makePrefix.m_dwPrice				=	atoi(lpStream);
					s_makePrefix.m_wComputePriceMethod	=	0;
				}
				break;
			}

			case	18	:	//	보석류
				if (stricmp("×",lpStream)==0)	s_makePrefix.m_aStickableItem[eIK_JEWEL]	=	FALSE;
				else							s_makePrefix.m_aStickableItem[eIK_JEWEL]	=	TRUE;
				break;

			case	19	:	//	헬멧 이후.. 주르르~~
			case	20	:
			case	21	:
			case	22	:
			case	23	:
			case	24	:
			case	25	:
			case	26	:
			case	27	:
			case	28	:
			case	29	:
			case	30	:
			case	31	:
			case	32	:
			case	33	:
			case	34	:
			case	35	:
			case	36	:
			case	37	:
			case	38	:
			case	39	:
			case	40	:
			case	41	:
			case	42	:
			case	43	:
			case	44	:
			case	45	:
			case	46	:
			case	47	:
			case	48	:
			case	49	:
			case	50	:
			case	51	:
			case	52	:
				if (stricmp("×",lpStream)==0)	s_makePrefix.m_aStickableItem[eIK_HELM+iIndex-19]	=	FALSE;
				else							s_makePrefix.m_aStickableItem[eIK_HELM+iIndex-19]	=	TRUE;
				break;

			case	53	:
			{
				int	iValue	=	decoder.AnalCode(lpStream);

				if (iValue < 255)
					s_makePrefix.m_bWeaponEffect	=	iValue;
				else
					s_makePrefix.m_bWeaponEffect	=	255;
				break;
			}
			case	54	:
			{
				int	iValue	=	decoder.AnalCode(lpStream);

				if (iValue < 255)
					s_makePrefix.m_bArmorEffect	=	iValue;
				else
					s_makePrefix.m_bArmorEffect	=	255;
				break;
			}

			case	55	:
			{
				s_makePrefix.m_wCorrectDropChance		=	atoi(lpStream);
				break;
			}
			case	56	:
			{
				s_makePrefix.m_saCorrectQualityPrefix[0]=	atoi(lpStream);
				break;
			}
			case	57	:
			{
				s_makePrefix.m_saCorrectQualityPrefix[1]=	atoi(lpStream);
				break;
			}
		}
	}

	if (iPrefixType	==	0xffff)
		return	FALSE;

	return	TRUE;
}

//
//
LRESULT CALLBACK 
cSET_ITEM_PREFIX::s_procItemPrefixListEvent(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)		// 09.08.11
{
	switch(message)
	{
		case WM_LBUTTONDOWN	:
		{
			s_iSelectPrefix	=	s_listPrefix.GetSelectData();
			Draw(hDlg);
			SetFocus(hDlg);
			break;
		}
		case WM_MOUSEWHEEL :
			{
				
				int zDelta = ((short)HIWORD(wParam));
				
				
				if	(zDelta>=0)
				{
					s_listPrefix.VScroll(SB_LINEUP);
				}
				else{
					s_listPrefix.VScroll(SB_LINEDOWN);
				}
				
			Draw(hDlg);
				break;
			}
	}

	return (0L);
}

//
//
LRESULT CALLBACK 
cSET_ITEM_PREFIX::s_procItemPrefixList(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_INITDIALOG	:
			InitItemPrefixListBox(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,FALSE);
			break;

		case WM_COMMAND :
		{
			switch( LOWORD( wParam ) )
            {
				case	IDC_LOAD_SCRIPT		:
					DecodeFile(hDlg);
					break;

				case	IDC_LOAD_EFFECT_SCRIPT	:
					DecodeEffectScriptFile(hDlg);
					break;

				case	IDOK				:
					EndDialog(hDlg,TRUE);
					break;

				case	IDCANCEL			:
					EndDialog(hDlg,FALSE);
					break;

				case	IDC_COPY			:
				{
					cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(s_iSelectPrefix);
					if (!lpPrefix)	break;

					s_clipPrefix.copy(lpPrefix);

					EnableWindow(GetDlgItem(hDlg,IDC_PASTE),TRUE);
					break;
				}

				case	IDC_ADD				:
				{
					int	iCount	=	g_itemPrefix.getCount();

					if (!Add(hDlg,0xffff))
					{
						if(iCount	<	g_itemPrefix.getCount())
						{
							ResetPrefixListBox(hDlg);
							Draw(hDlg);
						}
						break;
					}

					s_iSelectPrefix	=	g_itemPrefix.add(&s_makePrefix);

					if (s_iFilter	!=	0xffff	&&	s_makePrefix.m_wType	!=	s_iFilter)
					{
						s_iFilter	=	s_makePrefix.m_wType;

						cCOMBOBOX::SelectByData(GetDlgItem(hDlg,IDC_ITEM_PREFIX_TYPE),s_iFilter);

						ResetPrefixListBox(hDlg);
					}
					else
					{
						s_listPrefix.AddItem(0,_ms("%.4d",s_makePrefix.m_wDiscernmentCode),s_iSelectPrefix);
						s_listPrefix.AddItem(1,s_makePrefix.m_str1stPrefix,s_iSelectPrefix);
						s_listPrefix.AddItem(2,g_aItemPrefixType[s_makePrefix.m_wType].m_strName,s_iSelectPrefix);
						s_listPrefix.IncreaseLineCounter();
						s_listPrefix.Sort(0);

						s_listPrefix.selectByData(0,s_iSelectPrefix);
					}

					Draw(hDlg);
					break;
				}

				case	IDC_REMOVE			:
				{
					cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(s_iSelectPrefix);
					if (!lpPrefix)	break;

					if (cMSG::YESNO(dMSG_REALLY_REMOVE_FOLLOW_PREFIX,"%s/%s",lpPrefix->m_str1stPrefix,lpPrefix->m_str2stPrefix)	==	IDNO)
						break;

					g_itemPrefix.remove(lpPrefix->m_wSerial);

					ResetPrefixListBox(hDlg);
					Draw(hDlg);
					break;
				}

				case	IDC_EDIT			:
				{
					cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.get(s_iSelectPrefix);
					if (!lpPrefix)	break;

					if (Add(hDlg,s_iSelectPrefix))
					{
						lpPrefix->copy(&s_makePrefix);

						s_listPrefix.SetItemStr(0,s_listPrefix.m_iSelect,_ms("%.4d",lpPrefix->m_wDiscernmentCode));
						s_listPrefix.SetItemStr(1,s_listPrefix.m_iSelect,lpPrefix->m_str1stPrefix);
						s_listPrefix.SetItemStr(2,s_listPrefix.m_iSelect,g_aItemPrefixType[lpPrefix->m_wType].m_strName);

//						ResetPrefixListBox(hDlg);
						Draw(hDlg);
					}

					break;
				}

				case	IDC_ITEM_PREFIX_TYPE	:
				{
					switch (HIWORD(wParam))
                    {	case	CBN_SELCHANGE	:
						{
							int	iKind		=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_ITEM_PREFIX_TYPE));

							if (s_iFilter	!=	iKind)
							{
								s_iFilter	=	iKind;
								ResetPrefixListBox(hDlg);
								Draw(hDlg);
							}
							break;
						}
					}
					break;
				}

				case	IDC_PASTE			:
				{
					s_iSelectPrefix	=	g_itemPrefix.add(&s_clipPrefix);

					if (s_iFilter	!=	0xffff	&&	s_makePrefix.m_wType	!=	s_iFilter)
					{
						s_iFilter	=	s_makePrefix.m_wType;

						cCOMBOBOX::SelectByData(GetDlgItem(hDlg,IDC_ITEM_PREFIX_TYPE),s_iFilter);

						ResetPrefixListBox(hDlg);
					}
					else
					{
						s_listPrefix.AddItem(0,_ms("%.4d",s_makePrefix.m_wDiscernmentCode),s_iSelectPrefix);
						s_listPrefix.AddItem(1,s_makePrefix.m_str1stPrefix,s_iSelectPrefix);
						s_listPrefix.AddItem(2,g_aItemPrefixType[s_makePrefix.m_wType].m_strName,s_iSelectPrefix);
						s_listPrefix.IncreaseLineCounter();
						s_listPrefix.Sort(0);

						s_listPrefix.selectByData(0,s_iSelectPrefix);
					}

					Draw(hDlg);
					break;
				}
			}
			break;
		}
	}

	return (0L);
}	//	cSET_ITEM_PREFIX::s_procSetBasicItemList(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)


//
//	아이템 정보 윈
LRESULT CALLBACK
cSET_ITEM_PREFIX::s_procPrefixInfoWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_PAINT		:
		{	PAINTSTRUCT ps;

			BeginPaint( hwnd, &ps );

			s_dibPrefixDetailInfo.Active();
			s_dibPrefixDetailInfo.cDIBWND::Draw(ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right-ps.rcPaint.left,ps.rcPaint.bottom-ps.rcPaint.top);

			EndPaint(hwnd, &ps);
			break;
		}

		case WM_VSCROLL			:
		{
			if (s_sbDetailInfo.VScroll(wParam))	DrawPrefixInfo(hwnd);
			break;
		}

		default					:
			return( DefWindowProc( hwnd, uMsg, wParam, lParam ) );
	}

	return (0L);
}	//	cSET_BASIC_ITEM::s_procItemInfoWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)



//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃접두사 추가
BOOL	cSET_ITEM_PREFIX::s_isEditPrefix	=	FALSE;
int		cSET_ITEM_PREFIX::s_iLastPrefixType	=	0;
int		cSET_ITEM_PREFIX::s_iEditPrefix		=	0xffff;

//
//	접두사를 추가한다.
BOOL
cSET_ITEM_PREFIX::Add(HWND hwnd,int _iEditPrefix)
{
	s_makePrefix.reset();

	s_makePrefix.m_wType	=	s_iLastPrefixType;

	s_isEditPrefix	=	FALSE;
	s_iEditPrefix	=	_iEditPrefix;

	if (_iEditPrefix	!=	0xffff)
	{
		s_makePrefix.copy(g_itemPrefix.get(_iEditPrefix));
		s_isEditPrefix	=	TRUE;
	}

	int	result	=	DialogBox( MAIN.hINST,"SET_ITEM_PREFIX",hwnd,(DLGPROC)s_procSetPrefix);

	return	result;
}

//
//	아이템 부가효과 설정 박스 초기화
void
cSET_ITEM_PREFIX::InitSetItemPrefixDlg(HWND _hDlg)
{
//	접두사 타입
	{
		HWND	hComboPrefixType	=	GetDlgItem(_hDlg,IDC_PREFIX_TYPE);
		int		iIndex				=	0;

		while(1)
		{
			if (g_aItemPrefixType[iIndex].m_wType	==	0xffff)	break;

			cCOMBOBOX::Add(hComboPrefixType,g_aItemPrefixType[iIndex].m_strName,g_aItemPrefixType[iIndex].m_wType);

			iIndex++;
		}
	}	//	접두사 타입


//	가격 계산 방법
	{
		HWND	hComboMethod	=	GetDlgItem(_hDlg,IDC_COMPUTE_PRICE_METHOD);
		int		iIndex			=	0;

		while(1)
		{
			if (strcmp(g_strItemPrefixPriceComputeMethod[iIndex],"end")	==	0)	break;

			cCOMBOBOX::Add(hComboMethod,g_strItemPrefixPriceComputeMethod[iIndex],iIndex);

			iIndex++;
		}
	}	//	가격 계산 방법

//	무기 효과 리스트
	{
		HWND	hComboWEL		=	GetDlgItem(_hDlg,IDC_WEAPON_EFFECT_LIST);
		int		iIndex			=	0;

		cCOMBOBOX::Add(hComboWEL,"no effect",0xff);

		while(1)
		{
			if (strcmp(l_aPrefixOutputEffect[iIndex].m_strKeyword,"end")	==	0)	break;

			cCOMBOBOX::Add(hComboWEL,l_aPrefixOutputEffect[iIndex].m_strKeyword,l_aPrefixOutputEffect[iIndex].m_dwCode);

			iIndex++;
		}

		cCOMBOBOX::Select(hComboWEL,0);
	}

//	방어구 효과 리스트
	{
		HWND	hComboAEL		=	GetDlgItem(_hDlg,IDC_ARMOR_EFFECT_LIST);
		int		iIndex			=	0;

		cCOMBOBOX::Add(hComboAEL,"no effect",0xff);

		while(1)
		{
			if (strcmp(l_aPrefixOutputEffect[iIndex].m_strKeyword,"end")	==	0)	break;

			cCOMBOBOX::Add(hComboAEL,l_aPrefixOutputEffect[iIndex].m_strKeyword,l_aPrefixOutputEffect[iIndex].m_dwCode);

			iIndex++;
		}

		cCOMBOBOX::Select(hComboAEL,0);
	}

	FillSetItemPrefixDlg(_hDlg);
}

//
//	아이템 부가효과 설정 박스 내용 채우기
void
cSET_ITEM_PREFIX::FillSetItemPrefixDlg(HWND _hDlg)
{
	SetDlgItemText(_hDlg,IDC_SCRIPT,"");

//	설명
	{
		cITEM_EFFECT	effect;
		char			strExplain[512];

		effect.reset();
		effect.m_wEffect	=	s_makePrefix.m_wType;
		effect.m_aValue[2]	=	1;
		strcpy(strExplain,cPREFIX_EFFECT::GetComment(&effect,FALSE,"[수치1]","[수치2]"));

		SetDlgItemText(_hDlg,IDC_EXPLAIN,strExplain);
	}

//	접두사 2개
	SetDlgItemText(_hDlg,IDC_1ST_PREFIX,s_makePrefix.m_str1stPrefix);
	SetDlgItemText(_hDlg,IDC_2ST_PREFIX,s_makePrefix.m_str2stPrefix);

//	접두사 형태
	if (s_makePrefix.m_wPrefixForm	==	0)
		CheckRadioButton(_hDlg,IDC_NOUN,IDC_ADJECTIVE,IDC_NOUN);
	else
		CheckRadioButton(_hDlg,IDC_NOUN,IDC_ADJECTIVE,IDC_ADJECTIVE);

//	접두사 타입
	{
		HWND	hComboPrefixType	=	GetDlgItem(_hDlg,IDC_PREFIX_TYPE);

		cCOMBOBOX::SelectByData(hComboPrefixType,s_makePrefix.m_wType);
	}	//	접두사 타입

//	가격 계산 방법
	{
		HWND	hComboMethod	=	GetDlgItem(_hDlg,IDC_COMPUTE_PRICE_METHOD);

		cCOMBOBOX::SelectByData(hComboMethod,s_makePrefix.m_wComputePriceMethod);
	}	//	가격 계산 방법

//	무기효과
	{
		HWND	hComboWEL		=	GetDlgItem(_hDlg,IDC_WEAPON_EFFECT_LIST);

		cCOMBOBOX::SelectByData(hComboWEL,s_makePrefix.m_bWeaponEffect);
	}

//	방어구 효과
	{
		HWND	hComboAEL		=	GetDlgItem(_hDlg,IDC_ARMOR_EFFECT_LIST);

		cCOMBOBOX::SelectByData(hComboAEL,s_makePrefix.m_bArmorEffect);
	}

//	드랍 레벨
	SetDlgItemInt(_hDlg,IDC_DROP_LEVEL,s_makePrefix.m_wDropLevel,FALSE);

//	식별 코드
	SetDlgItemInt(_hDlg,IDC_DISCERNMENT_CODE,s_makePrefix.m_wDiscernmentCode,FALSE);

//	적용 가능 위치
	if (s_makePrefix.m_aEnableSet[0])
		CheckDlgButton(_hDlg,IDC_1ST,TRUE);
	else
		CheckDlgButton(_hDlg,IDC_1ST,FALSE);
	if (s_makePrefix.m_aEnableSet[1])
		CheckDlgButton(_hDlg,IDC_2ST,TRUE);
	else
		CheckDlgButton(_hDlg,IDC_2ST,FALSE);
	if (s_makePrefix.m_aEnableSet[2])
		CheckDlgButton(_hDlg,IDC_3ST,TRUE);
	else
		CheckDlgButton(_hDlg,IDC_3ST,FALSE);

//	수치-_-들
	SetDlgItemInt(_hDlg,IDC_VALUE1_1,s_makePrefix.m_aValue[0][0],FALSE);
	SetDlgItemInt(_hDlg,IDC_VALUE1_2,s_makePrefix.m_aValue[0][1],FALSE);
	SetDlgItemInt(_hDlg,IDC_VALUE2_1,s_makePrefix.m_aValue[1][0],FALSE);
	SetDlgItemInt(_hDlg,IDC_VALUE2_2,s_makePrefix.m_aValue[1][1],FALSE);
	SetDlgItemInt(_hDlg,IDC_VALUE3	,s_makePrefix.m_sValue3,FALSE);

	SetDlgItemInt(_hDlg,IDC_CORRECT_DROP_CHANCE		,s_makePrefix.m_wCorrectDropChance,FALSE);
	SetDlgItemInt(_hDlg,IDC_CORRECT_VALUE1_QUALITY	,s_makePrefix.m_saCorrectQualityPrefix[0],TRUE);
	SetDlgItemInt(_hDlg,IDC_CORRECT_VALUE2_QUALITY	,s_makePrefix.m_saCorrectQualityPrefix[1],TRUE);

	if	(s_makePrefix.m_wIsDXPrefix)
		CheckDlgButton(_hDlg,IDC_IS_DX_PREFIX,TRUE);
	else
		CheckDlgButton(_hDlg,IDC_IS_DX_PREFIX,FALSE);

	if	(s_makePrefix.m_wIsUltimatePrefix)
		CheckDlgButton(_hDlg,IDC_IS_ULTRA_PREFIX,TRUE);
	else
		CheckDlgButton(_hDlg,IDC_IS_ULTRA_PREFIX,FALSE);

//	가격 계산할때 곱할 것인가 더할 것인가
	if (s_makePrefix.m_wPriceFactor	==	0	)
		CheckRadioButton(_hDlg,IDC_MULTIPLE,IDC_PLUS,IDC_PLUS);
	else
		CheckRadioButton(_hDlg,IDC_MULTIPLE,IDC_PLUS,IDC_MULTIPLE);
	
//	가격 인자
	SetDlgItemInt(_hDlg,IDC_PRICE	,s_makePrefix.m_dwPrice,FALSE);

//	적용 가능 아이템
	{
		for (int i=IDC_HELM;i<=IDC_JEWEL;i++)
		{
			if (s_makePrefix.m_aStickableItem[i-IDC_HELM])
				CheckDlgButton(_hDlg,i,TRUE);
			else
				CheckDlgButton(_hDlg,i,FALSE);
		}

		if(s_makePrefix.m_bf1IsStickableItemScythe)
			CheckDlgButton(_hDlg,IDC_SCYTHE,TRUE);
		else
			CheckDlgButton(_hDlg,IDC_SCYTHE,FALSE);

		if(s_makePrefix.m_bf1IsStickAbleItemArmWeapon)
			CheckDlgButton(_hDlg,IDC_ARM_WEAPON,TRUE);
		else
			CheckDlgButton(_hDlg,IDC_ARM_WEAPON,FALSE);			
		if(s_makePrefix.m_bf1IsStickAbleItemBook)
			CheckDlgButton(_hDlg,IDC_BOOK,TRUE);
		else
			CheckDlgButton(_hDlg,IDC_BOOK,FALSE);	
	}	//	적용 가능 아이템
}

//
//	아이템 부가효과 설정 박스 내용 채우기
BOOL
cSET_ITEM_PREFIX::GetSetItemPrefixDlg(HWND _hDlg)
{
	cITEM_PREFIX	tempPrefix;
	BOOL			temp;

	tempPrefix.copy(&s_makePrefix);

//	접두사 2개
	GetDlgItemText(_hDlg,IDC_1ST_PREFIX,tempPrefix.m_str1stPrefix,sizeof(tempPrefix.m_str1stPrefix));
	GetDlgItemText(_hDlg,IDC_2ST_PREFIX,tempPrefix.m_str2stPrefix,sizeof(tempPrefix.m_str1stPrefix));

//	식별 코드
	tempPrefix.m_wDiscernmentCode=	GetDlgItemInt(_hDlg,IDC_DISCERNMENT_CODE,&temp,FALSE);
	ERRORHANDLER(!temp);

//	식별 코드/접두사 중복 확인
	{
		if (stricmp(tempPrefix.m_str1stPrefix,"")	==	0	||	stricmp(tempPrefix.m_str2stPrefix,"")	==	0)
			return	ERRMSG(dMSG_ADD_PREFIX_ERROR,dMSG_VALID_PREFIX);

		cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.findOverlappedData(tempPrefix.m_str1stPrefix,tempPrefix.m_str2stPrefix,tempPrefix.m_wDiscernmentCode,s_iEditPrefix);

		if (lpPrefix)	return	ERRMSG(dMSG_ADD_PREFIX_ERROR,dMSG_PREFIX_OVERLAPPING);
	}	//	식별 코드/접두사 중복 확인

//	접두사 형태
	if (IsDlgButtonChecked(_hDlg,IDC_NOUN		))	tempPrefix.m_wPrefixForm	=	0;
	if (IsDlgButtonChecked(_hDlg,IDC_ADJECTIVE	))	tempPrefix.m_wPrefixForm	=	1;

//	접두사 타입
	{
		HWND	hComboPrefixType	=	GetDlgItem(_hDlg,IDC_PREFIX_TYPE);

		tempPrefix.m_wType			=	cCOMBOBOX::GetData(hComboPrefixType);
	}	//	접두사 타입

//	가격 계산 방법
	{
		HWND	hComboMethod		=	GetDlgItem(_hDlg,IDC_COMPUTE_PRICE_METHOD);

		tempPrefix.m_wComputePriceMethod	=	cCOMBOBOX::GetData(hComboMethod);
	}	//	가격 계산 방법

//	드랍 레벨
	tempPrefix.m_wDropLevel		=	GetDlgItemInt(_hDlg,IDC_DROP_LEVEL,&temp,FALSE);
	ERRORHANDLER(!temp);

//	적용 가능 위치
	if (IsDlgButtonChecked(_hDlg,IDC_1ST))	s_makePrefix.m_aStickableItem[0]	=	1;
	else									s_makePrefix.m_aStickableItem[0]	=	0;
	if (IsDlgButtonChecked(_hDlg,IDC_2ST))	s_makePrefix.m_aStickableItem[1]	=	1;
	else									s_makePrefix.m_aStickableItem[1]	=	0;
	if (IsDlgButtonChecked(_hDlg,IDC_3ST))	s_makePrefix.m_aStickableItem[2]	=	1;
	else									s_makePrefix.m_aStickableItem[2]	=	0;

//	수치-_-들
	tempPrefix.m_aValue[0][0]	=	GetDlgItemInt(_hDlg,IDC_VALUE1_1,&temp,FALSE);
	ERRORHANDLER(!temp);
	tempPrefix.m_aValue[0][1]	=	GetDlgItemInt(_hDlg,IDC_VALUE1_2,&temp,FALSE);
	ERRORHANDLER(!temp);
	tempPrefix.m_aValue[1][0]	=	GetDlgItemInt(_hDlg,IDC_VALUE2_1,&temp,FALSE);
	ERRORHANDLER(!temp);
	tempPrefix.m_aValue[1][1]	=	GetDlgItemInt(_hDlg,IDC_VALUE2_2,&temp,FALSE);
	ERRORHANDLER(!temp);
	tempPrefix.m_sValue3		=	GetDlgItemInt(_hDlg,IDC_VALUE3,&temp,FALSE);
	ERRORHANDLER(!temp);

	tempPrefix.m_wCorrectDropChance			=	GetDlgItemInt(_hDlg,IDC_CORRECT_DROP_CHANCE,&temp,FALSE);
	ERRORHANDLER(!temp);
	tempPrefix.m_saCorrectQualityPrefix[0]	=	GetDlgItemInt(_hDlg,IDC_CORRECT_VALUE1_QUALITY,&temp,TRUE);
	ERRORHANDLER(!temp);
	tempPrefix.m_saCorrectQualityPrefix[1]	=	GetDlgItemInt(_hDlg,IDC_CORRECT_VALUE2_QUALITY,&temp,TRUE);
	ERRORHANDLER(!temp);

	if	(IsDlgButtonChecked(_hDlg,IDC_IS_DX_PREFIX))
		tempPrefix.m_wIsDXPrefix	=	TRUE;
	else
		tempPrefix.m_wIsDXPrefix	=	FALSE;

	if	(IsDlgButtonChecked(_hDlg,IDC_IS_ULTRA_PREFIX))
		tempPrefix.m_wIsUltimatePrefix	=	TRUE;
	else
		tempPrefix.m_wIsUltimatePrefix	=	FALSE;

//	가격 계산할때 곱할 것인가 더할 것인가
	if (IsDlgButtonChecked(_hDlg,IDC_PLUS))
		tempPrefix.m_wPriceFactor	=	0;
	else
		tempPrefix.m_wPriceFactor	=	1;
	
//	가격 인자
	tempPrefix.m_dwPrice		=	GetDlgItemInt(_hDlg,IDC_PRICE,&temp,FALSE);
	ERRORHANDLER(!temp);

	if (tempPrefix.m_dwPrice	==	0	&&	tempPrefix.m_wComputePriceMethod	!=	1)
	{
		SetFocus(GetDlgItem(_hDlg,IDC_PRICE));
		return	ERRMSG(dMSG_ADD_PREFIX_ERROR,dMSG_VALID_PRICE_VALUE);
	}

//	적용 가능 아이템
	{
		for (int i=IDC_HELM;i<=IDC_JEWEL;i++)
		{
			if (IsDlgButtonChecked(_hDlg,i))
				tempPrefix.m_aStickableItem[i-IDC_HELM]	=	TRUE;
			else
				tempPrefix.m_aStickableItem[i-IDC_HELM]	=	FALSE;
		}
		if	(IsDlgButtonChecked(_hDlg,IDC_SCYTHE))
			tempPrefix.m_bf1IsStickableItemScythe	=	TRUE;
		else
			tempPrefix.m_bf1IsStickableItemScythe	=	FALSE;

		if	(IsDlgButtonChecked(_hDlg,IDC_ARM_WEAPON))
			tempPrefix.m_bf1IsStickAbleItemArmWeapon	=	TRUE;
		else
			tempPrefix.m_bf1IsStickAbleItemArmWeapon	=	FALSE;


		if	(IsDlgButtonChecked(_hDlg,IDC_BOOK))
			tempPrefix.m_bf1IsStickAbleItemBook	=	TRUE;
		else
			tempPrefix.m_bf1IsStickAbleItemBook	=	FALSE;
	}	//	적용 가능 아이템


	s_makePrefix.copy(&tempPrefix);

	s_iLastPrefixType	=	s_makePrefix.m_wType;

	return	TRUE;

ERROR_LABEL:
	return	ERRMSG(dMSG_ADD_PREFIX_ERROR,dMSG_SOME_DATA_VALID);
}

//
//	스크립트 문장(?)을 해석 한다.
BOOL
cSET_ITEM_PREFIX::DecodeScript(HWND _hDlg)
{
	char		strScript[512];

	cDECODER	decoder;

	GetDlgItemText(_hDlg,IDC_SCRIPT,strScript,sizeof(strScript));

	if (strlen(strScript)	<=	80)
		return	cMSG::Error("접두사 분석 에러!!","이 작업은 'Project  ‘붉은 보석’ 아이템.xls'\n\n에 세부 접두사 탭에서 한 라인을 그대로 복사해 넣어야 가능합니다.");

	s_makePrefix.reset();
	FillSetItemPrefixDlg(_hDlg);

	decoder.UploadBuffer(strScript,NULL);

	char	*lpStream;
	int		iValidCount	=	0;
	int		iGrade		=	0;
	int		iPrefixType	=	0xffff;
	BOOL	isPercentage=	FALSE;

	while(1)
	{	
		lpStream	=	decoder.GetStream('	');

		iGrade++;

		if (!lpStream)
		{
			iValidCount++;
		
			if (iValidCount	>=	20)	break;

			continue;
		}

		int	iIndex	=	iGrade-2;

		switch(iIndex)
		{
			case	0	:
			{
				iPrefixType	=	g_itemPrefix.getPrefixType(lpStream);

				if (iPrefixType	==	0xffff)
					return	cMSG::Error("얼래? 등록되지 않은 접두사 타입인디... -_-a","'%s'\n\n제작자-_-a한테 문의 하세요~");

				cCOMBOBOX::SelectByData(GetDlgItem(_hDlg,IDC_PREFIX_TYPE),iPrefixType);
				break;
			}

			case	1	:
			{
				int	iCode	=	atoi(lpStream);
				SetDlgItemInt(_hDlg,IDC_DISCERNMENT_CODE,iCode,FALSE);
				break;
			}

			case	2	:
				SetDlgItemText(_hDlg,IDC_1ST_PREFIX,lpStream);
				break;

			case	3	:
				SetDlgItemText(_hDlg,IDC_2ST_PREFIX,lpStream);
				break;

			case	4	:
				if (stricmp("×",lpStream)==0)	CheckDlgButton(_hDlg,IDC_1ST,FALSE);
				else							CheckDlgButton(_hDlg,IDC_1ST,TRUE);
				break;

			case	5	:
				if (stricmp("×",lpStream)==0)	CheckDlgButton(_hDlg,IDC_2ST,FALSE);
				else							CheckDlgButton(_hDlg,IDC_2ST,TRUE);
				break;

			case	6	:
				if (stricmp("×",lpStream)==0)	CheckDlgButton(_hDlg,IDC_3ST,FALSE);
				else							CheckDlgButton(_hDlg,IDC_3ST,TRUE);
				break;

			case	7	:
				if (stricmp("noun",lpStream)==0)	CheckRadioButton(_hDlg,IDC_NOUN,IDC_ADJECTIVE,IDC_NOUN);
				else								CheckRadioButton(_hDlg,IDC_NOUN,IDC_ADJECTIVE,IDC_ADJECTIVE);
				break;

//			case	8	:
//				cMSG::Put("shape",lpStream);
//				break;

			case	9	:
			{
				int	iValue	=	atoi(lpStream);

				for (int i=0;i<+(int)strlen(lpStream);i++)
					if (lpStream[i]	==	'%')	isPercentage	=	TRUE;

				SetDlgItemInt(_hDlg,IDC_VALUE1_1,iValue,FALSE);
				break;
			}

			case	10	:
			{
				int	iValue	=	atoi(lpStream);

				SetDlgItemInt(_hDlg,IDC_VALUE1_2,iValue,FALSE);
				break;
			}


			case	11	:
			{
				int	iValue	=	atoi(lpStream);

				if (s_makePrefix.m_aValue[1][0]	==	0)
					for (int i=0;i<dPLAYER_JOB_COUNT;i++)
						if (stricmp(g_aDefaultJobName[i],lpStream)==0)
							iValue	=	i;

				SetDlgItemInt(_hDlg,IDC_VALUE2_1,iValue,FALSE);
				break;
			}


			case	12	:
			{
				int	iValue	=	atoi(lpStream);

				SetDlgItemInt(_hDlg,IDC_VALUE2_2,iValue,FALSE);
				break;
			}


			case	13	:
			{
				int	iValue	=	atoi(lpStream);

				SetDlgItemInt(_hDlg,IDC_VALUE3,iValue,FALSE);
				break;
			}

			case	14	:
			{
				break;
			}

			case	15	:
			{
				int	iValue	=	atoi(lpStream);

				SetDlgItemInt(_hDlg,IDC_DROP_LEVEL,iValue,FALSE);
				break;
			}

			case	16	:
			{
				if (stricmp("＋",lpStream)==0)	CheckRadioButton(_hDlg,IDC_MULTIPLE,IDC_PLUS,IDC_PLUS);
				else							CheckRadioButton(_hDlg,IDC_MULTIPLE,IDC_PLUS,IDC_MULTIPLE);
				break;
			}

			case	17	:
			{
				int	iValue	=	0;

				if (strstr(lpStream,"수치"))
				{
					if (isPercentage)
					{
						lpStream	=	strstr(lpStream,"*");
						iValue		=	atoi(lpStream+1);
						cCOMBOBOX::SelectByData(GetDlgItem(_hDlg,IDC_COMPUTE_PRICE_METHOD),3);
					}
					else
					{
						if (strstr(lpStream,"*"))
						{
							if (strstr(lpStream,"/"))
							{
								iValue		=	atoi(lpStream);
								cCOMBOBOX::SelectByData(GetDlgItem(_hDlg,IDC_COMPUTE_PRICE_METHOD),4);
							}
							else
							{
								lpStream	=	strstr(lpStream,"*");
								iValue		=	atoi(lpStream+1);

								cCOMBOBOX::SelectByData(GetDlgItem(_hDlg,IDC_COMPUTE_PRICE_METHOD),1);
							}
						}
						else
						if (strstr(lpStream,"/"))
						{
							lpStream	=	strstr(lpStream,"/");
							iValue		=	atoi(lpStream+1);
							cCOMBOBOX::SelectByData(GetDlgItem(_hDlg,IDC_COMPUTE_PRICE_METHOD),2);
						}
					}
				}
				else
				{
					cCOMBOBOX::SelectByData(GetDlgItem(_hDlg,IDC_COMPUTE_PRICE_METHOD),0);

					iValue	=	atoi(lpStream);
					SetDlgItemInt(_hDlg,IDC_PRICE	,iValue,FALSE);
				}

				SetDlgItemInt(_hDlg,IDC_PRICE	,iValue,FALSE);

				break;
			}

			case	18	:	//	보석류
				if (stricmp("×",lpStream)==0)	CheckDlgButton(_hDlg,IDC_JEWEL,FALSE);
				else							CheckDlgButton(_hDlg,IDC_JEWEL,TRUE);
				break;

			case	19	:	//	헬멧 이후.. 주르르~~
			case	20	:
			case	21	:
			case	22	:
			case	23	:
			case	24	:
			case	25	:
			case	26	:
			case	27	:
			case	28	:
			case	29	:
			case	30	:
			case	31	:
			case	32	:
			case	33	:
			case	34	:
			case	35	:
			case	36	:
			case	37	:
			case	38	:
			case	39	:
			case	40	:
			case	41	:
			case	42	:
			case	43	:
			case	44	:
			case	45	:
			case	46	:
			case	47	:
			case	48	:
				if (stricmp("×",lpStream)==0)	CheckDlgButton(_hDlg,IDC_HELM+iIndex-19,FALSE);
				else							CheckDlgButton(_hDlg,IDC_HELM+iIndex-19,TRUE);
				break;
		}
	}

	return	TRUE;
}

LRESULT CALLBACK
cSET_ITEM_PREFIX::s_procSetPrefix(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{	case WM_INITDIALOG	:
			InitSetItemPrefixDlg(hDlg);
			break;

		case WM_CLOSE		:
			EndDialog(hDlg,FALSE);
			break;

		case WM_COMMAND :
		{
			switch( LOWORD( wParam ) )
            {
				case	IDOK		:
					if (!GetSetItemPrefixDlg(hDlg))	break;
					EndDialog(hDlg,TRUE);
					break;

				case	IDCANCEL	:
				case	IDCANCEL2	:
					EndDialog(hDlg,FALSE);
					break;

				case	IDC_APPLY	:
				{
					if (!DecodeScript(hDlg))	break;
					if (!GetSetItemPrefixDlg(hDlg))	break;

					ShowWindow(GetDlgItem(hDlg,IDOK),SW_HIDE);
					ShowWindow(GetDlgItem(hDlg,IDCANCEL),SW_HIDE);
					ShowWindow(GetDlgItem(hDlg,IDCANCEL2),SW_SHOW);

					s_iSelectPrefix	=	g_itemPrefix.add(&s_makePrefix);

					SetDlgItemText(hDlg,IDC_SCRIPT,"");
					SetFocus(GetDlgItem(hDlg,IDC_SCRIPT));
					break;
				}

				case	IDC_PREFIX_TYPE	:
				{
					switch (HIWORD(wParam))
                    {	case	CBN_SELCHANGE	:
						{
							int	iType	=	cCOMBOBOX::GetData(GetDlgItem(hDlg,IDC_PREFIX_TYPE));

							cITEM_EFFECT	effect;
							char			strExplain[512];

							effect.reset();
							effect.m_wEffect	=	iType;
							effect.m_aValue[2]	=	1;
							strcpy(strExplain,cPREFIX_EFFECT::GetComment(&effect,FALSE,"[수치1]","[수치2]"));

							SetDlgItemText(hDlg,IDC_EXPLAIN,strExplain);
							break;
						}
					}
					break;
				}

			}
			break;
		}
	}

	return (0L);
}	//	cSET_ITEM_PREFIX::s_procSetPrefix(HWND hDlg,UINT message,WPARAM wParam, LPARAM lParam)

//┃접두사 추가
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━