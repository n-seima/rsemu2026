# Microsoft Developer Studio Project File - Name="RedStone_USER_DBServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=RedStone_USER_DBServer - Win32 forUsa
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RedStone_USER_DBServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RedStone_USER_DBServer.mak" CFG="RedStone_USER_DBServer - Win32 forUsa"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RedStone_USER_DBServer - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "RedStone_USER_DBServer - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "RedStone_USER_DBServer - Win32 DebugForChina" (based on "Win32 (x86) Console Application")
!MESSAGE "RedStone_USER_DBServer - Win32 ReleaseForChina" (based on "Win32 (x86) Console Application")
!MESSAGE "RedStone_USER_DBServer - Win32 Debug FOR JAPAN" (based on "Win32 (x86) Console Application")
!MESSAGE "RedStone_USER_DBServer - Win32 forThai" (based on "Win32 (x86) Console Application")
!MESSAGE "RedStone_USER_DBServer - Win32 forUsa" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/RedStone/RedStone_USER_DBServer", BGAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RedStone_USER_DBServer - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../Common" /I "../packet" /I "./GAME" /I "../../Common/Common" /I "../../common/packet" /D "_FOR_KOREA" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "__USE_ZLIB" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /debugtype:coff /machine:I386

!ELSEIF  "$(CFG)" == "RedStone_USER_DBServer - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../Common/Common" /I "../../common/packet" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__USE_ZLIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 zdll.lib ws2_32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /pdb:"Z:\540_Server_JPN\RedStone_User_DBServer\UserDB_ServerD.pdb" /map:"Z:\540_Server_JPN\RedStone_User_DBServer\UserDB_ServerD.map" /debug /machine:I386 /out:"Z:\540_Server_JPN\RedStone_User_DBServer\UserDB_ServerD.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RedStone_USER_DBServer - Win32 DebugForChina"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RedStone_USER_DBServer___Win32_DebugForChina"
# PROP BASE Intermediate_Dir "RedStone_USER_DBServer___Win32_DebugForChina"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "RedStone_USER_DBServer___Win32_DebugForChina"
# PROP Intermediate_Dir "RedStone_USER_DBServer___Win32_DebugForChina"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../Common" /I "../packet" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__USE_ZLIB" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../Common" /I "../packet" /I "../../Common/Common" /I "../../common/packet" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__USE_ZLIB" /D "_FOR_CHINA" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zdll.lib ws2_32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 zdll.lib ws2_32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"RedStone_USER_DBServer(china).exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "RedStone_USER_DBServer - Win32 ReleaseForChina"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "RedStone_USER_DBServer___Win32_ReleaseForChina"
# PROP BASE Intermediate_Dir "RedStone_USER_DBServer___Win32_ReleaseForChina"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "RedStone_USER_DBServer___Win32_ReleaseForChina"
# PROP Intermediate_Dir "RedStone_USER_DBServer___Win32_ReleaseForChina"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "../Common" /I "../packet" /I "./GAME" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "__USE_ZLIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../Common" /I "../packet" /I "./GAME" /I "../../Common/Common" /I "../../common/packet" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "__USE_ZLIB" /D "_FOR_CHINA" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /debugtype:coff /machine:I386
# ADD LINK32 zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /debugtype:coff /machine:I386

!ELSEIF  "$(CFG)" == "RedStone_USER_DBServer - Win32 Debug FOR JAPAN"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RedStone_USER_DBServer___Win32_Debug_FOR_JAPAN"
# PROP BASE Intermediate_Dir "RedStone_USER_DBServer___Win32_Debug_FOR_JAPAN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "RedStone_USER_DBServer___Win32_Debug_FOR_JAPAN"
# PROP Intermediate_Dir "RedStone_USER_DBServer___Win32_Debug_FOR_JAPAN"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../Common" /I "../packet" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__USE_ZLIB" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../Common" /I "../packet" /I "../../Common/Common" /I "../../common/packet" /D "_FOR_JAPAN" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__USE_ZLIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zdll.lib ws2_32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 zdll.lib ws2_32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /pdb:"Z:\540_Server_JPN\RedStone_User_DBServer\RedStone_USER_DBServer(Japan).pdb" /map:"Z:\540_Server_JPN\RedStone_User_DBServer\RedStone_USER_DBServer(Japan).map" /debug /machine:I386 /out:"Z:\540_Server_JPN\RedStone_User_DBServer\RedStone_USER_DBServer(Japan).exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RedStone_USER_DBServer - Win32 forThai"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RedStone_USER_DBServer___Win32_forThai"
# PROP BASE Intermediate_Dir "RedStone_USER_DBServer___Win32_forThai"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "forThai"
# PROP Intermediate_Dir "forThai"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../Common" /I "../packet" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__USE_ZLIB" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../Common" /I "../packet" /I "../../Common/Common" /I "../../common/packet" /D "_FOR_THAI" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__USE_ZLIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x41e /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zdll.lib ws2_32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 zdll.lib ws2_32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"RedStone_USER_DBServer(forThai).exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "RedStone_USER_DBServer - Win32 forUsa"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RedStone_USER_DBServer___Win32_forUsa"
# PROP BASE Intermediate_Dir "RedStone_USER_DBServer___Win32_forUsa"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "forUsa"
# PROP Intermediate_Dir "forUsa"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../Common" /I "../packet" /I "../../Common/Common" /I "../../common/packet" /D "_FOR_USA" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__USE_ZLIB" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../Common" /I "../packet" /I "../../Common/Common" /I "../../common/packet" /D "_FOR_USA" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__USE_ZLIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zdll.lib ws2_32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"RedStone_USER_DBServer(D_USA).exe" /pdbtype:sept
# ADD LINK32 zdll.lib ws2_32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"USER_DBServer(D_USA).exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "RedStone_USER_DBServer - Win32 Release"
# Name "RedStone_USER_DBServer - Win32 Debug"
# Name "RedStone_USER_DBServer - Win32 DebugForChina"
# Name "RedStone_USER_DBServer - Win32 ReleaseForChina"
# Name "RedStone_USER_DBServer - Win32 Debug FOR JAPAN"
# Name "RedStone_USER_DBServer - Win32 forThai"
# Name "RedStone_USER_DBServer - Win32 forUsa"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\common\common\adCommandDefine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cADO_BASE.cpp
# End Source File
# Begin Source File

