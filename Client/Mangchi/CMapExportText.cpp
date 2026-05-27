#include "cMAP.H"
#include "cDRAW.H"
#include "cMAPWND.H"
#include "cTEXT.H"
#include "cBUILDING.H"
#include "cMESSAGE.H"
#include "cPROJECT.H"
#include "cTOOLWND.H"
#include "CShop.H"
#include "localize.H"
#include "cMAIN.H"
#include "debug.H"

CFieldText	l_fieldSrc,l_fieldDest;

BOOL
cMAP::importText(BOOL _bIsSD,BOOL _bIsPart)
{
	char	strFileName[128];

	if	(_bIsSD)
		sprintf(strFileName,"%s/localize/%s/import/(expansion)%s.txt",_logFolder,MAIN.m_strLocalizeFolder,_exportFileName(m_strFName));
	else
		sprintf(strFileName,"%s/localize/%s/import/%s.txt",_logFolder,MAIN.m_strLocalizeFolder,_exportFileName(m_strFName));

	{
		cFILE	file;

		if	(file.Open(strFileName,"rb"))
		{
			BYTE	abCode[2];
			file.Read(abCode,2);
			
			file.Close();

			if	(abCode[0] == 255 && abCode[1] == 254 )
			{
				ERRMSG("","The file %s is Unicode.",strFileName);

				return	FALSE;
			}
		}
	}

	cDECODER	decoder;

	if	(!decoder.Upload(strFileName,NULL))
	{
		if	(_bIsPart)
			return	FALSE;
		
		cMSG::Error("can not find text file",strFileName);

		return	FALSE;
	}

	cACTOR					*lpActor	=	NULL;
	CConversation			*lpConversation;
	CConversationContainer	*lpcc;
	CKarmaContainer			*lpkc;
	cCHARACTER				*lpCharacter;
	cAREA_INFO				*lpArea		=	NULL;

	int		iConversationIndex	=	-1;
	int		iContentsIndex		=	-1;
	int		iActorSerial		=	-1;
	int		iCharacterSerial	=	-1;
	int		iArea				=	-1;
	BOOL	bIsExistCharacterNameError	=	FALSE;

	while(1)
	{
		char	*lpstrWord	=	decoder.GetWord();
		char	*lpstrStream;

		if (stricmp(lpstrWord,"fieldName")==0)
		{
			decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","There is no string after %s\n\nfieldName!",strFileName);

				return	FALSE;
			}

			lpstrStream	=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","There is a garbage value after the %s\n\nfieldName string!",strFileName);

				return	FALSE;
			}

			strcpy(m_strName,lpstrStream);

			continue;
		}

		if (stricmp(lpstrWord,"actor_name")==0)
		{
			decoder.GetNumber();
			iActorSerial=	decoder.GetNumber();

			lpActor		=	&m_setActor.m_aActor[iActorSerial];

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","There is no string after %s\n\nactor_name!\ncode: %d",strFileName,iActorSerial);

				return	FALSE;
			}

			lpstrStream	=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","There is a garbage value after the %s\n\nactor_name string!\n code: %d",strFileName,iActorSerial);

				return	FALSE;
			}

			if	(lpActor->m_iSerial	!=	0xffff)
				ustrncpy(lpActor->m_strName,lpstrStream,16);

			continue;
		}

		if (stricmp(lpstrWord,"actor_speech")==0)
		{
			decoder.GetNumber();

			iActorSerial		=	decoder.GetNumber();
			iConversationIndex	=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","There is no string after %s\n\nactor_speech!\ncode : %d,%d",strFileName,iActorSerial,iConversationIndex);

				return	FALSE;
			}

			lpstrStream			=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","There is a garbage value after the %s\n\nactor_name string!\n code : %d,%d",strFileName,iActorSerial,iConversationIndex);

				return	FALSE;
			}

			lpActor				=	&m_setActor.m_aActor[iActorSerial];

			if	(lpActor->m_iSerial	==	0xffff	||	lpActor->m_pCC	==	NULL	||	iConversationIndex	>=	lpActor->m_pCC->m_wConversationCount)
				continue;

			lpcc				=	lpActor->m_pCC;
			lpConversation		=	&lpcc->m_pConversation[iConversationIndex];
			
			lpConversation->setSpeech(lpstrStream);

			continue;
		}

		if (stricmp(lpstrWord,"actor_contents")==0)
		{
			decoder.GetNumber();

			iActorSerial		=	decoder.GetNumber();
			iConversationIndex	=	decoder.GetNumber();
			iContentsIndex		=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","There is no string after %s\n\nactor_contents!\ncode: %d,%d,%d",strFileName,iActorSerial,iConversationIndex,iContentsIndex);

				return	FALSE;
			}

			lpstrStream			=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","There is a garbage value after the %s\n\nactor_contents string!\n code: %d,%d,%d",strFileName,iActorSerial,iConversationIndex,iContentsIndex);

				return	FALSE;
			}

			lpActor				=	&m_setActor.m_aActor[iActorSerial];

			if	(iActorSerial	==	159	&&	iConversationIndex	==	36)
				iConversationIndex	=	36;

			if	(lpActor->m_iSerial	==	0xffff	||	lpActor->m_pCC	==	NULL	||	iConversationIndex	>=	lpActor->m_pCC->m_wConversationCount)
				continue;

			lpcc				=	lpActor->m_pCC;
			lpConversation		=	&lpcc->m_pConversation[iConversationIndex];

			if	(iContentsIndex		>=	lpConversation->m_wContentsCount)
				continue;

			lpkc				=	&lpConversation->m_pContents[iContentsIndex];

			lpkc->setTitle(lpstrStream);

			continue;
		}

		if (stricmp(lpstrWord,"actor_reaction")==0)
		{
			decoder.GetNumber();

			iActorSerial		=	decoder.GetNumber();
			iConversationIndex	=	decoder.GetNumber();
			iContentsIndex		=	decoder.GetNumber();
			int	iKarma			=	decoder.GetNumber();
			int	iReaction		=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","There is no string after %s\n\nactor_reaction!\ncode: %d,%d,%d,%d,%d",strFileName,iActorSerial,iConversationIndex,iContentsIndex,iKarma,iReaction);

				return	FALSE;
			}

			lpstrStream			=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","There is a garbage value after the %s\n\nactor_reaction string!\n code: %d,%d,%d,%d,%d",strFileName,iActorSerial,iConversationIndex,iContentsIndex,iKarma,iReaction);

				return	FALSE;
			}

			lpActor				=	&m_setActor.m_aActor[iActorSerial];

			if	(lpActor->m_iSerial	==	0xffff	||	lpActor->m_pCC	==	NULL	||	iConversationIndex	>=	lpActor->m_pCC->m_wConversationCount)
				continue;

			lpcc				=	lpActor->m_pCC;
			lpConversation		=	&lpcc->m_pConversation[iConversationIndex];

			if	(iContentsIndex		>=	lpConversation->m_wContentsCount)
				continue;

			lpkc				=	&lpConversation->m_pContents[iContentsIndex];

			if	(iKarma			>=	lpkc->m_wKarmaCount	||	iReaction	>=	lpkc->m_pKarma[iKarma].m_wReactionCount)
				continue;

			CKarmaItem	*lpKI	=	&lpkc->m_pKarma[iKarma].m_pReaction[iReaction];

			lpKI->SetString(lpstrStream);

			continue;
		}

		if (stricmp(lpstrWord,"actor")==0)
		{
			decoder.GetNumber();
			iActorSerial=	decoder.GetNumber();

			lpActor		=	&m_setActor.m_aActor[iActorSerial];

			lpstrStream	=	decoder.GetStream();

			if (lpActor->m_iSerial	!=	0xffff)
			{
				ustrncpy(lpActor->m_strName,lpstrStream,16);

				lpcc		=	lpActor->m_pCC;
			}
			else
				lpActor		=	NULL;
			continue;
		}

		if (stricmp(lpstrWord,"conversation")==0)
		{
			iConversationIndex	=	decoder.GetNumber();

			if (!lpActor)
				continue;

			if (iConversationIndex >= lpcc->m_wConversationCount)
			{
				ERRMSG("cMAP::importText()","iConversationIndex >= lpcc->m_wConversationCount\n\n%s actor %d",strFileName,lpActor->m_iSerial);
				MSGOUT("(iConversationIndex >= lpcc->m_wConversationCount) %s actor [%d]",strFileName,lpActor->m_iSerial);

				lpConversation	=	NULL;

				continue;
			}

			lpConversation	=	&lpcc->m_pConversation[iConversationIndex];

			continue;
		}

		if (stricmp(lpstrWord,"speech")==0)
		{
			decoder.GetNumber();

			lpstrStream	=	decoder.GetStream();

			if	(!lpActor	||	!lpConversation)
				continue;

			lpConversation->setSpeech(lpstrStream);

			continue;
		}

		if (stricmp(lpstrWord,"contents")==0)
		{
			decoder.GetNumber();

			iContentsIndex	=	decoder.GetNumber();

			lpstrStream		=	decoder.GetStream();

			if (!lpActor)
				continue;

			lpkc			=	NULL;

			if (iContentsIndex	<	lpConversation->m_wContentsCount)
			{
				lpkc			=	&lpConversation->m_pContents[iContentsIndex];
				lpkc->setTitle(lpstrStream);
			}

			continue;
		}

		if (stricmp(lpstrWord,"reaction")==0)
		{
			decoder.GetNumber();

			int	iKarma			=	decoder.GetNumber();
			int	iReaction		=	decoder.GetNumber();

			lpstrStream			=	decoder.GetStream();

			if	(!lpActor	|| !lpkc)
				continue;

			if	(iKarma	>=	lpkc->m_wKarmaCount	||	iReaction	>=	lpkc->m_pKarma[iKarma].m_wReactionCount)
				continue;

			CKarmaItem	*lpKI	=	&lpkc->m_pKarma[iKarma].m_pReaction[iReaction];

			lpKI->SetString(lpstrStream);

			continue;
		}

		if (stricmp(lpstrWord,"character")==0)
		{
			iCharacterSerial	=	decoder.GetNumber();
			iActorSerial		=	-1;
			iArea				=	-1;

			lpCharacter			=	&m_setCharacter.m_aCharacter[iCharacterSerial];
			continue;
		}

		if (stricmp(lpstrWord,"character_name")==0)
		{
			iCharacterSerial	=	decoder.GetNumber();
			iActorSerial		=	-1;
			iArea				=	-1;

			int	iCheckSum	=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\nThere is no string after character_name!\nCharacter Serial: %d",strFileName,iCharacterSerial);

				return	FALSE;
			}

			lpstrStream		=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\nThere is a garbage value after the character_name string!\n code: %d",strFileName,iCharacterSerial);

				return	FALSE;
			}


			lpCharacter		=	&m_setCharacter.m_aCharacter[iCharacterSerial];

			if	(strlen(lpstrStream) >= sizeof(lpCharacter->m_strName) && bIsExistCharacterNameError == FALSE)
			{
				cFILE	file;

				file.Open(_ms("c:/%s/character_name_error.txt",_logFolder),"at");

				file.writeStreamCR("%s",m_strFName);

				bIsExistCharacterNameError	=	TRUE;
			}
			ustrncpy(lpCharacter->m_strName,lpstrStream,sizeof(lpCharacter->m_strName)-1);

			continue;
		}

		if (stricmp(lpstrWord,"character_event")==0)
		{
			decoder.GetNumber();

			int	iEvent			=	decoder.GetNumber();
			int	iKarma			=	decoder.GetNumber();
			int	iReaction		=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\nThere is no string after character_event!\nCharacter Serial: %d",strFileName,iCharacterSerial);

				return	FALSE;
			}

			lpstrStream			=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\nThere is a garbage value after the character_event string!\n code: %d",strFileName,iCharacterSerial);

				return	FALSE;
			}

			if (!lpCharacter)
				continue;

			if (iEvent	>=	eMECC_COUNT)
				continue;

			lpkc				=	lpCharacter->m_pEvent[iEvent];

			if (lpkc	==	NULL	||	iKarma	>=	lpkc->m_wKarmaCount)
				continue;

			if (iReaction	>=	lpkc->m_pKarma[iKarma].m_wReactionCount)
				continue;

			CKarmaItem	*lpKI	=	&lpkc->m_pKarma[iKarma].m_pReaction[iReaction];

			lpKI->SetString(lpstrStream);

			continue;
		}

		if (stricmp(lpstrWord,"field_karma_reaction")==0)
		{
			decoder.GetNumber();

			int	iKarma			=	decoder.GetNumber();
			int	iReaction		=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","There is no string after %s\n\nfield_karma_reaction!\ncode : %d,%d",strFileName,iKarma,iReaction);

				return	FALSE;
			}

			lpstrStream			=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\nThere is a garbage value after the field_karma_reaction string!\n code: %d,%d",strFileName,iKarma,iReaction);

				return	FALSE;
			}

			lpkc				=	&m_karma;

			if	(iKarma			>=	lpkc->m_wKarmaCount)
				continue;

			if (iReaction		>=	lpkc->m_pKarma[iKarma].m_wReactionCount)
				continue;

			CKarmaItem	*lpKI	=	&lpkc->m_pKarma[iKarma].m_pReaction[iReaction];

			lpKI->SetString(lpstrStream);

			continue;
		}

		if (stricmp(lpstrWord,"area")==0)
		{
			decoder.GetNumber();

			iArea				=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","There is no string after %s\n\narea!\nCode: %d",strFileName,iArea);

				return	FALSE;
			}

			lpstrStream			=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\nThere is a garbage value after the narea string!\n code: %d",strFileName,iArea);

				return	FALSE;
			}

			iActorSerial		=	-1;
			iCharacterSerial	=	-1;

			ustrncpy(m_pArea->m_aArea[iArea].m_strName,lpstrStream,sizeof(m_pArea->m_aArea[iArea].m_strName)-2);
			lpArea	=	&m_pArea->m_aArea[iArea];
			lpcc	=	&m_pArea->m_aArea[iArea].m_cc;

			continue;
		}

		if (stricmp(lpstrWord,"area_event")==0)
		{
			iConversationIndex	=	decoder.GetNumber();

			if	(!lpArea	||	!lpcc)
				continue;

			if	(iConversationIndex	>=	lpcc->m_wConversationCount)
			{
				lpArea	=	NULL;
				continue;
			}

			lpConversation	=	&lpcc->m_pConversation[iConversationIndex];

			continue;
		}

		if (stricmp(lpstrWord,"area_speech")==0)
		{
			decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","There is no string after %s\n\narea_speech!\nCode: %d",strFileName,iArea);

				return	FALSE;
			}

			lpstrStream	=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","There is a garbage value after the %s\n\narea_speech string!\n code: %d",strFileName,iArea);

				return	FALSE;
			}

			if	(!lpArea	||	!lpcc	||	!lpConversation)
				continue;

			lpConversation->setSpeech(lpstrStream);

			continue;
		}

		if (stricmp(lpstrWord,"area_contents")==0)
		{
			decoder.GetNumber();

			iContentsIndex	=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","There is no string after %s\n\narea_contents!\nCode: %d,%d",strFileName,iArea,iContentsIndex);

				return	FALSE;
			}


			lpstrStream		=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","There is a garbage value after the %s\n\narea_contents string!\n code: %d",strFileName,iArea,iContentsIndex);

				return	FALSE;
			}

			if (!lpArea)
				continue;

			lpkc			=	NULL;

			if (iContentsIndex	<	lpConversation->m_wContentsCount)
			{
				lpkc			=	&lpConversation->m_pContents[iContentsIndex];
				lpkc->setTitle(lpstrStream);
			}

			continue;
		}

		if (stricmp(lpstrWord,"area_reaction")==0)
		{
			decoder.GetNumber();

			int	iKarma			=	decoder.GetNumber();
			int	iReaction		=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","There is no string after %s\n\narea_reaction!\nCode: %d,%d,%d",strFileName,iArea,iKarma,iReaction);

				return	FALSE;
			}

			lpstrStream			=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\narea_reaction There is a garbage value after the string!\n code: %d",strFileName,iArea,iKarma,iReaction);

				return	FALSE;
			}

			if (!lpArea	|| !lpkc)
				continue;

			if	(iKarma	>=	lpkc->m_wKarmaCount	||	iReaction	>=	lpkc->m_pKarma[iKarma].m_wReactionCount)
				continue;

			CKarmaItem	*lpKI	=	&lpkc->m_pKarma[iKarma].m_pReaction[iReaction];

			lpKI->SetString(lpstrStream);

			continue;
		}

		if (stricmp(lpstrWord,"shop")==0)
		{
			decoder.GetNumber();

			int	iShop			=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","There is no string after %s\n\nshop!\nshop Serial: %d",strFileName,iShop);

				return	FALSE;
			}

			lpstrStream			=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\nThere is a garbage value after the shop string!\n code: %d",strFileName,iShop);

				return	FALSE;
			}

			strcpy(m_shop.m_aShop[iShop].m_strName,lpstrStream);

			continue;
		}

		if (stricmp(lpstrWord,"isEndCode")==0)
			break;

		if	(iActorSerial	!=	-1)
			ERRMSG("","%s\n\n%s\nActor Serial : %d\nConversation Index %d\nContents Index : %d",strFileName,lpstrWord,iActorSerial,iConversationIndex,iContentsIndex);
		if	(iCharacterSerial	!=	-1)
			ERRMSG("","%s\n\n%s\nCharacter Serial : %d\nConversation Index %d\nContents Index : %d",strFileName,lpstrWord,iCharacterSerial,iConversationIndex,iContentsIndex);
		if	(iArea	!=	-1)
			ERRMSG("","%s\n\n%s\nArea Serial : %d\nConversation Index %d\nContents Index : %d",strFileName,lpstrWord,iArea,iConversationIndex,iContentsIndex);

		return	FALSE;
	}

	return	TRUE;
}

