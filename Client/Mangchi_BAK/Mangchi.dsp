# Microsoft Developer Studio Project File - Name="Mangchi" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Mangchi - Win32 Convertor
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Mangchi.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Mangchi.mak" CFG="Mangchi - Win32 Convertor"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Mangchi - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Mangchi - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Mangchi - Win32 Convertor" (based on "Win32 (x86) Application")
!MESSAGE "Mangchi - Win32 forJapan" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Mangchi", CVAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Mangchi - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../../common/packet" /I "../../common/common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_KOREA" /FR /YX /FD /c
# SUBTRACT CPP /u
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 comctl32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /nodefaultlib:"libcmt.lib" /out:"../../../../Red Stone dev Client/Mangchi.exe"
# SUBTRACT LINK32 /pdb:none /incremental:yes /nodefaultlib

!ELSEIF  "$(CFG)" == "Mangchi - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../common/packet" /I "../../common/common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_KOREA" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"../../../../Red Stone dev Client/Mangchi.exe" /pdbtype:sept
# SUBTRACT LINK32 /force

!ELSEIF  "$(CFG)" == "Mangchi - Win32 Convertor"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Mangchi___Win32_Convertor"
# PROP BASE Intermediate_Dir "Mangchi___Win32_Convertor"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Convertor"
# PROP Intermediate_Dir "Convertor"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "../server/common" /I "../common" /I "../../LNK Common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../server/common" /I "../common" /I "../../LNK Common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_ISCONVERTOR" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../Red Stone dev Client/Mangchi.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /force
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../Red Stone dev Client/MangchiC.exe" /pdbtype:sept
# SUBTRACT LINK32 /force

!ELSEIF  "$(CFG)" == "Mangchi - Win32 forJapan"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Mangchi___Win32_forJapan"
# PROP BASE Intermediate_Dir "Mangchi___Win32_forJapan"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Mangchi___Win32_forJapan"
# PROP Intermediate_Dir "Mangchi___Win32_forJapan"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "../server/common" /I "../common" /I "../../LNK_Common" /I "../LNK_Common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# SUBTRACT BASE CPP /u
# ADD CPP /nologo /W3 /GX /O2 /I "../../common/packet" /I "../../common/common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /l /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x411 /fo"forJapan/Mangchi.res" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 comctl32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /nodefaultlib:"libcmt.lib" /out:"../Red Stone dev Client/Mangchi.exe"
# SUBTRACT BASE LINK32 /pdb:none /incremental:yes /nodefaultlib
# ADD LINK32 comctl32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /nodefaultlib:"libcmt.lib" /out:"../Red Stone dev Client/MangchiForJapan.exe"
# SUBTRACT LINK32 /pdb:none /incremental:yes /nodefaultlib

!ENDIF 

# Begin Target

# Name "Mangchi - Win32 Release"
# Name "Mangchi - Win32 Debug"
# Name "Mangchi - Win32 Convertor"
# Name "Mangchi - Win32 forJapan"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\arca.cpp
# End Source File
# Begin Source File

SOURCE=.\cACTOR.cpp
# End Source File
# Begin Source File

SOURCE=.\cAREA.cpp
# End Source File
# Begin Source File

SOURCE=.\CBGPlay.cpp
# End Source File
# Begin Source File

SOURCE=.\cBODY.cpp
# End Source File
# Begin Source File

SOURCE=.\cBUILDING.cpp
# End Source File
# Begin Source File

SOURCE=.\cCHARACTER.cpp
# End Source File
# Begin Source File

SOURCE=.\CDecodeUniqueItemData.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CImageData.cpp
# End Source File
# Begin Source File

SOURCE=.\cJOB.cpp
# End Source File
# Begin Source File

SOURCE=.\cLISTCONTROL.cpp
# End Source File
# Begin Source File

SOURCE=.\cMAIN.cpp
# End Source File
# Begin Source File

