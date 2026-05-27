// cADO_WEB.cpp: implementation of the cADO_WEB class.
//
//////////////////////////////////////////////////////////////////////

#include "cADO_WEB.h"
#include "cSRVUTIL.h"
#include "Client.h"
#ifdef _FOR_USA
#include "../OGP_Lib/ogpapi.h"
#include "../tinyxml2.6.1/tinyxml.h"
#endif
#include <TChar.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cADO_WEB::cADO_WEB()
{
}

cADO_WEB::~cADO_WEB()
{
}

void	cADO_WEB::GetBuyList(char * _strId,DGMSG_BUYLIST * list)
{
//	CCritical		cs(&m_csAC);
	memset(list->List,0xff,sizeof(tsBuyLst)*dMAX_BUYLIST);
	list->wCount = 0;
	try{
		_RecordsetPtr	pRs = NULL;
		pRs.CreateInstance(__uuidof(Recordset));
		pRs->CursorLocation = adUseClient;

		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
#ifdef _FOR_USA	//	OGP
		pCmd->CommandText = L"spPItem_GetList";
#else
		pCmd->CommandText = L"sp_red_ItemList";
#endif
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL)	return;

#ifdef _FOR_USA	//	OGP
		DEF_PARAM(pCmd,"@id") = _variant_t((char *)_strId);
#else
		DEF_PARAM(pCmd,"@userid") = _variant_t((char *)_strId);
#endif
 		pRs = pCmd->Execute(NULL,NULL,NULL);
		_variant_t	v_varient;
		while(!pRs->adoEOF)
		{
			char		strDate[64];
#ifdef _FOR_USA	//	OGP
			v_varient							= pRs->Fields->Item[_variant_t("uid")]->Value;
			list->List[list->wCount].dwSqn		= (DWORD)v_varient.intVal;
			
			v_varient							= pRs->Fields->Item[_variant_t("itemno")]->Value;
			list->List[list->wCount].dwItemNo	= (int)v_varient.intVal;

			v_varient							= pRs->Fields->Item[_variant_t("end_date")]->Value;
			strcpy(strDate,(_bstr_t)v_varient);
			getTimeToUTime(strDate,&list->List[list->wCount].timeEndDate);

#else
			v_varient							= pRs->Fields->Item[_variant_t("itemno")]->Value;
			list->List[list->wCount].dwItemNo	= (int)v_varient.intVal;
			
			v_varient							= pRs->Fields->Item[_variant_t("sqn")]->Value;
			list->List[list->wCount].dwSqn		= (DWORD)v_varient.intVal;
			
			v_varient							= pRs->Fields->Item[_variant_t("enddate")]->Value;

			strcpy(strDate,(_bstr_t)v_varient);
			getTimeToUTime(strDate,&list->List[list->wCount].timeEndDate);
#endif
			list->wCount++;
			if	(list->wCount>=dMAX_BUYLIST)	break;
			pRs->MoveNext();
		}
		VariantClear(&v_varient);
		iErrorCount = 0;
		pCmd.Release();
		pRs->Close();
		return ;
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return ;
	}
}

