# Microsoft Developer Studio Project File - Name="RedStone_GameServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=RedStone_GameServer - Win32 forUsa
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RedStone_GameServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RedStone_GameServer.mak" CFG="RedStone_GameServer - Win32 forUsa"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RedStone_GameServer - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "RedStone_GameServer - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "RedStone_GameServer - Win32 DebugForChina" (based on "Win32 (x86) Console Application")
!MESSAGE "RedStone_GameServer - Win32 ReleaseForChina" (based on "Win32 (x86) Console Application")
!MESSAGE "RedStone_GameServer - Win32 ReleaseForJapan" (based on "Win32 (x86) Console Application")
!MESSAGE "RedStone_GameServer - Win32 HS" (based on "Win32 (x86) Console Application")
!MESSAGE "RedStone_GameServer - Win32 forInner" (based on "Win32 (x86) Console Application")
!MESSAGE "RedStone_GameServer - Win32 forThai" (based on "Win32 (x86) Console Application")
!MESSAGE "RedStone_GameServer - Win32 PreviewTool" (based on "Win32 (x86) Console Application")
!MESSAGE "RedStone_GameServer - Win32 Debug FOR JAPAN" (based on "Win32 (x86) Console Application")
!MESSAGE "RedStone_GameServer - Win32 forUsa" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/RedStone/RedStone_GameServer", NGAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RedStone_GameServer - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "../Common" /I "../packet" /I "../../Common/common" /I "../../common/packet" /D "_FOR_KOREA" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__USE_ZLIB" /FR /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /machine:I386 /out:"GameServerR.exe" /libpath:"../lib" /mapinfo:exports /mapinfo:lines
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../Common/common" /I "../../common/packet" /D "_SERVER_" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__USE_ZLIB" /FR"" /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /stack:0xa00000 /subsystem:console /map /debug /machine:I386 /out:"GameServerD.exe" /pdbtype:sept /libpath:"../tinyxml2.6.1"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 DebugForChina"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RedStone_GameServer___Win32_DebugForChina"
# PROP BASE Intermediate_Dir "RedStone_GameServer___Win32_DebugForChina"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugForChina"
# PROP Intermediate_Dir "DebugForChina"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../Common" /I "../packet" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__USE_ZLIB" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../Common/common" /I "../../common/packet" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__USE_ZLIB" /D "_FOR_CHINA" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zdll.lib ssl.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"RedStone_GameServer.exe" /pdbtype:sept /libpath:"../lib"
# ADD LINK32 zdll.lib ssl.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /machine:I386 /out:"RedStone_GameServer(china)debug.exe" /pdbtype:sept /libpath:"../lib"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 ReleaseForChina"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "RedStone_GameServer___Win32_ReleaseForChina"
# PROP BASE Intermediate_Dir "RedStone_GameServer___Win32_ReleaseForChina"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseForChina"
# PROP Intermediate_Dir "ReleaseForChina"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /O2 /I "../Common" /I "../packet" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__USE_ZLIB" /FR /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /O2 /I "../../Common/common" /I "../../common/packet" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__USE_ZLIB" /D "_FOR_CHINA" /FR /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zdll.lib ssl.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /debugtype:coff /machine:I386 /libpath:"../lib"
# ADD LINK32 zdll.lib ssl.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /debugtype:coff /machine:I386 /out:"RedStone_GameServer(china)release.exe" /libpath:"../lib"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 ReleaseForJapan"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "RedStone_GameServer___Win32_ReleaseForJapan"
# PROP BASE Intermediate_Dir "RedStone_GameServer___Win32_ReleaseForJapan"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseForJapan"
# PROP Intermediate_Dir "ReleaseForJapan"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /O2 /I "../Common" /I "../packet" /D "_FOR_KOREA" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__USE_ZLIB" /FR /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /O2 /I "../../Common/common" /I "../../common/packet" /D "_FOR_JAPAN" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__USE_ZLIB" /FR /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zdll.lib ssl.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /debugtype:coff /machine:I386 /libpath:"../lib"
# ADD LINK32 zdll.lib ssl.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /debugtype:coff /machine:I386 /out:"../../Red stone for Japan/RedStone_GameServer(forJapan)release.exe" /libpath:"../lib"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 HS"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RedStone_GameServer___Win32_HS"
# PROP BASE Intermediate_Dir "RedStone_GameServer___Win32_HS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "RedStone_GameServer___Win32_HS"
# PROP Intermediate_Dir "RedStone_GameServer___Win32_HS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../Common" /I "../packet" /I "HShield" /D "_USE_HS" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__USE_ZLIB" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../Common/common" /I "../../common/packet" /D "_USE_HS" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__USE_ZLIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 AntiCpSvr.lib zdll.lib ssl.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ssl.lib /nologo /subsystem:console /map /debug /machine:I386 /out:"RedStone_GameServer.exe" /pdbtype:sept /libpath:"../lib" /libpath:"HShield"
# ADD LINK32 AntiCpSvr.lib zdll.lib ssl.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ssl.lib /nologo /subsystem:console /map /debug /machine:I386 /out:"RedStone_GameServer.exe" /pdbtype:sept /libpath:"../lib" /libpath:"HShield"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forInner"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RedStone_GameServer___Win32_forInner"
# PROP BASE Intermediate_Dir "RedStone_GameServer___Win32_forInner"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "forInner"
# PROP Intermediate_Dir "forInner"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../Common" /I "../packet" /I "HShield" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__USE_ZLIB" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../Common/common" /I "../../common/packet" /D "_FOR_INNER" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__USE_ZLIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 AntiCpSvr.lib zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /machine:I386 /out:"RedStone_GameServer.exe" /pdbtype:sept /libpath:"../lib" /libpath:"HShield"
# ADD LINK32 zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /machine:I386 /out:"RedStone_GameServer(inner).exe" /pdbtype:sept /libpath:"../lib" /libpath:"HShield"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forThai"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RedStone_GameServer___Win32_forThai"
# PROP BASE Intermediate_Dir "RedStone_GameServer___Win32_forThai"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "forThai"
# PROP Intermediate_Dir "forThai"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../Common" /I "../packet" /I "HShield" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__USE_ZLIB" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../Common/common" /I "../../common/packet" /D "_FOR_THAI" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__USE_ZLIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x41e /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /stack:0xa00000 /subsystem:console /map /debug /machine:I386 /out:"RedStone_GameServer.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /stack:0xa00000 /subsystem:console /map /debug /machine:I386 /out:"RedStone_GameServer(forThai).exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 PreviewTool"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RedStone_GameServer___Win32_PreviewTool"
# PROP BASE Intermediate_Dir "RedStone_GameServer___Win32_PreviewTool"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PreviewTool"
# PROP Intermediate_Dir "PreviewTool"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../Common" /I "../packet" /I "../redstone_Gameserver" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__USE_ZLIB" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../Common/common" /I "../../common/packet" /D "_IS_TOOL" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__USE_ZLIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /stack:0xa00000 /subsystem:console /map /debug /machine:I386 /out:"RedStone_GameServer.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /stack:0xa00000 /subsystem:console /map /debug /machine:I386 /out:"GameServer(x4 Speed).exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 Debug FOR JAPAN"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RedStone_GameServer___Win32_Debug_FOR_JAPAN"
# PROP BASE Intermediate_Dir "RedStone_GameServer___Win32_Debug_FOR_JAPAN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "RedStone_GameServer___Win32_Debug_FOR_JAPAN"
# PROP Intermediate_Dir "RedStone_GameServer___Win32_Debug_FOR_JAPAN"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../Common" /I "../packet" /D "_FOR_JAPAN" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__USE_ZLIB" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../Common/common" /I "../../common/packet" /D "_SERVER_" /D "_FOR_JAPAN" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__USE_ZLIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 zdll.lib ssl.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /map /debug /machine:I386 /out:"RedStone_GameServer(forJapan)debug.exe" /pdbtype:sept /libpath:"../lib"
# ADD LINK32 tinyxmld.lib OGPAPI.lib zdll.lib ssl.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /pdb:"Z:\540_Server_JPN\RedStone_GameServer\RedStone_GameServer(forJapan)debug.pdb" /map:"Z:\540_Server_JPN\RedStone_GameServer\RedStone_GameServer(forJapan)debug.map" /debug /machine:I386 /out:"Z:\540_Server_JPN\RedStone_GameServer\RedStone_GameServer(forJapan)debug.exe" /pdbtype:sept /libpath:"../tinyxml2.6.1" /libpath:"../OGP_Lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forUsa"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "RedStone_GameServer___Win32_forUsa"
# PROP BASE Intermediate_Dir "RedStone_GameServer___Win32_forUsa"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "forUsa"
# PROP Intermediate_Dir "forUsa"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../OGP_Lib" /I "../../Common/common" /I "../../common/packet" /I "../tinyxml2.6.1" /D "_FOR_USA" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__USE_ZLIB" /FR"" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../OGP_Lib" /I "../../Common/common" /I "../../common/packet" /I "../tinyxml2.6.1" /D "_FOR_USA" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WINDOWS" /D "__USE_ZLIB" /FR"" /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 tinyxmld.lib zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /stack:0xa00000 /subsystem:console /map /debug /machine:I386 /out:"GameServerD.exe" /pdbtype:sept /libpath:"../OGP_lib" /libpath:"../tinyxml2.6.1"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 tinyxmld.lib zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib zdll.lib winmm.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /stack:0xa00000 /subsystem:console /map /debug /machine:I386 /out:"GameServer(D_USA).exe" /pdbtype:sept /libpath:"../OGP_lib" /libpath:"../tinyxml2.6.1"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "RedStone_GameServer - Win32 Release"
# Name "RedStone_GameServer - Win32 Debug"
# Name "RedStone_GameServer - Win32 DebugForChina"
# Name "RedStone_GameServer - Win32 ReleaseForChina"
# Name "RedStone_GameServer - Win32 ReleaseForJapan"
# Name "RedStone_GameServer - Win32 HS"
# Name "RedStone_GameServer - Win32 forInner"
# Name "RedStone_GameServer - Win32 forThai"
# Name "RedStone_GameServer - Win32 PreviewTool"
# Name "RedStone_GameServer - Win32 Debug FOR JAPAN"
# Name "RedStone_GameServer - Win32 forUsa"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\booking_work.cpp
# End Source File
# Begin Source File

