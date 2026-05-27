# Microsoft Developer Studio Project File - Name="AdminTool" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=AdminTool - Win32 forUsa
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AdminTool.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AdminTool.mak" CFG="AdminTool - Win32 forUsa"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AdminTool - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "AdminTool - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "AdminTool - Win32 DEBUG FOR CHINA" (based on "Win32 (x86) Application")
!MESSAGE "AdminTool - Win32 forThai" (based on "Win32 (x86) Application")
!MESSAGE "AdminTool - Win32 Debug For JapanTest KR Inner" (based on "Win32 (x86) Application")
!MESSAGE "AdminTool - Win32 Debug FOR JAPAN" (based on "Win32 (x86) Application")
!MESSAGE "AdminTool - Win32 forUsa" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/RedStoneServer/AdminTool", ZSAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AdminTool - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /Gm /GX /Zi /O2 /I "../../common/common" /I "../../common/packet" /I "./" /D "_ADMIN_TOOL" /D "_FOR_KOREA" /D "WIN32" /D "_WSPIAPI_COUNTOF" /D "_WINDOWS" /D "_MBCS" /D "__USE_ZLIB" /D "NDEBUG" /FR /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 zdll.lib ws2_32.lib winmm.lib Nafxcw.lib Libcmt.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"Nafxcw.lib Libcmt.lib" /out:"AdminTool_release.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "AdminTool - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../common/common" /I "../../common/packet" /I "./" /D "_ADMIN_TOOL" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "__USE_ZLIB" /D "_WSPIAPI_COUNTOF" /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 zdll.lib ws2_32.lib winmm.lib Nafxcwd.lib Libcmtd.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"Nafxcwd.lib Libcmtd.lib" /out:"AdminTool_debug.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "AdminTool - Win32 DEBUG FOR CHINA"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AdminTool___Win32_DEBUG_FOR_CHINA"
# PROP BASE Intermediate_Dir "AdminTool___Win32_DEBUG_FOR_CHINA"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "debugForChina"
# PROP Intermediate_Dir "debugForChina"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /I "../REDSTONE_GAMESERVER" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "__USE_ZLIB" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../common/common" /I "../../common/packet" /I "./" /D "_ADMIN_TOOL" /D "_FOR_CHINA" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "__USE_ZLIB" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zdll.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /out:"AdminTool20040225.exe" /pdbtype:sept
# ADD LINK32 zdll.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /out:"AdminTool(china).exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "AdminTool - Win32 forThai"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AdminTool___Win32_forThai"
# PROP BASE Intermediate_Dir "AdminTool___Win32_forThai"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "forThai"
# PROP Intermediate_Dir "forThai"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /I "../REDSTONE_GAMESERVER" /D "_ADMIN_TOOL" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "__USE_ZLIB" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../common/common" /I "../../common/packet" /I "./" /D "_ADMIN_TOOL" /D "_FOR_THAI" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "__USE_ZLIB" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x41e /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zdll.lib ws2_32.lib winmm.lib Nafxcwd.lib Libcmtd.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"Nafxcwd.lib Libcmtd.lib" /out:"AdminTool20060707.exe" /pdbtype:sept
# ADD LINK32 zdll.lib ws2_32.lib winmm.lib Nafxcwd.lib Libcmtd.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"Nafxcwd.lib Libcmtd.lib" /out:"AdminTool(forThai).exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "AdminTool - Win32 Debug For JapanTest KR Inner"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AdminTool___Win32_Debug_For_JapanTest_KR_Inner"
# PROP BASE Intermediate_Dir "AdminTool___Win32_Debug_For_JapanTest_KR_Inner"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "AdminTool___Win32_Debug_For_JapanTest_KR_Inner"
# PROP Intermediate_Dir "AdminTool___Win32_Debug_For_JapanTest_KR_Inner"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /I "../REDSTONE_GAMESERVER" /D "_ADMIN_TOOL" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "__USE_ZLIB" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../common/common" /I "../../common/packet" /I "./" /D "_ADMIN_TOOL" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "__USE_ZLIB" /D "__JPTEST_FOR_KR_INNER" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zdll.lib ws2_32.lib winmm.lib Nafxcwd.lib Libcmtd.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"Nafxcwd.lib Libcmtd.lib" /out:"AdminTool20060707.exe" /pdbtype:sept
# ADD LINK32 zdll.lib ws2_32.lib winmm.lib Nafxcwd.lib Libcmtd.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"Nafxcwd.lib Libcmtd.lib" /out:"AdminTool20060707.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "AdminTool - Win32 Debug FOR JAPAN"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AdminTool___Win32_Debug_FOR_JAPAN1"
# PROP BASE Intermediate_Dir "AdminTool___Win32_Debug_FOR_JAPAN1"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "AdminTool___Win32_Debug_FOR_JAPAN1"
# PROP Intermediate_Dir "AdminTool___Win32_Debug_FOR_JAPAN1"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /I "../REDSTONE_GAMESERVER" /D "_ADMIN_TOOL" /D "_FOR_JAPAN" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "__USE_ZLIB" /FR /FD /GZ /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../common/common" /I "../../common/packet" /I "./" /D "_ADMIN_TOOL" /D "_FOR_JAPAN" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "__USE_ZLIB" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Nafxcwd.lib Libcmtd.lib zdll.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"Nafxcwd.lib Libcmtd.lib" /out:"AdminTool(Japan).exe" /pdbtype:sept
# ADD LINK32 Nafxcwd.lib Libcmtd.lib zdll.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /pdb:"C:\MyDev\540_Server\AdminTool\AdminTool(Japan).pdb" /map:"C:\MyDev\540_Server\AdminTool\AdminTool(Japan).map" /debug /machine:I386 /nodefaultlib:"Nafxcwd.lib Libcmtd.lib" /out:"C:\MyDev\540_Server\AdminTool\AdminTool(Japan).exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "AdminTool - Win32 forUsa"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AdminTool___Win32_forUsa"
# PROP BASE Intermediate_Dir "AdminTool___Win32_forUsa"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "forUsa"
# PROP Intermediate_Dir "forUsa"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../common/common" /I "../../common/packet" /I "./" /D "_ADMIN_TOOL" /D "_FOR_USA" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "__USE_ZLIB" /D "_WSPIAPI_COUNTOF" /FR /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../common/common" /I "../../common/packet" /I "./" /D "_ADMIN_TOOL" /D "_FOR_USA" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "__USE_ZLIB" /D "_WSPIAPI_COUNTOF" /FR /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Nafxcwd.lib Libcmtd.lib zdll.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"Nafxcwd.lib Libcmtd.lib" /out:"AdminTool(D_USA).exe" /pdbtype:sept
# ADD LINK32 Nafxcwd.lib Libcmtd.lib zdll.lib ws2_32.lib winmm.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"Nafxcwd.lib Libcmtd.lib" /out:"AdminTool(D_USA).exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "AdminTool - Win32 Release"
# Name "AdminTool - Win32 Debug"
# Name "AdminTool - Win32 DEBUG FOR CHINA"
# Name "AdminTool - Win32 forThai"
# Name "AdminTool - Win32 Debug For JapanTest KR Inner"
# Name "AdminTool - Win32 Debug FOR JAPAN"
# Name "AdminTool - Win32 forUsa"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\common\common\adCommandDefine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\adCommandDefine.h
# End Source File
# Begin Source File

