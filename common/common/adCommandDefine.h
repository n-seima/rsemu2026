#ifndef __AdminCommandDefine_H_
#define __AdminCommandDefine_H_

#include "SFC.h"
#include "definePacketData.h"

#define dCOMMAND_COUNT		7

#define dDEFAULT_TIME		"2000-01-01 01:01:01"
#define dDEFAULT_TEXT		"ET"

typedef enum
{
//	로그인 서버로..
	eCOMMANDTYPE_LOGINSERVER	= 0,	//	로그인 서버 닫기/열기
	eCOMMONDTYPE_CHANGE_VERSION,//	버젼 수정하기
	eCOMMANDTYPE_CHANGE_MAXUSER,//	최대 수용 인원수를 변경한다.
	
//	게임서버로..
	eCOMMANDTYPE_NOTICE			= 21,	//	공지하기
	eCOMMANDTYPE_BANISH,		//	추방하기 , 1회성이다. 다시 못들어오게 하기 위해서는 다른 조취가 필요하다.
	eCOMMANDTYPE_BROADCAST_MSG,	//	유저들이 올리는 방송용 메세지
	
//	월드서버로..
	eCOMMONTYPE_CLOSEWORLD		= 41,	//	월드 서버를 닫을 시간을 정한다.
	eCOMMONTYPE_RENAME_AVATAR,

	eCOMMONTYPE_MAINTENANCE_COUNT,
}eCOMMANDTYPE;

char *	g_CT_Text[];

class	cAdCommandData
{
public:
	int		idx;				//	db에 등록된 인덱스
	UTime	releasetime;		//	활성화 될 시간
	UTime	finishtime;			//	종료될 시간
	int		commandtype;		//	어떤 명령인가?
	int		commandtarget;		//	명령이 실행될 서버 인덱스
	int		param1,param2;		//	기타 값 1,2
	char	param3[32];			//	기타 값 3
	char	msg[dNOTICE_MAX_LENGTH];			//	전달할 메세지
	char	who[32];			//	누가 전달한 메세지일까?
	UTime	registedtime;		//	예약한 시간
	char	whod[32];			//	누가 삭제했나
	UTime	deltime;			//	언제 삭제한건가?
	int		deltype;			//	어떻게 삭제된걸까?

	cAdCommandData()
	{
		memset(this,0,sizeof(cAdCommandData));
	}
};

void	DatetimeToUTime(char * _datetime, UTime * _utime);
void	DBDatetimeToUTime(char * _datetime, UTime * _utime);
void	UTimeToDatetime(UTime * _utime, char * _datetime);



#endif	//	__AdminCommandDefine_H_