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


WORD	cADO_ADMIN::spGetAvatarIndexOnly(char *_strId, char *_strName)
{
	_RecordsetPtr  pRs = NULL;
	_variant_t     v_bufferdata;
	char	       str_Query[1024];
	int i_RecCount;
	WORD	wResult = 9;
	char *pBuf = NULL;
	HRESULT hr = CoInitialize(0);

	sprintf(str_Query,"SELECT avaidx FROM Avatar_Current WHERE master='%s' and name='%s'",_strId,_strName);
	printf(str_Query);
	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount;

	if(FAILED(hr))		{	goto END_spGetAvatarIndexOnly;				}
	if(i_RecCount <= 0)	{	wResult = 8; goto END_spGetAvatarIndexOnly;	}

	v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
	wResult = v_bufferdata.intVal;

END_spGetAvatarIndexOnly:
	pRs->Close();
	return wResult;
}

BOOL	cADO_ADMIN::spChangeIndex(char *id,char *name,int index)
{
	cPLAYER_DATA		pData;
	_RecordsetPtr  pRs = NULL;
	_variant_t     v_bufferdata;
	char	       str_Query[1024];
	int i_RecCount;
	char *pBuf = NULL;
	HRESULT hr = CoInitialize(0);

	sprintf(str_Query,"SELECT avaidx , data FROM Avatar_Current WHERE master='%s' and name='%s'",id,name);
	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount; 				

	if(!FAILED(hr))
	{
		if(i_RecCount==0)
		{
			pRs->Close();
			return FALSE;
		}else{
			v_bufferdata = pRs->Fields->GetItem((LONG)1)->Value;
			char * uncompressdata = new char[sizeof(cPLAYER_DATA)];
			int unsize = VariantToBlob(v_bufferdata,uncompressdata);
			if(unsize<sizeof(cPLAYER_DATA))
			{
				WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)&pData,unsize,sizeof(cPLAYER_DATA));
				if(ww == 0)
				{
					if	(unsize >= 2048)
					{
						memset(&pData,0,sizeof(cPLAYER_DATA));
						memcpy(&pData,uncompressdata,unsize);
					}

					_log(" uncompress error 2");
				}
			}else{
				memcpy(&pData,uncompressdata,sizeof(cPLAYER_DATA));
			}
			pData.m_wAvatarIndex = index;
			delete [] uncompressdata;
		}
	}
	pRs->Close();

	_log(" CHECK :: ");
	_log(" spChangeIndex : [%s] idx [%d]", pData.m_strName, pData.m_wAvatarIndex);
	spSaveAva(&pData, FALSE);

	return TRUE;
}

void	cADO_ADMIN::spSaveAva(cPLAYER_DATA * pData,BOOL leveldown,BOOL isNew)
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spAvatar_SAVEAVAINFO";
		pCmd->CommandType  = adCmdStoredProc;

		cAVATAR_INFO	ainfo;
		pData->getAvatarData(&ainfo);

		char	* compressData = new char[sizeof(cPLAYER_DATA)];
		VARIANT bin;	VariantInit(&bin);
		memset(compressData,0,sizeof(cPLAYER_DATA));
		WORD sz = cSRVUTIL::Compress((unsigned char *)pData,(unsigned char *)compressData,sizeof(cPLAYER_DATA));
		BlobToVariant(bin,compressData,sz);

		DEF_PARAM(pCmd,"@VER") = _variant_t((long)8);
		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)0x7fffffff);
