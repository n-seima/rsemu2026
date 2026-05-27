#include "stdafx.h"
#include "ADB_PARAN.h"
#include "cSRVUTIL.h"
#include "userCustomDataDefine.h"


int
cADO_PARAN::spGetAvaInfo(char * _strName, char * _strId, cPLAYER_DATA * pData,WORD & oper)
{
	CCritical		cs(&m_csAC);
	BOOL ret= FALSE;
	try{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spAvatar_GETINFOAVATAR";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); 		return ret;	}

		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)0x7fffffff);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strName);
		pCmd->Execute(NULL,NULL,NULL);

		_variant_t v_data;
		char	* uncompressdata = new char[4000];
		memset(uncompressdata,0,4000);
		v_data		= pCmd->GetParameters()->GetItem("@DATA")->Value;

		int unsize = VariantToBlob(v_data,uncompressdata);
		if(unsize>0 && unsize<sizeof(cPLAYER_DATA))
		{
			WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)pData,unsize,sizeof(cPLAYER_DATA));
			if(ww == 0)
			{
				// dump 파일에 저장해서 내용 확인해 보자. 왜 압축을 풀때 에러나는지. 데이터가 잘못 넘어온건지..
//				char fn[512];
//				sprintf(fn,"uncom_error2_%s_%s_%d.dump",_strId,_strName, timeGetTime());
//				recoardDataDump(fn,uncompressdata,unsize);
			
				if	(unsize >= 2048 && unsize < sizeof(cPLAYER_DATA))
				{
					memcpy(pData,uncompressdata,unsize);
					ret = TRUE;
				}else
				{
					_log(" uncompress error 2  unsize : %d , %d",unsize,sizeof(cPLAYER_DATA));
					_log(" ERROR -_-;; [%s] [%s]",_strId,_strName);
				}
			}else
				ret = TRUE;
		}else if (unsize == sizeof(cPLAYER_DATA))
		{
			memcpy(pData,uncompressdata,sizeof(cPLAYER_DATA));
			ret = TRUE;
		}else
		{
			_log(" DATA ERROR -spGetAvaInfo- [%s][%s]",_strId,_strName);
		}
		delete [] uncompressdata;
		VariantClear(&v_data);
		
		VARIANT v_return;
//		v_return	= pCmd->GetParameters()->GetItem("@RESULT")->Value;
//		if(_dbIndex==0x7fffffff)
//		{
//			DWORD ret = g_Im.add((DWORD)v_return.intVal,_strName,_strId,pData->m_wAvatarIndex);
//			_log("spGetAvaInfo reindex [%d] [%s] [%s] [%d], ret %d",(DWORD)v_return.intVal,_strName,_strId,pData->m_wAvatarIndex,ret);
//		}
		v_return	= pCmd->GetParameters()->GetItem("@ISOPER")->Value;
		oper		= (WORD)v_return.intVal;
		v_return	= pCmd->GetParameters()->GetItem("@GUILD")->Value;
		pData->m_wGuildSerial = (int)v_return.intVal;
		v_return	= pCmd->GetParameters()->GetItem("@RANK")->Value;
		pData->m_wGuildRank = (int)v_return.intVal;
		VariantClear(&v_return);

		pCmd.Release();
	}
	catch( _com_error &e)
	{
		_log(" ERROR -_-;; [%s] [%s]",_strId,_strName);
		DMPcomERR(e)
	}
	return ret;
}

