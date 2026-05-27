# Microsoft Developer Studio Project File - Name="RedStone_BroadCastServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=RedStone_BroadCastServer - Win32 forUsa
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RedStone_BroadCastServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RedStone_BroadCastServer.mak" CFG="RedStone_BroadCastServer - Win32 forUsa"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RedStone_BroadCastServer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "RedStone_BroadCastServer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "RedStone_BroadCastServer - Win32 forThai" (based on "Win32 (x86) Application")
!MESSAGE "RedStone_BroadCastServer - Win32 Debug FOR JAPAN" (based on "Win32 (x86) Application")
!MESSAGE "RedStone_BroadCastServer - Win32 forUsa" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/RedStoneServer/RedStone_BroadCastServer", AVAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RedStone_BroadCastServer - Win32 Release"

# PROP BASE Use_MFC 6
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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../Common/common" /I "../../common/packet" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "RedStone_BroadCastServer - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../Common/common" /I "../../common/packet" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_KOREA" /D "dIS_REDSTONE_ONE" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib ws2_32.lib /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /out:"CommunityServerD.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "RedStone_BroadCastServer - Win32 forThai"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RedStone_BroadCastServer___Win32_forThai"
# PROP BASE Intermediate_Dir "RedStone_BroadCastServer___Win32_forThai"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "forThai"
# PROP Intermediate_Dir "forThai"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_KOREA" /D "dIS_REDSTONE_ONE" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /I "../../Common/common" /I "../../common/packet" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_THAI" /D "dIS_REDSTONE_ONE" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x41e /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib ws2_32.lib /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /out:"RedStone_CommunityServer.exe" /pdbtype:sept
# ADD LINK32 winmm.lib ws2_32.lib /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /out:"RedStone_CommunityServer(forThai).exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "RedStone_BroadCastServer - Win32 Debug FOR JAPAN"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RedStone_BroadCastServer___Win32_Debug_FOR_JAPAN"
# PROP BASE Intermediate_Dir "RedStone_BroadCastServer___Win32_Debug_FOR_JAPAN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "RedStone_BroadCastServer___Win32_Debug_FOR_JAPAN"
# PROP Intermediate_Dir "RedStone_BroadCastServer___Win32_Debug_FOR_JAPAN"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /D "_FOR_JAPAN" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "dIS_REDSTONE_ONE" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../Common/common" /I "../../common/packet" /D "_FOR_JAPAN" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "dIS_REDSTONE_ONE" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib ws2_32.lib /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /out:"RedStone_CommunityServer(forJapan).exe" /pdbtype:sept
# ADD LINK32 winmm.lib ws2_32.lib /nologo /subsystem:windows /pdb:"Z:\540_Server_JPN\COMMUNITY_SERVER\CommunityServer(forJapan).pdb" /map:"Z:\540_Server_JPN\COMMUNITY_SERVER\CommunityServer(forJapan).map" /debug /debugtype:both /machine:I386 /out:"Z:\540_Server_JPN\COMMUNITY_SERVER\CommunityServer(forJapan).exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RedStone_BroadCastServer - Win32 forUsa"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RedStone_BroadCastServer___Win32_forUsa"
# PROP BASE Intermediate_Dir "RedStone_BroadCastServer___Win32_forUsa"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "forUsa"
# PROP Intermediate_Dir "forUsa"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /I "../../Common/common" /I "../../common/packet" /D "_FOR_USA" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "dIS_REDSTONE_ONE" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /I "../../Common/common" /I "../../common/packet" /D "_FOR_USA" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "dIS_REDSTONE_ONE" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib ws2_32.lib /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /out:"RedStone_CommunityServer(D_USA).exe" /pdbtype:sept
# ADD LINK32 winmm.lib ws2_32.lib /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /out:"CommunityServer(D_USA).exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "RedStone_BroadCastServer - Win32 Release"
# Name "RedStone_BroadCastServer - Win32 Debug"
# Name "RedStone_BroadCastServer - Win32 forThai"
# Name "RedStone_BroadCastServer - Win32 Debug FOR JAPAN"
# Name "RedStone_BroadCastServer - Win32 forUsa"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Property Page"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DlgFindUserInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFindUserInfo.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\backup.cpp
# End Source File
# Begin Source File

SOURCE=.\ChattingDataBase.cpp
# End Source File
# Begin Source File

