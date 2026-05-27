#include "cMAIN.H"
#include "cMESSAGE.H"
#include "cSET.H"
#include "cDRAW.H"
#include "cITEM.H"
#include "cTEXT.H"
#include "cITEM_PREFIX_EFFECT.H"
#include "cITEM_EFFECT.H"
#include "cPROJECT.H"
#include "resource.h"

//
//	유니크 아이템 스크립트 문장(?)을 해석 한다.
BOOL
cSET_BASIC_ITEM::DecodeUniqueScriptFile(HWND _hDlg)
{
	char	strFn[512]	=	"";
	char	strFolder[512] = "";

	if (!cFILE::LoadBox(strFn,"텍스트 화일 {*.txt}\0*.txt\0\0","txt",NULL,"유니크 아이템 스크립트 데이터 화일 불러오기"))
	{
		SetLogFolder();
		return	FALSE;
	}

	if (cBASIC_ITEM::GetUniqueCount() > 0)
		if (cMSG::YESNO(_ms("'%s' 화일을 디코딩 합니다.",strFn),"디코딩 하기 전에 기존의 유니크 아이템 데이터를 초기화 시키시겠습니까?")==IDYES)
			cBASIC_ITEM::ResetUniqueItem();

	if (!DecodeUniqueScriptFile(_hDlg,strFn))	return	FALSE;

	return	TRUE;
}	//	cSET_BASIC_ITEM::DecodeUniqueScript(HWND _hDlg)

//
//화일을 분석해서 유니크 아이템으로 등록한다.
BOOL
cSET_BASIC_ITEM::DecodeUniqueScriptFile(HWND _hDlg,char *_strFName)
{
	cDECODER	decoder;

	if	(!decoder.Upload(_strFName,NULL))
		return	FALSE;

	int		iCount	=	cBASIC_ITEM::s_iCount,iErrorCount=0,iOverlappedItemCount=0;
	BOOL	isCry	=	TRUE;

	while(1)
	{
		char	*strOneLine	=	decoder.GetStream(13);

		if	(!strOneLine)
			break;

		if	(!DecodeUniqueScript(strOneLine))//,isCry))
		{	
			iErrorCount++;

			if (iErrorCount ==	3 )
			{
				cMSG::Put("유니크 데이터 화일 분석","이후의 에러는 표시 하지 않습니다.");
				isCry	=	FALSE;
			}

			continue;
		}

/*
		if (s_makeItem.m_iSerial	==	dVALID_ITEM)	continue;

		if (cBASIC_ITEM::Add(&s_makeItem)	==	0xffffffff)	
		{
			cMSG::Put("유니크 데이터 추가 에러!!","다음 아이템 이름이 중복 됩니다.\n\n%s",s_makeItem.m_strName);
			iOverlappedItemCount++;
			iErrorCount++;
			continue;
		}*/
	}

	ResetItemListBox(_hDlg,TRUE);

	DrawItemList(_hDlg);

	cMSG::Put("유니크 아이템 데이터 화일 분석 결과","%d개의 유니크 아이템을 추가 했습니다.\n\n%d개의 아이템에서 문제가 발생했습니다.\n\n%d개의 아이템 이름이 중복 되었습니다.",cBASIC_ITEM::s_iCount-iCount,iErrorCount,iOverlappedItemCount);

	return	TRUE;
}	//	cSET_BASIC_ITEM::DecodeUniqueScriptFile(char *_strFName)