//	?? 위의 것과 차이가 뭐지? - 사용하는데가 없네 -_-;
void	cADO_WEB::GetInfoOfItemList(DGMSG_PREMIUM_ITEM_PRICE_INFO * list)
{
	list->wCount = 0;
	memset(list->awPriceList,0,sizeof(WORD)*dMAX_PREMIUM_ITEM_COUNT);

	try{
		_RecordsetPtr	pRs = NULL;
		pRs.CreateInstance(__uuidof(Recordset));
		pRs->CursorLocation = adUseClient;
		_CommandPtr		pCmd = NULL;
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"sp_red_getiteminfo";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL){	pCmd.Release(); pRs.Release();	pRs = NULL;	return;	}
		pRs = pCmd->Execute(NULL,NULL,NULL);

		WORD index;
		_variant_t v_varient;
		while(!pRs->adoEOF)
		{
			v_varient = pRs->Fields->Item[_variant_t("itemno")]->Value;
			index = (WORD)v_varient.intVal;
			if(index>=dMAX_PREMIUM_ITEM_COUNT)
			{
				_log(" Getting Purchase Item List Error : item index -> %d",index);
				break;
			}
			v_varient = pRs->Fields->Item[_variant_t("itemprice")]->Value;
			list->awPriceList[index] = (WORD)v_varient.intVal;
			_log(" Premium Item List : %d , [%d]",index,list->awPriceList[index]);
			if(index+1 > list->wCount)	list->wCount = index+1;
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

//	유저가 구매한 목록을 가져온다.
WORD	cADO_WEB::UseBuy(int _iSqn,char * _strId,char * _strServerName, char * _strAvaName, UTime * time,int _lev,WORD _job)
{
//	CCritical		cs(&m_csAC);
	try{
		char	strDate[64];
		if (time->m_dwValue)
				sprintf(strDate,"%d-%d-%d %d:%d:%d",time->year+2000,time->month,time->day,time->hour,time->minute,time->second);
		else	sprintf(strDate,"1900-01-01 00:00:00");

		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
#ifdef _FOR_USA	//	OGP
		pCmd->CommandText = L"spPItem_TakeItem";
#else
		pCmd->CommandText = L"sp_red_ItemDraw";
#endif
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL)	return dRESULT_USEBUYITEM_FAIL;

#ifdef _FOR_USA	//	OGP
		DEF_PARAM(pCmd,"@uid") = _variant_t((long)_iSqn);
		DEF_PARAM(pCmd,"@id") = _variant_t((char *)_strId);
		DEF_PARAM(pCmd,"@server") = _variant_t((char *)_strServerName);
		DEF_PARAM(pCmd,"@charname") = _variant_t((char *)_strAvaName);
		DEF_PARAM(pCmd,"@enddate") = _variant_t((char *)strDate);
#else
		DEF_PARAM(pCmd,"@sqn") = _variant_t((long)_iSqn);
		DEF_PARAM(pCmd,"@userid") = _variant_t((char *)_strId);
		DEF_PARAM(pCmd,"@usageserver") = _variant_t((char *)_strServerName);
		DEF_PARAM(pCmd,"@usagecharname") = _variant_t((char *)_strAvaName);
		DEF_PARAM(pCmd,"@enddate") = _variant_t((char *)strDate);
		DEF_PARAM(pCmd,"@usagelev") = _variant_t((long)_lev);
		DEF_PARAM(pCmd,"@usagejob") = _variant_t((long)_job);
#endif
 		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))
		{
			printf("ERROR sp_red_ItemDraw \n");
			return dRESULT_USEBUYITEM_FAIL;
		}
		VARIANT regIdx;
#ifdef _FOR_USA	//	OGP
		regIdx = pCmd->GetParameters()->GetItem("@ret")->Value;
#else
		regIdx = pCmd->GetParameters()->GetItem("@result")->Value;
#endif
		int l_iResult = (int)regIdx.intVal;
		VariantClear(&regIdx);
		pCmd.Release();
		printf("ItemDraw [%d] result -> %d\n",_iSqn,(int)regIdx.intVal);

#ifdef _FOR_USA	//	OGP
		if(l_iResult==1)	return dRESULT_USEBUYITEM_NOTFIND;
		else				return dRESULT_USEBUYITEM_SUCCESS;
#else
		if(l_iResult==-1)	return dRESULT_USEBUYITEM_NOTFIND;
		else				return dRESULT_USEBUYITEM_SUCCESS;
#endif
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return dRESULT_USEBUYITEM_FAIL;
	}
}


