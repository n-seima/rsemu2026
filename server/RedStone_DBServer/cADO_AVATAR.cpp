// cADO_AVATAR.cpp: implementation of the cADO_USER class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "cADO_AVATAR.h"
#include "cSRVUTIL.h"
#include "RSComm.h"
#include "CSaveData.h"
#include "Client.h"
#include "userCustomDataDefine.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void recoardDataDump(char * _fn, void * data, int size)
{
	FILE * fp = fopen( _fn,"wb");
	if(!fp)
		return ;

	fwrite(data,size,1,fp);

	fclose(fp);
}


cADO_AVATAR::cADO_AVATAR()
{

}

cADO_AVATAR::~cADO_AVATAR()
{
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//		PRIVATE METHOD
/////////////////////////////////////////////////////////////////////////////////////////////////
void	cADO_AVATAR::GetDBIndex(char * master ,_DBINDEXTABLE & table )
{
	CCritical		cs(&m_csAC);
	DWORD			i;
	HRESULT			hr = CoInitialize(0);
	try
	{
		_RecordsetPtr	pRs = NULL;
		pRs.CreateInstance(__uuidof(Recordset));
		pRs->CursorLocation = adUseClient;
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spAVATAR_GETDBINDEX";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release();	pRs.Release();	pRs = NULL;	return ;	}

		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)master);
		pRs = pCmd->Execute(NULL,NULL, NULL);

		VARIANT regIdx;
		i = 0;
		memset(&table,0xff,sizeof(_DBINDEXTABLE));
		while(!pRs->adoEOF)
		{
			regIdx = pRs->Fields->GetItem((LONG)0)->Value;
			table.dwIndex[i] = (DWORD)regIdx.intVal;
			regIdx = pRs->Fields->GetItem((LONG)1)->Value;
			table.wAvatarIndex[i] = (WORD)regIdx.intVal;
			regIdx = pRs->Fields->GetItem((LONG)2)->Value;
			strcpy(table.strName[i],(_bstr_t)regIdx);
			i++;
			VariantClear(&regIdx);
			pRs->MoveNext();
		}

		pRs->Close();
		pRs.Release();	pRs = NULL;

		pCmd.Release();	

	}catch(_com_error &e)
	{
		DMPcomERR(e)
		return ;
	}
	return ;
}

BOOL	cADO_AVATAR::MakeDBIndex()
{
	CCritical		cs(&m_csAC);
	char			str_Query[1024];
	DWORD			dw_RecCount;
	DWORD			i;
	HRESULT			hr = CoInitialize(0);

	try
	{
		_RecordsetPtr	pRs = NULL;
		sprintf(str_Query,"SELECT idx,avaidx,master,name FROM Avatar_Current");
		pRs.CreateInstance(__uuidof(Recordset)); 
		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
		dw_RecCount = pRs->RecordCount;

		if(FAILED(hr))	return FALSE;

		CINDEX_TABLE	idxTable;
		for(i = 0 ;i < dw_RecCount ;i++)
		{
			idxTable.reset();
			_variant_t		v_bufferdata;
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			idxTable.iIndex = (DWORD)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)1)->Value;
			idxTable.wAvaIndex = (WORD)v_bufferdata.intVal;
			v_bufferdata = pRs->Fields->GetItem((LONG)2)->Value;
			strcpy(idxTable.strId,(_bstr_t)v_bufferdata);
			v_bufferdata = pRs->Fields->GetItem((LONG)3)->Value;
			strcpy(idxTable.strName,(_bstr_t)v_bufferdata);
			g_Im.addFirst(idxTable.iIndex,idxTable.strName,idxTable.strId,idxTable.wAvaIndex);
			VariantClear(&v_bufferdata);
			pRs->MoveNext();
		}
//		_log(" Add First IndexManager Count %d, rec count %d",g_Im.dwCount,dw_RecCount);
		pRs->Close();
		pRs.Release();	pRs = NULL;
		return TRUE;
	}catch(_com_error &e)
	{
		DMPcomERR(e)
	}
	
	return FALSE;
}

