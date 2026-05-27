#include "StdAfx.h"
#include "cPLAYER_DATA.h"
#include "cADO_ADMIN.h"
#include <stdio.h>
#include <process.h>
#include "cSRVUTIL.h"
#include "RSComm.h"
#include "CSkill.h"
#include "CharacterInfo.h"
#include "SFC.h"
#include "CMessage.h"
#include "cITEM.h"


//
//	데이터 로딩
BOOL
cADO_ADMIN::loadData(char *_lpstrId,char *_lpstrName,void *_lpData,int _iWantCount,int _iJob,int _iLevel)
{
	try
	{
		CAvatarManager	*lpAm	=	(CAvatarManager	*)_lpData;

		_RecordsetPtr  pRs = NULL; 	
		char	       str_Query[1024];
		int				i_RecCount = 0;
		int				i = 0;
		char			*pBuf = NULL;
		HRESULT			hr = CoInitialize(0);

		if	(_lpstrId	==	NULL	&&	_lpstrName	==	NULL	&&	_iWantCount)
			sprintf(str_Query,"select top %d data from Avatar_Current where job = %d and lev = %d order by lastdate desc",_iWantCount,_iJob,_iLevel);
		else
		if	(_lpstrId)
			sprintf(str_Query,"SELECT data FROM Avatar_Current WHERE master='%s'",_lpstrId);
		else
		if	(_lpstrName)	
			sprintf(str_Query,"SELECT data FROM Avatar_Current WHERE name='%s'",_lpstrName);
		else
		{
			MessageBox(GetActiveWindow(),g_strToolMessage[30],"Data Load Error !!",MB_ICONERROR);
			return	FALSE;
		}

		pRs.CreateInstance(__uuidof(Recordset)); 

		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
		i_RecCount = pRs->RecordCount;

		SetCurrentDirectory(g_strLogFolder);

		char	* uncompressdata = new char[sizeof(cPLAYER_DATA)];

		if	(!FAILED(hr))
		{
			for(i = 0 ;i < i_RecCount ;i++)
			{
				_variant_t     v_bufferdata;
				VariantInit(&v_bufferdata);
				v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
				memset(&lpAm->m_aData[i],0,sizeof(CAvatarData));
				memset(uncompressdata,0,sizeof(cPLAYER_DATA));
				int unsize = VariantToBlob(v_bufferdata,uncompressdata);
				if(unsize<sizeof(cPLAYER_DATA) && unsize != 2300 && unsize !=2272)
				{
					WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)&lpAm->m_aData[i],unsize,sizeof(cPLAYER_DATA));
					if(ww == 0)
					{
						if	(unsize >= 2048	&&	unsize	<sizeof(cPLAYER_DATA))
						{
							memset(&lpAm->m_aData[i],0,sizeof(cPLAYER_DATA));
							memcpy(&lpAm->m_aData[i],uncompressdata,unsize);
						}

						_log(" uncompress error 2");
					}
				}else{
					memcpy(&lpAm->m_aData[i],uncompressdata,sizeof(cPLAYER_DATA));
				}
				VariantClear(&v_bufferdata);
				pRs->MoveNext();
			}
		}
		delete [] uncompressdata;

		lpAm->m_iLoadDataCount	=	i_RecCount;

		pRs->Close();
		lpAm->m_bank.reset();
		lpAm->m_bIsChangeBankData	=	FALSE;

		if	(lpAm->m_iLoadDataCount	<=	0	)
			return TRUE;
	//	if (m_bIsNormalDB			==	FALSE)	return TRUE;

		sprintf(str_Query,"SELECT item FROM Avatar_Bank WHERE master='%s'",lpAm->m_aData[0].m_strId);

		pRs->CursorLocation  = adUseClient;	
		hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
		i_RecCount = pRs->RecordCount;

		if	(!FAILED(hr))
		{
			char	* uncompress = new char[sizeof(CBankInfo)];

			if	(i_RecCount	==	0)
			{
				lpAm->m_bank.reset();
				lpAm->m_bIsInvalidBankData	=	TRUE;
			}

			for(i = 0 ;i < i_RecCount ;i++)
			{
				_variant_t     v_bufferdata;
				VariantInit(&v_bufferdata);

				v_bufferdata	=	pRs->Fields->GetItem((LONG)0)->Value;

				if	(v_bufferdata.vt == VT_NULL)
				{
					lpAm->m_bank.reset();
					lpAm->m_bIsInvalidBankData	=	TRUE;
					VariantClear(&v_bufferdata);
					continue;
				}

				lpAm->m_bIsInvalidBankData	=	FALSE;

				memset(&lpAm->m_bank,0,sizeof(CBankInfo));
				memset(uncompress,0,sizeof(CBankInfo));

				int unsize = VariantToBlob(v_bufferdata,uncompress);

				if	(unsize<sizeof(CBankInfo))
				{
					WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)&lpAm->m_bank,unsize,sizeof(CBankInfo));

					if	(ww == 0)
						_log(" uncompress error 2");
				}
				else
				{
					memcpy(&lpAm->m_bank,uncompressdata,sizeof(CBankInfo));
				}

				VariantClear(&v_bufferdata);
				pRs->MoveNext();
			}

			delete [] uncompress;
		}

		pRs->Close();
	}
	catch(_com_error &e)
	{
		DMPcomERR(e);
		return FALSE;
	}

	return	TRUE;

}	//	cADO_ADMIN::loadData(char *_lpstrId,char *_lpstrName)