void
cMAP::exportText(BOOL _bIsSD,BOOL _bIsExportSomeMap)
{
	int		i,j;
	cFILE	file;
	char	strFileName[128];
	char	*lpstrTextFolder	=	"lastest";

	if	(_bIsExportSomeMap)
		lpstrTextFolder	=	"some_map";

	if	(_bIsSD)
		sprintf(strFileName,"%s/localize/%s/(expansion)%s.txt",_logFolder,lpstrTextFolder,_exportFileName(m_strFName));
	else
		sprintf(strFileName,"%s/localize/%s/%s.txt",_logFolder,lpstrTextFolder,_exportFileName(m_strFName));

	file.Open(strFileName,"wb");

	RemoveQuotationMark(m_strName);

	file.writeStreamCR("#fieldName [%d] \"%s\"",GetTextCheckSum(m_strName),m_strName);
	file.writeStreamCR(" ");

	{
		CKarmaContainer	*lpkc	=	&m_karma;

		for (int iKarma=0;iKarma<lpkc->m_wKarmaCount;iKarma++)
		{
			CKarma	*lpKarma	=	&lpkc->m_pKarma[iKarma];

			for (int iReaction=0;iReaction<lpKarma->m_wReactionCount;iReaction++)
			{
				CKarmaItem	*lpKI	=	&lpKarma->m_pReaction[iReaction];

				if (lpKI->m_wStringSize)
				{
					RemoveQuotationMark(lpKI->m_strString);
					file.writeStreamCR("#field_karma_reaction [%d] %d,%d \"%s\"",GetTextCheckSum(lpKI->m_strString),iKarma,iReaction,lpKI->m_strString);
				}
			}
		}
	}

	file.writeStreamCR(" ");

	{
		for (i=0;i<dMAX_ACTOR_COUNT;i++)
		{
			if (m_setActor.m_aActor[i].m_iSerial == 0xffff)
				continue;

			RemoveQuotationMark(m_setActor.m_aActor[i].m_strName);

			file.writeStreamCR("#actor_name [%d] %d \"%s\"",GetTextCheckSum(m_setActor.m_aActor[i].m_strName),i,m_setActor.m_aActor[i].m_strName);

			CConversationContainer	*lpcc	=	m_setActor.m_aActor[i].m_pCC;

			if (!lpcc)
			{
				file.writeStreamCR(" ");
				continue;
			}

			for (j=0;j<lpcc->m_wConversationCount;j++)
			{
				CConversation	*lpSpeech	=	&lpcc->m_pConversation[j];

				if (lpSpeech->m_pstrSpeech)
				{
					RemoveQuotationMark(lpSpeech->m_pstrSpeech);
					file.writeStreamCR("	#actor_speech [%d] %d,%d \"%s\"",GetTextCheckSum(lpSpeech->m_pstrSpeech),i,j,lpSpeech->m_pstrSpeech);
				}

				for (int iContent=0;iContent<lpSpeech->m_wContentsCount;iContent++)
				{
					CKarmaContainer	*lpkc	=	&lpSpeech->m_pContents[iContent];

					if (lpkc->m_wTitleLength)
					{
						RemoveQuotationMark(lpkc->m_pstrTitle);
						file.writeStreamCR("		#actor_contents	[%d] %d,%d,%d \"%s\"",GetTextCheckSum(lpkc->m_pstrTitle),i,j,iContent,lpkc->m_pstrTitle);
					}

					for (int iKarma=0;iKarma<lpkc->m_wKarmaCount;iKarma++)
					{
						CKarma	*lpKarma	=	&lpkc->m_pKarma[iKarma];

						for (int iReaction=0;iReaction<lpKarma->m_wReactionCount;iReaction++)
						{
							CKarmaItem	*lpKI	=	&lpKarma->m_pReaction[iReaction];

							if (lpKI->m_wStringSize)
							{
								RemoveQuotationMark(lpKI->m_strString);
								file.writeStreamCR("			#actor_reaction	[%d] %d,%d,%d,%d,%d \"%s\"",GetTextCheckSum(lpKI->m_strString),i,j,iContent,iKarma,iReaction,lpKI->m_strString);
							}
						}
					}
				}
			}

			file.writeStreamCR("  ");
		}
	}

	{
		for (i=0;i<dMAX_CHARACTER;i++)
		{
			cCHARACTER		*lpCharacter	=	&m_setCharacter.m_aCharacter[i];
			CKarmaContainer	*lpkc			=	NULL;

			if (lpCharacter->m_wSerial	==	0xffff)
				continue;

			file.writeStreamCR("#character_name %d [%d] \"%s\"",i,GetTextCheckSum(lpCharacter->m_strName),lpCharacter->m_strName);

			for (int iEventCount=0;iEventCount<eMECC_COUNT;iEventCount++)
			{
				lpkc	=	lpCharacter->m_pEvent[iEventCount];

				if (lpkc	==	NULL)
					continue;

				for (int iKarma=0;iKarma<lpkc->m_wKarmaCount;iKarma++)
				{
					CKarma	*lpKarma	=	&lpkc->m_pKarma[iKarma];

					for (int iReaction=0;iReaction<lpKarma->m_wReactionCount;iReaction++)
					{
						CKarmaItem	*lpKI	=	&lpKarma->m_pReaction[iReaction];

						if (lpKI->m_wStringSize)
						{
							RemoveQuotationMark(lpKI->m_strString);

							file.writeStreamCR("    #character_event [%d] %d,%d,%d \"%s\"",GetTextCheckSum(lpKI->m_strString),iEventCount,iKarma,iReaction,lpKI->m_strString);
						}
					}
				}
			}

			file.writeStreamCR("  ");
		}
	}

	for (i = 0;i<dMAX_AREA;i++)
	{
		cAREA_INFO *lpArea	=	&m_pArea->m_aArea[i];

		if	(lpArea->m_wSerial	==	0xffff)
			continue;

		if	(lpArea->m_wKind	!=	dAREA_HUNTING_AREA	&&
			lpArea->m_wKind		!=	dAREA_NAMED_AREA	&&	lpArea->m_cc.m_wConversationCount	==	0)
			continue;

		RemoveQuotationMark(lpArea->m_strName);

		file.writeStreamCR("#area [%d] %d \"%s\"",GetTextCheckSum(lpArea->m_strName),i,lpArea->m_strName);

		CConversationContainer	*lpcc	=	&lpArea->m_cc;

		if (lpcc->m_wConversationCount	==	0)
		{
			file.writeStreamCR(" ");
			continue;
		}

		for (j=0;j<lpcc->m_wConversationCount;j++)
		{
			CConversation	*lpSpeech	=	&lpcc->m_pConversation[j];

			file.writeStreamCR("  #area_event %d",j);

			if (lpSpeech->m_pstrSpeech)
			{
				RemoveQuotationMark(lpSpeech->m_pstrSpeech);
				file.writeStreamCR("    #area_speech [%d] \"%s\"",GetTextCheckSum(lpSpeech->m_pstrSpeech),lpSpeech->m_pstrSpeech);
			}

			for (int iContent=0;iContent<lpSpeech->m_wContentsCount;iContent++)
			{
				CKarmaContainer	*lpkc	=	&lpSpeech->m_pContents[iContent];

				if (lpkc->m_wTitleLength)
				{
					RemoveQuotationMark(lpkc->m_pstrTitle);
					file.writeStreamCR("    #area_contents [%d] %d \"%s\"",GetTextCheckSum(lpkc->m_pstrTitle),iContent,lpkc->m_pstrTitle);
				}

				for (int iKarma=0;iKarma<lpkc->m_wKarmaCount;iKarma++)
				{
					CKarma	*lpKarma	=	&lpkc->m_pKarma[iKarma];

					for (int iReaction=0;iReaction<lpKarma->m_wReactionCount;iReaction++)
					{
						CKarmaItem	*lpKI	=	&lpKarma->m_pReaction[iReaction];

						if (lpKI->m_wStringSize)
						{
							RemoveQuotationMark(lpKI->m_strString);
							file.writeStreamCR("        #area_reaction [%d] %d,%d \"%s\"",GetTextCheckSum(lpKI->m_strString),iKarma,iReaction,lpKI->m_strString);
						}
					}
				}
			}
		}

		file.writeStreamCR(" ");
	}

	for (i = 0;i<dMAX_SHOP;i++)
	{
		CShop	*lpShop	=	&m_shop.m_aShop[i];

		if (lpShop->m_wSerial	==	0xffff)
			continue;

		RemoveQuotationMark(lpShop->m_strName);

		file.writeStreamCR("#shop [%d] %d \"%s\"",GetTextCheckSum(lpShop->m_strName),i,lpShop->m_strName);
	}

	file.Close();
	//	상점
}


