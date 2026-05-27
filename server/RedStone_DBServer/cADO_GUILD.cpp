// cADO_GUILD.cpp: implementation of the cADO_USER class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "cADO_AVATAR.h"
#include "cSRVUTIL.h"
#include "RSComm.h"
#include "CSaveData.h"
#include "Client.h"
#include "userCustomDataDefine.h"

void	cADO_AVATAR::queryGetGuild_Data(GuildListDB * List)
{
	char	       str_Query[1024];
	int i_RecCount;
	int i;
	HRESULT hr = CoInitialize(0);

//	CCritical		cs(&m_csAC);
	try
	{
		_RecordsetPtr  pRs = NULL; 	
		sprintf(str_Query , "SELECT gIdx, binPet, binGuardian, binInventory, binStructure FROM GUILD_DATA");
		pRs.CreateInstance(__uuidof(Recordset)); 

		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
		i_RecCount = pRs->RecordCount;

		char	* uncompressdata = new char[56000];
		int		iComressSize = 0, iTmpCount = 0,gidx=0;

		if	(!FAILED(hr))
		{ 
			for(i = 0 ;i < i_RecCount ;i++) 		
			{
				_variant_t     v_bufferdata;

				//	gIdx
				v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
				gidx = (int)v_bufferdata.intVal;
				for(int z=0; z<i_RecCount; z++)
				{
					if(List[z].dataColumn.m_wSerial == gidx)
					{	gidx = z;
						break;
					}
				}
				//	binPet
				{
					for	(iTmpCount =0 ; iTmpCount< dGUILD_PET_TYPE_COUNT ; iTmpCount++)
						List[gidx].dataPet.m_aGuildPet[iTmpCount].reset();
					v_bufferdata = pRs->Fields->GetItem((LONG)1)->Value;
					if	(v_bufferdata.vt != VT_NULL)
					{
						memset(uncompressdata,0,56000);
						iComressSize = VariantToBlob(v_bufferdata,uncompressdata);
						if(iComressSize>0 && iComressSize<sizeof(CGuildSaveInfoPet) )
						{
							WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)&List[gidx].dataPet,iComressSize,sizeof(CGuildSaveInfoPet));
							if(ww == 0)
								_log(" uncompress error : cADO_AVATAR::queryGetGuild_Data [%d] line %d ",gidx,__LINE__);
						}else
						if(iComressSize == sizeof(CGuildSaveInfoPet))
						{
							memcpy(List[gidx].dataPet.m_aGuildPet,uncompressdata,iComressSize);
						}
						else
						{
							_log(" DATA ERROR -queryGetGuild_Data- %d, %d",i,__LINE__);
						}
					}
					else
					{
//						_log("m_aGuildPet - init cADO_AVATAR::queryGetGuild_Data [%d]",gidx);
					}
				}
				//	binGuardian
				{
					v_bufferdata = pRs->Fields->GetItem((LONG)2)->Value;
					for(iTmpCount =0 ; iTmpCount< dGUILD_GUARDIAN_TYPE_COUNT ; iTmpCount++)
						List[gidx].dataGuardian.m_aGuildGuardian[iTmpCount].reset();
					if(v_bufferdata.vt != VT_NULL)
					{
						memset(uncompressdata,0,56000);
						iComressSize = VariantToBlob(v_bufferdata,uncompressdata);
						if(iComressSize>0 && iComressSize<sizeof(CGuildSaveInfoGuardian) )
						{
							WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)&List[gidx].dataGuardian,iComressSize,sizeof(CGuildSaveInfoGuardian));
							if(ww == 0)
								_log(" uncompress error : cADO_AVATAR::queryGetGuild_Data [%d] line %d ",gidx,__LINE__);
						}else if(iComressSize==sizeof(CGuildSaveInfoGuardian))
						{
							memcpy(List[gidx].dataGuardian.m_aGuildGuardian,uncompressdata,iComressSize);
						}else
						{
							_log(" DATA ERROR -queryGetGuild_Data- %d, %d",i,__LINE__);
						}
					}else{
//						_log("m_aGuildGuardian - init cADO_AVATAR::queryGetGuild_Data [%d]",gidx);
					}
				}
				//	binInventory
				{
					v_bufferdata = pRs->Fields->GetItem((LONG)3)->Value;

					for	(iTmpCount =0 ; iTmpCount< dMAX_GUILD_INVENTORY_COUNT ; iTmpCount++)
						List[gidx].dataInventory.m_aInventory[iTmpCount].reset();

					if(v_bufferdata.vt != VT_NULL)
					{
						memset(uncompressdata,0,56000);
						iComressSize = VariantToBlob(v_bufferdata,uncompressdata);
						if(iComressSize>0 && iComressSize<sizeof(CGuildSaveInfoInventory) )
						{
							WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)&List[gidx].dataInventory,iComressSize,sizeof(CGuildSaveInfoInventory));
							if(ww == 0)	
								_log(" uncompress error : cADO_AVATAR::queryGetGuild_Data [%d] line %d ",gidx,__LINE__);
						}else if	(iComressSize == sizeof(CGuildSaveInfoInventory))
						{
							memcpy(List[gidx].dataInventory.m_aInventory,uncompressdata,iComressSize);
						}else
							_log(" DATA ERROR -queryGetGuild_Data- %d, %d",i,__LINE__);
					}else{
//						_log("m_aGuildInventory - init cADO_AVATAR::queryGetGuild_Data [%d]",gidx);
					}
				}
				//	binStructure
				{
					v_bufferdata = pRs->Fields->GetItem((LONG)4)->Value;

					for	(iTmpCount =0 ; iTmpCount< dMAX_GUILD_STRUCTURE_COUNT ; iTmpCount++)
						List[gidx].dataStructure.m_aGuildStructure[iTmpCount].reset();

					if(v_bufferdata.vt != VT_NULL)
					{
						memset(uncompressdata,0,56000);
						iComressSize = VariantToBlob(v_bufferdata,uncompressdata);
						if(iComressSize>0 && iComressSize<sizeof(CGuildSaveInfoStructure) )
						{
							WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)&List[gidx].dataStructure,iComressSize,sizeof(CGuildSaveInfoStructure));
							if(ww == 0)
								_log(" uncompress error : cADO_AVATAR::queryGetGuild_Data [%d] line %d ",gidx,__LINE__);
						}
						else if	(iComressSize == sizeof(CGuildSaveInfoStructure))
						{
							memcpy(List[gidx].dataStructure.m_aGuildStructure,uncompressdata,iComressSize);
						}else
							_log(" DATA ERROR -queryGetGuild_Data- %d, %d",i,__LINE__);
					}else{
//						_log("m_aGuildStructure - init cADO_AVATAR::queryGetGuild_Data [%d]",gidx);
					}
				}
				VariantClear(&v_bufferdata);
				pRs->MoveNext();
			}
		}
		delete [] uncompressdata;
		uncompressdata = NULL;
		pRs->Close();
		pRs.Release();
		pRs = NULL;
	}catch(_com_error &e)
	{
		DMPcomERR(e)
	}
}