//
//	스크립트 문장(?)을 해석 한다.
BOOL
cSET_BASIC_ITEM::DecodeUniqueScript2(char *_strScript)
{
	cDECODER	decoder;

	s_makeItem.reset();

	if (strlen(_strScript)	<=	80)	return	TRUE;

	decoder.UploadBuffer(_strScript,NULL);

	char	*lpStream,strName[64];
	int		iValidCount	=	0;
	int		iGrade		=	0;
	int		iValueIndex	=	0;

	cBASIC_ITEM	*lpEditItem;

	while(1)
	{	
		lpStream	=	decoder.GetStream('	');

		if (!lpStream)
		{
			iValidCount++;

			if (iValidCount	>=	20)	break;

			iGrade++;

			continue;
		}

		switch(iGrade)
		{
//			case	0	:	break;	//	인덱스 무시~
			case	1	:	//	이름
			{
				strcpy(strName,lpStream);

				lpEditItem	=	cBASIC_ITEM::Get(strName);

				if	(lpEditItem	&&	lpEditItem->m_iSerial == 314)
					MSGOUT("a");

				if	(!lpEditItem)
					ERRMSG("asd",lpStream);
				break;
			}

//			case	2	:	break;	//	영문 이름

			case	3	:	//	기본 아이템 이름
			{
				cBASIC_ITEM	*lpItem	=	cBASIC_ITEM::Get(lpStream);
				cBASIC_ITEM	item;

				if	(!lpItem	)
					ERRMSG("asd",lpStream);
				if	(!lpItem )
					return	FALSE;

				lpEditItem->m_wDurability	=	lpItem->m_wDurability;
/*
				item.copy(lpItem);
				item.setComment(lpEditItem->m_pItemComment);
				item.m_wSpeed					=	lpEditItem->m_wSpeed;
				item.m_wRange					=	lpEditItem->m_wRange;
				item.m_iSerial					=	lpEditItem->m_iSerial;

				item.m_wEquipUseLimitMethod		=	0xffff;
				item.m_wEquipUseLimitContents	=	0xffff;

				strcpy(item.m_strName,lpEditItem->m_strName);
//				memcpy(item.m_aUniqueData,lpEditItem->m_aUniqueData,sizeof(item.m_aUniqueData));

				lpEditItem->copy(&item);*/
				break;
			}

			case	5	:
			case	7	:
			case	9	:
			case	11	:
			case	13	:
			case	15	:
			{
				int	iIndex	=	atoi(lpStream);

				cITEM_PREFIX	*lpPrefix	=	g_itemPrefix.getIndexByDiscernmentCode(iIndex);

				if	(!lpPrefix)
					return	ERRMSG("유니크 아이템 스크립트 분석 에러!!","인식번호 %d를 갖는 접두사가 없습니다.",iIndex);

				lpStream	=	decoder.GetStream('	');
				int	iValue	=	atoi(lpStream);

				lpEditItem->m_aUniqueData[iValueIndex].m_wEffect	=	lpPrefix->m_wType;
				lpEditItem->m_aUniqueData[iValueIndex].m_aValue[0]	=	iValue;

				if (lpPrefix->m_wType	==	eIP_JOB_SKILL_LEVEL)
					lpEditItem->m_aUniqueData[iValueIndex].m_aValue[1]	=	lpPrefix->m_aValue[1][0];

				if (strstr(lpStream,"max"))
					lpPrefix->getMaxValue(lpEditItem->m_aUniqueData[iValueIndex].m_aValue);

				if (strstr(lpStream,","))
				{
					lpStream	=	strstr(lpStream,",");
					iValue		=	atoi(lpStream+1);
					lpEditItem->m_aUniqueData[iValueIndex].m_aValue[1]	=	iValue;
				}
				else
				if (strstr(lpStream,"/"))
				{
					lpStream	=	strstr(lpStream,"/");

					lpEditItem->m_aUniqueData[iValueIndex].m_aValue[0]	=	atoi(lpStream+1);
					lpEditItem->m_aUniqueData[iValueIndex].m_aValue[1]	=	iValue;
				}

				iGrade++;
				iValueIndex++;
				break;
			}

			case	17	:	//	공격 속도
			{
				lpEditItem->m_wSpeed	=	StringToNumber(lpStream,2,2);
				break;
			}

			case	18	:	//	사거리
			{
				lpEditItem->m_wRange	=	atoi(lpStream);
				break;
			}

			case	19	:
			{
				char	*lpStr	=	lpStream+strlen("[아이템가격]*");
				int		iValue	=	atoi(lpStr);

				lpEditItem->m_wPriceValue	=	iValue*100;
				break;
			}

			case	20	:
			{
				lpEditItem->m_wRequireLevel	=	atoi(lpStream);
				break;
			}

			case	21	:
			{
				lpEditItem->m_wDropLevel	=	atoi(lpStream);
				break;
			}
			case	22	:
			{
				lpEditItem->m_wCorrectDropChance	=	StringToNumber(lpStream,1000,1);
				break;
			}
		}

		iGrade++;
	}

	return	TRUE;
}