int
cADO_PARAN::LoadBankInfo(char * master, CBankInfo * bank)
{
	CCritical		cs(&m_csAC);
	try{
		//Command 객체 이용
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spAvatar_GETBANK";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); 	return -1;	}

		DEF_PARAM(pCmd,"@MASTER") = _variant_t((char *)master);
		pCmd->Execute(NULL,NULL,NULL);

		VARIANT v_return;
		v_return = pCmd->GetParameters()->GetItem("@RESULT")->Value; 
		int	l_iResult = (int)v_return.intVal;
		VariantClear(&v_return);

		if( l_iResult==2)
		{
			bank->reset();
			strcpy(bank->m_strId,master);
			bank->m_dwSaveCounter = 2;
		}
		else if( l_iResult==1)
		{
			_variant_t v_data;
			char	* uncompressdata = new char[sizeof(CBankInfo)];
			memset(uncompressdata,0,sizeof(CBankInfo));
			v_data		= pCmd->GetParameters()->GetItem("@BANK")->Value;
			int unsize = VariantToBlob(v_data,uncompressdata);
			if(unsize>0 && unsize<sizeof(CBankInfo))
			{
				WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)bank,unsize,sizeof(CBankInfo));
				if(ww == 0)	
				{
					_log(" uncompress error 3 %d %d",unsize,sizeof(CBankInfo));
					_log(" ERROR -_-;; [%s]",master);
				}
			}else if(unsize == sizeof(CBankInfo)){
				memcpy(bank,uncompressdata,sizeof(CBankInfo));
			}else
			{
				_log(" DATA ERROR -LoadBankInfo- [%s]",master);
			}
			bank->m_dwSaveCounter += 1;
			delete [] uncompressdata;
			VariantClear(&v_data);
		}else{
			//	새롭게 등록한 뱅크다.
			bank->reset();
			strcpy(bank->m_strId,master);
			bank->m_dwSaveCounter = 2;
		}
		pCmd.Release(); 
		return l_iResult;
	}
	catch( _com_error &e)
	{
		DMPcomERR(e)
	}
	return -1;
}

int
cADO_PARAN::spPD_GetData(int _iIdx,char * _strId, char * _strName,void * _binData)
{
	int iResult = 0;
	int idebugLastLine = __LINE__;
	CCritical		cs(&m_csAC);
	try
	{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spPD_GetData";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release();		return -2;	}

		DEF_PARAM(pCmd,"@IDX") = _variant_t((long)_iIdx);
		idebugLastLine = __LINE__;
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strName);
		idebugLastLine = __LINE__;
		pCmd->Execute(NULL,NULL,NULL);

		VARIANT regIdx;
		VariantInit(&regIdx);
		idebugLastLine = __LINE__;
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		iResult = (int)regIdx.intVal;
		VariantClear(&regIdx);

		if( iResult==-1)	//	현재 등록되어 있지 않다.
		{
			idebugLastLine = __LINE__;
			((cUserCustomDataDefine *)_binData)->reset();
		}
		else
		{
			idebugLastLine = __LINE__;
			_variant_t v_data;
			char	* uncompressdata = new char[sizeof(cUserCustomDataDefine)];
			memset(uncompressdata,0,sizeof(cUserCustomDataDefine));
			v_data		= pCmd->GetParameters()->GetItem("@BINDATA")->Value;
			idebugLastLine = __LINE__;
			int unsize = VariantToBlob(v_data,uncompressdata);
			idebugLastLine = __LINE__;
			if(unsize>0 && unsize<sizeof(cUserCustomDataDefine))
			{
				WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)_binData,unsize,sizeof(cUserCustomDataDefine));
				idebugLastLine = __LINE__;
				if(ww == 0)
				{
					_log(" uncompress error 4 %d %d",unsize,sizeof(cUserCustomDataDefine));
	//				((cUserCustomDataDefine *)_binData)->reset();
				}
			}else if(unsize==sizeof(cUserCustomDataDefine))
			{
				memcpy(_binData, uncompressdata, sizeof(cUserCustomDataDefine));
			}else		
				_log(" DATA ERROR -LoadPDInfo- [%d] [%s] [%s]",_iIdx,_strId,_strName);
			idebugLastLine = __LINE__;
			delete [] uncompressdata;
			VariantClear(&v_data);
		}
		pCmd.Release();
		return iResult;
	}
	catch( _com_error &e)
	{
		DMPcomERR(e);
		_log(" Serious error : %d",idebugLastLine);
	}
	return	-1;
}