//
//	데이터 로딩
BOOL
cADO_ADMIN::checkRich(char *_lpstrFileName,DWORD _dwGold)
{
	_RecordsetPtr  pRs = NULL; 	
	_variant_t     v_bufferdata;
	char	       str_Query[1024];
	int				i_RecCount;
	int				i,iCount=0;
	char			*pBuf = NULL;
	HRESULT			hr = CoInitialize(0);

	sprintf(str_Query,"SELECT data FROM Avatar_Current");

	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount;

	SetCurrentDirectory(g_strLogFolder);

	FILE	*fp;

	fp		=	fopen(_lpstrFileName,"wb");

	char	* uncompressdata = new char[sizeof(cPLAYER_DATA)];

	if(!FAILED(hr))
	{
		for(i = 0 ;i < i_RecCount ;i++)
		{
			cPLAYER_DATA	data;

			_variant_t     v_bufferdata;
			VariantInit(&v_bufferdata);
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			memset(&data,0,sizeof(CAvatarData));
			memset(uncompressdata,0,sizeof(cPLAYER_DATA));
			int unsize = VariantToBlob(v_bufferdata,uncompressdata);
			if(unsize<sizeof(cPLAYER_DATA) && unsize != 2300 && unsize !=2272)
			{
				WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)&data,unsize,sizeof(cPLAYER_DATA));
				if(ww == 0)
				{
					if	(unsize >= 2048	&&	unsize	<sizeof(cPLAYER_DATA))
					{
						memset(&data,0,sizeof(cPLAYER_DATA));
						memcpy(&data,uncompressdata,unsize);
					}

					_log(" uncompress error 2");
				}
			}else{
				memcpy(&data,uncompressdata,sizeof(cPLAYER_DATA));
			}

			VariantClear(&v_bufferdata);

			if (data.m_iGold	>=	(int)_dwGold || data.m_iGold < 0)
			{
				fprintf(fp,"[%20s/%20s] [%3d/%d]\n",data.m_strId,data.m_strName,data.m_iLevel,data.m_iGold);
				fflush(fp);
				iCount++;
			}

			pRs->MoveNext();
		}
	}

	delete	[]	uncompressdata;

	fclose(fp);

	pRs->Close();

	return	iCount;
}	//	cADO_ADMIN::loadData(char *_lpstrId,char *_lpstrName)


//
//	데이터 로딩
BOOL
cADO_ADMIN::saveData(cP_PLAYER_SAVE_DATA *_lpData,BOOL isNew,BOOL _bIsNotCheckLevelDown)
{
	spSaveAva((cPLAYER_DATA *)_lpData,_bIsNotCheckLevelDown, 1);

	return	TRUE;
}	//	cADO_ADMIN::loadData(char *_lpstrId,char *_lpstrName)

