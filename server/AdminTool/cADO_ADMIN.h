#ifndef __C_ADO_ADMIN_H
#define __C_ADO_ADMIN_H

#pragma warning( push )
#pragma warning( disable : 4786 )

#include "cADO_BASE.h"
#include "cPLAYER_DATA.h"
#include "adCommandDefine.h"
#include "eventDefine.h"
#include "carrotshopDefine.h"
#include "cGuildDefine.h"
#include <vector>
#include <map>

using namespace std;

typedef struct ava4
{
	cPLAYER_DATA		ava[4];
}_ava4;

typedef struct _stIDInfo
{
	char		strConnectedServerName[dNAME_LENGTH];		//	현재 접속중인 서버 이름
	DWORD		dwStartConnect;								//	시작한 날
	DWORD		dwLastConnect;								//	마지막으로 접속한 날짜
	DWORD		dwBlock;									//	언제까지 블럭인가?
	char		strWhyBlock[128];							//	블럭된 이유
}stIDInfo;

typedef struct 
{
	struct stMark
	{
		int		iIndex;
		char	strName[64];
		CGuildMarkInfo cMark;
	};

	stMark *	m_pstMark;
	int			m_iCount;
}stGuildMarkCheck;

class cWCGInfo
{
public:
	enum
	{
		eMaxCountOfRanking	= 100,
		eMaxCountOfTeam		= 1024,
	};

	int		m_iTeamCount;	//WCG에 등록한 팀의 수 - c_iDuelTeamMaxCount 이하여야 되니 주의!!
	int		m_iRankingCount;
	cDuelTeamInfo	* m_pcInfo;	//팀 리스트 담기
	cDuelRankingInfo * m_pcRank;//MAX는 eMaxCountOfRanking이다.

	cWCGInfo()
	{
		m_iTeamCount = 0;
		m_iRankingCount = 0;
		m_pcInfo = NULL;
		m_pcRank = new cDuelRankingInfo[eMaxCountOfTeam];
	}

	~cWCGInfo()
	{
		release();
	}

	void init(int _teamCount)
	{
		m_iTeamCount = (_teamCount>eMaxCountOfTeam)?(eMaxCountOfTeam):(_teamCount);

		if	(m_pcInfo)
		{
			delete m_pcInfo;
			m_pcInfo = NULL;
		}
		m_pcInfo = new cDuelTeamInfo[m_iTeamCount];
	}

	void release()
	{
		if(m_pcInfo)
		{
			delete m_pcInfo;
			m_pcInfo = NULL;
		}
		if(m_pcRank)
		{
			delete m_pcRank;
			m_pcRank = NULL;
		}
	}
};

class	cADO_ADMIN	:	public cADO_BASE
{
private:
	std::map<int,int>	m_mapWCGTeamIndex;

public:
	int		iMaxCount;
	char	pro[1024];

	BOOL				m_bIsNormalDB;
	cWCGInfo			m_cWCGInfo;

public:
	cADO_ADMIN();
	virtual ~cADO_ADMIN();

	void	reConnect(int num);

///////////////////////////////////////////////////////
//	로그인 
	int		spAdminLogin(char *name,char *pw,int ver);
//	로그인 
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
//	for binary data 
	void	BlobToVariant(VARIANT &varChunk,void *pData, UINT nBytes);	//	data -> blob
	int		VariantToBlob(VARIANT &varChunk,void *pData);	//	blob -> data
//	for binary data 
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
//	Operating Account
	WORD	spSetBlockByRecover(char *id,char *why);
	WORD	spSetBadUser(char *name,int dd,char *why);
	void	spGetBadInfo(char *id,char *why);
	int		spRecoverID(char *id,char *newid);
	int		spDeleteID(char *name);
	WORD	spGetIDInfo(char * id, stIDInfo	* _info);
	DWORD	spGetLastDate(char * id);