void	cADO_AVATAR::queryGetGuild_List(GuildListDB * List, int & _iListCount)
{
	char	       str_Query[1024];
	int i_RecCount;
	int i;
	HRESULT hr = CoInitialize(0);
	char	strDate[64];

//	CCritical		cs(&m_csAC);
	try
	{
		_RecordsetPtr  pRs = NULL; 	
		sprintf(str_Query , "SELECT * FROM GUILD_INFO");
		pRs.CreateInstance(__uuidof(Recordset)); 

		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
		i_RecCount = pRs->RecordCount;
		_iListCount=0;

		char	uncompressdata[320];
		int		iComressSize = 0, iTmpCount = 0;
		if(!FAILED(hr))
		{ 
			for(i = 0 ;i < i_RecCount ;i++) 		
			{
				_variant_t     v_bufferdata;
				v_bufferdata = pRs->Fields->GetItem((LONG)0)->GetValue();//	[iIndex] [int]
				List[i].dataColumn.m_wSerial = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)1)->GetValue();//	[strName] [nvarchar] (34)
				strcpy(List[i].dataColumn.m_strName,(_bstr_t)v_bufferdata);
				v_bufferdata = pRs->Fields->GetItem((LONG)2)->GetValue();//	[iLevel] [smallint]
				List[i].dataColumn.m_wLevel = (WORD)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)3)->GetValue();//	[iHallLevel] [smallint]
				List[i].dataColumn.m_wHallLevel = (WORD)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)4)->GetValue();//	[iVicPoint] [int]
				List[i].dataColumn.m_iVictoryPoint = (int)v_bufferdata.intVal;

				v_bufferdata = pRs->Fields->GetItem((LONG)5)->GetValue();//	[iHonorPoint] [int]
				List[i].dataColumn.m_iHonorPoint = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)6)->GetValue();//	[iGuildPoint] [smallint]
				List[i].dataColumn.m_wGuildPoint = (WORD)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)7)->GetValue();//	[iHallShape] [smallint]
				List[i].dataColumn.m_wGuildHallShape = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)8)->GetValue();//	[biGold] [bigint]