int
cADO_PARAN::SaveBank(CBankInfo * bank)
{
	CCritical		cs(&m_csAC);
	if(bank==NULL)
	{	_log("ERROR SaveBank : pData is NULL");
		return -1;
	}
//	CCritical		cs(&m_csAC);
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spAvatar_SAVEBANK";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); 		return -1;	}

		DEF_PARAM(pCmd,"@VER") = _variant_t((long)8);
		DEF_PARAM(pCmd,"@MASTER") = _variant_t((char *)bank->m_strId);
		DEF_PARAM(pCmd,"@COUNT") = _variant_t((long)bank->m_dwSaveCounter);
		
		VARIANT bin;
		char	* compressData = new char[sizeof(CBankInfo)];
		memset(compressData,0,sizeof(CBankInfo));
		WORD sz = cSRVUTIL::Compress((unsigned char *)bank,(unsigned char *)compressData,sizeof(CBankInfo));
		if(sz>0 && sz<sizeof(CBankInfo))
			BlobToVariant(bin,compressData,sz);
		else
		{
//			_log(" compress fail : sz size =%d , %d , %d",sz,sizeof(CBankInfo),__LINE__);
			BlobToVariant(bin,bank,sizeof(CBankInfo));
		}
		pCmd->Parameters->Item[_variant_t("@BANK")]->AppendChunk(bin);
		delete [] compressData;
		VariantClear(&bin);

		pCmd->Execute(NULL,NULL,NULL);

		VARIANT regIdx;
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		int	l_iResult = (int)regIdx.intVal;
		VariantClear(&regIdx);
		pCmd.Release();
		return l_iResult;
	}
	catch( _com_error &e){
		DMPcomERR(e)
	}
	return -1;
}