int		cADO_AVATAR::queryBlankAvaPos(char *master)
{
	int		i;
	int		ava[dMAX_AVATAR_COUNT];
	memset(ava, 0xff, sizeof(int)*dMAX_AVATAR_COUNT);
	HRESULT hr = CoInitialize(0);
	try
	{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spAVATAR_GETLIST";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release();	return dCREATEGUILD_RESULT_FAIL;	}

		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)master);
		pCmd->Execute(NULL,NULL, NULL);

		VARIANT regIdx;
		std::string paramName = "@IDX";
		std::string paramNameValue;
		char	digit[8];
		for(i=0; i<dMAX_AVATAR_COUNT ; i++)
		{
			paramNameValue = paramName;
			itoa(i,digit,8);
			paramNameValue.append( digit );
			regIdx = pCmd->GetParameters()->GetItem(paramNameValue.c_str())->GetValue();	
			if(regIdx.intVal == -1)
			{
				pCmd.Release();
				return i;
			}
		}
		pCmd.Release();	

	}catch(_com_error &e)
	{
		DMPcomERR(e)
		return -1;
	}
	return -1;
}


/////////////////////////////////////////////////////////////////////////////
//			Stored Procedure
/////////////////////////////////////////////////////////////////////////////

int		cADO_AVATAR::spCreateAva(DWORD & _dwIndex, cPLAYER_DATA * pData,int blankPos)
{
	int avaidx = blankPos;
	if(avaidx==-1)	avaidx = queryBlankAvaPos((char *)pData->m_strId);
	if(avaidx==-1)
	{	//	 삭제할것..
		_log("can not find empty slot!!");
		return dSP_CREATEAVA_FULL;
	}
	if(!pData)
	{
		_log("pData is NULL");
		return dSP_CREATEAVA_FULL;
	}

//	CCritical		cs(&m_csAC);
	try{
		_CommandPtr		pCmd = NULL;
		pData->m_wAvatarIndex = avaidx;

		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spAvatar_CREATE";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); 	return dSP_FAIL;	}

		cAVATAR_INFO	ainfo;
		pData->getAvatarData(&ainfo);
		DEF_PARAM(pCmd,"@MASTER") = _variant_t((char *)pData->m_strId);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)pData->m_strName);
		DEF_PARAM(pCmd,"@AVAIDX") = _variant_t((long)avaidx);
		DEF_PARAM(pCmd,"@STARTDATE") = _variant_t((long)cSRVUTIL::GetTime());
		_log("try to make avatar : [%s] [%s] [%d]",pData->m_strId,pData->m_strName,avaidx);

		VARIANT bin;
		char	compressData[dMAX_AVATAR_DATA_SIZE];memset(compressData,0,dMAX_AVATAR_DATA_SIZE);
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
		DEF_PARAM(pCmd,"@WEAPON") = _variant_t((long)ainfo.m_wWeapon);
		DEF_PARAM(pCmd,"@ARMOR") = _variant_t((long)ainfo.m_wArmor);
		DEF_PARAM(pCmd,"@SHIELD") = _variant_t((long)ainfo.m_wShield);
		DEF_PARAM(pCmd,"@LASTFIELD") = _variant_t((long)pData->m_wCurrentField);
		pCmd->Execute(NULL,NULL,NULL);

		VARIANT regIdx;
		regIdx = pCmd->GetParameters()->GetItem("@DBINDEX")->Value;
		_dwIndex = regIdx.intVal;
		regIdx = pCmd->GetParameters()->GetItem("@IDX")->Value;
		int		l_iResult = (int)regIdx.intVal;
		VariantClear(&regIdx);
//		_log(" CREATE RESULT [%s/%s] : %d, dbindex%d",pData->m_strId,pData->m_strName,l_iResult,_dwIndex);

		pCmd.Release();
		if(l_iResult == -1)		return dSP_CREATEAVA_USEDNAME;
		if(l_iResult == -2)		return dSP_CREATEAVA_INCORRRECT_IDX;
		return avaidx;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return dSP_FAIL;
	}
}

