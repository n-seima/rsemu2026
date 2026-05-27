# Microsoft Developer Studio Project File - Name="Red Stone" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Red Stone - Win32 forInner
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Red Stone.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Red Stone.mak" CFG="Red Stone - Win32 forInner"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Red Stone - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Red Stone - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Red Stone - Win32 forOperator" (based on "Win32 (x86) Application")
!MESSAGE "Red Stone - Win32 forJapanOperator" (based on "Win32 (x86) Application")
!MESSAGE "Red Stone - Win32 forJapan" (based on "Win32 (x86) Application")
!MESSAGE "Red Stone - Win32 TestForJapan" (based on "Win32 (x86) Application")
!MESSAGE "Red Stone - Win32 forUSAOperator" (based on "Win32 (x86) Application")
!MESSAGE "Red Stone - Win32 forUSA" (based on "Win32 (x86) Application")
!MESSAGE "Red Stone - Win32 forThaiOperator" (based on "Win32 (x86) Application")
!MESSAGE "Red Stone - Win32 forThai" (based on "Win32 (x86) Application")
!MESSAGE "Red Stone - Win32 forInner" (based on "Win32 (x86) Application")
!MESSAGE "Red Stone - Win32 DEV" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Red Stone", VCAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Red Stone - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "../engine" /I "../../common/packet" /I "../../common/common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_KOREA" /D "_WSPIAPI_COUNTOF" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmt.lib" /nodefaultlib:"libcmtd.lib" /out:"../../../../../Red Stone dev Client/Red Stone.exe" /libpath:"HShield"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Red Stone - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../engine" /I "../../common/packet" /I "../../common/common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_KOREA" /D "_IS_DEV_CLIENT" /D "_WSPIAPI_COUNTOF" /Fr /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBC" /out:"../../../../../Red Stone dev Client/Red Stone(Debug).exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forOperator"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Red_Stone___Win32_forOperator"
# PROP BASE Intermediate_Dir "Red_Stone___Win32_forOperator"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "forOperator"
# PROP Intermediate_Dir "forOperator"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "../engine" /I "../server/packet" /I "../server/common" /I "../common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../engine" /I "../../common/packet" /I "../../common/common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_KOREA" /D "_WSPIAPI_COUNTOF" /Fr /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"../붉은 보석/Red Stone(Debug).exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /incremental:no /map
# ADD LINK32 version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBC" /out:"../../../../../Red Stone dev Client/Red Stone(forOperator).exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /incremental:no

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapanOperator"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Red_Stone___Win32_forJapanOperator"
# PROP BASE Intermediate_Dir "Red_Stone___Win32_forJapanOperator"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "forJapanOperator"
# PROP Intermediate_Dir "forJapanOperator"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /I "../engine" /I "../server/packet" /I "../server/common" /I "../common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FORCHINA" /YX /FD /GZ /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../engine" /I "../../common/packet" /I "../../common/common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_JAPAN" /D "_WSPIAPI_COUNTOF" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib /nologo /subsystem:windows /incremental:no /debug /debugtype:both /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"../붉은 보석 for China/Red Stone(forChinaOperator).exe"
# SUBTRACT BASE LINK32 /map
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib /nologo /subsystem:windows /incremental:no /machine:I386 /nodefaultlib:"LIBCMT.lib" /nodefaultlib /out:"../../Red stone for Japan/Red Stone(forOperator).exe"
# SUBTRACT LINK32 /map /debug

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapan"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Red_Stone___Win32_forJapan"
# PROP BASE Intermediate_Dir "Red_Stone___Win32_forJapan"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "forJapan"
# PROP Intermediate_Dir "forJapan"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Zi /O2 /I "../engine" /I "../server/packet" /I "../server/common" /I "../common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FORKOREAN" /Fr /YX /FD /c
# ADD CPP /nologo /W3 /GX /Zi /O2 /I "../engine" /I "../../common/packet" /I "../../common/common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_JAPAN" /D "_WSPIAPI_COUNTOF" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"../붉은 보석/Red Stone.exe"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 imm32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"C:\Program Files\GameON\RED STONE\Red Stone.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Red Stone - Win32 TestForJapan"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Red_Stone___Win32_TestForJapan"
# PROP BASE Intermediate_Dir "Red_Stone___Win32_TestForJapan"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "TestForJapan"
# PROP Intermediate_Dir "TestForJapan"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Zi /O2 /I "../engine" /I "../server/packet" /I "../server/common" /I "../common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_JAPAN" /Fr /YX /FD /c
# ADD CPP /nologo /W3 /GX /Zi /O2 /I "../engine" /I "../../common/packet" /I "../../common/common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_JAPAN" /D "_FOR_TEST" /D "_WSPIAPI_COUNTOF" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"../Red stone for Japan/Red Stone.exe"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"../../Red stone for Japan/Red Stone(forTest).exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSAOperator"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Red_Stone___Win32_forUSAOperator"
# PROP BASE Intermediate_Dir "Red_Stone___Win32_forUSAOperator"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "forUSAOperator"
# PROP Intermediate_Dir "forUSAOperator"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /I "../engine" /I "../server/packet" /I "../server/common" /I "../common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_KOREA" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I "../engine" /I "../../common/packet" /I "../../common/common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_USA" /D "_FOR_ENG" /D "_WSPIAPI_COUNTOF" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib /nologo /subsystem:windows /incremental:no /debug /debugtype:both /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"../Red Stone dev Client/Red Stone(forOperator).exe"
# SUBTRACT BASE LINK32 /pdb:none /map
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib /nologo /subsystem:windows /incremental:no /debug /debugtype:both /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"../../../Red stone for USA/Red Stone(forOperator).exe"
# SUBTRACT LINK32 /pdb:none /map

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Red_Stone___Win32_forUSA"
# PROP BASE Intermediate_Dir "Red_Stone___Win32_forUSA"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "forUSA"
# PROP Intermediate_Dir "forUSA"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /O2 /I "hshield" /I "../engine" /I "../server/packet" /I "../server/common" /I "../common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_KOREA" /Fr /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "../engine" /I "../../common/packet" /I "../../common/common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_USA" /D "_FOR_ENG" /D "_WSPIAPI_COUNTOF" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 version.lib HShield.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /nodefaultlib:"LIBC.lib" /out:"../Red Stone dev Client/Red Stone.exe" /libpath:"HShield"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /nodefaultlib:"LIBC.lib" /out:"../../../Red stone for USA/Red Stone.exe" /libpath:"HShield"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThaiOperator"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Red_Stone___Win32_forThaiOperator"
# PROP BASE Intermediate_Dir "Red_Stone___Win32_forThaiOperator"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "forThaiOperator"
# PROP Intermediate_Dir "forThaiOperator"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /Gm /GX /Zi /Od /I "../engine" /I "../server/packet" /I "../server/common" /I "../common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_JAPAN" /YX /FD /GZ /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MT /W3 /Gm /GX /Zi /Od /I "../engine" /I "../../common/packet" /I "../../common/common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_THAI" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x41e /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib /nologo /subsystem:windows /incremental:no /debug /debugtype:both /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"../Red stone for Japan/Red Stone(forOperator).exe"
# SUBTRACT BASE LINK32 /map
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib /nologo /subsystem:windows /incremental:no /debug /debugtype:both /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"../../../Red stone for Thai/Red Stone(forOperator).exe"
# SUBTRACT LINK32 /map

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThai"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Red_Stone___Win32_forThai"
# PROP BASE Intermediate_Dir "Red_Stone___Win32_forThai"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "forThai"
# PROP Intermediate_Dir "forThai"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Zi /O2 /I "../engine" /I "../server/packet" /I "../server/common" /I "../common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_JAPAN" /Fr /YX /FD /c
# ADD CPP /nologo /W3 /GX /Zi /O2 /I "../engine" /I "../../common/packet" /I "../../common/common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_THAI" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x41e /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"../Red stone for Japan/Red Stone.exe"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"../../../Red stone for Thai/Red Stone.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forInner"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Red_Stone___Win32_forInner"
# PROP BASE Intermediate_Dir "Red_Stone___Win32_forInner"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Red_Stone___Win32_forInner"
# PROP Intermediate_Dir "Red_Stone___Win32_forInner"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../engine" /I "../../common/packet" /I "../../common/common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_KOREA" /D "_IS_DEV_CLIENT" /Fr /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../engine" /I "../../common/packet" /I "../../common/common" /D "_FOR_INNER" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_KOREA" /D "_WSPIAPI_COUNTOF" /Fr /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBC.lib" /out:"../../../../../Red Stone dev Client/Red Stone(Debug).exe" /pdbtype:sept /libpath:"HShield"
# SUBTRACT BASE LINK32 /incremental:no /map
# ADD LINK32 version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBC.lib" /out:"../../../../../Red Stone dev Client/Red Stone(forInner).exe" /pdbtype:sept /libpath:"HShield"
# SUBTRACT LINK32 /incremental:no /map