WORD
cADO_PARAN::spSaveAva(char * _strOldId,cPLAYER_DATA * pData,BOOL leveldown,int playtime)
{
	CCritical		cs(&m_csAC);
	int idebugLastLine = __LINE__;
	if	(pData==NULL)
	{
		_log("ERROR spSaveAva : pData is NULL");

		return	FALSE;
	}

//	DWORD	_dbIndex = g_Im.getIndex(pData->m_strName,pData->m_strId);
	DWORD	_dbIndex = 0x7fffffff;
//	CCritical		cs(&m_csAC);
	try
	{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spAvatar_SAVEAVAINFO";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release();		return FALSE;	}

		idebugLastLine = __LINE__;
		cAVATAR_INFO	ainfo;
//		pData->getAvatarData(&ainfo);
		{
			strcpy(ainfo.m_strName,pData->m_strName);
			ainfo.m_wJob		=	pData->m_wJob;
			ainfo.m_wLevel		=	pData->m_iLevel;

			if (pData->m_wJob%2==0)
					ainfo.m_wWeapon	=	pData->m_aEquip[dEQUIP_WEAPON].m_wBaseItem;
			else	ainfo.m_wWeapon	=	pData->m_aEquip[dEQUIP_WEAPON_2].m_wBaseItem;

			ainfo.m_wShield		=	pData->m_aEquip[dEQUIP_SHIELD_BULLET].m_wBaseItem;
			ainfo.m_wArmor		=	pData->m_aEquip[dEQUIP_ARMOR].m_wBaseItem;
			ainfo.m_wLastField	=	pData->m_wCurrentField;
			ainfo.m_wAvatarIndex=	pData->m_wAvatarIndex;
		}

		DEF_PARAM(pCmd,"@VER") = _variant_t((long)8);
		idebugLastLine = __LINE__;
		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)_dbIndex);
		idebugLastLine = __LINE__;
		DEF_PARAM(pCmd,"@LEVELDOWN") = _variant_t((long)leveldown);
		idebugLastLine = __LINE__;
		DEF_PARAM(pCmd,"@MASTER") = _variant_t((char *)_strOldId);
		idebugLastLine = __LINE__;
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)pData->m_strName);
		idebugLastLine = __LINE__;
		
		if(pData->m_wRedStonePoint > 60000)
			pData->m_wRedStonePoint = 0;

		VARIANT bin;char	compressData[4000];memset(compressData,0,4000);
		WORD sz = cSRVUTIL::Compress((unsigned char *)pData,(unsigned char *)compressData,sizeof(cPLAYER_DATA));
		idebugLastLine = __LINE__;
		if(sz>0 && sz<sizeof(cPLAYER_DATA))
			BlobToVariant(bin,compressData,sz);
		else
		{
//			_log(" compress fail : sz size =%d , %d ,%d",sz,sizeof(cPLAYER_DATA),__LINE__);
			BlobToVariant(bin,pData,sizeof(cPLAYER_DATA));
		}
		idebugLastLine = __LINE__;
		pCmd->Parameters->Item[_variant_t("@DATA")]->AppendChunk(bin);
		idebugLastLine = __LINE__;
		VariantClear(&bin);

		DEF_PARAM(pCmd,"@JOB") = _variant_t((long)pData->m_wJob);
		idebugLastLine = __LINE__;
		DEF_PARAM(pCmd,"@LEV") = _variant_t((long)pData->m_iLevel);
		idebugLastLine = __LINE__;
		DEF_PARAM(pCmd,"@EXP") = _variant_t((long)pData->m_wRedStonePoint);
		idebugLastLine = __LINE__;
		DEF_PARAM(pCmd,"@PLAYTIME") = _variant_t((long)pData->m_dwPlayTime);
		idebugLastLine = __LINE__;
		DEF_PARAM(pCmd,"@SAVECOUNT") = _variant_t((long)pData->m_dwSaveCount);
		idebugLastLine = __LINE__;
		DEF_PARAM(pCmd,"@WEAPON") = _variant_t((long)ainfo.m_wWeapon);
		idebugLastLine = __LINE__;
		DEF_PARAM(pCmd,"@ARMOR") = _variant_t((long)ainfo.m_wArmor);
		idebugLastLine = __LINE__;
		DEF_PARAM(pCmd,"@SHIELD") = _variant_t((long)ainfo.m_wShield);
		DEF_PARAM(pCmd,"@LASTFIELD") = _variant_t((long)pData->m_wCurrentField);
		DEF_PARAM(pCmd,"@REBIRTHCOUNT") = _variant_t((long)pData->m_bf4RebirthCount);
		pCmd->Execute(NULL,NULL,NULL);

		VARIANT regIdx;
		VariantInit(&regIdx);
		idebugLastLine = __LINE__;
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		idebugLastLine = __LINE__;
		WORD	l_wResult = (WORD)regIdx.intVal;
		VariantClear(&regIdx);
		iErrorCount = 0;

		pCmd.Release();
		return l_wResult;
	}
	catch( _com_error &e)
	{
		DMPcomERR(e);
		_log(" Serious error : %d , id[%s], name[%s], ",idebugLastLine,pData->m_strId,pData->m_strName);
	}
	
	return	TRUE;
}