SOURCE=.\CBookedWork.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CCompress.cpp
# End Source File
# Begin Source File

SOURCE=.\CExceptionHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\CImageData.cpp
# End Source File
# Begin Source File

SOURCE=.\cJOB.cpp
# End Source File
# Begin Source File

SOURCE=.\cNPC.cpp
# End Source File
# Begin Source File

SOURCE=.\cPACKET.cpp
# End Source File
# Begin Source File

SOURCE=.\cPROJECT.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cServerStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\cSPLINE.cpp
# End Source File
# Begin Source File

SOURCE=.\debugCode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\HardwareInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\objectImage.cpp
# End Source File
# Begin Source File

SOURCE=.\packetManger.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\seedex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\SMTP.cpp
# End Source File
# Begin Source File

SOURCE=.\stressTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\Tracer.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\booking_work.h
# End Source File
# Begin Source File

SOURCE=.\buildVersion.h
# End Source File
# Begin Source File

SOURCE=.\CBookedWork.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CCompress.h
# End Source File
# Begin Source File

SOURCE=.\CExceptionHandler.h
# End Source File
# Begin Source File

SOURCE=.\CImageData.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CImageDataDefine.h
# End Source File
# Begin Source File

SOURCE=.\cJOB.H
# End Source File
# Begin Source File