!ELSEIF  "$(CFG)" == "Red Stone - Win32 DEV"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Red_Stone___Win32_DEV"
# PROP BASE Intermediate_Dir "Red_Stone___Win32_DEV"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Red_Stone___Win32_DEV"
# PROP Intermediate_Dir "Red_Stone___Win32_DEV"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /O2 /I "../engine" /I "../../common/packet" /I "../../common/common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_KOREA" /D "_WSPIAPI_COUNTOF" /Fr /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../engine" /I "../../common/packet" /I "../../common/common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_FOR_KOREA" /D "_WSPIAPI_COUNTOF" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /nodefaultlib:"LIBC.lib" /out:"../../../../../Red Stone dev Client/Red Stone.exe" /libpath:"HShield"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib imm32.lib /nologo /subsystem:windows /incremental:yes /map /debug /machine:I386 /nodefaultlib:"LIBC.lib" /out:"../../../../../Red Stone dev Client/Red Stone.exe" /pdbtype:sept /libpath:"HShield"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Red Stone - Win32 Release"
# Name "Red Stone - Win32 Debug"
# Name "Red Stone - Win32 forOperator"
# Name "Red Stone - Win32 forJapanOperator"
# Name "Red Stone - Win32 forJapan"
# Name "Red Stone - Win32 TestForJapan"
# Name "Red Stone - Win32 forUSAOperator"
# Name "Red Stone - Win32 forUSA"
# Name "Red Stone - Win32 forThaiOperator"
# Name "Red Stone - Win32 forThai"
# Name "Red Stone - Win32 forInner"
# Name "Red Stone - Win32 DEV"
# Begin Group "_version"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\version.cpp
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BattleLog.cpp
# End Source File
# Begin Source File

SOURCE=.\CCompress.cpp
# End Source File
# Begin Source File

SOURCE=.\CDebugCode.cpp
# End Source File
# Begin Source File

SOURCE=.\CImageData.cpp
# End Source File
# Begin Source File

SOURCE=.\CLog.cpp
# End Source File
# Begin Source File

SOURCE=.\CMain.cpp
# End Source File
# Begin Source File