int
cADO_PARAN::spPD_SetData(int _iIdx,char * _strId, char * _strName, void * _binData)
{
	CCritical		cs(&m_csAC);
	int iResult = 0;
	int idebugLastLine = __LINE__;
//	CCritical		cs(&m_csAC);
	if(strcmp(_strId,"")==0)
	{
		_log(" spPD_SetData : index %d  id is empty.. ",_iIdx);
		return 0;
	}
	if(!_binData)
	{
		_log(" spPD_SetData : _binData = NULL");
		return 0;
	}
	try
	{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spPD_SetData";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release();		return -2;	}

		idebugLastLine = __LINE__;
		DEF_PARAM(pCmd,"@IDX") = _variant_t((long)_iIdx);
		idebugLastLine = __LINE__;
		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)_strId);
		idebugLastLine = __LINE__;
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strName);
		idebugLastLine = __LINE__;
		
		VARIANT bin;char	compressData[2048];memset(compressData,0,2048);
		WORD sz = cSRVUTIL::Compress((unsigned char *)_binData,(unsigned char *)compressData,sizeof(cUserCustomDataDefine));
		idebugLastLine = __LINE__;
		if(sz>0 && sz<sizeof(cUserCustomDataDefine))
			BlobToVariant(bin,compressData,sz);
		else
		{
//			_log(" compress fail : sz size =%d , %d ,%d",sz,sizeof(cUserCustomDataDefine),__LINE__);
			BlobToVariant(bin,_binData,sizeof(cUserCustomDataDefine));
		}
		idebugLastLine = __LINE__;
		pCmd->Parameters->Item[_variant_t("@BINDATA")]->AppendChunk(bin);
		idebugLastLine = __LINE__;
		VariantClear(&bin);

		pCmd->Execute(NULL,NULL,NULL);

		VARIANT regIdx;
		VariantInit(&regIdx);
		idebugLastLine = __LINE__;
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		idebugLastLine = __LINE__;
		iResult = (int)regIdx.intVal;
		VariantClear(&regIdx);

		pCmd.Release();
		return iResult;
	}
	catch( _com_error &e)
	{
		DMPcomERR(e);
		_log(" Serious error : %d",idebugLastLine);
	}
	return	0;
}

int
cADO_PARAN::deleteOldAvatar(char * _strId, char * _strName, BOOL _isUsingSP)	//	해당 서버의 예전 아이디에 있던 캐릭터를 지움. 주민번호 중복시 처리할 내용
{
	CCritical		cs(&m_csAC);
	int iDbIndex = 0x6fffffff;
	
	try{
		if(_isUsingSP)
		{
			_CommandPtr		pCmd = NULL;
			pCmd.CreateInstance(__uuidof(Command));
			pCmd->ActiveConnection = GetConnection();
			pCmd->CommandText = L"spAvatar_DELETE";
			pCmd->CommandType  = adCmdStoredProc;
			if(pCmd->ActiveConnection==NULL){	pCmd.Release(); 	return 0;	}

			DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)iDbIndex);
			DEF_PARAM(pCmd,"@MASTER") = _variant_t((char *)_strId);
			DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strName);
			pCmd->Execute(NULL,NULL,NULL);

			VARIANT regIdx;
			regIdx = pCmd->GetParameters()->GetItem("@GUILD")->Value;
			int _wGuild = (int)regIdx.intVal;
			VariantInit(&regIdx);
			regIdx = pCmd->GetParameters()->GetItem("@RESULT")->Value;
			int		l_iResult = (int)regIdx.intVal;
			VariantClear(&regIdx);
			pCmd.Release(); 

			if(l_iResult  == -1)	return 0;

			_log("deleteOldAvatar : [%s] [%s]",_strName,_strId);

			return l_iResult ;
		}else
		{
			char			str_Query[1024];
			HRESULT			hr = CoInitialize(0);
			sprintf(str_Query,"DELETE FROM AVATAR_CURRENT WHERE master='%s' and name='%s'",_strId,_strName);

			_RecordsetPtr	pRs = NULL;
			pRs.CreateInstance(__uuidof(Recordset)); 
			pRs->CursorLocation  = adUseClient;	
			hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);

			if(pRs->GetState() == adStateClosed) return TRUE;
			if(FAILED(hr))			return 0;
			if(!pRs->RecordCount)	return 0;
			
			VARIANT v_return;
			v_return = pRs->Fields->Item[_variant_t("RETURN VALUE")]->Value;
			int	l_iResult = (int)v_return.intVal;
			VariantClear(&v_return);

			pRs->Close();
			pRs.Release();	pRs = NULL;
		}
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return 0;
	}
	return 1;
}