SOURCE=.\cNPC.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CPetDefine.h
# End Source File
# Begin Source File

SOURCE=.\cPROJECT.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cServerStatus.h
# End Source File
# Begin Source File

SOURCE=.\cSPLINE.h
# End Source File
# Begin Source File

SOURCE=.\debugCode.h
# End Source File
# Begin Source File

SOURCE=.\defineWarningMessage.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\doorDefine.h
# End Source File
# Begin Source File

SOURCE=.\gameCodeHeader.h
# End Source File
# Begin Source File

SOURCE=.\ggsrv.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\HardwareInfo.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\jobDefine.h
# End Source File
# Begin Source File

SOURCE=.\objectImage.h
# End Source File
# Begin Source File

SOURCE=.\packetManager.h
# End Source File
# Begin Source File

SOURCE=.\ProcessThread.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\rebirth_define.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\regular_random.h
# End Source File
# Begin Source File

SOURCE=.\stressTest.h
# End Source File
# Begin Source File

SOURCE=.\summonBeast.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\summonBeastDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\userCustomDataDefine.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Text"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\config-Battle.txt"
# End Source File
# Begin Source File

SOURCE=".\config-game.txt"
# End Source File
# Begin Source File

SOURCE=".\config-game_dev.txt"
# End Source File
# End Group
# Begin Group "Packet"

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

SOURCE=..\..\common\packet\cPACKET_LOGSERVER.h
# End Source File
# Begin Source File

SOURCE=..\..\common\packet\cPACKET_USERDBSERVER.h
# End Source File
# Begin Source File