void
cMAP::exportNpcInfoWithDialog()
{
	int		i,j;
	cFILE	file;
	char	strFileName[128];

	sprintf(strFileName,"%s/%s.txt",_logFolder,_exportFileName(m_strFName));

	file.Open(strFileName,"wb");

	RemoveQuotationMark(m_strName);

	file.writeStreamCR("필드명 \"%s\"",m_strName);
	file.writeStreamCR(" ");

	file.writeStreamCR(" ");

	for (i=0;i<dMAX_ACTOR_COUNT;i++)
	{
		if	(m_setActor.m_aActor[i].m_iSerial == 0xffff)
			continue;

		cACTOR	*lpActor	=	&m_setActor.m_aActor[i];

		RemoveQuotationMark(lpActor->m_strName);

		file.writeStreamCR("NPC 이름	\"%s\"	[%s]",lpActor->m_strName,g_strNpcCategorty[lpActor->m_wType]);

		CConversationContainer	*lpcc	=	m_setActor.m_aActor[i].m_pCC;

		if (!lpcc)
		{
			file.writeStreamCR(" ");
			continue;
		}

		for (j=0;j<lpcc->m_wConversationCount;j++)
		{
			CConversation	*lpSpeech	=	&lpcc->m_pConversation[j];

			if (lpSpeech->m_pstrSpeech)
			{
				RemoveQuotationMark(lpSpeech->m_pstrSpeech);
				file.writeStreamCR("	대사	\"%s\"",lpSpeech->m_pstrSpeech);
			}

			for (int iContent=0;iContent<lpSpeech->m_wContentsCount;iContent++)
			{
				CKarmaContainer	*lpkc	=	&lpSpeech->m_pContents[iContent];

				if (lpkc->m_wTitleLength)
				{
					RemoveQuotationMark(lpkc->m_pstrTitle);
					file.writeStreamCR("		선택문	\"%s\"",lpkc->m_pstrTitle);
				}
			}
		}

		file.writeStreamCR("  ");
	}

	file.Close();

	cMSG::Put("Extract NPC information (including dialog)", "Done\n\nFile: %s", strFileName);
}

BOOL
cMAP::UploadTextForCompare(char *_lpstrFileName,void *_lpText)
{
	if	(!_lpText)
		_lpText	=	&l_fieldSrc;
		
	char	strFileName[1024];

	CFieldText	*lpText	=	(CFieldText	*)_lpText;

	sprintf(strFileName,_lpstrFileName);

	cDECODER	decoder;

	{
		cFILE	file;

		if	(file.Open(strFileName,"rb"))
		{
			BYTE	abCode[2];
			file.Read(abCode,2);
			
			file.Close();

			if	(abCode[0] == 255 && abCode[1] == 254 )
			{
				ERRMSG("","The file %s is Unicode.",strFileName);

				return	FALSE;
			}
		}
	}

	if	(!decoder.Upload(strFileName,NULL))
		return FALSE;

	lpText->reset();

	CActorText			*lpActor	=	NULL;
	CConversationText	*lpConversation;
	CContentsText		*lpContents;
	cCharacterText		*lpCharacter=	NULL;
	CActorText			*lpArea		=	NULL;
	CKarmaText			*lpKarmaText=	NULL;
	char				*lpstrStream;

	int					iActorSerial	=	-1,iCharacterSerial	=	-1,iConversationIndex	=	-1,iContentsIndex	=	-1;
	int					iAreaSerial	=	-1;

	while(1)
	{
		char	*lpstrWord	=	decoder.GetWord();

		if (stricmp(lpstrWord,"fieldName")==0)
		{
			lpText->m_iFieldNameCheckSum	=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","There is no string after %s\n\nfieldName!",strFileName);

				return	FALSE;
			}


			lpText->m_string.Set(decoder.GetStream());

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","There is a garbage value after the %s\n\nfieldName string",strFileName);

				return	FALSE;
			}

			continue;
		}

		if (stricmp(lpstrWord,"character")==0)
		{
			iCharacterSerial	=	decoder.GetNumber();

			iActorSerial	=	-1,iAreaSerial	=	-1;

			lpCharacter			=	&lpText->m_aCharacter[iCharacterSerial];
			
			if	(lpCharacter->m_iCheckSum	!=	0x12345678)
				ERRMSG("","%s\n\nCharacter Serial : %d\nAlready exist Character Serial",strFileName,iCharacterSerial);

			continue;
		}

		if (stricmp(lpstrWord,"character_name")==0)
		{
			iActorSerial	=	-1,iAreaSerial	=	-1;

			iCharacterSerial		=	decoder.GetNumber();
			int		iCheckSum		=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\nThere is no string after character_name!\ncode: %d",strFileName,iCharacterSerial);

				return	FALSE;
			}

			char	*lpstrStream	=	decoder.GetStream();


			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\nThere is a garbage value after the character_name string!\n code: %d",strFileName,iCharacterSerial);

				return	FALSE;
			}

			lpCharacter				=	&lpText->m_aCharacter[iCharacterSerial];

			lpCharacter->m_strName.Set(lpstrStream);
			lpCharacter->m_iCheckSum=	iCheckSum;

			continue;
		}

		if (stricmp(lpstrWord,"character_event")==0)
		{
			int				iCheckSum	=	decoder.GetNumber();
			int				iEvent		=	decoder.GetNumber();
			int				iKarma		=	decoder.GetNumber();
			int				iReaction	=	decoder.GetNumber();

			if	(lpCharacter->m_apContents[iEvent]==	NULL)
				lpCharacter->m_apContents[iEvent]	=	new	CContentsText();

			if	(lpCharacter->m_apContents[iEvent]->m_apKarma[iKarma]	==	NULL)
				lpCharacter->m_apContents[iEvent]->m_apKarma[iKarma]	=	new	cKarmaText();

			if	(lpCharacter->m_apContents[iEvent]->m_apKarma[iKarma]->m_apReaction[iReaction]	==	NULL)
				lpCharacter->m_apContents[iEvent]->m_apKarma[iKarma]->m_apReaction[iReaction]	=	new	CReactionText();


			CReactionText	*lpReaction	=	lpCharacter->m_apContents[iEvent]->m_apKarma[iKarma]->m_apReaction[iReaction];

			lpReaction->m_iCheckSum		=	iCheckSum;
			lpReaction->m_iKarma		=	iKarma;
			lpReaction->m_iEvent		=	iEvent;

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","%s\n\nThere is no string after character_event!\ncode: %d,%d,%d,%d",strFileName,iCharacterSerial,iEvent,iKarma,iReaction);

				return	FALSE;
			}

			char	*lpstrText			=	decoder.GetStream();

			lpReaction->m_string.Set(lpstrText);

			continue;
		}

		if (stricmp(lpstrWord,"field_karma_reaction")==0)
		{
			int	iCheckSum		=	decoder.GetNumber();
			int	iKarma			=	decoder.GetNumber();
			int	iReaction		=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","There is no string after %s\n\nfield_karma_reaction!\ncode: %d,%d",strFileName,iKarma,iReaction);

				return	FALSE;
			}

			char	*lpstrStream=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\nThere is a garbage value after the field_karma_reaction string!\n code: %d,%d",strFileName,iKarma,iReaction);

				return	FALSE;
			}

			lpText->m_aKarma[iKarma].m_apReaction[iReaction]	=	new	CReactionText();

			CReactionText	*lpReaction			=	lpText->m_aKarma[iKarma].m_apReaction[iReaction];

			lpReaction->m_iCheckSum				=	iCheckSum;
			lpReaction->m_iKarma				=	iKarma;
			lpReaction->m_string.Set(lpstrStream);
			lpText->m_aKarma[iKarma].m_iCheckSum	=	1;

			continue;
		}

		if (stricmp(lpstrWord,"actor_name")==0)
		{
			int	iActorCheckSum	=	decoder.GetNumber();
			iActorSerial		=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","There is no string after %s\n\nactor_name!\nActor Serial: %d",strFileName,iActorSerial);

				return	FALSE;
			}

			lpstrStream			=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","There is a garbage value after the %s\n\nactor_name string!\n code: %d",strFileName,iActorSerial);

				return	FALSE;
			}

			lpActor				=	&lpText->m_aActor[iActorSerial];
			lpActor->m_iCheckSum=	iActorCheckSum;

			lpActor->m_string.Set(lpstrStream);

			continue;
		}

		if (stricmp(lpstrWord,"actor_speech")==0)
		{
			int	iCheckSum		=	decoder.GetNumber();
			iActorSerial		=	decoder.GetNumber();
			iConversationIndex	=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","There is no string after %s\n\nactor_speech!\nActor Serial: %d,%d",strFileName,iActorSerial,iConversationIndex);

				return	FALSE;
			}

			lpstrStream			=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\nThere is a garbage value after the nactor_speech string!\n code: %d,%d",strFileName,iActorSerial,iConversationIndex);

				return	FALSE;
			}

			lpActor				=	&lpText->m_aActor[iActorSerial];

			if	(lpActor->m_apConversation[iConversationIndex]	==	NULL)
				lpActor->m_apConversation[iConversationIndex]	=	new CConversationText();

			lpConversation				=	lpActor->m_apConversation[iConversationIndex];
			lpConversation->m_iCheckSum	=	iCheckSum;
			lpConversation->m_string.Set(lpstrStream);

			continue;
		}

		if (stricmp(lpstrWord,"actor_contents")==0)
		{
			int	iCheckSum		=	decoder.GetNumber();
			iActorSerial		=	decoder.GetNumber();
			iConversationIndex	=	decoder.GetNumber();
			iContentsIndex		=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","There is no string after %s\n\nactor_contents!\nActor Serial: %d,%d,%d",strFileName,iActorSerial,iConversationIndex,iContentsIndex);

				return	FALSE;
			}

			lpstrStream			=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","There is a garbage value after the %s\n\nactor_contents string!\n code : %d,%d,%d",strFileName,iActorSerial,iConversationIndex,iContentsIndex);

				return	FALSE;
			}

			lpActor				=	&lpText->m_aActor[iActorSerial];

			if	(lpActor->m_apConversation[iConversationIndex]	==	NULL)
			{
				lpActor->m_apConversation[iConversationIndex]				=	new CConversationText();
				lpActor->m_apConversation[iConversationIndex]->m_iCheckSum	=	0x12345678;
			}

			lpConversation		=	lpActor->m_apConversation[iConversationIndex];

			if	(lpConversation->m_apContents[iContentsIndex]	==	NULL)
				lpConversation->m_apContents[iContentsIndex]=	new CContentsText();

			lpContents			=	lpConversation->m_apContents[iContentsIndex];

			lpContents->m_iCheckSum	=	iCheckSum;

			lpContents->m_string.Set(lpstrStream);
			
			continue;
		}

		if (stricmp(lpstrWord,"actor_reaction")==0)
		{
			int	iCheckSum		=	decoder.GetNumber();

			iActorSerial		=	decoder.GetNumber();
			iConversationIndex	=	decoder.GetNumber();
			iContentsIndex		=	decoder.GetNumber();
			int	iKarma			=	decoder.GetNumber();
			int	iReaction		=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","There is no string after %s\n\nactor_reaction!\nActor Serial: %d,%d,%d,%d,%d",strFileName,iActorSerial,iConversationIndex,iContentsIndex,iKarma,iReaction);

				return	FALSE;
			}

			lpstrStream			=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","There is a garbage value after the %s\n\nactor_reaction string!\n code: %d,%d,%d,%d,%d",strFileName,iActorSerial,iConversationIndex,iContentsIndex,iKarma,iReaction);

				return	FALSE;
			}

			lpActor				=	&lpText->m_aActor[iActorSerial];

			if	(lpActor->m_apConversation[iConversationIndex]	==	NULL)
				lpActor->m_apConversation[iConversationIndex]	=	new CConversationText();

			lpConversation		=	lpActor->m_apConversation[iConversationIndex];

			if	(lpConversation->m_apContents[iContentsIndex]	==	NULL)
				lpConversation->m_apContents[iContentsIndex]=	new CContentsText();

			lpContents			=	lpConversation->m_apContents[iContentsIndex];

			if	(lpContents->m_apKarma[iKarma]	==	NULL)
				lpContents->m_apKarma[iKarma]	=	new cKarmaText();

			if	(lpContents->m_apKarma[iKarma]->m_apReaction[iReaction]	==	NULL)
				lpContents->m_apKarma[iKarma]->m_apReaction[iReaction]	=	new	CReactionText ();

			CReactionText	*lpReaction	=	lpContents->m_apKarma[iKarma]->m_apReaction[iReaction];

			lpReaction->m_iCheckSum		=	iCheckSum;
			lpReaction->m_iKarma		=	iKarma;
			lpReaction->m_string.Set(lpstrStream);

			continue;
		}

		if (stricmp(lpstrWord,"actor")==0)
		{
			int	iActorCheckSum	=	decoder.GetNumber();
			iActorSerial		=	decoder.GetNumber();

			lpActor				=	&lpText->m_aActor[iActorSerial];
			lpActor->m_iCheckSum=	iActorCheckSum;


			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","There is no string after %s\n\nactor!\nActor Serial: %d",strFileName);

				return	FALSE;
			}

			lpActor->m_string.Set(decoder.GetStream());

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","There is a garbage value after the %s\n\nactor string!\ncode: %d",strFileName);

				return	FALSE;
			}

			continue;
		}
		
		if (stricmp(lpstrWord,"conversation")==0)
		{
			iConversationIndex	=	decoder.GetNumber();

			lpActor->m_apConversation[iConversationIndex]	=	new CConversationText();
			lpConversation									=	lpActor->m_apConversation[iConversationIndex];

			continue;
		}

		if (stricmp(lpstrWord,"speech")==0)
		{
			lpConversation->m_iCheckSum	=	decoder.GetNumber();

			char	*lpstrStream	=	decoder.GetStream();

			lpConversation->m_string.Set(lpstrStream);

			continue;
		}

		if (stricmp(lpstrWord,"contents")==0)
		{
			int	iCheckSum			=	decoder.GetNumber();
			iContentsIndex			=	decoder.GetNumber();

			lpConversation->m_apContents[iContentsIndex]=	new CContentsText();
			lpContents									=	lpConversation->m_apContents[iContentsIndex];

			lpContents->m_iCheckSum						=	iCheckSum;

			lpContents->m_string.Set(decoder.GetStream());

			if (lpConversation->m_iCheckSum	==	0x12345678)
				lpConversation->m_iCheckSum	=	0x87654321;

			continue;
		}

		if (stricmp(lpstrWord,"reaction")==0)
		{
			int	iCheckSum		=	decoder.GetNumber();
			int	iKarma			=	decoder.GetNumber();
			int	iReaction		=	decoder.GetNumber();

			if	(lpContents->m_apKarma[iKarma]	==	NULL)
				lpContents->m_apKarma[iKarma]	=	new cKarmaText();

			lpContents->m_apKarma[iKarma]->m_apReaction[iReaction]	=	new	CReactionText ();

			lpContents->m_apKarma[iKarma]->m_apReaction[iReaction]->m_iCheckSum	=	iCheckSum;
			lpContents->m_apKarma[iKarma]->m_apReaction[iReaction]->m_iKarma		=	iKarma;
			lpContents->m_apKarma[iKarma]->m_apReaction[iReaction]->m_string.Set(decoder.GetStream());

			continue;
		}

		if (stricmp(lpstrWord,"area")==0)
		{
			int	iCheckSum		=	decoder.GetNumber();
			iAreaSerial			=	decoder.GetNumber();

			iActorSerial	=	-1,iCharacterSerial	=	-1;

			lpText->m_aArea[iAreaSerial].m_iCheckSum	=	iCheckSum;

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","There is no string after %s\n\narea!\ncode: %d",iAreaSerial,strFileName);

				return	FALSE;
			}

			lpText->m_aArea[iAreaSerial].m_string.Set(decoder.GetStream());

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\nThere is a garbage value after the narea string!\n code: %d",strFileName,iAreaSerial);

				return	FALSE;
			}

			lpArea				=	&lpText->m_aArea[iAreaSerial];

			continue;
		}

		if (stricmp(lpstrWord,"area_event")==0)
		{
			iConversationIndex	=	decoder.GetNumber();

			lpArea->m_apConversation[iConversationIndex]	=	new CConversationText();
			lpConversation									=	lpArea->m_apConversation[iConversationIndex];

			continue;
		}

		if (stricmp(lpstrWord,"area_speech")==0)
		{
			lpConversation->m_iCheckSum	=	decoder.GetNumber();

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","There is no string after %s\n\narea_speech!\ncode: %d",strFileName,iAreaSerial);

				return	FALSE;
			}

			char	*lpstrStream	=	decoder.GetStream();

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","There is a garbage value after the %s\n\narea_speech string!\n code: %d",strFileName,iAreaSerial);

				return	FALSE;
			}

			lpConversation->m_string.Set(lpstrStream);

			continue;
		}

		if (stricmp(lpstrWord,"area_contents")==0)
		{
			int	iCheckSum			=	decoder.GetNumber();
			iContentsIndex			=	decoder.GetNumber();

			lpConversation->m_apContents[iContentsIndex]=	new CContentsText();
			lpContents									=	lpConversation->m_apContents[iContentsIndex];

			lpContents->m_iCheckSum						=	iCheckSum;

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","There is no string after %s\n\narea_contents!\ncode : %d,%d",strFileName,iAreaSerial,iContentsIndex);

				return	FALSE;
			}

			lpContents->m_string.Set(decoder.GetStream());

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","There is a garbage value after the %s\n\narea_contents string!\n code : %d,%d",strFileName,iAreaSerial,iContentsIndex);

				return	FALSE;
			}

			if (lpConversation->m_iCheckSum	==	0x12345678)
				lpConversation->m_iCheckSum	=	0x87654321;

			continue;
		}

		if (stricmp(lpstrWord,"area_reaction")==0)
		{
			int	iCheckSum		=	decoder.GetNumber();
			int	iKarma			=	decoder.GetNumber();
			int	iReaction		=	decoder.GetNumber();

			if	(lpContents->m_apKarma[iKarma]	==	NULL)
				lpContents->m_apKarma[iKarma]	=	new cKarmaText();

			lpContents->m_apKarma[iKarma]->m_apReaction[iReaction]					=	new	CReactionText ();

			lpContents->m_apKarma[iKarma]->m_apReaction[iReaction]->m_iCheckSum		=	iCheckSum;
			lpContents->m_apKarma[iKarma]->m_apReaction[iReaction]->m_iKarma		=	iKarma;

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","There is no string after %s\n\narea_reaction!\ncode : %d,%d,%d",strFileName,iAreaSerial,iKarma,iReaction);

				return	FALSE;
			}

			lpContents->m_apKarma[iKarma]->m_apReaction[iReaction]->m_string.Set(decoder.GetStream());

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\narea_reaction There is a garbage value after the string!\n code: %d,%d,%d",strFileName,iAreaSerial,iKarma,iReaction);

				return	FALSE;
			}

			continue;
		}

		if (stricmp(lpstrWord,"shop")==0)
		{
			int	iCheckSum		=	decoder.GetNumber();
			int	iShop			=	decoder.GetNumber();

			lpText->m_aShop[iShop].m_iCheckSum	=	iCheckSum;

			if	(!decoder.findCharWithRange('"',5))
			{
				ERRMSG("","There is no string after %s\n\nshop!\ncode: %d",strFileName,iShop);

				return	FALSE;
			}

			lpText->m_aShop[iShop].m_string.Set(decoder.GetStream());

			if	(!decoder.isEmptyLine())
			{
				ERRMSG("","%s\n\nThere is a garbage value after the shop string!\n code: %d",strFileName,iShop);

				return	FALSE;
			}

			continue;
		}

		if (stricmp(lpstrWord,"isEndCode")==0)
			break;

		if	(iActorSerial	!=	-1)
		{
			ERRMSG("","%s\n\n%s\nActor Serial : %d\nConversation Index %d\nContents Index : %d",strFileName,lpstrWord,iActorSerial,iConversationIndex,iContentsIndex);

			MSGOUT("%s\nActor Serial : %d\nConversation Index %d\nContents Index : %d",_lpstrFileName,iActorSerial,iConversationIndex,iContentsIndex);
		}
		if	(iCharacterSerial	!=	-1)
		{
			ERRMSG("","%s\n\n%s\nCharacter Serial : %d\nConversation Index %d\nContents Index : %d",strFileName,lpstrWord,iCharacterSerial,iConversationIndex,iContentsIndex);

			MSGOUT("%s\nCharacter Serial : %d\nConversation Index %d\nContents Index : %d",lpstrWord,iCharacterSerial,iConversationIndex,iContentsIndex);
		}
		if	(iAreaSerial	!=	-1)
		{
			ERRMSG("","%s\n\n%s\nArea Serial : %d\nConversation Index %d\nContents Index : %d",strFileName,lpstrWord,iAreaSerial,iConversationIndex,iContentsIndex);

			MSGOUT("%s\nArea Serial : %d\nConversation Index %d\nContents Index : %d",lpstrWord,iAreaSerial,iConversationIndex,iContentsIndex);
		}

		break;
	}

	return	TRUE;
}