SOURCE=.\cOBJECT.cpp
# End Source File
# Begin Source File

SOURCE=.\cPROJECT.cpp
# End Source File
# Begin Source File

SOURCE=.\CShop.cpp
# End Source File
# Begin Source File

SOURCE=.\cSKILL.cpp
# End Source File
# Begin Source File

SOURCE=.\CTitle.cpp
# End Source File
# Begin Source File

SOURCE=.\cVALUE.cpp
# End Source File
# Begin Source File

SOURCE=.\debug.cpp
# End Source File
# Begin Source File

SOURCE=.\MANGCHI.cpp
# End Source File
# Begin Source File

SOURCE=.\partrolRoad_mw.cpp
# End Source File
# Begin Source File

SOURCE=.\patrolRoad.cpp
# End Source File
# Begin Source File

SOURCE=.\road_tool_wnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\svn.cpp
# End Source File
# Begin Source File

SOURCE=.\trapSkill.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\arca.h
# End Source File
# Begin Source File

SOURCE=.\cACTOR.h
# End Source File
# Begin Source File

SOURCE=.\cAI.h
# End Source File
# Begin Source File

SOURCE=.\cAREA.h
# End Source File
# Begin Source File

SOURCE=.\CBGPlay.h
# End Source File
# Begin Source File

SOURCE=.\cBODY.h
# End Source File
# Begin Source File

SOURCE=.\cBUILDING.h
# End Source File
# Begin Source File

SOURCE=.\cCHARACTER.h
# End Source File
# Begin Source File

SOURCE=.\CDialog.h
# End Source File
# Begin Source File

SOURCE=.\CImageData.h
# End Source File
# Begin Source File

SOURCE=.\cJOB.h
# End Source File
# Begin Source File

SOURCE=.\cLISTCONTROL.h
# End Source File
# Begin Source File

SOURCE=.\cMAIN.h
# End Source File
# Begin Source File

SOURCE=.\cOBJECT.h
# End Source File
# Begin Source File

SOURCE=.\command.h
# End Source File
# Begin Source File

SOURCE=.\cPROJECT.h
# End Source File
# Begin Source File

SOURCE=.\CShop.h
# End Source File
# Begin Source File

SOURCE=.\cSKILL.h
# End Source File
# Begin Source File

SOURCE=.\CSound.h
# End Source File
# Begin Source File

SOURCE=.\CSoundEffect.h
# End Source File
# Begin Source File

SOURCE=.\CTitle.h
# End Source File
# Begin Source File

SOURCE=.\cVALUE.h
# End Source File
# Begin Source File

SOURCE=.\debug.h
# End Source File
# Begin Source File

SOURCE=.\patrolRoad.h
# End Source File
# Begin Source File

SOURCE=.\STDAFX.H
# End Source File
# Begin Source File

SOURCE=.\svn.h
# End Source File
# Begin Source File

SOURCE=.\trapSkill.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Resource\Cursor.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\cursor_c.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\cursor_h.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\Mangchi.ico
# End Source File
# Begin Source File

SOURCE=.\Resource\Tool.bmp
# End Source File
# End Group
# Begin Group "Karma"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cKARMA.cpp
# End Source File
# Begin Source File

SOURCE=.\cKARMA.h
# End Source File
# Begin Source File

SOURCE=.\cKARMA_ADD.cpp
# End Source File
# Begin Source File

SOURCE=.\cKARMA_REACTION.cpp
# End Source File
# Begin Source File

SOURCE=.\cKARMA_TEXT.cpp
# End Source File
# Begin Source File

SOURCE=.\cKARMA_TRIGGER.cpp
# End Source File
# Begin Source File

SOURCE=.\cKARMA_WND.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CKarmaDefine.h
# End Source File
# Begin Source File

SOURCE=.\CKarmaKeyword.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\eventDefine.cpp
# End Source File
# End Group
# Begin Group "Global Message"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cMESSAGE.cpp
# End Source File
# Begin Source File