SOURCE=.\CMap.cpp
# End Source File
# Begin Source File

SOURCE=.\CMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\CObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cPetDefine.cpp
# End Source File
# Begin Source File

SOURCE=.\CSound.cpp
# End Source File
# Begin Source File

SOURCE=.\CTile.cpp
# End Source File
# Begin Source File

SOURCE=.\devCode.cpp
# End Source File
# Begin Source File

SOURCE=.\gg.cpp
# End Source File
# Begin Source File

SOURCE=.\mainForChina.cpp
# End Source File
# Begin Source File

SOURCE=.\minimap.cpp
# End Source File
# Begin Source File

SOURCE=".\Red Stone.cpp"
# End Source File
# Begin Source File

SOURCE=.\screen_focus.cpp
# End Source File
# Begin Source File

SOURCE=.\TDLanguage.cpp
# End Source File
# Begin Source File

SOURCE=.\tdspring_motion.cpp
# End Source File
# Begin Source File

SOURCE=.\TDUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\tool.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\banner.h
# End Source File
# Begin Source File

SOURCE=.\BattleLog.h
# End Source File
# Begin Source File

SOURCE=.\cACTION.H
# End Source File
# Begin Source File

SOURCE=.\cCOMPRESS.h
# End Source File
# Begin Source File

SOURCE=.\CImageData.h
# End Source File
# Begin Source File

SOURCE=.\CImageDataDefine.h
# End Source File
# Begin Source File

SOURCE=.\CLog.h
# End Source File
# Begin Source File

SOURCE=.\cMAIN.h
# End Source File
# Begin Source File

SOURCE=.\cMAP.h
# End Source File
# Begin Source File

SOURCE=.\cOBJECT.h
# End Source File
# Begin Source File

SOURCE=.\CSound.h
# End Source File
# Begin Source File

SOURCE=.\cTILE.h
# End Source File
# Begin Source File

SOURCE=.\cWEAPON.h
# End Source File
# Begin Source File

SOURCE=.\devCode.h
# End Source File
# Begin Source File

SOURCE=.\kls.h
# End Source File
# Begin Source File

SOURCE=.\ngpBrown.h
# End Source File
# Begin Source File

SOURCE=.\NPGameLib.h
# End Source File
# Begin Source File

SOURCE=.\screen_focus.h
# End Source File
# Begin Source File

SOURCE=.\speedhackchecker.h
# End Source File
# Begin Source File

SOURCE=.\ssl.h
# End Source File
# Begin Source File

SOURCE=.\TDLanguage.h
# End Source File
# Begin Source File

SOURCE=.\tdspring_motion.h
# End Source File
# Begin Source File

SOURCE=.\TDUtil.h
# End Source File
# Begin Source File

SOURCE=.\XTrap4Client.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\cur00002.cur
# End Source File
# Begin Source File

SOURCE=.\cur00003.cur
# End Source File
# Begin Source File

SOURCE=.\cur00004.cur
# End Source File
# Begin Source File

SOURCE=.\cur00005.cur
# End Source File
# Begin Source File

SOURCE=.\cur00006.cur
# End Source File
# Begin Source File

SOURCE=.\cursor.cur
# End Source File
# Begin Source File

SOURCE=.\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\disarm.cur
# End Source File
# Begin Source File

SOURCE=.\event.cur
# End Source File
# Begin Source File

SOURCE=.\normal_c.cur
# End Source File
# Begin Source File

SOURCE=.\open_door.cur
# End Source File
# Begin Source File

SOURCE=".\red stone.ico"
# End Source File
# Begin Source File

SOURCE=".\Red Stone.rc"
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\sword.cur
# End Source File
# Begin Source File

SOURCE=.\unlock.cur
# End Source File
# Begin Source File

SOURCE=.\warning1.cur
# End Source File
# Begin Source File

SOURCE=.\warning2.cur
# End Source File
# Begin Source File

SOURCE=.\warning3.cur
# End Source File
# Begin Source File

SOURCE=.\warning4.cur
# End Source File
# End Group
# Begin Group "Main Frame"

# PROP Default_Filter ""
# Begin Group "Select Avatar"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cGAME_SELECT_AVATAR.cpp
# End Source File
# Begin Source File

SOURCE=.\cGAME_SELECT_AVATAR.h
# End Source File
# End Group
# Begin Group "Create Avatar"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cGAME_CREATE_AVATAR.cpp
# End Source File
# Begin Source File

SOURCE=.\cGAME_CREATE_AVATAR.h
# End Source File
# End Group
# Begin Group "Credit"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cGAME_CREDIT.cpp
# End Source File
# Begin Source File

SOURCE=.\cGAME_CREDIT.h
# End Source File
# End Group
# Begin Group "Option"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cGAME_OPTION.cpp
# End Source File
# Begin Source File

SOURCE=.\cGAME_OPTION.h
# End Source File
# End Group
# Begin Group "Logo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cGAME_LOGO.cpp
# End Source File
# Begin Source File

SOURCE=.\cGAME_LOGO.h
# End Source File
# End Group
# Begin Group "Opening"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cGAME_OPENING.cpp
# End Source File
# Begin Source File

SOURCE=.\cGAME_OPENING.h
# End Source File
# End Group
# Begin Group "Endding"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cGAME_ENDDING.cpp
# End Source File
# Begin Source File

SOURCE=.\cGAME_ENDDING.h
# End Source File
# End Group
# Begin Group "Connect Game Server"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cGAME_CONNECT_GAME_SERVER.cpp
# End Source File
# Begin Source File