BOOL
cADO_ADMIN::resetAllPlayerData(BOOL isTestServer,BOOL _bResetStatePoint,BOOL _bResetSkillPoint,int *_lpiJobList)
{
/*	_RecordsetPtr	pRs = NULL; 	
	_variant_t		v_bufferdata;
	char			str_Query[1024];
	int				i_RecCount;
	int				i,iCount=0;
	char			*pBuf = NULL;
	HRESULT			hr = CoInitialize(0);

	sprintf(str_Query,"SELECT data FROM Avatar_Current where lev>6");

	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount;

	SetCurrentDirectory(g_strLogFolder);

	if (_bResetStatePoint && _bResetSkillPoint)	cMSG::Put("스탯,스킬 포인트 재분배","총 %d개의 캐릭터의 스탯,스킬 포인트 재분배를 시작합니다.",i_RecCount);
	else	if (_bResetSkillPoint)				cMSG::Put("스킬 포인트 재분배","총 %d개의 캐릭터의 스킬 포인트 재분배를 시작합니다.",i_RecCount);
	else	if (_bResetStatePoint)				cMSG::Put("스탯 포인트 재분배","총 %d개의 캐릭터의 스탯 포인트 재분배를 시작합니다.",i_RecCount);

	FILE	*fp;

	fp	=	fopen("리셋 리스트.txt","wb");
	
	if(!FAILED(hr))
	{
		for(i = 0 ;i < i_RecCount ;i++)
		{
			cPLAYER_DATA	data;

			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			VariantToBlob(v_bufferdata,&data);

			BOOL	bApply	=	TRUE;

			if (_lpiJobList && _lpiJobList[0] != 0xffff)
			{
				bApply	=	FALSE;

				int	iIndex=0;

				while(_lpiJobList[iIndex] != 0xffff && bApply == FALSE)
				{
					if (data.m_wJob/2 == _lpiJobList[iIndex]/2)	bApply	=	TRUE;

					iIndex	++;
				}
			}

			if (bApply)
			{
				DWORD	dwCorrectSkillPoint	=	data.getCurrentSkillPointByLevel();

				fprintf(fp,"[%20s/%20s] %d,%d\n",data.m_strId,data.m_strName,data.m_dwSaveCount,data.m_dwSkillExperience);
				fflush(fp);

				if (_bResetSkillPoint)	data.resetSkillPoint();
				if (_bResetStatePoint)	data.resetStatePoint();

				spSaveAva(&data);
			}

			pRs->MoveNext();
		}
	}

	fclose(fp);

	pRs->Close();
*/
	return	TRUE;
}

