#pragma	once

#include <icrsint.h> 
#include "definePacketData.h"
#include "cADO_BASE.h"
#include "cPLAYER_DATA.h"


typedef struct {
	DWORD						u_serial;
	DWORD						worldSerial;			//	world 서버의 유저 시리얼
	char						strID[dID_LENGTH];
	cAVATAR_INFO				aCharacter[4];
}DLMSG_GETAVATARS;


class cParanInfo
{
public:
	char strParanId[32];
	char strNewId[32];
	BOOL isLastData;
	BOOL isOverwrite;
	int	 iServerIdx;
	int	 iIdx[4];			//	살아남을 캐릭터들..

	cParanInfo()
	{
		memset(strParanId,0,32);
		memset(strNewId,0,32);
		isOverwrite = FALSE;
		iServerIdx = -1;
		isLastData = FALSE;
		memset(iIdx,0xff,sizeof(int)*4);
	}

	void operator=(const cParanInfo * _info)
	{
		strcpy(strParanId, _info->strParanId);
		strcpy(strNewId, _info->strNewId);
		isOverwrite = _info->isOverwrite;
		iServerIdx = _info->iServerIdx;
		isLastData = _info->isLastData;
		memcpy(iIdx,_info->iIdx,sizeof(int)*4);
	}
};

class cADO_PARAN	:	public cADO_BASE
{
public:
	cADO_PARAN();
	~cADO_PARAN();

//	Account에 적용할 것
	cParanInfo	getNewConvertingData();		//	변경할 아이디가 있는지 검사해서 정보를 가져온다.
	int		unsetBlock(char * _strId);		//	변경이 완료된 아이디의 블럭정보를 해제한다.
	int		finishConvertingWork(char * _strId, int _srvIdx);
	int		convertPvPInfo(char * _strYahooId,char * _strNewId);

//	Avatar 쪽에만 사용할 것
//	int		converting(cParanInfo * _info, BOOL _isParan = FALSE);//	지정된 서버에 주어진 정보를 수정한다. 이 안에 개인 데이터와 길드등의 정보를 같이 수정한다.

	//	실질적인 AVATAR 작업들
//	int		GetAvaSInfo(char * master , DLMSG_GETAVATARS & avas);	//	선택화면에 들어갈 정보들
	int		spGetAvaInfo(char * _strName, char * _strId, cPLAYER_DATA * pData,WORD & oper);
	int		LoadBankInfo(char * master, CBankInfo * bank);
	int		spPD_GetData(int _iIdx,char * _strId, char * _strName,void * _binData);

	int		SaveBank(CBankInfo * bank);
	WORD	spSaveAva(char * _strOldId, cPLAYER_DATA * pData,BOOL leveldown,int playtime=0);
	DWORD	spCreateAva(cPLAYER_DATA * pData);
	int		spPD_SetData(int _iIdx,char * _strId, char * _strName, void * _binData);
	
	int		delBankId(char * _strOldId);
	int		delCurtomDataId(char * _strOldId);
	int		chgBankId(char * _strOldId, char * _strNewId);
	int		chgAvaId(char * _strOldId, char * _strNewId);
	int		chgCurtomDataId(char * _strOldId, char * _strNewId,DWORD idx);

	int		deleteOldAvatar(char * _strId, char * _strName, BOOL _isUsingSP = TRUE);	//	해당 서버의 예전 아이디에 있던 캐릭터를 지움. 주민번호 중복시 처리할 내용
	int		updateAvatarInfo(char * _strOldId, char * _strNewId);	//	
	int		changeGuildInfo(char * _strOldId, char * _strNewId);//이걸 적용하면 다음 정기점검 이후에 정상적으로 나타남.
};