SOURCE=.\cGAME_CONNECT_GAME_SERVER.h
# End Source File
# End Group
# Begin Group "Title Screen"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cGAME_TITLE.cpp
# End Source File
# Begin Source File

SOURCE=.\cGAME_TITLE.h
# End Source File
# Begin Source File

SOURCE=.\titleScreenForMultiLoginServer.cpp
# End Source File
# Begin Source File

SOURCE=.\titleScreenWorldList.cpp
# End Source File
# End Group
# Begin Group "Close"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cGAME_CLOSE.cpp
# End Source File
# Begin Source File

SOURCE=.\cGAME_CLOSE.h
# End Source File
# End Group
# Begin Group "Initilize"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cGAME_INIT.cpp
# End Source File
# Begin Source File

SOURCE=.\cGAME_INIT.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CGame.cpp
# End Source File
# Begin Source File

SOURCE=.\cGAME.h
# End Source File
# Begin Source File

SOURCE=.\CGameInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CGameInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\CGameSaveOption.h
# End Source File
# Begin Source File

SOURCE=.\CGameUtil.cpp
# End Source File
# End Group
# Begin Group "Game Play"

# PROP Default_Filter ""
# Begin Group "Skill"

# PROP Default_Filter ""
# Begin Group "Skill Type"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\aidAttackSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\ast_chain_lightning.cpp
# End Source File
# Begin Source File

SOURCE=.\ast_chain_lightning.h
# End Source File
# Begin Source File

SOURCE=.\ast_fire_wall.cpp
# End Source File
# Begin Source File

SOURCE=.\ast_fire_wall.h
# End Source File
# Begin Source File

SOURCE=.\ast_giga_lightning.cpp
# End Source File
# Begin Source File

SOURCE=.\ast_one_bite.cpp
# End Source File
# Begin Source File

SOURCE=.\ast_one_bite.h
# End Source File
# Begin Source File

SOURCE=.\astArcMissile.cpp
# End Source File
# Begin Source File

SOURCE=.\astBigBullet.cpp
# End Source File
# Begin Source File

SOURCE=.\astBouncingMissile.cpp
# End Source File
# Begin Source File

SOURCE=.\astContinuousBit.cpp
# End Source File
# Begin Source File

SOURCE=.\astContinuousHitOfDoppelganger.cpp
# End Source File
# Begin Source File

SOURCE=.\astDoubleTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\astLightningWinder.cpp
# End Source File
# Begin Source File

SOURCE=.\astMultiMissile.cpp
# End Source File
# Begin Source File

SOURCE=.\astRollingLog.cpp
# End Source File
# Begin Source File

SOURCE=.\astWideMissile.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillAid.cpp
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

SOURCE=.\CSkillBunshine.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillCharge.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillContinuousAttack.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillContinuousShoot.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillContinuousWhirlwind.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillDropOnHead.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillExplosionDefendOnImage.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillExplosionMissile.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillFireBall.cpp
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

SOURCE=.\CSkillLinerChainExplosion.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillMachineMissile.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillMirrorTower.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillMissile.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillMotion.cpp
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

SOURCE=.\CSkillSpecialMissile.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillSpreadArrow.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillSurprisingRaid.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillTailChaser.cpp
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

SOURCE=.\CSkillWhirlthrusting.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillWhirlwind.cpp
# End Source File
# Begin Source File

SOURCE=.\gasTypeSkill.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\abilityToolTip.cpp
# End Source File
# Begin Source File

SOURCE=.\CAbility.cpp
# End Source File
# Begin Source File

SOURCE=.\CAbilityInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CActiveSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\CGuardianPost.cpp
# End Source File
# Begin Source File

SOURCE=.\CGuardianPost.h
# End Source File
# Begin Source File

SOURCE=.\CSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\cSKILL.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CSkillDefine.h
# End Source File
# Begin Source File

SOURCE=.\CSkillInit.cpp
# End Source File
# End Group
# Begin Group "Interface"

# PROP Default_Filter ""
# Begin Group "book window"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\book_gpw.cpp
# End Source File
# Begin Source File

SOURCE=.\book_gpw.h
# End Source File
# Begin Source File

SOURCE=.\book_MonsterDictionary.cpp
# End Source File
# Begin Source File

SOURCE=.\book_MonsterDictionary.h
# End Source File
# End Group
# Begin Group "World Map"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CWorldMap.cpp
# End Source File
# Begin Source File

SOURCE=.\CWorldMap.h
# End Source File
# Begin Source File

SOURCE=.\worldMapFieldInfo.cpp
# End Source File
# End Group
# Begin Group "Debug Window"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CDebugWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\CDebugWindow.h
# End Source File
# End Group
# Begin Group "Main Quest Event"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\main_quest_event.cpp
# End Source File
# Begin Source File

SOURCE=.\main_quest_event.h
# End Source File
# Begin Source File

SOURCE=.\main_quest_event_draw.cpp
# End Source File
# Begin Source File

SOURCE=.\main_quest_event_update.cpp
# End Source File
# End Group
# Begin Group "help"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\helpScreen.cpp
# End Source File
# Begin Source File

SOURCE=.\helpScreen.h
# End Source File
# End Group
# Begin Group "party window"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CPartyWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\CPartyWindow.h
# End Source File
# Begin Source File

SOURCE=.\partyListWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\requestJoinPartyUserListWindow.cpp
# End Source File
# End Group
# Begin Group "guild interface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CGuildBattleScheduler.cpp
# End Source File
# Begin Source File

SOURCE=.\CGuildBattleScheduler.h
# End Source File
# Begin Source File

SOURCE=.\CGuildMarkComposer.cpp
# End Source File
# Begin Source File

