# Microsoft Developer Studio Project File - Name="RedStone_WorldServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=RedStone_WorldServer - Win32 forUsa
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RedStone_WorldServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RedStone_WorldServer.mak" CFG="RedStone_WorldServer - Win32 forUsa"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RedStone_WorldServer - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "RedStone_WorldServer - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "RedStone_WorldServer - Win32 DebugForChina" (based on "Win32 (x86) Console Application")
!MESSAGE "RedStone_WorldServer - Win32 ReleaseForChina" (based on "Win32 (x86) Console Application")
!MESSAGE "RedStone_WorldServer - Win32 Debug FOR JAPAN" (based on "Win32 (x86) Console Application")
!MESSAGE "RedStone_WorldServer - Win32 forThai" (based on "Win32 (x86) Console Application")
!MESSAGE "RedStone_WorldServer - Win32 forInner" (based on "Win32 (x86) Console Application")
!MESSAGE "RedStone_WorldServer - Win32 forUsa" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/RedStone/RedStone_WorldServer", CGAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RedStone_WorldServer - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../Common" /I "../packet" /I "./GAME" /I "../../Common/Common" /I "../../common/packet" /D "_FOR_KOREA" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "__USE_ZLIB" /FR /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /debugtype:coff /machine:I386

!ELSEIF  "$(CFG)" == "RedStone_WorldServer - Win32 Debug"

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
# ADD LINK32 zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /debugtype:both /machine:I386 /out:"WorldServerD.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "RedStone_WorldServer - Win32 DebugForChina"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RedStone_WorldServer___Win32_DebugForChina"
# PROP BASE Intermediate_Dir "RedStone_WorldServer___Win32_DebugForChina"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "RedStone_WorldServer___Win32_DebugForChina"
# PROP Intermediate_Dir "RedStone_WorldServer___Win32_DebugForChina"
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
# ADD BASE LINK32 zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"RedStone_WorldServer.exe" /pdbtype:sept
# ADD LINK32 zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"RedStone_WorldServer(china).exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "RedStone_WorldServer - Win32 ReleaseForChina"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "RedStone_WorldServer___Win32_ReleaseForChina"
# PROP BASE Intermediate_Dir "RedStone_WorldServer___Win32_ReleaseForChina"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "RedStone_WorldServer___Win32_ReleaseForChina"
# PROP Intermediate_Dir "RedStone_WorldServer___Win32_ReleaseForChina"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "../Common" /I "../packet" /I "./GAME" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "__USE_ZLIB" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../Common" /I "../packet" /I "./GAME" /I "../../Common/Common" /I "../../common/packet" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "__USE_ZLIB" /D "_FOR_CHINA" /FR /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /debugtype:coff /machine:I386
# ADD LINK32 zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /debugtype:coff /machine:I386

!ELSEIF  "$(CFG)" == "RedStone_WorldServer - Win32 Debug FOR JAPAN"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RedStone_WorldServer___Win32_Debug_FOR_JAPAN"
# PROP BASE Intermediate_Dir "RedStone_WorldServer___Win32_Debug_FOR_JAPAN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "RedStone_WorldServer___Win32_Debug_FOR_JAPAN"
# PROP Intermediate_Dir "RedStone_WorldServer___Win32_Debug_FOR_JAPAN"
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
# ADD BASE LINK32 zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"RedStone_WorldServer.exe" /pdbtype:sept
# ADD LINK32 zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /pdb:"Z:\540_Server_JPN\RedStone_WorldServer\RedStone_WorldServer(Japan).pdb" /map:"Z:\540_Server_JPN\RedStone_WorldServer\RedStone_WorldServer(Japan).map" /debug /machine:I386 /out:"Z:\540_Server_JPN\RedStone_WorldServer\RedStone_WorldServer(Japan).exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RedStone_WorldServer - Win32 forThai"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RedStone_WorldServer___Win32_forThai"
# PROP BASE Intermediate_Dir "RedStone_WorldServer___Win32_forThai"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "forThai"
# PROP Intermediate_Dir "forThai"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__USE_ZLIB" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /I "../../Common/Common" /I "../../common/packet" /D "_FOR_THAI" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__USE_ZLIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x41e /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /debugtype:both /machine:I386 /out:"RedStone_WorldServer.exe" /pdbtype:sept
# ADD LINK32 zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /debugtype:both /machine:I386 /out:"RedStone_WorldServer(forThai).exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "RedStone_WorldServer - Win32 forInner"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RedStone_WorldServer___Win32_forInner"
# PROP BASE Intermediate_Dir "RedStone_WorldServer___Win32_forInner"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "RedStone_WorldServer___Win32_forInner"
# PROP Intermediate_Dir "RedStone_WorldServer___Win32_forInner"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../Common/Common" /I "../../common/packet" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__USE_ZLIB" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../Common/Common" /I "../../common/packet" /D "_FOR_INNER" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__USE_ZLIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /debugtype:both /machine:I386 /out:"WorldServerD.exe" /pdbtype:sept
# ADD LINK32 zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /debugtype:both /machine:I386 /out:"WorldServerD.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "RedStone_WorldServer - Win32 forUsa"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RedStone_WorldServer___Win32_forUsa"
# PROP BASE Intermediate_Dir "RedStone_WorldServer___Win32_forUsa"
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
# ADD BASE LINK32 zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /machine:I386 /out:"RedStone_WorldServer(D_USA).exe" /pdbtype:sept
# ADD LINK32 zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /machine:I386 /out:"WorldServer(D_USA).exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "RedStone_WorldServer - Win32 Release"
# Name "RedStone_WorldServer - Win32 Debug"
# Name "RedStone_WorldServer - Win32 DebugForChina"
# Name "RedStone_WorldServer - Win32 ReleaseForChina"
# Name "RedStone_WorldServer - Win32 Debug FOR JAPAN"
# Name "RedStone_WorldServer - Win32 forThai"
# Name "RedStone_WorldServer - Win32 forInner"
# Name "RedStone_WorldServer - Win32 forUsa"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\common\common\adCommandDefine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cADO_BASE.cpp
# End Source File
# Begin Source File