WORD	cADO_AVATAR::spDeleteAva(char * master,char * name, WORD  & _wGuild)	//	int idx
{
	int iDbIndex = g_Im.getIndex(name,master);
//	CCritical		cs(&m_csAC);	
	try{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spAvatar_DELETE";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); 	return dSP_FAIL;	}

		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)iDbIndex);
		DEF_PARAM(pCmd,"@MASTER") = _variant_t((char *)master);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)name);
		pCmd->Execute(NULL,NULL,NULL);

		VARIANT regIdx;
		regIdx = pCmd->GetParameters()->GetItem("@GUILD")->Value;
		_wGuild = (int)regIdx.intVal;
		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		int		l_iResult = (int)regIdx.intVal;
		VariantClear(&regIdx);
		pCmd.Release(); 

		if(l_iResult  == -1)	return dSP_DELETEAVA_NOMATCH;

		if(iDbIndex<0x7fffffff && l_iResult >=0 && l_iResult <dMAX_AVATAR_COUNT)
		{
			_log("spDeleteAva : [%s] [%s] [%d]",name,master,iDbIndex);
			g_Im.remove(iDbIndex);
		}else if(iDbIndex>=0x7fffffff && l_iResult >=0 && l_iResult <dMAX_AVATAR_COUNT)
		{
			_log("spDeleteAva : [%s] [%s]",name,master);
			g_Im.remove(name,master);
		}
		return (WORD)l_iResult ;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return dSP_FAIL;
	}
	return 0;
}

void	cADO_AVATAR::GetAvaSInfo(char * master ,DLMSG_GETAVATARS & avas)
{
	int		l_iIndex[dMAX_AVATAR_COUNT];
	for(int i = 0;i<dMAX_AVATAR_COUNT;i++)	
		l_iIndex[i] = 0x7fffffff;
	//	dMAX_AVATAR_COUNT개의 인덱스를 찾아서 보낸다.
	g_Im.getIndexById(master,l_iIndex);

//	CCritical		cs(&m_csAC);
	try{
/*		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spAvatar_GETINFOAVATAR";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); 		return;	}
*/
		_RecordsetPtr	pRs = NULL;
		pRs.CreateInstance(__uuidof(Recordset));
		pRs->CursorLocation = adUseClient;
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spAvatar_GETINFOAVATARS";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); pRs.Release();	pRs = NULL;	return;	}

		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)master);

		VARIANT regIdx;
		std::string paramName = "@IDX";
		std::string paramNameValue;
		char	digit[8];
		for(i=0; i<dMAX_AVATAR_COUNT ; i++)
		{
			paramNameValue = paramName;
			itoa(i,digit,8);
			paramNameValue.append( digit );
			DEF_PARAM(pCmd,paramNameValue.c_str()) = _variant_t((long)l_iIndex[i]);
		}
		pRs = pCmd->Execute(NULL,NULL,NULL);
		WORD l_wCount = 0;
		_variant_t v_varient;
		while(!pRs->adoEOF)
		{
			v_varient = pRs->Fields->Item[_variant_t("avaidx")]->Value;
			avas.aCharacter[l_wCount].m_wAvatarIndex = (WORD)v_varient.intVal;
			v_varient = pRs->Fields->Item[_variant_t("name")]->Value;
			strcpy(avas.aCharacter[l_wCount].m_strName,(_bstr_t)v_varient);
			v_varient = pRs->Fields->Item[_variant_t("job")]->Value;
			avas.aCharacter[l_wCount].m_wJob = (WORD)v_varient.intVal;
			v_varient = pRs->Fields->Item[_variant_t("lev")]->Value;
			avas.aCharacter[l_wCount].m_wLevel = (WORD)v_varient.intVal;
			v_varient = pRs->Fields->Item[_variant_t("iWeapon")]->Value;
			avas.aCharacter[l_wCount].m_wWeapon = (WORD)v_varient.intVal;
			v_varient = pRs->Fields->Item[_variant_t("iArmor")]->Value;
			avas.aCharacter[l_wCount].m_wArmor = (WORD)v_varient.intVal;
			v_varient = pRs->Fields->Item[_variant_t("iShield")]->Value;
			avas.aCharacter[l_wCount].m_wShield = (WORD)v_varient.intVal;
			v_varient = pRs->Fields->Item[_variant_t("iLastField")]->Value;
			avas.aCharacter[l_wCount].m_wLastField = (WORD)v_varient.intVal;
			l_wCount++;
			if(l_wCount>=dMAX_AVATAR_COUNT)	break;
			pRs->MoveNext();
		}
		VariantClear(&v_varient);
		pCmd.Release(); 
		pRs->Close();	pRs.Release();	pRs = NULL;
	}
	catch( _com_error &e)
	{
		DMPcomERR(e)
	}
}

