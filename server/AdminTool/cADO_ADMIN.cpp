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

char strAvatarDBCId[32];
char strAvatarDBCPw[32];
char strAccountDBCId[32];
char strAccountDBCPw[32];
char strLogFtpId[32];
char strLogFtpPw[32];
char strLoginDBId[32];
char strLoginDBPw[32];
int	 g_iWorldFtpPort,g_iLogFtpPort;
int	 g_iDBPort;

cADO_ADMIN	g_db;

_variant_t  vtEmpty2 (DISP_E_PARAMNOTFOUND, VT_ERROR);

//static	char	checkID[80000][dID_LENGTH];
//static	int		checkCount;

cADO_ADMIN::cADO_ADMIN() : cADO_BASE()
{
	iMaxCount	=	0;
/*	GlastLine = 0;
	InitializeCriticalSection(&csConnectionPool);
	hThread = NULL;
	isEnd = FALSE;
	iMaxCount = 0;
	pConnection = NULL;
*/
}

cADO_ADMIN::~cADO_ADMIN()
{
	Close();
	DeleteCriticalSection(&csConnectionPool);
}

void	cADO_ADMIN::reConnect(int num)
{
	try
	{	_bstr_t		strProvider(pro);
		pConnection[num].CreateInstance("ADODB.Connection");
		pConnection[num]->Open(strProvider,"","",NULL);
	}
	catch(_com_error &e){	DMPcomERR(e);	}
}

void	cADO_ADMIN::BlobToVariant(VARIANT &varChunk,void *pData, UINT nBytes)	//	data -> blob
{
	HRESULT hr;
	long lngOffset = 0;
	UCHAR chData;
	SAFEARRAY FAR *psa = NULL;
	SAFEARRAYBOUND rgsabound[1];

	try
	{
		//Create a safe array to store the array of BYTES 
		rgsabound[0].lLbound = 0;
		rgsabound[0].cElements = nBytes;
		psa = SafeArrayCreate(VT_UI1,1,rgsabound);
		if(psa!=NULL)
		{
			while(lngOffset < (long)nBytes)
			{
				chData	= ((UCHAR*)pData)[lngOffset];
				hr = SafeArrayPutElement(psa, &lngOffset, &chData);

				lngOffset++;
			}
			lngOffset = 0;

			//Assign the Safe array  to a variant.
			varChunk.vt = VT_ARRAY|VT_UI1;
			varChunk.parray = psa;
		}else{
			_log("SERROR BLOB : psa = NULL");
		}
	}
	catch(_com_error &e)
	{
		DMPcomERR(e);
	}
}

int		cADO_ADMIN::VariantToBlob(VARIANT & varChunk,void * pData)	//	blob -> data
{
	HRESULT			hr = 0;
	UCHAR chData =0;
	long lBytesCopied = 0;

	if(varChunk.vt == VT_NULL){	pData = NULL;	return FALSE;	}

	long length = varChunk.parray->rgsabound->cElements;
	memset(pData,0,length);
	for(long lIndex = 0; lIndex < length ; lIndex++)
	{
		hr= SafeArrayGetElement(varChunk.parray, &lIndex, &chData);
		if(SUCCEEDED(hr))
		{
			((UCHAR*)pData)[lBytesCopied] = chData;
			lBytesCopied++;
		}
		else
			break;
	}
	return (int)lBytesCopied;
}

class	CCharacterData
{
public:
	WORD		m_wJob;
	char		m_strName[20];
	cP_ITEM		m_aItems[42];
	cP_ITEM		m_aEquip[30];
};


void
GetCheckName(int _iResult,char *_lpstrCheck)
{
	switch(_iResult)
	{
		case	eCADR_ISOK:
			strcpy(_lpstrCheck,"eCADR_ISOK");
			break;
		case	eCADR_IS_NPC:
			strcpy(_lpstrCheck,"eCADR_IS_NPC");
			break;
		case	eCADR_IS_NPC2:
			strcpy(_lpstrCheck,"eCADR_IS_NPC2");
			break;
		case	eCADR_VALID_SERIAL:
			strcpy(_lpstrCheck,"eCADR_VALID_SERIAL");
			break;
		case	eCADR_INCORRECT_JOB:
			strcpy(_lpstrCheck,"eCADR_INCORRECT_JOB");
			break;
		case	eCADR_INCORRECT_ITEM_SLOT_COUNT:
			strcpy(_lpstrCheck,"eCADR_INCORRECT_ITEM_SLOT_COUNT");
			break;
		case	eCADR_INCORRECT_LEVEL:
			strcpy(_lpstrCheck,"eCADR_INCORRECT_LEVEL");
			break;
		case	eCADR_INCORRECT_SKILL_DATA:
			strcpy(_lpstrCheck,"eCADR_INCORRECT_SKILL_DATA");
			break;
		case	eCADR_MISMATCH_SKILL_AND_JOB:
			strcpy(_lpstrCheck,"eCADR_MISMATCH_SKILL_AND_JOB");
			break;
		case	eCADR_MISMATCH_NAME_BY_CLIENT:
			strcpy(_lpstrCheck,"eCADR_MISMATCH_NAME_BY_CLIENT");
			break;
		case	eCADR_MISMATCH_ID_BY_CLIENT:
			strcpy(_lpstrCheck,"eCADR_MISMATCH_ID_BY_CLIENT");
			break;
	}
}

	enum
	{
		eSAVE_ITEM,
		eLOAD_ITEM,
	};