void
cADO_WEB::insertPremiumItemToWebDB(G2D_BUY_PREMIUM_ITEM_TO_WEB_DB * _packet)
{
	try
	{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"sp_red_ItemBuyAd_Renewal";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL)	return ;
		
 		DEF_PARAM(pCmd,"@userid") = _variant_t((char *)_packet->strId);
 		DEF_PARAM(pCmd,"@itemno") = _variant_t((long)_packet->iItemIndex);
		DEF_PARAM(pCmd,"@itemprice") = _variant_t((long)_packet->iPrice);
		DEF_PARAM(pCmd,"@itemCnt") = _variant_t((long)_packet->iItemCount);
		DEF_PARAM(pCmd,"@giftid") = _variant_t((char *)_packet->strReceiverName);
		DEF_PARAM(pCmd,"@igLevel") = _variant_t((long)_packet->iLev);
		DEF_PARAM(pCmd,"@igChr") = _variant_t((char *)_packet->strName);
		
		hr = pCmd->Execute(NULL,NULL,NULL);
		if(FAILED(hr))
		{
			printf("ERROR sp_red_ItemBuyAd_Renewal \n");
			return ;
		}

		VARIANT regIdx;
		regIdx = pCmd->GetParameters()->GetItem("@rv")->Value;
		int l_iResult = (int)regIdx.intVal;

		regIdx = pCmd->GetParameters()->GetItem("@item")->Value;
		char itemName[256];
		strcpy( itemName, (char *)regIdx.bstrVal);

		VariantClear(&regIdx);
		pCmd.Release();
	}
	catch( _com_error &e){
		DMPcomERR(e)
		return;
	}
}

void	__cdecl
cADO_WEB::SendInfo(char * _id,WORD _type,char * _sname,char * _txt,...)
{
//	CCritical		cs(&m_csAC);
	TCHAR buffer[256];
	va_list args;

	va_start( args, _txt );
	_vstprintf( buffer, _txt, args );
	va_end( args );

	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"LogGame_INPUT";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL)	return ;

		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)_id);
		DEF_PARAM(pCmd,"@TYPE") = _variant_t((long)_type);
		DEF_PARAM(pCmd,"@SNAME") = _variant_t((char *)_sname);
		DEF_PARAM(pCmd,"@TXT") = _variant_t((char *)buffer);
		hr = pCmd->Execute(NULL,NULL,NULL);

		pCmd.Release();
		if(FAILED(hr)){		_log("ERROR : link Web Log Save Fail- Execute !! \n");		}
	}
	catch( _com_error &e){
		DMPcomERR(e)
	}
	return ;
}


/////////////////////////////////////////////////
//	Carrot
cADO_CARROT::cADO_CARROT()
{
}

cADO_CARROT::~cADO_CARROT()
{
}

int
cADO_CARROT::GetCarrot_OGP(char * _strId, char * _strIp, char * _strToken)
{
#ifdef _OGP_SERVICE
	char query[1024];
	char ret[2048];
	memset(ret,0,2048);

	if	(g_bIsMainTestServer)
		sprintf(query,"http://api.ogplanet.com/getUserAstro.og?ServiceID=%d&ServiceKey=%s&Token=%s&UserID=%s&UserIP=%s",
					dOGP_GAME_SERVICE_ID, dOGP_GAME_SERVICE_KEY, _strToken, _strId, _strIp
					);
	else
		sprintf(query,"https://billing.ogplanet.com/api/getUserAstro.og?ServiceID=%d&ServiceKey=%s&Token=%s&UserID=%s&UserIP=%s",
					dOGP_GAME_SERVICE_ID, dOGP_GAME_SERVICE_KEY, _strToken, _strId, _strIp
					);

	_SendDirectHTTP(query, ret);
	_log(query);
	MessageBox(NULL,ret, "error", MB_OK);
	_log(ret);

	//	ret 분석. 차감에 실패하면.. 여기서 리턴.
	TiXmlDocument doc;
	doc.Parse( ret );
	if ( doc.Error() )
	{
		_log( "UseCarrot_OGP Error in %s: %s\n", doc.Value(), doc.ErrorDesc() );
		return -1;
	}
	TiXmlNode* node = 0;
	TiXmlElement* ItemInfoEl = 0;
	
	node = doc.FirstChild( "XMLResponseData" );
	//	에러가 발생했나?
	{
		ItemInfoEl = node->FirstChildElement("ActionInfo");
		TiXmlElement* valueEl = ItemInfoEl->FirstChildElement("ReturnCode");
		int retCode = atoi(valueEl->GetText());
		switch(retCode)
		{
		case 10001:	//	로그인이 되지 않음.
		case 10002: //	블럭된 사용자
		case 10003: //	블럭된 IP
		case 10004: //	접속 불가 지역
		case 10005: //	존재하지 않는 사용자
		case 10006: //	존재하지 않는 서비스
		case 10007: //	유효하지 않은 토큰
		case 10008: //	존재하지 않는 토큰입니다.
		case 10009:	//	ServiceID와 토큰이 일치하지 않습니다.
		case 10010:	//	UserID와 토큰이 일치하지 않습니다.
		case 10011:	//	ServiceKey가 정상적이지 않습니다.
		case 10012:	//	접근 권한을 가지고 있지 않습니다.
		case 10013:	//	암호가 일치하지 않습니다. 로그인 실패
		case 10014:	//	X번이상 암호를 실패하였습니다. - 경고
		case 10015:	//	X번이상 실패로 아이디가 블럭되었습니다. 
		case 10016:	//	활성화되지 않은 사용자입니다.
		case 10017:	//	180일 이상 로그인하지 않아 폐기된 아이디입니다. 
			return -1;
		case 10000:	//	성공
			break;
		}
	}

	_log(" user astro");
	{
		//	차감된 금액 확인
		ItemInfoEl = node->FirstChildElement("UserAstro");
		TiXmlElement* valueEl2 = ItemInfoEl->FirstChildElement("Astro");	//	남은 금액
		int curAstro = atoi(valueEl2->GetText());
		doc.Clear();

		_log(" user astro %d " , curAstro);
		return curAstro;
	}
#endif

	return -1;
}