BOOL
cMAP::ForceSyncText(char *_lpstrFileName,BOOL _bIsSD)
{
	char	strFileName[512];

	if	(_bIsSD)
		sprintf(strFileName,"%s/localize/%s/lastest/(expansion)%s.txt",_logFolder,MAIN.m_strLocalizeFolder,_lpstrFileName);
	else
		sprintf(strFileName,"%s/localize/%s/lastest/%s.txt",_logFolder,MAIN.m_strLocalizeFolder,_lpstrFileName);

	if (!UploadTextForCompare(strFileName,&l_fieldSrc))
	{
		cMSG::Error("can not find text file",strFileName);

		return	TRUE;
	}

	if	(_bIsSD)
		sprintf(strFileName,"%s/localize/%s/import/(expansion)%s.txt",_logFolder,MAIN.m_strLocalizeFolder,_lpstrFileName);
	else
		sprintf(strFileName,"%s/localize/%s/import/%s.txt",_logFolder,MAIN.m_strLocalizeFolder,_lpstrFileName);
	if (!UploadTextForCompare(strFileName,&l_fieldDest))
		return	FALSE;

	if	(_bIsSD)
		sprintf(strFileName,"%s/localize/%s/force sync text/(expansion)%s.txt",_logFolder,MAIN.m_strLocalizeFolder,_lpstrFileName);
	else
		sprintf(strFileName,"%s/localize/%s/force sync text/%s.txt",_logFolder,MAIN.m_strLocalizeFolder,_lpstrFileName);

	cFILE	file;

	l_fieldDest.m_iFieldNameCheckSum	=	l_fieldSrc.m_iFieldNameCheckSum;

	if (!file.Open(strFileName,"wb"))
		return	FALSE;

	int	iActor,iArea,iShop,iCharacter,iKarma;

	for (iKarma=0;iKarma<1024;iKarma++)
	{
		CKarmaText		*lpSrcKarma		=	&l_fieldSrc.m_aKarma[iKarma];
		CKarmaText		*lpDestKarma	=	&l_fieldDest.m_aKarma[iKarma];

		if	(lpSrcKarma->m_iCheckSum	==	0x12345678)
			continue;

		lpSrcKarma->m_bIsChangeText		=	TRUE;

		for (int iReaction=0;iReaction<c_iLocalizeContentsCount;iReaction++)
		{
			CReactionText	*lpSrcReaction	=	lpSrcKarma->m_apReaction[iReaction];
			CReactionText	*lpDestReaction	=	lpDestKarma->m_apReaction[iReaction];

			if (!lpSrcReaction || lpSrcReaction->m_iCheckSum	==	0x12345678)
				continue;

			if (!lpDestReaction)
			{
				cMSG::Put("error","in cMAP::ForceSyncText - !lpDestReaction");

				return	FALSE;
			}

			lpDestReaction->m_iCheckSum	=	lpSrcReaction->m_iCheckSum;
			lpDestReaction->m_iKarma	=	lpSrcReaction->m_iKarma;
		}
	}

	for (iActor=0;iActor<1024;iActor++)
	{
		CActorText	*lpSrcActor	=	&l_fieldSrc.m_aActor[iActor];
		CActorText	*lpDestActor=	&l_fieldDest.m_aActor[iActor];

		if	(lpSrcActor->m_iCheckSum	==	0x12345678)
			continue;

		lpSrcActor->m_bIsChangeText	=	TRUE;

		lpDestActor->m_iCheckSum	=	lpSrcActor->m_iCheckSum;

		for (int iConversation=0;iConversation<c_iLocalizeContentsCount;iConversation++)
		{
			CConversationText	*lpSrcConversation	=	lpSrcActor->m_apConversation[iConversation];
			CConversationText	*lpDestConversation	=	lpDestActor->m_apConversation[iConversation];

			if	(!lpSrcConversation	||	lpSrcConversation->m_iCheckSum	==	0x12345678)
				continue;

			if	(!lpDestConversation)
			{
				cMSG::Put("error","in cMAP::ForceSyncText - !lpDestConversation");

				return	FALSE;
			}

			lpDestConversation->m_iCheckSum	=	lpSrcConversation->m_iCheckSum;

			for (int iContents=0;iContents<c_iLocalizeContentsCount;iContents++)
			{
				CContentsText	*lpSrcContents	=	lpSrcConversation->m_apContents[iContents];
				CContentsText	*lpDestContents	=	lpDestConversation->m_apContents[iContents];

				if (!lpSrcContents	||	lpSrcContents->m_iCheckSum	==	0x12345678)
					continue;

				if (!lpDestContents)
				{
					cMSG::Put("error","in cMAP::ForceSyncText - !lpDestContents");

					return	FALSE;
				}

				lpDestContents->m_iCheckSum	=	lpSrcContents->m_iCheckSum;

				for (int iKarma=0;iKarma<c_iLocalizeContentsCount;iKarma++)
				{
					cKarmaText	*lpSrcKarma		=	lpSrcContents->m_apKarma[iKarma];
					cKarmaText	*lpDestKarma	=	lpDestContents->m_apKarma[iKarma];

					if	(!lpSrcKarma)
						continue;

					if	(!lpDestKarma)
					{
						cMSG::Put("error","in cMAP::ForceSyncText - !lpDestKarma");

						return	FALSE;
					}

					for (int iReaction=0;iReaction<c_iLocalizeContentsCount;iReaction++)
					{
						CReactionText	*lpSrcReaction	=	lpSrcKarma->m_apReaction[iReaction];
						CReactionText	*lpDestReaction	=	lpDestKarma->m_apReaction[iReaction];

						if	(!lpSrcReaction || lpSrcReaction->m_iCheckSum	==	0x12345678)
							continue;

						if	(!lpDestReaction)
						{
							cMSG::Put("error","in cMAP::ForceSyncText - !lpDestReaction");

							return	FALSE;
						}

						lpDestReaction->m_iCheckSum	=	lpSrcReaction->m_iCheckSum;
						lpDestReaction->m_iKarma	=	lpSrcReaction->m_iKarma;
					}
				}
			}
		}
	}

	for (iCharacter=0;iCharacter<1024;iCharacter++)
	{
		cCharacterText	*lpSrcCharacter	=	&l_fieldSrc.m_aCharacter[iCharacter];
		cCharacterText	*lpDestCharacter=	&l_fieldDest.m_aCharacter[iCharacter];

		if (lpSrcCharacter->m_iCheckSum	==	0x12345678)
			continue;

		lpSrcCharacter->m_bIsChangeText	=	TRUE;

		for (int iEvent=0;iEvent<100;iEvent++)
		{
			CContentsText	*lpSrcEvent	=	lpSrcCharacter->m_apContents[iEvent];
			CContentsText	*lpDestEvent=	lpDestCharacter->m_apContents[iEvent];

			if (!lpSrcEvent	||	lpSrcEvent->m_iCheckSum	==	0x12345678)
				continue;

			if (!lpDestEvent)
			{
				cMSG::Put("error","in cMAP::ForceSyncText - !lpDestConversation");

				return	FALSE;
			}

			lpDestEvent->m_iCheckSum	=	lpSrcEvent->m_iCheckSum;

			for (int iKarma=0;iKarma<100;iKarma++)
			{
				cKarmaText	*lpSrcKarma	=	lpSrcEvent->m_apKarma[iKarma];
				cKarmaText	*lpDestKarma=	lpDestEvent->m_apKarma[iKarma];

				if	(!lpSrcKarma)
					continue;

				if	(!lpDestKarma)
				{
					cMSG::Put("error","in cMAP::ForceSyncText - !lpDestContents");

					return	FALSE;
				}

				for (int iReaction=0;iReaction<100;iReaction++)
				{
					CReactionText	*lpSrcReaction	=	lpSrcKarma->m_apReaction[iReaction];
					CReactionText	*lpDestReaction	=	lpDestKarma->m_apReaction[iReaction];

					if (!lpSrcReaction || lpSrcReaction->m_iCheckSum	==	0x12345678)
						continue;

					if (!lpDestReaction)
					{
						cMSG::Put("error","in cMAP::ForceSyncText - !lpDestReaction");

						return	FALSE;
					}

					lpDestReaction->m_iCheckSum	=	lpSrcReaction->m_iCheckSum;
					lpDestReaction->m_iKarma	=	lpSrcReaction->m_iKarma;
					lpDestReaction->m_iEvent	=	lpSrcReaction->m_iEvent;
				}
			}
		}
	}

	for (iArea=0;iArea<1024;iArea++)
	{
		CActorText	*lpSrcActor	=	&l_fieldSrc.m_aActor[iArea];
		CActorText	*lpDestActor=	&l_fieldDest.m_aActor[iArea];

		if (lpSrcActor->m_iCheckSum	==	0x12345678)
			continue;

		lpSrcActor->m_bIsChangeText	=	TRUE;

		lpDestActor->m_iCheckSum	=	lpSrcActor->m_iCheckSum;

		for (int iConversation=0;iConversation<100;iConversation++)
		{
			CConversationText	*lpSrcConversation	=	lpSrcActor->m_apConversation[iConversation];
			CConversationText	*lpDestConversation	=	lpDestActor->m_apConversation[iConversation];

			if (!lpSrcConversation	||	lpSrcConversation->m_iCheckSum	==	0x12345678)
				continue;

			if (!lpDestConversation)
			{
				cMSG::Put("error","in cMAP::ForceSyncText - !lpDestConversation");

				return	FALSE;
			}

			lpDestConversation->m_iCheckSum	=	lpSrcConversation->m_iCheckSum;

			for (int iContents=0;iContents<100;iContents++)
			{
				CContentsText	*lpSrcContents	=	lpSrcConversation->m_apContents[iContents];
				CContentsText	*lpDestContents	=	lpDestConversation->m_apContents[iContents];

				if (!lpSrcContents	||	lpSrcContents->m_iCheckSum	==	0x12345678)
					continue;

				if (!lpDestContents)
				{
					cMSG::Put("error","in cMAP::ForceSyncText - !lpDestContents");

					return	FALSE;
				}

				lpDestContents->m_iCheckSum	=	lpSrcContents->m_iCheckSum;

				for (int iKarma=0;iKarma<100;iKarma++)
				{
					cKarmaText	*lpSrcKarma	=	lpSrcContents->m_apKarma[iKarma];
					cKarmaText	*lpDestKarma=	lpDestContents->m_apKarma[iKarma];

					if	(!lpSrcKarma	||	!lpDestKarma)
						continue;

					if	(!lpDestKarma)
					{
						cMSG::Put("error","in cMAP::ForceSyncText - !lpDestReaction");

						return	FALSE;
					}

					for (int iReaction=0;iReaction<100;iReaction++)
					{
						CReactionText	*lpSrcReaction	=	lpSrcKarma->m_apReaction[iReaction];
						CReactionText	*lpDestReaction	=	lpDestKarma->m_apReaction[iReaction];

						if	(!lpSrcReaction || lpSrcReaction->m_iCheckSum	==	0x12345678)
							continue;

						if	(!lpDestReaction)
						{
							cMSG::Put("error","in cMAP::ForceSyncText - !lpDestReaction");

							return	FALSE;
						}
						lpDestReaction->m_iCheckSum	=	lpSrcReaction->m_iCheckSum;
						lpDestReaction->m_iKarma	=	lpSrcReaction->m_iKarma;
					}
				}
			}
		}
/*
		if (l_fieldSrc.m_aArea[iArea].m_iCheckSum	==	0x12345678)
		{
			l_fieldDest.m_aArea[iArea].m_iCheckSum	=	0;
			continue;
		}

		l_fieldDest.m_aArea[iArea].m_iCheckSum		=	l_fieldSrc.m_aArea[iArea].m_iCheckSum;
*/
	}

	for (iShop=0;iShop<1024;iShop++)
	{
		if (l_fieldSrc.m_aShop[iShop].m_iCheckSum	==	0x12345678)
		{
			l_fieldDest.m_aShop[iShop].m_iCheckSum	=	0;
			continue;
		}

		l_fieldDest.m_aShop[iShop].m_iCheckSum		=	l_fieldSrc.m_aShop[iShop].m_iCheckSum;
	}

	file.writeStreamCR("#fieldName [%d] \"%s\"\n",l_fieldSrc.m_iFieldNameCheckSum,l_fieldDest.m_string.String);

	for (iKarma=0;iKarma<1024;iKarma++)
	{
		CKarmaText		*lpSrcKarma		=	&l_fieldSrc.m_aKarma[iKarma];
		CKarmaText		*lpDestKarma	=	&l_fieldDest.m_aKarma[iKarma];

		if	(lpSrcKarma->m_bIsChangeText==	FALSE)
			continue;

		for (int iReaction=0;iReaction<100;iReaction++)
		{
			CReactionText	*lpSrcReaction	=	lpSrcKarma->m_apReaction[iReaction];
			CReactionText	*lpDestReaction	=	lpDestKarma->m_apReaction[iReaction];

			if (!lpSrcReaction || lpSrcReaction->m_iCheckSum	==	0x12345678)
				continue;

			file.writeStreamCR("#field_karma_reaction [%d] %d,%d \"%s\"",lpSrcReaction->m_iCheckSum,lpDestReaction->m_iKarma,iKarma,lpDestReaction->m_string.String);
		}
	}

	for (iActor=0;iActor<1024;iActor++)
	{
		CActorText	*lpDestActor	=	&l_fieldDest.m_aActor[iActor];
		CActorText	*lpSrcActor		=	&l_fieldSrc.m_aActor[iActor];

		if (lpSrcActor->m_bIsChangeText	==	FALSE)
			continue;

		file.writeStreamCR("#actor_name [%d] %d \"%s\"",lpSrcActor->m_iCheckSum,iActor,lpDestActor->m_string.String);

		for (int iConversation=0;iConversation<c_iLocalizeContentsCount;iConversation++)
		{
			CConversationText	*lpDestConversation	=	lpDestActor->m_apConversation[iConversation];
			CConversationText	*lpSrcConversation	=	lpSrcActor->m_apConversation[iConversation];

			if	(!lpSrcConversation || lpSrcConversation->m_iCheckSum	==	0x12345678)
				continue;

			if	(!lpSrcConversation || lpSrcConversation->m_iCheckSum	!=	0x87654321)
				file.writeStreamCR("	#actor_speech [%d] %d,%d \"%s\"",lpSrcConversation->m_iCheckSum,iActor,iConversation,lpDestConversation->m_string.String);

			for (int iContents=0;iContents<c_iLocalizeContentsCount;iContents++)
			{
				CContentsText	*lpDestContents	=	lpDestConversation->m_apContents[iContents];
				CContentsText	*lpSrcContents	=	lpSrcConversation->m_apContents[iContents];

				if (!lpSrcContents || lpSrcContents->m_iCheckSum	==	0x12345678)
					continue;

				if	(lpSrcContents->m_iCheckSum	!=	0x87654321)
					file.writeStreamCR("		#actor_contents	[%d] %d,%d,%d \"%s\"",lpSrcContents->m_iCheckSum,iActor,iConversation,iContents,lpDestContents->m_string.String);

				for (int iKarma=0;iKarma<c_iLocalizeContentsCount;iKarma++)
				{
					cKarmaText	*lpSrcKarmaText	=	lpSrcContents->m_apKarma[iKarma];
					cKarmaText	*lpDestKarmaText=	lpDestContents->m_apKarma[iKarma];

					if	(!lpSrcKarmaText	||	!lpDestKarmaText)
						continue;

					for (int iReaction=0;iReaction<c_iLocalizeContentsCount;iReaction++)
					{
						CReactionText	*lpSrcReaction	=	lpSrcKarmaText->m_apReaction[iReaction];
						CReactionText	*lpDestReaction	=	lpDestKarmaText->m_apReaction[iReaction];
						
						if	(!lpSrcReaction || lpSrcReaction->m_iCheckSum	==	0x12345678)
							continue;

						file.writeStreamCR("			#actor_reaction	[%d] %d,%d,%d,%d,%d \"%s\"",lpSrcReaction->m_iCheckSum,iActor,iConversation,iContents,iKarma,iReaction,lpDestReaction->m_string.String);
					}
				}
			}
		}

		file.writeStreamCR("  ");
	}

	for (iCharacter=0;iCharacter<1024;iCharacter++)
	{
		cCharacterText	*lpDestCharacter	=	&l_fieldDest.m_aCharacter[iCharacter];
		cCharacterText	*lpSrcCharacter		=	&l_fieldSrc.m_aCharacter[iCharacter];

		if	(lpSrcCharacter->m_bIsChangeText	==	FALSE)
			continue;

		file.writeStreamCR("#character_name %d [%d] \"%s\"",iCharacter,GetTextCheckSum(lpSrcCharacter->m_strName.String),lpSrcCharacter->m_strName.String);

		for (int iEvent=0;iEvent<100;iEvent++)
		{
			CContentsText	*lpDestContents	=	lpDestCharacter->m_apContents[iEvent];
			CContentsText	*lpSrcContents	=	lpSrcCharacter->m_apContents[iEvent];

			if	(!lpSrcContents || lpSrcContents->m_iCheckSum	==	0x12345678)
				continue;

			for (int iKarma=0;iKarma<100;iKarma++)
			{
				cKarmaText	*lpDestKarma	=	lpDestContents->m_apKarma[iKarma];
				cKarmaText	*lpSrcKarma		=	lpSrcContents->m_apKarma[iKarma];

				if	(!lpSrcKarma	||	!lpDestKarma)
					continue;

				for (int iReaction=0;iReaction<100;iReaction++)
				{
					CReactionText	*lpSrcReaction	=	lpSrcKarma->m_apReaction[iReaction];
					CReactionText	*lpDestReaction	=	lpDestKarma->m_apReaction[iReaction];

					if	(!lpSrcReaction || lpSrcReaction->m_iCheckSum	==	0x12345678)
						continue;

					file.writeStreamCR("    #character_event [%d] %d,%d,%d \"%s\"",lpSrcReaction->m_iCheckSum,iEvent,lpDestReaction->m_iKarma,iReaction,lpDestReaction->m_string.String);
				}
			}
		}

		file.writeStreamCR("  ");
	}

	for (iArea=0;iArea<1024;iArea++)
	{
		CActorText	*lpDestActor	=	&l_fieldDest.m_aArea[iActor];
		CActorText	*lpSrcActor		=	&l_fieldSrc.m_aArea[iActor];

		if (lpSrcActor->m_bIsChangeText	==	FALSE)
			continue;

		file.writeStreamCR("#area [%d] %d \"%s\"",lpSrcActor->m_iCheckSum,iActor,lpDestActor->m_string.String);

		for (int iConversation=0;iConversation<100;iConversation++)
		{
			CConversationText	*lpDestConversation	=	lpDestActor->m_apConversation[iConversation];
			CConversationText	*lpSrcConversation	=	lpSrcActor->m_apConversation[iConversation];

			if (!lpSrcConversation || lpSrcConversation->m_iCheckSum	==	0x12345678)
				continue;

			file.writeStreamCR("  #area_event %d",iConversation);

			if (!lpSrcConversation || lpSrcConversation->m_iCheckSum	!=	0x87654321)
				file.writeStreamCR("    #area_speech [%d] \"%s\"",lpSrcConversation->m_iCheckSum,lpDestConversation->m_string.String);

			for (int iContents=0;iContents<100;iContents++)
			{
				CContentsText	*lpDestContents	=	lpDestConversation->m_apContents[iContents];
				CContentsText	*lpSrcContents	=	lpSrcConversation->m_apContents[iContents];

				if (!lpSrcContents || lpSrcContents->m_iCheckSum	==	0x12345678)
					continue;

				if (lpSrcContents->m_iCheckSum	!=	0x87654321)
					file.writeStreamCR("    #area_contents [%d] %d \"%s\"",lpSrcContents->m_iCheckSum,iContents,lpDestContents->m_string.String);

				for (int iKarma=0;iKarma<100;iKarma++)
				{
					cKarmaText	*lpDestKarma	=	lpDestContents->m_apKarma[iKarma];
					cKarmaText	*lpSrcKarma		=	lpSrcContents->m_apKarma[iKarma];

					if	(!lpSrcKarma	||	!lpDestKarma)
						continue;
					
					for (int iReaction=0;iReaction<100;iReaction++)
					{
						CReactionText	*lpDestReaction	=	lpDestKarma->m_apReaction[iReaction];
						CReactionText	*lpSrcReaction	=	lpSrcKarma->m_apReaction[iReaction];

						if (!lpSrcReaction || lpSrcReaction->m_iCheckSum	==	0x12345678)
							continue;

						file.writeStreamCR("        #area_reaction [%d] %d,%d \"%s\"",lpSrcReaction->m_iCheckSum,lpDestReaction->m_iKarma,iReaction,lpDestReaction->m_string.String);
					}
				}
			}
		}

		file.writeStreamCR("  ");
	}

	for (iShop=0;iShop<1024;iShop++)
	{
		cSTRING	*lpString	=	&l_fieldDest.m_aShop[iShop].m_string;

		if (l_fieldDest.m_aShop[iShop].m_iCheckSum)
			file.writeStreamCR("#shop [%d] %d \"%s\"",l_fieldSrc.m_aShop[iShop].m_iCheckSum,iShop,lpString->String);
	}

	file.Close();

	return	TRUE;
}