SOURCE=.\AdminCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminLogCode.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminTool.cpp
# End Source File
# Begin Source File

SOURCE=.\AdminToolDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AGEGRAPH.cpp
# End Source File
# Begin Source File

SOURCE=.\cADO_ADMIN.cpp
# End Source File
# Begin Source File

SOURCE=.\cADO_ADMIN_FOR_ACCOUNT.cpp
# End Source File
# Begin Source File

SOURCE=.\cADO_ADMIN_FOR_AVATAR.cpp
# End Source File
# Begin Source File

SOURCE=.\cADO_ADMIN_FOR_COMMAND.cpp
# End Source File
# Begin Source File

SOURCE=.\cADO_ADMIN_FOR_EVENT.cpp
# End Source File
# Begin Source File

SOURCE=.\cADO_ADMIN_FOR_GUILD.cpp
# End Source File
# Begin Source File

SOURCE=.\cADO_ADMIN_FOR_OPERATOR.cpp
# End Source File
# Begin Source File

SOURCE=.\cADO_ADMIN_FOR_PVP.cpp
# End Source File
# Begin Source File

SOURCE=.\cADO_ADMIN_FOR_STATISTICS.cpp
# End Source File
# Begin Source File

SOURCE=.\cADO_ADMIN_FOR_TEMPWORK.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cADO_BASE.cpp
# End Source File
# Begin Source File