SOURCE=..\..\common\packet\cPACKET_WORLDSERVER.h
# End Source File
# Begin Source File

SOURCE=..\..\common\packet\definePacketData.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\RSComm.h
# End Source File
# End Group
# Begin Group "Server"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BufferForPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\BufferForPacket.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cADO_BASE.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cADO_BASE.h
# End Source File
# Begin Source File

SOURCE=.\Client.cpp
# End Source File
# Begin Source File

SOURCE=.\Client.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cLOG_QUE.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cScript.cpp
# End Source File
# Begin Source File

SOURCE=.\cServer.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cSOCKET.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cSRVUTIL.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\PacketBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\PacketBuffer.h
# End Source File
# Begin Source File

SOURCE=.\Server.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\ssl.h
# End Source File
# End Group
# Begin Group "Field"

# PROP Default_Filter ""
# Begin Group "Operate Packet"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CFieldBankPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\CFieldCartPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\CFieldCommunityPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\CFieldEventPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\CFieldGuildPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\CFieldOperatePacket.cpp
# End Source File
# Begin Source File

SOURCE=.\CFieldOperateTradePacket.cpp
# End Source File
# Begin Source File

SOURCE=.\CFieldPitchmanShopPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\filedDeliverPacket.cpp
# End Source File
# End Group
# Begin Group "Send Packet"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CFieldAddSendPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\CFieldSendPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\CFieldSendStoredPacket.cpp
# End Source File
# End Group
# Begin Group "Field Event"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CFieldOXQuiz.cpp
# End Source File
# Begin Source File

SOURCE=.\karma_field.cpp
# End Source File
# Begin Source File

SOURCE=.\word_quiz_field.cpp
# End Source File
# End Group
# Begin Group "필드 운영 명령"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\field_oc.cpp
# End Source File
# Begin Source File

SOURCE=.\field_oc.h
# End Source File
# End Group
# Begin Group "결투 관련"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\duelForField.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\banner_field.cpp
# End Source File
# Begin Source File

SOURCE=.\CField.cpp
# End Source File
# Begin Source File

SOURCE=.\cFIELD.h
# End Source File
# Begin Source File

SOURCE=.\CFieldActor.cpp
# End Source File
# Begin Source File

SOURCE=.\CFieldAddActor.cpp
# End Source File
# Begin Source File

SOURCE=.\CFieldBooking.cpp
# End Source File
# Begin Source File

SOURCE=.\CFieldGuild.cpp
# End Source File
# Begin Source File

SOURCE=.\CFieldHealSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\CFieldItem.cpp
# End Source File
# Begin Source File

SOURCE=.\CFieldLocalPartition.cpp
# End Source File
# Begin Source File

SOURCE=.\CFieldOperateBookedWork.cpp
# End Source File
# Begin Source File

SOURCE=.\CFieldParty.cpp
# End Source File
# Begin Source File

SOURCE=.\CFieldSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\CLocalPartion.h
# End Source File
# Begin Source File

SOURCE=.\field_arca.cpp
# End Source File
# Begin Source File

SOURCE=.\field_area.cpp
# End Source File
# Begin Source File

SOURCE=.\field_character_pattern.cpp
# End Source File
# Begin Source File

SOURCE=.\field_door.cpp
# End Source File
# Begin Source File

SOURCE=.\field_guild_battle.cpp
# End Source File
# Begin Source File

SOURCE=.\field_terrain.cpp
# End Source File
# Begin Source File

SOURCE=.\trapInField.cpp
# End Source File
# Begin Source File

SOURCE=.\utilField.cpp
# End Source File
# End Group
# Begin Group "Actor"

# PROP Default_Filter ""
# Begin Group "Character"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CActorMonster.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorNPC.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorPet.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorQuest.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorSendLog.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorSlave.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorSummonBeast.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorTamedMonster.cpp
# End Source File
# End Group
# Begin Group "Actor Item"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CActorEquipmentEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorItem.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorItemUseToCharacter.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorItemUseToItem.cpp
# End Source File
# End Group
# Begin Group "Actor Skill"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\actor_attack_object.cpp
# End Source File
# Begin Source File

SOURCE=.\actorSkillAI.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorSkillAura.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorSkillExtraEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\checkBuffActor.cpp
# End Source File
# End Group
# Begin Group "ai"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\actor_ai_sage_master.cpp
# End Source File
# Begin Source File

SOURCE=.\actor_pattern.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorAi.cpp
# End Source File
# Begin Source File

