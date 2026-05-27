#include "StdAfx.h"
#include "cPLAYER_DATA.h"
#include "cADO_ADMIN.h"
#include <stdio.h>
#include <process.h>
#include "cSRVUTIL.h"
#include "RSComm.h"
#include "cDECODER.h"
#include "cITEM.h"
#include "CGuildDefine.h"

int		cADO_ADMIN::spGetGuildSkillData(char *_gname, CGuildSaveInfoSkill * _skillData)
{
	_RecordsetPtr  pRs = NULL; 	
	_variant_t     v_bufferdata;
	char	       str_Query[1024];
	int i_RecCount = 0;
	char *pBuf = NULL;
	HRESULT hr = CoInitialize(0);

	sprintf(str_Query,"SELECT binSkill FROM  GUILD_INFO WHERE strName=\'%s\' ", _gname);
	pRs.CreateInstance(__uuidof(Recordset)); 
	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);

	if(!FAILED(hr))
	{
		v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
		char * uncompressdata = new char[sizeof(CGuildSaveInfoSkill)];
		int unsize = VariantToBlob(v_bufferdata,uncompressdata);
		if(unsize == sizeof(CGuildMarkInfo))
			memcpy(_skillData,uncompressdata,unsize);
		else
			WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)_skillData,unsize,sizeof(CGuildSaveInfoSkill));
		delete [] uncompressdata;
	}
	pRs->Close();

	return 1;
}

int		cADO_ADMIN::spGetGuildData(char * _gname , BOOL _isOld)
{
	_RecordsetPtr  pRs = NULL; 	
	_variant_t     v_bufferdata;
	char	       str_Query[1024];
	int i_RecCount = 0;
	char *pBuf = NULL;
	HRESULT hr = CoInitialize(0);

	if(_isOld)
		sprintf(str_Query,"SELECT gIdx, binPet, binGuardian, binInventory, binStructure FROM GUILD_DATA_OLD WHERE strName=\"%s\" ", _gname);
	else
		sprintf(str_Query,"SELECT gIdx, binPet, binGuardian, binInventory, binStructure FROM GUILD_DATA WHERE strName=\"%s\" ", _gname);
/*
	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	_stMark->m_iCount = i_RecCount = pRs->RecordCount;

	_stMark->m_pstMark = new stGuildMarkCheck::stMark[_stMark->m_iCount];

	for(i = 0;i<_stMark->m_iCount;i++)
	{
		_stMark->m_pstMark[i].cMark.reset();
		_stMark->m_pstMark[i].iIndex = -1;
		_stMark->m_pstMark[i].strName[0] = NULL;
	}

	if(!FAILED(hr))
	{
		for(i = 0 ;i < i_RecCount ;i++)
		{
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			_stMark->m_pstMark[i].iIndex = (int)v_bufferdata.intVal;

			v_bufferdata = pRs->Fields->GetItem((LONG)1)->Value;
			strcpy(_stMark->m_pstMark[i].strName,(_bstr_t)v_bufferdata);

			v_bufferdata = pRs->Fields->GetItem((LONG)2)->Value;
			char * uncompressdata = new char[sizeof(CGuildMarkInfo)];
			int unsize = VariantToBlob(v_bufferdata,uncompressdata);
			if(unsize == sizeof(CGuildMarkInfo))
				memcpy(&_stMark->m_pstMark[i].cMark,uncompressdata,unsize);
			else
			{
				WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)&_stMark->m_pstMark[i].cMark,unsize,sizeof(CGuildMarkInfo));
//				if(ww == 0)	
//					MessageBox(NULL,"FAIL UNCOMPRESS!!","ERROR",MB_OK);
			}
			delete [] uncompressdata;

			pRs->MoveNext();
		}
	}
	pRs->Close();

	_stMark->m_iCount = i_RecCount;
*/
	return i_RecCount;
}