int
cADO_CARROT::GetCarrot(char * _strId)
{
	int	ret = -1;

	try
	{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"sp_red_PointCheck2";
		pCmd->CommandType  = adCmdStoredProc;

		if	(pCmd->ActiveConnection==NULL)
			return ret;

		DEF_PARAM(pCmd,"@uid") = _variant_t((char *)_strId);
		hr	=	pCmd->Execute(NULL,NULL,NULL);

		VARIANT regIdx;
//		regIdx = pCmd->GetParameters()->GetItem("@ECD")->Value;
//		ret= (DWORD)regIdx.intVal;
//		if(ret==0)
//		{
			regIdx	=	pCmd->GetParameters()->GetItem("@WP")->Value;
			ret		=	regIdx.intVal;
//		}else
//			ret = 0xffffffff;
		VariantClear(&regIdx);
		pCmd.Release();

		if	(FAILED(hr))
		{	
			_log("ERROR : link Web Log Save Fail- Execute !! \n");
		}
	}
	catch( _com_error &e){
		DMPcomERR(e)
	}

	return ret;
}
void
cADO_WEB::UseCarrot_OGP(GDMSG_USE_CARROT_LOG * rPacket,DGMSG_USE_CARROT_RESULT * sPacket)
{
#ifdef _OGP_SERVICE
	int identity =0;
	int transId = 0;
	int isError = 2;
	//	사이버 머니를 뺀다.
	{
		char query[1024];
		char ret[2048];
		memset(query, 0,  1024);
		memset(ret, 0 ,2048);

		if	(g_bIsMainTestServer)
			sprintf(query,"http://api.ogplanet.com/buyItem.og?ServiceID=%d&ServiceKey=%s&Token=%s&UserID=%s&ReceiverID=%s&UserIP=%s&Astro=%d&ItemID=%d&ItemName=%s&Quantity=%d&UsedType=%d",
				dOGP_GAME_SERVICE_ID, dOGP_GAME_SERVICE_KEY, rPacket->ogpServiceKey.m_strToken, rPacket->strId, rPacket->strReceiverId, rPacket->strIP, rPacket->sPrice, rPacket->itemno,rPacket->ogpServiceKey.m_strItemName, rPacket->itcnt,rPacket->ogpServiceKey.m_wUsedType
				);
		else
			sprintf(query,"https://billing.ogplanet.com/api/buyItem.og?ServiceID=%d&ServiceKey=%s&Token=%s&UserID=%s&ReceiverID=%s&UserIP=%s&Astro=%d&ItemID=%d&ItemName=%s&Quantity=%d&UsedType=%d",
				dOGP_GAME_SERVICE_ID, dOGP_GAME_SERVICE_KEY, rPacket->ogpServiceKey.m_strToken, rPacket->strId, rPacket->strReceiverId, rPacket->strIP, rPacket->sPrice, rPacket->itemno,rPacket->ogpServiceKey.m_strItemName, rPacket->itcnt,rPacket->ogpServiceKey.m_wUsedType
				);

		_SendDirectHTTP(query, ret);

		_log(query);
		_log(ret);
		
		//	ret 분석. 차감에 실패하면.. 여기서 리턴.
		TiXmlDocument doc;
		doc.Parse( ret );
		if ( doc.Error() )
		{
			_log( "UseCarrot_OGP Error in %s: %s\n", doc.Value(), doc.ErrorDesc() );
			return ;
		}
		TiXmlNode* node = 0;
		TiXmlElement* ItemInfoEl = 0;
		
		node = doc.FirstChild( "XMLResponseData" );
		//	에러가 발생했나?
		{
			ItemInfoEl = node->FirstChildElement("ActionInfo");
			TiXmlElement* valueEl = ItemInfoEl->FirstChildElement("ReturnCode");
			int retCode = atoi(valueEl->GetText());
			switch(retCode)
			{
			case 10000:	//	성공
				sPacket->wResult = eUCR_SUCCESS;
//				if(!rPacket->wIsPublicItem)
//					isError = 1;	//	테이블 삽입 과정을 거치지 않는다.
				break;
			case 10001:	//	로그인이 되지 않음.
			case 10002: //	블럭된 사용자
			case 10003: //	블럭된 IP
			case 10004: //	접속 불가 지역
			case 10005: //	존재하지 않는 사용자
			case 10006: //	존재하지 않는 서비스
			case 10007: //	유효하지 않은 토큰
			case 10008: //	존재하지 않는 토큰입니다.
			case 10009:	//	ServiceID와 토큰이 일치하지 않습니다.
			case 10010:	//	UserID와 토큰이 일치하지 않습니다.
			case 10011:	//	ServiceKey가 정상적이지 않습니다.
			case 10012:	//	접근 권한을 가지고 있지 않습니다.
			case 10013:	//	암호가 일치하지 않습니다. 로그인 실패
			case 10014:	//	X번이상 암호를 실패하였습니다. - 경고
			case 10015:	//	X번이상 실패로 아이디가 블럭되었습니다. 
			case 10016:	//	활성화되지 않은 사용자입니다.
			case 10017:	//	180일 이상 로그인하지 않아 폐기된 아이디입니다. 
			default:
				sPacket->wResult = eUCR_FAILED;
				_log(" OGP_ERROR(ASTRO): buyItem.og ret value : %d", retCode);
				return ;
			}
		}

		//	차감된 금액 확인
		{
			ItemInfoEl = node->FirstChildElement("ItemInfo");
			
			TiXmlElement* transEl = ItemInfoEl->FirstChildElement("TransactionID");	//	차감된 금액
			transId = atoi(transEl->GetText());
			
			TiXmlElement* valueEl = ItemInfoEl->FirstChildElement("Astro");	//	차감된 금액
			int minusAstro = atoi(valueEl->GetText());

			TiXmlElement* valueEl2 = ItemInfoEl->FirstChildElement("AstroBalance");	//	남은 금액
			sPacket->crcnt = atoi(valueEl2->GetText());
			_log(" Success !! sPacket->crcnt [%d]", sPacket->crcnt);
		}
		doc.Clear();
	}

	//	정상적으로 차감이 되었으면 아이템을 넣는다. -> 무조건 들어가게..
//	if	(rPacket->wIsPublicItem)
	{
		int _ERROR_LINE = 0;
		try{
			_CommandPtr		pCmd = NULL;
			HRESULT			hr = CoInitialize(0);
			
			//Command 객체 이용
			pCmd.CreateInstance(__uuidof(Command));
			pCmd->ActiveConnection = GetConnection();
			pCmd->CommandText = L"spPItem_Insert";
			pCmd->CommandType  = adCmdStoredProc;
			if(pCmd->ActiveConnection==NULL)
			{
				sPacket->crcnt = 0;
				sPacket->wResult = eUCR_FAILED;
				isError = 2;
			}else
			{
				char strDate[64];
				sprintf(strDate,"%d-%d-%d %d:%d:%d",cSRVUTIL::GetYear()+1900,cSRVUTIL::GetMonth()+1,cSRVUTIL::GetDay(),cSRVUTIL::GetHour(),cSRVUTIL::GetMin(),cSRVUTIL::GetSec());
				DEF_PARAM(pCmd,"@id") = _variant_t((char *)rPacket->strId);
				DEF_PARAM(pCmd,"@itemno") = _variant_t((long)rPacket->itemno);
				DEF_PARAM(pCmd,"@payment") = _variant_t((long)rPacket->ogpServiceKey.m_wUsedType);
				_log("strDate = [%s] ",strDate);
				DEF_PARAM(pCmd,"@end_date") = _variant_t((char *)strDate);
				DEF_PARAM(pCmd,"@isShare") = _variant_t((long)rPacket->wIsPublicItem);
				hr = pCmd->Execute(NULL,NULL,NULL);
				VARIANT regIdx;
				if(FAILED(hr))
				{
					_log("spPItem_Insert ERROR\n");
					isError = 2;
				}else
				{
					regIdx = pCmd->GetParameters()->GetItem("@ret")->Value;
					identity = (int)regIdx.intVal;
					if(identity != 0)
					{
						sPacket->wResult = eUCR_SUCCESS;
						isError = 1;
					}
					else
					{
						_log(" OGP_ERROR(ASTRO): spPItem_Insert : identity %d", identity);
						sPacket->wResult = eUCR_FAILED;
						isError = 2;
					}
				}
				VariantClear(&regIdx);
			}

			pCmd.Release();
		}
		catch( _com_error &e){
			DMPcomERR(e)
			_log("ERROR LINE : %d \n", _ERROR_LINE);
			isError = 2;
		}
	}

	//	정상적으로 아이템이 들어갔다면 사이버 머니 차감을 최종 커밋한다.
	{
		char query[1024];
		char ret[2048];
		memset(query, 0,  1024);
		memset(ret, 0,  2048);

		if	(g_bIsMainTestServer)
			sprintf(query,"http://api.ogplanet.com/callbackBuyItem.og?ServiceID=%d&ServiceKey=%s&Token=%s&UserID=%s&UserIP=%s&TransactionID=%d&Status=%d",	//	1 지급 완료.
				dOGP_GAME_SERVICE_ID, dOGP_GAME_SERVICE_KEY, rPacket->ogpServiceKey.m_strToken, rPacket->strId, rPacket->strIP, transId , isError
				);
		else
			sprintf(query,"https://billing.ogplanet.com/api/callbackBuyItem.og?ServiceID=%d&ServiceKey=%s&Token=%s&UserID=%s&UserIP=%s&TransactionID=%d&Status=%d",	//	1 지급 완료.
				dOGP_GAME_SERVICE_ID, dOGP_GAME_SERVICE_KEY, rPacket->ogpServiceKey.m_strToken, rPacket->strId, rPacket->strIP, transId , isError
				);

		_SendDirectHTTP(query, ret);

		_log(query);
		_log(ret);

		if(isError == 2)	//	결과를 확인해서 문제가 있다면 삽입한 아이템을 빼야 한다. 
		{
			//	xml에서 돌려진 값을 확인한다.
			{
				TiXmlDocument doc;
				doc.Parse( ret );
				if ( doc.Error() )
				{
					_log( "UseCarrot_OGP Error in %s: %s\n", doc.Value(), doc.ErrorDesc() );
					return ;
				}
				TiXmlNode* node = 0;
				TiXmlElement* ItemInfoEl = 0;
				
				node = doc.FirstChild( "XMLResponseData" );
				
				{
					ItemInfoEl = node->FirstChildElement("ItemInfo");
					
					TiXmlElement* valueEl2 = ItemInfoEl->FirstChildElement("AstroBalance");	//	남은 금액
					sPacket->crcnt = atoi(valueEl2->GetText());
					_log(" error !! sPacket->crcnt [%d]", sPacket->crcnt);
				}
				doc.Clear();
			}				
		}
	}
#endif 
}