SOURCE=.\cADO_COMMAND.cpp
# End Source File
# Begin Source File

SOURCE=.\cADO_USER.cpp
# End Source File
# Begin Source File

SOURCE=.\Client.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cScript.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cServerStatus.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cSRVUTIL.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\HardwareInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Server.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\SFC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\Tracer.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\buildVersion.h
# End Source File
# Begin Source File

SOURCE=..\Common\cADO_BASE.h
# End Source File
# Begin Source File

SOURCE=.\cADO_COMMAND.h
# End Source File
# Begin Source File

SOURCE=.\cADO_USER.h
# End Source File
# Begin Source File

SOURCE=.\Client.h
# End Source File
# Begin Source File

SOURCE=.\common.h
# End Source File
# Begin Source File

SOURCE=..\common\cServerStatus.h
# End Source File
# Begin Source File

SOURCE=..\packet\definePacketData.h
# End Source File
# Begin Source File

SOURCE=..\common\HardwareInfo.h
# End Source File
# Begin Source File

SOURCE=..\common\Msjexhnd.h
# End Source File
# Begin Source File

SOURCE=..\common\Tracer.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "이벤트"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\event.cpp
# End Source File
# Begin Source File

SOURCE=.\event.h
# End Source File
# End Group
# Begin Group "lib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\zdll.lib

!IF  "$(CFG)" == "RedStone_USER_DBServer - Win32 Release"

!ELSEIF  "$(CFG)" == "RedStone_USER_DBServer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_USER_DBServer - Win32 DebugForChina"

!ELSEIF  "$(CFG)" == "RedStone_USER_DBServer - Win32 ReleaseForChina"

!ELSEIF  "$(CFG)" == "RedStone_USER_DBServer - Win32 Debug FOR JAPAN"

!ELSEIF  "$(CFG)" == "RedStone_USER_DBServer - Win32 forThai"

!ELSEIF  "$(CFG)" == "RedStone_USER_DBServer - Win32 forUsa"

!ENDIF 

# End Source File
# End Group
# Begin Group "common"

# PROP Default_Filter ""
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
# End Group
# Begin Group "버젼"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\common\reg_common.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\server_type_define.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\version.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\common\common\actorDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\adCommandDefine.h
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

SOURCE=..\..\common\common\buildVersion.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cADO_BASE.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\carrotShopDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CGuildDefine.h
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

SOURCE=..\..\common\common\cPLAYER_DATA.h
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

SOURCE=..\..\common\common\cSOCKET.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cSRVUTIL.h
# End Source File
# Begin Source File

SOURCE=..\..\common\packet\definePacketData.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\doorDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\eventDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\HardwareInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\inputConsoleCommand.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\inputConsoleCommand.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\jobDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\life_code_common.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\miniPetCommon.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\PacketBuffer.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\partyDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\playerJobCommon.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\reg_common.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\RSComm.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\server_type_define.h
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

SOURCE=..\..\common\common\Tracer.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\userCustomDataDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\zconf.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\zlib.h
# End Source File
# End Group
# Begin Group "패킷"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\packet\cPACKET_BASE.h
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

SOURCE=..\..\common\packet\cPACKET_USERDBSERVER.h
# End Source File
# Begin Source File

SOURCE=..\..\common\packet\cPACKET_WORLDSERVER.h
# End Source File
# End Group
# Begin Source File

SOURCE=".\config-userdb.txt"
# End Source File
# End Target
# End Project
