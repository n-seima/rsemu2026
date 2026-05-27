// Converter_Paran.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//////////////////////////////////////////////
//	PARAN 회원 이관 작업을 위해 작업한 어플
//	by 이지현 2007.10.9
//	USER DB, AVATAR DB에 연결된다.
//
//////////////////////////////////////////////

#include <process.h>
#include "cSOCKET.h"
#include "ADB_PARAN.h"

#include "cSRVUTIL.h"
#include "Tracer.h"

//#define dACCDB_IP	"211.189.66.53"
#define dACCDB_ID	"RedMaster"
#define dACCDB_PW	"CJFVKSRNDL"


//////////////////////////////////////////////
//	파란 유저 이관 신청 절차 
//  웹 -> Account DBC에 신규가입 -> bad 정보 수정 -> 완료
//	APP -> Bad정보 확인해서 처리안된 파란 유저 정보 확인 -> 모든 Avatar DB 뒤져서 데이터 컨버팅 -> Bad정보 풀구 -> (유저에게 통보할까?) -> 완료.
//	bad 정보 세팅 내역 : block : 다음날 , why_block : 파란회원 이관작업중입니다. 잠시 뒤에 다시 시도해 보시기 바랍니다.
//////////////////////////////////////////////

#define dACCDB_IP	"211.189.66.53"
#define dACCDB_ID	"RedMaster"
#define dACCDB_PW	"CJFVKSRNDL"
#define dDLMSG_DELETE_LIST		0x4721
#define dLDMSG_DELETE_LIST		0x4321		//	특정 유저의 리스트 파일 지우기

#pragma pack(1)
class	cMSG_BASE_TYPE_FORDB
{
public:
	WORD	wSize,				//	패킷 사이즈
			wType;				//	패킷 타입

	inline	void	set(WORD size,WORD type)	{wSize	=	size,wType	=	type;}
};

struct LDMSG_DELETE_LIST
{
	cMSG_BASE_TYPE_FORDB		base;

	char		strId[dID_LENGTH];
};

struct DLMSG_DELETE_LIST
{
	cMSG_BASE_TYPE_FORDB		base;

	char		strId[dID_LENGTH];	//	누구 리스트인지만 알려주면 되지뭐 ;;
};

#pragma pack()


int		g_iServerCount,g_iInfoCount;
char	g_strServerInfo[30][64] = {
//	"소룬드",	"211.189.66.57","RedMaster","CJFVKSRNDL","211.189.66.57",
	"프란델",	"211.189.66.61","RedMaster","CJFVKSRNDL","211.189.66.62",
	"에립트",	"211.189.66.69","RedMaster","CJFVKSRNDL","211.189.66.70",
	"제노스",	"211.189.66.76","RedMaster","CJFVKSRNDL","211.189.66.77",
	"가디우스",	"211.189.66.83","RedMaster","CJFVKSRNDL","211.189.66.84",
	"바헬",		"211.189.66.90","RedMaster","CJFVKSRNDL","211.189.66.91",
	"리트린",	"211.189.66.103","RedMaster","CJFVKSRNDL","211.189.66.55",
};

//	Paran_UserMoveList : 유저 이관 내용이 기록된 테이블
//	spAccInput : 유저들이 정한 내용을 Paran_UserMoveList에 저장하여 이관작업을 실행하도록 한다.
//	spAllAccList : 해당 서버에 접속하여 해당 계정이 가진 유저 리스트를 가져온다.


cADO_PARAN	_db[6];
cADO_PARAN	_accdb;


void
DeleteList(int iSelectedServerCur, char * strId)
{
	if(strcmp(strId,"")==0)
		return ;

	LDMSG_DELETE_LIST	delList;
	delList.base.set(sizeof(LDMSG_DELETE_LIST),dLDMSG_DELETE_LIST);
	strcpy(delList.strId,strId);
	cSOCKET	dbcsock;
	if( !dbcsock.init(dDBCACHE_PORT,g_strServerInfo[g_iInfoCount*iSelectedServerCur+4],"Ava DBC",sizeof(cMSG_BASE_TYPE_FORDB),TRUE) )
	{
		_log("AVA DBC [%s] 의 [%s] 리스트 갱신 실패!!!",g_strServerInfo[g_iInfoCount*iSelectedServerCur],strId);
		return;
	}
	//	DBC에 연결되었으면 할일을 한다.
	dbcsock.SendPacket((char *)&delList,delList.base.wSize);
	//	메세지가 돌아오기를 기다린다.
	int	sleepcount = 0;
	DLMSG_DELETE_LIST	sDelList;
	memset(&sDelList,0,sizeof(DLMSG_DELETE_LIST));

	char recvpacket[dMAX_PACKET_LENGTH];
	while(sleepcount<10000)
	{
		++sleepcount;
		if(dbcsock.GetRecvPacket(recvpacket))
		{
			_log("AVA DBC [%s] 의 [%s] 리스트 갱신 성공!!!",g_strServerInfo[g_iInfoCount*iSelectedServerCur],strId);
			break;
		}
		Sleep(1);
	}
	dbcsock.kill();
}