/*원래longlong*/List[i].dataColumn.m_llGuildGold = (long)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)9)->GetValue();//	[iNoticeType] [smallint]
				List[i].dataColumn.m_wNoticeType = (WORD)v_bufferdata.intVal;

				v_bufferdata = pRs->Fields->GetItem((LONG)10)->GetValue();//[strNotice] [nvarchar] (256)
				strcpy(List[i].dataColumn.m_strNotice,(_bstr_t)v_bufferdata);
				v_bufferdata = pRs->Fields->GetItem((LONG)11)->GetValue();//[strMaster] [nvarchar] (20)
				strcpy(List[i].dataColumn.m_strMasterName,(_bstr_t)v_bufferdata);
				v_bufferdata = pRs->Fields->GetItem((LONG)12)->GetValue();//[timeOrganize] [datetime]
				strcpy(strDate,(_bstr_t)v_bufferdata);
				getTimeToUTime(strDate,&List[i].dataColumn.m_timeOrganizeDate);
//				printf(" [%s]'s m_timeOrganizeDate %s",List[i].dataColumn.m_strName,strDate);
				v_bufferdata = pRs->Fields->GetItem((LONG)13)->GetValue();//[timeLastConnect] [datetime]
				strcpy(strDate,(_bstr_t)v_bufferdata);
				getTimeToUTime(strDate,&List[i].dataColumn.m_timeLastConnectTime);
