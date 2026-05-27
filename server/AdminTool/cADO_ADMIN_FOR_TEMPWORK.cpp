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


void	cADO_ADMIN::sp20030509ReSet()
{
	_RecordsetPtr  pRs = NULL; 	
	_variant_t     v_bufferdata;
	char	       str_Query[1024];
	int i_RecCount;
	int i;
	char *pBuf = NULL;
	HRESULT hr = CoInitialize(0);
/*
	//	이름을 읽어온다.
	FILE *fp = fopen("okreal.txt","r");
	char goldName[2389][22];
	char	_readline[32];

	for(i=0;i<2388;i++)
	{
		int count = 0;
		memset(goldName[i],0,22);
		memset(_readline,0,32);
		char tmp;
		tmp = fgetc(fp);
		while(tmp!=EOF && tmp!=10)
		{
			_readline[count++] = (char)tmp;
			tmp = fgetc(fp);
		}
		if(strcmp(_readline,"END;")==0)	break;
		memcpy(goldName[i],_readline,22);
		printf("name = [%s] \n",goldName[i]);
	}
	fclose(fp);	fp = NULL;
*/
	sprintf(str_Query,"SELECT master,name,data FROM Avatar_Current where master='kyung1007'");//master=''
	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount;

//	int	check[2388];
//	memset(check,0,sizeof(int)*2388);
//	int ccc = 0;

//	FILE	*fp1,*fp2,*fp3,*fp,*fp4;
//FILE	*fp1;
//	fp	=	fopen("_업데이트 해야할 넘들.txt","wb");
//	fp1	=	fopen("_이름 이상한 넘들.txt","w");
//	fp2	=	fopen("_기존 데이터가 잘못된 넘들.txt","wb");
//	fp3	=	fopen("_기존 데이터가 잘못된 넘들2.txt","wb");
//	fp4	=	fopen("_세이브 데이터가 이상한 넘들.txt","wb");
	char name[22];char id[20];

	if(!FAILED(hr))
	{
		for(i = 0 ;i < i_RecCount ;i++)
		{
			cPLAYER_DATA	pData;
			v_bufferdata = pRs->Fields->GetItem((LONG)1)->Value;
			strcpy(name,(_bstr_t)v_bufferdata);
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			strcpy(id,(_bstr_t)v_bufferdata);
			v_bufferdata = pRs->Fields->GetItem((LONG)2)->Value;
			VariantToBlob(v_bufferdata,&pData);
			
/*			if(	stricmp(name,"EMPTY")==0 || stricmp(name,"dummy!!")==0 || 
					strstr(name,"　") || strstr(name,"\\") || strstr(name,"%") || 
					strstr(name,"\"") || strstr(name,"\'") ||
					strstr(id,"　") || strstr(id,"\\") || strstr(id,"%") || 
					strstr(id,"\"") || strstr(id,"\'") ||
					stricmp(name,pData.m_strName)!=0 || stricmp(id,pData.m_strId)!=0
			)
			{
//				fprintf(fp1,"[%-20s] [%-22s] [%-20s] [%-22s]\n",id,name,pData.m_strId,pData.m_strName);
//				if(stricmp(name,pData.m_strName)!=0 || stricmp(id,pData.m_strId)!=0)
//				{
//					strcpy(pData.m_strName,name);	strcpy(pData.m_strId,id);
//					spSaveAva(&pData);
//				}
//				fflush(fp1);
			}
*/
			pRs->MoveNext();
		}
	}

//	fclose(fp);
//	fclose(fp1);
//	fclose(fp2);
//	fclose(fp3);

	MessageBox(NULL,"Save Complete","",MB_OK);
	pRs->Close();
}

/*
BOOL	cADO_ADMIN::_sqlChangeSkillData()
{
	_RecordsetPtr  pRs = NULL; 	
	_variant_t     v_bufferdata;
	char	       str_Query[1024];
	int i_RecCount;
	int i;
	char *pBuf = NULL;
	HRESULT hr = CoInitialize(0);

	sprintf(str_Query,"SELECT data FROM Avatar_Current");
	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount; 				
	printf("총 %d개의 아바타 데이타... \n",i_RecCount);
	cP_PLAYER_SAVE_DATA		*pData;
	pData = new cP_PLAYER_SAVE_DATA[i_RecCount];
	cPLAYER_DATA	* player;

	if(!FAILED(hr))
	{ 
		for(i = 0 ;i < i_RecCount ;i++)
		{
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			VariantToBlob(v_bufferdata,&pData[i]);
			player = (cPLAYER_DATA *)&pData[i];
			printf("[%s]를 불러온다. \n",pData[i].m_strName);
			player->rebuildSkillInfo();
			pRs->MoveNext();
			printf("다음으로... %d\n",i);
		}
		for(i = 0 ;i < i_RecCount ;i++)
		{
			player = (cPLAYER_DATA *)&pData[i];
			spSaveAva(player);
			printf("다음으로... %d\n",i);
		}
	}
	if(i_RecCount==0)
	{
		pRs->Close();
		return FALSE;
	}
	pRs->Close();	


	return TRUE;
}
*/