SOURCE=.\cMESSAGE.h
# End Source File
# End Group
# Begin Group "Base Code"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\c16BITSPRITE.cpp
# End Source File
# Begin Source File

SOURCE=.\c8BITSPRITE.cpp
# End Source File
# Begin Source File

SOURCE=.\cBARMENU.cpp
# End Source File
# Begin Source File

SOURCE=.\cBARMENU.h
# End Source File
# Begin Source File

SOURCE=.\cBMP.cpp
# End Source File
# Begin Source File

SOURCE=.\cDECODER.cpp
# End Source File
# Begin Source File

SOURCE=.\cDECODER.h
# End Source File
# Begin Source File

SOURCE=.\cDIB.cpp
# End Source File
# Begin Source File

SOURCE=.\cDIB.h
# End Source File
# Begin Source File

SOURCE=.\cDRAW.cpp
# End Source File
# Begin Source File

SOURCE=.\cDRAW.h
# End Source File
# Begin Source File

SOURCE=.\cIMAGE.cpp
# End Source File
# Begin Source File

SOURCE=.\cIMAGE.h
# End Source File
# Begin Source File

SOURCE=.\cPCX.cpp
# End Source File
# Begin Source File

SOURCE=.\cTEXT.cpp
# End Source File
# Begin Source File

SOURCE=.\cTEXT.h
# End Source File
# Begin Source File

SOURCE=.\cTGA.cpp
# End Source File
# Begin Source File

SOURCE=.\cTRANS.cpp
# End Source File
# Begin Source File

SOURCE=.\cWINDOW.cpp
# End Source File
# Begin Source File

SOURCE=.\cWINDOW.h
# End Source File
# Begin Source File

SOURCE=.\SFC.cpp
# End Source File
# Begin Source File

SOURCE=.\SFC.h
# End Source File
# End Group
# Begin Group "ToolWnd"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cTOOLWND.cpp
# End Source File
# Begin Source File

SOURCE=.\cTOOLWND.h
# End Source File
# Begin Source File

SOURCE=.\cTOOLWND_AREA.cpp
# End Source File
# Begin Source File

SOURCE=.\cTOOLWND_BUILDING.cpp
# End Source File
# Begin Source File

SOURCE=.\cTOOLWND_CHARACTER.cpp
# End Source File
# Begin Source File

SOURCE=.\cTOOLWND_OBJECT.cpp
# End Source File
# Begin Source File

SOURCE=.\cTOOLWND_TILE.cpp
# End Source File
# End Group
# Begin Group "Setting"

# PROP Default_Filter ""
# Begin Group "Dialog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CSetDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CSetDialog.h
# End Source File
# Begin Source File

SOURCE=.\CSetDialogContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\CSetDialogKarma.cpp
# End Source File
# Begin Source File

SOURCE=.\CSetDialogKarma.h
# End Source File
# Begin Source File

SOURCE=.\CSetDialogList.cpp
# End Source File
# Begin Source File

SOURCE=.\CSetDialogList.h
# End Source File
# Begin Source File

SOURCE=.\CSetDialogMonster.cpp
# End Source File
# Begin Source File

SOURCE=.\CSetDialogMonster.h
# End Source File
# Begin Source File

SOURCE=.\cSetDialogNpcData.cpp
# End Source File
# Begin Source File

SOURCE=.\cSetDialogNpcData.h
# End Source File
# Begin Source File

SOURCE=.\CSetDialogSelectSpeech.cpp
# End Source File
# Begin Source File

SOURCE=.\CSetDialogSpeech.cpp
# End Source File
# Begin Source File

SOURCE=.\CSetDialogSpeech.h
# End Source File
# End Group
# Begin Group "Piece Item"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\makeItemPiece.cpp
# End Source File
# Begin Source File

SOURCE=.\makePieceItem.cpp
# End Source File
# Begin Source File

SOURCE=.\setPieceItem.cpp
# End Source File
# Begin Source File