SOURCE=.\FindWay.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\common\common\actorDefine.h
# End Source File
# Begin Source File

SOURCE=.\CActor.cpp
# End Source File
# Begin Source File

SOURCE=.\cACTOR.h
# End Source File
# Begin Source File

SOURCE=.\CActorBank.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorBattle.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorCart.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorCorrectData.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorGuild.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorGuildSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorMove.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorParty.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorPassiveSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorPitchmanShop.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorReactionSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorSave.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorSend.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorStatus.h
# End Source File
# Begin Source File

SOURCE=.\CActorTitle.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorWrongStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\GreateGodGrace.cpp
# End Source File
# Begin Source File

SOURCE=.\trapForActor.cpp
# End Source File
# End Group
# Begin Group "Skill"

# PROP Default_Filter ""
# Begin Group "Skill Type"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ast_chain_lightning.cpp
# End Source File
# Begin Source File

SOURCE=.\ast_chain_lightning.h
# End Source File
# Begin Source File

SOURCE=.\ast_ContinuousWhirlwind.cpp
# End Source File
# Begin Source File

SOURCE=.\ast_ContinuousWhirlwind.h
# End Source File
# Begin Source File

SOURCE=.\ast_fire_wall.cpp
# End Source File
# Begin Source File

SOURCE=.\ast_fire_wall.h
# End Source File
# Begin Source File

SOURCE=.\ast_gas.cpp
# End Source File
# Begin Source File

SOURCE=.\ast_gas.h
# End Source File
# Begin Source File

SOURCE=.\ast_giga_lightning.cpp
# End Source File
# Begin Source File

SOURCE=.\ast_giga_lightning.h
# End Source File
# Begin Source File

SOURCE=.\ast_mirror_image.cpp
# End Source File
# Begin Source File

SOURCE=.\ast_mirror_image.h
# End Source File
# Begin Source File

SOURCE=.\ast_one_bite.cpp
# End Source File
# Begin Source File

SOURCE=.\ast_one_bite.h
# End Source File
# Begin Source File

SOURCE=.\astBigBullet.cpp
# End Source File
# Begin Source File

SOURCE=.\astBigBullet.h
# End Source File
# Begin Source File

SOURCE=.\astBouncingMissile.cpp
# End Source File
# Begin Source File

SOURCE=.\astBouncingMissile.h
# End Source File
# Begin Source File

SOURCE=.\astContinuousBit.cpp
# End Source File
# Begin Source File

SOURCE=.\astContinuousBit.h
# End Source File
# Begin Source File

SOURCE=.\astContinuousHitOfDoppelganger.cpp
# End Source File
# Begin Source File

SOURCE=.\astContinuousHitOfDoppelganger.h
# End Source File
# Begin Source File

SOURCE=.\astDependOnImageTrap.cpp
# End Source File
# Begin Source File

SOURCE=.\astDependOnImageTrap.h
# End Source File
# Begin Source File

SOURCE=.\astDoubleTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\astDoubleTarget.h
# End Source File
# Begin Source File

SOURCE=.\astExplosionWithImage.cpp
# End Source File
# Begin Source File

SOURCE=.\astExplosionWithImage.h
# End Source File
# Begin Source File

SOURCE=.\astFlorenceSpecial.cpp
# End Source File
# Begin Source File

SOURCE=.\astFlorenceSpecial.h
# End Source File
# Begin Source File

SOURCE=.\astLightningWinder.cpp
# End Source File
# Begin Source File

SOURCE=.\astLightningWinder.h
# End Source File
# Begin Source File

SOURCE=.\astMultiMissile.cpp
# End Source File
# Begin Source File

SOURCE=.\astMultiMissile.h
# End Source File
# Begin Source File

SOURCE=.\astRollingLog.cpp
# End Source File
# Begin Source File

SOURCE=.\astRollingLog.h
# End Source File
# Begin Source File

SOURCE=.\astThrowItem.cpp
# End Source File
# Begin Source File

SOURCE=.\astThrowItem.h
# End Source File
# Begin Source File

SOURCE=.\astWideMissile.cpp
# End Source File
# Begin Source File

SOURCE=.\astWideMissile.h
# End Source File
# Begin Source File

