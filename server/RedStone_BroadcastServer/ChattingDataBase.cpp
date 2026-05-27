// ChattingDataBase.cpp: implementation of the CChattingDataBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RedStone_BroadCastServer.h"
#include "RedStone_BroadCastServerDlg.h"
#include "ChattingDataBase.h"
#include "Client.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChattingDataBase::CChattingDataBase()
{
//	InitializeCriticalSection(&m_csList);
}

CChattingDataBase::~CChattingDataBase()
{
//	DeleteCriticalSection(&m_csList);
}

void
CChattingDataBase::addData(CChattingLog * _data)
{
//	CCritical	cs(&m_csList);
//	m_stdList.push_back(*_data);
//	incTotalCount();
}

int
CChattingDataBase::sendData()
{
/*	EnterCriticalSection(&m_csList);

	CChattingLog _data;

	if(m_stdList.empty())
	{
		LeaveCriticalSection(&m_csList);
		return 0;
	}
	memcpy(&_data,&(m_stdList.at(0)),sizeof(CChattingLog));
	m_stdList.pop_front();
	LeaveCriticalSection(&m_csList);

	//	DB작업 할 것!!
	try
	{
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"spChatLog_Add";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release();		return -2;	}

		DEF_PARAM(pCmd,"@SERVERNAME") = _variant_t((char *)CRedStone_BroadCastServerDlg::getWorldName());
		char strDate[64];
		cSRVUTIL::GetMSSQLDBDateNTime(strDate,(cpUTime *)&_data.m_stTime);
		DEF_PARAM(pCmd,"@TIME") = _variant_t((char *)strDate);
		DEF_PARAM(pCmd,"@FIELDINDEX") = _variant_t((long)_data.m_wFieldIndex);
		DEF_PARAM(pCmd,"@CHATTINGTYPE") = _variant_t((long)_data.m_wChattingType);
		DEF_PARAM(pCmd,"@SENDERID") = _variant_t((char *)_data.m_stSender.m_strId);
		DEF_PARAM(pCmd,"@SENDERNAME") = _variant_t((char *)_data.m_stSender.m_strName);
		DEF_PARAM(pCmd,"@RECEIVERID") = _variant_t((char *)_data.m_stReceiver.m_strId);
		DEF_PARAM(pCmd,"@RECEIVERNAME") = _variant_t((char *)_data.m_stReceiver.m_strName);
		DEF_PARAM(pCmd,"@MESSAGE") = _variant_t((char *)_data.m_strMessage);
		pCmd->Execute(NULL,NULL,NULL);

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		WORD	l_wResult = (WORD)regIdx.intVal;
		VariantClear(&regIdx);

		pCmd.Release();
		return l_wResult;
	}
	catch( _com_error &e)
	{
		DMPcomERR(e);
	}
*/
	return 0;
}

BOOL
CChattingDataBase::add(void *_lpSender,void *_lpReceiver,int _iChatType,char *_lpstrMessage)
{
/*	if	(!_lpSender)
		return	FALSE;

	CClient	*lpSender	=	(CClient	*)_lpSender;
	CClient	*lpReceiver	=	(CClient	*)_lpReceiver;

	CChattingLog	log;
	CTimeInfo		curTime;

	curTime.update();

	log.m_stTime.year		=	curTime.m_wYear-2000;	//	연
	log.m_stTime.month		=	curTime.m_wMonth;	//	월
	log.m_stTime.day		=	curTime.m_wDay;	//	일
	log.m_stTime.hour		=	curTime.m_wHour;	//	시
	log.m_stTime.minute		=	curTime.m_wMinute;	//	분
	log.m_stTime.second		=	curTime.m_wSecond;	//	초
	log.m_wFieldIndex		=	lpSender->m_wCurrentField;
	log.m_wChattingType		=	_iChatType;

	strcpy(log.m_strMessage,_lpstrMessage);
	strcpy(log.m_stSender.m_strId,lpSender->m_strId);
	strcpy(log.m_stSender.m_strName,lpSender->m_strName);

	if	(lpReceiver)
	{
		strcpy(log.m_stReceiver.m_strId,lpReceiver->m_strId);
		strcpy(log.m_stReceiver.m_strName,lpReceiver->m_strName);
	}

	addData(&log);
*/
	return	TRUE;
}