int
cMAP::CompareText(char *_lpstrFileName,BOOL _bIsSD,BOOL _bIsWantCheck)
{
	char	strFileName[512];

	char	strCheckFolder[512];
	char	strCheckResultFolder[512];
	char	strSourceFolder[512];

	sprintf(strCheckFolder,"localize/%s/lastest",MAIN.m_strLocalizeFolder);
	strcpy(strSourceFolder,"old");
	strcpy(strCheckResultFolder,"update");

	if	(_bIsWantCheck)
	{
		strcpy(strSourceFolder,"_modify result");
		strcpy(strCheckResultFolder,"_check result");
	}

	if	(_bIsSD)
		sprintf(strFileName,"%s/%s/(expansion)%s.txt",_logFolder,strCheckFolder,_lpstrFileName);
	else
		sprintf(strFileName,"%s/%s/%s.txt",_logFolder,strCheckFolder,_lpstrFileName);

	if	(!UploadTextForCompare(strFileName,&l_fieldSrc))
	{
//		cMSG::Error("can not find text file",strFileName);

		return	TRUE;
	}

	if	(_bIsSD)
		sprintf(strFileName,"%s/localize/%s/%s/(expansion)%s.txt",_logFolder,MAIN.m_strLocalizeFolder,strSourceFolder,_lpstrFileName);
	else
		sprintf(strFileName,"%s/localize/%s/%s/%s.txt",_logFolder,MAIN.m_strLocalizeFolder,strSourceFolder,_lpstrFileName);

	if	(!UploadTextForCompare(strFileName,&l_fieldDest))
		return	FALSE;

	if	(_bIsSD)
		sprintf(strFileName,"%s/localize/%s/%s/(expansion)%s.txt",_logFolder,MAIN.m_strLocalizeFolder,strCheckResultFolder,_lpstrFileName);
	else
		sprintf(strFileName,"%s/localize/%s/%s/%s.txt",_logFolder,MAIN.m_strLocalizeFolder,strCheckResultFolder,_lpstrFileName);

	cFILE	file;

	if (!file.Open(strFileName,"wb"))
		return	FALSE;

	int	iCharacter,iActor,iArea,iShop,iFK;

	for (iFK=0;iFK<1024;iFK++)
	{
		CKarmaText	*lpSrcKarma			=	&l_fieldSrc.m_aKarma[iFK];
		CKarmaText	*lpDestKarma		=	&l_fieldDest.m_aKarma[iFK];

		if	(lpSrcKarma->m_iCheckSum	==	0x12345678)
			continue;

		for (int iReaction=0;iReaction<100;iReaction++)
		{
			CReactionText	*lpSrcReaction	=	lpSrcKarma->m_apReaction[iReaction];
			CReactionText	*lpDestReaction	=	lpDestKarma->m_apReaction[iReaction];

			if (!lpSrcReaction || lpSrcReaction->m_iCheckSum	==	0x12345678)
				continue;

			if (!lpDestReaction	||	lpSrcReaction->m_iCheckSum	!=	lpDestReaction->m_iCheckSum	||	lpSrcReaction->m_iKarma!=	lpDestReaction->m_iKarma)
			{
				lpSrcReaction->m_bIsChangeText	=	TRUE;
				lpSrcKarma->m_bIsChangeText		=	TRUE;
				break;
			}
		}
	}

	for (iCharacter=0;iCharacter<1024;iCharacter++)
	{
		cCharacterText	*lpSrcCharacter	=	&l_fieldSrc.m_aCharacter[iCharacter];
		cCharacterText	*lpDestCharacter=	&l_fieldDest.m_aCharacter[iCharacter];

		if	(!lpSrcCharacter	||	lpSrcCharacter->m_iCheckSum	==	0x12345678)
			continue;

		if	(lpSrcCharacter->m_iCheckSum	!=	lpDestCharacter->m_iCheckSum)
			lpSrcCharacter->m_bIsChangeText	=	TRUE;

		for (int iEvent=0;iEvent<100;iEvent++)
		{
			CContentsText	*lpSrcEvent	=	lpSrcCharacter->m_apContents[iEvent];
			CContentsText	*lpDestEvent=	lpDestCharacter->m_apContents[iEvent];

			if	(!lpSrcEvent)
				continue;

			if	(!lpDestEvent)
			{
				lpSrcCharacter->m_bIsChangeText	=	TRUE;
				break;
			}

			for (int iKarma=0;iKarma<100;iKarma++)
			{
				cKarmaText	*lpSrcKarma	=	lpSrcEvent->m_apKarma[iKarma];
				cKarmaText	*lpDestKarma=	lpDestEvent->m_apKarma[iKarma];

				if	(!lpSrcKarma)
					continue;

				if	(!lpDestKarma)
				{
					lpSrcCharacter->m_bIsChangeText	=	TRUE;
					break;
				}

				for (int iReaction=0;iReaction<100;iReaction++)
				{
					CReactionText	*lpSrcReaction	=	lpSrcKarma->m_apReaction[iReaction];
					CReactionText	*lpDestReaction	=	NULL;
					
					if	(lpDestKarma)
						lpDestReaction	=	lpDestKarma->m_apReaction[iReaction];

					if (!lpSrcReaction || lpSrcReaction->m_iCheckSum	==	0x12345678)
						continue;

					if (!lpDestReaction	||	lpSrcReaction->m_iCheckSum	!=	lpDestReaction->m_iCheckSum	||	lpSrcReaction->m_iKarma!=	lpDestReaction->m_iKarma)
					{
						lpSrcCharacter->m_bIsChangeText	=	TRUE;
						break;
					}
				}

				if (lpSrcCharacter->m_bIsChangeText)
					break;
			}

			if (lpSrcCharacter->m_bIsChangeText)
				break;
		}
	}


	for (iArea=0;iArea<1024;iArea++)
	{
		CActorText	*lpSrcActor	=	&l_fieldSrc.m_aArea[iArea];
		CActorText	*lpDestActor=	&l_fieldDest.m_aArea[iArea];

		if (lpSrcActor->m_iCheckSum	==	0x12345678)
			continue;

		if (lpSrcActor->m_iCheckSum	!=	lpDestActor->m_iCheckSum	)
		{
//			return	3;
			lpSrcActor->m_bIsChangeText	=	TRUE;
			continue;
		}

		for (int iConversation=0;iConversation<100;iConversation++)
		{
			CConversationText	*lpSrcConversation	=	lpSrcActor->m_apConversation[iConversation];
			CConversationText	*lpDestConversation	=	lpDestActor->m_apConversation[iConversation];

			if (!lpSrcConversation	||	lpSrcConversation->m_iCheckSum	==	0x12345678)
				continue;

			if (!lpDestConversation	||	lpSrcConversation->m_iCheckSum	!=	lpDestConversation->m_iCheckSum )
			{
//				return	3;
				lpSrcActor->m_bIsChangeText	=	TRUE;
				break;
			}


			for (int iContents=0;iContents<100;iContents++)
			{
				CContentsText	*lpSrcContents	=	lpSrcConversation->m_apContents[iContents];
				CContentsText	*lpDestContents	=	lpDestConversation->m_apContents[iContents];

				if (!lpSrcContents	||	lpSrcContents->m_iCheckSum	==	0x12345678)
					continue;

				if (!lpDestContents	||	lpSrcContents->m_iCheckSum	!=	lpDestContents->m_iCheckSum)
				{
//					return	3;
					lpSrcActor->m_bIsChangeText	=	TRUE;
					break;
				}

				for (int iKarma=0;iKarma<100;iKarma++)
				{
					cKarmaText	*lpSrcKarma	=	lpSrcContents->m_apKarma[iKarma];
					cKarmaText	*lpDestKarma=	lpDestContents->m_apKarma[iKarma];

					if	(!lpSrcKarma)
						continue;

					for (int iReaction=0;iReaction<100;iReaction++)
					{
						CReactionText	*lpSrcReaction	=	lpSrcKarma->m_apReaction[iReaction];
						CReactionText	*lpDestReaction	=	NULL;
						
						if	(lpDestKarma)
							lpDestReaction	=	lpDestKarma->m_apReaction[iReaction];

						if (!lpSrcReaction || lpSrcReaction->m_iCheckSum	==	0x12345678)
							continue;

						if (!lpDestReaction	||	lpSrcReaction->m_iCheckSum	!=	lpDestReaction->m_iCheckSum	||	lpSrcReaction->m_iKarma!=	lpDestReaction->m_iKarma)
						{
	//						return	3;
							lpSrcActor->m_bIsChangeText	=	TRUE;
							break;
						}
					}
				}

				if (lpSrcActor->m_bIsChangeText)
					break;
			}

			if (lpSrcActor->m_bIsChangeText)
				break;
		}
	}

	for (iShop=0;iShop<1024;iShop++)
	{
		if (l_fieldSrc.m_aShop[iShop].m_iCheckSum	==	0x12345678)
		{
			l_fieldSrc.m_aShop[iShop].m_iCheckSum	=	0;
			continue;
		}

		if (l_fieldSrc.m_aShop[iShop].m_iCheckSum	!=	l_fieldDest.m_aShop[iShop].m_iCheckSum)
			l_fieldSrc.m_aShop[iShop].m_iCheckSum	=	1;
		else
			l_fieldSrc.m_aShop[iShop].m_iCheckSum	=	0;
	}


	if	(l_fieldSrc.m_iFieldNameCheckSum	!=	l_fieldDest.m_iFieldNameCheckSum)
		file.writeStreamCR("#fieldName [%d] \"%s\"\n",GetTextCheckSum(l_fieldSrc.m_string.String),l_fieldSrc.m_string.String);

	for	(iFK=0;iFK<1024;iFK++)
	{
		CKarmaText	*lpKT	=	&l_fieldSrc.m_aKarma[iFK];

		if	(lpKT->m_bIsChangeText	==	FALSE)
			continue;

		for (int iReaction=0;iReaction<100;iReaction++)
		{
			CReactionText	*lpSrcReaction	=	lpKT->m_apReaction[iReaction];

			if	(!lpSrcReaction || lpSrcReaction->m_iCheckSum	==	0x12345678	||	lpSrcReaction->m_bIsChangeText	==	FALSE)
				continue;

			file.writeStreamCR("#field_karma_reaction	[%d] %d,%d \"%s\"\n",GetTextCheckSum(lpSrcReaction->m_string.String),lpSrcReaction->m_iKarma,iReaction,lpSrcReaction->m_string.String);
		}
	}

	for (iActor=0;iActor<1024;iActor++)
	{
		CActorText	*lpSrcActor	=	&l_fieldSrc.m_aActor[iActor];
		CActorText	*lpDestActor=	&l_fieldDest.m_aActor[iActor];

		BOOL		bIsUpdate	=	FALSE;

		if	(lpSrcActor->m_iCheckSum	==	0x12345678)
			continue;

		if	(lpSrcActor->m_iCheckSum	!=	lpDestActor->m_iCheckSum	)
		{
			file.writeStreamCR("#actor_name [%d] %d \"%s\"",GetTextCheckSum(lpSrcActor->m_string.String),iActor,lpSrcActor->m_string.String);
			bIsUpdate	=	TRUE;
		}

		for (int iConversation=0;iConversation<c_iLocalizeContentsCount;iConversation++)
		{
			CConversationText	*lpSrcConversation	=	lpSrcActor->m_apConversation[iConversation];
			CConversationText	*lpDestConversation	=	lpDestActor->m_apConversation[iConversation];

			if	(!lpSrcConversation	||	lpSrcConversation->m_iCheckSum	==	0x12345678 || lpSrcConversation->m_string.String == NULL)
				continue;

			if	(!lpDestConversation	||	lpSrcConversation->m_iCheckSum	!=	lpDestConversation->m_iCheckSum )
			{
				file.writeStreamCR("	#actor_speech [%d] %d,%d \"%s\"",GetTextCheckSum(lpSrcConversation->m_string.String),iActor,iConversation,lpSrcConversation->m_string.String);
				bIsUpdate	=	TRUE;
			}

			for (int iContents=0;iContents<c_iLocalizeContentsCount;iContents++)
			{
				CContentsText	*lpSrcContents	=	lpSrcConversation->m_apContents[iContents];
				CContentsText	*lpDestContents	=	NULL;
				
				if	(lpDestConversation)
					lpDestContents	=	lpDestConversation->m_apContents[iContents];

				if	(!lpSrcContents	||	lpSrcContents->m_iCheckSum	==	0x12345678)
					continue;

				if	(!lpDestContents	||	lpSrcContents->m_iCheckSum	!=	lpDestContents->m_iCheckSum)
				{
					file.writeStreamCR("		#actor_contents	[%d] %d,%d,%d \"%s\"",GetTextCheckSum(lpSrcContents->m_string.String),iActor,iConversation,iContents,lpSrcContents->m_string.String);
					bIsUpdate	=	TRUE;
				}

				for (int iKarma=0;iKarma<c_iLocalizeContentsCount;iKarma++)
				{
					cKarmaText	*lpSrcKarmaText		=	lpSrcContents->m_apKarma[iKarma];
					cKarmaText	*lpDestKarmaText	=	NULL;
					
					if	(lpDestContents)
						lpDestKarmaText	=	lpDestContents->m_apKarma[iKarma];

					if	(!lpSrcKarmaText)
						continue;

					for (int iReaction=0;iReaction<c_iLocalizeContentsCount;iReaction++)
					{
						CReactionText	*lpSrcReaction	=	lpSrcKarmaText->m_apReaction[iReaction];
						CReactionText	*lpDestReaction	=	NULL;

						if	(lpDestKarmaText)
							lpDestReaction	=	lpDestKarmaText->m_apReaction[iReaction];

						if	(!lpSrcReaction || lpSrcReaction->m_iCheckSum	==	0x12345678)
							continue;

						if	(!lpDestReaction	||	lpSrcReaction->m_iCheckSum	!=	lpDestReaction->m_iCheckSum	||	lpSrcReaction->m_iKarma!=	lpDestReaction->m_iKarma)
						{
							file.writeStreamCR("			#actor_reaction	[%d] %d,%d,%d,%d,%d \"%s\"",GetTextCheckSum(lpSrcReaction->m_string.String),iActor,iConversation,iContents,iKarma,iReaction,lpSrcReaction->m_string.String);
							bIsUpdate	=	TRUE;
						}
					}
				}
			}
		}

		if	(bIsUpdate)
			file.writeStreamCR("  ");
	}
	
	for (iCharacter=0;iCharacter<1024;iCharacter++)
	{
		cCharacterText	*lpSrcCharacter	=	&l_fieldSrc.m_aCharacter[iCharacter];

		if	(lpSrcCharacter->m_bIsChangeText	==	FALSE)
			continue;

		file.writeStreamCR("#character_name %d [%d] \"%s\"",iCharacter,GetTextCheckSum(lpSrcCharacter->m_strName.String),lpSrcCharacter->m_strName.String);

		for (int iEvent=0;iEvent<100;iEvent++)
		{
			CContentsText	*lpSrcContents	=	lpSrcCharacter->m_apContents[iEvent];

			if (!lpSrcContents)
				continue;

			for (int iKarma=0;iKarma<100;iKarma++)
			{
				cKarmaText	*lpSrcKarma	=	lpSrcContents->m_apKarma[iKarma];

				if	(!lpSrcKarma)
					continue;

				for (int iReaction=0;iReaction<100;iReaction++)
				{
					CReactionText	*lpSrcReaction	=	lpSrcKarma->m_apReaction[iReaction];

					if	(!lpSrcReaction || lpSrcReaction->m_iCheckSum	==	0x12345678)
						continue;

					file.writeStreamCR("    #character_event [%d] %d,%d,%d \"%s\"",GetTextCheckSum(lpSrcReaction->m_string.String),iEvent,lpSrcReaction->m_iKarma,iReaction,lpSrcReaction->m_string.String);
				}
			}
		}

		file.writeStreamCR("  ");
	}

	for (iArea=0;iArea<1024;iArea++)
	{
		CActorText	*lpSrcActor	=	&l_fieldSrc.m_aArea[iArea];

		if (lpSrcActor->m_bIsChangeText	==	FALSE)
			continue;

		file.writeStreamCR("#area [%d] %d \"%s\"",GetTextCheckSum(lpSrcActor->m_string.String),iArea,lpSrcActor->m_string.String);

		for (int iConversation=0;iConversation<100;iConversation++)
		{
			CConversationText	*lpSrcConversation	=	lpSrcActor->m_apConversation[iConversation];

			if (!lpSrcConversation || lpSrcConversation->m_iCheckSum	==	0x12345678)
				continue;

			file.writeStreamCR("  #area_event %d",iConversation);

			if (!lpSrcConversation || lpSrcConversation->m_iCheckSum	!=	0x87654321)
				file.writeStreamCR("    #area_speech [%d] \"%s\"",GetTextCheckSum(lpSrcConversation->m_string.String),lpSrcConversation->m_string.String);

			for (int iContents=0;iContents<100;iContents++)
			{
				CContentsText	*lpSrcContents	=	lpSrcConversation->m_apContents[iContents];

				if (!lpSrcContents || lpSrcContents->m_iCheckSum	==	0x12345678)
					continue;

				if (lpSrcContents->m_iCheckSum	!=	0x87654321)
					file.writeStreamCR("    #area_contents [%d] %d \"%s\"",GetTextCheckSum(lpSrcContents->m_string.String),iContents,lpSrcContents->m_string.String);

				for (int iKarma=0;iKarma<100;iKarma++)
				{
					cKarmaText	*lpSrcKarma	=	lpSrcContents->m_apKarma[iKarma];

					if	(!lpSrcKarma)
						continue;

					for (int iReaction=0;iReaction<100;iReaction++)
					{
						CReactionText	*lpSrcReaction	=	lpSrcKarma->m_apReaction[iReaction];

						if (!lpSrcReaction || lpSrcReaction->m_iCheckSum	==	0x12345678)
							continue;

						file.writeStreamCR("        #area_reaction [%d] %d,%d \"%s\"",GetTextCheckSum(lpSrcReaction->m_string.String),lpSrcReaction->m_iKarma,iReaction,lpSrcReaction->m_string.String);
					}
				}
			}
		}

		file.writeStreamCR("  ");
	}

	for (iShop=0;iShop<1024;iShop++)
	{
		cSTRING	*lpString	=	&l_fieldSrc.m_aShop[iShop].m_string;

		if (l_fieldSrc.m_aShop[iShop].m_iCheckSum)
			file.writeStreamCR("#shop [%d] %d \"%s\"",GetTextCheckSum(lpString->String),iShop,lpString->String);
	}

	BOOL	bIsValidFile	=	FALSE;

	if (file.Locate()	<=	4)
		bIsValidFile	=	TRUE;

	file.Close();

	if (bIsValidFile)
		DeleteFile(strFileName);

	return	TRUE;
}

