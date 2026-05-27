# Microsoft Developer Studio Project File - Name="Install" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Install - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Install.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Install.mak" CFG="Install - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Install - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Install - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Install - Win32 japan" (based on "Win32 (x86) Application")
!MESSAGE "Install - Win32 forUSA" (based on "Win32 (x86) Application")
!MESSAGE "Install - Win32 forThai" (based on "Win32 (x86) Application")
!MESSAGE "Install - Win32 japanHanGame" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Install", KIAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Install - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /D "_FOR_KOREA" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"../../../../../패치/_SetupRS(core).exe"

!ELSEIF  "$(CFG)" == "Install - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "Install - Win32 japan"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Install___Win32_japan"
# PROP BASE Intermediate_Dir "Install___Win32_japan"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "japan"
# PROP Intermediate_Dir "japan"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "_FOR_KOREA" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "_FOR_JAPAN" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"../패치/SetupRS(core).exe"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"../../../../../Red stone for Japan/patch/_SetupRSforJP(core).exe"

!ELSEIF  "$(CFG)" == "Install - Win32 forUSA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Install___Win32_forUSA"
# PROP BASE Intermediate_Dir "Install___Win32_forUSA"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "forUSA"
# PROP Intermediate_Dir "forUSA"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "_FOR_KOREA" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Zi /O2 /D "_FOR_ENG" /D "_FOR_USA" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"../패치/SetupRS(core).exe"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"../../../Red stone for USA/patch/SetupRS(core).exe"

!ELSEIF  "$(CFG)" == "Install - Win32 forThai"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Install___Win32_forThai"
# PROP BASE Intermediate_Dir "Install___Win32_forThai"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "forThai"
# PROP Intermediate_Dir "forThai"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "_FOR_ENG" /D "_FOR_USA" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "_FOR_THAI" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x41e /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"../Red stone for USA/patch/SetupRS(core).exe"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"../Red stone for Thai/patch/SetupRS(core).exe"

!ELSEIF  "$(CFG)" == "Install - Win32 japanHanGame"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Install___Win32_japanHanGame"
# PROP BASE Intermediate_Dir "Install___Win32_japanHanGame"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "japanHanGame"
# PROP Intermediate_Dir "japanHanGame"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "_FOR_JAPAN" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "_FOR_JAPAN_HANGAME" /D "_FOR_JAPAN" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"../Red stone for Japan/patch/SetupRS(core).exe"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"../../../../../Red stone for Japan/patch/_SetupRSforJPHanGame(core).exe"

!ENDIF 

# Begin Target

# Name "Install - Win32 Release"
# Name "Install - Win32 Debug"
# Name "Install - Win32 japan"
# Name "Install - Win32 forUSA"
# Name "Install - Win32 forThai"
# Name "Install - Win32 japanHanGame"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\_resource.sd.cpp
# End Source File
# Begin Source File

SOURCE=.\install.cpp
# End Source File
# Begin Source File

SOURCE=.\install.rc
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\_resource.sd.h
# End Source File
# Begin Source File

SOURCE=.\main.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\icon.ico
# End Source File
# End Group
# Begin Group "message"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\message.h
# End Source File
# Begin Source File

SOURCE=.\messageForInter.cpp
# End Source File
# Begin Source File

SOURCE=.\messageForJapan.cpp
# End Source File
# Begin Source File

SOURCE=.\messageForKorea.cpp
# End Source File
# Begin Source File

SOURCE=.\messageForThai.cpp
# End Source File
# Begin Source File

SOURCE=.\messageForUSA.cpp
# End Source File
# End Group
# Begin Group "base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bar_menu.cpp
# End Source File
# Begin Source File

SOURCE=.\bar_menu.h
# End Source File
# Begin Source File

SOURCE=.\c16BITSPRITE.cpp
# End Source File
# Begin Source File

SOURCE=.\c8BITSPRITE.cpp
# End Source File
# Begin Source File

SOURCE=.\cBMP.cpp
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

SOURCE=.\cNUX.cpp
# End Source File
# Begin Source File

SOURCE=.\cNUX.h
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

SOURCE=.\image_scroll_bar.cpp
# End Source File
# Begin Source File

SOURCE=.\image_scroll_bar.h
# End Source File
# Begin Source File

SOURCE=.\SFC.cpp
# End Source File
# Begin Source File

SOURCE=.\SFC.h
# End Source File
# End Group
# Begin Group "frame"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\complete.cpp
# End Source File
# Begin Source File

SOURCE=.\complete.h
# End Source File
# Begin Source File

SOURCE=.\installData.cpp
# End Source File
# Begin Source File

SOURCE=.\installData.h
# End Source File
# Begin Source File

SOURCE=.\license.cpp
# End Source File
# Begin Source File

SOURCE=.\license.h
# End Source File
# Begin Source File

SOURCE=.\welcome.cpp
# End Source File
# Begin Source File

SOURCE=.\welcome.h
# End Source File
# End Group
# Begin Group "리소스"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\_resource_jp.sd.cpp

!IF  "$(CFG)" == "Install - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 japan"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 forUSA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 forThai"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 japanHanGame"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\_resource_jp.sd.h

!IF  "$(CFG)" == "Install - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 Debug"

!ELSEIF  "$(CFG)" == "Install - Win32 japan"

!ELSEIF  "$(CFG)" == "Install - Win32 forUSA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 forThai"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 japanHanGame"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\_resource_kor.sd.cpp

!IF  "$(CFG)" == "Install - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 japan"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 forUSA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 forThai"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 japanHanGame"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\_resource_kor.sd.h

!IF  "$(CFG)" == "Install - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 Debug"

!ELSEIF  "$(CFG)" == "Install - Win32 japan"

!ELSEIF  "$(CFG)" == "Install - Win32 forUSA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 forThai"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 japanHanGame"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\_resource_thai.sd.cpp

!IF  "$(CFG)" == "Install - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 japan"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 forUSA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 forThai"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 japanHanGame"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\_resource_thai.sd.h

!IF  "$(CFG)" == "Install - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 Debug"

!ELSEIF  "$(CFG)" == "Install - Win32 japan"

!ELSEIF  "$(CFG)" == "Install - Win32 forUSA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 forThai"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 japanHanGame"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\_resource_USA.sd.cpp

!IF  "$(CFG)" == "Install - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 japan"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 forUSA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 forThai"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 japanHanGame"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\_resource_USA.sd.h

!IF  "$(CFG)" == "Install - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 Debug"

!ELSEIF  "$(CFG)" == "Install - Win32 japan"

!ELSEIF  "$(CFG)" == "Install - Win32 forUSA"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 forThai"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Install - Win32 japanHanGame"

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\STDAFX.H
# End Source File
# End Target
# End Project