//				printf(" [%s]'s m_timeLastConnectTime %s",List[i].dataColumn.m_strName,strDate);
				//	[binBasicData] [varbinary] (96)
				v_bufferdata = pRs->Fields->GetItem((LONG)14)->Value;
				memset(&List[i].dataBinary,0,sizeof(CGuildSaveInfoForBinary));
				if(v_bufferdata.vt != VT_NULL)
				{
					memset(uncompressdata,0,320);
					iComressSize = VariantToBlob(v_bufferdata,uncompressdata);
					if	(iComressSize>0 && iComressSize<sizeof(CGuildSaveInfoForBinary))
					{
						WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)&List[i].dataBinary,iComressSize,sizeof(CGuildSaveInfoForBinary));

						if	(ww == 0)
						{
							memcpy(&List[i].dataBinary,uncompressdata,iComressSize);
							_log("copy data %d byte",iComressSize);
						}
					}else
					if	(iComressSize == sizeof(CGuildSaveInfoForBinary))
					{
						memcpy(&List[i].dataBinary,uncompressdata,sizeof(CGuildSaveInfoForBinary));
					}else
					{
						_log(" DATA ERROR -queryGetGuild_List- %d, %d",i,__LINE__);
					}
				}else{
					_log("m_awGuildBinary - GUILD LIST : GUILD [%d]",List[i].dataColumn.m_wSerial);
				}
				//	[binMark] [varbinary] (32)
				v_bufferdata = pRs->Fields->GetItem((LONG)15)->Value;
				memset(&List[i].dataMark,0,sizeof(CGuildSaveInfoMark));
				List[i].dataMark.m_guildMarkInfo.reset();
				if(v_bufferdata.vt != VT_NULL)
				{
					memset(uncompressdata,0,320);
					iComressSize = VariantToBlob(v_bufferdata,uncompressdata);
					if(iComressSize>0 && iComressSize<sizeof(CGuildSaveInfoMark))
					{
						WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)&List[i].dataMark,iComressSize,sizeof(CGuildSaveInfoMark));
						if(ww == 0)	
							_log(" uncompress error : cADO_AVATAR::queryGetGuild_Data [%d] line %d ",List[i].dataColumn.m_wSerial,__LINE__);
					}else
					if(iComressSize == sizeof(CGuildSaveInfoMark))
					{
						memcpy(&List[i].dataMark,uncompressdata,sizeof(CGuildSaveInfoMark));

						//
						
							

					}else
					{
						_log(" DATA ERROR -queryGetGuild_List- %d, %d",i,__LINE__);
					}
				}else{
					_log("m_awGuildMark - GUILD LIST : GUILD [%d]",List[i].dataColumn.m_wSerial);
				}
				//	[binBank] [varbinary] (32)
				v_bufferdata = pRs->Fields->GetItem((LONG)16)->Value;
				memset(&List[i].dataBank,0,sizeof(CGuildSaveInfoBank));
				if(v_bufferdata.vt != VT_NULL)
				{
					memset(uncompressdata,0,320);
					iComressSize = VariantToBlob(v_bufferdata,uncompressdata);
					if(iComressSize>0 && iComressSize<sizeof(CGuildSaveInfoBank))
					{
						WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)&List[i].dataBank,iComressSize,sizeof(CGuildSaveInfoBank));
						if(ww == 0)	
							_log(" uncompress error : cADO_AVATAR::queryGetGuild_Data [%d] line %d ",List[i].dataColumn.m_wSerial,__LINE__);
					}else
					if(iComressSize == sizeof(CGuildSaveInfoBank))
					{
						memcpy(&List[i].dataBank,uncompressdata,sizeof(CGuildSaveInfoBank));
					}
					else
					{
						_log(" DATA ERROR -queryGetGuild_List- %d, %d",i,__LINE__);
					}
				}else{
					_log("m_awGuildBank - GUILD LIST : GUILD [%d]",List[i].dataColumn.m_wSerial);
				}
				//	[binRelShip] [varbinary] (32)
				v_bufferdata = pRs->Fields->GetItem((LONG)17)->Value;
				memset(&List[i].dataRelShip,0,sizeof(CGuildSaveInfoRelShip));
				if(v_bufferdata.vt != VT_NULL)
				{
					memset(uncompressdata,0,320);
					iComressSize = VariantToBlob(v_bufferdata,uncompressdata);
					if(iComressSize>0 && iComressSize<sizeof(CGuildSaveInfoRelShip))
					{
						WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)&List[i].dataRelShip,iComressSize,sizeof(CGuildSaveInfoRelShip));
						if(ww == 0)	
							_log(" uncompress error : cADO_AVATAR::queryGetGuild_Data [%d] line %d ",List[i].dataColumn.m_wSerial,__LINE__);
					}else
					if(iComressSize == sizeof(CGuildSaveInfoRelShip))
					{
						memcpy(&List[i].dataBank,uncompressdata,iComressSize);
					}else
					{
						_log(" DATA ERROR -queryGetGuild_List- %d, %d",i,__LINE__);
					}
				}else{
					_log("m_awGuildRelShip - GUILD LIST : GUILD [%d]",List[i].dataColumn.m_wSerial);
				}
				//	[binSkill] [varbinary] (320)
				v_bufferdata = pRs->Fields->GetItem((LONG)18)->Value;
				memset(&List[i].dataSkill,0,sizeof(CGuildSaveInfoSkill));
				if(v_bufferdata.vt != VT_NULL)
				{
					memset(uncompressdata,0,320);
					iComressSize = VariantToBlob(v_bufferdata,uncompressdata);
					if(iComressSize>0 && iComressSize<sizeof(CGuildSaveInfoSkill))
					{
						WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)&List[i].dataSkill,iComressSize,sizeof(CGuildSaveInfoSkill));
						if(ww == 0)	
							_log(" uncompress error : cADO_AVATAR::queryGetGuild_Data [%d] line %d ",List[i].dataColumn.m_wSerial,__LINE__);
					}else
					if(iComressSize == sizeof(CGuildSaveInfoSkill))
					{
						memcpy(&List[i].dataSkill,uncompressdata,iComressSize);
					}else
					{
						_log(" DATA ERROR -queryGetGuild_List- %d, %d",i,__LINE__);
					}
				}else{
					_log("m_awGuildSkill - GUILD LIST : GUILD [%d]",List[i].dataColumn.m_wSerial);
				}
				
				VariantClear(&v_bufferdata);
				++_iListCount;
				pRs->MoveNext();
			}
		}
		pRs->Close();	pRs.Release();	pRs = NULL;
	}catch(_com_error &e)
	{
		DMPcomERR(e)
	}
}

