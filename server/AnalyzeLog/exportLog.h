#pragma	once

#include <WINDOWS.H>
#include <map>
#include <string>

class	cLevelAndExp
{
public:
	int			m_iLevel,m_iExp;
	BOOL		m_bIsRookie;
	int			m_iRebirthCount;
	CTimeInfo	m_time;
};

typedef	std::map<std::string,cLevelAndExp>	MapLevelLog;
typedef	std::pair<std::string,cLevelAndExp>	PairLevelLog;

extern	MapLevelLog	g_mapLevelForRebirthLog;

extern	void	exportItemFullInfo(char *_lpstrText,char *_lpstrComment,CItemDefine *_lpItem,BOOL _bIsExportComment=TRUE);
extern	void	exportItemInfo(char *_lpstrText,CItemDefine *_lpItem);

extern	BOOL	checkIsSerchData(char *_lpstrId,char *_lpstrName,char* _lpstrSerial,char* _lpstrIp,char *_lpstrCmpId,char *_lpstrCmpName,DWORD _dwCmpSerial,char *_lpstrCmpIp);
extern	void	makeItemPickLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText);
extern	BOOL	makeItemPickFullLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText);
extern	BOOL	makeCreateItemLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText);
extern	BOOL	makeCreateItemFullLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText);
extern	void	makeEnchantItemLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText);
extern	void	makeEnchantItemFullLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText);
extern	void	makeSystemLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText);
extern	void	makeSystemFullLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText,char *_lpstrComment,FILE *_lpFP);
extern	void	makePetLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText);
extern	void	makePetFullLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText);
extern	void	makeEventLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText);
extern	void	makeEventFullLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText,FILE *fp);

extern	void	makeGBWLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText);
extern	void	makeGBWFullLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText);

extern	void	makeGWLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText);
extern	void	makeGWFullLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText);


extern	void	makeDelItemLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText);
extern	void	makeDelItemFullLog(ALL_LOGMSG *_lpLog,char *_lpstrReturnText,char *_lpstrComment);

extern	BOOL	makeLogText(ALL_LOGMSG *_lpLog,char *_lpstrReturnText);
extern	BOOL	makeFullLogText(ALL_LOGMSG *_lpLog,char *_lpstrReturnText,char *_lpstrComment,FILE *fp=NULL);

extern	BOOL	g_bIsHideItemInfo;
extern	BOOL	g_bIsExportIncorrectPrefixItem;

inline	char*
GetMoneyString(LONGLONG _llMoney)
{
	static	char	s_strGold[40];
	BOOL	bIsAsia	=	FALSE;

#ifdef	_FOR_KOREA
	bIsAsia	=	TRUE;
#endif

	if (bIsAsia)
	{
		int	iUk	=	(int)(_llMoney/100000000),iMan=(int)((_llMoney/10000)%10000),iGold=(int)(_llMoney%10000);

		if	(iUk && iMan && iGold)
		{
			sprintf(s_strGold,"%d억 %d만 %d",iUk,iMan,iGold);
		}
		else
		if	(iUk && iMan)
		{
			sprintf(s_strGold,"%d억 %d만",iUk,iMan);
		}
		else
		if	(iUk && iGold)
		{
			sprintf(s_strGold,"%d억 %d",iUk,iGold);
		}
		else
		if	(iUk)
		{
			sprintf(s_strGold,"%d억",iUk);
		}
		else
		if	(iMan && iGold)
		{
			sprintf(s_strGold,"%d만 %d",iMan,iGold);
		}
		else
		if	(iMan)
		{
			sprintf(s_strGold,"%d만",iMan);
		}
		else
		{
			sprintf(s_strGold,"%d",iGold);
		}
	}
	else
	{
		int	iD3	=	(int)(_llMoney/1000000000),iUk	=	(int)((_llMoney/1000000)%1000),iMan=(int)((_llMoney/1000)%1000),iGold=(int)(_llMoney%1000);

		if	(iD3)
		{
			sprintf(s_strGold,"%d,%.3d,%.3d,%.3d",iD3,iUk,iMan,iGold);
		}
		else
		if	(iUk)
		{
			sprintf(s_strGold,"%d,%.3d,%.3d",iUk,iMan,iGold);
		}
		else
		if	(iMan)
		{
			sprintf(s_strGold,"%d,%.3d",iMan,iGold);
		}
		else
			sprintf(s_strGold,"%d",iGold);
	}

	return	s_strGold;
}