	//	운영자 관리하기
	WORD	spGetOperatorCount();
	WORD	spResetOperator();
	WORD	spSetOperatorInAccount(char * _strId,int _isOper);
//	Operating Account
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
//	About Avatar DB 
	WORD	spGetAvatarIndexOnly(char *_strId, char *_strName);
	void	spSaveAva(cPLAYER_DATA * pData,BOOL leveldown = FALSE,BOOL isNew = FALSE);
	int		spSaveBank(CBankInfo * pBank);
	BOOL	spChangeIndex(char *id,char *name,int index);
	BOOL	spReadAva(BOOL isTestServer,char *ID,_ava4 & p);
	BOOL	spReadAva(char * strName,cPLAYER_DATA * _data);
	int		LoadBankInfo(char * master, CBankInfo * bank);
	int		spDeleteAvatar(char *id,char *name);
//	int		spAvaRecover(char *id,char *newid,int idx,char * resultName);	//	resultname은 새로운 이름, 결과 이름 이중으로 사용해라.
//	int 	spAvaRecoverDel(char *id,int idx,char * name);
	int		spAvaRename(char *id,char * oldname,char * newname);
	int		spAvaRenameSyc(char *id,char * newname, char * oldName = NULL);
	int		spAvaRecover(char *id,char * oldname,char * newname);
	int		spAvaRecoverOld(char *id,char * oldname,char * newname);
	void	spDeleteLoginInfo(char * _worldName);		//	해당 월드의 접속 정보를 모두 삭제한다.

	//	운영자 관리하기
	int		spGetOperLevel(char * _name);
	WORD	spIsOper(char *name);
	void	spSetOper(char *name,int level);
	int		spGetOperCount();
//	About Avatar DB 
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
//	명령 예약하기
	int		spCTGetList();
	int		spCTReserve(cAdCommandData * _data);
	int		spCTModify(cAdCommandData * _data);
	int		spCTCancel(int _idx);
//	명령 예약하기
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
//	통계
	struct stUserStatsPack
	{
		int iAllUserCount;
		int iReserve1UserCount;//yahoo
		int iReserve2UserCount;//Anycall
		int iReserve3UserCount;//
		int iReserve4UserCount;//
		int iReserve5UserCount;//
	};

	int		spGetTotalUserCountByDay(int day,int & y_result,int & p_result,int &week, int &week_y,int &week_p,int &month,int &month_y,int &month_p);
	int		spGetDetailUserCountStat(int day,int srvIdx, vector<stUserStatsPack> * _pVector);
	void	spGetAgeConnectCount(char * tablename, int today,int * ttt);
	void	spGetAgePlayTimeCount(char * tablename, int today,int * ttt);
	void	spGetDistriLevel(int today, char * data);
	//	임시 통계
	void	spSUM_ForLevel();	//	전체, 직업별 레벨 분포표
//	통계
///////////////////////////////////////////////////////


///////////////////////////////////////////////////////
//	길드 마스터를 바꾸기 위한..
	void	spGetGuildBasicData(GUILDINFO_FORADMIN *_guilddata);
	int		spGetGuildMarkCheck(stGuildMarkCheck * _stMark);
	int		spGetGuildInfo(char * gname, char * gmname);
	int		spGetGuildData(char *_gname , BOOL _isOld = FALSE);
	int		spGetGuildSkillData(char *_gname, CGuildSaveInfoSkill * _skillData);

	void	spGetAllGuildInfo(int _idx, CGuildAdvanceInfo * _pinfo){}
	void	spGetAllGuildInfo(char * _gname, CGuildAdvanceInfo * _pinfo){}

	int		spGetAllOldGuildInfo(GuildListDBOld * _list);

	int		spSaveGuildInfo(CGuildSaveInfoForColumn * _lpColumnInfo, CGuildSaveInfoForBinary * _lpBinaryInfo, CGuildSaveInfoMark * _lpMarkInfo, CGuildSaveInfoBank * _lpBankInfo, CGuildSaveInfoRelShip * _lpRelShipInfo);
	int		spSaveGuildSkill(int _iIndex, char * _strName, WORD _wGuildPoint, CGuildSaveInfoSkill * _lpInfo);
	int		spSaveGuildPet(int _iIndex, char * _strName, CGuildSaveInfoPet * _lpInfo);
	int		spSaveGuildGuardian(int _iIndex, char * _strName, CGuildSaveInfoGuardian * _lpInfo);
	int		spSaveGuildInventory(int _iIndex, char * _strName, CGuildSaveInfoInventory * _lpInfo);
	int		spSaveGuildstructure(int _iIndex, char * _strName, CGuildSaveInfoStructure * _lpInfo);