void	cADO_AVATAR::queryGetGuild_Member(WORD _wGuildIndex, DWMSG_GUILD_MEMBER * member)
{
	char	       str_Query[1024];
	int i_RecCount;
	int i;
	HRESULT hr = CoInitialize(0);

//	CCritical		cs(&m_csAC);
	try
	{
		_RecordsetPtr  pRs = NULL; 	
		sprintf(str_Query , "SELECT * FROM GUILD_MEMBER WHERE wGuildSerial = %d",_wGuildIndex);
		pRs.CreateInstance(__uuidof(Recordset)); 

		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
		if(!FAILED(hr))
		{ 
			i_RecCount = pRs->RecordCount;
			member->wCount = i_RecCount;
			member->base.wSize = sizeof(cMSG_BASE_TYPE_FORDB)+sizeof(WORD)*3+sizeof(GuildMemberDB)*i_RecCount;
			for(i = 0 ;i < i_RecCount ;i++) 		
			{
				_variant_t     v_bufferdata;
				v_bufferdata = pRs->Fields->GetItem((LONG)1)->GetValue();
				strcpy(member->members[i].strId,(_bstr_t)v_bufferdata);
				v_bufferdata = pRs->Fields->GetItem((LONG)2)->GetValue();
				strcpy(member->members[i].strName,(_bstr_t)v_bufferdata);
				v_bufferdata = pRs->Fields->GetItem((LONG)3)->GetValue();
				member->members[i].wRank = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)4)->GetValue();
				member->members[i].wJob = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)5)->GetValue();
				member->members[i].wLevel = (int)v_bufferdata.intVal;
				v_bufferdata = pRs->Fields->GetItem((LONG)6)->GetValue();
				member->members[i].dwBestowValue = (int)v_bufferdata.intVal;
				VariantClear(&v_bufferdata);
				pRs->MoveNext();
			}
		}else{
			member->wCount = 0;
			member->base.wSize = sizeof(cMSG_BASE_TYPE_FORDB)+sizeof(WORD)*2;
		}
		pRs->Close();	pRs.Release();	pRs = NULL;
	}catch(_com_error &e)
	{
		DMPcomERR(e)
	}
}

WORD	cADO_AVATAR::CreateGuild(WORD _wGuildindex,char * _strGuildName,char * _strMasterId,char * _strMasterName,WORD _wJob,WORD _wLevel)
{
//	CCritical		cs(&m_csAC);
	try{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGUILD_CREATE";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL)
		{	
			pCmd.Release();	 
			return dCREATEGUILD_RESULT_FAIL;	
		}

		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)_wGuildindex);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strGuildName);
		DEF_PARAM(pCmd,"@MASTERID") = _variant_t((char *)_strMasterId);
		DEF_PARAM(pCmd,"@MASTERNAME") = _variant_t((char *)_strMasterName);
		DEF_PARAM(pCmd,"@MASTERJOB") = _variant_t((long)_wJob);
		DEF_PARAM(pCmd,"@MASTERLEVEL") = _variant_t((long)_wLevel);
		pCmd->Execute(NULL,NULL, NULL);

		VARIANT regIdx;
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		WORD	l_wResult = (WORD)regIdx.intVal;
		pCmd.Release();	
		VariantClear(&regIdx);
		return regIdx.intVal;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return dCREATEGUILD_RESULT_FAIL;
	}
}

WORD	cADO_AVATAR::DeleteGuild(WORD _wGuildindex,char * _strGuildName)
{
//	CCritical		cs(&m_csAC);
	try{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGUILD_DELETE";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL)
		{
			pCmd.Release();
			return dCREATEGUILD_RESULT_FAIL;	
		}

		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)_wGuildindex);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strGuildName);
		pCmd->Execute(NULL,NULL,NULL);

		VARIANT regIdx;
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		WORD l_wResult = (WORD)regIdx.intVal;
		VariantClear(&regIdx);
		pCmd.Release(); 

		if(l_wResult == 1)	return dDELETEGUILD_RESULT_NOTFIND;
		return dDELETEGUILD_RESULT_SUCCESS;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return dCREATEGUILD_RESULT_FAIL;
	}
}

WORD	cADO_AVATAR::JoinGuild(WORD _wGuildindex,char * _strGuildName, char * _strId, char * _strName,WORD wRank,WORD wJob,WORD wLevel)
{
//	CCritical		cs(&m_csAC);
	try{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGUILD_JOIN";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); 	return dCREATEGUILD_RESULT_FAIL;	}

		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)_wGuildindex);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strGuildName);
		DEF_PARAM(pCmd,"@JOINID") = _variant_t((char *)_strId);
		DEF_PARAM(pCmd,"@JOINNAME") = _variant_t((char *)_strName);
		DEF_PARAM(pCmd,"@RANK") = _variant_t((long)wRank);
		DEF_PARAM(pCmd,"@JOB") = _variant_t((long)wJob);
		DEF_PARAM(pCmd,"@LEVEL") = _variant_t((long)wLevel);
		pCmd->Execute(NULL,NULL,NULL);

		VARIANT regIdx;
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		WORD	l_wResult = (WORD)	regIdx.intVal;
		VariantClear(&regIdx);
		pCmd.Release(); 
		return l_wResult;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return dJOINGUILD_RESULT_FAIL;
	}
}