SOURCE=.\setPieceItem.h
# End Source File
# End Group
# Begin Group "Set Item"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\makeSetItem.cpp
# End Source File
# Begin Source File

SOURCE=.\setSetItem.cpp
# End Source File
# Begin Source File

SOURCE=.\setSetItem.h
# End Source File
# End Group
# Begin Group "Arca"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\makeArca.cpp
# End Source File
# Begin Source File

SOURCE=.\setArca.cpp
# End Source File
# Begin Source File

SOURCE=.\setArca.h
# End Source File
# End Group
# Begin Group "Custom Item"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CSetCustomItem.cpp
# End Source File
# Begin Source File

SOURCE=.\CSetCustomItem.h
# End Source File
# Begin Source File

SOURCE=.\CSetCustomItemList.cpp
# End Source File
# End Group
# Begin Group "Secret Dungeon setting"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\makeSecretDungeon.cpp
# End Source File
# Begin Source File

SOURCE=.\makeSecretDungeonFloor.cpp
# End Source File
# Begin Source File

SOURCE=.\setSecretDungeon.cpp
# End Source File
# Begin Source File

SOURCE=.\setSecretDungeon.h
# End Source File
# End Group
# Begin Group "book setting"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\setBook.cpp
# End Source File
# Begin Source File

SOURCE=.\setBook.h
# End Source File
# End Group
# Begin Group "main quest setting"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\setMainQuest.cpp
# End Source File
# Begin Source File

SOURCE=.\setMainQuest.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\cSET.cpp
# End Source File
# Begin Source File

SOURCE=.\cSET.h
# End Source File
# Begin Source File

SOURCE=.\cSET_ABILITY.cpp
# End Source File
# Begin Source File

SOURCE=.\cSET_AREA.cpp
# End Source File
# Begin Source File

SOURCE=.\cSET_AREA_EDIT.cpp
# End Source File
# Begin Source File

SOURCE=.\cSET_BASIC_ITEM.cpp
# End Source File
# Begin Source File

SOURCE=.\cSET_CHARACTER.cpp
# End Source File
# Begin Source File

SOURCE=.\cSET_FIELD_CHARACTER.cpp
# End Source File
# Begin Source File

SOURCE=.\cSET_FONT.cpp
# End Source File
# Begin Source File

SOURCE=.\cSET_ITEM_PREFIX.cpp
# End Source File
# Begin Source File

SOURCE=.\cSET_JOB.cpp
# End Source File
# Begin Source File

SOURCE=.\cSET_JOB_LIST.cpp
# End Source File
# Begin Source File

SOURCE=.\cSET_MAP.cpp
# End Source File
# Begin Source File

SOURCE=.\cSET_PROJECT.cpp
# End Source File
# Begin Source File

SOURCE=.\cSET_SELECT_JOB.cpp
# End Source File
# Begin Source File

SOURCE=.\cSET_VALUE.cpp
# End Source File
# Begin Source File

SOURCE=.\CSetCharacterEventData.cpp
# End Source File
# Begin Source File

SOURCE=.\CSetCharacterEventTree.cpp
# End Source File
# Begin Source File

SOURCE=.\CSetQuest.cpp
# End Source File
# Begin Source File

SOURCE=.\CSetShop.cpp
# End Source File
# Begin Source File

SOURCE=.\CSetShop.h
# End Source File
# Begin Source File

SOURCE=.\CSetShopItem.cpp
# End Source File
# Begin Source File

SOURCE=.\CSetShopList.cpp
# End Source File
# Begin Source File

SOURCE=.\setMakeItem.cpp
# End Source File
# Begin Source File

SOURCE=.\setMakeItem.h
# End Source File
# End Group
# Begin Group "MapWindow"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cMAPWND.cpp
# End Source File
# Begin Source File

SOURCE=.\cMAPWND.h
# End Source File
# Begin Source File