void	cADO_AVATAR::spGetAvaInfo(char * _strName, char * _strId, cPLAYER_DATA * pData,WORD & oper)
{
	DWORD	_dbIndex = g_Im.getIndex(_strName,_strId);

//	CCritical		cs(&m_csAC);
	try{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spAvatar_GETINFOAVATAR";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); 		return;	}

		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)_dbIndex);
		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)_strId);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strName);
		pCmd->Execute(NULL,NULL,NULL);

		_variant_t v_data;
		char	* uncompressdata = new char[dMAX_AVATAR_DATA_SIZE];
		memset(uncompressdata,0,dMAX_AVATAR_DATA_SIZE);
		v_data		= pCmd->GetParameters()->GetItem("@DATA")->Value;

		int unsize = VariantToBlob(v_data,uncompressdata);
		if(unsize>0 && unsize<sizeof(cPLAYER_DATA))
		{
			WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)pData,unsize,sizeof(cPLAYER_DATA));
			if(ww == 0)
			{
				// dump 파일에 저장해서 내용 확인해 보자. 왜 압축을 풀때 에러나는지. 데이터가 잘못 넘어온건지..
/*				char fn[512];
				sprintf(fn,"uncom_error2_%s_%s_%d.dump",_strId,_strName, timeGetTime());
				recoardDataDump(fn,uncompressdata,unsize);
*/			
				if	(unsize >= 2048 && unsize < sizeof(cPLAYER_DATA))
					memcpy(pData,uncompressdata,unsize);
				else
				{
					_log(" uncompress error 2  unsize : %d , %d",unsize,sizeof(cPLAYER_DATA));
					_log(" ERROR -_-;; [%s] [%s]",_strId,_strName);
				}
			}
		}else if (unsize == sizeof(cPLAYER_DATA))
		{
			memcpy(pData,uncompressdata,sizeof(cPLAYER_DATA));
		}else
		{
			_log(" DATA ERROR -spGetAvaInfo- [%s][%s]",_strId,_strName);
		}
		delete [] uncompressdata;
		VariantClear(&v_data);
		
		VARIANT v_return;
		v_return	= pCmd->GetParameters()->GetItem("@RESULT")->Value;
		if(_dbIndex==0x7fffffff && ((DWORD)v_return.intVal)!=0x7fffffff)
		{
			DWORD ret = g_Im.add((DWORD)v_return.intVal,_strName,_strId,pData->m_wAvatarIndex);
			_log("spGetAvaInfo reindex [%d] [%s] [%s] [%d], ret %d",(DWORD)v_return.intVal,_strName,_strId,pData->m_wAvatarIndex,ret);
		}
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
}