void
cADO_WEB::UseCarrot(GDMSG_USE_CARROT_LOG * rPacket,DGMSG_USE_CARROT_RESULT * sPacket)
{
#ifdef _OGP_SERVICE
	UseCarrot_OGP(rPacket,sPacket);
	return;
#endif

	
	try{
		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);
		
		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection = GetConnection();
		pCmd->CommandText = L"sp_red_CIG_BUY_test";
		pCmd->CommandType  = adCmdStoredProc;
		if(pCmd->ActiveConnection==NULL)
		{
			sPacket->crcnt = 0;
			sPacket->wResult = eUCR_FAILED;
			return ;
		}
		char strDate[64];
		//		sprintf(strDate,"%d%02d%02d%02d%02d02%d",cSRVUTIL::GetYear()+1900,cSRVUTIL::GetMonth(),cSRVUTIL::GetDay(),cSRVUTIL::GetHour(),cSRVUTIL::GetMin(),cSRVUTIL::GetSec());
		sprintf(strDate,"%d-%d-%d %d:%d:%d",cSRVUTIL::GetYear()+1900,cSRVUTIL::GetMonth()+1,cSRVUTIL::GetDay(),cSRVUTIL::GetHour(),cSRVUTIL::GetMin(),cSRVUTIL::GetSec());
		
		DEF_PARAM(pCmd,"@SID") = _variant_t((char *)rPacket->strServerName);
		DEF_PARAM(pCmd,"@ID") = _variant_t((char *)rPacket->strId);
		DEF_PARAM(pCmd,"@CN") = _variant_t((long)rPacket->strName);
		DEF_PARAM(pCmd,"@JOB") = _variant_t((long)rPacket->job);
		DEF_PARAM(pCmd,"@LEV") = _variant_t((long)rPacket->lev);
		DEF_PARAM(pCmd,"@ITMNO") = _variant_t((long)rPacket->itemno);
		DEF_PARAM(pCmd,"@ITCNT") = _variant_t((long)rPacket->itcnt);
		DEF_PARAM(pCmd,"@CRCNT") = _variant_t((long)rPacket->sPrice);
		DEF_PARAM(pCmd,"@CODE") = _variant_t((char *)strDate);
		DEF_PARAM(pCmd,"@ISRECTOWEB") = _variant_t((long)rPacket->wIsPublicItem);	//장바구니로 보낸다.

		hr = pCmd->Execute(NULL,NULL,NULL);
		
		VARIANT regIdx;
		regIdx = pCmd->GetParameters()->GetItem("@RWP")->Value;
		sPacket->crcnt = regIdx.intVal;
		regIdx = pCmd->GetParameters()->GetItem("@ECD")->Value;
		sPacket->wResult = (WORD)regIdx.intVal;
		if(sPacket->wResult == 0)
			sPacket->wResult = eUCR_SUCCESS;
		else
		{
			switch(sPacket->wResult)
			{
			case 100:	//	없는 아이디
				sPacket->wResult = eUCR_NOTFOUNDUSER;
				break;
			case 101:	//	당근 수 부족
				sPacket->wResult = eUCR_LOW_COUNT;
				break;
			case 103:	//	미동의 , 남은 당근갯수는 0으로 무조건 처리
				sPacket->wResult = eUCR_AGREEMENT_FAILED;
				break;
			case 104:	//	아이템이 없다.
				sPacket->wResult = eUCR_FAILED;
				break;
			}
		}
	

		VariantClear(&regIdx);
		pCmd.Release();
		if(FAILED(hr)){		_log("ERROR : link Web Log Save Fail- Execute !! \n");		}
	}
	catch( _com_error &e){
		DMPcomERR(e)
	}
	return;
}