SOURCE=.\cMW_AREA.cpp
# End Source File
# Begin Source File

SOURCE=.\cMW_BUILDING.cpp
# End Source File
# Begin Source File

SOURCE=.\cMW_CHARACTER.cpp
# End Source File
# Begin Source File

SOURCE=.\cMW_COPYORCUT.cpp
# End Source File
# Begin Source File

SOURCE=.\cMW_EDIT.cpp
# End Source File
# Begin Source File

SOURCE=.\cMW_OBJECT.cpp
# End Source File
# Begin Source File

SOURCE=.\cMW_TILE.cpp
# End Source File
# End Group
# Begin Group "Item"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cITEM.cpp
# End Source File
# Begin Source File

SOURCE=.\cITEM.h
# End Source File
# Begin Source File

SOURCE=.\cITEM_EFFECT.cpp
# End Source File
# Begin Source File

SOURCE=.\cITEM_EFFECT.h
# End Source File
# Begin Source File

SOURCE=.\cITEM_PREFIX.cpp
# End Source File
# Begin Source File

SOURCE=.\cITEM_PREFIX.h
# End Source File
# Begin Source File

SOURCE=.\cITEM_PREFIX_EFFECT.cpp
# End Source File
# Begin Source File

SOURCE=.\cITEM_PREFIX_EFFECT.h
# End Source File
# Begin Source File

SOURCE=.\itemPackList.cpp
# End Source File
# Begin Source File

SOURCE=.\itemPackList.h
# End Source File
# Begin Source File

SOURCE=.\pieceItem.cpp
# End Source File
# Begin Source File

SOURCE=.\pieceItem.h
# End Source File
# Begin Source File

SOURCE=.\setItem.cpp
# End Source File
# Begin Source File

SOURCE=.\setItem.h
# End Source File
# End Group
# Begin Group "Pattern"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CPattn.cpp
# End Source File
# Begin Source File

SOURCE=.\CPattn.hpp
# End Source File
# Begin Source File

SOURCE=.\CPattnCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CPattnCtrl.hpp
# End Source File
# Begin Source File

SOURCE=.\CPattnCtrlCliff.cpp
# End Source File
# End Group
# Begin Group "Map"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cMAP.cpp
# End Source File
# Begin Source File

SOURCE=.\cMAP.h
# End Source File
# Begin Source File

SOURCE=.\cMAP_BUILDING.cpp
# End Source File
# Begin Source File

SOURCE=.\cMAP_OBJECT.cpp
# End Source File
# Begin Source File

SOURCE=.\exportMapInfo.cpp
# End Source File
# End Group
# Begin Group "Localize"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CLoadTextCode.cpp
# End Source File
# Begin Source File

SOURCE=.\CLoadTextCode.h
# End Source File
# Begin Source File

SOURCE=.\CLocalize.cpp
# End Source File
# Begin Source File

SOURCE=.\CMapExportText.cpp
# End Source File
# Begin Source File

SOURCE=.\compareText.cpp
# End Source File
# Begin Source File

SOURCE=.\exportText.cpp
# End Source File
# Begin Source File

SOURCE=.\exportTool.cpp
# End Source File
# Begin Source File

SOURCE=.\foceSyncText.cpp
# End Source File
# Begin Source File

SOURCE=.\localize.h
# End Source File
# Begin Source File

SOURCE=.\modifyText.cpp
# End Source File
# End Group
# Begin Group "common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\common\commonClass.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\commonDefine.cpp
# End Source File
# End Group
# Begin Group "book"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\book.cpp
# End Source File
# Begin Source File

SOURCE=.\book.h
# End Source File
# Begin Source File

SOURCE=.\book_category.cpp
# End Source File
# Begin Source File

SOURCE=.\book_category.h
# End Source File
# Begin Source File

SOURCE=..\SERVER\common\book_define.h
# End Source File
# Begin Source File

SOURCE=.\book_info.cpp
# End Source File
# Begin Source File