BOOL
cADO_ADMIN::findItem(cITEM *_lpItems,int _iItemCount,char *_lpstrFileName)
{
	_RecordsetPtr  pRs = NULL; 	
	_variant_t     v_bufferdata;
	char	       str_Query[1024];
	int				i_RecCount,iItem;
	int				i,iCount=0;
	char			*pBuf = NULL;
	HRESULT			hr = CoInitialize(0);

	sprintf(str_Query,"SELECT data FROM Avatar_Current");

	pRs.CreateInstance(__uuidof(Recordset)); 

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount;


	SetCurrentDirectory(g_strLogFolder);

	FILE	*fp;

	fp	=	fopen(_lpstrFileName,"wb");

	int		*apItemCount	=	new int [_iItemCount];

	memset(apItemCount,0,_iItemCount*4);

	char	* uncompressdata = new char[sizeof(cPLAYER_DATA)];

	if(!FAILED(hr))
	{
		for(i = 0 ;i < i_RecCount ;i++)
		{
			cPLAYER_DATA	data;
			v_bufferdata = pRs->Fields->GetItem((LONG)0)->Value;
			{	//	 압축풀기
				memset(uncompressdata,0,sizeof(cPLAYER_DATA));
				int unsize = VariantToBlob(v_bufferdata,uncompressdata);
				if(unsize<sizeof(cPLAYER_DATA) && unsize !=2300 && unsize !=2272)
				{
					WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompressdata,(unsigned char *)&data,unsize,sizeof(cPLAYER_DATA));
					if(ww == 0)
					{
						if	(unsize >= 2048	&&	unsize	<sizeof(cPLAYER_DATA))
						{
							memset(&data,0,sizeof(cPLAYER_DATA));
							memcpy(&data,uncompressdata,unsize);
						}
						else
							MessageBox(NULL,"ad8f0ds8f","aodf80d",MB_OK);	
					}
				}else{
					memcpy(&data,uncompressdata,sizeof(cPLAYER_DATA));
				}
			}

			for (iItem=0;iItem<dOWN_ITEM_COUNT;iItem++)
			{
				for (int iCheckItem= 0;iCheckItem<_iItemCount;iCheckItem++)
				{
					cITEM	*lpItem	=	&_lpItems[iCheckItem];

					if (data.m_aItems[iItem].m_wBaseItem == lpItem->m_wBaseItem && data.m_aItems[iItem].m_dwSerial == lpItem->m_dwSerial)
					{
						apItemCount[iCheckItem]++;

						fprintf(fp,"[\"%s\"/\"%s\"] equipment %s[%d,%d]\n",data.m_strId,data.m_strName,lpItem->getName(),lpItem->m_wBaseItem,lpItem->m_dwSerial);
					}
				}
			}

			for (iItem=0;iItem<dEQUIPMENT_PART_COUNT;iItem++)
			{
				for (int iCheckItem= 0;iCheckItem<_iItemCount;iCheckItem++)
				{
					cITEM	*lpItem	=	&_lpItems[iCheckItem];

					if (data.m_aEquip[iItem].m_wBaseItem == lpItem->m_wBaseItem && data.m_aEquip[iItem].m_dwSerial == lpItem->m_dwSerial)
					{
						apItemCount[iCheckItem]++;

						fprintf(fp,"[\"%s\"/\"%s\"] Inventory %s[%d,%d]\n",data.m_strId,data.m_strName,lpItem->getName(),lpItem->m_wBaseItem,lpItem->m_dwSerial);
					}
				}
			}

			pRs->MoveNext();
		}
	}

	fprintf(fp,"  \n");
	fprintf(fp,"  \n");

	pRs->Close();

	sprintf(str_Query,"SELECT item FROM Avatar_Bank");

	pRs->CursorLocation  = adUseClient;	
	hr = pRs->Open((_variant_t)str_Query,_variant_t((IDispatch *)GetConnection(),true), adOpenKeyset,adLockReadOnly, adCmdText);
	i_RecCount = pRs->RecordCount;

	char	* uncompress = new char[sizeof(CBankInfo)];

	if(!FAILED(hr))
	{

		for(i = 0 ;i < i_RecCount ;i++)
		{
			CBankInfo		bank;

			_variant_t     v_bufferdata;
			VariantInit(&v_bufferdata);
			v_bufferdata	=	pRs->Fields->GetItem((LONG)0)->Value;

			if(v_bufferdata.vt == VT_NULL)
			{
				bank.reset();
				VariantClear(&v_bufferdata);
				continue;
			}
			memset(&bank,0,sizeof(CBankInfo));
			memset(uncompress,0,sizeof(CBankInfo));
			int unsize = VariantToBlob(v_bufferdata,uncompress);
			if(unsize<sizeof(CBankInfo))
			{
				WORD ww = cSRVUTIL::Uncompress((unsigned char *)uncompress,(unsigned char *)&bank,unsize,sizeof(CBankInfo));
				if(ww == 0)	_log(" uncompress error 2");
			}else
			{
				memcpy(&bank,uncompress,sizeof(CBankInfo));
			}

			for (iItem=0;iItem<dMAX_ITEM_IN_BANK;iItem++)
			{
				cITEM	*lpBankItem	=	(cITEM	*)&bank.m_aItems[iItem];

				for (int iCheckItem	= 0;iCheckItem<_iItemCount;iCheckItem++)
				{
					cITEM	*lpItem	=	&_lpItems[iCheckItem];

					if (lpBankItem->m_wBaseItem == lpItem->m_wBaseItem && lpBankItem->m_dwSerial == lpItem->m_dwSerial)
					{
						apItemCount[iCheckItem]++;

						fprintf(fp,"[\"%s\"] BANK %s[%d,%d]\n",bank.m_strId,lpItem->getName(),lpItem->m_wBaseItem,lpItem->m_dwSerial);
					}
				}
			}

			VariantClear(&v_bufferdata);
			pRs->MoveNext();
		}
	}

	pRs->Close();

	fprintf(fp,"  \n");
	fprintf(fp,"  \n");

	for (int iCheckItem= 0;iCheckItem<_iItemCount;iCheckItem++)
	{
		cITEM	*lpItem	=	&_lpItems[iCheckItem];

		fprintf(fp,"%3dunit(s) %s[%d/%d]\n",apItemCount[iCheckItem],lpItem->getName(),lpItem->m_wBaseItem,lpItem->m_dwSerial);
	}

	delete [] uncompress;
	delete [] uncompressdata;
	delete [] apItemCount;

	fclose(fp);

	return	iCount;
}
