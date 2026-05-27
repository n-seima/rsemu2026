# Microsoft Developer Studio Project File - Name="common" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=common - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "common.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "common.mak" CFG="common - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "common - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "common - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/RedStone/common", YJAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "common - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../redstone_gameserver/" /D "_FOR_KOREA" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "common - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../redstone_gameserver/" /D "_FOR_KOREA" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "common - Win32 Release"
# Name "common - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\adCommandDefine.cpp
# End Source File
# Begin Source File

SOURCE=.\cADO_BASE.cpp
# End Source File
# Begin Source File

SOURCE=.\CCompress.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckNic.cpp
# End Source File
# Begin Source File

SOURCE=.\cListManage.cpp
# End Source File
# Begin Source File

SOURCE=.\cPetDefine.cpp
# End Source File
# Begin Source File

SOURCE=.\cProfile.cpp
# End Source File
# Begin Source File

SOURCE=.\cScript.cpp
# End Source File
# Begin Source File

SOURCE=.\cServerStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\CSharedMemory.cpp
# End Source File
# Begin Source File

SOURCE=.\cSRVUTIL.cpp
# End Source File
# Begin Source File

SOURCE=.\eventDefine.cpp
# End Source File
# Begin Source File

SOURCE=.\ExScript.cpp
# End Source File
# Begin Source File

SOURCE=.\FTPclient.cpp
# End Source File
# Begin Source File

SOURCE=.\HardwareInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Msjexhnd.cpp
# End Source File
# Begin Source File

SOURCE=.\PacketBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\regular_random.cpp
# End Source File
# Begin Source File

SOURCE=.\seedex.cpp
# End Source File
# Begin Source File

SOURCE=.\SFC.cpp
# End Source File
# Begin Source File

SOURCE=.\SMTP.cpp
# End Source File
# Begin Source File

SOURCE=.\Tracer.cpp
# End Source File
# Begin Source File

SOURCE=.\TracerFromSaveLog.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\adCommandDefine.h
# End Source File
# Begin Source File

SOURCE=.\buildVersion.h
# End Source File
# Begin Source File

SOURCE=.\cADO_BASE.h
# End Source File
# Begin Source File

SOURCE=.\CCompress.h
# End Source File
# Begin Source File

SOURCE=.\CheckNic.h
# End Source File
# Begin Source File

SOURCE=.\cListManage.h
# End Source File
# Begin Source File

SOURCE=.\cProfile.h
# End Source File
# Begin Source File

SOURCE=.\cScript.h
# End Source File
# Begin Source File

SOURCE=.\cServerStatus.h
# End Source File
# Begin Source File

SOURCE=.\CSharedMemory.h
# End Source File
# Begin Source File

SOURCE=.\cSRVUTIL.h
# End Source File
# Begin Source File

SOURCE=.\ExScript.h
# End Source File
# Begin Source File

SOURCE=.\FTPclient.h
# End Source File
# Begin Source File

SOURCE=.\HardwareInfo.h
# End Source File
# Begin Source File

SOURCE=.\Msjexhnd.h
# End Source File
# Begin Source File

SOURCE=.\PacketBuffer.h
# End Source File
# Begin Source File

SOURCE=.\RSComm.h
# End Source File
# Begin Source File

SOURCE=.\seedex.h
# End Source File
# Begin Source File

SOURCE=.\SFC.h
# End Source File
# Begin Source File

SOURCE=.\SMTP.h
# End Source File
# Begin Source File

SOURCE=.\ssl.h
# End Source File
# Begin Source File

SOURCE=.\STACK.H
# End Source File
# Begin Source File

SOURCE=.\StringWrapper.h
# End Source File
# Begin Source File

SOURCE=.\Tracer.h
# End Source File
# Begin Source File

SOURCE=.\TracerFromSaveLog.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "life_code"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\life_code_common.cpp
# End Source File
# Begin Source File

SOURCE=.\life_code_common.h
# End Source File
# End Group
# Begin Group "소켓"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cSOCKET.cpp
# End Source File
# Begin Source File

SOURCE=.\cSOCKET.h
# End Source File
# End Group
# Begin Group "게임"