//		DEF_PARAM(pCmd,"@LEVELDOWN") = _variant_t((long)leveldown);
		DEF_PARAM(pCmd,"@LEVELDOWN") = _variant_t((long)1);
		DEF_PARAM(pCmd,"@MASTER") = _variant_t((char *)pData->m_strId);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)pData->m_strName);
		pCmd->Parameters->Item[_variant_t("@DATA")]->AppendChunk(bin);
		DEF_PARAM(pCmd,"@JOB") = _variant_t((long)pData->m_wJob);
		DEF_PARAM(pCmd,"@LEV") = _variant_t((long)pData->m_iLevel);
		DEF_PARAM(pCmd,"@EXP") = _variant_t((long)pData->m_wRedStonePoint);
		DEF_PARAM(pCmd,"@PLAYTIME") = _variant_t((long)pData->m_dwPlayTime);
		DEF_PARAM(pCmd,"@SAVECOUNT") = _variant_t((long)pData->m_dwSaveCount);
		DEF_PARAM(pCmd,"@WEAPON") = _variant_t((long)ainfo.m_wWeapon);
		DEF_PARAM(pCmd,"@ARMOR") = _variant_t((long)ainfo.m_wArmor);
		DEF_PARAM(pCmd,"@SHIELD") = _variant_t((long)ainfo.m_wShield);
		DEF_PARAM(pCmd,"@LASTFIELD") = _variant_t((long)pData->m_wCurrentField);
		DEF_PARAM(pCmd,"@REBIRTHCOUNT") = _variant_t((long)pData->m_bf4RebirthCount);
		VariantClear(&bin);
		delete [] compressData;
		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))	printf("Save Fail- Execute !! \n");		

		VARIANT regIdx;		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.lVal = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		if((int)regIdx.lVal == -1)
		{		
			MessageBox(NULL,"Save Fail !! \n","",MB_OK);		
		}
		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
	}
	catch( _com_error &e){
		DMPcomERR(e)
	}
}

BOOL	cADO_ADMIN::spReadAva(char * strName,cPLAYER_DATA * pData)
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spAvatar_GETINFOAVATAR";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); 		return FALSE;	}

		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)0x7fffffff);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)strName);
		pCmd->Execute(NULL,NULL,NULL);

		_variant_t v_data;
		char	* uncompressdata = new char[sizeof(cPLAYER_DATA)];
		memset(uncompressdata,0,sizeof(cPLAYER_DATA));
		v_data		= pCmd->GetParameters()->GetItem("@DATA")->Value;
		int unsize = VariantToBlob(v_data,uncompressdata);
		if(unsize<sizeof(cPLAYER_DATA)  && unsize != 2300 && unsize !=2272)
		{
			WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)pData,unsize,sizeof(cPLAYER_DATA));
			if(ww == 0)
			{
				if	(unsize >= 2048)
				{
					memset(pData,0,sizeof(cPLAYER_DATA));
					memcpy(pData,uncompressdata,unsize);
				}
				_log(" uncompress error 2");
			}
		}else{
			memcpy(pData,uncompressdata,sizeof(cPLAYER_DATA));
		}
		delete [] uncompressdata;
		VariantClear(&v_data);
		pCmd.Release();
	}catch( _com_error &e){
		DMPcomERR(e)
		return FALSE;
	}
	return TRUE;
}

BOOL	cADO_ADMIN::spReadAva(BOOL isTestServer,char *ID,_ava4  & p)
{
	_RecordsetPtr  pRs = NULL; 	
	_variant_t     v_bufferdata;
	char	       str_Query[1024];
	int i_RecCount;
	int i;
	char *pBuf = NULL;
	HRESULT hr = CoInitialize(0);

	sprintf(str_Query,"SELECT data FROM Avatar_Current WHERE master='%s'",ID);
	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount; 				

	if(!FAILED(hr))
	{ 
		char	* uncompressdata = new char[sizeof(cPLAYER_DATA)];
		for(i = 0 ;i < i_RecCount ;i++)
		{
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			if(isTestServer)
			{
				memset(&p.ava[i],0,sizeof(cPLAYER_DATA));
				memset(uncompressdata,0,sizeof(cPLAYER_DATA));
				int unsize = VariantToBlob(v_bufferdata,uncompressdata);
				if(unsize<sizeof(cPLAYER_DATA))
				{
					WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)&p.ava[i],unsize,sizeof(cPLAYER_DATA));
					if(ww == 0)
					{
						if	(unsize >= 2048	&&	unsize	<sizeof(cPLAYER_DATA))
						{
							memset(&p.ava[i],0,sizeof(cPLAYER_DATA));
							memcpy(&p.ava[i],uncompressdata,unsize);
						}

						_log(" uncompress error 2");
					}
				}else{
					memcpy(&p.ava[i],uncompressdata,sizeof(cPLAYER_DATA));
				}

//				cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)&p.ava[i],unsize,sizeof(cPLAYER_DATA));
			}else{
				VariantToBlob(v_bufferdata,&p.ava[i]);
			}
			pRs->MoveNext();
		}
		delete [] uncompressdata;
	}

	if(i_RecCount==0)
	{
		pRs->Close();
		return FALSE;
	}
	pRs->Close();

	return TRUE;
}

