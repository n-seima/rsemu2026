// cADOClass.h: interface for the cADOClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CADO_AVATAR_H__AD1D9ECD_B80F_427C_A43A_3B36C4087638__INCLUDED_)
#define AFX_CADO_AVATAR_H__AD1D9ECD_B80F_427C_A43A_3B36C4087638__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "definePacketData.h"
#include <icrsint.h> 
#include "cADO_BASE.h"
#include "cPACKET_DBSERVER.h"
#include "cPLAYER_DATA.h"
#include "eventDefine.h"

//	DEFINE for spCreateAva
#define dSP_CREATEAVA_USEDNAME			-3	//사용중인 이름이다.
#define dSP_CREATEAVA_FULL				-4	//더이상 아바타를 만들수 없다.
#define dSP_CREATEAVA_INCORRRECT_IDX	-5	//잘못된 인덱스
//	DEFINE for spDeleteAva`
#define	dSP_DELETEAVA_NOMATCH			-3	//이름이나 일련번호가 일치하지 않는다.

typedef struct 
{
	DWORD	dwIndex[dMAX_AVATAR_COUNT];
	WORD	wAvatarIndex[dMAX_AVATAR_COUNT];
	char	strName[dMAX_AVATAR_COUNT][dNAME_LENGTH];
}_DBINDEXTABLE;

class cADO_AVATAR	:	public cADO_BASE
{
public:
	cADO_AVATAR();
	virtual ~cADO_AVATAR();

	WORD	DeleteAva(char * master,char *name, WORD  & _wGuild);	//	int idx
	BOOL	MakeDBIndex();
	void	GetDBIndex(char * master ,_DBINDEXTABLE & table );
	//	전체 데이타를 읽어와야 하니까.. 두번 부를 필요는 없도록 해라.
	void	GetAvaSInfo(char * master , DLMSG_GETAVATARS & avas);				//	선택화면에 들어갈 정보들
	int		LoadBankInfo(char * master, CBankInfo * bank);
	int		SaveBank(CBankInfo * bank);
	int		SaveAva(cPLAYER_DATA * pData,int playtime,BOOL leveldown = FALSE);	//	캐릭터 데이타를 저장한다. 

	int		GetProvisionRank(DGMSG_PROVISION_RANK_RET * _rank);

	//	FOR Guild
	WORD	CreateGuild(WORD _wGuildindex,char * _strGuildName,char * _strMasterId,char * _strMasterName,WORD _wJob,WORD _wLevel);
	WORD	DeleteGuild(WORD _wGuildindex,char * _strGuildName);
	WORD	JoinGuild(WORD _wGuildindex,char * _strGuildName, char * _strId, char * _strName,WORD wRank,WORD wJob,WORD wLevel);
	WORD	ExitGuild(WORD _wGuildindex,char * _strGuildName, char * _strName);
	WORD	AppointMaster(WORD _wGuildindex,char * _strGuildName, char * _strOldMaster, char * _strNewMaster,WORD _wRank);
	WORD	ChangeRank(WORD _wGuildindex,char * _strGuildName, char * _strMember, WORD _wRank);

	void	queryGetGuild_List(GuildListDB * List, int & _iListCount);
	void	queryGetGuild_Data(GuildListDB * List);
	void	queryGetGuild_Member(WORD _wGuildIndex, DWMSG_GUILD_MEMBER * member);

	WORD	SaveGuild_Info(WDMSG_SAVEGUILDBASE * baseinfo);
	WORD	SaveGuild_Mark(WORD _wGuildindex,char * _strGuildName, void * ,DWORD	_dwSize);
	WORD	SaveGuild_Skill(WORD _wGuildindex,char * _strGuildName,WORD _wGuildPoint, void * ,DWORD	_dwSize);
	WORD	SaveGuild_Pet(WORD _wGuildindex,char * _strGuildName, void * ,DWORD	_dwSize);
	WORD	SaveGuild_Guardian(WORD _wGuildindex,char * _strGuildName, void * ,DWORD	_dwSize);
	WORD	SaveGuild_Inventory(WORD _wGuildindex,char * _strGuildName, void * ,DWORD	_dwSize);
	WORD	SaveGuild_Structure(WORD _wGuildindex,char * _strGuildName, void * ,DWORD	_dwSize);

	//	Guild Battle
	int		spGuildBattle_Get(UTime _stTime, CGuildBattleSchedule * _cGBSchedule);
	int		spGuildBattle_Set(UTime _stTime, int _iIdx, CGuildBattleInField * _cGBField);

	//	Stored Procedure 를 사용한 메소드 
	int		spCreateAva(DWORD & _dwIndex,cPLAYER_DATA * pData,int blankPos);
	WORD	spDeleteAva(char * master,char * name,WORD & _wGuild);	//	int idx
	WORD	spSaveAva(cPLAYER_DATA * pData,BOOL leveldown,int playtime=0);
	void	spGetAvaInfo(char * _strName, char * _strId, cPLAYER_DATA * pData,WORD & oper);

	//	personal data 
	int		spPD_GetData(int _iIdx,char * _strId, char * _strName,void * _binData);
	int		spPD_SetData(int _iIdx,char * _strId, char * _strName, void * _binData);

	//	Event
	int		spEvent_GoldRush_Get_First(cGoldRushDefine * _cGoldRush);
	int		spEvent_ItemPack_Get_First(int _iSerial, cItemPackInfo * _cIPInfo);
	int		spEvent_GoldRush_Get(cGoldRushDefine * _cGoldRush);
	int		spEvent_ItemPack_Get(cItemPackInfo * _cIPInfo);

	//	login

	int		spLogin_Reset();
	int		spLogin_In(char* _strId, char* _strName, const int& _externalType);
	int		spLogin_Out(char* _strId);

private:
	int		queryBlankAvaPos(char * master);
};

#endif // !defined(AFX_CADO_AVATAR_H__AD1D9ECD_B80F_427C_A43A_3B36C4087638__INCLUDED_)
