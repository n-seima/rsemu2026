// AdminCommand.cpp: implementation of the CAdminCommand class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "admintool.h"
#include "AdminCommand.h"
#include "cSRVUTIL.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define dLIST_MAXCOUNT		100

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAdminCommand::CAdminCommand()
{
	isCanConnect = FALSE;
	memset(commanddbip,0,dIP_SIZE);
	memset(commanddbname,0,64);
#ifdef _FOR_CHINA
	strcpy(commandID,"redstone");
	strcpy(commandPW,"redstone");
	commandPort = 50505;
#elif _FOR_JAPAN
	strcpy(commandID,"redstone");
	strcpy(commandPW,"qnfrdmsqhtjr");
	commandPort = 50605;
#elif _FOR_USA
	strcpy(commandID,"redstone");
	strcpy(commandPW,"usatest");
	commandPort = 50605;
#elif _FOR_INTER
	strcpy(commandID,"redstone");
	strcpy(commandPW,"usatest");
	commandPort = 40605;
#elif _FOR_KOREA
	strcpy(commandID,"RedMaster");
	strcpy(commandPW,"CJFVKSRNDL");
	commandPort = 5316;
#endif

	cAdCommandList = new cAdCommandData[dLIST_MAXCOUNT];
	memset(cAdCommandList,0,sizeof(cAdCommandData)*dLIST_MAXCOUNT);
	commandCount = 0;
}

CAdminCommand::~CAdminCommand()
{
	delete [] cAdCommandList;
	cAdCommandList = NULL;
}

BOOL	CAdminCommand::init(char * ip,char * dbname)
{
	strcpy(commanddbip,ip);
	strcpy(commanddbname,dbname);

	if(strcmp(commanddbip,"")==0 || strcmp(commanddbname,"")==0)
	{
		MessageBox(GetActiveWindow()," ","Incorrect Command DB Info !!",MB_OK);
		return FALSE;
	}
	isCanConnect = TRUE;
	memset(cAdCommandList,0,sizeof(cAdCommandData)*dLIST_MAXCOUNT);
	commandCount = 0;
	return TRUE;
}

BOOL	CAdminCommand::connectCommandDB()
{
	return this->Connect(commanddbip,commandID,commandPW,commanddbname,1);
}

BOOL	CAdminCommand::disCommandDB()
{
	this->Close();
	return TRUE;
}

int		CAdminCommand::DeleteCommand(int idx,char * whod)
{
	try
	{	_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command °´¼ ¿
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = this->GetConnection();
		pCmd->CommandText = L"spCL_DEL";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@IDX") = _variant_t((long)idx);
		DEF_PARAM(pCmd,"@WHOD") = _variant_t((char *)whod);

		hr = pCmd->Execute(NULL,NULL,NULL);

		if(FAILED(hr)){	return 0;}
		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.lVal = pCmd->GetParameters()->GetItem("@RET")->Value;
		int iResult = (int)regIdx.lVal;
		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
		return iResult;
	}catch( _com_error &e)
	{	DMPcomERR(e)
		return 0;
	}
	return 0;
}

int		CAdminCommand::ReserveCommand(UTime _regTime,UTime _finTime,int comType, int comTarget, int param1, int param2,char * param3,char * msg, char * who)
{
	try
	{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		char	strDate[32];
		//Command °´¼ ¿
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = this->GetConnection();
		pCmd->CommandText = L"spCL_ADD";
		pCmd->CommandType  = adCmdStoredProc;

		memset(strDate,0,32);
		if(_regTime.m_dwValue==1)
			cSRVUTIL::GetMSSQLDBDateNTime(strDate);
		else
			UTimeToDatetime(&_regTime,strDate);
		DEF_PARAM(pCmd,"@RELTIME") = _variant_t((char *)strDate);
		memset(strDate,0,32);
		UTimeToDatetime(&_finTime,strDate);
		DEF_PARAM(pCmd,"@FINTIME") = _variant_t((char *)strDate);
		DEF_PARAM(pCmd,"@COMMANDTYPE") = _variant_t((long)comType);
		DEF_PARAM(pCmd,"@COMMANDTARGET") = _variant_t((long)comTarget);
		DEF_PARAM(pCmd,"@PARAM1") = _variant_t((long)param1);
		DEF_PARAM(pCmd,"@PARAM2") = _variant_t((long)param2);
		DEF_PARAM(pCmd,"@PARAM3") = _variant_t((char *)param3);
		DEF_PARAM(pCmd,"@MSG") = _variant_t((char *)msg);
		DEF_PARAM(pCmd,"@WHO") = _variant_t((char *)who);

		hr = pCmd->Execute(NULL,NULL,NULL);

		if(FAILED(hr)){	return 0;}
		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.lVal = pCmd->GetParameters()->GetItem("@RET")->Value;
		int iResult = (int)regIdx.lVal;
		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
		return iResult;

	}catch( _com_error &e)
	{
		DMPcomERR(e)
		return 0;
	}

	return 0;
}