int
cADO_PARAN::updateAvatarInfo(char * _strOldId, char * _strNewId)
{
	//	기본 아이디의 은행 정보를 삭제한다.
	delBankId(_strOldId);
	delCurtomDataId(_strOldId);
	chgBankId(_strOldId, _strNewId);
	chgAvaId(_strOldId, _strNewId);
//	chgCurtomDataId(_strOldId, _strNewId, 0);

	return TRUE;
}

int
cADO_PARAN::changeGuildInfo(char * _strOldId, char * _strNewId)
{
	CCritical		cs(&m_csAC);
	char			str_Query[1024];
	HRESULT			hr = CoInitialize(0);
	try
	{
		sprintf(str_Query,"UPDATE GUILD_MEMBER SET strId = '%s' WHERE strId='%s'",_strNewId,_strOldId);

		_RecordsetPtr	pRs = NULL;
		pRs.CreateInstance(__uuidof(Recordset)); 
		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);

		if(pRs->GetState() == adStateClosed) return TRUE;
		if(FAILED(hr))			return 0;
		if(!pRs->RecordCount)	return 0;
		
		VARIANT v_return;
		v_return = pRs->Fields->Item[_variant_t("RETURN VALUE")]->Value;
		int	l_iResult = (int)v_return.intVal;
		VariantClear(&v_return);

		pRs->Close();
		pRs.Release();	pRs = NULL;
	}catch(_com_error &e)
	{
		DMPcomERR(e)
	}

	return TRUE;
}

int	
cADO_PARAN::delBankId(char * _strOldId)
{
	CCritical		cs(&m_csAC);
	char			str_Query[1024];
	HRESULT			hr = CoInitialize(0);
	try
	{
		_RecordsetPtr	pRs = NULL;
		sprintf(str_Query,"DELETE FROM Avatar_Bank WHERE master = '%s' ",_strOldId);
		pRs.CreateInstance(__uuidof(Recordset)); 
		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);

		if(pRs->GetState() == adStateClosed) return TRUE;
		if(FAILED(hr))			return 0;
		if(!pRs->RecordCount)	return 0;
		
		VARIANT v_return;
		v_return = pRs->Fields->Item[_variant_t("RETURN VALUE")]->Value;
		int	l_iResult = (int)v_return.intVal;
		VariantClear(&v_return);

		pRs->Close();
		pRs.Release();	pRs = NULL;
	}catch(_com_error &e)
	{
		DMPcomERR(e)
	}

	return 1;
}

int
cADO_PARAN::delCurtomDataId(char * _strOldId)
{
	CCritical		cs(&m_csAC);
	char			str_Query[1024];
	HRESULT			hr = CoInitialize(0);
	try
	{
		_RecordsetPtr	pRs = NULL;
		sprintf(str_Query,"DELETE FROM PERSONALDATA WHERE id = '%s' ",_strOldId);
		pRs.CreateInstance(__uuidof(Recordset)); 
		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);

		if(pRs->GetState() == adStateClosed) return TRUE;
		if(FAILED(hr))			return 0;
		if(!pRs->RecordCount)	return 0;
		
		VARIANT v_return;
		v_return = pRs->Fields->Item[_variant_t("RETURN VALUE")]->Value;
		int	l_iResult = (int)v_return.intVal;
		VariantClear(&v_return);

		pRs->Close();
		pRs.Release();	pRs = NULL;
	}catch(_com_error &e)
	{
		DMPcomERR(e)
	}

	return 1;
}