int cADO_AVATAR::LoadBankInfo(char * master, CBankInfo * bank)
{
//	CCritical		cs(&m_csAC);
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

		if	( l_iResult==2)
		{
			bank->reset();
			strcpy(bank->m_strId,master);
			bank->m_dwSaveCounter = 2;
		}
		else
		if	( l_iResult==1)
		{
			_variant_t v_data;
			char	* uncompressdata = new char[sizeof(CBankInfo)];
			memset(uncompressdata,0,sizeof(CBankInfo));

			v_data		= pCmd->GetParameters()->GetItem("@BANK")->Value;
			int unsize = VariantToBlob(v_data,uncompressdata);

			if	(unsize>0 && unsize<sizeof(CBankInfo))
			{
				WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)bank,unsize,sizeof(CBankInfo));
				if(ww == 0)	
				{
					_log(" uncompress error 3 %d %d",unsize,sizeof(CBankInfo));
					_log(" ERROR -_-;; [%s]",master);
				}
			}
			else
			if	(unsize == sizeof(CBankInfo))
			{
				memcpy(bank,uncompressdata,sizeof(CBankInfo));
			}
			else
			if	(unsize == 0)
			{
				bank->reset();
				strcpy(bank->m_strId,master);
				bank->m_dwSaveCounter = 2;
			}
			else
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

int	cADO_AVATAR::SaveBank(CBankInfo * bank)
{
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

WORD	cADO_AVATAR::spSaveAva(cPLAYER_DATA * pData,BOOL leveldown,int playtime)
{
	int idebugLastLine = __LINE__;
	if	(pData==NULL)
	{
		_log("ERROR spSaveAva : pData is NULL");

		return	eSAVE_DB_DATA_NULL;
	}

	DWORD	_dbIndex = g_Im.getIndex(pData->m_strName,pData->m_strId);
//	CCritical		cs(&m_csAC);
	try
	{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spAvatar_SAVEAVAINFO";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release();		return -2;	}

		cAVATAR_INFO	ainfo;
		pData->getAvatarData(&ainfo);

		DEF_PARAM(pCmd,"@VER") = _variant_t((long)8);
		DEF_PARAM(pCmd,"@INDEX") = _variant_t((long)_dbIndex);
		DEF_PARAM(pCmd,"@LEVELDOWN") = _variant_t((long)leveldown);
		DEF_PARAM(pCmd,"@MASTER") = _variant_t((char *)pData->m_strId);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)pData->m_strName);
		
		if	(pData->m_wRedStonePoint > 60000)
			pData->m_wRedStonePoint = 0;

		VARIANT bin;char	compressData[dMAX_AVATAR_DATA_SIZE];memset(compressData,0,dMAX_AVATAR_DATA_SIZE);
		WORD sz = cSRVUTIL::Compress((unsigned char *)pData,(unsigned char *)compressData,sizeof(cPLAYER_DATA));

		if	(sz>0 && sz<sizeof(cPLAYER_DATA))
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
		DEF_PARAM(pCmd,"@EXP") = _variant_t((long)pData->m_wRedStonePoint);
		DEF_PARAM(pCmd,"@PLAYTIME") = _variant_t((long)pData->m_dwPlayTime);
		DEF_PARAM(pCmd,"@SAVECOUNT") = _variant_t((long)pData->m_dwSaveCount);
		DEF_PARAM(pCmd,"@WEAPON") = _variant_t((long)ainfo.m_wWeapon);
		DEF_PARAM(pCmd,"@ARMOR") = _variant_t((long)ainfo.m_wArmor);
		DEF_PARAM(pCmd,"@SHIELD") = _variant_t((long)ainfo.m_wShield);
		DEF_PARAM(pCmd,"@LASTFIELD") = _variant_t((long)pData->m_wCurrentField);
		DEF_PARAM(pCmd,"@REBIRTHCOUNT") = _variant_t((long)pData->m_bf4RebirthCount);
		pCmd->Execute(NULL,NULL,NULL);

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		WORD	l_wResult = (WORD)regIdx.intVal;
		VariantClear(&regIdx);
		iErrorCount = 0;

		pCmd.Release();
		return l_wResult;
	}
	catch( _com_error &e)
	{
		DMPcomERR(e);
	}
	
	return	eSAVE_DB_UNKNOWN_ERROR;
}

