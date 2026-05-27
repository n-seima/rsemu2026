# Microsoft Developer Studio Project File - Name="SMManager" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SMManager - Win32 forUsa
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SMManager.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SMManager.mak" CFG="SMManager - Win32 forUsa"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SMManager - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SMManager - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "SMManager - Win32 Debug FOR CHINA" (based on "Win32 (x86) Application")
!MESSAGE "SMManager - Win32 Debug FOR JAPAN" (based on "Win32 (x86) Application")
!MESSAGE "SMManager - Win32 forThai" (based on "Win32 (x86) Application")
!MESSAGE "SMManager - Win32 forUsa" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/RedStoneServer/SMManager", FDBAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SMManager - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../common" /I "../packet" /I "../../Common/Common" /I "../../common/packet" /D "_FOR_KOREA" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "dDEBUG_MODE" /D "_AFXDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /map /debug /machine:I386 /out:"AvatarDBC_SMManagerR.exe"
# SUBTRACT LINK32 /incremental:yes /nodefaultlib

!ELSEIF  "$(CFG)" == "SMManager - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../Common/Common" /I "../../common/packet" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /pdb:"Z:\540_Server_JPN\AvatarDBC_SMManager\AvatarDBC_SMManagerD.pdb" /map:"Z:\540_Server_JPN\AvatarDBC_SMManager\AvatarDBC_SMManagerD.map" /debug /machine:I386 /out:"Z:\540_Server_JPN\AvatarDBC_SMManager\AvatarDBC_SMManagerD.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "SMManager - Win32 Debug FOR CHINA"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SMManager___Win32_Debug_FOR_CHINA"
# PROP BASE Intermediate_Dir "SMManager___Win32_Debug_FOR_CHINA"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SMManager___Win32_Debug_FOR_CHINA"
# PROP Intermediate_Dir "SMManager___Win32_Debug_FOR_CHINA"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /I "../../Common/Common" /I "../../common/packet" /D "_FOR_CHINA" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"SMManager(china).exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "SMManager - Win32 Debug FOR JAPAN"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SMManager___Win32_Debug_FOR_JAPAN"
# PROP BASE Intermediate_Dir "SMManager___Win32_Debug_FOR_JAPAN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "SMManager___Win32_Debug_FOR_JAPAN"
# PROP Intermediate_Dir "SMManager___Win32_Debug_FOR_JAPAN"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /I "../../Common/Common" /I "../../common/packet" /D "_FOR_JAPAN" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /pdb:"Z:\540_Server_JPN\AvatarDBC_SMManager\SMManager(Japan).pdb" /map:"Z:\540_Server_JPN\AvatarDBC_SMManager\SMManager(Japan).map" /debug /machine:I386 /out:"Z:\540_Server_JPN\AvatarDBC_SMManager\SMManager(Japan).exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "SMManager - Win32 forThai"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SMManager___Win32_forThai"
# PROP BASE Intermediate_Dir "SMManager___Win32_forThai"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "forThai"
# PROP Intermediate_Dir "forThai"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /I "../../Common/Common" /I "../../common/packet" /D "_FOR_THAI" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x41e /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /map /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Nafxcwd.lib Libcmtd.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"Nafxcwd.lib Libcmtd.lib" /out:"SMManager(forThai).exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "SMManager - Win32 forUsa"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SMManager___Win32_forUsa"
# PROP BASE Intermediate_Dir "SMManager___Win32_forUsa"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "forUsa"
# PROP Intermediate_Dir "forUsa"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /I "../../Common/Common" /I "../../common/packet" /D "_FOR_USA" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /I "../../Common/Common" /I "../../common/packet" /D "_FOR_USA" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"SMManager(D_USA).exe" /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"SMManager(D_USA).exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SMManager - Win32 Release"
# Name "SMManager - Win32 Debug"
# Name "SMManager - Win32 Debug FOR CHINA"
# Name "SMManager - Win32 Debug FOR JAPAN"
# Name "SMManager - Win32 forThai"
# Name "SMManager - Win32 forUsa"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CClient.cpp
# End Source File
# Begin Source File

SOURCE=.\CDataManager.cpp
# End Source File
# Begin Source File

SOURCE=..\redstone_dbserver\CPlayerBuff.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CSharedMemory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\SFC.cpp
# End Source File
# Begin Source File

SOURCE=.\SMManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SMManager.rc
# End Source File
# Begin Source File

SOURCE=.\SMManagerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\buildVersion.h
# End Source File
# Begin Source File

SOURCE=.\CDataManager.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SMManager.h
# End Source File
# Begin Source File

SOURCE=.\SMManagerDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\SMManager.ico
# End Source File
# Begin Source File

SOURCE=.\res\SMManager.rc2
# End Source File
# End Group
# Begin Group "¹öÁ¯"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\common\reg_common.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\reg_common.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\server_type_define.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\server_type_define.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\version.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\version.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