int		cADO_ADMIN::spGetGuildMarkCheck(stGuildMarkCheck * _stMark)
{
	_RecordsetPtr  pRs = NULL; 	
	_variant_t     v_bufferdata;
	char	       str_Query[1024];
	int i_RecCount;
	int i;
	char *pBuf = NULL;
	HRESULT hr = CoInitialize(0);

	sprintf(str_Query,"SELECT iIndex,strName,binMark FROM  GUILD_INFO");
	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	_stMark->m_iCount = i_RecCount = pRs->RecordCount;

	_stMark->m_pstMark = new stGuildMarkCheck::stMark[_stMark->m_iCount];

	for(i = 0;i<_stMark->m_iCount;i++)
	{
		_stMark->m_pstMark[i].cMark.reset();
		_stMark->m_pstMark[i].iIndex = -1;
		_stMark->m_pstMark[i].strName[0] = NULL;
	}

	if(!FAILED(hr))
	{
		for(i = 0 ;i < i_RecCount ;i++)
		{
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			_stMark->m_pstMark[i].iIndex = (int)v_bufferdata.intVal;

			v_bufferdata = pRs->Fields->GetItem((LONG)1)->Value;
			strcpy(_stMark->m_pstMark[i].strName,(_bstr_t)v_bufferdata);

			v_bufferdata = pRs->Fields->GetItem((LONG)2)->Value;
			char * uncompressdata = new char[sizeof(CGuildMarkInfo)];
			int unsize = VariantToBlob(v_bufferdata,uncompressdata);
			if(unsize == sizeof(CGuildMarkInfo))
				memcpy(&_stMark->m_pstMark[i].cMark,uncompressdata,unsize);
			else
			{
				WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)&_stMark->m_pstMark[i].cMark,unsize,sizeof(CGuildMarkInfo));
//				if(ww == 0)	
//					MessageBox(NULL,"FAIL UNCOMPRESS!!","ERROR",MB_OK);
			}
			delete [] uncompressdata;

			pRs->MoveNext();
		}
	}
	pRs->Close();

	_stMark->m_iCount = i_RecCount;
	return i_RecCount;
}

void	cADO_ADMIN::spGetGuildBasicData(GUILDINFO_FORADMIN * _guilddata)
{
	_RecordsetPtr  pRs = NULL; 	
	_variant_t     v_bufferdata;
	char	       str_Query[1024];
	int i_RecCount;
	int i;
	char *pBuf = NULL;
	HRESULT hr = CoInitialize(0);

	for(i = 0;i<dGUILD_MAX_COUNT;i++)
	{
		memset(&_guilddata[i],0xff,sizeof(GUILDINFO_FORADMIN));
		strcpy(_guilddata[i].strName,"EMPTY");
	}
	sprintf(str_Query,"SELECT iIndex,strName,iLevel,iHallLevel,strMasterName,iTargetGuild,iInvadeGuild,iGuildPoint,iVicPoint FROM  GUILD_INFO");//,iVicContinue,iWin,iLost,iDraw,iCancel
	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount;

	if(!FAILED(hr))
	{
		for(i = 0 ;i < i_RecCount ;i++)
		{
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			int	iIdx = v_bufferdata.intVal;
			_guilddata[iIdx].wIndex = iIdx;

			v_bufferdata = pRs->Fields->GetItem((LONG)1)->Value;
			strcpy(_guilddata[iIdx].strName,(_bstr_t)v_bufferdata);
			v_bufferdata = pRs->Fields->GetItem((LONG)2)->Value;
			_guilddata[iIdx].iLevel = v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)3)->Value;
			_guilddata[iIdx].iHallLevel = v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)4)->Value;
			strcpy(_guilddata[iIdx].strMasterName,(_bstr_t)v_bufferdata);
			v_bufferdata = pRs->Fields->GetItem((LONG)5)->Value;
			_guilddata[iIdx].iTargetGuild = v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)6)->Value;
			_guilddata[iIdx].iInvadeGuild = v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)7)->Value;
			_guilddata[iIdx].iGuildPoint = v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)8)->Value;
			_guilddata[iIdx].iVicPoint = v_bufferdata.intVal;
/*			v_bufferdata = pRs->Fields->GetItem((LONG)9)->Value;
			_guilddata[iIdx].iVicContinue = v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)10)->Value;
			_guilddata[iIdx].iWin = v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)11)->Value;
			_guilddata[iIdx].iLost = v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)12)->Value;
			_guilddata[iIdx].iDraw = v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)13)->Value;
			_guilddata[iIdx].iCancel = v_bufferdata.intVal;
*/			pRs->MoveNext();
		}
	}

	pRs->Close();
}

//	길드 마스터를 바꾸기 위한..
int	cADO_ADMIN::spGetGuildInfo(char * gname, char * gmname)
{
	try{
		_CommandPtr		pCmd = NULL;
		VARIANT bin;
		HRESULT			hr = CoInitialize(0);
		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGUILD_AD_GMCINFO";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@GNAME") = _variant_t((char *)gname);
		DEF_PARAM(pCmd,"@OLDGMNAME") = _variant_t((char *)gmname);
		hr = pCmd->Execute(NULL,NULL,NULL);
		VariantClear(&bin);
		if(FAILED(hr))	return 0;

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.lVal = pCmd->GetParameters()->GetItem("@RET")->Value;
		int iResult = (DWORD)regIdx.lVal;
		VariantClear(&regIdx);

		if(iResult >=0)
		{
			VariantInit(&regIdx);
			regIdx = pCmd->GetParameters()->GetItem("@GMNAME")->Value;
			strcpy(gmname,(_bstr_t)regIdx);
		}
		VariantClear(&regIdx);



		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
		
		return iResult;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
	return 1;
}