	/////////////////////////////////////////////////////////
	//	주의할 것.. 길드정보 변경만을 위한 인터페이스..
	int		spInsertGuildInfo(CGuildSaveInfoForColumn * _lpColumnInfo, CGuildSaveInfoForBinary * _lpBinaryInfo, CGuildSaveInfoMark * _lpMarkInfo, CGuildSaveInfoBank * _lpBankInfo, CGuildSaveInfoRelShip * _lpRelShipInfo, CGuildSaveInfoSkill * _lpInfo);
	int		spInsertGuildData(int _iIndex, char * _strName, CGuildSaveInfoPet * _lpInfoPet, CGuildSaveInfoGuardian * _lpInfoGuardian, CGuildSaveInfoInventory * _lpInfoInventory, CGuildSaveInfoStructure * _lpInfoStructure);
	//	주의할 것.. 길드정보 변경만을 위한 인터페이스..
	/////////////////////////////////////////////////////////


	int		SaveAllGuildInfo(CGuildAdvanceInfo * _lpInfo);
//	길드 마스터를 바꾸기 위한..
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
//	For Event
	int		spEvent_GoldRush_Create(cGoldRushDefine * _cGoldRush,char * _strWho);
	int		spEvent_GoldRush_Get(cGoldRushDefine * _cGoldRush);

	int		spEvent_ItemPack_Create(int _iSerial,cItemPackInfo * _cIPInfo);
	int		spEvent_ItemPack_Get(int _iSerial,cItemPackInfo * _cIPInfo);

	int		GetCarrotShopInfo(cCarrotShopDefine * _cpCSInfo,int _isFirst = 1);
	int		SetCarrotShopInfo(cCarrotShopDefine * _cpCSInfo);
//	For Event
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
//	For PvP or WCG
private:
	int		spGetWCGTeamList();//WCG 팀 리스트를 모두 가져온다.
	int		spGetWCGTeamUserList();//(WCG팀 리스트를 가져온 후) 각 팀에 소속된 인원들의 리스트를 가져온다.

public:
	void	spClearWCGTeamList();//	WCG 팀 리스트를 모두 삭제한다. - 팀원들의 정보도 삭제된다.
	int		spInsertWCGTeamList(char * _strTeamName);//	새로운 WCG팀을 등록한다.
	int		spInsertWCGTeamUserList(int _idx, char * _id, char * _name, int _job);//_idx WCG팀의 팀원을 삽입한다.
	void	spGetWCGTeamInfo()	//	돌려진 값은 m_cWCGInfo를 참고하세요.
	{
		spGetWCGTeamList();
		spGetWCGTeamUserList();
	}
	
	void	spClearWCGRank();//WCG 랭킹을 삭제한다.
	int		spGetWCGRank();	//WCG 랭킹 가져오기. 100위 까지만...
	int		spModifyPvPTeamScore(char * _strName, cDuelTeamRecord * _record);//_strName 팀의 WCG 성적을 수정한다.
	int		spModifyPvPScore(cDuelRecord * _record);//_strName 캐릭터의 WCG 성적을 수정한다.
	int		spGetPvPScore(char * _strId, cDuelRecord * _record);

//	For PvP or WCG
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
//	Temporary functions.  Don't use those.
	int		CheckIDs();
	int		spCreateAva(cPLAYER_DATA * pData);
	void	sp20030509ReSet();
	void	spGetBankConvert20031203();
	void	spBankConvert20031203();
	void	spBankConvert20040108();
//	Temporary Function
///////////////////////////////////////////////////////


//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃영찬이가 추가한 코드
public:
	BOOL	loadData(char *_lpstrId,char *_lpstrName,void *_lpData,int _iWantCount=0,int _iJob=0,int _iLevel=0);
	BOOL	checkRich(char *_lpstrFileName,DWORD _dwGold);
	BOOL	saveData(cP_PLAYER_SAVE_DATA *_lpData,BOOL isNew = FALSE,BOOL _bIsNotCheckLevelDown=FALSE);
	BOOL	resetAllPlayerData(BOOL isTestServer,BOOL _bResetStatePoint,BOOL _bResetSkillPoint,int *_lpiJobList);
	BOOL	findItem(cITEM *_lpItems,int _iItemCount,char *_lpstrFileName);
//┃영찬이가 추가한 코드
//┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	
	
//	for Debug 
	int		GlastLine;
};


extern	cADO_ADMIN	g_db;

#pragma warning( pop )

#endif //	__C_ADO_BASE_H