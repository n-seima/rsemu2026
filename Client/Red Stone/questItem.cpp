#include "CQuest.h"
#include "CItem.h"

char*
CQuestItem::getExplain(int _iStatus,BOOL _bIsChecker)
{
	if (_bIsChecker && m_aInfo[_iStatus].m_pstrChecker)
		return	m_aInfo[_iStatus].m_pstrChecker;

	return	m_aInfo[_iStatus].m_pstrText;
}

char*
CQuestItem::getValueString(int _iStatus)
{
	cQuestInfo	*lpInfo	=	&m_aInfo[_iStatus];

	if (lpInfo->m_bf1IsClearValue)
		return	NULL;

	if (lpInfo->m_pstrQuestValue)
		return	lpInfo->m_pstrQuestValue;

	if (m_strQuestValueComment[0])
		return	m_strQuestValueComment;

	return	NULL;
}

char*
CQuestItem::getClientName(int _iStatus)
{
	cQuestInfo	*lpInfo	=	&m_aInfo[_iStatus];

	if	(lpInfo->m_bf1IsClearClient)
		return	NULL;

	if	(lpInfo->m_pstrClientName)
		return	lpInfo->m_pstrClientName;

	if	(m_strQuestClientInfo[0])
		return	m_strQuestClientInfo;

	return	NULL;
}

int
CQuestItem::getClientNpcSerial(int _iStatus)
{
	return	m_aInfo[_iStatus].m_wClientNpcSerial;
}

int
CQuestItem::getClientField(int _iStatus)
{
	cQuestInfo	*lpInfo	=	&m_aInfo[_iStatus];

	return	lpInfo->m_wClientField;
}

BOOL
CQuestItem::isNpcAboutProcessQuest(char *_lpstrName,int _iProcessLevel,int _iField,int _iNpcActorSerial)
{
	if	(_iProcessLevel	>=	dQUEST_STATUS_COUNT)
		return	FALSE;

	cQuestInfo	*lpInfo	=	&m_aInfo[_iProcessLevel];

	for (int i=0;i<lpInfo->m_wLinkNpcCount;i++)
	{
		if	(lpInfo->m_pNpcList[i].m_wField	!=	_iField)
			continue;

		if	(lpInfo->m_pNpcList[i].m_wNpcSerial	!=	0xffff)
		{
			if	(lpInfo->m_pNpcList[i].m_wNpcSerial == _iNpcActorSerial)
				return	TRUE;

			continue;
		}

		if	(lpInfo->m_pNpcList[i].m_pStrName && STRICMP(lpInfo->m_pNpcList[i].m_pStrName,_lpstrName)	==	0)
			return	TRUE;
	}

	return	FALSE;
}

BOOL
CQuestItem::isMonsterAboutProcessQuest(char *_lpstrName,int _iProcessLevel,int _iField,int _iNpcActorSerial)
{
	cQuestInfo	*lpInfo	=	&m_aInfo[_iProcessLevel];

	for (int i=0;i<lpInfo->m_wTargetMonsterCount;i++)
	{
		if	(lpInfo->m_pTargetMonsterList[i].m_wField	!=	_iField)
			continue;

		if	(lpInfo->m_pTargetMonsterList[i].m_wNpcSerial	!=	0xffff)
		{
			if	(lpInfo->m_pTargetMonsterList[i].m_wNpcSerial == _iNpcActorSerial)
				return	TRUE;

			continue;
		}

		if	(STRICMP(lpInfo->m_pTargetMonsterList[i].m_pStrName,_lpstrName)	==	0)
			return	TRUE;
	}

	return	FALSE;
}