WORD	cADO_AVATAR::ExitGuild(WORD _wGuildindex,char * _strGuildName, char * _strName)
{
//	CCritical		cs(&m_csAC);
	try{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGUILD_SECEDE";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); 	return dCREATEGUILD_RESULT_FAIL;	}

		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)_wGuildindex);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strGuildName);
		DEF_PARAM(pCmd,"@SECEDENAME") = _variant_t((char *)_strName);
		pCmd->Execute(NULL,NULL,NULL);

		VARIANT regIdx;
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		WORD	l_wResult = (WORD)regIdx.intVal;
		VariantClear(&regIdx);
		pCmd.Release();
		return l_wResult ;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return dEXITGUILD_RESULT_FAIL;
	}
}

WORD	cADO_AVATAR::SaveGuild_Info(WDMSG_SAVEGUILDBASE * baseinfo)
{
	try{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGUILD_SAVE";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); 	return FALSE;	}

		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)baseinfo->dataColumn.m_wSerial);
		DEF_PARAM(pCmd,"@LEVEL") = _variant_t((short)baseinfo->dataColumn.m_wLevel);
		DEF_PARAM(pCmd,"@HALLLEVEL") = _variant_t((short)baseinfo->dataColumn.m_wHallLevel);
		DEF_PARAM(pCmd,"@VICPOINT") = _variant_t((long)baseinfo->dataColumn.m_iVictoryPoint);
		DEF_PARAM(pCmd,"@HONORPOINT") = _variant_t((long)baseinfo->dataColumn.m_iHonorPoint);
		DEF_PARAM(pCmd,"@GUILDPOINT") = _variant_t((short)baseinfo->dataColumn.m_wGuildPoint);
		DEF_PARAM(pCmd,"@HALLSHAPE") = _variant_t((short)baseinfo->dataColumn.m_wGuildHallShape);
/*longlong값이야 원래!!*/DEF_PARAM(pCmd,"@GOLD") = _variant_t((long)baseinfo->dataColumn.m_llGuildGold);
		DEF_PARAM(pCmd,"@NTYPE") = _variant_t((short)baseinfo->dataColumn.m_wNoticeType);
		DEF_PARAM(pCmd,"@NOTICE") = _variant_t((char *)baseinfo->dataColumn.m_strNotice);
		char	datetime[20];
		cSRVUTIL::GetMSSQLDBDateNTime(datetime,(cpUTime *)&baseinfo->dataColumn.m_timeLastConnectTime);
		DEF_PARAM(pCmd,"@LASTCONNECT") = _variant_t((char *)datetime);

		VARIANT bin;
		VariantInit(&bin);
		BlobToVariant(bin,&baseinfo->dataBinary,sizeof(CGuildSaveInfoForBinary));
		pCmd->Parameters->Item[_variant_t("@BINDATA")]->AppendChunk(bin);
		VariantClear(&bin);

		VariantInit(&bin);
		BlobToVariant(bin,&baseinfo->dataMark,sizeof(CGuildSaveInfoMark));
		pCmd->Parameters->Item[_variant_t("@BINMARK")]->AppendChunk(bin);
		VariantClear(&bin);
		
		VariantInit(&bin);
		BlobToVariant(bin,&baseinfo->dataBank,sizeof(CGuildSaveInfoBank));
		pCmd->Parameters->Item[_variant_t("@BINBANK")]->AppendChunk(bin);
		VariantClear(&bin);

		VariantInit(&bin);
		BlobToVariant(bin,&baseinfo->dataRelShip,sizeof(CGuildSaveInfoRelShip));
		pCmd->Parameters->Item[_variant_t("@BINRELSHIP")]->AppendChunk(bin);
		VariantClear(&bin);
		
		HRESULT hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))	return 0;

		pCmd.Release();		pCmd.Detach();		pCmd = NULL;

		return TRUE;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return FALSE;
	}
}