SOURCE=.\book_info.h
# End Source File
# Begin Source File

SOURCE=.\book_page.cpp
# End Source File
# Begin Source File

SOURCE=.\book_page.h
# End Source File
# End Group
# Begin Group "quest"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cQUEST.cpp
# End Source File
# Begin Source File

SOURCE=.\cQUEST.h
# End Source File
# Begin Source File

SOURCE=.\mainQuest.cpp
# End Source File
# Begin Source File

SOURCE=.\mainQuest.h
# End Source File
# Begin Source File

SOURCE=.\mainQuestChapterInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\mainQuestChapterInfo.h
# End Source File
# End Group
# Begin Group "secret dungeon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\secretDungeon.cpp
# End Source File
# Begin Source File

SOURCE=.\secretDungeon.h
# End Source File
# End Group
# Begin Group "LNK_Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=LNK_Common\error_code.cpp
# End Source File
# Begin Source File

SOURCE=LNK_Common\error_code.h
# End Source File
# Begin Source File

SOURCE=LNK_Common\exceptionhandler.cpp
# End Source File
# Begin Source File

SOURCE=LNK_Common\exceptionhandler.h
# End Source File
# Begin Source File

SOURCE=LNK_Common\file.cpp
# End Source File
# Begin Source File

SOURCE=LNK_Common\file.h
# End Source File
# Begin Source File

SOURCE=LNK_Common\lnk_common.h
# End Source File
# Begin Source File

SOURCE=LNK_Common\lnk_define.h
# End Source File
# Begin Source File

SOURCE=LNK_Common\lnk_util.cpp
# End Source File
# Begin Source File

SOURCE=LNK_Common\lnk_util.h
# End Source File
# Begin Source File

SOURCE=LNK_Common\mem.h
# End Source File
# Begin Source File

SOURCE=LNK_Common\memory_pool.cpp
# End Source File
# Begin Source File

SOURCE=LNK_Common\memory_pool.h
# End Source File
# Begin Source File

SOURCE=LNK_Common\memory_pool_normal.cpp
# End Source File
# Begin Source File

SOURCE=LNK_Common\memory_pool_safe.cpp
# End Source File
# Begin Source File

SOURCE=LNK_Common\safeMemory.cpp
# End Source File
# Begin Source File

SOURCE=LNK_Common\safeMemory.h
# End Source File
# Begin Source File

SOURCE=LNK_Common\safeMemoryForDebug.cpp
# End Source File
# Begin Source File

SOURCE=LNK_Common\safeMemoryForDebug.h
# End Source File
# Begin Source File

SOURCE=LNK_Common\smart_pointer.h
# End Source File
# Begin Source File

SOURCE=LNK_Common\Timer.cpp
# End Source File
# Begin Source File

SOURCE=LNK_Common\Timer.h
# End Source File
# End Group
# Begin Group "resource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Mangchi.rc
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "character pattern"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\pattern.cpp
# End Source File
# Begin Source File

SOURCE=.\pattern.h
# End Source File
# Begin Source File

SOURCE=.\set_character_pattern.cpp
# End Source File
# Begin Source File

SOURCE=.\set_character_pattern.h
# End Source File
# End Group
# Begin Group "¹Ì´Ï¸Ê"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MinimapWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\MinimapWindow.h
# End Source File
# End Group
# Begin Group "Undo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\URdoBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\URdoBuffer.h
# End Source File
# End Group
# Begin Group "MemoWindow"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cMemoWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\cMemoWindow.h
# End Source File
# End Group
# Begin Group "Tool"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\command.cpp
# End Source File
# End Group
# Begin Group "HelpWindow"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cHelpWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\cHelpWindow.h
# End Source File
# End Group
# Begin Group "CompareBox"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cCompareBox.cpp
# End Source File
# Begin Source File

SOURCE=.\cCompareBox.h
# End Source File
# End Group
# End Target
# End Project