SOURCE=.\CGuildMarkComposer.h
# End Source File
# Begin Source File

SOURCE=.\CGuildWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\CGuildWindow.h
# End Source File
# Begin Source File

SOURCE=.\guild_feed_pet_window.cpp
# End Source File
# Begin Source File

SOURCE=.\guild_feed_pet_window.h
# End Source File
# Begin Source File

SOURCE=.\guild_hall_list.cpp
# End Source File
# Begin Source File

SOURCE=.\guild_hall_list.h
# End Source File
# Begin Source File

SOURCE=.\guild_honor_point_list.cpp
# End Source File
# Begin Source File

SOURCE=.\guild_honor_point_list.h
# End Source File
# Begin Source File

SOURCE=.\guild_inventory.cpp
# End Source File
# Begin Source File

SOURCE=.\guild_inventory.h
# End Source File
# Begin Source File

SOURCE=.\guild_related_place_list.cpp
# End Source File
# Begin Source File

SOURCE=.\guild_related_place_list.h
# End Source File
# End Group
# Begin Group "dialog box"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\dialog_box.cpp
# End Source File
# Begin Source File

SOURCE=.\dialog_box.h
# End Source File
# End Group
# Begin Group "그외 인터페이스"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CFriendWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\CFriendWindow.h
# End Source File
# Begin Source File

SOURCE=.\CMakeGuildWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\CMakeGuildWindow.h
# End Source File
# Begin Source File

SOURCE=.\CPetWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\CPetWindow.h
# End Source File
# Begin Source File

SOURCE=.\CQuestWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\CQuestWindow.h
# End Source File
# Begin Source File

SOURCE=.\CSelectVillageWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\CSelectVillageWindow.h
# End Source File
# Begin Source File

SOURCE=.\CStatusWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\CSystemWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\CWindowInterface.h
# End Source File
# Begin Source File

SOURCE=.\putBanner.cpp
# End Source File
# Begin Source File

SOURCE=.\putBanner.h
# End Source File
# End Group
# Begin Group "Message Box"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CGamePlayChat.cpp
# End Source File
# Begin Source File

SOURCE=.\CGamePlayNotice.cpp
# End Source File
# Begin Source File

SOURCE=.\CGamePlaySystemMessage.cpp
# End Source File
# End Group
# Begin Group "Item Window"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bottomless_box_gpw.cpp
# End Source File
# Begin Source File

SOURCE=.\bottomless_box_gpw.h
# End Source File
# Begin Source File

SOURCE=.\carrotShop.cpp
# End Source File
# Begin Source File

SOURCE=.\carrotShop.h
# End Source File
# Begin Source File

SOURCE=.\CBankWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\CBankWindow.h
# End Source File
# Begin Source File

SOURCE=.\CCartWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\CCartWindow.h
# End Source File
# Begin Source File

SOURCE=.\cCopyOrCutPrefixWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\cCopyOrCutPrefixWnd.h
# End Source File
# Begin Source File

SOURCE=.\CInventoryWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\CPitchmanShopWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\CPitchmanShopWindow.h
# End Source File
# Begin Source File

SOURCE=.\CRepairItemWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\CRepairItemWindow.h
# End Source File
# Begin Source File

SOURCE=.\CShopWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\CShopWindow.h
# End Source File
# Begin Source File

SOURCE=.\CTradeWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\CUpgradeShop.cpp
# End Source File
# Begin Source File

SOURCE=.\CUpgradeShop.h
# End Source File
# Begin Source File

SOURCE=.\judgeToItemWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\judgeToItemWnd.h
# End Source File
# Begin Source File

SOURCE=.\mergeItemWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\mergeItemWnd.h
# End Source File
# Begin Source File

SOURCE=.\removePrefixWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\removePrefixWnd.h
# End Source File
# Begin Source File

SOURCE=.\selectItemInPack.cpp
# End Source File
# Begin Source File

SOURCE=.\selectItemInPack.h
# End Source File
# End Group
# Begin Group "미니펫 창"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\miniPetWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\miniPetWindow.h
# End Source File
# End Group
# Begin Group "스킬"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CSkillWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkillWindow.h
# End Source File
# Begin Source File

SOURCE=.\easySkillTree.cpp
# End Source File
# Begin Source File

SOURCE=.\easySkillTree.h
# End Source File
# End Group
# Begin Group "공용 리스트 윈도우"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\commonUI.cpp
# End Source File
# Begin Source File

SOURCE=.\commonUI.h
# End Source File
# Begin Source File

SOURCE=.\duel_ranking_ui.cpp
# End Source File
# Begin Source File

SOURCE=.\listWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\listWindow.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CGameBottomInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\CGamePlayInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\CGamePlayInterface.h
# End Source File
# Begin Source File

SOURCE=.\CGamePlayMinimap.cpp
# End Source File
# Begin Source File

SOURCE=.\FogMap.cpp
# End Source File
# End Group
# Begin Group "Quest"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CQuest.cpp
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
# End Group
# Begin Group "Item"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CFieldItem.cpp
# End Source File
# Begin Source File

SOURCE=.\CFieldItem.h
# End Source File
# Begin Source File

SOURCE=.\CItem.cpp
# End Source File
# Begin Source File

SOURCE=.\cITEM.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CItemDefine.h
# End Source File
# Begin Source File

SOURCE=.\CItemEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\CItemEffect.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CItemEffectDefine.h
# End Source File
# Begin Source File

SOURCE=.\CItemPrefix.cpp
# End Source File
# Begin Source File

SOURCE=.\CItemPrefix.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\itemDefine.cpp
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
# Begin Source File

