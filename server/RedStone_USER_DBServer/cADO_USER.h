// cADO_USER.h: interface for the cADOClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CADO_USER_H__AD1D9ECD_B80F_427C_A43A_3B36C4087638__INCLUDED_)
#define AFX_CADO_USER_H__AD1D9ECD_B80F_427C_A43A_3B36C4087638__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "definePacketData.h"
#include "cADO_BASE.h"
#include "carrotShopDefine.h"
#include <map>
#include <string>

//	Create	ID
#define dSP_CREATEID_USEDID		-3
//	Delete	ID
#define dSP_DELETEID_NOTEXISTS	-3	//	아이디 혹은 비번이 틀림
#define dSP_DELETEID_USED		-4	//	사용중이라 삭제 불가능
//	Login
#define dSP_LOGIN_NOTEXISTS		"-3"	//	아이디 혹은 비번이 틀림
#define dSP_LOGIN_BADUSER		"-4"	//	불량 유저라 일정기간 사용못한다.

typedef struct	_sCREATEINFO	//	새로운 아이디 만들때 필요한 내용들... 나중에 자리 찾아가라.
{
	char	name[10];
	char	id[10];
	char	pw[10];
	int		ratetype;

	int		birthday;
	char	tel[18];
	char	phone[18];
	char	addr1[100];
	char	addr2[100];
	char	email[64];
}sCREATEINFO;

class cADO_USER	:	public cADO_BASE
{
public:
	cADO_USER();
	~cADO_USER();

	std::string	Login(char *id, char *pw,char * maccaddress, char * clientCode, char *sv,char *ip, char *why,WORD & isOper, BOOL& isRegistOTP,int isDis=TRUE);		//	접속중인걸 끊고 접속할지 여부도 첨부 
	std::string	WLogin(char *id,char *pw,char * maccaddress, char * clientCode, char *sv,char *ip, char *why,WORD & isOper,WORD & isNetCafe_User, BOOL& isRegistOTP, int isDis=TRUE);
	int		CheckOTP(char *id, char *otp, char *ip);
	int		Logout(char *id,char *ip);

	//	Carrot Shop
	int		GetCarrotShopInfo(cCarrotShopDefine * _cpCSInfo,int _isFirst = 0);
	//	Event Info  Added by 지현(2007.11.15)
	int		GetList_EventInGame(int * _list);
	int		InsertInfo_EventInGame(int _isreset, int _idx, char * _id, char * _name, int _max, int _isAllowDuplication, int &_lastValue);

	int		GetPvPDailyRanking(cDuelRankingInfo * _list);
	int		GetPvPWeeklyRanking(cDuelRankingInfo * _list);
	int		GetPvPData(char * _strId, cDuelRecord * _record);
	void	SetPvPData(char * _strId, cDuelRecord * _record);

	int		GetPvPTrialGameRanking(cDuelRankingInfo * _list);

	int		GetPvPTeamList(cDuelTeamInfo * _pList,std::map<int, int> & _list);		//	팀전체 리스트를 받는다.
	int		GetPvPTeamUserList(cDuelTeamInfo * _pList,std::map<int, int> & _list);	//	각 팀별 인원을 받는다.
	void	SetPvPTeamInfoInit();	//	전체의 초기화
	void	SetPvPTeamInfo(char * _strTeamName, cDuelTeamRecord * _record);
	void	initWeeklyPvPInfo();		//	주간 랭킹/WCG 랭킹을 산정 하고 모든 계정의 주간 전적을 초기화 시킨다.
	void	initDailyPvPRankingInfo();	//	일간 랭킹 정보 초기화
	void	updateDailyPvPRankingInfo();//	일간/WCG 랭킹 정보 산정

private:
	int		CreateID(sCREATEINFO * cinfo);
	int		ChangePass(char *id, char *oldpw, char *pw);
	int		DeleteID(char *id,char *pw);					//	항상 지우면 먼저 삭제Backup DB로 연결되고 거기서 6개월이 지나면 완전 삭제된다.

	//	Stored Procedure 를 사용한 메소드 
	int		spCreateId(sCREATEINFO * cinfo);
	int		spChangePass(char *id, char *oldpw, char *pw);
	int		spDeleteId(char *id,char *pw);
	std::string	spLogin(char *id, char *pw,char * maccaddress,char * clientCode,char *sv,char *ip, char *why,WORD & isOper,WORD & isNetCafe_User, BOOL& isRegistOTP, int isDis);
	int		spLogout(char *id,char *ip);
	int		spGetPoll(char *id);	//	마지막 참여한 pool인덱스를 어어온다.
	int		spSetPoll(char *id, int poll);	//	마지막 참여한 pool인덱스를 설정한다.

};

#endif // !defined(AFX_CADO_USER_H__AD1D9ECD_B80F_427C_A43A_3B36C4087638__INCLUDED_)