class	CBankLogInfo
{
public:
	int			m_iMethod;
	int			m_iCount;
	CItemDefine	m_aItem[100];
	char		m_strId[dID_LENGTH];
	int			m_iGold;

	void		reset()
	{
		m_strId[0]	=	0;
		m_iCount	=	0;

		for (int i=0;i<100;i++)	m_aItem[i].reset();
	}
};

CBankInfo	*l_lpBankBuffer	=	NULL;
int			l_iBankCount	=	0;

CBankInfo*
GetBank(char *_lpstrId)
{
	for (int i=0;i<l_iBankCount;i++)
	{
		if (stricmp(l_lpBankBuffer[i].m_strId,_lpstrId)==0)	return	&l_lpBankBuffer[i];
	}

	return	NULL;
}


void	cADO_ADMIN::spBankConvert20040108()
{
	return;
/*	
	int lostcount = 0;

	_RecordsetPtr  pRs = NULL; 	
	_variant_t     v_bufferdata;
	char	       str_Query[1024];
	int i_RecCount;
	char *pBuf = NULL;
	HRESULT hr = CoInitialize(0);
//	sprintf(str_Query,"SELECT item FROM Avatar_Bank where master='alsdkdi'");
	sprintf(str_Query,"SELECT item FROM Avatar_Bank");
	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount;

	if(!FAILED(hr))
	{
		int		iBankSlot;
		CBankInfo			bank;
		for(int i = 0 ;i < i_RecCount ;i++)
		{
			v_bufferdata	=	pRs->Fields->GetItem((LONG)0)->Value;

			if(v_bufferdata.vt == VT_NULL)
			{
				bank.reset();
			}
			else if(!VariantToBlob(v_bufferdata,&bank))
			{
				pRs->MoveNext();	
				continue;
			}
//			spSaveBank(&bank);

			pRs->MoveNext();
			Sleep(1);
		}
	}
	//	저장실패.
	MessageBox(NULL,"Bank Data Change Complete","",MB_OK);
	pRs->Close();
*/
}

int	cADO_ADMIN::spSaveBank(CBankInfo * bank)
{
	if	(bank==NULL)
	{
		_log("ERROR SaveBank : pData is NULL");
		return -1;
	}
	try
	{
		bank->m_dwSaveCounter++;

		_CommandPtr		pCmd = NULL;
		HRESULT			hr = CoInitialize(0);

		char	* compressData = new char[sizeof(CBankInfo)];

		VARIANT bin;	
		
		VariantInit(&bin);
		memset(compressData,0,sizeof(CBankInfo));
		WORD sz = cSRVUTIL::Compress((unsigned char *)bank,(unsigned char *)compressData,sizeof(CBankInfo));
		BlobToVariant(bin,compressData,sz);

		//Command 객체 이용
		pCmd.CreateInstance(__uuidof(Command));
		pCmd->ActiveConnection		=	GetConnection();
		pCmd->CommandText			=	L"spAvatar_SAVEBANK";
		pCmd->CommandType			=	adCmdStoredProc;

		DEF_PARAM(pCmd,"@VER")		=	_variant_t((long)8);
		DEF_PARAM(pCmd,"@MASTER")	=	_variant_t((char *)bank->m_strId);
		DEF_PARAM(pCmd,"@COUNT")	=	_variant_t((long)bank->m_dwSaveCounter);

		pCmd->Parameters->Item[_variant_t("@BANK")]->AppendChunk(bin);
		hr = pCmd->Execute(NULL,NULL,NULL);

		VariantClear(&bin);

		delete [] compressData;
		compressData = NULL;

		if	(FAILED(hr))
		{
			_log("ERROR : BANK Save Fail- Execute !! \n");
		}

		VARIANT regIdx;
		VariantInit(&regIdx);
		regIdx.vt = VT_INT;
		regIdx.lVal = pCmd->GetParameters()->GetItem("@RESULT")->Value;
		int	iResult = (int)regIdx.lVal;
		VariantClear(&bin);
		pCmd.Release();
		pCmd.Detach();
		pCmd = NULL;
	}
	catch( _com_error &e)
	{
		DMPcomERR(e)
	}

	return -1;
}

int cADO_ADMIN::LoadBankInfo(char * master, CBankInfo * bank)
{
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
			if(unsize<sizeof(CBankInfo))
			{
				WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)bank,unsize,sizeof(CBankInfo));
				if(ww == 0)	_log(" uncompress error 3");
			}else{
				memcpy(bank,uncompressdata,sizeof(CBankInfo));
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