int
cMAP::ModifyText(char *_lpstrFileName,BOOL _bIsSD)
{
	char	strFileName[512];

	if	(_bIsSD)
		sprintf(strFileName,"%s/localize/%s/old/(expansion)%s.txt",_logFolder,MAIN.m_strLocalizeFolder,_lpstrFileName);
	else
		sprintf(strFileName,"%s/localize/%s/old/%s.txt",_logFolder,MAIN.m_strLocalizeFolder,_lpstrFileName);

	if	(!UploadTextForCompare(strFileName,&l_fieldSrc))
		return	FALSE;

	if	(_bIsSD)
		sprintf(strFileName,"%s/localize/%s/_want modify/(expansion)%s.txt",_logFolder,MAIN.m_strLocalizeFolder,_lpstrFileName);
	else
		sprintf(strFileName,"%s/localize/%s/_want modify/%s.txt",_logFolder,MAIN.m_strLocalizeFolder,_lpstrFileName);

	if	(!UploadTextForCompare(strFileName,&l_fieldDest))
		return	FALSE;

	if	(_bIsSD)
		sprintf(strFileName,"%s/localize/%s/_modify result/(expansion)%s.txt",_logFolder,MAIN.m_strLocalizeFolder,_lpstrFileName);
	else
		sprintf(strFileName,"%s/localize/%s/_modify result/%s.txt",_logFolder,MAIN.m_strLocalizeFolder,_lpstrFileName);

	cFILE	file;

	if (!file.Open(strFileName,"wb"))
	{
		cMSG::Error("can not open follow text file",strFileName);
		return	FALSE;
	}

	int	iCharacter,iActor,iArea,iShop,iFK;

	if (l_fieldDest.m_iFieldNameCheckSum	!=	0x12345678)
		file.writeStreamCR("#fieldName [%d] \"%s\"\n",l_fieldDest.m_iFieldNameCheckSum,l_fieldDest.m_string.String);
	else
		file.writeStreamCR("#fieldName [%d] \"%s\"\n",l_fieldSrc.m_iFieldNameCheckSum,l_fieldSrc.m_string.String);

	for	(iFK=0;iFK<1024;iFK++)
	{
		CKarmaText	*lpSrcKarma		=	&l_fieldSrc.m_aKarma[iFK];
		CKarmaText	*lpDestKarma	=	&l_fieldDest.m_aKarma[iFK];

		if	(lpSrcKarma->m_iCheckSum	==	0x12345678	&&	lpDestKarma->m_iCheckSum	==	0x12345678)
			continue;

		for (int iReaction=0;iReaction<100;iReaction++)
		{
			CReactionText	*lpSrcReaction	=	lpSrcKarma->m_apReaction[iReaction];
			CReactionText	*lpDestReaction	=	lpDestKarma->m_apReaction[iReaction];

			if	(!lpSrcReaction && !lpDestReaction)
				continue;

			if	(lpDestReaction && lpDestReaction->m_iCheckSum	!=	0x12345678)
				lpSrcReaction	=	lpDestReaction;

			file.writeStreamCR("#field_karma_reaction [%d] %d,%d \"%s\"\n",lpSrcReaction->m_iCheckSum,lpSrcReaction->m_iKarma,iReaction,lpSrcReaction->m_string.String);
		}
	}

	for (iActor=0;iActor<1024;iActor++)
	{
		CActorText	*lpSrcActor		=	&l_fieldSrc.m_aActor[iActor];
		CActorText	*lpDestActor	=	&l_fieldDest.m_aActor[iActor];

		if	(lpSrcActor->m_iCheckSum	==	0x12345678	&&	lpDestActor->m_iCheckSum	==	0x12345678)
			continue;

		if	(lpDestActor->m_iCheckSum	!=	0x12345678)
			file.writeStreamCR("#actor_name [%d] %d \"%s\"",lpDestActor->m_iCheckSum,iActor,lpDestActor->m_string.String);
		else
			file.writeStreamCR("#actor_name [%d] %d \"%s\"",lpSrcActor->m_iCheckSum,iActor,lpSrcActor->m_string.String);

		for (int iConversation=0;iConversation<c_iLocalizeContentsCount;iConversation++)
		{
			CConversationText	*lpSrcConversation	=	lpSrcActor->m_apConversation[iConversation];
			CConversationText	*lpDestConversation	=	lpDestActor->m_apConversation[iConversation];

			if	(
					(!lpSrcConversation || lpSrcConversation->m_iCheckSum	==	0x12345678)	&&	
					(!lpDestConversation||	lpDestConversation->m_iCheckSum	==	0x12345678)
				)
			{
				continue;
			}

			BOOL	bIsPutSrcConversation=FALSE,bIsPutDestConversation=FALSE;

			if	(lpSrcConversation && lpSrcConversation->m_iCheckSum	!=	0x87654321	&& lpSrcConversation->m_iCheckSum	!=	0x12345678)
			{
				bIsPutSrcConversation	=	TRUE;

				if	(lpDestConversation	&&	lpDestConversation->m_iCheckSum	!=	0x12345678	&&	lpSrcConversation->m_iCheckSum	!=	lpDestConversation->m_iCheckSum)
				{
					bIsPutSrcConversation	=	FALSE;
					bIsPutDestConversation	=	TRUE;
				}
			}
			else
			if	(lpDestConversation && lpDestConversation->m_iCheckSum	!=	0x87654321	&& lpDestConversation->m_iCheckSum	!=	0x12345678)
				bIsPutDestConversation	=	TRUE;

			if	(bIsPutSrcConversation)
				file.writeStreamCR("	#actor_speech [%d] %d,%d \"%s\"",lpSrcConversation->m_iCheckSum,iActor,iConversation,lpSrcConversation->m_string.String);
			if	(bIsPutDestConversation)
				file.writeStreamCR("	#actor_speech [%d] %d,%d \"%s\"",lpDestConversation->m_iCheckSum,iActor,iConversation,lpDestConversation->m_string.String);

			for (int iContents=0;iContents<c_iLocalizeContentsCount;iContents++)
			{
				CContentsText	*lpSrcContents	=	NULL;
				CContentsText	*lpDestContents	=	NULL;

				if	(lpSrcConversation)
					lpSrcContents	=	lpSrcConversation->m_apContents[iContents];
				if	(lpDestConversation)
					lpDestContents	=	lpDestConversation->m_apContents[iContents];

				if	((!lpSrcContents || lpSrcContents->m_iCheckSum	==	0x12345678) && (!lpDestContents || lpDestContents->m_iCheckSum	==	0x12345678) )
					continue;

				BOOL	bIsPutSrcContents=FALSE,bIsPutDestContents=FALSE;

				if	(lpSrcContents && lpSrcContents->m_iCheckSum	!=	0x87654321	&& lpSrcContents->m_iCheckSum	!=	0x12345678)
				{
					bIsPutSrcContents	=	TRUE;

					if	(lpDestContents	&&	lpDestContents->m_iCheckSum	!=	0x12345678	&&	lpSrcContents->m_iCheckSum	!=	lpDestContents->m_iCheckSum)
					{
						bIsPutSrcContents	=	FALSE;
						bIsPutDestContents	=	TRUE;
					}
				}
				else
				if	(lpDestContents	&& lpDestContents->m_iCheckSum	!=	0x87654321	&& lpDestContents->m_iCheckSum	!=	0x12345678)
					bIsPutDestContents	=	TRUE;

				if	(bIsPutSrcContents)
					file.writeStreamCR("		#actor_contents	[%d] %d,%d,%d \"%s\"",lpSrcContents->m_iCheckSum,iActor,iConversation,iContents,lpSrcContents->m_string.String);
				if	(bIsPutDestContents)
					file.writeStreamCR("		#actor_contents	[%d] %d,%d,%d \"%s\"",lpDestContents->m_iCheckSum,iActor,iConversation,iContents,lpDestContents->m_string.String);


				for (int iKarma=0;iKarma<c_iLocalizeContentsCount;iKarma++)
				{
					cKarmaText	*lpSrcKarma		=	NULL;
					cKarmaText	*lpDestKarma	=	NULL;

					if	(lpSrcContents)
						lpSrcKarma		=	lpSrcContents->m_apKarma[iKarma];
					if	(lpDestContents)
						lpDestKarma		=	lpDestContents->m_apKarma[iKarma];

					if	(!lpSrcKarma && !lpDestKarma)
						continue;

					for (int iReaction=0;iReaction<c_iLocalizeContentsCount;iReaction++)
					{
						CReactionText	*lpSrcReaction	=	NULL;
						CReactionText	*lpDestReaction	=	NULL;

						if	(lpSrcKarma)
							lpSrcReaction	=	lpSrcKarma->m_apReaction[iReaction];

						if	(lpDestKarma)
							lpDestReaction	=	lpDestKarma->m_apReaction[iReaction];

						if	((!lpSrcReaction || lpSrcReaction->m_iCheckSum	==	0x12345678) && (!lpDestReaction || lpDestReaction->m_iCheckSum	==	0x12345678) )
							continue;

						BOOL	bIsPutSrcReaction=FALSE,bIsPutDestReaction=FALSE;

						if	(lpSrcReaction && lpSrcReaction->m_iCheckSum	!=	0x87654321	&& lpSrcReaction->m_iCheckSum	!=	0x12345678)
						{
							bIsPutSrcReaction		=	TRUE;

							if	(lpDestReaction	&&	lpSrcReaction->m_iCheckSum	!=	lpDestReaction->m_iCheckSum)
							{
								bIsPutSrcReaction	=	FALSE;
								bIsPutDestReaction	=	TRUE;
							}
						}
						else
						if	(lpDestReaction	&& lpDestReaction->m_iCheckSum	!=	0x87654321	&& lpDestReaction->m_iCheckSum	!=	0x12345678)
							bIsPutDestReaction	=	TRUE;

						if	(bIsPutSrcReaction)
							file.writeStreamCR("			#actor_reaction	[%d] %d,%d,%d,%d,%d \"%s\"",lpSrcReaction->m_iCheckSum,iActor,iConversation,iContents,iKarma,iReaction,lpSrcReaction->m_string.String);
						if	(bIsPutDestReaction)
							file.writeStreamCR("			#actor_reaction	[%d] %d,%d,%d,%d,%d \"%s\"",lpDestReaction->m_iCheckSum,iActor,iConversation,iContents,iKarma,iReaction,lpDestReaction->m_string.String);
					}
				}
			}
		}

		file.writeStreamCR("  ");
	}

	for (iCharacter=0;iCharacter<1024;iCharacter++)
	{
		cCharacterText	*lpSrcCharacter	=	&l_fieldSrc.m_aCharacter[iCharacter];
		cCharacterText	*lpDestCharacter=	&l_fieldDest.m_aCharacter[iCharacter];

		if	(lpDestCharacter->m_iCheckSum	!=	0x12345678)
			lpSrcCharacter	=	lpDestCharacter;

		if	(lpSrcCharacter->m_iCheckSum	==	0x12345678)
			continue;

		file.writeStreamCR("#character_name %d [%d] \"%s\"",iCharacter,lpSrcCharacter->m_iCheckSum,lpSrcCharacter->m_strName.String);

		for (int iEvent=0;iEvent<100;iEvent++)
		{
			CContentsText	*lpSrcEvent	=	lpSrcCharacter->m_apContents[iEvent];

			if (!lpSrcEvent)
				continue;

			for (int iKarma=0;iKarma<100;iKarma++)
			{
				cKarmaText	*lpSrcKarma	=	lpSrcEvent->m_apKarma[iKarma];

				if (!lpSrcKarma)
					continue;

				for (int iReaction=0;iReaction<100;iReaction++)
				{
					CReactionText	*lpSrcReaction	=	lpSrcKarma->m_apReaction[iReaction];

					if (!lpSrcReaction || lpSrcReaction->m_iCheckSum	==	0x12345678)
						continue;

					file.writeStreamCR("    #character_event [%d] %d,%d,%d \"%s\"",lpSrcReaction->m_iCheckSum,iEvent,lpSrcReaction->m_iKarma,iReaction,lpSrcReaction->m_string.String);
				}
			}
		}

		file.writeStreamCR("  ");
	}

	for (iArea=0;iArea<1024;iArea++)
	{
		CActorText	*lpSrcActor		=	&l_fieldSrc.m_aArea[iArea];
		CActorText	*lpDestActor	=	&l_fieldDest.m_aArea[iArea];

		if (lpDestActor->m_iCheckSum!=	0x12345678)
			lpSrcActor	=	lpDestActor;
		
		if (lpSrcActor->m_iCheckSum	==	0x12345678)
			continue;

		file.writeStreamCR("#area [%d] %d \"%s\"",lpSrcActor->m_iCheckSum,iArea,lpSrcActor->m_string.String);

		for (int iConversation=0;iConversation<100;iConversation++)
		{
			CConversationText	*lpSrcConversation	=	lpSrcActor->m_apConversation[iConversation];

			if (!lpSrcConversation || lpSrcConversation->m_iCheckSum	==	0x12345678)
				continue;

			file.writeStreamCR("  #area_event %d",iConversation);

			if (!lpSrcConversation || lpSrcConversation->m_iCheckSum	!=	0x87654321)
				file.writeStreamCR("    #area_speech [%d] \"%s\"",lpSrcConversation->m_iCheckSum,lpSrcConversation->m_string.String);

			for (int iContents=0;iContents<100;iContents++)
			{
				CContentsText	*lpSrcContents	=	lpSrcConversation->m_apContents[iContents];

				if (!lpSrcContents || lpSrcContents->m_iCheckSum	==	0x12345678)
					continue;

				if (lpSrcContents->m_iCheckSum	!=	0x87654321)
					file.writeStreamCR("    #area_contents [%d] %d \"%s\"",lpSrcContents->m_iCheckSum,iContents,lpSrcContents->m_string.String);

				for (int iKarma=0;iKarma<100;iKarma++)
				{
					cKarmaText	*lpSrcKarma	=	lpSrcContents->m_apKarma[iKarma];

					if	(!lpSrcKarma)
						continue;

					for (int iReaction=0;iReaction<100;iReaction++)
					{
						CReactionText	*lpSrcReaction	=	lpSrcKarma->m_apReaction[iReaction];

						if (!lpSrcReaction || lpSrcReaction->m_iCheckSum	==	0x12345678)
							continue;

						file.writeStreamCR("        #area_reaction [%d] %d,%d \"%s\"",lpSrcReaction->m_iCheckSum,lpSrcReaction->m_iKarma,iReaction,lpSrcReaction->m_string.String);
					}
				}
			}
		}

		file.writeStreamCR("  ");
	}

	for (iShop=0;iShop<1024;iShop++)
	{
		CShopText	*lpSrcShop	=	&l_fieldSrc.m_aShop[iShop];
		CShopText	*lpDestShop	=	&l_fieldDest.m_aShop[iShop];

		if (lpDestShop->m_iCheckSum	!=	0x12345678)
			lpSrcShop	=	lpDestShop;

		if (lpSrcShop->m_iCheckSum	==	0x12345678)
			continue;

		file.writeStreamCR("#shop [%d] %d \"%s\"",lpSrcShop->m_iCheckSum,iShop,lpSrcShop->m_string.String);
	}

	BOOL	bIsValidFile	=	FALSE;

	if (file.Locate()	<=	4)
		bIsValidFile	=	TRUE;

	file.Close();

	if (bIsValidFile)
		DeleteFile(strFileName);

	return	TRUE;
}