int		CAdminCommand::GetCommandList(int _isBackUpList)
{
	DWORD			i;
	HRESULT			hr = CoInitialize(0);

	memset(cAdCommandList,0,sizeof(cAdCommandData)*dLIST_MAXCOUNT);
	commandCount = 0;

	try
	{
		_RecordsetPtr	pRs = NULL;
		pRs.CreateInstance(__uuidof(Recordset));
		pRs->CursorLocation = adUseClient;
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spCL_GETLIST";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); pRs.Release();	pRs = NULL;	return 0;	}

		_variant_t	v_varient;

		DEF_PARAM(pCmd,"@ISBACKUPLIST") = _variant_t((long)_isBackUpList);
		pRs = pCmd->Execute(NULL,NULL, NULL);

		char	strDate[32];
		i = 0;
		
		while(!pRs->adoEOF)
		{
			v_varient = pRs->Fields->Item[_variant_t("idx")]->Value;
			cAdCommandList[i].idx = (int)v_varient.intVal;

			v_varient = pRs->Fields->Item[_variant_t("releasetime")]->Value;
			strcpy(strDate,(_bstr_t)v_varient);
			getTimeToUTime(strDate,&cAdCommandList[i].releasetime);
			v_varient = pRs->Fields->Item[_variant_t("finishtime")]->Value;
			strcpy(strDate,(_bstr_t)v_varient);
			getTimeToUTime(strDate,&cAdCommandList[i].finishtime);
			v_varient = pRs->Fields->Item[_variant_t("commandtype")]->Value;
			cAdCommandList[i].commandtype = (int)v_varient.intVal;
			v_varient = pRs->Fields->Item[_variant_t("commandtarget")]->Value;
			cAdCommandList[i].commandtarget = (int)v_varient.intVal;
			v_varient = pRs->Fields->Item[_variant_t("param1")]->Value;
			cAdCommandList[i].param1 = (int)v_varient.intVal;
			v_varient = pRs->Fields->Item[_variant_t("param2")]->Value;
			cAdCommandList[i].param2 = (int)v_varient.intVal;
			v_varient = pRs->Fields->Item[_variant_t("param3")]->Value;
			strcpy(cAdCommandList[i].param3,(_bstr_t)v_varient);
			v_varient = pRs->Fields->Item[_variant_t("msg")]->Value;
			strcpy(cAdCommandList[i].msg,(_bstr_t)v_varient);
			v_varient = pRs->Fields->Item[_variant_t("who")]->Value;
			strcpy(cAdCommandList[i].who,(_bstr_t)v_varient);
			v_varient = pRs->Fields->Item[_variant_t("registedtime")]->Value;
			strcpy(strDate,(_bstr_t)v_varient);
			getTimeToUTime(strDate,&cAdCommandList[i].registedtime);

			if(_isBackUpList)
			{
				v_varient = pRs->Fields->Item[_variant_t("whod")]->Value;
				strcpy(cAdCommandList[i].whod,(_bstr_t)v_varient);
				v_varient = pRs->Fields->Item[_variant_t("deltime")]->Value;
				strcpy(strDate,(_bstr_t)v_varient);
				getTimeToUTime(strDate,&cAdCommandList[i].deltime);
				v_varient = pRs->Fields->Item[_variant_t("deltype")]->Value;
				cAdCommandList[i].deltype = (int)v_varient.intVal;
			}
			i++;
			pRs->MoveNext();
		}
		VariantClear(&v_varient);

		commandCount = i;	//	¾¾¿ °¹¼

		pCmd.Release();		pCmd = NULL;
		pRs->Close();	pRs.Release();	pRs = NULL;
		return 1;
	}catch(_com_error &e)
	{
		DMPcomERR(e)
		return 0;
	}
	return 0;
}

int		CAdminCommand::ModifyCommand(int idx,UTime _regTime,UTime _finTime,int comType, int comTarget, int param1, int param2,char * param3,char * msg, char * who)
{
	try
	{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		char	strDate[32];
		//Command °´¼ ¿
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = this->GetConnection();
		pCmd->CommandText = L"spCL_MOD";
		pCmd->CommandType  = adCmdStoredProc;

		DEF_PARAM(pCmd,"@IDX") = _variant_t((long)idx);
		memset(strDate,0,32);
		UTimeToDatetime(&_regTime,strDate);
		DEF_PARAM(pCmd,"@RELTIME") = _variant_t((char *)strDate);
		memset(strDate,0,32);
		UTimeToDatetime(&_finTime,strDate);
		DEF_PARAM(pCmd,"@FINTIME") = _variant_t((char *)strDate);
		DEF_PARAM(pCmd,"@COMMANDTYPE") = _variant_t((long)comType);
		DEF_PARAM(pCmd,"@COMMANDTARGET") = _variant_t((long)comTarget);
		DEF_PARAM(pCmd,"@PARAM1") = _variant_t((long)param1);
		DEF_PARAM(pCmd,"@PARAM2") = _variant_t((long)param2);
		DEF_PARAM(pCmd,"@PARAM3") = _variant_t((char *)param3);
		DEF_PARAM(pCmd,"@MSG") = _variant_t((char *)msg);
		DEF_PARAM(pCmd,"@WHO") = _variant_t((char *)who);

		hr = pCmd->Execute(NULL,NULL,NULL);

		if(FAILED(hr)){	return 0;}
		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.lVal = pCmd->GetParameters()->GetItem("@RET")->Value;
		int iResult = (int)regIdx.lVal;
		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
		return iResult;

	}catch( _com_error &e)
	{
		DMPcomERR(e)
		return 0;
	}

	return 0;
}