int
cADO_PARAN::chgBankId(char * _strOldId, char * _strNewId)
{
	CCritical		cs(&m_csAC);
	char			str_Query[1024];
	HRESULT			hr = CoInitialize(0);
	try
	{
		_RecordsetPtr	pRs = NULL;
		sprintf(str_Query,"UPDATE Avatar_Bank SET master = '%s' WHERE master='%s'",_strNewId,_strOldId);
		pRs.CreateInstance(__uuidof(Recordset)); 
		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);

		if(pRs->GetState() == adStateClosed) return TRUE;
		if(FAILED(hr))			return 0;
		if(!pRs->RecordCount)	return 0;
		
		VARIANT v_return;
		v_return = pRs->Fields->Item[_variant_t("RETURN VALUE")]->Value;
		int	l_iResult = (int)v_return.intVal;
		VariantClear(&v_return);


		pRs->Close();
		pRs.Release();	pRs = NULL;
	}catch(_com_error &e)
	{
		DMPcomERR(e)
	}

	return 1;
}

int
cADO_PARAN::chgAvaId(char * _strOldId, char * _strNewId)
{
	CCritical		cs(&m_csAC);
	char			str_Query[1024];
	HRESULT			hr = CoInitialize(0);
	try
	{
		_RecordsetPtr	pRs = NULL;
		sprintf(str_Query,"UPDATE Avatar_Current SET master = '%s' WHERE master='%s'",_strNewId,_strOldId);
		pRs.CreateInstance(__uuidof(Recordset)); 
		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);

		if(pRs->GetState() == adStateClosed) return TRUE;
		if(FAILED(hr))			return 0;
		if(!pRs->RecordCount)	return 0;
		
		VARIANT v_return;
		v_return = pRs->Fields->Item[_variant_t("RETURN VALUE")]->Value;
		int	l_iResult = (int)v_return.intVal;
		VariantClear(&v_return);

		pRs->Close();
		pRs.Release();	pRs = NULL;
	}catch(_com_error &e)
	{
		DMPcomERR(e)
	}

	return 1;
}

int
cADO_PARAN::chgCurtomDataId(char * _strOldId, char * _strNewId,DWORD _idx)
{
	CCritical		cs(&m_csAC);
	char			str_Query[1024];
	HRESULT			hr = CoInitialize(0);
	try
	{
		_RecordsetPtr	pRs = NULL;
		sprintf(str_Query,"UPDATE PERSONALDATA SET idx = %d, id = '%s' WHERE id='%s'",_idx, _strNewId,_strOldId);
		pRs.CreateInstance(__uuidof(Recordset)); 
		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);

		if(pRs->GetState() == adStateClosed) return TRUE;
		if(FAILED(hr))			return 0;
		if(!pRs->RecordCount)	return 0;
		
		VARIANT v_return;
		v_return = pRs->Fields->Item[_variant_t("RETURN VALUE")]->Value;
		int	l_iResult = (int)v_return.intVal;
		VariantClear(&v_return);

		pRs->Close();
		pRs.Release();	pRs = NULL;
	}catch(_com_error &e)
	{
		DMPcomERR(e)
	}

	return 1;
}