int		cADO_ADMIN::spGetAllOldGuildInfo(GuildListDBOld * _list)
{
	char	       str_Query[1024];
	int i_RecCount;
	int i;
	HRESULT hr = CoInitialize(0);
	char	strDate[64];
	CCritical		cs(&m_csAC);
	try
	{
		_RecordsetPtr  pRs = NULL; 	
		sprintf(str_Query , "SELECT * FROM BaseGuildInfo");
		pRs.CreateInstance(__uuidof(Recordset)); 

		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
		i_RecCount = pRs->RecordCount;

		char	uncompressdata[3000];
		int		iComressSize = 0, iTmpCount = 0;
		if(!FAILED(hr))
		{ 
			for(i = 0 ;i < i_RecCount ;i++) 		
			{
				_variant_t     v_bufferdata;

				v_bufferdata = pRs->Fields->GetItem((LONG)0)->GetValue();
				_list[i].wIndex = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)1)->GetValue();
				strcpy(_list[i].strGuildName,(_bstr_t)v_bufferdata);
				v_bufferdata = pRs->Fields->GetItem((LONG)2)->GetValue();
				_list[i].wLevel = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)3)->GetValue();
				_list[i].wHallLevel = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)4)->GetValue();
				_list[i].dwExp = (DWORD)v_bufferdata.intVal;
//				v_bufferdata = pRs->Fields->GetItem((LONG)5)->GetValue();
//				_list[i].dwGold = (DWORD)v_bufferdata.intVal;
				_list[i].dwGold = 0;
				v_bufferdata = pRs->Fields->GetItem((LONG)6)->GetValue();
				strcpy(_list[i].strNotice,(_bstr_t)v_bufferdata);
				v_bufferdata = pRs->Fields->GetItem((LONG)7)->GetValue();
				_list[i].wNoticeType = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)8)->GetValue();
				strcpy(_list[i].strMaster,(_bstr_t)v_bufferdata);
//				v_bufferdata = pRs->Fields->GetItem((LONG)9)->GetValue();
//				_list[i].wElderCount = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)10)->GetValue();
				_list[i].wMasterLevel = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)11)->GetValue();
				strcpy(strDate,(_bstr_t)v_bufferdata);
				getTimeToUTime(strDate,&_list[i].dwStartMasterDate);
				v_bufferdata = pRs->Fields->GetItem((LONG)12)->GetValue();
				_list[i].wIsGetExp = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)13)->GetValue();
				_list[i].wRateGetExp = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)14)->GetValue();
				_list[i].wDateChangeExp = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)15)->GetValue();
				_list[i].wMaxMember = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)16)->GetValue();
				_list[i].wMaxSubMaster = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)17)->GetValue();
				_list[i].wMaxCongress = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)18)->GetValue();
				_list[i].wVillage[0] = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)19)->GetValue();
				_list[i].wVillage[1] = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)20)->GetValue();
				_list[i].wVillage[2] = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)21)->GetValue();
				_list[i].wVillage[3] = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)22)->GetValue();
				_list[i].wVillage[4] = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)23)->GetValue();
				_list[i].wVillage[5] = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)24)->GetValue();
				_list[i].wVillage[6] = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)25)->GetValue();
				memcpy(&_list[i].dwLastConnect,&v_bufferdata.intVal,sizeof(DWORD));
				v_bufferdata = pRs->Fields->GetItem((LONG)26)->GetValue();
				_list[i].wBestowItemLevel = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)27)->GetValue();
				_list[i].wSellingAgentItemLevel = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)28)->GetValue();
				_list[i].wTargetGuild = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)29)->GetValue();
				_list[i].wInvadeGuild = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)30)->GetValue();
				strcpy(strDate,(_bstr_t)v_bufferdata);
				getTimeToUTime(strDate,&_list[i].dwLastConnectTime);
				v_bufferdata = pRs->Fields->GetItem((LONG)31)->GetValue();
				_list[i].GuildPoint = (WORD)v_bufferdata.intVal;
	
				v_bufferdata = pRs->Fields->GetItem((LONG)32)->Value;
				memset(&_list[i].m_guildMarkInfo,0,sizeof(CGuildMarkInfo));
				_list[i].m_guildMarkInfo.reset();
				if(v_bufferdata.vt != VT_NULL)
				{
					memset(uncompressdata,0,3000);
					iComressSize = VariantToBlob(v_bufferdata,uncompressdata);
					if(iComressSize == sizeof(_list[i].m_guildMarkInfo))
						memcpy(&_list[i].m_guildMarkInfo,uncompressdata,iComressSize);
					else
					{
						WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)&_list[i].m_guildMarkInfo,iComressSize,sizeof(CGuildMarkInfo));
						if(ww == 0)	
							_log(" uncompress error 4-1");
					}
				}else{
					_log("m_guildMarkInfo - Init GUILD DATA : GUILD [%d]",_list[i].wIndex);
				}
				
				v_bufferdata = pRs->Fields->GetItem((LONG)33)->Value;
				memset(_list[i].m_aGuildSkill,0,sizeof(CGuildSkillInfo)*dMAX_GUILD_SKILL_COUNT);
				if(v_bufferdata.vt != VT_NULL)
				{
					memset(uncompressdata,0,3000);
					iComressSize = VariantToBlob(v_bufferdata,uncompressdata);
					if(iComressSize == sizeof(_list[i].m_aGuildSkill))
						memcpy(_list[i].m_aGuildSkill,uncompressdata,iComressSize);
					else
					{
						WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)_list[i].m_aGuildSkill,iComressSize,sizeof(CGuildSkillInfo)*dMAX_GUILD_SKILL_COUNT);
						if(ww == 0)	_log(" uncompress error 5");
					}
				}else{
					_log("m_aGuildSkill - Init GUILD DATA : GUILD [%d]",_list[i].wIndex);
				}
				v_bufferdata = pRs->Fields->GetItem((LONG)34)->Value;
				_list[i].dwVicPoint = (DWORD)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)35)->Value;
				_list[i].wVicContinue = (WORD)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)36)->Value;
				_list[i].wFightAtWeek = (WORD)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)37)->Value;
				_list[i].wFight[0] = (WORD)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)38)->Value;
				_list[i].wFight[1] = (WORD)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)39)->Value;
				_list[i].wFight[2] = (WORD)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)40)->Value;
				_list[i].wFight[3] = (WORD)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)41)->Value;
				_list[i].dwWin = (DWORD)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)42)->Value;
				_list[i].dwLose = (DWORD)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)43)->Value;
				_list[i].dwDraw = (DWORD)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)44)->Value;
				_list[i].dwCancel = (DWORD)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)45)->Value;
				strcpy(strDate,(_bstr_t)v_bufferdata);
				getTimeToUTime(strDate,&_list[i].dwLastConnectedMasterTime);

				VariantClear(&v_bufferdata);
				pRs->MoveNext();
			}
		}
		pRs->Close();	pRs.Release();	pRs = NULL;
	}catch(_com_error &e)
	{
		DMPcomERR(e)
		return -1;
	}

	return i_RecCount;
}