SOURCE=.\CSkillAidAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillAidMagic.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillAlteringHitter.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillAwesomeFortress.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillBiteHanging.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillBitGlider.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillBoomerang.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillBouncingLinear.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillBunshineAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillCharging.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillContinuousAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillContinuousShoot.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillDropOnHead.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillExplosion.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillExplosionMissile.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillFireBall.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillGuardianPost.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillHeal.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillIcyStalagmite.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillImpact.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillIntervalShooter.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillJump.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillJumpAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillLandMarker.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillLaser.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillMachine.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillMirrorTower.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillMissile.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillPetCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillPiercingMissile.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillRadialArc.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillRush.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillScimitarCutting.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillShimmeringShield.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillSpreadArrow.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillSurprisingRaid.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillTeleport.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillTornado.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillWaterFall.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillWhirlThrusting.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillWhirlwind.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ast_define.h
# End Source File
# Begin Source File

SOURCE=.\CAbility.h
# End Source File
# Begin Source File

SOURCE=.\CAbilityInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CActiveSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\CActiveSkill.h
# End Source File
# Begin Source File

SOURCE=.\CSkill.cpp
# End Source File
# Begin Source File

SOURCE=..\common\common\cSKILL.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CSkillDefine.h
# End Source File
# Begin Source File

SOURCE=.\CSkillInit.cpp
# End Source File
# End Group
# Begin Group "Base Code"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cDECODER.cpp
# End Source File
# Begin Source File

SOURCE=.\cDECODER.h
# End Source File
# Begin Source File

SOURCE=.\cIMAGE.cpp
# End Source File
# Begin Source File

SOURCE=.\cIMAGE.h
# End Source File
# Begin Source File

SOURCE=.\CProfiler.cpp
# End Source File
# Begin Source File

SOURCE=.\CProfiler.h
# End Source File
# Begin Source File

SOURCE=.\cPROTRACTOR.cpp
# End Source File
# Begin Source File

SOURCE=.\cPROTRACTOR.h
# End Source File
# Begin Source File

SOURCE=.\CSolidCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\CSolidCheck.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\regular_random.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\SFC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\SFC.h
# End Source File
# Begin Source File

SOURCE=.\STDAFX.H
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

SOURCE=..\..\common\common\CItemDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CItemEffectDefine.h
# End Source File
# Begin Source File

SOURCE=.\CItemManager.cpp
# End Source File
# Begin Source File

SOURCE=.\item_book.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\itemDefine.cpp
# End Source File
# Begin Source File

SOURCE=.\itemPackList.cpp
# End Source File
# Begin Source File

SOURCE=.\itemPackList.h
# End Source File
# Begin Source File

SOURCE=.\itemSet.cpp
# End Source File
# Begin Source File

SOURCE=.\itemSet.h
# End Source File
# Begin Source File

SOURCE=.\pieceItem.cpp
# End Source File
# Begin Source File

SOURCE=.\pieceItem.h
# End Source File
# End Group
# Begin Group "Event"

# PROP Default_Filter ""
# Begin Group "tresure map"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\common\treasureMapDefine.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CDropItemEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\CEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\CEvent.h
# End Source File
# Begin Source File

SOURCE=.\CItemSaleEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\CPlusDropGoldEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\oxQuiz.cpp
# End Source File
# Begin Source File

SOURCE=.\word_quiz.cpp
# End Source File
# End Group
# Begin Group "Game"

# PROP Default_Filter ""
# Begin Group "Game Packet"

# PROP Default_Filter ""
# Begin Group "Broad Cast Server"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\receive_packet_from_bcs.cpp
# End Source File
# Begin Source File

SOURCE=.\send_packet_to_BCS.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\CReceivePacketFromDB.cpp
# End Source File
# Begin Source File

SOURCE=.\CReceivePacketFromWorld.cpp
# End Source File
# Begin Source File

SOURCE=.\CReceivePartyPacketFromWorld.cpp
# End Source File
# Begin Source File

SOURCE=.\CSendPacketToClient.cpp
# End Source File
# Begin Source File

SOURCE=.\CSendPacketToDB.cpp
# End Source File
# Begin Source File

SOURCE=.\CSendPacketToWorld.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\cGAME.cpp
# End Source File
# Begin Source File

SOURCE=.\cGAME.h
# End Source File
# Begin Source File

SOURCE=.\CGameActor.cpp
# End Source File
# Begin Source File

SOURCE=.\CGameLog.cpp
# End Source File
# Begin Source File

SOURCE=.\CGameParty.cpp
# End Source File
# Begin Source File

SOURCE=.\utilGame.cpp
# End Source File
# End Group
# Begin Group "Title"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CTitle.cpp
# End Source File
# Begin Source File