SOURCE=.\questItem.cpp
# End Source File
# End Group
# Begin Group "Actor"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\common\actorDefine.h
# End Source File
# Begin Source File

SOURCE=.\CActor.cpp
# End Source File
# Begin Source File

SOURCE=.\cACTOR.H
# End Source File
# Begin Source File

SOURCE=.\CActorAction.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorAfterImage.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorAnmInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorHit.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorInline.h
# End Source File
# Begin Source File

SOURCE=.\CActorManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorMove.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorOperatePacket.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorPut.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\CActorWrongStatus.cpp
# End Source File
# End Group
# Begin Group "Hero"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CHero.cpp
# End Source File
# Begin Source File

SOURCE=.\cHERO.h
# End Source File
# Begin Source File

SOURCE=.\CHeroBattle.cpp
# End Source File
# Begin Source File

SOURCE=.\CHeroGuildSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\CHeroInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CHeroInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\CHeroItem.cpp
# End Source File
# Begin Source File

SOURCE=.\CHeroItemEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\CHeroMove.cpp
# End Source File
# Begin Source File

SOURCE=.\CHeroPet.cpp
# End Source File
# Begin Source File

SOURCE=.\CHeroQuest.cpp
# End Source File
# Begin Source File

SOURCE=.\CHeroSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\CHeroSkillExtraEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\CHeroStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\CHeroTitle.cpp
# End Source File
# Begin Source File

SOURCE=.\CHeroUseSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\CHeroWrongStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\CPlayerStatus.h
# End Source File
# Begin Source File

SOURCE=.\hero_area.cpp
# End Source File
# Begin Source File

SOURCE=.\hero_old_data.cpp
# End Source File
# Begin Source File

SOURCE=.\hero_trap.cpp
# End Source File
# Begin Source File

SOURCE=.\user_custom_data.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\userCustomDataDefine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\userCustomDataDefine.h
# End Source File
# End Group
# Begin Group "Object"

# PROP Default_Filter ""
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

SOURCE=.\CGuildMark.cpp
# End Source File
# Begin Source File

SOURCE=.\CGuildSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\CGuildSkill.h
# End Source File
# Begin Source File

SOURCE=.\CGuildSkillManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\guild_common.cpp
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
# Begin Source File

SOURCE=.\CTitleManager.cpp
# End Source File
# End Group
# Begin Group "Event"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CDialog.h
# End Source File
# Begin Source File

SOURCE=.\CEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\CEvent.h
# End Source File
# Begin Source File

SOURCE=.\CShop.h
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

SOURCE=.\cKARMA_VALUE.cpp
# End Source File
# Begin Source File

SOURCE=.\cKARMA_VALUE.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\CKarmaDefine.h
# End Source File
# End Group
# Begin Group "Effect"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CColorEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\CColorEffect.h
# End Source File
# Begin Source File

SOURCE=.\CHitEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\CHitEffect.h
# End Source File
# Begin Source File

SOURCE=.\CShake.cpp
# End Source File
# Begin Source File

SOURCE=.\cSHAKE.h
# End Source File
# Begin Source File

SOURCE=.\effectManager.cpp
# End Source File
# Begin Source File

SOURCE=.\effectManager.h
# End Source File
# Begin Source File

SOURCE=.\firecracker.cpp
# End Source File
# Begin Source File

SOURCE=.\firecracker.h
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
# Begin Group "banner"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\banner.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\bannerDefine.h
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
# Begin Group "area"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\common\areaDefine.h
# End Source File
# Begin Source File

SOURCE=.\CArea.cpp
# End Source File
# Begin Source File

SOURCE=.\cAREA.h
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
# Begin Group "secret dungeon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\secretDungeon.cpp
# End Source File
# Begin Source File

SOURCE=.\secretDungeon.h
# End Source File
# End Group
# Begin Group "party"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CParty.cpp
# End Source File
# Begin Source File

SOURCE=.\CParty.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\partyDefine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\partyDefine.h
# End Source File
# Begin Source File

SOURCE=.\partyList.cpp
# End Source File
# End Group
# Begin Group "trap"

# PROP Default_Filter ""
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
# Begin Source File

SOURCE=.\trapSkill.h
# End Source File
# End Group
# Begin Group "shopping cart class"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CShoppingCart.cpp
# End Source File
# Begin Source File

SOURCE=.\CShoppingCart.h
# End Source File
# End Group
# Begin Group "노점상"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CPitchmanShop.cpp
# End Source File
# Begin Source File

SOURCE=.\CPitchmanShop.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\common\common\carrotShopDefine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\carrotShopDefine.h
# End Source File
# Begin Source File

SOURCE=.\CPortal.cpp
# End Source File
# Begin Source File

SOURCE=.\CPortal.h
# End Source File
# Begin Source File

SOURCE=.\CTrade.cpp
# End Source File
# Begin Source File

SOURCE=.\CTrade.h
# End Source File
# Begin Source File

SOURCE=.\shop.cpp
# End Source File
# Begin Source File

SOURCE=.\summonBeast.h
# End Source File
# Begin Source File

SOURCE=.\tool.h
# End Source File
# End Group
# Begin Group "job"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CJob.cpp
# End Source File
# Begin Source File

SOURCE=.\cJOB.H
# End Source File
# Begin Source File

SOURCE=..\..\common\common\jobDefine.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\playerJobCommon.cpp
# End Source File
# Begin Source File

SOURCE=..\..\common\common\playerJobCommon.h
# End Source File
# End Group
# Begin Group "pet"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CPet.h
# End Source File
# End Group
# Begin Group "Boss Monster Event"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bm_arch_devil.cpp
# End Source File
# Begin Source File