int
cADO_ADMIN::spSaveGuildInfo(CGuildSaveInfoForColumn * _lpColumnInfo, CGuildSaveInfoForBinary * _lpBinaryInfo, CGuildSaveInfoMark * _lpMarkInfo, CGuildSaveInfoBank * _lpBankInfo, CGuildSaveInfoRelShip * _lpRelShipInfo)
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGUILD_SAVE";
		pCmd->CommandType  = adCmdStoredProc;

		_log(" spSaveGuildInfo Line %d",__LINE__);
		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)_lpColumnInfo->m_wSerial);
		_log(" spSaveGuildInfo Line %d",__LINE__);
		DEF_PARAM(pCmd,"@LEVEL") = _variant_t((short)_lpColumnInfo->m_wLevel);
		_log(" spSaveGuildInfo Line %d",__LINE__);
		DEF_PARAM(pCmd,"@HALLLEVEL") = _variant_t((short)_lpColumnInfo->m_wHallLevel);
		_log(" spSaveGuildInfo Line %d",__LINE__);
		DEF_PARAM(pCmd,"@VICPOINT") = _variant_t((long)_lpColumnInfo->m_iVictoryPoint);
		_log(" spSaveGuildInfo Line %d",__LINE__);
		DEF_PARAM(pCmd,"@HONORPOINT") = _variant_t((long)_lpColumnInfo->m_iHonorPoint);
		_log(" spSaveGuildInfo Line %d",__LINE__);
		DEF_PARAM(pCmd,"@GUILDPOINT") = _variant_t((long)_lpColumnInfo->m_wGuildPoint);
		_log(" spSaveGuildInfo Line %d",__LINE__);
		DEF_PARAM(pCmd,"@HALLSHAPE") = _variant_t((short)_lpColumnInfo->m_wGuildHallShape);
		_log(" spSaveGuildInfo Line %d",__LINE__);
		DEF_PARAM(pCmd,"@GOLD") = _variant_t((DOUBLE)_lpColumnInfo->m_llGuildGold);
		_log(" spSaveGuildInfo Line %d",__LINE__);
		DEF_PARAM(pCmd,"@NTYPE") = _variant_t((short)_lpColumnInfo->m_wNoticeType);
		_log(" spSaveGuildInfo Line %d",__LINE__);
		DEF_PARAM(pCmd,"@NOTICE") = _variant_t((char *)_lpColumnInfo->m_strNotice);
		_log(" spSaveGuildInfo Line %d",__LINE__);
		char	datetime[20];
		UTimeToDatetime(&_lpColumnInfo->m_timeLastConnectTime,datetime);
		DEF_PARAM(pCmd,"@LASTCONNECT") = _variant_t((char *)datetime);
		_log(" spSaveGuildInfo Line %d",__LINE__);
		char	* compressData = new char[100];	//	@BINDATA 가 96으로 최고.. 아래 4개중..
		VARIANT bin;WORD sz;
		VariantInit(&bin);
		memset(compressData,0,100);
		_log(" spSaveGuildInfo Line %d",__LINE__);
		sz = cSRVUTIL::Compress((unsigned char *)_lpBinaryInfo,(unsigned char *)compressData,sizeof(CGuildSaveInfoForBinary));
		_log(" spSaveGuildInfo Line %d",__LINE__);
		BlobToVariant(bin,compressData,sz);
		_log(" spSaveGuildInfo Line %d",__LINE__);
		pCmd->Parameters->Item[_variant_t("@BINDATA")]->AppendChunk(bin);
		_log(" spSaveGuildInfo Line %d",__LINE__);
		VariantClear(&bin);

		VariantInit(&bin);
		memset(compressData,0,100);
		sz = cSRVUTIL::Compress((unsigned char *)_lpMarkInfo,(unsigned char *)compressData,sizeof(CGuildSaveInfoMark));
		_log(" spSaveGuildInfo Line %d",__LINE__);
		BlobToVariant(bin,compressData,sz);
		_log(" spSaveGuildInfo Line %d",__LINE__);
		pCmd->Parameters->Item[_variant_t("@BINMARK")]->AppendChunk(bin);
		_log(" spSaveGuildInfo Line %d",__LINE__);
		VariantClear(&bin);
		
		VariantInit(&bin);
		memset(compressData,0,100);
		sz = cSRVUTIL::Compress((unsigned char *)_lpBankInfo,(unsigned char *)compressData,sizeof(CGuildSaveInfoBank));
		_log(" spSaveGuildInfo Line %d",__LINE__);
		BlobToVariant(bin,compressData,sz);
		_log(" spSaveGuildInfo Line %d",__LINE__);
		pCmd->Parameters->Item[_variant_t("@BINBANK")]->AppendChunk(bin);
		_log(" spSaveGuildInfo Line %d",__LINE__);
		VariantClear(&bin);

		VariantInit(&bin);
		memset(compressData,0,100);
		sz = cSRVUTIL::Compress((unsigned char *)_lpRelShipInfo,(unsigned char *)compressData,sizeof(CGuildSaveInfoRelShip));
		_log(" spSaveGuildInfo Line %d",__LINE__);
		BlobToVariant(bin,compressData,sz);
		_log(" spSaveGuildInfo Line %d",__LINE__);
		pCmd->Parameters->Item[_variant_t("@BINRELSHIP")]->AppendChunk(bin);
		_log(" spSaveGuildInfo Line %d",__LINE__);
		VariantClear(&bin);
		_log(" spSaveGuildInfo Line %d",__LINE__);
		
		delete [] compressData;

		_log(" spSaveGuildInfo Line %d",__LINE__);
		hr = pCmd->Execute(NULL,NULL,NULL);
		_log(" spSaveGuildInfo Line %d",__LINE__);
		VariantClear(&bin);
		if(FAILED(hr))	return 0;

		_log(" spSaveGuildInfo Line %d",__LINE__);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
	
		return 1;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
	return 1;
}