SOURCE=.\CTitle.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CTitleDefine.h
# End Source File
# End Group
# Begin Group "Guild"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CGuild.cpp
# End Source File
# Begin Source File

SOURCE=.\CGuild.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CGuildDefine.h
# End Source File
# Begin Source File

SOURCE=.\CGuildSendPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\CGuildSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\CGuildSkill.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\guild_common.cpp
# End Source File
# Begin Source File

SOURCE=.\guild_manager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\guild_skill_define.h
# End Source File
# Begin Source File

SOURCE=.\instance_guild_battle_manager.cpp
# End Source File
# Begin Source File

SOURCE=.\instance_guild_battle_manager.h
# End Source File
# End Group
# Begin Group "Game Object"

# PROP Default_Filter ""
# Begin Group "Karma"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CKarma.cpp
# End Source File
# Begin Source File

SOURCE=.\CKarma.h
# End Source File
# Begin Source File

SOURCE=.\CKarmaContainer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CKarmaDefine.h
# End Source File
# Begin Source File

SOURCE=.\CKarmaReaction.cpp
# End Source File
# Begin Source File

SOURCE=.\CKarmaTrigger.cpp
# End Source File
# Begin Source File

SOURCE=.\karma_delay.cpp
# End Source File
# End Group
# Begin Group "secret dungeon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\instanceField.cpp
# End Source File
# Begin Source File

SOURCE=.\secretDungeon.cpp
# End Source File
# Begin Source File

SOURCE=.\secretDungeon.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\secretDungeonDefine.h
# End Source File
# End Group
# Begin Group "arca"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\arca.cpp
# End Source File
# Begin Source File

SOURCE=.\arca.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\arcaDefine.h
# End Source File
# End Group
# Begin Group "bank"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CBank.cpp
# End Source File
# Begin Source File

SOURCE=.\CBank.h
# End Source File
# End Group
# Begin Group "carrot shop"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\carrotShop.cpp
# End Source File
# Begin Source File

SOURCE=.\carrotShop.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\carrotShopDefine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\carrotShopDefine.h
# End Source File
# End Group
# Begin Group "banner"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\banner.cpp
# End Source File
# Begin Source File

SOURCE=.\banner.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\bannerDefine.h
# End Source File
# End Group
# Begin Group "fire work"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\fireWork.cpp
# End Source File
# Begin Source File

SOURCE=.\fireWork.h
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

SOURCE=..\..\common\common\book_define.h
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

SOURCE=..\..\common\common\pattern_define.h
# End Source File
# End Group
# Begin Group "미니 펫"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\actor_mini_pet.cpp
# End Source File
# Begin Source File

SOURCE=.\actor_mini_pet.h
# End Source File
# Begin Source File

SOURCE=.\miniPet.cpp
# End Source File
# Begin Source File

SOURCE=.\miniPet.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\miniPetCommon.h
# End Source File
# End Group
# Begin Group "결투"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\duel.cpp
# End Source File
# Begin Source File

SOURCE=.\duel.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CDataSave.cpp
# End Source File
# Begin Source File

SOURCE=.\CDataSave.h
# End Source File
# Begin Source File

SOURCE=.\CDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialog.h
# End Source File
# Begin Source File

SOURCE=.\cPARTY.cpp
# End Source File
# Begin Source File

SOURCE=.\cPARTY.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cPetDefine.cpp
# End Source File
# Begin Source File

SOURCE=.\CPitchmanShop.cpp
# End Source File
# Begin Source File

SOURCE=.\CPitchmanShop.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CPitchmanShopDefine.h
# End Source File
# Begin Source File

SOURCE=.\CPortal.cpp
# End Source File
# Begin Source File

SOURCE=.\CPortal.h
# End Source File
# Begin Source File

SOURCE=.\CQuest.cpp
# End Source File
# Begin Source File

SOURCE=.\CQuest.h
# End Source File
# Begin Source File

SOURCE=.\CShop.cpp
# End Source File
# Begin Source File

SOURCE=.\CShop.h
# End Source File
# Begin Source File

SOURCE=.\CShoppingCart.cpp
# End Source File
# Begin Source File

SOURCE=.\CShoppingCart.h
# End Source File
# Begin Source File

SOURCE=.\CTrade.cpp
# End Source File
# Begin Source File

SOURCE=.\CTrade.h
# End Source File
# Begin Source File

SOURCE=.\cUnionParty.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\partyDefine.h
# End Source File
# Begin Source File