int		cADO_ADMIN::spDeleteAvatar(char *id,char *name)
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spAvatar_DELETE";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@MASTER") = _variant_t((char *)id);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)name);

		hr = pCmd->Execute(NULL,NULL,NULL);

		if(FAILED(hr)){	return -1;}
		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.lVal = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		int iResult = (int)regIdx.lVal;
		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
		return iResult;
	}catch( _com_error &e){
		DMPcomERR(e)
		return -1;
	}
}

int		cADO_ADMIN::spAvaRename(char *id,char * oldname,char * newname)
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spAvatar_RENAME";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)id);
		DEF_PARAM(pCmd,"@OLDNAME") = _variant_t((char *)oldname);
		DEF_PARAM(pCmd,"@NEWNAME") = _variant_t((char *)newname);

		hr = pCmd->Execute(NULL,NULL,NULL);

		if(FAILED(hr)){	return -1;}
		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.lVal = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		int iResult = (int)regIdx.lVal;
		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
		return iResult;
	}catch( _com_error &e){
		DMPcomERR(e)
		return -1;
	}
}

int		cADO_ADMIN::spAvaRenameSyc(char *id,char * newname, char * getOldName)
{
	cPLAYER_DATA		pData;
	_RecordsetPtr  pRs = NULL; 	
	
	WORD			guildIndx = 0xffff;
	char	       str_Query[1024];
	int i_RecCount;
	char *pBuf = NULL;
	HRESULT hr = CoInitialize(0);

	sprintf(str_Query,"SELECT data,guild FROM Avatar_Current WHERE master='%s' and name='%s'",id,newname);
	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount; 				

	if(!FAILED(hr))
	{
		if(i_RecCount==0)
		{
			pRs->Close();
			return guildIndx;
		}else{
			_variant_t v_data;
			char	* uncompressdata = new char[sizeof(cPLAYER_DATA)];
			memset(uncompressdata,0,sizeof(cPLAYER_DATA));
			v_data		= pRs->Fields->GetItem((LONG)0)->Value;
			int unsize = VariantToBlob(v_data,uncompressdata);
			if(unsize<sizeof(cPLAYER_DATA) && unsize != 2300 && unsize !=2272)
			{
				WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)&pData,unsize,sizeof(cPLAYER_DATA));
				if(ww == 0)
				{
					if	(unsize >= 2048)
					{
						memset(&pData,0,sizeof(cPLAYER_DATA));
						memcpy(&pData,uncompressdata,unsize);
					}

					_log(" uncompress error 2");
				}

			}else{
				memcpy(&pData,uncompressdata,sizeof(cPLAYER_DATA));
			}
			delete [] uncompressdata;

			v_data = pRs->Fields->GetItem((LONG)1)->Value;
			guildIndx = (WORD)v_data.lVal;

			VariantClear(&v_data);
		}
	}
	pRs->Close();
	if(getOldName != NULL)
		strcpy(getOldName , pData.m_strName);
	strcpy(pData.m_strName,newname);
	spSaveAva(&pData, FALSE);

	return guildIndx;
}

