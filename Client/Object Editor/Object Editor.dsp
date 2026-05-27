# Microsoft Developer Studio Project File - Name="Object Editor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Object Editor - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Object Editor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Object Editor.mak" CFG="Object Editor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Object Editor - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Object Editor - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Object Editor", QFBAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Object Editor - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"Object Editor.exe"

!ELSEIF  "$(CFG)" == "Object Editor - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Object Editor.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Object Editor - Win32 Release"
# Name "Object Editor - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\c16BITSPRITE.cpp
# End Source File
# Begin Source File

SOURCE=.\c8BITSPRITE.cpp
# End Source File
# Begin Source File

SOURCE=.\cANM.cpp
# End Source File
# Begin Source File

SOURCE=.\cBMP.cpp
# End Source File
# Begin Source File

SOURCE=.\cBMPWND.cpp
# End Source File
# Begin Source File

SOURCE=.\cDIB.cpp
# End Source File
# Begin Source File

SOURCE=.\cDRAW.cpp
# End Source File
# Begin Source File

SOURCE=.\cFRMWND.cpp
# End Source File
# Begin Source File

SOURCE=.\cIMAGE.cpp
# End Source File
# Begin Source File

SOURCE=.\cIMGWND.cpp
# End Source File
# Begin Source File

SOURCE=.\cMAIN.cpp
# End Source File
# Begin Source File

SOURCE=.\cPCX.cpp
# End Source File
# Begin Source File

SOURCE=.\cSET.cpp
# End Source File
# Begin Source File

SOURCE=.\cTEXT.cpp
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

SOURCE=.\cWORKWND.cpp
# End Source File
# Begin Source File

SOURCE=.\exceptionhandler.cpp
# End Source File
# Begin Source File

SOURCE=".\Object Editor.cpp"
# End Source File
# Begin Source File

SOURCE=".\Object Editor.rc"
# End Source File
# Begin Source File

SOURCE=.\SFC.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\cANM.h
# End Source File
# Begin Source File

SOURCE=.\cANM2.h
# End Source File
# Begin Source File

SOURCE=.\cBMPWND.h
# End Source File
# Begin Source File

SOURCE=.\cDIB.h
# End Source File
# Begin Source File

SOURCE=.\cDRAW.h
# End Source File
# Begin Source File

SOURCE=.\cFRMWND.h
# End Source File
# Begin Source File

SOURCE=.\cIMAGE.h
# End Source File
# Begin Source File

SOURCE=.\cIMGWND.h
# End Source File
# Begin Source File

SOURCE=.\cMAIN.h
# End Source File
# Begin Source File

SOURCE=.\cSET.h
# End Source File
# Begin Source File

SOURCE=.\cTEXT.h
# End Source File
# Begin Source File

SOURCE=.\cTRANS.h
# End Source File
# Begin Source File

SOURCE=.\cWINDOW.h
# End Source File
# Begin Source File

SOURCE=.\cWORKWND.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\SFC.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Resource\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\cursor2.cur
# End Source File
# Begin Source File

SOURCE=.\handcursor.cur
# End Source File
# Begin Source File

SOURCE=.\Leaf.ico
# End Source File
# Begin Source File

SOURCE=.\Resource\null.cur
# End Source File
# Begin Source File

SOURCE=.\spoidCursor.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\Tool.bmp
# End Source File
# End Group
# End Target
# End Project