void	cADO_ADMIN::spGetBankConvert20031203()
{
/*	int lostcount = 0;

	_RecordsetPtr  pRs = NULL; 	
	_variant_t     v_bufferdata;
	char	       str_Query[1024];
	int i_RecCount;
	int i;
	char *pBuf = NULL;
	HRESULT hr = CoInitialize(0);
	sprintf(str_Query,"SELECT item FROM Avatar_Bank");
	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount;

	if(!FAILED(hr))
	{
		int iBankSlot;
		for(i = 0 ;i < i_RecCount ;i++)
		{
			CBankInfo20031203	oldBank;
			CBankInfo			bank;

			int size = sizeof(CBankInfo20031203);
			int size2 = sizeof(CBankInfo);
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;

			if(v_bufferdata.vt == VT_NULL)
			{	
				pRs->MoveNext();
				continue;
			}

			if(!VariantToBlob(v_bufferdata,&oldBank))
			{	
				pRs->MoveNext();	
				continue;	
			}
			//	뱅크 데이타 변환하고... 파일에 저장한다.

			strcpy(bank.m_strId,oldBank.m_strId);

			bank.m_dwGold			=	oldBank.m_dwGold;
			bank.m_dwSaveCounter	=	oldBank.m_dwSaveCounter;

			for (iBankSlot=0;iBankSlot<dMAX_ITEM_IN_BANK;iBankSlot++)
			{
				bank.m_aItems[iBankSlot].reset();

				if (oldBank.m_aItems[iBankSlot].m_wBaseItem != 0xffff)
				{
					memcpy(&bank.m_aItems[iBankSlot],&oldBank.m_aItems[iBankSlot],sizeof(cP_ITEM));
					
					int	a=0;
					a++;
				}
			}

			cFILE	file;

			char	strFileName[256];

			sprintf(strFileName,"%s.dat",bank.m_strId);

			file.Open(strFileName,"wb");
			file.Write(&bank,sizeof(bank));
			file.Close();

			//	저장하기..
//			if(spSaveBank(&bank)!=2)
//			{
//				lostcount++;
//			}
			pRs->MoveNext();
		}
	}

	//	저장실패.
	char txt[32];
	sprintf(txt,"Bank Error %d",lostcount);
	MessageBox(NULL,txt,"ERROR BANK",MB_OK);
	MessageBox(NULL,"은행 변경 작업 완료","",MB_OK);
	pRs->Close();
*/
}


/*
void	cADO_ADMIN::RecoverChechID()
{
//	checkCount = 0;

//	FILE * fp = fopen("IDs.txt","r");
//	if(!fp)		return;
//	while(1)
//	{
//		int		tmp;
//		int		count=0;
//		char	_readline[dID_LENGTH+2];
//		memset(_readline,0,sizeof(_readline));

//		tmp = fgetc(fp);
//		while(tmp!=EOF && tmp!=10)
//		{
//			_readline[count++] = (char)tmp;
//			tmp = fgetc(fp);
//		}
//		if(tmp==EOF)	break;
//		_readline[count]=0;
//		strcpy(checkID[checkCount++],_readline);
//	}
//	fclose(fp);

	_RecordsetPtr  pRs = NULL;
	_variant_t     v_bufferdata;
	char	       str_Query[1024];
	int i_RecCount;
	int i;
	char *pBuf = NULL;
	HRESULT hr = CoInitialize(0);

	sprintf(str_Query,"SELECT master,data FROM Avatar_Current WHERE master='f18mm' and avaidx<>0");
	printf(str_Query);
	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount;

	if(!FAILED(hr))
	{ 
		char	strid[dID_LENGTH];
		int cc = 0;
		for(i = 0 ;i < i_RecCount ;i++)
		{
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			strcpy(strid,(_bstr_t)v_bufferdata);
//			for(int c = 0;c<checkCount;c++)
//				if(stricmp(checkID[c],strid)==0)
//					break;
//			if(c<checkCount)	//	복구한다.
			{
				cPLAYER_DATA		pData;
				v_bufferdata = pRs->Fields->GetItem((LONG)1)->Value;
				VariantToBlob(v_bufferdata,&pData);
				char fn[128];
				sprintf(fn,"%s-%d.sav",strid,cc++);
				FILE * fp = fopen(fn,"wb");
				fwrite(&pData,sizeof(cPLAYER_DATA),1,fp);
				fclose(fp);
			}
			pRs->MoveNext();
		}
	}
	pRs->Close();
//	fclose(fps);	fclose(fpf);
}
*/


int		cADO_ADMIN::CheckIDs()
{
/*	_RecordsetPtr  pRs = NULL;
	_variant_t     v_bufferdata;
	char	       str_Query[1024];
	int i_RecCount;
	int i;
	char *pBuf = NULL;
	HRESULT hr = CoInitialize(0);

	sprintf(str_Query,"SELECT GameId FROM USER_TABLE WHERE lastdate>20031114");
	printf(str_Query);
	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount;

	if(!FAILED(hr))
	{ 
		FILE * fp = fopen("IDs-0.txt","w+");
		if(!fp)	return 0;
		char	_strId[dID_LENGTH];
		for(i = 0 ;i < i_RecCount ;i++)
		{
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			strcpy(_strId,(_bstr_t)v_bufferdata);
			fprintf(fp,"%s\n",_strId);fflush(fp);
			pRs->MoveNext();
			if(i>0 && i%10000 == 0)
			{
				fclose(fp);
				char fn[128];
				sprintf(fn,"IDs-%d.txt",i/10000);
				FILE * fp = fopen(fn,"w+");
				if(!fp)
				{
					int j;
					j++;
				}
			}
		}
		fclose(fp);
	}
	pRs->Close();

	return checkCount;


*/
	
	
/*	FILE * fp = NULL;
	fp = fopen("f18mm-2.sav","rb");
	if(fp)
	{
		cPLAYER_DATA		pData;	memset(&pData,0,sizeof(cPLAYER_DATA));
		fread(&pData,sizeof(cPLAYER_DATA),1,fp);
		fclose(fp);
		if(pData.m_dwSaveCount>0)
		{	//	저장한다.
			int result = spCreateAva(&pData);
			if(result>=0 && result<4)
			{
				DeleteFile("f18mm-2.sav");
			}
		}
	}
*/
	return 1;
}
