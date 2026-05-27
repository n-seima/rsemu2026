# Microsoft Developer Studio Project File - Name="Agent" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Agent - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Agent.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Agent.mak" CFG="Agent - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Agent - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Agent - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/server/Agent", MGHAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Agent - Win32 Release"

# PROP BASE Use_MFC 5
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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../common" /I "../packet" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib ws2_32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Agent - Win32 Debug"

# PROP BASE Use_MFC 5
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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../common" /I "../packet" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib ws2_32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Agent - Win32 Release"
# Name "Agent - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Agent.cpp
# End Source File
# Begin Source File

SOURCE=.\Agent.odl
# End Source File
# Begin Source File

SOURCE=.\AgentDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\client.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProxy.cpp
# End Source File
# Begin Source File

SOURCE=.\killProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\server_info.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Agent.h
# End Source File
# Begin Source File

SOURCE=.\AgentDlg.h
# End Source File
# Begin Source File

SOURCE=.\client.h
# End Source File
# Begin Source File

SOURCE=.\DlgProxy.h
# End Source File
# Begin Source File

SOURCE=.\killProcess.h
# End Source File
# Begin Source File

SOURCE=.\server_info.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Agent.ico
# End Source File
# End Group
# Begin Group "rc 관련"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Agent.rc
# End Source File
# Begin Source File

SOURCE=.\res\Agent.rc2
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# End Group
# Begin Group "쓰레드"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\accept.cpp
# End Source File
# Begin Source File

SOURCE=.\accept.h
# End Source File
# Begin Source File

SOURCE=.\LiveThread.cpp
# End Source File
# Begin Source File

SOURCE=.\LiveThread.h
# End Source File
# Begin Source File

SOURCE=.\LogInServerThread.cpp
# End Source File
# Begin Source File

SOURCE=.\LogInServerThread.h
# End Source File
# Begin Source File

SOURCE=.\ReconnectThread.cpp
# End Source File
# Begin Source File

SOURCE=.\ReconnectThread.h
# End Source File
# Begin Source File

SOURCE=.\WorkerThread.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkerThread.h
# End Source File
# End Group
# Begin Group "공용"

# PROP Default_Filter ""
# Begin Group "네트워크"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\packet\cPACKET_AGENT.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\cSOCKET.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\cSOCKET.h
# End Source File
# End Group
# Begin Group "버젼"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\reg_common.cpp
# End Source File
# Begin Source File

SOURCE=..\common\reg_common.h
# End Source File
# Begin Source File

SOURCE=..\common\server_type_define.cpp
# End Source File
# Begin Source File

SOURCE=..\common\server_type_define.h
# End Source File
# Begin Source File

SOURCE=..\common\version.cpp
# End Source File
# Begin Source File

SOURCE=..\common\version.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\common\common\cSRVUTIL.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\cSRVUTIL.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\PacketBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\common\PacketBuffer.h
# End Source File
# Begin Source File

SOURCE=..\common\SFC.cpp
# End Source File
# Begin Source File

SOURCE=..\common\SFC.h
# End Source File
# Begin Source File

SOURCE=..\..\common\common\Tracer.cpp
# End Source File
# Begin Source File

SOURCE=..\common\Tracer.h
# End Source File
# Begin Source File

SOURCE=..\common\TracerFromSaveLog.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Agent.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