SOURCE=.\ChattingProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\Client.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cServerStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\DebuggingDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\FriendsListUpdater.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\HardwareInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\lnk_util.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessThread.cpp
# End Source File
# Begin Source File

SOURCE=.\RedStone_BroadCastServer.cpp
# End Source File
# Begin Source File

SOURCE=.\RedStone_BroadCastServerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TemporaryListForEntry.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\userCustomDataDefine.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\backup.h
# End Source File
# Begin Source File

SOURCE=.\buildVersion.h
# End Source File
# Begin Source File

SOURCE=.\ChattingDataBase.h
# End Source File
# Begin Source File

SOURCE=.\Client.h
# End Source File
# Begin Source File

SOURCE=.\DebuggingDialog.h
# End Source File
# Begin Source File

SOURCE=.\FriendsListUpdater.h
# End Source File
# Begin Source File

SOURCE=.\RedStone_BroadCastServer.h
# End Source File
# Begin Source File

SOURCE=.\RedStone_BroadCastServerDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TemporaryListForEntry.h
# End Source File
# Begin Source File

SOURCE=.\util.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\RedStone_BroadCastServer.ico
# End Source File
# Begin Source File

SOURCE=.\RedStone_BroadCastServer.rc
# End Source File
# Begin Source File

SOURCE=.\res\RedStone_BroadCastServer.rc2
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\common\actorDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\arcaDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\bannerDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\book_define.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cADO_BASE.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cADO_BASE.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\carrotShopDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CItemDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CItemEffectDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CPetDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CPitchmanShopDefine.h
# End Source File
# Begin Source File

SOURCE=..\common\CPitchmanShopDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cPLAYER_DATA.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cScript.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cScript.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cServerStatus.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CSkillDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cSOCKET.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cSOCKET.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cSRVUTIL.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cSRVUTIL.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\doorDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\eventDefine.h
# End Source File
# Begin Source File

SOURCE=.\exceptionhandler.cpp
# End Source File
# Begin Source File

SOURCE=.\exceptionhandler.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\HardwareInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\jobDefine.h
# End Source File
# Begin Source File

SOURCE=.\lnk_define.h
# End Source File
# Begin Source File

SOURCE=.\lnk_util.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\miniPetCommon.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\PacketBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\PacketBuffer.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\playerJobCommon.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\RSComm.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\SFC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\SFC.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\STACK.H
# End Source File
# Begin Source File

SOURCE=..\..\common\common\summonBeastDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\Tracer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\Tracer.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\userCustomDataDefine.h
# End Source File
# End Group
# Begin Group "PACKET"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\packet\cPACKET_BASE.h
# End Source File
# Begin Source File

SOURCE=..\..\common\packet\cPACKET_BCSERVER.h
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

SOURCE=..\..\common\packet\cPACKET_WORLDSERVER.h
# End Source File
# Begin Source File

SOURCE=..\..\common\packet\definePacketData.h
# End Source File
# End Group
# Begin Group "party"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\party.cpp
# End Source File
# Begin Source File

SOURCE=.\party.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\partyDefine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\partyDefine.h
# End Source File
# End Group
# Begin Group "operate packet"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\operate_packet.cpp
# End Source File
# Begin Source File

SOURCE=.\operate_packet.h
# End Source File
# End Group
# Begin Group "guild"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\common\CGuildDefine.h
# End Source File
# Begin Source File

SOURCE=.\guild.cpp
# End Source File
# Begin Source File

SOURCE=.\guild.h
# End Source File
# End Group
# Begin Group "field"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\field.cpp
# End Source File
# Begin Source File

SOURCE=.\field.h
# End Source File
# End Group
# Begin Group "lib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\zdll.lib

!IF  "$(CFG)" == "RedStone_BroadCastServer - Win32 Release"

!ELSEIF  "$(CFG)" == "RedStone_BroadCastServer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_BroadCastServer - Win32 forThai"

!ELSEIF  "$(CFG)" == "RedStone_BroadCastServer - Win32 Debug FOR JAPAN"

!ELSEIF  "$(CFG)" == "RedStone_BroadCastServer - Win32 forUsa"

!ENDIF 

# End Source File
# End Group
# Begin Group "생존 코드"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\life_code.cpp
# End Source File
# Begin Source File

SOURCE=.\life_code.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\life_code_common.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\life_code_common.h
# End Source File
# End Group
# Begin Group "버젼"

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

SOURCE=".\config-bc.txt"
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
