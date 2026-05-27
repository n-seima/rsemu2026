#include	"command.h"
#include	"cDecoder.h"
#include	"cMain.h"
#include	"cProject.h"
#include	"cMapWnd.h"
#include	"secretDungeon.h"

HWND		cCommandDlg::s_hWnd	=	NULL;

void
cCommandDlg::Init(HWND _hDlg)
{
	DialogBox( MAIN.hINST,"COMMAND_WORK",_hDlg,(DLGPROC)ProcDlg);
}

void
cCommandDlg::InitDlg(HWND _hDlg)
{
	cFILE	file;

	if	(!file.Open("dev data/mangchi/help.txt","rb"))
		return;

	char	*pText	=	new	char [file.Length+1];

	file.Read(pText,file.Length);

	pText[file.Length]	=	NULL;

	file.Close();

	SetDlgItemText(_hDlg,IDC_HELP_SCRIPT,pText);

	pKILL(pText);
}

void
cCommandDlg::Launch(HWND _hDlg)
{
	cDECODER	decoder;

	char		strCommandText[256];

	GetDlgItemText(_hDlg,IDC_COMMAND_LINE,strCommandText,256);

	decoder.UploadBuffer(strCommandText,NULL);

	char	*lpstrCommand	=	decoder.GetWord();

	if	(stricmp(lpstrCommand,"return_dx_item")	==	0)
	{
		cDECODER	dec;

		if	(!dec.Upload(_ms("%s/returnItem.txt",_logFolder),NULL))
			return;

		while(1)
		{
			char	*lpstrItem	=	dec.GetStream();

			if	(stricmp(lpstrItem,"end")	==	0)
			{
				cMSG::Put("DX Item Processing,070817","Complete");
				return;
			}


			cBASIC_ITEM	*lpItem	=	cBASIC_ITEM::Get(lpstrItem);

			if	(!lpItem)
				continue;

			int		iMin	=	dec.GetNumber();
			int		iMax	=	dec.GetNumber();

			lpItem->m_wMinDamage	=	iMin;
			lpItem->m_wMaxDamage	=	iMax;

		}
	}

	if	(stricmp(lpstrCommand,"export_secret_dungeon_info")	==	0)
	{
		cFILE	file;
		int		i;

		file.Open("secret_dungeon_info.txt","wt");

		cPROJECT::SetMapFolder();

		cSTRINGS	*lpFiles	=	cFOLDER::GetFiles("*.rmd");

		if	(!lpFiles)
		{
			ERRMSG("error!!","can not find map list!");
			return;
		}

		for (i=0;i<lpFiles->Count;i++)
		{
			cSTRING	*lpFile	=	lpFiles->Pick(i);

			cPROJECT::SetMapFolder();

			cMAP::Load(lpFile->String,0);

			char	*lpstrSDName	=	"valid";

			cSecretDungeon	*lpSD	=	g_sdManager.get(g_aMap[0].m_wLinkSecretDungeon);
			
			if	(lpSD)
				lpstrSDName	=	lpSD->m_strName;

			file.writeStream("%d	%s	%s	%d	%s\n",i,g_aMap[0].m_strFName,g_aMap[0].m_strName,g_aMap[0].m_bf4MinimapDisplayLevel,lpstrSDName);

			SetLogFolder();
			Sleep(1);
		}

		KILL(lpFiles);

		cPROJECT::SetMapFolder();

		SetCurrentDirectory("expansion");

		lpFiles	=	cFOLDER::GetFiles("*.rmd");

		if	(lpFiles)
		{
			for (i=0;i<lpFiles->Count;i++)
			{
				cSTRING	*lpFile	=	lpFiles->Pick(i);

				cPROJECT::SetMapFolder();

				SetCurrentDirectory("expansion");
				cMAP::Load(lpFile->String,0);

				char	*lpstrSDName	=	"valid";

				cSecretDungeon	*lpSD	=	g_sdManager.get(g_aMap[0].m_wLinkSecretDungeon);
				
				if	(lpSD)
					lpstrSDName	=	lpSD->m_strName;

				file.writeStream("%d	%s	%s	%d	%s\n",0,g_aMap[0].m_strFName,g_aMap[0].m_strName,g_aMap[0].m_bf4MinimapDisplayLevel,lpstrSDName);

				SetLogFolder();
				Sleep(1);
			}
		}

		KILL(lpFiles);

		file.Close();

		cMSG::Put("Extract minimap","Operation completed");

		return;
	}
	
	if	(stricmp(lpstrCommand,"export_all_minimap")	==	0)
	{
		int	iScale	=	decoder.GetNumber();
		int	i;

		if	(iScale	>	100	||	iScale	<	1)
		{
			cMSG::Put("All minimap extraction operations failed!","Please check the help again");
			return;
		}

		if (cMSG::YESNO("Extract minimap", "Extract all minimaps to %d percent scale.\n\nDo you want to proceed?", iScale) == IDNO)
			return;

		SetLogFolder();

		CreateDirectory("export_minimap",NULL);

		cPROJECT::SetMapFolder();

		cSTRINGS	*lpFiles	=	cFOLDER::GetFiles("*.rmd");

		if	(!lpFiles)
		{
			ERRMSG("error!!","can not find map list!");
			return;
		}

		for (i=0;i<lpFiles->Count;i++)
		{
			cSTRING	*lpFile	=	lpFiles->Pick(i);

			cPROJECT::SetMapFolder();

			cMAP::Load(lpFile->String,0);

			char	strName[512];
			sprintf(strName,"%s/export_minimap/%s.tga",_logFolder,g_aMap[0].m_strFName);

			g_MAPWND.drawMapToFile(strName,iScale);

			SetLogFolder();
			g_MAPWND.Draw();
			Sleep(1);
		}

		KILL(lpFiles);

		cPROJECT::SetMapFolder();

		SetCurrentDirectory("expansion");

		lpFiles	=	cFOLDER::GetFiles("*.rmd");

		if	(lpFiles)
		{
			for (i=0;i<lpFiles->Count;i++)
			{
				cSTRING	*lpFile	=	lpFiles->Pick(i);

				cPROJECT::SetMapFolder();

				SetCurrentDirectory("expansion");
				cMAP::Load(lpFile->String,0);

				char	strName[512];
				sprintf(strName,"%s/export_minimap/(expansion)%s.tga",_logFolder,g_aMap[0].m_strFName);

				g_MAPWND.drawMapToFile(strName,iScale);

				SetLogFolder();
				g_MAPWND.Draw();
				Sleep(1);
			}
		}

		KILL(lpFiles);


		cMSG::Put("Extract minimap","Operation completed");
		return;
	}
	
	if	(stricmp(lpstrCommand,"export_correct_int")	==	0)
	{
		if	(cMSG::YESNO("Extract character knowledge correction information","Do you want to proceed?") == IDNO)
			return;

		SetLogFolder();

		cPROJECT::SetMapFolder();

		cFILE	file;

		file.Open("secret_dungeon_info.txt","wt");

		cSTRINGS	*lpFiles	=	cFOLDER::GetFiles("*.rmd");

		if	(!lpFiles)
		{
			ERRMSG("error!!","can not find map list!");
			return;
		}

		for (int i=0;i<lpFiles->Count;i++)
		{
			cSTRING	*lpFile	=	lpFiles->Pick(i);

			cPROJECT::SetMapFolder();

			cMAP::Load(lpFile->String,0);

			cCHARACTER_SET	*lpCS	=	&g_lpActmap->m_setCharacter;

			for (int j=0;j<dMAX_CHARACTER;j++)
			{
				cCHARACTER	*lpCharacter	=	lpCS->getCharacter(j);

				if	(!lpCharacter)
					continue;

				if	(lpCharacter->m_bf1IsReviseIntelligence)
					file.writeStream("%s	%s\n",g_aMap[0].m_strFName,lpCharacter->m_strName);
			}

			Sleep(1);
		}

		KILL(lpFiles);

		cPROJECT::SetMapFolder();

		SetCurrentDirectory("expansion");

		lpFiles	=	cFOLDER::GetFiles("*.rmd");

		if	(lpFiles)
		{
			for (int i=0;i<lpFiles->Count;i++)
			{
				cSTRING	*lpFile	=	lpFiles->Pick(i);

				cPROJECT::SetMapFolder();

				SetCurrentDirectory("expansion");
				cMAP::Load(lpFile->String,0);

				cCHARACTER_SET	*lpCS	=	&g_lpActmap->m_setCharacter;

				for (int j=0;j<dMAX_CHARACTER;j++)
				{
					cCHARACTER	*lpCharacter	=	lpCS->getCharacter(j);

					if	(!lpCharacter)
						continue;

					if	(lpCharacter->m_bf1IsReviseIntelligence)
						file.writeStream("%s	%s\n",g_aMap[0].m_strFName,lpCharacter->m_strName);
				}

				Sleep(1);
			}
		}

		KILL(lpFiles);

		file.Close();

		cMSG::Put("Extract","Operation completed");
		return;
	}
	if	(stricmp(lpstrCommand,"export_item")	==	0)
	{
		BOOL	bIsOk			=	FALSE;
		int		iBeginItem		=	decoder.GetNumber();
		int		iEndItem		=	decoder.GetNumber();
		char	*lpstrFileName	=	decoder.GetStream();

		if	(cMSG::YESNO("Item Extraction","Save items %d to %d in file '%s'.\n\nDo you want to continue the operation?",iBeginItem,iEndItem,lpstrFileName) == IDNO )
			return;

		cFILE	file,textFile;

		if	(!file.Open(_ms("data/scenario/red stone/%s",lpstrFileName),"wb"))
		{
			cMSG::Put("Item extraction error!","Failed to open file.");
			return;
		}

		textFile.Open("_______tempText.~~~","wb");

		int	iItemCount	=	0;

		file.Write(&iItemCount,4);
		file.Write(&iItemCount,4);

		for	(int i=iBeginItem;i<=iEndItem;i++)
		{
			cBASIC_ITEM	*lpItem	=	&g_aBasicItem[i];

			if	(lpItem->m_iSerial	==	0xffffffff)
				continue;

			lpItem->m_dwCommentAddress	=	textFile.Locate();
			int		iTextSize			=	strlen(lpItem->m_pItemComment);
			file.Write(lpItem,sizeof(CBasicItemData));

			textFile.Write(&lpItem->m_dwCommentSize,4);	//	아이템 설명 사이즈 저장
			textFile.Write(lpItem->m_pItemComment,lpItem->m_dwCommentSize);	//	아이템 설명 저장

			iItemCount++;
		}
		textFile.Close();

		int	iTextDataLocate	=	file.Locate();

		file.Rewind();
		file.Write(&iItemCount,4);
		file.Write(&iTextDataLocate,4);
		file.Seek(iTextDataLocate,SEEK_SET);

		file.Append("_______tempText.~~~");

		DeleteFile("_______tempText.~~~");

		file.Close();							//	닫는다.

		cMSG::Put("Item extraction complete!","Saved in file %s.",lpstrFileName);

		return;
	}

	if	(stricmp(lpstrCommand,"import_item")	==	0)
	{
		char	*lpstrFileName	=	decoder.GetStream();

		if	(cMSG::YESNO("Add item","Add item data to file '%s' to current item data.\n\nDo you want to continue with the operation?",lpstrFileName) == IDNO)
			return;

		cFILE	file,textFile;

		if	(!file.Open(_ms("data/scenario/red stone/%s",lpstrFileName),"rb"))
		{
			cMSG::Put("Error adding item!","File not found.");
			return;
		}

		textFile.Open(_ms("data/scenario/red stone/%s",lpstrFileName),"rb");

		int		iItemCount,iTextLocate,iBufferSize=2048,i;
		char	*pBuffer	=	new char [iBufferSize];

		file.Read(&iItemCount,4);
		file.Read(&iTextLocate,4);

		for	 (i=0;i<iItemCount;i++)
		{
			cBASIC_ITEM	basicItem;

			file.Read(&basicItem,sizeof(CBasicItemData));

			if	(cBASIC_ITEM::Get(basicItem.m_strName))
			{
				cMSG::Put("Error adding item!","An item with the same name already exists.\n\n%s",basicItem.m_strName);

				return;
			}
		}

		file.Seek(8,SEEK_SET);

		cBASIC_ITEM::Arrange();

		for	 (i=0;i<iItemCount;i++)
		{
			cBASIC_ITEM	basicItem;

			file.Read(&basicItem,sizeof(CBasicItemData));

			{
				int	iTextSize;

				textFile.Seek(basicItem.m_dwCommentAddress+iTextLocate,SEEK_SET);
				textFile.Read(&iTextSize,4);

				if	(iTextSize	> iBufferSize)
				{
					iBufferSize	=	iTextSize;

					pKILL(pBuffer);

					pBuffer		=	new char [iBufferSize];
				}

				textFile.Read(pBuffer,iTextSize);
				basicItem.setComment(pBuffer);
			}

			if	(cBASIC_ITEM::s_iCount	>=	dBASIC_ITEM_COUNT)
			{
				cMSG::Put("Error adding item!","The number of items exceeded the maximum.");

				return;
			}

			if	(cBASIC_ITEM::Add(&basicItem)	==	dVALID_ITEM)
			{
				cMSG::Put("Error adding item!","Item data is corrupted.");

				return;
			}
		}

		cMSG::Put("Item addition complete!","%d items were added.",iItemCount);

		return;
	}
}

LRESULT CALLBACK
cCommandDlg::ProcDlg(HWND _hWnd, UINT _uMsg, WPARAM _wParam, LPARAM _lParam)
{
	switch(_uMsg)
	{	
		case WM_INITDIALOG	:
			InitDlg(_hWnd);
			break;

		case WM_CLOSE		:
			EndDialog(_hWnd,IDCANCEL);
			return TRUE;

		case	WM_COMMAND	:
			switch( LOWORD( _wParam ) )
            {
				case	IDOK					:
					Launch(_hWnd);
					break;

				case	IDCANCEL				:
					EndDialog(_hWnd,IDCANCEL);
					break;
			}
			break;
	}

	return (0L);
}

void
cCommandDlg::ExportItemData(int _iBegin,int _iEnd,char *_lpstrFN)
{
}

void
cCommandDlg::ImportItemData(char *_lpstrFN)
{
}