WORD	cADO_AVATAR::SaveGuild_Skill(WORD _wGuildindex,char * _strGuildName, WORD _wGuildPoint,void * _lpData,DWORD	_dwSize)
{
//	CCritical		cs(&m_csAC);
	try{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGUILD_SAVESKILL";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); 	return FALSE;	}

		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)_wGuildindex);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strGuildName);
		DEF_PARAM(pCmd,"@GUILDPOINT") = _variant_t((long)_wGuildPoint);
		
		VARIANT bin;
		char	compressData[3000];
		memset(compressData,0,3000);

		WORD sz	=	(WORD)cSRVUTIL::Compress((unsigned char *)_lpData,(unsigned char *)compressData,(WORD)_dwSize);

		if	(sz>0 && sz<_dwSize)
			BlobToVariant(bin,compressData,sz);
		else
		{
//			_log(" compress fail : sz size =%d , %d ,%d",sz,_dwSize,__LINE__);
			BlobToVariant(bin,_lpData,_dwSize);
		}
		pCmd->Parameters->Item[_variant_t("@DATA")]->AppendChunk(bin);
		pCmd->Execute(NULL,NULL,NULL);

		VARIANT regIdx;
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		WORD	l_wResult = (WORD)regIdx.intVal;
		VariantClear(&regIdx);

		VariantClear(&bin);
		pCmd.Release(); 
		return l_wResult;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 10;
	}
}

WORD	cADO_AVATAR::SaveGuild_Mark(WORD _wGuildindex,char * _strGuildName, void * _lpData,DWORD _dwSize)
{
/*	CCritical		cs(&m_csAC);
	try{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGUILD_SAVEMARK";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); 	return FALSE;	}

		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)_wGuildindex);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strGuildName);
		
		VARIANT bin;
		char	compressData[3000];
		memset(compressData,0,3000);
		WORD sz = cSRVUTIL::Compress((unsigned char *)_lpData,(unsigned char *)compressData,_dwSize);
		if(sz > 0)	BlobToVariant(bin,compressData,sz);
		else		BlobToVariant(bin,_lpData,_dwSize);
		pCmd->Parameters->Item[_variant_t("@DATA")]->AppendChunk(bin);
		VariantClear(&bin);

		HRESULT hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))	return 0;

		pCmd.Release();		pCmd.Detach();		pCmd = NULL;

		return TRUE;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 10;
	}
*/
	return TRUE;
}

WORD	cADO_AVATAR::SaveGuild_Structure(WORD _wGuildindex,char * _strGuildName, void * _lpData,DWORD	_dwSize)
{
//	CCritical		cs(&m_csAC);
	try{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGUILD_SAVESTRUCTURE";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); 	return FALSE;	}

		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)_wGuildindex);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strGuildName);
		
		VARIANT bin;
		char	compressData[3000];
		memset(compressData,0,3000);
		WORD sz = (WORD)cSRVUTIL::Compress((unsigned char *)_lpData,(unsigned char *)compressData,(WORD)_dwSize);
		if(sz>0 && sz<_dwSize)
			BlobToVariant(bin,compressData,sz);
		else
		{
//			_log(" compress fail : sz size =%d , %d ,%d",sz,_dwSize,__LINE__);
			BlobToVariant(bin,_lpData,_dwSize);
		}
//		_log(" SAVE GUILD ST : [%s] , sz %d",_strGuildName,sz);
		pCmd->Parameters->Item[_variant_t("@DATA")]->AppendChunk(bin);

		HRESULT hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))	return 0;

		VariantClear(&bin);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;

		return TRUE;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 10;
	}
}

WORD	cADO_AVATAR::SaveGuild_Guardian(WORD _wGuildindex,char * _strGuildName, void * _lpData,DWORD	_dwSize)
{
//	CCritical		cs(&m_csAC);
	try{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGUILD_SAVEGUARDIAN";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); 	return FALSE;	}

		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)_wGuildindex);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strGuildName);

		VARIANT bin;
		char	compressData[3000];
		memset(compressData,0,3000);
		WORD sz = (WORD)cSRVUTIL::Compress((unsigned char *)_lpData,(unsigned char *)compressData,(WORD)_dwSize);
		if(sz>0 && sz<_dwSize)
			BlobToVariant(bin,compressData,sz);
		else
		{
//			_log(" compress fail : sz size =%d , %d ,%d",sz,_dwSize,__LINE__);
			BlobToVariant(bin,_lpData,_dwSize);
		}
//		_log(" SAVE GUILD GD : [%s] , sz %d",_strGuildName,sz);
		pCmd->Parameters->Item[_variant_t("@DATA")]->AppendChunk(bin);

		HRESULT hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))	return 0;

		VariantClear(&bin);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;

		return TRUE;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 10;
	}
}