int		cADO_ADMIN::spAvaRecover(char *id,char * oldname,char * newname)
{
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spAvatar_Recover";
		pCmd->CommandType  = adCmdStoredProc;

		cLOG->outputf("id [%s], oldname [%s], newname[%s]", id, oldname, newname);

		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)id);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)oldname);
		DEF_PARAM(pCmd,"@NEWNAME") = _variant_t((char *)newname);

		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr)){	return -1;}

		_variant_t regResult;
		VariantInit(&regResult);
		regResult = pCmd->GetParameters()->GetItem("@RESULT")->GetValue();
		int result = (int)regResult.lVal;
		VariantClear(&regResult);

		_variant_t regIdx;
		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@INDEX")->GetValue();
		int index = (int)regIdx.lVal;
		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;

		switch(index)
		{
		case 0:{	result = 10;	break;	}
		case 1:{	result = 11;	break;	}
		case 2:{	result = 12;	break;	}
		case 3:{	result = 13;	break;	}
		case 4:{	result = 14;	break;	}
		case 5:{	result = 15;	break;	}
		case -1:{	result = 5;		break;	}	//	해당 이름이 존재하지 않음
		case -2:{	result = 4;		break;	}	//	최대 카운터 수 초과2	
		case -3:{	result = 6;		break;	}	//	너무 오래된 데이터이므로 복구 불가. 한국에서만 사용하는 것!
		case -4:{	result = 17;	break;	}	//	이미 등록된 이름.
		}

		return result;
	}catch( _com_error &e){
		DMPcomERR(e)
		return -1;
	}
}

int		cADO_ADMIN::spAvaRecoverOld(char *id,char * oldname,char * newname)
{
	return 0;

	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		int				result = 0;

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spAvatar_GETBACKUPDATA";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)id);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)oldname);

		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr)){	return 0;}

		cP_PLAYER_SAVE_DATA		pDataOld;	memset(&pDataOld,0,sizeof(cP_PLAYER_SAVE_DATA));
		_variant_t	v_data;
		v_data = pCmd->GetParameters()->GetItem("@DATA")->Value;
		VariantToBlob(v_data,&pDataOld);
		VariantClear(&v_data);

		VARIANT v_oper;	VariantInit(&v_oper);
		v_oper.vt = VT_INT;
		v_oper.lVal = pCmd->GetParameters()->GetItem("@RESULT")->Value; 
		result = (int)v_oper.lVal;
		VariantClear(&v_oper);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;

		if(result>0)
		{	
			if(strnicmp(id,pDataOld.m_strId,dID_LENGTH-1)!=0 || strnicmp(oldname,pDataOld.m_strName,dNAME_LENGTH-1)!=0)	return 6;
			//	복구한다.
			cPLAYER_DATA	pData;
			if(!pData.convertData(&pDataOld))			return 0;
			if(stricmp(newname,pData.m_strName)!=0)		strcpy(pData.m_strName,newname);
//			result = spCreateAva(&pData);
			spSaveAva(&pData);
			if(result>=0 && result<4)	result++;
		}
		return result;
	}catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
}

int		cADO_ADMIN::spCreateAva(cPLAYER_DATA * pData)
{
	try{
		_CommandPtr		pCmd = NULL;
		VARIANT bin;
		HRESULT			hr = CoInitialize(0);
		BlobToVariant(bin,pData,sizeof(cPLAYER_DATA));
		printf("Create [%s] [%s] \n",pData->m_strId,pData->m_strName);
		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spAvatar_CREATE";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@MASTER") = _variant_t((char *)pData->m_strId);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)pData->m_strName);
		DEF_PARAM(pCmd,"@AVAIDX") = _variant_t((long)pData->m_wAvatarIndex);
		DEF_PARAM(pCmd,"@STARTDATE") = _variant_t((long)cSRVUTIL::GetTime());
		pCmd->Parameters->Item[_variant_t("@DATA")]->AppendChunk(bin);
		DEF_PARAM(pCmd,"@JOB") = _variant_t((long)pData->m_wJob);
		hr = pCmd->Execute(NULL,NULL,NULL);
		VariantClear(&bin);
		if(FAILED(hr))
		{
			return dSP_FAIL;
		}
		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.lVal = pCmd->GetParameters()->GetItem("@IDX")->Value;
		int iResult = (int)regIdx.lVal;
		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;

		if((int)iResult == -1)
		{
			return 11;
		}
		return pData->m_wAvatarIndex;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return dSP_FAIL;
	}
}