SOURCE=.\bm_arch_devil.h
# End Source File
# Begin Source File

SOURCE=.\bm_draco_lich.cpp
# End Source File
# Begin Source File

SOURCE=.\bm_draco_lich.h
# End Source File
# Begin Source File

SOURCE=.\bm_event.cpp
# End Source File
# Begin Source File

SOURCE=.\bm_event.h
# End Source File
# Begin Source File

SOURCE=.\bm_sage_event.cpp
# End Source File
# Begin Source File

SOURCE=.\bm_sage_event.h
# End Source File
# Begin Source File

SOURCE=.\bm_tiamath.cpp
# End Source File
# Begin Source File

SOURCE=.\bm_tiamath.h
# End Source File
# Begin Source File

SOURCE=.\bm_water_dragon.cpp
# End Source File
# Begin Source File

SOURCE=.\bm_water_dragon.h
# End Source File
# End Group
# Begin Group "미니펫"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\hero_mini_pet.cpp
# End Source File
# Begin Source File

SOURCE=.\hero_mini_pet.h
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
# Begin Group "길드"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=.\CGamePlay.cpp
# End Source File
# Begin Source File

SOURCE=.\CGamePlay.h
# End Source File
# Begin Source File

SOURCE=.\CGamePlayDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\CGamePlayMain.cpp
# End Source File
# Begin Source File

SOURCE=.\CGamePlayParty.cpp
# End Source File
# Begin Source File

SOURCE=.\CGamePlayUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\gpPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\gpWarningMessage.cpp
# End Source File
# End Group
# Begin Group "Project"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cPROJECT.cpp
# End Source File
# Begin Source File

SOURCE=.\cPROJECT.h
# End Source File
# End Group
# Begin Group "Agent"

# PROP Default_Filter ""
# Begin Group "Operate Packet"

# PROP Default_Filter ""
# Begin Group "Trade"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CAgentReceiveTradePacket.cpp
# End Source File
# Begin Source File

SOURCE=.\CAgentSendTradePacket.cpp
# End Source File
# End Group
# Begin Group "Bank Packet"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CAgentReceiveBankPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\CAgentSendBankPacket.cpp
# End Source File
# End Group
# Begin Group "Item Packet"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CAgentReceiveItemPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\CAgentSendItemPacket.cpp
# End Source File
# End Group
# Begin Group "Event Packet"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CAgentReceiveEventPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\CAgentSendEventPacket.cpp
# End Source File
# End Group
# Begin Group "Community Packet"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CAgentReceiveCommunityPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\CAgentSendCommunityPacket.cpp
# End Source File
# End Group
# Begin Group "Guild Packet"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CAgentReceiveGuildPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\CAgentSendGuildPacket.cpp
# End Source File
# End Group
# Begin Group "Shopping Cart"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CAgentReceiveCartPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\CAgentSendCartPacket.cpp
# End Source File
# End Group
# Begin Group "Pitchman Shop"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CAgentReceivePitchmanShop.cpp
# End Source File
# Begin Source File

SOURCE=.\CAgentSendPichmanShopPacket.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\CAgentReceiveGamePacket.cpp
# End Source File
# Begin Source File

SOURCE=.\CAgentReceivePacket.cpp
# End Source File
# Begin Source File

SOURCE=.\CAgentReceivePetPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\CAgentReceiveSkillPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\CAgentSendPacket.cpp
# End Source File
# Begin Source File

SOURCE=.\receiveTrapPacket.cpp
# End Source File
# End Group
# Begin Group "Packet Define"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cPACKET.h
# End Source File
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
# Begin Source File

SOURCE=.\cAGENT.cpp
# End Source File
# Begin Source File

SOURCE=.\cAGENT.h
# End Source File
# Begin Source File

SOURCE=.\cPACKET_QUE.cpp
# End Source File
# Begin Source File

SOURCE=.\mac.cpp
# End Source File
# Begin Source File

SOURCE=.\mac.h
# End Source File
# End Group
# Begin Group "Base Code"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BASETSD.H
# End Source File
# Begin Source File

SOURCE=..\..\common\common\RSComm.h
# End Source File
# Begin Source File

SOURCE=.\STDAFX.H
# End Source File
# End Group
# Begin Group "Text Script"

# PROP Default_Filter ""
# Begin Group "simple Text"

# PROP Default_Filter ""
# Begin Source File

SOURCE="text\common.txt"
# End Source File
# Begin Source File

SOURCE="text\equipmentAllignment.txt"
# End Source File
# Begin Source File

SOURCE="text\equipmentName.txt"
# End Source File
# Begin Source File

SOURCE="text\eventMob.txt"
# End Source File
# Begin Source File

SOURCE="text\extraItemGrade.txt"
# End Source File
# Begin Source File

SOURCE="text\GreateGodGrace.txt"
# End Source File
# Begin Source File

SOURCE="text\guildRank.txt"
# End Source File
# Begin Source File

SOURCE="text\heroJobText.txt"
# End Source File
# Begin Source File

SOURCE="text\innerHelp.txt"
# End Source File
# Begin Source File

SOURCE="text\magicResistance.txt"
# End Source File
# Begin Source File

SOURCE="text\MonsterDic.txt"
# End Source File
# Begin Source File

SOURCE="text\monsterGrade.txt"
# End Source File
# Begin Source File

SOURCE="text\monsterType.txt"
# End Source File
# Begin Source File

SOURCE="text\npcName.txt"
# End Source File
# Begin Source File

