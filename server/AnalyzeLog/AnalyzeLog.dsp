# Microsoft Developer Studio Project File - Name="AnalyzeLog" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=AnalyzeLog - Win32 Debug FOR JAPAN
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AnalyzeLog.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AnalyzeLog.mak" CFG="AnalyzeLog - Win32 Debug FOR JAPAN"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AnalyzeLog - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "AnalyzeLog - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "AnalyzeLog - Win32 DebugForChina" (based on "Win32 (x86) Application")
!MESSAGE "AnalyzeLog - Win32 ReleaseForChina" (based on "Win32 (x86) Application")
!MESSAGE "AnalyzeLog - Win32 Release FOR JAPAN" (based on "Win32 (x86) Application")
!MESSAGE "AnalyzeLog - Win32 Server Debug" (based on "Win32 (x86) Application")
!MESSAGE "AnalyzeLog - Win32 Server Debug FOR JAPAN" (based on "Win32 (x86) Application")
!MESSAGE "AnalyzeLog - Win32 Server DebugForChina" (based on "Win32 (x86) Application")
!MESSAGE "AnalyzeLog - Win32 Server Release" (based on "Win32 (x86) Application")
!MESSAGE "AnalyzeLog - Win32 Server Release FOR JAPAN" (based on "Win32 (x86) Application")
!MESSAGE "AnalyzeLog - Win32 forUSA" (based on "Win32 (x86) Application")
!MESSAGE "AnalyzeLog - Win32 forThai" (based on "Win32 (x86) Application")
!MESSAGE "AnalyzeLog - Win32 forThaiServer" (based on "Win32 (x86) Application")
!MESSAGE "AnalyzeLog - Win32 Debug FOR JAPAN" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/RedStoneServer/AnalyzeLog", WFBAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AnalyzeLog - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "../common" /I "../packet" /I "../REDSTONE_GAMESERVER" /I "../../common/common" /I "../../common/packet" /D "_FOR_KOREA" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ws2_32.lib winmm.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /out:"AnalyzeLog(Release).exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../../common/common" /I "../../common/packet" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_RSO" /FR /YX"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib winmm.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"AnalyzeLog(Debug).exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 DebugForChina"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AnalyzeLog___Win32_DebugForChina"
# PROP BASE Intermediate_Dir "AnalyzeLog___Win32_DebugForChina"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "AnalyzeLog___Win32_DebugForChina"
# PROP Intermediate_Dir "AnalyzeLog___Win32_DebugForChina"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /I "../../common/common" /I "../../common/packet" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CHINA" /FR /YX"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib FTPClient.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/AnalyzeLog20040225.exe" /pdbtype:sept
# ADD LINK32 winmm.lib FTPClient.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/AnalyzeLog(china).exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 ReleaseForChina"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AnalyzeLog___Win32_ReleaseForChina"
# PROP BASE Intermediate_Dir "AnalyzeLog___Win32_ReleaseForChina"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "AnalyzeLog___Win32_ReleaseForChina"
# PROP Intermediate_Dir "AnalyzeLog___Win32_ReleaseForChina"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "../common" /I "../packet" /I "../REDSTONE_GAMESERVER" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../common" /I "../packet" /I "../REDSTONE_GAMESERVER" /I "../../common/common" /I "../../common/packet" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CHINA" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ws2_32.lib winmm.lib FTPClient.lib /nologo /subsystem:windows /map /debug /debugtype:coff /machine:I386 /out:"Release/AnalyzeLog20040225.exe"
# ADD LINK32 ws2_32.lib winmm.lib FTPClient.lib /nologo /subsystem:windows /map /debug /debugtype:coff /machine:I386 /out:"AnalyzeLog(forChina).exe"

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 Release FOR JAPAN"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AnalyzeLog___Win32_Release_FOR_JAPAN"
# PROP BASE Intermediate_Dir "AnalyzeLog___Win32_Release_FOR_JAPAN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseForJAPAN"
# PROP Intermediate_Dir "ReleaseForJAPAN"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "../common" /I "../packet" /I "../REDSTONE_GAMESERVER" /D "_FOR_KOREA" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../common" /I "../packet" /I "../REDSTONE_GAMESERVER" /I "../../common/common" /I "../../common/packet" /D "_FOR_JAPAN" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ws2_32.lib winmm.lib FTPClient.lib /nologo /subsystem:windows /map /debug /debugtype:coff /machine:I386 /out:"Release/AnalyzeLog20040225.exe"
# ADD LINK32 ws2_32.lib winmm.lib /nologo /subsystem:windows /map /debug /debugtype:coff /machine:I386 /out:"AnalyzeLog(Release_Japan).exe"

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 Server Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AnalyzeLog___Win32_Server_Debug"
# PROP BASE Intermediate_Dir "AnalyzeLog___Win32_Server_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "AnalyzeLog___Win32_Server_Debug"
# PROP Intermediate_Dir "AnalyzeLog___Win32_Server_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /I "../../common/common" /I "../../common/packet" /D "FOR_DOWNLOADSERVER" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib FTPClient.lib /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /out:"Debug/AnalyzeLog20040225.exe" /pdbtype:sept
# ADD LINK32 winmm.lib FTPClient.lib /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /out:"Debug/AnalyzeLogServer(Debug).exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 Server Debug FOR JAPAN"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AnalyzeLog___Win32_Server_Debug_FOR_JAPAN"
# PROP BASE Intermediate_Dir "AnalyzeLog___Win32_Server_Debug_FOR_JAPAN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "AnalyzeLog___Win32_Server_Debug_FOR_JAPAN"
# PROP Intermediate_Dir "AnalyzeLog___Win32_Server_Debug_FOR_JAPAN"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /D "_FOR_JAPAN" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /I "../../common/common" /I "../../common/packet" /D "FOR_DOWNLOADSERVER" /D "_FOR_JAPAN" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib FTPClient.lib /nologo /subsystem:windows /debug /machine:I386 /out:"AnalyzeLog(Japan).exe" /pdbtype:sept
# ADD LINK32 winmm.lib FTPClient.lib /nologo /subsystem:windows /pdb:"Z:\540_Server_JPN\RedStone_LogServer\AnalyzeLogServer(Debug_Japan).pdb" /map:"Z:\540_Server_JPN\RedStone_LogServer\AnalyzeLogServer(Debug_Japan).map" /debug /machine:I386 /out:"Z:\540_Server_JPN\RedStone_LogServer\AnalyzeLogServer(Debug_Japan).exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 Server DebugForChina"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AnalyzeLog___Win32_Server_DebugForChina"
# PROP BASE Intermediate_Dir "AnalyzeLog___Win32_Server_DebugForChina"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "AnalyzeLog___Win32_Server_DebugForChina"
# PROP Intermediate_Dir "AnalyzeLog___Win32_Server_DebugForChina"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CHINA" /FR /YX"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /I "../../common/common" /I "../../common/packet" /D "FOR_DOWNLOADSERVER" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_CHINA" /FR /YX"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib FTPClient.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/AnalyzeLog(china).exe" /pdbtype:sept
# ADD LINK32 winmm.lib FTPClient.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/AnalyzeLog(china).exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 Server Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AnalyzeLog___Win32_Server_Release"
# PROP BASE Intermediate_Dir "AnalyzeLog___Win32_Server_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "AnalyzeLog___Win32_Server_Release"
# PROP Intermediate_Dir "AnalyzeLog___Win32_Server_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "../common" /I "../packet" /I "../REDSTONE_GAMESERVER" /D "_FOR_KOREA" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../common" /I "../packet" /I "../REDSTONE_GAMESERVER" /I "../../common/common" /I "../../common/packet" /D "FOR_DOWNLOADSERVER" /D "_FOR_KOREA" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ws2_32.lib winmm.lib FTPClient.lib /nologo /subsystem:windows /map /debug /debugtype:coff /machine:I386 /out:"Release/AnalyzeLog20040225.exe"
# ADD LINK32 ws2_32.lib winmm.lib FTPClient.lib /nologo /subsystem:windows /map /debug /debugtype:coff /machine:I386 /out:"Release/AnalyzeLogServer(Release).exe"

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 Server Release FOR JAPAN"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AnalyzeLog___Win32_Server_Release_FOR_JAPAN"
# PROP BASE Intermediate_Dir "AnalyzeLog___Win32_Server_Release_FOR_JAPAN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "AnalyzeLog___Win32_Server_Release_FOR_JAPAN"
# PROP Intermediate_Dir "AnalyzeLog___Win32_Server_Release_FOR_JAPAN"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "../common" /I "../packet" /I "../REDSTONE_GAMESERVER" /D "_FOR_JAPAN" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../common" /I "../packet" /I "../REDSTONE_GAMESERVER" /I "../../common/common" /I "../../common/packet" /D "FOR_DOWNLOADSERVER" /D "_FOR_JAPAN" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ws2_32.lib winmm.lib FTPClient.lib /nologo /subsystem:windows /map /debug /debugtype:coff /machine:I386 /out:"AnalyzeLog(forJapan)Release.exe"
# ADD LINK32 ws2_32.lib winmm.lib FTPClient.lib /nologo /subsystem:windows /map /debug /debugtype:coff /machine:I386 /out:"AnalyzeLogServer(Release_Japan).exe"

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 forUSA"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AnalyzeLog___Win32_forUSA"
# PROP BASE Intermediate_Dir "AnalyzeLog___Win32_forUSA"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "forUSA"
# PROP Intermediate_Dir "forUSA"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "../common" /I "../packet" /I "../REDSTONE_GAMESERVER" /D "_FOR_KOREA" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../common" /I "../packet" /I "../REDSTONE_GAMESERVER" /I "../../common/common" /I "../../common/packet" /D "_FOR_USA" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ws2_32.lib winmm.lib /nologo /subsystem:windows /map /debug /debugtype:coff /machine:I386 /out:"Release/AnalyzeLog(Release).exe"
# ADD LINK32 ws2_32.lib winmm.lib /nologo /subsystem:windows /map /debug /debugtype:coff /machine:I386 /out:"AnalyzeLog(USA).exe"

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 forThai"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AnalyzeLog___Win32_forThai"
# PROP BASE Intermediate_Dir "AnalyzeLog___Win32_forThai"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "forThai"
# PROP Intermediate_Dir "forThai"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_RSO" /FR /YX"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /I "../../common/common" /I "../../common/packet" /D "_FOR_THAI" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_RSO" /FR /YX"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x41e /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ws2_32.lib winmm.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"Debug/AnalyzeLog(Debug).exe" /pdbtype:sept
# ADD LINK32 ws2_32.lib winmm.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"AnalyzeLog(forThai).exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 forThaiServer"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AnalyzeLog___Win32_forThaiServer"
# PROP BASE Intermediate_Dir "AnalyzeLog___Win32_forThaiServer"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "AnalyzeLog___Win32_forThaiServer"
# PROP Intermediate_Dir "AnalyzeLog___Win32_forThaiServer"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_RSO" /FR /YX"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /I "../../common/common" /I "../../common/packet" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_RSO" /FR /YX"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ws2_32.lib winmm.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"Debug/AnalyzeLog(Debug).exe" /pdbtype:sept
# ADD LINK32 ws2_32.lib winmm.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"Debug/AnalyzeLog(Debug).exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 Debug FOR JAPAN"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AnalyzeLog___Win32_Debug_FOR_JAPAN0"
# PROP BASE Intermediate_Dir "AnalyzeLog___Win32_Debug_FOR_JAPAN0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "AnalyzeLog___Win32_Debug_FOR_JAPAN0"
# PROP Intermediate_Dir "AnalyzeLog___Win32_Debug_FOR_JAPAN0"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /D "_FOR_JAPAN" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_RSO" /FR /YX"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /I "../../common/common" /I "../../common/packet" /D "_FOR_JAPAN" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_RSO" /FR /YX"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib ws2_32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"AnalyzeLog(Debug_Japan).exe" /pdbtype:sept
# ADD LINK32 winmm.lib ws2_32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"AnalyzeLog(Debug_Japan).exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "AnalyzeLog - Win32 Release"
# Name "AnalyzeLog - Win32 Debug"
# Name "AnalyzeLog - Win32 DebugForChina"
# Name "AnalyzeLog - Win32 ReleaseForChina"
# Name "AnalyzeLog - Win32 Release FOR JAPAN"
# Name "AnalyzeLog - Win32 Server Debug"
# Name "AnalyzeLog - Win32 Server Debug FOR JAPAN"
# Name "AnalyzeLog - Win32 Server DebugForChina"
# Name "AnalyzeLog - Win32 Server Release"
# Name "AnalyzeLog - Win32 Server Release FOR JAPAN"
# Name "AnalyzeLog - Win32 forUSA"
# Name "AnalyzeLog - Win32 forThai"
# Name "AnalyzeLog - Win32 forThaiServer"
# Name "AnalyzeLog - Win32 Debug FOR JAPAN"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AnalyzeLog.cpp
# End Source File
# Begin Source File