//	현재 DB 설정에 문제가 있다. binary 저장할 때 하나의 테이블에 여러 바이너리를 두는 것도 상관없는거 같은데.. 말썽이네.

WORD	cADO_AVATAR::SaveGuild_Pet(WORD _wGuildindex,char * _strGuildName, void * _lpData,DWORD	_dwSize)
{
//	CCritical		cs(&m_csAC);
	try{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGUILD_SAVEPET";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); 	return FALSE;	}

		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)_wGuildindex);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strGuildName);
		
		VARIANT bin;char	compressData[3000];memset(compressData,0,3000);
		WORD sz = (WORD)cSRVUTIL::Compress((unsigned char *)_lpData,(unsigned char *)compressData,(WORD)_dwSize);
		if(sz>0 && sz<_dwSize)
			BlobToVariant(bin,compressData,sz);
		else
		{
//			_log(" compress fail : sz size =%d , %d ,%d",sz,_dwSize,__LINE__);
			BlobToVariant(bin,_lpData,_dwSize);
		}
//		_log(" SAVE GUILD PET : [%s] , sz %d",_strGuildName,sz);
		pCmd->Parameters->Item[_variant_t("@DATA")]->AppendChunk(bin);

		HRESULT hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))	return 0;

		VariantClear(&bin);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;

		return TRUE;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 10;
	}
}

WORD	cADO_AVATAR::SaveGuild_Inventory(WORD _wGuildindex,char * _strGuildName, void * _lpData,DWORD	_dwSize)
{
	if(!_lpData)
		return FALSE;
//	CCritical		cs(&m_csAC);
	try{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGUILD_SAVEINVENTORY";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); 	return FALSE;	}

		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)_wGuildindex);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strGuildName);
		
		VARIANT bin;char	compressData[56000];memset(compressData,0,56000);
		WORD sz = (WORD)cSRVUTIL::Compress((unsigned char *)_lpData,(unsigned char *)compressData,(WORD)_dwSize);
		if(sz>0 && sz<_dwSize)
			BlobToVariant(bin,compressData,sz);
		else
		{
//			_log(" compress fail : sz size =%d , %d ,%d",sz,_dwSize,__LINE__);
			BlobToVariant(bin,_lpData,_dwSize);
		}
//		_log(" SAVE GUILD IV : [%s] , sz %d",_strGuildName,sz);
		pCmd->Parameters->Item[_variant_t("@DATA")]->AppendChunk(bin);
		
		HRESULT hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))	return 0;

		VariantClear(&bin);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;

		return TRUE;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		
		return 10;
	}
}

WORD	cADO_AVATAR::AppointMaster(WORD _wGuildindex,char * _strGuildName, char * _strOldMaster, char * _strNewMaster,WORD _wRank)
{
//	CCritical		cs(&m_csAC);
	try{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGUILD_APPOINT_MASTER";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); 	return dCHANGERANK_RESULT_FAIL;	}

		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)_wGuildindex);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strGuildName);
		DEF_PARAM(pCmd,"@OLDMASTER") = _variant_t((char *)_strOldMaster);
		DEF_PARAM(pCmd,"@NEWMASTER") = _variant_t((char *)_strNewMaster);
		DEF_PARAM(pCmd,"@SUBRANK") = _variant_t((long)_wRank);
		pCmd->Execute(NULL,NULL,NULL);

		VARIANT vResult;
		vResult = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		WORD	l_wResult = (WORD)vResult.intVal;
		VariantClear(&vResult);
		pCmd.Release();
		return l_wResult ;
	}
	catch( _com_error &e)
	{
		DMPcomERR(e)
		return FALSE;
	}
}

WORD	cADO_AVATAR::ChangeRank(WORD _wGuildindex,char * _strGuildName, char * _strMember, WORD _wRank)
{
//	CCritical		cs(&m_csAC);
	try
	{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spGUILD_CHANGE_RANK";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); 	return dCHANGERANK_RESULT_FAIL;	}

		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)_wGuildindex);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strGuildName);
		DEF_PARAM(pCmd,"@MEMBER") = _variant_t((char *)_strMember);
		DEF_PARAM(pCmd,"@RANK") = _variant_t((long)_wRank);
		pCmd->Execute(NULL,NULL,NULL);

		VARIANT vResult;
		vResult = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		WORD	l_wResult = (WORD)vResult.intVal;
		VariantClear(&vResult);
		pCmd.Release();
		return l_wResult ;
	}
	catch( _com_error &e)
	{
		DMPcomERR(e)
		return FALSE;
	}
}