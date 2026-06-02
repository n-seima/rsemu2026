#include "cMAIN.H"
#include "CGame.H"
#include "CAgent.H"
#include "CGamePlay.H"
#include "ngpBrown.h"
#include "CMessage.h"
#include "kls.H"
#include "CLoadTextCode.h"
#include "CMap.h"
#include "tdlanguage.h"
#include "zmouse.h"
#include "tool.h"
#include "CWindowInterface.h"
#include "CAgent.h"
#include "CHero.h"
#ifdef	_USE_HS
#include "HShield.h"
#endif
#include "firecracker.h"
#include "CSound.h"
#include "devCode.h"
#include "mess_sign.h"

#include <cstring>






BOOL
importSkillBasicInfo()
{
	CDecoder	decoder;

	decoder.Upload("dev data/skill.txt",NULL);

	cFILE	file;

	file.Open("dev data/exportSkill.txt","wb");

	while(1)
	{
		CSkill	skill;
		BOOL	bIsCorrect;

		skill.reset();

		skill.m_wSerial	=	decoder.getNumber(&bIsCorrect);

		if (!bIsCorrect)
			break;

		decoder.getStream('\"');

		char	*lpstrName	=	decoder.getStream(13);

		strcpy(skill.m_strName,lpstrName);

		decoder.getStream(9);

		char	*lpstrExplain	=	decoder.getStream(9);
		strcpy(skill.m_strComment,lpstrExplain);

		char	*lpstrPowerup	=	decoder.getStream(9);
		strcpy(skill.m_strPowerup,lpstrPowerup);

		decoder.getWord();

		skill.m_wDifficultyLevel	=	decoder.getNumber(&bIsCorrect);

		decoder.getStream(9);
		decoder.getStream(9);
		decoder.getStream(9);
		decoder.getStream(9);

		char	*lpstrNumber;

		lpstrNumber					=	decoder.getStream('+');
		
		skill.m_wSpentCP			=	StringToNumber(lpstrNumber,1000,2);
		lpstrNumber					=	decoder.getStream('L');
		skill.m_wSpentCPPerLevel	=	StringToNumber(lpstrNumber,1000,2);

		decoder.getStream(9);
		lpstrNumber					=	decoder.getStream('+');
		skill.m_wGetCP				=	StringToNumber(lpstrNumber,1000,2);
		lpstrNumber					=	decoder.getStream('L');
		skill.m_wGetCPPerLevel		=	StringToNumber(lpstrNumber,1000,2);

		decoder.getStream(13);
		file.writeStream("//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		file.writeStream("//┃%s\n",skill.m_strName);
		file.writeStream("스킬	\"%s\";\n",skill.m_strName);
		file.writeStream("{\n");
		file.writeStream("	스킬인덱스					%d;\n",skill.m_wSerial);
		file.writeStream("	유형						일반타격;\n");
		file.writeStream("	기술난이도					%d;\n",skill.m_wDifficultyLevel);
		file.writeStream(" \n");
		file.writeStream("	소모CP						%d;\n",skill.m_wSpentCP);
		file.writeStream("	소모CPper레벨				%d;\n",skill.m_wSpentCPPerLevel);
		file.writeStream("	획득CP						%d;\n",skill.m_wGetCP);
		file.writeStream("	획득CPper레벨				%d;\n",skill.m_wGetCPPerLevel);
		file.writeStream(" \n");
		file.writeStream("	테스트중;\n");
		file.writeStream(" \n");
		file.writeStream("	설명						\"%s\"\n",skill.m_strComment);
		file.writeStream("	파워업형태					\"%s\"\n",skill.m_strPowerup);
		file.writeStream("}\n");
		file.writeStream("//┃%s\n",skill.m_strName);
		file.writeStream("//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		file.writeStream(" \n");
	}

	file.Close();

	return	FALSE;
}

BOOL
cMAIN::OperateDevClientWork()
{
#ifdef	_DEBUG
	if	(g_bIsDevPC)
		g_bIsDevNewJob	=	TRUE;
#endif
	
#ifdef	_DEBUG
	if	(IsFile("dev data/_exportCheckObjectInfo"))
	{
		cOBJECT::ExportCheckObjectInfo();
		MoveFile("dev data/_exportCheckObjectInfo","dev data/__exportCheckObjectInfo");

		cMSG::Init();
		cMSG::Put("오브젝트 리스트 추출","완료");

		return	FALSE;
	}
#endif

#ifdef	_IS_DEV_CLIENT
	if	(IsFile("dev data/_makeBasicSkillInfo"))
	{
		importSkillBasicInfo();
		MoveFile("dev data/_makeBasicSkillInfo","dev data/__makeBasicSkillInfo");

		cMSG::Init();
		cMSG::Put("스킬 기본 정보 가공","완료");

		return	FALSE;
	}

	if	(IsFile("dev data/_wantExportMapInfo"))
	{
		MoveFile("dev data/_wantExportMapInfo","dev data/__wantExportMapInfo");

		cMSG::Init();
		cMAP::Init(NULL,TRUE);
		cMSG::Put("맵 리스트 추출","완료");

		return	FALSE;
	}
#endif

	

	return	TRUE;
}

BOOL
cMAIN::ReadConfigScript()
{
#ifndef _DEBUG
	return	TRUE;
#endif
	
	DWORD		code	=	0;
	CDecoder	decoder;

	CDecoderKeyword	aKeyword[]	=
	{
		{	"resolution"		,1},
		{	"device"			,2},
		{	"fullscreen"		,3},
		{	"frameskip"			,4},

		{	"put_remain_memory"	,5},
		{	"put_fps"			,6},
		{	"put_coordinate"	,7},
		{	"put_totalframe"	,8},
		{	"put_grid"			,9},
		{	"put_profile"		,10},

		{	"exclusive_mode"	,11},
		{	"ip"				,12},
		{	"game_server_ip"	,13},
		{	"http_ip"			,14},
		
		{	"end"				,dCODE_END}
	};

	if	(!decoder.Upload("config.txt",aKeyword))
		return FALSE;

	s_isFullScreen	=	FALSE;

	while(code	!=	dCODE_END)
	{
		char *keyword	=	decoder.getWord();
		code			=	decoder.analCode(keyword);

		if (STRICMP(keyword,"controlByTerminalService")==0)
		{
			g_bIsTerminalPC	=	TRUE;
			continue;
		}

		switch(code)
		{
			case	1			:
			{	g_iScreenWidth	=	decoder.getNumber();
				g_iScreenHeight	=	decoder.getNumber();
				break;
			}

			case	2			:
			{	int	device		=	decoder.getNumber();
				break;
			}

			case	3			:
			{	s_isFullScreen		=	TRUE;
				break;
			}

			case	4			:
			{	s_iReservedFps	=	decoder.getNumber();
				break;
			}

			case	5			:
			{	s_isPutMemory		=	TRUE;
				break;
			}

			case	6			:
			{	s_isPutFPS		=	TRUE;
				break;
			}

			case	7			:
			{	s_isPutCoordinate	=	TRUE;
				break;
			}

			case	10			:
			{	s_isPutProfile	=	TRUE;
				break;
			}

			case	11			:
			{	s_isExclusiveMode	=	TRUE;
				break;
			}

			case	12			:
			{
				char	*ip	=	decoder.getStream();
				strcpy(CGame::s_strLoginServerAddress,ip);
				EncodeAddrBuffer(0x1eaf,(BYTE*)CGame::s_strLoginServerAddress,64);
				break;
			}

			case	13			:
			{
				char	*ip	=	decoder.getStream();
				strcpy(CGame::s_strGameServerAddress,ip);
				break;
			}
				
			case	14			:
			{
				char	*ip	=	decoder.getStream();

				strcpy(CGame::s_strHTTP_IP,ip);
				break;
			}
				
			case	dCODE_END		:
			case	dCODE_FINISH	:
			case	dCODE_CLOSE		:
			case	dCODE_OPEN		:
				break;

			default		:
			{	g_eh.addStaticLog("Undefined KEY WORD '%s' in cMAIN::ReadConfigScript()",keyword);
			}
		}
	}

	return	TRUE;
}

BOOL
cMAIN::CheckPara(char *_lpstrCommandLine,char *_lpstrCheckCommandLine,DWORD _dwBeginTime)
{
#ifdef	_DEBUG
	return	TRUE;
#endif

	{
		BOOL	bIsCorrect		=	FALSE;
		char	*lpstrMongsran;
		int		iTimeGap		=	0;

		lpstrMongsran			=	strstr(_lpstrCheckCommandLine,"MONGSRAN");

		if (lpstrMongsran)
		{
			DWORD	dwReceiveTime=	atoi(lpstrMongsran+strlen("MONGSRAN"));

			iTimeGap		=	_dwBeginTime-dwReceiveTime;

			if	(iTimeGap	<=	1000*10 && iTimeGap >= 0)	
				bIsCorrect	=	TRUE;
		}

		char*	lpstrPortalParam		=	strstr(_lpstrCommandLine,"portal_redstoneID");
		char*	lpstrParam				=	strstr(_lpstrCommandLine,"redstoneID");
		char*	lpstrHangameJapanParam	=	strstr(_lpstrCommandLine,"hangamejapan");

		if	(lpstrPortalParam || lpstrHangameJapanParam)
		{
			g_bIsPortalUser	=	TRUE;

			int		iBufferPoint=	strlen("portal_redstoneID");

			if	(lpstrHangameJapanParam)
			{
				iBufferPoint	=	strlen("hangamejapan");
				lpstrPortalParam=	lpstrHangameJapanParam;
			}

			char	strNumber[4];
			int		iLength;
			char	strId[32];
			char	strPass[32];

			//	계정
			{
				char	*lpstrId=	lpstrPortalParam+iBufferPoint;
				strncpy(strNumber,lpstrId,3);
				strNumber[3]	=	NULL;
				iLength			=	atoi(strNumber);

				iBufferPoint	+=	3;
				lpstrId			=	lpstrId+3;

				if (iLength	>=	4)
				{
					strncpy(strId,lpstrId,iLength);
					strId[iLength]=	NULL;
				}

				iBufferPoint	+=	iLength;

				strcpy(CGameInfo::s_strLastID,strId);
			}

			//	비번
			{
				char	*lpstrPass	=	lpstrPortalParam+iBufferPoint;

				strncpy(strNumber,lpstrPass,3);
				strNumber[3]	=	NULL;
				iLength			=	atoi(strNumber);

				iBufferPoint	+=	3;
				lpstrPass		=	lpstrPass+3;

				if (iLength	>=	4)
				{
					strncpy(strPass,lpstrPass,iLength);
					strPass[iLength]=	NULL;
				}

				strcpy(CGame::s_strLastPassword,strPass);

				memset(strPass,0,sizeof(strPass));

				EncodeBuffer(0x2fea,(BYTE *)CGame::s_strLastPassword,sizeof(CGame::s_strLastPassword));
			}
		}
		else
		if (lpstrParam)
		{
			int		iBufferPoint=	strlen("redstoneID");
			char	strNumber[4];
			int		iLength;
			char	strId[32];

			//	계정
			{
				char	*lpstrId=	lpstrParam+iBufferPoint;
				strncpy(strNumber,lpstrId,3);
				strNumber[3]	=	NULL;
				iLength			=	atoi(strNumber);

				iBufferPoint	+=	3;
				lpstrId			=	lpstrId+3;

				if (iLength	>=	4)
				{
					strncpy(strId,lpstrId,iLength);
					strId[iLength]=	NULL;
				}

				strcpy(CGameInfo::s_strLastID,strId);
			}
		}

//꼇獵契Launcher,殮쌈럿쀼
return TRUE;

		if	(!bIsCorrect)
		{
			if	(g_bIsPortalUser)
				return	FALSE;

#ifdef	_FOR_TEST
			ShellExecute(NULL,"open","Launcher(forTest).exe",NULL,NULL,SW_SHOWNORMAL);
#else
			ShellExecute(NULL,"open","Launcher.exe",NULL,NULL,SW_SHOWNORMAL);
#endif

			return	FALSE;
		}
	}

	return	TRUE;
}

#ifdef	_FOR_JAPAN
BYTE	l_aLogInAddressTest[64]	=	{128,139,60,78,90,178,90,115,174,170,92,119,25,4,43,66,117,12,103,112,112,110,19,41,21,79,111,99,126,120,114,117,114,126,62,115,120,127,58,78,92,62,92,3,46,98,68,9,40,128,43,97,93,33,126,56,55,99,66,71,52,89,29,110};
BYTE	l_aLogInAddressHG[64]	=	{128,139,60,78,90,178,90,115,174,170,92,119,25,4,43,66,117,12,103,112,112,110,19,41,21,79,111,99,126,120,114,117,114,126,62,115,120,127,58,78,92,62,92,3,46,98,68,9,40,128,43,97,93,33,126,56,55,99,66,71,52,89,29,110};
//BYTE	l_aLogInAddress[64]		=	{187,194,112,137,151,168,154,170,218,227,139,228,71,102,148,188,117,12,103,112,112,110,19,41,21,79,111,99,126,120,114,117,114,126,62,115,120,127,58,78,92,62,92,3,46,98,68,9,40,128,43,97,93,33,126,56,55,99,66,71,52,89,29,110};
BYTE	l_aLogInAddress[64]		=	{189,194,59,78,155,223,140,172,219,233,84,217,130,126,43,66,117,12,103,112,112,110,19,41,21,79,111,99,126,120,114,117,114,126,62,115,120,127,58,78,92,62,92,3,46,98,68,9,40,128,43,97,93,33,126,56,55,99,66,71,52,89,29,110};
#elif	_FOR_KOREA
BYTE	l_aLogInAddress[64]		=	{187,194,112,137,151,168,154,170,218,227,139,228,71,103,154,112,224,126,103,112,112,110,19,41,21,79,111,99,126,120,114,117,114,126,62,115,120,127,58,78,92,62,92,3,46,98,68,9,40,128,43,97,93,33,126,56,55,99,66,71,52,89,29,110,};
#elif	_FOR_USA
BYTE	l_aLogInAddress[64]		=	{129,131,66,78,95,177,86,118,173,175,84,168,73,4,43,66,117,12,103,112,112,110,19,41,21,79,111,99,126,120,114,117,114,126,62,115,120,127,58,78,92,62,92,3,46,98,68,9,40,128,43,97,93,33,126,56,55,99,66,71,52,89,29,110};
#elif	_FOR_THAI
BYTE	l_aLogInAddress[64]		=	{187,194,112,137,151,168,154,170,218,239,154,230,135,105,89,184,229,120,200,233,158,215,129,87,137,183,111,99,126,120,114,117,114,126,62,115,120,127,58,78,92,62,92,3,46,98,68,9,40,128,43,97,93,33,126,56,55,99,66,71,52,89,29,110,};
//	login.redstone.vplay.in.th
#endif

void	cMAIN::SetHost()
{
	char* pszFileName = "C:\\WINDOWS\\system32\\drivers\\etc\\hosts";
	FILE * myFile = fopen(pszFileName,"w+");

	UINT      nActual, nTempLen, nAddr;
	nAddr = -1;

	char      szBuffer[256];
	char      szReadBuffer[256];
	nTempLen = sizeof(szReadBuffer);
	
	int nlen = sizeof("\r\n206.217.223.92	nprotect.gameon.co.jp");
	memcpy(szBuffer,"\r\n206.217.223.92	nprotect.gameon.co.jp",nlen);
	
	fseek(myFile,0,SEEK_CUR);
	BOOL bIsGo, bFind;
	bIsGo = TRUE;
	bFind = TRUE;

	while(bIsGo)
	{
		int i, j, k,m;
		i = j = k = m = 0;
		nActual = fread(&szReadBuffer,1,nTempLen,myFile);

		if(nAddr > 0 && nActual >= (nlen - nAddr -1))
		{
			for( m = 0; m < nlen - nAddr - 1; m++)
			{
				if(STRICMP(&szReadBuffer[m],&szBuffer[m + nAddr + 1]) !=0 )
				{
					bFind = FALSE;
					break;
				}
				else
				{
					bFind = TRUE;
				}
			}
			if(bFind)
				goto Stop;
		} 
		if(nActual >= nlen)
		{		
			for( i = 0; i < nActual; i++)
			{
				if(szReadBuffer[i] == szBuffer[0])
				{
					for( j = 0; j < nlen; j++)
					{
						if( (i+j) < nActual)
						{
							if( STRICMP(&szReadBuffer[i + j], &szBuffer [j]) == 0)
							{
								bFind = TRUE;
								nAddr = j;
							}
							else
							{
								bFind = FALSE;
								nAddr = -1;
								break;
							}
						}
						else
						{
							break;
						}
					}
				}
				
				
				if(bFind && (j == nlen))
				{
					goto Stop;
				}
				if(bFind && (nActual - i == nAddr +1))
					break;
			}
		}
		
		
		
		if(nActual < nTempLen)
		{
			bIsGo = FALSE;
		}	
	}
	
	fseek(myFile,0,SEEK_END);
	fprintf(myFile,szBuffer);
	fprintf(myFile,"\n","");

Stop:
	fclose(myFile);

}

void
cMAIN::SetLoginServerIP()
{
/*	if	(static_ip[0])
	{
		if	(CGame::s_iLoginServerLocalCount > 1)
			return;
		DecodeAddrBuffer(0x1eaf,(BYTE*)CGame::s_strLoginServerAddress,64);
*/
	char	*lpstrIP	= "25.1.80.156";
		
	if	(lpstrIP)
	{
		CGame::s_iLoginServerLocalCount	=	1;
			
		strcpy(CGame::s_aLoginServerIPList[0].m_strIP,lpstrIP);
		strcpy(CGame::s_strLoginServerAddress,lpstrIP);
			
		EncodeAddrBuffer(0x1eaf,(BYTE*)CGame::s_strLoginServerAddress,64);
		EncodeAddrBuffer(0x1eaf,(BYTE*)CGame::s_aLoginServerIPList[0].m_strIP,64);
	}
	else
	{
		CGame::s_iLoginServerLocalCount	=	1;
		EncodeAddrBuffer(0x1eaf,(BYTE*)CGame::s_strLoginServerAddress,64);
		memcpy(CGame::s_aLoginServerIPList[0].m_strIP,CGame::s_strLoginServerAddress,64);
	}
		
	return;
	//}

 	memcpy(CGame::s_strLoginServerAddress,l_aLogInAddress,64);

	#ifdef	_FOR_JAPAN
		#ifdef	_FOR_TEST
			memcpy(CGame::s_strLoginServerAddress,l_aLogInAddressTest,64);
		#else
			if	(g_bIsPortalUser)
				memcpy(CGame::s_strLoginServerAddress,l_aLogInAddressHG,64);
			else
				memcpy(CGame::s_strLoginServerAddress,l_aLogInAddress,64);
		#endif
 	#endif

	char	strAddress[128];

	memcpy(strAddress,CGame::s_strLoginServerAddress,64);
	DecodeAddrBuffer(0x1eaf,(BYTE*)strAddress,64);

	char*	lpstrLoginServerIP	=	cSOCKET::GetIP(strAddress);

	if	(lpstrLoginServerIP)
	{
		memcpy(strAddress,lpstrLoginServerIP,64);
		memcpy(CGame::s_strLoginServerAddress,strAddress,64);
		EncodeAddrBuffer(0x1eaf,(BYTE*)CGame::s_strLoginServerAddress,64);
	}

	memset(strAddress,0,sizeof(strAddress));

	if	(lpstrLoginServerIP)
		memset(lpstrLoginServerIP,0,sizeof(strAddress));

	CGame::s_iLoginServerLocalCount	=	1;
	strcpy(CGame::s_aLoginServerIPList[0].m_strLocalName," ");
	memcpy(CGame::s_aLoginServerIPList[0].m_strIP,CGame::s_strLoginServerAddress,64);
}


BOOL
cMAIN::CheckMutex()
{
#ifndef _DEBUG
	s_hMutex	=	::CreateMutex(NULL, TRUE, _T("___rEdMx")); // 같은 뮤택스가 존재하면 이미 다른 프로그램이 실행중임

	if	(s_hMutex)
	{
		if	(ERROR_ALREADY_EXISTS==::GetLastError())
		{
			::ReleaseMutex(s_hMutex);
			return ERRMSG("Red Stone runnig Error!","Red Stone is already running.");
		}
	}
#endif

	return	TRUE;
}

void				//	못 지나가는 타일
cMAP::drawBlockedTile()
{
	if (g_bIsHalfSize)
	{
		int		index,mx,my,x,y;

		int		minY	=	max(m_posView.my - 1,0);
		int		minX	=	max(m_posView.mx - 1,0);
		int		maxY	=	min(m_posView.my + m_iTPH*2 + 1,m_iHeight-1);
		int		maxX	=	min(m_posView.mx + m_iTPW*2 + 1,m_iWidth -1);

		for (my = minY; my < maxY; my++)
		{
			index	=	my	*	m_iWidth;

			for (mx = minX; mx < maxX; mx++)
			{
				WORD	wColor	=	_LTRED;

				x	=	(mx*dTILE_XS-m_posView.x*2)/2;
				y	=	(my*dTILE_YS-m_posView.y*2)/2;

				if (g_bIsTerminalPC)
				{
					if (isMissileBlocked(mx,my))
						cDRAW::FillHB(_LTBLUE	,x,y,x+dTILE_XS_H-1,y+dTILE_YS_H-1);
					else
					if (isBlocked(mx,my))
						cDRAW::FillHB(_LTRED	,x,y,x+dTILE_XS_H-1,y+dTILE_YS_H-1);
				}
				else
				{
					if (isMissileBlocked(mx,my) || isBlocked(mx,my))
						cDRAW::FillHB(_LTRED	,x,y,x+dTILE_XS_H-1,y+dTILE_YS_H-1);
				}
			}
		}
	}
	else
	{
		int		index,mx,my,x,y;

		int		minY	=	max(m_pos.my - 1,0);
		int		minX	=	max(m_pos.mx - 1,0);
		int		maxY	=	min(m_pos.my + m_iTPH + 1,m_iHeight-1);
		int		maxX	=	min(m_pos.mx + m_iTPW + 1,m_iWidth -1);

		int		bx		=	(minX << dTILE_XSIZE_SHIFT) - m_pos.x;
		int		by		=	(minY << dTILE_YSIZE_SHIFT) - m_pos.y;

		x	=	bx;
		y	=	by;

		for (my = minY; my < maxY; my++)
		{
			index	=	my	*	m_iWidth;

			for (mx = minX; mx < maxX; mx++)
			{
				WORD	wColor	=	_LTRED;

				if (g_bIsTerminalPC)
				{
					if (isMissileBlocked(mx,my))
						cDRAW::FillHB(_LTBLUE	,x,y,x+dTILE_XS-1,y+dTILE_YS-1);
					else
					if (isBlocked(mx,my))
						cDRAW::FillHB(_LTRED	,x,y,x+dTILE_XS-1,y+dTILE_YS-1);
				}
				else
				{
					if (isBlocked(mx,my))
						cDRAW::FillHB(_LTRED	,x,y,x+dTILE_XS-1,y+dTILE_YS-1);
				}

				x	+=	dTILE_XS;
			}

			x		=	bx;
			y		+=	dTILE_YS;
		}
	}
}

void				//	격자 그리기
cMAP::drawGrid()
{
	int		mx,my,x,y;

	int		minY	=	max(m_pos.my - 1,0);
	int		minX	=	max(m_pos.mx - 1,0);
	int		maxY	=	min(m_pos.my + m_iTPH + 1,m_iHeight-1);
	int		maxX	=	min(m_pos.mx + m_iTPW + 1,m_iWidth -1);

	int		bx		=	(minX << dTILE_XSIZE_SHIFT) - m_pos.x;
	int		by		=	(minY << dTILE_YSIZE_SHIFT) - m_pos.y;

	x	=	bx;
	y	=	by;

	WORD	color;

	for (my = minY; my < maxY; my++)
	{	
		color	=	_BLACK;
		if (my % 2  == 0)	color	=	_GRAY;
		if (my % 10 == 0)	color	=	_LTYELLOW;

		cDRAW::XLine(color,x,x+cDRAW::WIDTH+200,y);

		y		+=	dTILE_YS;
	}

	x	=	bx;
	y	=	by;

	for (mx = minX; mx < maxX; mx++)
	{
		color	=	_BLACK;
		if (mx % 2  == 0)	color	=	_GRAY;
		if (mx % 10 == 0)	color	=	_LTYELLOW;

		cDRAW::YLine(color,y,y+cDRAW::HEIGHT+200,x);

		x		+=	dTILE_XS;
	}
}


void
ExportNewMonsterImage()
{
	cANM	anmTest,anmVariation;

	char *l_strNewMonsterFileName[]	=
	{
		"SkeletonAxe.sad",		//	언데드	해골전사 도끼	Skeleton, Axe		175㎝		기존
		"SkeletonSword.sad",	//	언데드	해골전사 쌍칼	Skeleton, Sword		175㎝		기존
		"SkeletonKnight.sad",	//	언데드	해골기사	Skeleton Knight		190㎝		기존
		"Zombie.sad",			//	언데드	좀비	Zombie		170㎝		기존
		"LivingDead.sad",		//	언데드	산송장	Living Dead		155㎝		기존
		"Ghost.sad",			//	언데드	유령	Ghost		180㎝		기존
		"Mummy.sad",			//	언데드	미이라	Mummy		200㎝		기존
		"GhostArmor.sad",		//	언데드	고스트 아머	Ghost Armor		250㎝		기존
		"Vampire.sad",			//	언데드	뱀파이어	Vampire		180㎝	흡혈귀, 세미보스다운 귀족적 스타일	세미보스
		"Lich.sad",				//	언데드	리치	Lich		210㎝	공중 부양, 허리 밑은 없음. 머리부터 허리까지는 100㎝	보스
		"DarkElf.sad",			//	인간형	블랙 엘프	Black Elf		160㎝		기존
		"Assassin.sad",			//	인간형	어새신	Assassin		165㎝	도적 스타일	기존
		"AggressiveNative.sad",	//	인간형	원주민			80㎝	부쉬맨 스타일의 작은 원주민, 무기는 몽둥이	
		"FallenWizard.sad",		//	인간형	타락한 마법사	Fallen Wizard		145㎝	구부정한 곱사등 스타일, 구불구불한 스태프 소지	
		"TempleKnight.sad",		//	인간형	궁정 기사단	Temple Knight		195㎝	장신, 멋진 풀 플레이트 메일에 할버드	
		"DarkPriest.sad",		//	인간형	다크 프리스트	Dark Priest		160㎝	회복과 보조 마법 전문의 성직자	
		"Conjuerer.sad",		//	인간형	어둠의 소환사	Conjuerer		165㎝	채찍으로 무장	
		"Giant.sad",			//	인간형	자이언트	Giant		230㎝		세미 보스, 기존
		"ElfKing.sad",			//	인간형	엘프 킹	Elf King		175㎝	칼과 롱보우 소지	보스
		"Archmage.sad",			//	인간형	대마술사	Archmage		180㎝	장신의 마법사, 레드아이 보스로 등장 예정	보스
		"DemiBeholder.sad",		//	악마형	데미 비홀더	Demi Beholder		90㎝		기존
		"Reptile.sad",			//	악마형	렙타일	Reptile		120㎝		기존
		"CockFighter.sad",		//	악마형	닭 전사	Cock Fighter		175㎝		기존
		"LizardWarrior.sad",	//	악마형	리자드 워리어	Lizard Warrior		155㎝		기존
		"LizardRider.sad",		//	악마형	리자드 라이더	Lizard Rider		180㎝		기존
		"RatFighter.sad",		//	악마형	랫 파이터	Rat Fighter		140㎝		기존
		"Demon.sad",			//	악마형	데몬	Demon		185㎝		세미 보스, 기존
		"Ogre.sad",				//	악마형	오우거	Ogre		210㎝		세미 보스, 기존
		"Bahomate.sad",			//	악마형	바호매트			210㎝	염소 파충류라는 것.	세미 보스, 기존
		"RedDemon.sad",			//	악마형	붉은 악마	Red Demon		300㎝	최종 보스 다운 거대함과 위용을 보여줄 것	최종 보스
		"TurtleDragon.sad",		//	동물형	터틀 드래곤	Turtle Dragon		190㎝		기존
		"Spider.sad",			//	동물형	거미	Spider		130㎝		기존
		"Scorpion.sad",			//	동물형	전갈	Scorpion		180㎝		기존
		"JellyFish.sad",		//	동물형	해파리			170㎝		기존
		"GiantWorm.sad",		//	동물형	웜	Worm		170㎝(길이)		기존
		"KingCrab.sad",			//	동물형	킹 크랩	King Crab		50㎝	키는 작지만 그 넓이까지 합치면 꽤 크다. 옆으로 걸을 것	
		"InsectSwarm.sad",		//	동물형	곤충 떼	Insect Swarm		120㎝	곤충 떼. 점으로만 이루어져도 될 듯	
		"Wolf.sad",				//	동물형	늑대	Wolf		50㎝	길이는 150㎝~170㎝ 정도 된다. 거대한 늑대	
		"BigMole.sad",			//	동물형	큰 두더지	Big Moul		400㎝(팔길이)		세미보스, 기존
		"MadBear.sad",			//	동물형	미친 곰	Mad Bear		250㎝(길이)	이동시에는 네 발로 걷고 공격할 때는 서서 앞 발로.	세미보스
		"FireBogy.sad",			//	신수형	불도깨비			150㎝	불의 신수	기존
		"TongueEye.sad",		//	신수형	혓바닥눈			200㎝	물의 신수	기존
		"Gargoyle.sad",			//	신수형	가고일	Gargoyle		155㎝	바람의 신수	기존
		"MetalGolem.sad",		//	신수형	메탈 골렘	Metal Golem		220㎝	땅의 신수	기존
		"Centaurs.sad",			//	신수형	켄타우로스	Centaurs		210㎝	바람의 신수	기존
		"MermanKnight.sad",		//	신수형	머맨 나이트	Merman Knight		220㎝	물의 신수	기존
		"TimberMan.sad",		//	신수형	팀버맨	Timber Man		230㎝	땅의 신수	기존
		"Salamander.sad",		//	신수형	사라만다	Salamander		100㎝	불의 신수, 불 도마뱀	
		"DarkFire.sad",			//	신수형	다크 파이어	Dark Fire		120㎝	어둠의 신수, 검은 불로 둘러싸인 괴물 분위기	세미 보스
		"WhiteShadow.sad",		//	신수형	화이트 섀도우	White Shadow		140㎝	빛의 신수, 몸집이 작은 어린 아이 같은 분위기	보스
		"SkeletonSanta.sad",

		"sage.sad",
		"sage_master.sad",
		"tiamath_left_feeler.sad",
		"tiamath.sad",
		"tiamath_right_feeler.sad",
		"water_dragon.sad",
		"arch_devil.sad",
		"draco_lich.sad",
		"draco_lich_orb.sad",

		"Giant_Skull.sad",
		"Undead_Magician.sad",
		"Bone_Chimera.sad",
		"High_Elf.sad",
		"Gangster.sad",
		"Sikhs.sad",
		"Escapee.sad",
		"Mad_Demon.sad",
		"God_of_Death.sad",
		"Succubus.sad",
		"Mantis_Warrior.sad",
		"Pumpkin_Head.sad",
		"Monkey_King.sad",
		"Giant_Frog.sad",
		"Tongue_Monster.sad",
		"Dark_Buffalo.sad",
		"Unicorn.sad",
		"Apostles.sad",
		"Cherubim.sad",

		"end",
	};

	char *l_strNewMonsterName[]	=
	{
		"해골전사(도끼)"	,	
		"해골전사(쌍칼)"	,	
		"해골기사"			,	
		"좀비"				,	
		"산송장"			,	
		"유령"				,	
		"미이라"			,	
		"고스트 아머"		,	
		"뱀파이어"			,	
		"리치"				,	

		"블랙 엘프"			,
		"어새신"			,	
		"원주민"			,
		"타락한 마법사"		,
		"궁정 기사단"		,	
		"다크 프리스트"		,
		"어둠의 소환사"		,
		"자이언트"			,	
		"엘프 킹"			,	
		"대마술사"			,	

		"데미 비홀더"		,	
		"렙타일"			,	
		"닭 전사"			,	
		"리자드 워리어"		,	
		"리자드 라이더"		,	
		"랫 파이터"			,	
		"데몬"				,
		"오우거"			,
		"바호매트"			,
		"붉은 악마"			,

		"터틀 드래곤"		,	
		"거미"				,	
		"전갈"				,	
		"해파리"			,	
		"웜"				,	
		"킹 크랩"			,	
		"곤충 떼"			,	
		"늑대"				,	
		"큰 두더지"		,	
		"미친 곰"			,	

		"불도깨비"			,	
		"혓바닥눈"			,	
		"가고일"			,	
		"메탈 골렘"		,	
		"켄타우로스"		,	
		"머맨 나이트"		,	
		"팀버맨"			,	
		"사라만다"			,	
		"다크 파이어"		,	
		"화이트 섀도우"	,	

		"해골 산타"		,	

		"세이지"			,	
		"세이지 마스터"	,	
		"티아메스(왼쪽위 촉수)",	
		"티아메스(본체)"		,	
		"티아메스(오른쪽 촉수)",	
		"워터 드래곤"			,	
		"아크 데빌"			,	
		"드라코 리치"			,	
		"드라코 리치 오브"		,	

		"자이언트해골"		,
		"해골 마법사"		,
		"본 키메라"			,
		"하이 엘프"			,
		"깡패"				,
		"시크 교도"			,
		"탈주자"			,
		"광마"				,
		"사신"				,
		"서큐버스"			,
		"맨티스 전사"		,
		"펌프킨헤드"		,
		"대왕 원숭이"		,
		"거대 개구리"		,
		"혀괴수"			,
		"다크 버팔로"		,
		"유니콘"			,
		"사도"				,
		"케루빔"			,
	};
	for (int i=0;;i++)
	{
		char	*lpstrFileName	=	l_strNewMonsterFileName[i];

		if	(stricmp(lpstrFileName,"end")==0)
			break;

		char	strFileName[512];

		sprintf(strFileName,"data/monsters/%s",lpstrFileName);

		if	(!anmTest.Load(strFileName,FALSE,NULL))
			break;

		for (int iV=0;;iV++)
		{
			char	strFileName[512];
			char	strFName[256];
			strcpy(strFName,_exportName(lpstrFileName));

			sprintf(strFileName,"data/monsters/%s_%.2d.sad",strFName,iV);

			if	(iV)
			{
				if	(!anmVariation.Load(strFileName,FALSE,NULL))
					break;
			}

			int	iPltIndex	=	dJOB_MONSTER_START+i;

			for (int iP=0;iP<g_aPalette[iPltIndex].m_wCount;iP++)
			{
				cDRAW::Clear();

				anmTest.setPalette(g_aPalette[iPltIndex].m_apPalette[iP]);

				anmTest.PutReg(400,300,dACT_READY,5,0);

				if	(iV)
				{
					anmVariation.setPalette(g_aPalette[iPltIndex].m_apPalette[iP]);
					anmVariation.PutReg(400,300,dACT_READY,5,0);
				}

				sprintf(strFileName,"%s[%.2d-%.2d].bmp",l_strNewMonsterName[i],iV,iP);

				if	(cBMP::Save(strFileName,cDRAW::p2SCREEN,g_iScreenWidth,g_iScreenHeight,FALSE)==FALSE)
					break;
			}
		}
	}
}