SOURCE=.\AnalyzeLogDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\book.cpp
# End Source File
# Begin Source File

SOURCE=.\cADO_AnalyzeTool.cpp
# End Source File
# Begin Source File

SOURCE=.\CBmpMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\CDecoder.cpp
# End Source File
# Begin Source File

SOURCE=.\cITEM.cpp
# End Source File
# Begin Source File

SOURCE=.\CItemPrefix.cpp
# End Source File
# Begin Source File

SOURCE=.\cLOG_QUE.cpp
# End Source File
# Begin Source File

SOURCE=.\CMessage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cPetDefine.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadLog.cpp
# End Source File
# Begin Source File

SOURCE=.\file_buffer.cpp
# End Source File
# Begin Source File

SOURCE=.\job.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkedList.cpp
# End Source File
# Begin Source File

SOURCE=.\LOGLISTBOX.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\quest.cpp
# End Source File
# Begin Source File

SOURCE=.\setItem.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AnalyzeLog.h
# End Source File
# Begin Source File

SOURCE=.\AnalyzeLogDlg.h
# End Source File
# Begin Source File

SOURCE=.\book.h
# End Source File
# Begin Source File

SOURCE=.\buildVersion.h
# End Source File
# Begin Source File

SOURCE=.\cADO_AnalyzeTool.h
# End Source File
# Begin Source File