SOURCE="text\power.txt"
# End Source File
# Begin Source File

SOURCE="text\server.txt"
# End Source File
# Begin Source File

SOURCE="text\slanderFilter.txt"
# End Source File
# Begin Source File

SOURCE="text\statusToolTip.txt"
# End Source File
# Begin Source File

SOURCE="text\tip.txt"
# End Source File
# End Group
# Begin Group "script text"

# PROP Default_Filter ""
# Begin Source File

SOURCE="text\_fontName.txt"
# End Source File
# Begin Source File

SOURCE="text\debug.txt"
# End Source File
# End Group
# Begin Source File

SOURCE=.\CLoadTextCode.cpp
# End Source File
# Begin Source File

SOURCE=.\CLoadTextCode.h
# End Source File
# Begin Source File

SOURCE=.\cMESSAGE.h
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
# Begin Group "movie"

# PROP Default_Filter ""
# Begin Group "anm_player"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\movie_anm_player.cpp
# End Source File
# Begin Source File

SOURCE=.\movie_anm_player.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\movie.cpp
# End Source File
# Begin Source File

SOURCE=.\movie.h
# End Source File
# Begin Source File

SOURCE=.\movie_event.cpp
# End Source File
# Begin Source File

SOURCE=.\movie_frame.cpp
# End Source File
# Begin Source File

SOURCE=.\movie_frame_operate.cpp
# End Source File
# Begin Source File

SOURCE=.\movie_io.cpp
# End Source File
# Begin Source File

SOURCE=.\screen_filter.cpp
# End Source File
# Begin Source File

SOURCE=.\screen_filter.h
# End Source File
# End Group
# Begin Group "Lib & DLL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ZipArchive.lib

!IF  "$(CFG)" == "Red Stone - Win32 Release"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapanOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 TestForJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSAOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSA"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThaiOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThai"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forInner"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 DEV"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\dkls.lib

!IF  "$(CFG)" == "Red Stone - Win32 Release"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapanOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 TestForJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSAOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSA"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThaiOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThai"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forInner"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 DEV"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\kls.lib

!IF  "$(CFG)" == "Red Stone - Win32 Release"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapanOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 TestForJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSAOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSA"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThaiOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThai"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forInner"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 DEV"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ngpBlue.lib

!IF  "$(CFG)" == "Red Stone - Win32 Release"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapanOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 TestForJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSAOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSA"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThaiOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThai"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forInner"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 DEV"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ngpBrown.lib

!IF  "$(CFG)" == "Red Stone - Win32 Release"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapanOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 TestForJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSAOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSA"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThaiOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThai"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forInner"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 DEV"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NPGameLib.lib

!IF  "$(CFG)" == "Red Stone - Win32 Release"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapanOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 TestForJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSAOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSA"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThaiOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThai"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forInner"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 DEV"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SpeedHackCheckerDll.lib

!IF  "$(CFG)" == "Red Stone - Win32 Release"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapanOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 TestForJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSAOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSA"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThaiOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThai"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forInner"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 DEV"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ssl.lib

!IF  "$(CFG)" == "Red Stone - Win32 Release"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapanOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 TestForJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSAOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSA"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThaiOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThai"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forInner"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 DEV"

!ENDIF 

# End Source File
# End Group
# Begin Group "작업 메뉴얼"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Include.zip
# End Source File
# Begin Source File

SOURCE=".\RS인수인계.xls"
# End Source File
# Begin Source File

SOURCE=".\VS 설치후 해야 할 일.TXT"
# End Source File
# Begin Source File

SOURCE=".\맵 데이터 추가 관련 필요작업.txt"
# End Source File
# Begin Source File

SOURCE=".\일본 업데이트 전달 사항.txt"
# End Source File
# Begin Source File

SOURCE=".\일본 예외 사항.txt"
# End Source File
# Begin Source File

SOURCE=".\직업 추가 관련 필요 작업.txt"
# End Source File
# End Group
# Begin Group "XTrap"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XTrap\Xtrap_C_Interface.h
# End Source File
# Begin Source File

SOURCE=.\XTrap\XTrap4Client_ClientPart_mt.lib

!IF  "$(CFG)" == "Red Stone - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 Debug"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapanOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 TestForJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSAOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSA"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThaiOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThai"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forInner"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 DEV"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XTrap\XTrap4Client_mt.lib

!IF  "$(CFG)" == "Red Stone - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 Debug"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapanOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 TestForJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSAOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSA"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThaiOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThai"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forInner"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 DEV"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XTrap\XTrap4Client_ServerPart_mt.lib

!IF  "$(CFG)" == "Red Stone - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 Debug"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapanOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 TestForJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSAOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSA"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThaiOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThai"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forInner"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 DEV"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XTrap\XTrap_Unipher_mt.lib

!IF  "$(CFG)" == "Red Stone - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 Debug"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapanOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 TestForJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSAOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSA"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThaiOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThai"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forInner"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 DEV"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "보안 코드"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\common\common\mess_sign.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\CClientGameType.cpp
# End Source File
# Begin Source File

SOURCE="..\Red Stone dev Client\config.txt"

!IF  "$(CFG)" == "Red Stone - Win32 Release"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapanOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 TestForJapan"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSAOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forUSA"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThaiOperator"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forThai"

!ELSEIF  "$(CFG)" == "Red Stone - Win32 forInner"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Red Stone - Win32 DEV"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GVG.TXT
# End Source File
# Begin Source File

SOURCE=".\Red Stone.exe.manifest"
# End Source File
# End Target
# End Project