int
cADO_ADMIN::spSaveGuildSkill(int _iIndex, char * _strName, WORD _wGuildPoint, CGuildSaveInfoSkill * _lpInfo)
{
	try
	{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGUILD_SAVESKILL";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)_iIndex);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strName);
		DEF_PARAM(pCmd,"@GUILDPOINT") = _variant_t((long)_wGuildPoint);
		char	* compressData = new char[sizeof(CGuildSaveInfoSkill)];	//	@BINDATA 가 96으로 최고.. 아래 4개중..
		VARIANT bin;WORD sz;
		VariantInit(&bin);
		memset(compressData,0,sizeof(CGuildSaveInfoSkill));
		sz = cSRVUTIL::Compress((unsigned char *)_lpInfo,(unsigned char *)compressData,sizeof(CGuildSaveInfoSkill));
		BlobToVariant(bin,compressData,sz);
		pCmd->Parameters->Item[_variant_t("@DATA")]->AppendChunk(bin);
		VariantClear(&bin);
		delete [] compressData;

		hr = pCmd->Execute(NULL,NULL,NULL);
		VariantClear(&bin);
		if(FAILED(hr))	return 0;

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.lVal = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		int ret = (int)regIdx.lVal;
		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;

		return ret;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
	return 1;
}

int
cADO_ADMIN::spSaveGuildPet(int _iIndex, char * _strName, CGuildSaveInfoPet * _lpInfo)
{
	try
	{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGUILD_SAVEPET";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)_iIndex);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strName);
		char	* compressData = new char[sizeof(CGuildSaveInfoPet)];	//	@BINDATA 가 96으로 최고.. 아래 4개중..
		VARIANT bin;WORD sz;
		VariantInit(&bin);
		memset(compressData,0,sizeof(CGuildSaveInfoPet));
		sz = cSRVUTIL::Compress((unsigned char *)_lpInfo,(unsigned char *)compressData,sizeof(CGuildSaveInfoPet));
		BlobToVariant(bin,compressData,sz);
		pCmd->Parameters->Item[_variant_t("@DATA")]->AppendChunk(bin);
		VariantClear(&bin);
		delete [] compressData;

		hr = pCmd->Execute(NULL,NULL,NULL);
		VariantClear(&bin);
		if(FAILED(hr))	return 0;

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.lVal = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		int ret = (int)regIdx.lVal;
		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;

		return ret;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
	return 1;
}

int
cADO_ADMIN::spSaveGuildGuardian(int _iIndex, char * _strName, CGuildSaveInfoGuardian * _lpInfo)
{
	try
	{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGUILD_SAVEGUARDIAN";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)_iIndex);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strName);
		char	* compressData = new char[sizeof(CGuildSaveInfoGuardian)];	//	@BINDATA 가 96으로 최고.. 아래 4개중..
		VARIANT bin;WORD sz;
		VariantInit(&bin);
		memset(compressData,0,sizeof(CGuildSaveInfoGuardian));
		sz = cSRVUTIL::Compress((unsigned char *)_lpInfo,(unsigned char *)compressData,sizeof(CGuildSaveInfoGuardian));
		BlobToVariant(bin,compressData,sz);
		pCmd->Parameters->Item[_variant_t("@DATA")]->AppendChunk(bin);
		VariantClear(&bin);
		delete [] compressData;

		hr = pCmd->Execute(NULL,NULL,NULL);
		VariantClear(&bin);
		if(FAILED(hr))	return 0;

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.lVal = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		int ret = (int)regIdx.lVal;
		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;

		return ret;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
	return 1;
}

int
cADO_ADMIN::spSaveGuildInventory(int _iIndex, char * _strName, CGuildSaveInfoInventory * _lpInfo)
{
	try
	{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGUILD_SAVEINVENTORY";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)_iIndex);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strName);
		char	* compressData = new char[sizeof(CGuildSaveInfoInventory)];	//	@BINDATA 가 96으로 최고.. 아래 4개중..
		VARIANT bin;WORD sz;
		VariantInit(&bin);
		memset(compressData,0,sizeof(CGuildSaveInfoInventory));
		sz = cSRVUTIL::Compress((unsigned char *)_lpInfo,(unsigned char *)compressData,sizeof(CGuildSaveInfoInventory));
		BlobToVariant(bin,compressData,sz);
		pCmd->Parameters->Item[_variant_t("@DATA")]->AppendChunk(bin);
		VariantClear(&bin);
		delete [] compressData;

		hr = pCmd->Execute(NULL,NULL,NULL);
		VariantClear(&bin);
		if(FAILED(hr))	return 0;

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.lVal = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		int ret = (int)regIdx.lVal;
		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;

		return ret;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
	return 1;
}

int
cADO_ADMIN::spSaveGuildstructure(int _iIndex, char * _strName, CGuildSaveInfoStructure * _lpInfo)
{
	try
	{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGUILD_SAVESTRUCTURE";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)_iIndex);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strName);
		char	* compressData = new char[sizeof(CGuildSaveInfoStructure)];	//	@BINDATA 가 96으로 최고.. 아래 4개중..
		VARIANT bin;WORD sz;
		VariantInit(&bin);
		memset(compressData,0,sizeof(CGuildSaveInfoStructure));
		sz = cSRVUTIL::Compress((unsigned char *)_lpInfo,(unsigned char *)compressData,sizeof(CGuildSaveInfoStructure));
		BlobToVariant(bin,compressData,sz);
		pCmd->Parameters->Item[_variant_t("@DATA")]->AppendChunk(bin);
		VariantClear(&bin);
		delete [] compressData;

		hr = pCmd->Execute(NULL,NULL,NULL);
		VariantClear(&bin);
		if(FAILED(hr))	return 0;

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.lVal = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		int ret = (int)regIdx.lVal;
		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;

		return ret;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
	return 1;
}

int
cADO_ADMIN::SaveAllGuildInfo(CGuildAdvanceInfo * _lpInfo)
{
	int ret = 0;

	CGuildSaveInfoForColumn	ColumnInfo;
	CGuildSaveInfoForBinary	BinaryInfo;
	CGuildSaveInfoMark		MarkInfo;
	CGuildSaveInfoBank		BankInfo;
	CGuildSaveInfoRelShip	RelShipInfo;
	CGuildSaveInfoSkill		SkillInfo;
	CGuildSaveInfoPet		PetInfo;
	CGuildSaveInfoGuardian	GuardianInfo;
	CGuildSaveInfoInventory	InventoryInfo;
	CGuildSaveInfoStructure	StructureInfo;

	_lpInfo->getSaveData_Column(&ColumnInfo);
	_lpInfo->getSaveData_Binary(&BinaryInfo);
	_lpInfo->getSaveData_Mark(&MarkInfo);
	_lpInfo->getSaveData_Bank(&BankInfo);
	_lpInfo->getSaveData_RelShip(&RelShipInfo);
	_lpInfo->getSaveData_Skill(&SkillInfo);
	_lpInfo->getSaveData_Pet(&PetInfo);
	_lpInfo->getSaveData_Guardian(&GuardianInfo);
	_lpInfo->getSaveData_Inventory(&InventoryInfo);
	_lpInfo->getSaveData_Structure(&StructureInfo);

	ret = spSaveGuildInfo(&ColumnInfo,&BinaryInfo,&MarkInfo,&BankInfo,&RelShipInfo);
	if(!ret)	return ret;

	//	기타 바이너리도...
	ret = spSaveGuildSkill(_lpInfo->m_wSerial,_lpInfo->m_strName,_lpInfo->m_wGuildPoint,&SkillInfo);
	if(!ret)	return ret;
	ret = spSaveGuildPet(_lpInfo->m_wSerial,_lpInfo->m_strName,&PetInfo);
	if(!ret)	return ret;
	ret = spSaveGuildGuardian(_lpInfo->m_wSerial,_lpInfo->m_strName,&GuardianInfo);
	if(!ret)	return ret;
	ret = spSaveGuildInventory(_lpInfo->m_wSerial,_lpInfo->m_strName,&InventoryInfo);
	if(!ret)	return ret;
	ret = spSaveGuildstructure(_lpInfo->m_wSerial,_lpInfo->m_strName,&StructureInfo);
	return ret;
}

int
cADO_ADMIN::spInsertGuildInfo(CGuildSaveInfoForColumn * _lpColumnInfo, CGuildSaveInfoForBinary * _lpBinaryInfo, CGuildSaveInfoMark * _lpMarkInfo, CGuildSaveInfoBank * _lpBankInfo, CGuildSaveInfoRelShip * _lpRelShipInfo, CGuildSaveInfoSkill * _lpInfo)
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGUILD_INFO_INSERT";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)_lpColumnInfo->m_wSerial);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_lpColumnInfo->m_strName);
		DEF_PARAM(pCmd,"@LEVEL") = _variant_t((short)_lpColumnInfo->m_wLevel);
		DEF_PARAM(pCmd,"@HALLLEVEL") = _variant_t((short)_lpColumnInfo->m_wHallLevel);
		DEF_PARAM(pCmd,"@VICPOINT") = _variant_t((long)_lpColumnInfo->m_iVictoryPoint);
		DEF_PARAM(pCmd,"@HONORPOINT") = _variant_t((long)_lpColumnInfo->m_iHonorPoint);
		DEF_PARAM(pCmd,"@GUILDPOINT") = _variant_t((short)_lpColumnInfo->m_wGuildPoint);
		DEF_PARAM(pCmd,"@HALLSHAPE") = _variant_t((short)_lpColumnInfo->m_wGuildHallShape);
		DEF_PARAM(pCmd,"@GOLD") = _variant_t((DOUBLE)_lpColumnInfo->m_llGuildGold);
		DEF_PARAM(pCmd,"@NTYPE") = _variant_t((short)_lpColumnInfo->m_wNoticeType);
		DEF_PARAM(pCmd,"@NOTICE") = _variant_t((char *)_lpColumnInfo->m_strNotice);
		DEF_PARAM(pCmd,"@MASTER") = _variant_t((char *)_lpColumnInfo->m_strMasterName);
		char	datetime[20];
		UTimeToDatetime(&_lpColumnInfo->m_timeLastConnectTime,datetime);
		DEF_PARAM(pCmd,"@LASTCONNECT") = _variant_t((char *)datetime);

		char	* compressData = new char[320];	//	@BINDATA 가 96으로 최고.. 아래 4개중..
		VARIANT bin;WORD sz;
		VariantInit(&bin);
		memset(compressData,0,320);
		sz = cSRVUTIL::Compress((unsigned char *)_lpBinaryInfo,(unsigned char *)compressData,sizeof(CGuildSaveInfoForBinary));
		if(!sz)
			BlobToVariant(bin,_lpBinaryInfo,sizeof(CGuildSaveInfoForBinary));
		else
			BlobToVariant(bin,compressData,sz);
		pCmd->Parameters->Item[_variant_t("@BINDATA")]->AppendChunk(bin);
		VariantClear(&bin);

		VariantInit(&bin);
		memset(compressData,0,320);
		sz = cSRVUTIL::Compress((unsigned char *)_lpMarkInfo,(unsigned char *)compressData,sizeof(CGuildSaveInfoMark));
		if(!sz)
			BlobToVariant(bin,_lpMarkInfo,sizeof(CGuildSaveInfoMark));
		else
			BlobToVariant(bin,compressData,sz);
		pCmd->Parameters->Item[_variant_t("@BINMARK")]->AppendChunk(bin);
		VariantClear(&bin);
		
		VariantInit(&bin);
		memset(compressData,0,320);
		sz = cSRVUTIL::Compress((unsigned char *)_lpBankInfo,(unsigned char *)compressData,sizeof(CGuildSaveInfoBank));
		if(!sz)
			BlobToVariant(bin,_lpBankInfo,sizeof(CGuildSaveInfoBank));
		else
			BlobToVariant(bin,compressData,sz);
		pCmd->Parameters->Item[_variant_t("@BINBANK")]->AppendChunk(bin);
		VariantClear(&bin);

		VariantInit(&bin);
		memset(compressData,0,320);
		sz = cSRVUTIL::Compress((unsigned char *)_lpRelShipInfo,(unsigned char *)compressData,sizeof(CGuildSaveInfoRelShip));
		if(!sz)
			BlobToVariant(bin,_lpRelShipInfo,sizeof(CGuildSaveInfoRelShip));
		else
			BlobToVariant(bin,compressData,sz);
		pCmd->Parameters->Item[_variant_t("@BINRELSHIP")]->AppendChunk(bin);
		VariantClear(&bin);
		
		VariantInit(&bin);
		memset(compressData,0,320);
		sz = cSRVUTIL::Compress((unsigned char *)_lpInfo,(unsigned char *)compressData,sizeof(CGuildSaveInfoSkill));
		if(!sz)
			BlobToVariant(bin,_lpInfo,sizeof(CGuildSaveInfoSkill));
		else
			BlobToVariant(bin,compressData,sz);
		pCmd->Parameters->Item[_variant_t("@BINSKILL")]->AppendChunk(bin);
		VariantClear(&bin);

		delete [] compressData;

		hr = pCmd->Execute(NULL,NULL,NULL);
		VariantClear(&bin);
		if(FAILED(hr))	return 0;

		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
	
		return 1;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
	return 1;
}

int
cADO_ADMIN::spInsertGuildData(int _iIndex, char * _strName, CGuildSaveInfoPet * _lpInfoPet, CGuildSaveInfoGuardian * _lpInfoGuardian, CGuildSaveInfoInventory * _lpInfoInventory, CGuildSaveInfoStructure * _lpInfoStructure)
{
	try
	{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGUILD_DATA_INSERT";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)_iIndex);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strName);

		char	* compressData = new char[5600];
		VARIANT bin;WORD sz;

		VariantInit(&bin);
		memset(compressData,0,5600);
		sz = cSRVUTIL::Compress((unsigned char *)_lpInfoPet,(unsigned char *)compressData,sizeof(CGuildSaveInfoPet));
		BlobToVariant(bin,compressData,sz);
		pCmd->Parameters->Item[_variant_t("@BINPET")]->AppendChunk(bin);
		VariantClear(&bin);
		
		VariantInit(&bin);
		memset(compressData,0,5600);
		sz = cSRVUTIL::Compress((unsigned char *)_lpInfoGuardian,(unsigned char *)compressData,sizeof(CGuildSaveInfoGuardian));
		BlobToVariant(bin,compressData,sz);
		pCmd->Parameters->Item[_variant_t("@BINGUARDIAN")]->AppendChunk(bin);
		VariantClear(&bin);		
		
		VariantInit(&bin);
		memset(compressData,0,5600);
		sz = cSRVUTIL::Compress((unsigned char *)_lpInfoInventory,(unsigned char *)compressData,sizeof(CGuildSaveInfoInventory));
		BlobToVariant(bin,compressData,sz);
		pCmd->Parameters->Item[_variant_t("@BININVENTORY")]->AppendChunk(bin);
		VariantClear(&bin);
		
		VariantInit(&bin);
		memset(compressData,0,5600);
		sz = cSRVUTIL::Compress((unsigned char *)_lpInfoStructure,(unsigned char *)compressData,sizeof(CGuildSaveInfoStructure));
		BlobToVariant(bin,compressData,sz);
		pCmd->Parameters->Item[_variant_t("@BINSTRUCTURE")]->AppendChunk(bin);
		VariantClear(&bin);
		
		delete [] compressData;

		hr = pCmd->Execute(NULL,NULL,NULL);
		VariantClear(&bin);
		if(FAILED(hr))	return 0;

		return 1;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
	return 1;
}