SOURCE=.\CBmpMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\cDlgChangeGMaster.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\EventTimeManager.cpp
# End Source File
# Begin Source File

SOURCE=.\LOGIN.cpp
# End Source File
# Begin Source File

SOURCE=.\LottoEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\MSGDLG.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\Msjexhnd.cpp
# End Source File
# Begin Source File

SOURCE=.\NEWNAME.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\PacketBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\PAGE_NOTICE.cpp
# End Source File
# Begin Source File

SOURCE=.\PAGE_PVPTOOL.cpp
# End Source File
# Begin Source File

SOURCE=.\PAGE_SUM.cpp
# End Source File
# Begin Source File

SOURCE=.\PAGE_USERSETTING.cpp
# End Source File
# Begin Source File

SOURCE=.\PollDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SEARCHPAGE.cpp
# End Source File
# Begin Source File

SOURCE=.\setItem.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TentativeGuildManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\Tracer.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldScore.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AdminCommand.h
# End Source File
# Begin Source File

SOURCE=.\AdminLogCode.h
# End Source File
# Begin Source File

SOURCE=.\AdminTool.h
# End Source File
# Begin Source File

SOURCE=.\AdminToolDlg.h
# End Source File
# Begin Source File

SOURCE=.\AGEGRAPH.h
# End Source File
# Begin Source File

SOURCE=.\buildVersion.h
# End Source File
# Begin Source File

SOURCE=.\cADO_ADMIN.h
# End Source File
# Begin Source File

SOURCE=.\CBmpMenu.h
# End Source File
# Begin Source File

SOURCE=.\cDlgChangeGMaster.h
# End Source File
# Begin Source File

SOURCE=.\cListManage.h
# End Source File
# Begin Source File

SOURCE=.\CMessage.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CSkill.h
# End Source File
# Begin Source File

SOURCE=.\EventTimeManager.h
# End Source File
# Begin Source File

SOURCE=.\LOGIN.h
# End Source File
# Begin Source File

SOURCE=.\LottoEvent.h
# End Source File
# Begin Source File

SOURCE=.\MSGDLG.h
# End Source File
# Begin Source File

SOURCE=.\NEWNAME.h
# End Source File
# Begin Source File

SOURCE=.\PAGE_NOTICE.h
# End Source File
# Begin Source File

SOURCE=.\PAGE_PVPTOOL.h
# End Source File
# Begin Source File

SOURCE=.\PAGE_SUM.h
# End Source File
# Begin Source File

SOURCE=.\PAGE_USERSETTING.h
# End Source File
# Begin Source File

SOURCE=.\PollDlg.h
# End Source File
# Begin Source File

SOURCE=.\SEARCHPAGE.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TentativeGuildManager.h
# End Source File
# Begin Source File

SOURCE=.\threadFunc.h
# End Source File
# Begin Source File

SOURCE=.\WorldScore.h
# End Source File
# Begin Source File

SOURCE=.\zconf.h
# End Source File
# Begin Source File

SOURCE=.\zlib.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\AdminTool.ico
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\graph.ico
# End Source File
# Begin Source File

SOURCE=".\res\red stone.ico"
# End Source File
# End Group
# Begin Group "Add Code"

# PROP Default_Filter ""
# Begin Group "Item Pack List"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ItemPackList.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemPackList.h
# End Source File
# End Group
# Begin Group "Carrot Shop"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\carrotShop.cpp
# End Source File
# Begin Source File