SOURCE=.\trap.cpp
# End Source File
# Begin Source File

SOURCE=.\trap.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\trapDefine.h
# End Source File
# Begin Source File

SOURCE=.\trapSkill.cpp
# End Source File
# End Group
# Begin Group "area"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\area_arca.cpp
# End Source File
# Begin Source File

SOURCE=.\area_door.cpp
# End Source File
# Begin Source File

SOURCE=.\area_trap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\areaDefine.h
# End Source File
# Begin Source File

SOURCE=.\cAREA.cpp
# End Source File
# Begin Source File

SOURCE=.\cAREA.h
# End Source File
# End Group
# Begin Group "XTrap"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XTrap\XTrap4Server.cpp
# End Source File
# Begin Source File

SOURCE=.\XTrap4Server.dll

!IF  "$(CFG)" == "RedStone_GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 DebugForChina"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 ReleaseForChina"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 ReleaseForJapan"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 HS"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forInner"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forThai"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 PreviewTool"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 Debug FOR JAPAN"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forUsa"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XTrap\Xtrap_S_Interface.h
# End Source File
# End Group
# Begin Group "Lib&DLL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AntiCpSvr.dll

!IF  "$(CFG)" == "RedStone_GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 DebugForChina"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 ReleaseForChina"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 ReleaseForJapan"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 HS"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forInner"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forThai"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 PreviewTool"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 Debug FOR JAPAN"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forUsa"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ggauth.dll

!IF  "$(CFG)" == "RedStone_GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 DebugForChina"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 ReleaseForChina"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 ReleaseForJapan"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 HS"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forInner"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forThai"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 PreviewTool"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 Debug FOR JAPAN"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forUsa"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ggsrvdll.dll

!IF  "$(CFG)" == "RedStone_GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 DebugForChina"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 ReleaseForChina"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 ReleaseForJapan"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 HS"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forInner"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forThai"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 PreviewTool"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 Debug FOR JAPAN"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forUsa"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SSDBInfo.bin

!IF  "$(CFG)" == "RedStone_GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 DebugForChina"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 ReleaseForChina"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 ReleaseForJapan"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 HS"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forInner"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forThai"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 PreviewTool"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 Debug FOR JAPAN"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forUsa"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ssl.dll

!IF  "$(CFG)" == "RedStone_GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 DebugForChina"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 ReleaseForChina"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 ReleaseForJapan"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 HS"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forInner"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forThai"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 PreviewTool"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 Debug FOR JAPAN"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forUsa"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zlib1.dll

!IF  "$(CFG)" == "RedStone_GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 DebugForChina"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 ReleaseForChina"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 ReleaseForJapan"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 HS"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forInner"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forThai"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 PreviewTool"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 Debug FOR JAPAN"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forUsa"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ssl.lib

!IF  "$(CFG)" == "RedStone_GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 DebugForChina"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 ReleaseForChina"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 ReleaseForJapan"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 HS"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forInner"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forThai"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 PreviewTool"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 Debug FOR JAPAN"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forUsa"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\zdll.lib

!IF  "$(CFG)" == "RedStone_GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 DebugForChina"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 ReleaseForChina"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 ReleaseForJapan"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 HS"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forInner"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forThai"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 PreviewTool"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 Debug FOR JAPAN"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forUsa"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ggsrvdll.lib

!IF  "$(CFG)" == "RedStone_GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 DebugForChina"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 ReleaseForChina"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 ReleaseForJapan"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 HS"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forInner"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forThai"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 PreviewTool"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 Debug FOR JAPAN"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forUsa"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ggsrvlib.lib

!IF  "$(CFG)" == "RedStone_GameServer - Win32 Release"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 DebugForChina"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 ReleaseForChina"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 ReleaseForJapan"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 HS"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forInner"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forThai"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 PreviewTool"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 Debug FOR JAPAN"

!ELSEIF  "$(CFG)" == "RedStone_GameServer - Win32 forUsa"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Life Code"

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
# Begin Group "common"

# PROP Default_Filter ""
# Begin Group "서버 관리"

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

SOURCE=..\..\common\common\commonDefine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CSharedMemory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CSharedMemory.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\inputConsoleCommand.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\inputConsoleCommand.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\MemoryModule.c
# End Source File
# Begin Source File

SOURCE=..\..\common\common\MemoryModule.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\mess_sign.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\playerJobCommon.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\playerJobCommon.h
# End Source File
# End Group
# End Target
# End Project