SOURCE=.\cDECODER.cpp
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

SOURCE=..\..\common\common\cSOCKET.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cSRVUTIL.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\ExScript.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\HardwareInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\Msjexhnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\PacketBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\party.cpp
# End Source File
# Begin Source File

SOURCE=.\partyPacket.cpp
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
# Begin Source File

SOURCE=.\UnionParty.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\common\adCommandDefine.h
# End Source File
# Begin Source File

SOURCE=.\AuthControlManager.h
# End Source File
# Begin Source File

SOURCE=.\buildVersion.h
# End Source File
# Begin Source File

SOURCE=..\common\carrotShopDefine.h
# End Source File
# Begin Source File

SOURCE=.\cDECODER.h
# End Source File
# Begin Source File

SOURCE=.\Client.h
# End Source File
# Begin Source File

SOURCE=..\common\cServerStatus.h
# End Source File
# Begin Source File

SOURCE=..\Common\cSOCKET.h
# End Source File
# Begin Source File

SOURCE=..\Common\cSRVUTIL.h
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

SOURCE=..\common\PacketBuffer.h
# End Source File
# Begin Source File

SOURCE=.\party.h
# End Source File
# Begin Source File

SOURCE=..\Common\RSComm.h
# End Source File
# Begin Source File

SOURCE=.\server.h
# End Source File
# Begin Source File

SOURCE=..\common\SFC.h
# End Source File
# Begin Source File

SOURCE=..\common\STACK.H
# End Source File
# Begin Source File

SOURCE=..\common\Tracer.h
# End Source File
# Begin Source File

SOURCE=..\common\zconf.h
# End Source File
# Begin Source File

SOURCE=..\common\zlib.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Etc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\CEvent.h
# End Source File
# Begin Source File

SOURCE=.\CUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\CUtil.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\eventDefine.h
# End Source File
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

SOURCE=.\CGuildBattleManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CGuildBattleManager.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CGuildDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cGuildManager.cpp
# End Source File
# Begin Source File

SOURCE=.\guild.cpp
# End Source File
# Begin Source File

SOURCE=.\guild.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\guild_common.cpp
# End Source File
# End Group
# Begin Group "lib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\zdll.lib

!IF  "$(CFG)" == "RedStone_WorldServer - Win32 Release"

!ELSEIF  "$(CFG)" == "RedStone_WorldServer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_WorldServer - Win32 DebugForChina"

!ELSEIF  "$(CFG)" == "RedStone_WorldServer - Win32 ReleaseForChina"

!ELSEIF  "$(CFG)" == "RedStone_WorldServer - Win32 Debug FOR JAPAN"

!ELSEIF  "$(CFG)" == "RedStone_WorldServer - Win32 forThai"

!ELSEIF  "$(CFG)" == "RedStone_WorldServer - Win32 forInner"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_WorldServer - Win32 forUsa"

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

SOURCE=..\..\common\common\inputConsoleCommand.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\inputConsoleCommand.h
# End Source File
# End Group
# Begin Group "패킷"

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

SOURCE=..\..\common\packet\cPACKET_USERDBSERVER.h
# End Source File
# Begin Source File

SOURCE=..\..\common\packet\cPACKET_WORLDSERVER.h
# End Source File
# End Group
# Begin Source File

SOURCE=".\config-world.txt"
# End Source File
# Begin Source File

SOURCE=".\config-world_dev.txt"
# End Source File
# End Target
# End Project