SOURCE=.\carrotShop.h
# End Source File
# End Group
# Begin Group "quest"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\quest.cpp
# End Source File
# Begin Source File

SOURCE=.\quest.h
# End Source File
# Begin Source File

SOURCE=.\questList.cpp
# End Source File
# Begin Source File

SOURCE=.\questList.h
# End Source File
# End Group
# Begin Group "Event Manager"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\common\eventDefine.cpp
# End Source File
# Begin Source File

SOURCE=.\EventManager.cpp
# End Source File
# Begin Source File

SOURCE=.\EventManager.h
# End Source File
# End Group
# Begin Group "Base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cDECODER.cpp
# End Source File
# Begin Source File

SOURCE=.\cDECODER.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\SFC.cpp
# End Source File
# Begin Source File

SOURCE=.\windowExtendAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\windowExtendAPI.h
# End Source File
# End Group
# Begin Group "server tool"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rookieEvent090721.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerTool.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerTool.h
# End Source File
# Begin Source File

SOURCE=.\serverTool050720.cpp
# End Source File
# Begin Source File

SOURCE=.\serverTool050818.cpp
# End Source File
# Begin Source File

SOURCE=.\serverToolOld.cpp
# End Source File
# End Group
# Begin Group "Inventory"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\cInventory.h
# End Source File
# End Group
# Begin Group "ftp class"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ftpclass\blockingsocket.cpp
# End Source File
# Begin Source File

SOURCE=.\ftpclass\blockingsocket.h
# End Source File
# Begin Source File

SOURCE=.\CFTPclient.cpp
# End Source File
# Begin Source File

SOURCE=.\CFTPclient.h
# End Source File
# Begin Source File

SOURCE=.\ftpclass\definements.h
# End Source File
# Begin Source File

SOURCE=.\ftpclass\ftpdatatypes.cpp
# End Source File
# Begin Source File

SOURCE=.\ftpclass\ftpdatatypes.h
# End Source File
# Begin Source File

SOURCE=.\ftpclass\ftpfilestate.cpp
# End Source File
# Begin Source File

SOURCE=.\ftpclass\ftpfilestate.h
# End Source File
# Begin Source File

SOURCE=.\ftpclass\ftplistparse.cpp
# End Source File
# Begin Source File

SOURCE=.\ftpclass\ftplistparse.h
# End Source File
# Begin Source File

SOURCE=.\ftpclass\rc_smart_ptr.h
# End Source File
# End Group
# Begin Group "Guild"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\common\guild_common.cpp
# End Source File
# Begin Source File

SOURCE=.\guildInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\guildInfo.h
# End Source File
# Begin Source File

SOURCE=.\GuildInventory.cpp
# End Source File
# Begin Source File

SOURCE=.\GuildInventory.h
# End Source File
# Begin Source File

SOURCE=.\GuildPetAndGuardian.cpp
# End Source File
# Begin Source File

SOURCE=.\GuildPetAndGuardian.h
# End Source File
# End Group
# Begin Group "미니펫 리스트"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cMiniPetList.cpp
# End Source File
# Begin Source File

SOURCE=.\cMiniPetList.h
# End Source File
# End Group
# Begin Group "펫 리스트"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cPetList.cpp
# End Source File
# Begin Source File

SOURCE=.\cPetList.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\book.cpp
# End Source File
# Begin Source File

SOURCE=.\book.h
# End Source File
# Begin Source File

SOURCE=.\CCharacterData.cpp
# End Source File
# Begin Source File

SOURCE=.\CharacterInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CharacterInfo.h
# End Source File
# Begin Source File

SOURCE=.\CMessage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cPetDefine.cpp
# End Source File
# Begin Source File

SOURCE=.\CTitle.cpp
# End Source File
# Begin Source File

SOURCE=.\CTitle.h
# End Source File
# Begin Source File

SOURCE=.\ExpCalc.cpp
# End Source File
# Begin Source File

