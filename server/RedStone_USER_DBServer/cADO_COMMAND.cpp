// cADO_COMMAND.cpp: implementation of the cADO_COMMAND class.
//
//////////////////////////////////////////////////////////////////////

#include "cADO_COMMAND.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cADO_COMMAND::cADO_COMMAND()
{

}

cADO_COMMAND::~cADO_COMMAND()
{

}

int
cADO_COMMAND::GetCommand(cAdCommandData * _command)
{
	try
	{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		char	strDate[32];
		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = this->GetConnection();
		pCmd->CommandText = L"spCL_GETCOMMAND_FORSERVER";
		pCmd->CommandType  = adCmdStoredProc;
		DEF_PARAM(pCmd,"@RESERVE") = _variant_t((long)0);
		hr = pCmd->Execute(NULL,NULL,NULL);

		if(FAILED(hr)){	return 0;}
		VARIANT regIdx;
		VariantInit(&regIdx);

		regIdx.lVal = pCmd->GetParameters()->GetItem("@RET")->Value;
		int iResult = (int)regIdx.lVal;

		if(iResult)
		{
			memset(strDate,0,32);
			regIdx = pCmd->GetParameters()->GetItem("@RELTIME")->Value;
			strcpy(strDate,(_bstr_t)regIdx);
			getTimeToUTime(strDate,&_command->releasetime);

			memset(strDate,0,32);
			regIdx = pCmd->GetParameters()->GetItem("@FINTIME")->Value;
			strcpy(strDate,(_bstr_t)regIdx);
			getTimeToUTime(strDate,&_command->finishtime);

			regIdx.lVal = pCmd->GetParameters()->GetItem("@COMMANDTYPE")->Value;
			_command->commandtype = (int)regIdx.lVal;
			regIdx.lVal = pCmd->GetParameters()->GetItem("@COMMANDTARGET")->Value;
			_command->commandtarget = (int)regIdx.lVal;
			regIdx.lVal = pCmd->GetParameters()->GetItem("@PARAM1")->Value;
			_command->param1 = (int)regIdx.lVal;
			regIdx.lVal = pCmd->GetParameters()->GetItem("@PARAM2")->Value;
			_command->param2 = (int)regIdx.lVal;
			regIdx = pCmd->GetParameters()->GetItem("@PARAM3")->Value;
			strcpy(_command->param3,(_bstr_t)regIdx);
			regIdx = pCmd->GetParameters()->GetItem("@MSG")->Value;
			strcpy(_command->msg,(_bstr_t)regIdx);
		}
		VariantClear(&regIdx);
		pCmd.Release();		pCmd.Detach();		pCmd = NULL;
		return iResult;

	}catch( _com_error &e)
	{
		DMPcomERR(e)
	}
	return 0;
}

int
#if defined(_FOR_KOREA) || defined(_FOR_THAI)
cADO_COMMAND::UpdateServerInfo(SERVERINFO * _info, DWORD * _usercountList)
#else
cADO_COMMAND::UpdateServerInfo(SERVERINFO * _info)
#endif
{

	try{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spSI_UPDATE_SERVERINFO";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); 	return 0;	}

		DEF_PARAM(pCmd,"@IDX") = _variant_t((long)_info->idx);
		DEF_PARAM(pCmd,"@WORLDIDX") = _variant_t((long)_info->worldidx);
		DEF_PARAM(pCmd,"@STYPE") = _variant_t((long)_info->servertype);
		DEF_PARAM(pCmd,"@CURUSERCOUNT") = _variant_t((long)_info->curusercount);
		DEF_PARAM(pCmd,"@MAXUSERCOUNTTODAY") = _variant_t((long)_info->maxusercounttoday);

#if defined(_FOR_KOREA) || defined(_FOR_THAI)
		if(_info->servertype==0)	//	world일때만..
		{
			DEF_PARAM(pCmd,"@CURUSERCOUNT1") = _variant_t((long)_usercountList[0]);
			DEF_PARAM(pCmd,"@CURUSERCOUNT2") = _variant_t((long)_usercountList[1]);
			DEF_PARAM(pCmd,"@CURUSERCOUNT3") = _variant_t((long)_usercountList[2]);
			DEF_PARAM(pCmd,"@CURUSERCOUNT4") = _variant_t((long)_usercountList[3]);
			DEF_PARAM(pCmd,"@CURUSERCOUNT5") = _variant_t((long)_usercountList[4]);
		}else
		{
			DEF_PARAM(pCmd,"@CURUSERCOUNT1") = _variant_t((long)0);
			DEF_PARAM(pCmd,"@CURUSERCOUNT2") = _variant_t((long)0);
			DEF_PARAM(pCmd,"@CURUSERCOUNT3") = _variant_t((long)0);
			DEF_PARAM(pCmd,"@CURUSERCOUNT4") = _variant_t((long)0);
			DEF_PARAM(pCmd,"@CURUSERCOUNT5") = _variant_t((long)0);
		}
#endif
		DEF_PARAM(pCmd,"@IP") = _variant_t((char *)_info->ip);

		pCmd->Execute(NULL,NULL,NULL);
		pCmd.Release();
		return 1 ;
	}
	catch( _com_error &e)
	{
		DMPcomERR(e)
		return 0;
	}
}

int	cADO_COMMAND::logInOut(WU_LOG_INOUT* _data)
{
	try{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spLOG_InOut";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); 	return 0;	}

		DEF_PARAM(pCmd,"@isIn") = _variant_t((long)_data->isIn);
		DEF_PARAM(pCmd,"@world") = _variant_t((char *)_data->worldName);
		DEF_PARAM(pCmd,"@account") = _variant_t((char *)_data->strID);
		DEF_PARAM(pCmd,"@name") = _variant_t((char *)_data->strName);
		DEF_PARAM(pCmd,"@lev") = _variant_t((long)_data->lev);
		DEF_PARAM(pCmd,"@job") = _variant_t((long)_data->job);

		pCmd->Execute(NULL,NULL,NULL);
		pCmd.Release();
		return 1 ;
	}
	catch( _com_error &e)
	{
		DMPcomERR(e)
			return 0;
	}
}
int		cADO_COMMAND::logQuest(WU_LOG_QUEST* _data)
{
	try{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		
		if(_data->isStartQuest == 0)
			pCmd->CommandText = L"spLOG_QuestComplete";
		else 
		if(_data->isStartQuest == 1)
			pCmd->CommandText = L"spLOG_QuestStart";
		else 
		if(_data->isStartQuest == 2)
			pCmd->CommandText = L"spLOG_QuestCancel";

		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); 	return 0;	}

		DEF_PARAM(pCmd,"@questIdx") = _variant_t((long)_data->questIdx);
		DEF_PARAM(pCmd,"@world") = _variant_t((char *)_data->worldName);
		DEF_PARAM(pCmd,"@account") = _variant_t((char *)_data->strID);
		DEF_PARAM(pCmd,"@name") = _variant_t((char *)_data->strName);
		DEF_PARAM(pCmd,"@lev") = _variant_t((long)_data->lev);
		DEF_PARAM(pCmd,"@job") = _variant_t((long)_data->job);

		pCmd->Execute(NULL,NULL,NULL);
		pCmd.Release();
		return 1 ;
	}
	catch( _com_error &e)
	{
		DMPcomERR(e)
			return 0;
	}
}