void
CQuestItem::cQuestInfo::copy(CQuestItem::cQuestTextForDecode *_lpQuestText)
{
	int	i;

	if (_lpQuestText->m_wLinkFieldCount)
	{
		m_pLinkField	=	new WORD [_lpQuestText->m_wLinkFieldCount];
		m_wLinkFieldCount=	_lpQuestText->m_wLinkFieldCount;
		memcpy(m_pLinkField,_lpQuestText->m_awLinkField,sizeof(WORD)*_lpQuestText->m_wLinkFieldCount);
	}
	if (_lpQuestText->m_wRequireItemCount)
	{
		m_pRequireItemList	=	new WORD [_lpQuestText->m_wRequireItemCount*2];
		m_wRequireItemCount	=	_lpQuestText->m_wRequireItemCount;
		memcpy(m_pRequireItemList,_lpQuestText->m_awRequireItemList,sizeof(WORD)*2*_lpQuestText->m_wRequireItemCount);
	}
	if (_lpQuestText->m_wLinkNpcCount)
	{
		m_pNpcList			=	new cTargetInfo [_lpQuestText->m_wLinkNpcCount];
		m_wLinkNpcCount		=	_lpQuestText->m_wLinkNpcCount;

		for (i=0;i<m_wLinkNpcCount;i++)
			m_pNpcList[i].copy(&_lpQuestText->m_aNpcList[i]);
	}
	if (_lpQuestText->m_wTargetMonsterCount)
	{
		m_pTargetMonsterList=	new cTargetInfo [_lpQuestText->m_wTargetMonsterCount];
		m_wTargetMonsterCount=	_lpQuestText->m_wTargetMonsterCount;

		for (i=0;i<m_wTargetMonsterCount;i++)
			m_pTargetMonsterList[i].copy(&_lpQuestText->m_aTargetMonsterList[i]);
	}
}