int cADO_AVATAR::spPD_GetData(int _iIdx,char * _strId, char * _strName, void * _binData)
{
	int iResult = 0;
	int idebugLastLine = __LINE__;
//	CCritical		cs(&m_csAC);
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
			delete [] uncompressdata;
			VariantClear(&v_data);
		}
		pCmd.Release();
		return iResult;
	}
	catch( _com_error &e)
	{
		DMPcomERR(e);
	}
	return	-1;
}

int cADO_AVATAR::spPD_SetData(int _iIdx,char * _strId, char * _strName, void * _binData)
{
	int iResult = 0;
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

		DEF_PARAM(pCmd,"@IDX") = _variant_t((long)_iIdx);
		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)_strId);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char *)_strName);
		
		VARIANT bin;char	compressData[2048];memset(compressData,0,2048);
		WORD sz = cSRVUTIL::Compress((unsigned char *)_binData,(unsigned char *)compressData,sizeof(cUserCustomDataDefine));
		if(sz>0 && sz<sizeof(cUserCustomDataDefine))
			BlobToVariant(bin,compressData,sz);
		else
		{
//			_log(" compress fail : sz size =%d , %d ,%d",sz,sizeof(cUserCustomDataDefine),__LINE__);
			BlobToVariant(bin,_binData,sizeof(cUserCustomDataDefine));
		}
		pCmd->Parameters->Item[_variant_t("@BINDATA")]->AppendChunk(bin);
		VariantClear(&bin);

		pCmd->Execute(NULL,NULL,NULL);

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		iResult = (int)regIdx.intVal;
		VariantClear(&regIdx);

		pCmd.Release();
		return iResult;
	}
	catch( _com_error &e)
	{
		DMPcomERR(e);
	}
	return	0;
}

//////////////////////////////////////////////////////////////////////////
//	login/out
int		cADO_AVATAR::spLogin_Reset()
{
	try
	{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spLogin_Reset";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release();		return -2;	}
		
		DEF_PARAM(pCmd,"@rev") = _variant_t((long)0);
	
		pCmd->Execute(NULL,NULL,NULL);
	
		pCmd.Release();
		return 1;
	}
	catch( _com_error &e)
	{
		DMPcomERR(e);
	}
	return	0;
}

int		cADO_AVATAR::spLogin_In(char* _strId, char* _strName, const int& _externalType)
{
	try
	{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spLogin_In";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release();		return -2;	}
		
		DEF_PARAM(pCmd,"@ID") = _variant_t((char*)_strId);
		DEF_PARAM(pCmd,"@NAME") = _variant_t((char*)_strName);
		DEF_PARAM(pCmd,"@ETYPE") = _variant_t((long)_externalType);
		
		pCmd->Execute(NULL,NULL,NULL);
		
		pCmd.Release();
		return 1;
	}
	catch( _com_error &e)
	{
		DMPcomERR(e);
	}
	return	0;
}

int		cADO_AVATAR::spLogin_Out(char* _strId)
{
	try
	{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spLogin_Out";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release();		return -2;	}
		
		DEF_PARAM(pCmd,"@ID") = _variant_t((char*)_strId);

		pCmd->Execute(NULL,NULL,NULL);
		
		pCmd.Release();
		return 1;
	}
	catch( _com_error &e)
	{
		DMPcomERR(e);
	}
	return	0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////
//		PUBLIC METHOD
/////////////////////////////////////////////////////////////////////////////////////////////////

WORD	cADO_AVATAR::DeleteAva(char *master,char *name, WORD & _wGuild)	//	int idx
{
	return spDeleteAva(master , name, _wGuild);
}

int		cADO_AVATAR::SaveAva(cPLAYER_DATA * pData,int playtime,BOOL leveldown)
{
	pData->m_bf1IsLevelDown	=	FALSE;

	return spSaveAva(pData,leveldown,playtime);
}