int main(int argc, char* argv[])
{
	g_iServerCount = 6;
	g_iInfoCount = 5;

	cLOG = new DebugTracer(DTType_ALL, "LogAboutYahoo.txt");
	_log(" Start Converting Tool----------------------------------------");

	//	시작 : 모든 db에 대한 연결을 테스트 한다.
	if(!_accdb.Connect( dACCDB_IP, dACCDB_ID, dACCDB_PW,"Redstone_id_source",1))
	{
		_log(" ACC DB Connect Error");
		goto END;
	}

	int i;
	for(i=0;i<g_iServerCount;i++)
	{
		if(!_db[i].Connect(g_strServerInfo[g_iInfoCount*i+1],g_strServerInfo[g_iInfoCount*i+2],g_strServerInfo[g_iInfoCount*i+3],"Redstone_Avatar_Source",1))
		{
			_log(" DB Connect Error : [%s] ",g_strServerInfo[g_iInfoCount*i] );
			goto END;
		}
	}
	//	끝 : 모든 db에 대한 연결을 테스트 한다.


	while(1)
	{
		Sleep(1);
		cParanInfo info = _accdb.getNewConvertingData();

		if(info.strParanId[0] == 0)
			continue;

		_log(" START Convert [%s -> %s] -------------------", info.strParanId, info.strNewId);
		FILE * fp = fopen("list_Yahoo.txt","a+");
		if(!fp)
			_log(" 리스트에 기록 불가..");
		else
		{
			fprintf(fp," %s , %s \n",info.strParanId, info.strNewId);
			fclose(fp); fp = NULL;
		}

		//	Account DB에 PvP 관련 정보를 수정한다.
		if(_accdb.convertPvPInfo(info.strParanId, info.strNewId))
		{
			_log("[%s->%s]의 PVP 정보 변경 완료.",info.strParanId,info.strNewId);
		}

		//	Avatar DB관련 정보를 수정한다.
		
		if(!info.isOverwrite)//	각 서버별로 돌아가면서...
		{
			for(i=0;i<g_iServerCount;i++)
			{
				_log("서버 [%s] 처리 시작........",g_strServerInfo[i*g_iInfoCount]);
				//_db[i].converting( &info , 1);
				DeleteList(i,info.strNewId);
				_log("서버 [%s] 처리 끝  ........",g_strServerInfo[i*g_iInfoCount]);
			}
		}else
		{
			_log("서버 [%s] [%s->%s]처리 시작........",g_strServerInfo[info.iServerIdx*g_iInfoCount],info.strParanId,info.strNewId);
			//_db[info.iServerIdx].converting( &info);
			DeleteList(info.iServerIdx,info.strNewId);
			_log("서버 [%s] 처리 끝........",g_strServerInfo[info.iServerIdx*g_iInfoCount]);
		}

		//	계정의 블럭 정보는 TRUE가 리턴되면 블럭을 해제한다.
		if(info.isLastData || !info.isOverwrite)
		{
			_log(" [%s] 유저의 블럭 정보 해제",info.strNewId);
			_accdb.unsetBlock(info.strNewId);
//			_accdb.unsetBlock(info.strParanId);
		}
		_accdb.finishConvertingWork(info.strNewId,info.iServerIdx);
		_log(" End   Convert [%s -> %s] -------------------", info.strParanId, info.strNewId);
	}

END: 
	for(i=0;i<g_iServerCount;i++)
		_db[i].Close();
	_accdb.Close();

	_log(" Finish Converting Tool---------------------------------------");
	return 0;
}

