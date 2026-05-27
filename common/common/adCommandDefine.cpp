#include "adCommandDefine.h"

char	*g_CT_Text[]=
{
#ifdef _FOR_KOREA
	"로그인 서버 닫기/열기",	//	eCOMMANDTYPE_LOGINSERVER
	"로그인 서버 버젼 수정하기",//	eCOMMONDTYPE_CHANGE_VERSION
	"로그인 서버 최대 수용 인원수 변경",//	eCOMMANDTYPE_CHANGE_MAXUSER

	"공지하기",					//	eCOMMANDTYPE_NOTICE
	"추방하기",					//	eCOMMANDTYPE_BANISH
	"유저들 방송용 메세지",		//	eCOMMANDTYPE_BROADCAST_MSG

	"정기점검 변경",		//eCOMMONTYPE_CLOSEWORLD
	"캐릭터명 변경",		//eCOMMONTYPE_RENAME_AVATAR
#else
	"LOGIN SERVER CLOSE/OPEN",
	"LOGIN SERVER CHANGING VERSION",
	"LOGIN SERVER CHANGING MAX USER COUNT",
	
	"NOTICE TO SERVER",
	"BANISH A CLIENT",
	"BROADCASTING OF A CLIENT",

	"Changing maintenance",
	"Changing Avatar Name",
#endif
};

void	UTimeToDatetime(UTime * _utime, char * _datetime)
{
	sprintf(_datetime,"%04d-%02d-%02d %02d:%02d:%02d",_utime->year+2000,_utime->month,_utime->day,_utime->hour,_utime->minute,_utime->second);
}
