#ifndef _LOG_H
#define _LOG_H

#include "cNEMO.H"
#include "cPACKET.H"
#include "CGame.H"

#define	dMAX_CHAT_LOG_COUNT		1024
#define	dACCUSE_TIME			1000*60
const	int	c_iLogChatLength	=	dCHAT_LENGTH;

class	CChatMessage
{
public:
	char		m_strName[dNAME_LENGTH];
	char		m_strMessage[c_iLogChatLength];
	WORD		m_wChatType;

	DWORD		mix();
};

class	CLogChatMessage : public CGame
{

	CChatMessage			m_aData[dMAX_CHAT_LOG_COUNT];
	CRITICAL_SECTION		m_csWork;
	int						m_iFirstQue,m_iLastQue,m_iCount;		//	큐 사이즈 첫번째,마지막 큐, 쌓여 있는 데이터 수

	DWORD					m_dwBookedChattingLogTime,m_dwBookedTradeLogTime;
	int						m_iChattingLogBeginLine,m_iTradeLogBeginLine;

	char					m_strOwnerName[dNAME_LENGTH],m_strTargetName[dNAME_LENGTH],m_strLogFileName[512];

public:
							CLogChatMessage();
							~CLogChatMessage();

	void					init();	//	초기화
	void					close();									//	종료
	void					reset();									//	리셋

	inline	int				getCount()				{return	m_iCount;}

	BOOL					add(char *_lpstrName,char *_lpstrMessage,int _iChatType);	//	패킷 추가

	void					update();
	char*					updateChattingLog();
	char*					updateTradeLog();
	
	BOOL					accuseChattingLog();
	BOOL					accuseTradeLog(char *_lpstrOwner,char *_lpstrTarget,BOOL _bIsPitchmanShop=FALSE);
	char*					writeChattingLog();
	char*					writeTradeLog();

};

extern	CLogChatMessage		g_logChat;

//
//	Loged Chat Message Type
enum
{
	eLCMT_CHAT,
	eLCMT_SHOUT,
	eLCMT_SEND_WHISPER,
	eLCMT_RECEIVE_WHISPER,
	eLCMT_GUILD,
	eLCMT_PARTY,
};

#endif