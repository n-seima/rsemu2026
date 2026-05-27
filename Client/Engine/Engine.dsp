# Microsoft Developer Studio Project File - Name="Engine" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Engine - Win32 forInner
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Engine.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Engine.mak" CFG="Engine - Win32 forInner"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Engine - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Engine - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Engine - Win32 forOperator" (based on "Win32 (x86) Static Library")
!MESSAGE "Engine - Win32 forJapan" (based on "Win32 (x86) Static Library")
!MESSAGE "Engine - Win32 forJapanOperator" (based on "Win32 (x86) Static Library")
!MESSAGE "Engine - Win32 HS" (based on "Win32 (x86) Static Library")
!MESSAGE "Engine - Win32 testForJapan" (based on "Win32 (x86) Static Library")
!MESSAGE "Engine - Win32 forUSAOperator" (based on "Win32 (x86) Static Library")
!MESSAGE "Engine - Win32 forUSA" (based on "Win32 (x86) Static Library")
!MESSAGE "Engine - Win32 forThai" (based on "Win32 (x86) Static Library")
!MESSAGE "Engine - Win32 forThaiOperator" (based on "Win32 (x86) Static Library")
!MESSAGE "Engine - Win32 forInner" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Engine", WFAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Engine - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_FOR_KOREA" /FR /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Engine - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_FOR_KOREA" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Engine - Win32 forOperator"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Engine___Win32_forOperator"
# PROP BASE Intermediate_Dir "Engine___Win32_forOperator"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "forOperator"
# PROP Intermediate_Dir "forOperator"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_FOR_KOREA" /Fr /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Engine - Win32 forJapan"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Engine___Win32_forJapan"
# PROP BASE Intermediate_Dir "Engine___Win32_forJapan"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Engine___Win32_forJapan"
# PROP Intermediate_Dir "Engine___Win32_forJapan"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /c
# ADD CPP /nologo /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_FOR_JAPAN" /FR /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Engine - Win32 forJapanOperator"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Engine___Win32_forJapanOperator"
# PROP BASE Intermediate_Dir "Engine___Win32_forJapanOperator"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Engine___Win32_forJapanOperator"
# PROP Intermediate_Dir "Engine___Win32_forJapanOperator"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_FOR_JAPAN" /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Engine - Win32 HS"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Engine___Win32_HS0"
# PROP BASE Intermediate_Dir "Engine___Win32_HS0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Engine___Win32_HS0"
# PROP Intermediate_Dir "Engine___Win32_HS0"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_FOR_KOREA" /FR /YX /FD /c
# ADD CPP /nologo /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_FOR_KOREA" /FR /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Engine - Win32 testForJapan"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Engine___Win32_testForJapan"
# PROP BASE Intermediate_Dir "Engine___Win32_testForJapan"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Engine___Win32_testForJapan"
# PROP Intermediate_Dir "Engine___Win32_testForJapan"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_FOR_JAPAN" /FR /YX /FD /c
# ADD CPP /nologo /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_FOR_JAPAN" /FR /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Engine - Win32 forUSAOperator"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Engine___Win32_forUSAOperator"
# PROP BASE Intermediate_Dir "Engine___Win32_forUSAOperator"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "forUSAOperator"
# PROP Intermediate_Dir "forUSAOperator"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_FOR_KOREA" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_FOR_USA" /D "_FOR_ENG" /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Engine - Win32 forUSA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Engine___Win32_forUSA"
# PROP BASE Intermediate_Dir "Engine___Win32_forUSA"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "forUSA"
# PROP Intermediate_Dir "forUSA"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_FOR_KOREA" /FR /YX /FD /c
# ADD CPP /nologo /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_FOR_USA" /D "_FOR_ENG" /FR /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Engine - Win32 forThai"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Engine___Win32_forThai"
# PROP BASE Intermediate_Dir "Engine___Win32_forThai"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "forThai"
# PROP Intermediate_Dir "forThai"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_FOR_INTER" /D "_FOR_ENG" /FR /YX /FD /c
# ADD CPP /nologo /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_FOR_THAI" /FR /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x41e /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Engine - Win32 forThaiOperator"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Engine___Win32_forThaiOperator"
# PROP BASE Intermediate_Dir "Engine___Win32_forThaiOperator"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "forThaiOperator"
# PROP Intermediate_Dir "forThaiOperator"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_FOR_INTER" /D "_FOR_ENG" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_FOR_THAI" /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x41e /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Engine - Win32 forInner"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Engine___Win32_forInner"
# PROP BASE Intermediate_Dir "Engine___Win32_forInner"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Engine___Win32_forInner"
# PROP Intermediate_Dir "Engine___Win32_forInner"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_FOR_KOREA" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_FOR_INNER" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_FOR_KOREA" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "Engine - Win32 Release"
# Name "Engine - Win32 Debug"
# Name "Engine - Win32 forOperator"
# Name "Engine - Win32 forJapan"
# Name "Engine - Win32 forJapanOperator"
# Name "Engine - Win32 HS"
# Name "Engine - Win32 testForJapan"
# Name "Engine - Win32 forUSAOperator"
# Name "Engine - Win32 forUSA"
# Name "Engine - Win32 forThai"
# Name "Engine - Win32 forThaiOperator"
# Name "Engine - Win32 forInner"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\bmp.cpp
# End Source File
# Begin Source File

SOURCE=.\c16BITSPRITE555.cpp
# End Source File
# Begin Source File

SOURCE=.\c16BITSPRITE565.cpp
# End Source File
# Begin Source File