DWORD
cADO_PARAN::spCreateAva(cPLAYER_DATA * pData)
{
	CCritical		cs(&m_csAC);

	try
	{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spAvatar_CREATE_FOR_PARAN";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release();		return FALSE;	}

		cAVATAR_INFO	ainfo;
		{
			strcpy(ainfo.m_strName,pData->m_strName);
			ainfo.m_wJob		=	pData->m_wJob;
			ainfo.m_wLevel		=	pData->m_iLevel;

			if (pData->m_wJob%2==0)
					ainfo.m_wWeapon	=	pData->m_aEquip[dEQUIP_WEAPON].m_wBaseItem;
			else	ainfo.m_wWeapon	=	pData->m_aEquip[dEQUIP_WEAPON_2].m_wBaseItem;

			ainfo.m_wShield		=	pData->m_aEquip[dEQUIP_SHIELD_BULLET].m_wBaseItem;
			ainfo.m_wArmor		=	pData->m_aEquip[dEQUIP_ARMOR].m_wBaseItem;
			ainfo.m_wLastField	=	pData->m_wCurrentField;
			ainfo.m_wAvatarIndex=	pData->m_wAvatarIndex;
		}

		DEF_PARAM(pCmd,"@MASTER") = _variant_t((char *)pData->m_strId);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)pData->m_strName);
		DEF_PARAM(pCmd,"@AVAIDX") = _variant_t((long)pData->m_wAvatarIndex);
		DEF_PARAM(pCmd,"@STARTDATE") = _variant_t((long)20071105);
		
		if(pData->m_wRedStonePoint > 60000)
			pData->m_wRedStonePoint = 0;

		VARIANT bin;char	compressData[4000];memset(compressData,0,4000);
		WORD sz = cSRVUTIL::Compress((unsigned char *)pData,(unsigned char *)compressData,sizeof(cPLAYER_DATA));
		if(sz>0 && sz<sizeof(cPLAYER_DATA))
			BlobToVariant(bin,compressData,sz);
		else
		{
//			_log(" compress fail : sz size =%d , %d ,%d",sz,sizeof(cPLAYER_DATA),__LINE__);
			BlobToVariant(bin,pData,sizeof(cPLAYER_DATA));
		}
		pCmd->Parameters->Item[_variant_t("@DATA")]->AppendChunk(bin);
		VariantClear(&bin);

		DEF_PARAM(pCmd,"@JOB") = _variant_t((long)pData->m_wJob);
		DEF_PARAM(pCmd,"@LEV") = _variant_t((long)pData->m_iLevel);
		DEF_PARAM(pCmd,"@WEAPON") = _variant_t((long)ainfo.m_wWeapon);
		DEF_PARAM(pCmd,"@ARMOR") = _variant_t((long)ainfo.m_wArmor);
		DEF_PARAM(pCmd,"@SHIELD") = _variant_t((long)ainfo.m_wShield);
		DEF_PARAM(pCmd,"@LASTFIELD") = _variant_t((long)pData->m_wCurrentField);
		DEF_PARAM(pCmd,"@GUILD") = _variant_t((long)pData->m_wGuildSerial);
		DEF_PARAM(pCmd,"@GUILDRANK") = _variant_t((long)pData->m_wGuildRank);
		_log("CREATE AVA : [%s(%s)'s guild info : %d, rank %d]",pData->m_strId, pData->m_strName, pData->m_wGuildSerial, pData->m_wGuildRank);
		pCmd->Execute(NULL,NULL,NULL);

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@DBINDEX")->Value;
		DWORD	l_wResult = (DWORD)regIdx.intVal;
		regIdx = pCmd->GetParameters()->GetItem("@IDX")->Value;
		WORD	l_wIDX = (WORD)regIdx.intVal;
		VariantClear(&regIdx);
		iErrorCount = 0;

		pCmd.Release();
		return l_wResult;
	}
	catch( _com_error &e)
	{
		DMPcomERR(e);
		_log(" Serious error : id[%s], name[%s], ",pData->m_strId,pData->m_strName);
	}
	
	return	FALSE;
}

/*
CREATE PROCEDURE spAvatar_CREATE_FOR_PARAN -- 파란 이관용 임시 SP
(
	-- 전달된 값들
	 @MASTER		AS nvarchar(20)
	,@NAME		AS nvarchar(22) 
	,@AVAIDX		as int
	,@STARTDATE		AS int
	,@DATA		AS varbinary(3000)
	,@JOB			AS int
	,@LEV			as int
	,@WEAPON		as int
	,@ARMOR		as int
	,@SHIELD		as int
	,@LASTFIELD		as int
	,@GUILD		as int
	,@GUILDRANK		as int

	,@DBINDEX	as	int	OUTPUT
	,@IDX		as 	int	OUTPUT
)
*/