SOURCE=.\CBmpMenu.h
# End Source File
# Begin Source File

SOURCE=.\CDecoder.h
# End Source File
# Begin Source File

SOURCE=.\cITEM.h
# End Source File
# Begin Source File

SOURCE=.\cITEM_PREFIX.h
# End Source File
# Begin Source File

SOURCE=.\CItemPrefix.h
# End Source File
# Begin Source File

SOURCE=.\CMessage.h
# End Source File
# Begin Source File

SOURCE=.\DownloadLog.h
# End Source File
# Begin Source File

SOURCE=.\file_buffer.h
# End Source File
# Begin Source File

SOURCE=.\job.h
# End Source File
# Begin Source File

SOURCE=.\LinkedList.h
# End Source File
# Begin Source File

SOURCE=.\LOGLISTBOX.h
# End Source File
# Begin Source File

SOURCE=.\ProcessInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\quest.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\AnalyzeLog.ico
# End Source File
# Begin Source File

SOURCE=.\res\menutitl.bmp
# End Source File
# End Group
# Begin Group "FTPCLIENT"

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
# Begin Group "리소스 소스"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AnalyzeLog.rc
# End Source File
# Begin Source File

SOURCE=.\res\AnalyzeLog.rc2
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# End Group
# Begin Group "lib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\zdll.lib