BOOL
CQuestItem::setExplain(int _iStatus,char *_lpstrText)
{
	CDecoder			decoder;

	int					iResult;
	BOOL				bErrored	=	FALSE;
	cQuestInfo			*lpExplain	=	&m_aInfo[_iStatus];
	cQuestTextForDecode	questText;
	BOOL				bIsExistQuestCode	=	FALSE;

	lpExplain->reset();

	decoder.UploadBuffer(_lpstrText);

	while(1)
	{
		char	*lpstrWord	=	decoder.getWord();

		if (STRICMP(lpstrWord,dDC_WORD_FINISH)==0 && bIsExistQuestCode)
			continue;

		if (STRICMP(lpstrWord,"field")==0)
		{
			while(1)
			{
				int	iField			=	decoder.getNumber(&iResult,TRUE);

				if (iResult	!=	eGNR_SUCCED)
					break;

				if (iField	!=	-1)
					questText.m_awLinkField[questText.m_wLinkFieldCount++]	=	iField;
			}

			bIsExistQuestCode	=	TRUE;
			
			continue;
		}
		if (STRICMP(lpstrWord,"client")==0)
		{
			int		iField			=	decoder.getNumber(&iResult);
			char	*lpstrName		=	decoder.readStream('\'');

			lpExplain->m_wClientField	=	iField;
			lpExplain->setClientName(lpstrName);

			bIsExistQuestCode	=	TRUE;
			continue;
		}
		if (STRICMP(lpstrWord,"npc")==0)
		{
			int		iField			=	decoder.getNumber(&iResult);
			char	*lpstrName		=	decoder.readStream('\'');

			questText.m_aNpcList[questText.m_wLinkNpcCount].m_wField	=	iField;
			questText.m_aNpcList[questText.m_wLinkNpcCount].setName(lpstrName);

			questText.m_wLinkNpcCount++;
			bIsExistQuestCode	=	TRUE;
			continue;
		}
		if (STRICMP(lpstrWord,"monster")==0)
		{
			int		iField			=	decoder.getNumber(&iResult);
			char	*lpstrName		=	decoder.readStream('\'');

			questText.m_aTargetMonsterList[questText.m_wTargetMonsterCount].m_wField	=	iField;
			questText.m_aTargetMonsterList[questText.m_wTargetMonsterCount].setName(lpstrName);

			questText.m_wTargetMonsterCount++;

			bIsExistQuestCode	=	TRUE;
			continue;
		}
		if (STRICMP(lpstrWord,"item")==0)
		{
			int		iCount			=	decoder.getNumber(&iResult);
			char	*lpstrItem		=	decoder.readStream('\'');
			int		iItem			=	-1;

			if	(IsNumberText(lpstrItem))
				iItem	=	atoi(lpstrItem);
			else
				iItem	=	cItem::GetItemIndexByName(lpstrItem);

			char	*strArbeit[2]	=	{"quest","arbeit"};

			if	(iItem	==	0xffff)
			{
				g_eh.addStaticLog("can not find item '%s' in [%s %.3d %s]",lpstrItem,strArbeit[m_wQuestType],m_wSerial,m_strName);
				bErrored			=	TRUE;
			}
			else
			if	(iItem	>	g_iBasicItemCount)
			{
				cMSG::Error("Error!! in load item","can not find Item index [%d] in Quest [%s]",iItem,m_strName);

				return	FALSE;				
			}
			else
			if (iResult	!=	eGNR_SUCCED)
			{
				g_eh.addStaticLog("can not find item '%s' in [%s %.3d %s]",lpstrItem,strArbeit[m_wQuestType],m_wSerial,m_strName);
				bErrored			=	TRUE;
			}
			else
			{
				questText.m_awRequireItemList[questText.m_wRequireItemCount][0]	=	iItem;
				questText.m_awRequireItemList[questText.m_wRequireItemCount][1]	=	iCount;

				questText.m_wRequireItemCount++;
			}

			bIsExistQuestCode	=	TRUE;
			continue;
		}
		if (STRICMP(lpstrWord,"value")==0)
		{
			char	*lpstrValue		=	decoder.readStream('\'');

			lpExplain->setValueComment(lpstrValue);
			bIsExistQuestCode	=	TRUE;
			continue;
		}
		if (STRICMP(lpstrWord,"countDown")==0)
		{
			char	*lpstrValue		=	decoder.readStream('\'');

			lpExplain->setCountdownComment(lpstrValue);
			bIsExistQuestCode	=	TRUE;
			continue;
		}
		if (STRICMP(lpstrWord,"checker")==0)
		{
			char	*lpstrChecker	=	decoder.readStream('\'');

			lpExplain->setCheckerComment(lpstrChecker);
			bIsExistQuestCode	=	TRUE;
			continue;
		}
		if (STRICMP(lpstrWord,"clearClient")==0)
		{
			lpExplain->m_bf1IsClearClient	=	TRUE;
			bIsExistQuestCode	=	TRUE;
			continue;
		}
		if (STRICMP(lpstrWord,"clearValue")==0)
		{
			lpExplain->m_bf1IsClearValue	=	TRUE;
			bIsExistQuestCode	=	TRUE;
			continue;
		}
		if (STRICMP(lpstrWord,"_codeEnd")==0)
		{
			decoder.findCharOver(0x0a);

			lpExplain->setText(decoder.get());

			lpExplain->copy(&questText);

			if (bErrored)
				return	ERROR("error in load Quest","please check 'log.txt' file",lpstrWord);

			break;
		}

		if (bIsExistQuestCode)
			return	ERROR("error in load Quest","is incorrect quest keyword '%s'",lpstrWord);

		lpExplain->setText(_lpstrText);

		return	TRUE;
	}

	return	TRUE;
}

BOOL
CQuestItem::isEnableJob(int _iJob)
{
	if	(!m_bf1IsMaleJobEquipAble && c_aIsMale[_iJob])
		return	FALSE;
	if	(!m_bf1IsFemaleJobEquipAble && !c_aIsMale[_iJob])
		return	FALSE;

	UINT	uiCheckMask	=	1<<_iJob;

	if	(m_uiEnableJobMask & uiCheckMask)
		return	TRUE;

	return	FALSE;
}