SOURCE=.\c8BITSPRITE555.cpp
# End Source File
# Begin Source File

SOURCE=.\c8BITSPRITE565.cpp
# End Source File
# Begin Source File

SOURCE=.\cANGLE.cpp
# End Source File
# Begin Source File

SOURCE=.\cBARMENU.cpp
# End Source File
# Begin Source File

SOURCE=.\cCANVAS.cpp
# End Source File
# Begin Source File

SOURCE=.\CDDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\cDECODER.cpp
# End Source File
# Begin Source File

SOURCE=.\cDIB.cpp
# End Source File
# Begin Source File

SOURCE=.\cDRAW.cpp
# End Source File
# Begin Source File

SOURCE=.\cDROPDOWNLIST.cpp
# End Source File
# Begin Source File

SOURCE=.\CExceptionHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\cIMAGE.cpp
# End Source File
# Begin Source File

SOURCE=.\CImageScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=.\cINPDEV.cpp
# End Source File
# Begin Source File

SOURCE=.\cJPG.cpp
# End Source File
# Begin Source File

SOURCE=.\cLAYER555.cpp
# End Source File
# Begin Source File

SOURCE=.\cLAYER565.cpp
# End Source File
# Begin Source File

SOURCE=.\COGGPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\Console.cpp
# End Source File
# Begin Source File

SOURCE=.\cPCX.cpp
# End Source File
# Begin Source File

SOURCE=.\CPopupMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\cPROFILE.cpp
# End Source File
# Begin Source File

SOURCE=.\cSOCKET.cpp
# End Source File
# Begin Source File

SOURCE=.\CSolidCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\CSound.cpp
# End Source File
# Begin Source File

SOURCE=.\cSPLINE.cpp
# End Source File
# Begin Source File

SOURCE=.\cTALKBOX.cpp
# End Source File
# Begin Source File

SOURCE=.\cTEXT.cpp
# End Source File
# Begin Source File

SOURCE=.\CTextDC.cpp
# End Source File
# Begin Source File

SOURCE=.\CTextList.cpp
# End Source File
# Begin Source File

SOURCE=.\CTextMT.cpp
# End Source File
# Begin Source File

SOURCE=.\cTGA.cpp
# End Source File
# Begin Source File

SOURCE=.\CTooltip.cpp
# End Source File
# Begin Source File

SOURCE=.\cTRANS.cpp
# End Source File
# Begin Source File

SOURCE=.\cWND.cpp
# End Source File
# Begin Source File

SOURCE=.\regular_random.cpp
# End Source File
# Begin Source File

SOURCE=.\SFC.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\cANGLE.h
# End Source File
# Begin Source File

SOURCE=.\cBARMENU.h
# End Source File
# Begin Source File

SOURCE=.\cCANVAS.h
# End Source File
# Begin Source File

SOURCE=.\CDDraw.h
# End Source File
# Begin Source File

SOURCE=.\cDECODER.h
# End Source File
# Begin Source File

SOURCE=.\cDIB.h
# End Source File
# Begin Source File

SOURCE=.\cDRAW.h
# End Source File
# Begin Source File

SOURCE=.\cDROPDOWNLIST.h
# End Source File
# Begin Source File

SOURCE=.\CExceptionHandler.h
# End Source File
# Begin Source File

SOURCE=.\cIMAGE.h
# End Source File
# Begin Source File

SOURCE=.\CImageScrollBar.h
# End Source File
# Begin Source File

SOURCE=.\cINPDEV.h
# End Source File
# Begin Source File

SOURCE=.\cMUSE.h
# End Source File
# Begin Source File

SOURCE=.\CNemo.h
# End Source File
# Begin Source File

SOURCE=.\ogg\codec.h
# End Source File
# Begin Source File

SOURCE=.\COGGPlayer.h
# End Source File
# Begin Source File

SOURCE=.\ogg\config_types.h
# End Source File
# Begin Source File

SOURCE=.\Console.h
# End Source File
# Begin Source File

SOURCE=.\CPopupMenu.h
# End Source File
# Begin Source File

SOURCE=.\cPROFILE.h
# End Source File
# Begin Source File

SOURCE=.\cSOCKET.h
# End Source File
# Begin Source File

SOURCE=.\CSolidCheck.h
# End Source File
# Begin Source File

SOURCE=.\CSound.h
# End Source File
# Begin Source File

SOURCE=.\cSPLINE.h
# End Source File
# Begin Source File

SOURCE=.\cTALKBOX.h
# End Source File
# Begin Source File

SOURCE=.\cTEXT.h
# End Source File
# Begin Source File

SOURCE=.\cTOOLTIP.h
# End Source File
# Begin Source File

SOURCE=.\cTRANS.h
# End Source File
# Begin Source File

SOURCE=.\cWND.h
# End Source File
# Begin Source File

SOURCE=.\ogg\ogg.h
# End Source File
# Begin Source File

SOURCE=.\ogg\os_types.h
# End Source File
# Begin Source File

SOURCE=.\regular_random.h
# End Source File
# Begin Source File

SOURCE=.\SFC.h
# End Source File
# Begin Source File

SOURCE=.\STDAFX.H
# End Source File
# Begin Source File

SOURCE=.\timer.h
# End Source File
# Begin Source File

SOURCE=.\ogg\vorbisenc.h
# End Source File
# Begin Source File

SOURCE=.\ogg\vorbisfile.h
# End Source File
# Begin Source File

SOURCE=.\zconf.h
# End Source File
# Begin Source File

SOURCE=.\zlib.h
# End Source File
# End Group
# End Target
# End Project