SOURCE=.\ExpCalc.h
# End Source File
# Begin Source File

SOURCE=.\IPCurrentStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\IPCurrentStatus.h
# End Source File
# Begin Source File

SOURCE=.\playerData.cpp
# End Source File
# Begin Source File

SOURCE=.\RenewItemExpireDate.cpp
# End Source File
# Begin Source File

SOURCE=.\RenewItemExpireDate.h
# End Source File
# Begin Source File

SOURCE=.\skill.cpp
# End Source File
# Begin Source File

SOURCE=.\skill.h
# End Source File
# End Group
# Begin Group "resource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AdminTool.rc
# End Source File
# Begin Source File

SOURCE=.\res\AdminTool.rc2
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# End Group
# Begin Group "lib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\zdll.lib

!IF  "$(CFG)" == "AdminTool - Win32 Release"

!ELSEIF  "$(CFG)" == "AdminTool - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "AdminTool - Win32 DEBUG FOR CHINA"

!ELSEIF  "$(CFG)" == "AdminTool - Win32 forThai"

!ELSEIF  "$(CFG)" == "AdminTool - Win32 Debug For JapanTest KR Inner"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "AdminTool - Win32 Debug FOR JAPAN"

!ELSEIF  "$(CFG)" == "AdminTool - Win32 forUsa"

!ENDIF 

# End Source File
# End Group
# Begin Group "패킷"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\packet\cPACKET_BASE.h
# End Source File
# Begin Source File

SOURCE=..\..\common\packet\cPACKET_CONTROLSERVER.h
# End Source File
# Begin Source File

SOURCE=..\..\common\packet\cPACKET_DBSERVER.h
# End Source File
# Begin Source File

SOURCE=..\..\common\packet\cPACKET_GAMESERVER.h
# End Source File
# Begin Source File

SOURCE=..\..\common\packet\cPACKET_LOGINSERVER.h
# End Source File
# Begin Source File

SOURCE=..\..\common\packet\cPACKET_LOGSERVER.h
# End Source File
# Begin Source File

SOURCE=..\..\common\packet\cPACKET_TOOLLOGSERVER.h
# End Source File
# Begin Source File

SOURCE=..\..\common\packet\cPACKET_USERDBSERVER.h
# End Source File
# Begin Source File

SOURCE=..\..\common\packet\cPACKET_WORLDSERVER.h
# End Source File
# End Group
# Begin Group "아이템"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cITEM.cpp
# End Source File
# Begin Source File

SOURCE=.\cITEM.h
# End Source File
# Begin Source File

SOURCE=.\cITEM_PREFIX.h
# End Source File
# Begin Source File

SOURCE=.\cMakeItem.cpp
# End Source File
# Begin Source File

SOURCE=.\cMakeItem.h
# End Source File
# End Group
# Begin Group "common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\common\commonDefine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cPLAYER_DATA.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cScript.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cSOCKET.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cSRVUTIL.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkedList.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkedList.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\playerJobCommon.cpp
# End Source File
# End Group
# Begin Group "DLL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\zlib1.dll

!IF  "$(CFG)" == "AdminTool - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "AdminTool - Win32 Debug"

!ELSEIF  "$(CFG)" == "AdminTool - Win32 DEBUG FOR CHINA"

!ELSEIF  "$(CFG)" == "AdminTool - Win32 forThai"

!ELSEIF  "$(CFG)" == "AdminTool - Win32 Debug For JapanTest KR Inner"

!ELSEIF  "$(CFG)" == "AdminTool - Win32 Debug FOR JAPAN"

!ELSEIF  "$(CFG)" == "AdminTool - Win32 forUsa"

!ENDIF 

# End Source File
# End Group
# Begin Group "Searching Avatar DB "

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SelectFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectFunc.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\controltext.txt
# End Source File
# Begin Source File

SOURCE=.\message.txt
# End Source File
# Begin Source File

SOURCE=.\message_eng.txt
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\ReadMe_dev.txt
# End Source File
# Begin Source File

SOURCE=..\ServerDownList.txt
# End Source File
# End Target
# End Project