!IF  "$(CFG)" == "AnalyzeLog - Win32 Release"

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 DebugForChina"

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 ReleaseForChina"

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 Release FOR JAPAN"

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 Server Debug"

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 Server Debug FOR JAPAN"

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 Server DebugForChina"

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 Server Release"

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 Server Release FOR JAPAN"

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 forUSA"

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 forThai"

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 forThaiServer"

!ELSEIF  "$(CFG)" == "AnalyzeLog - Win32 Debug FOR JAPAN"

!ENDIF 

# End Source File
# End Group
# Begin Group "로그 필터링"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\duel_filter.cpp
# End Source File
# Begin Source File

SOURCE=.\duel_filter.h
# End Source File
# Begin Source File

SOURCE=.\log_filtering.cpp
# End Source File
# Begin Source File

SOURCE=.\log_filtering.h
# End Source File
# End Group
# Begin Group "백업 코드"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LoadLogFile.cpp
# End Source File
# End Group
# Begin Group "로그 추출"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\exportLog.cpp
# End Source File
# Begin Source File

SOURCE=.\exportLog.h
# End Source File
# End Group
# Begin Group "공용"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\common\cNUX.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\commonDefine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cScript.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cSRVUTIL.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\Msjexhnd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\SFC.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\Tracer.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