# PROP Default_Filter ""
# Begin Group "아이템"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CItemDefine.h
# End Source File
# Begin Source File

SOURCE=.\CItemEffectDefine.h
# End Source File
# Begin Source File

SOURCE=.\itemDefine.cpp
# End Source File
# End Group
# Begin Group "길드"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CGuildDefine.h
# End Source File
# Begin Source File

SOURCE=.\cGuildManager.cpp
# End Source File
# Begin Source File

SOURCE=.\cGuildManager.h
# End Source File
# Begin Source File

SOURCE=.\guild_common.cpp
# End Source File
# Begin Source File

SOURCE=.\guild_skill_define.h
# End Source File
# End Group
# Begin Group "스킬"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\CSkill.h
# End Source File
# Begin Source File

SOURCE=.\CSkillDefine.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\actorDefine.h
# End Source File
# Begin Source File

SOURCE=.\arcaDefine.h
# End Source File
# Begin Source File

SOURCE=.\areaDefine.h
# End Source File
# Begin Source File

SOURCE=.\bannerDefine.h
# End Source File
# Begin Source File

SOURCE=.\book_define.h
# End Source File
# Begin Source File

SOURCE=.\carrotShopDefine.h
# End Source File
# Begin Source File

SOURCE=.\CImageDataDefine.h
# End Source File
# Begin Source File

SOURCE=.\CKarmaDefine.h
# End Source File
# Begin Source File

SOURCE=.\cPartyManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CPitchmanShopDefine.h
# End Source File
# Begin Source File

SOURCE=.\cPLAYER_DATA.cpp
# End Source File
# Begin Source File

SOURCE=.\cPLAYER_DATA.h
# End Source File
# Begin Source File

SOURCE=.\doorDefine.h
# End Source File
# Begin Source File

SOURCE=.\eventDefine.h
# End Source File
# Begin Source File

SOURCE=.\field_common.h
# End Source File
# Begin Source File

SOURCE=.\jobCommon.h
# End Source File
# Begin Source File

SOURCE=.\jobDefine.h
# End Source File
# Begin Source File

SOURCE=.\miniPetCommon.h
# End Source File
# Begin Source File

SOURCE=.\partyDefine.cpp
# End Source File
# Begin Source File

SOURCE=.\partyDefine.h
# End Source File
# Begin Source File

SOURCE=.\pattern_define.h
# End Source File
# Begin Source File

SOURCE=.\playerJobCommon.cpp
# End Source File
# Begin Source File

SOURCE=.\playerJobCommon.h
# End Source File
# Begin Source File

SOURCE=.\poll_common.h
# End Source File
# Begin Source File

SOURCE=.\quest_common.h
# End Source File
# Begin Source File

SOURCE=.\rebirth_define.h
# End Source File
# Begin Source File

SOURCE=.\regular_random.h
# End Source File
# Begin Source File

SOURCE=.\summonBeastDefine.h
# End Source File
# Begin Source File

SOURCE=.\trapDefine.h
# End Source File
# Begin Source File

SOURCE=.\treasureMapDefine.h
# End Source File
# Begin Source File

SOURCE=.\userCustomDataDefine.cpp
# End Source File
# Begin Source File

SOURCE=.\userCustomDataDefine.h
# End Source File
# End Group
# Begin Group "관리"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\reg_common.cpp
# End Source File
# Begin Source File

SOURCE=.\reg_common.h
# End Source File
# Begin Source File

SOURCE=.\server_type_define.cpp
# End Source File
# Begin Source File

SOURCE=.\server_type_define.h
# End Source File
# Begin Source File

SOURCE=.\version.cpp
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# End Group
# Begin Group "콘솔 입력 받기"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\inputConsoleCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\inputConsoleCommand.h
# End Source File
# End Group
# Begin Group "보안"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\mess_sign.cpp
# End Source File
# Begin Source File

SOURCE=.\mess_sign.h
# End Source File
# End Group
# Begin Source File

SOURCE="..\각종 길이제한 참조.txt"
# End Source File
# Begin Source File

SOURCE="..\서버 수정 사항.txt"
# End Source File
# End Target
# End Project
