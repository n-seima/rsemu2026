#include	"movie.h"
#include	"cDECODER.h"
#include	"cDRAW.h"
#include	"cGAME.h"
#include	"cMap.h"

BOOL
cMovie::load(char *_lpstrFile)
{
	cFILE	file;

	if	(!file.Open(_lpstrFile,"rb"))
		return	FALSE;

	__destroy();

	file.Read(&m_wEventCount,sizeof(m_wEventCount));
	file.Read(&m_iFPS,sizeof(m_iFPS));

	m_pEvents	=	new cMovieEvent [m_wEventCount];

	for(int i=0;i<m_wEventCount;i++)
	{
		cMovieEvent*	lpEvent	=	&m_pEvents[i];
		if(!lpEvent)
			continue;
		file.Read(&lpEvent->m_wFrame,sizeof(lpEvent->m_wFrame));
		file.Read(&lpEvent->m_wWorkCount,sizeof(lpEvent->m_wWorkCount));

		lpEvent->init(lpEvent->m_wWorkCount);

		for	(int iWork=0;iWork<lpEvent->m_wWorkCount;iWork++)
		{
			cMovieFrame	*lpFrame	=	&lpEvent->m_pWork[iWork];

			file.Read(&lpFrame->m_wWork,sizeof(lpFrame->m_wWork));
			file.Read(&lpFrame->m_wBufferSize,sizeof(lpFrame->m_wBufferSize));

			if	(lpFrame->m_wBufferSize)
			{
				lpFrame->alloc(lpFrame->m_wWork);
				file.Read(lpFrame->m_pBuffer,lpFrame->m_wBufferSize);

				if	(lpFrame->m_wWork	==	eMovie_play_anm)
				{
					lpFrame->getPlayAnm()->m_iXPos	=	lpFrame->getPlayAnm()->m_iXPos*g_iGameScale/100;
					lpFrame->getPlayAnm()->m_iYPos	=	lpFrame->getPlayAnm()->m_iYPos*g_iGameScale/100;
				}
				if	(lpFrame->m_wWork	==	eMovie_change_anm_pos)
				{
					lpFrame->getChangeAnmPos()->m_iXPos	=	lpFrame->getChangeAnmPos()->m_iXPos*g_iGameScale/100;
					lpFrame->getChangeAnmPos()->m_iYPos	=	lpFrame->getChangeAnmPos()->m_iYPos*g_iGameScale/100;
				}
			}
		}
	}

	file.Close();

	init();

	return	TRUE;
}

BOOL
cMovie::save(char *_lpstrFile)
{
	cFILE	file;

	if	(!file.Open(_lpstrFile,"wb"))
		return	FALSE;

	file.Write(&m_wEventCount,sizeof(m_wEventCount));
	file.Write(&m_iFPS,sizeof(m_iFPS));

	for(int i=0;i<m_wEventCount;i++)
	{
		cMovieEvent*	lpEvent	=	&m_pEvents[i];

		file.Write(&lpEvent->m_wFrame,sizeof(lpEvent->m_wFrame));
		file.Write(&lpEvent->m_wWorkCount,sizeof(lpEvent->m_wWorkCount));

		for	(int iWork=0;iWork<lpEvent->m_wWorkCount;iWork++)
		{
			cMovieFrame	*lpFrame	=	&lpEvent->m_pWork[iWork];

			file.Write(&lpFrame->m_wWork,sizeof(lpFrame->m_wWork));
			file.Write(&lpFrame->m_wBufferSize,sizeof(lpFrame->m_wBufferSize));

			if	(lpFrame->m_wBufferSize)
				file.Write(lpFrame->m_pBuffer,lpFrame->m_wBufferSize);
		}
	}

	file.Close();

	return	TRUE;
}

BOOL
cMovie::loadScript(char *_lpstrScriptFile,int _iFPS)
{
	CDecoderKeyword	aMovieEventColor[128]	=
	{
		{	"white"			,_WHITE		},
		{	"black"			,_BLACK		},
		{	""				,dCODE_END	}
	};


	CDecoder	decoder;

	if	(!decoder.Upload(_lpstrScriptFile))
		return	FALSE;

	m_iFPS		=	_iFPS;

	__destroy();

	m_pEvents	=	new cMovieEvent [256];

	int			iCurrentTime	=	-1;

	cMovieEvent	event;

	while(1)
	{
		char		*lpstrKeyWord	=	decoder.getWord();

		if	(stricmp(lpstrKeyWord,"end")	==	0)
		{
			m_pEvents[m_wEventCount].copy(&event);	//	이벤트 복사
			m_wEventCount++;
			break;
		}

		if	(stricmp(lpstrKeyWord,dDC_WORD_FINISH)	==	0)
			continue;
		if	(stricmp(lpstrKeyWord,"time")	!=	0)
		{
			break;
		}

		int	iTime	=	decoder.getNumber();

		if	(iTime	!=	iCurrentTime)	//	시간이 바뀌었다.
		{
			if	(iTime	<	iCurrentTime)
				return	ERRMSG("무비 스크립터 디코드 에러!!","'%s' 파일에 시간이 순서대로 배열되지 않았습니다.",_lpstrScriptFile);

			if	(iCurrentTime	!=	-1)	//	처음이 아니다.
			{
				m_pEvents[m_wEventCount].copy(&event);	//	이벤트 복사
				m_wEventCount++;
			}

			//	편집 이벤트를 초기화 하고 시간 설정
			{
				event.init(256);
				event.m_wWorkCount	=	0;
				event.m_wFrame		=	iTime*m_iFPS/100;
			}
		}

		iCurrentTime	=	iTime;

		lpstrKeyWord	=	decoder.getWord();

		cMovieFrame	*lpCurFrame		=	&event.m_pWork[event.m_wWorkCount];

		if	(stricmp(lpstrKeyWord,"load_anm")	==	0)
		{
			int		iIndex			=	decoder.getNumber();
			char	*lpstrFileName	=	decoder.getStream();

			lpCurFrame->setLoadAnm(iIndex,lpstrFileName);
		}
		else
		if	(stricmp(lpstrKeyWord,"load_image")	==	0)
		{
			char	*lpstrFileName	=	decoder.getStream();

			lpCurFrame->setLoadImage(lpstrFileName);
		}
		else
		if	(stricmp(lpstrKeyWord,"play_wav")	==	0)
		{
			char	*lpstrFileName	=	decoder.getStream();

			lpCurFrame->setPlayWav(lpstrFileName);
		}
		else
		if	(stricmp(lpstrKeyWord,"put_image")	==	0)
		{
			char	*lpstrPos		=	decoder.getWord();
			int		iX				=	decoder.getNumber();
			int		iY				=	decoder.getNumber();

			char	*lpstrImage		=	decoder.getWord();
			int		iImage			=	decoder.getNumber();

			char	*lpstrLayer		=	decoder.getWord();
			int		iLayer			=	decoder.getNumber();

			lpCurFrame->setPutImage(iX,iY,iImage,iLayer);
		}
		else
		if	(stricmp(lpstrKeyWord,"cancel_put_image")	==	0)
		{
			lpCurFrame->setCancelPutImage();
		}
		else
		if	(stricmp(lpstrKeyWord,"change_anm_pos")	==	0)
		{
			int		iIndex		=	decoder.getNumber();
			int		iX,iY;

			while(1)
			{
				char	*lpstrWord	=	decoder.getWord();

				if	(stricmp(lpstrWord,"pos")==0)
				{
					iX			=	decoder.getNumber();
					iY			=	decoder.getNumber();
				}
				else
				if	(stricmp(lpstrWord,dDC_WORD_FINISH)==0)
					break;
				else
				if	(stricmp(lpstrWord,dDC_WORD_END)==0)
					return	FALSE;
			}

			lpCurFrame->setChangeAnmPos(iIndex,iX*dTILE_XS+dTILE_XS/2,iY*dTILE_YS+dTILE_YS/2);
		}
		else
		if	(stricmp(lpstrKeyWord,"delay_anm")	==	0)
		{
			int		iIndex		=	decoder.getNumber();
			int		iTime;
			
			while(1)
			{
				char	*lpstrWord	=	decoder.getWord();
				
				if	(stricmp(lpstrWord,"time")==0)
				{
					iTime	=	decoder.getNumber();
				}
				else
				if	(stricmp(lpstrWord,dDC_WORD_FINISH)==0)
					break;
				else
					if	(stricmp(lpstrWord,dDC_WORD_END)==0)
						return	FALSE;
			}
			
			lpCurFrame->setDelayAnm(iIndex,iTime);
		}
		else
		if	(stricmp(lpstrKeyWord,"play_anm")	==	0)
		{
			int		iIndex		=	decoder.getNumber();

			int		iAnm,iX,iY,iLayer;
			int		iScale		=	100;
			BOOL	bIsInvertPutShadowOrder	=	FALSE;

			while(1)
			{
				char	*lpstrWord	=	decoder.getWord();

				if	(stricmp(lpstrWord,"anm")==0)
				{
					iAnm		=	decoder.getNumber();
				}
				else
				if	(stricmp(lpstrWord,"pos")==0)
				{
					iX			=	decoder.getNumber();
					iY			=	decoder.getNumber();
				}
				else
				if	(stricmp(lpstrWord,"layer")==0)
				{
					iLayer		=	decoder.getNumber();
				}
				else
				if	(stricmp(lpstrWord,"scale")==0)
				{
					iScale		=	decoder.getNumber();
				}
				else
				if	(stricmp(lpstrWord,"invert_put_shadow_order")==0)
				{
					bIsInvertPutShadowOrder	=	TRUE;
				}
				else
				if	(stricmp(lpstrWord,dDC_WORD_FINISH)==0)
					break;
				else
				if	(stricmp(lpstrWord,dDC_WORD_END)==0)
					return	FALSE;
			}

			lpCurFrame->setPlayAnm(iIndex,iAnm,iX*dTILE_XS+dTILE_XS/2,iY*dTILE_YS+dTILE_YS/2,iLayer,iScale,bIsInvertPutShadowOrder);
		}
		else
		if	(stricmp(lpstrKeyWord,"set_fade_screen_filter")	==	0)
		{
			char	*lpstrBeginColor	=	decoder.getWord();
			WORD	wBeginColor			=	(WORD)CDecoder::AnalCode(aMovieEventColor,lpstrBeginColor);
			int		iBeginDepth			=	decoder.getNumber();
			char	*lpstrEndColor		=	decoder.getWord();
			WORD	wEndColor			=	(WORD)CDecoder::AnalCode(aMovieEventColor,lpstrEndColor);
			int		iEndDepth			=	decoder.getNumber();
			char	*lpstrSpeed			=	decoder.getWord();
			int		iOperateTime		=	decoder.getNumber();
			char	*lpstrLayer			=	decoder.getWord();
			int		iLayer				=	decoder.getNumber();

			lpCurFrame->setFadeScreenFilter(wBeginColor,iBeginDepth,wEndColor,iEndDepth,iOperateTime,iLayer);
		}
		else
		if	(stricmp(lpstrKeyWord,"set_focus")	==	0)
		{
			char	*lpstrMethod		=	decoder.getWord();
			WORD	wWork;
			BOOL	bSkipRemain			=	FALSE;

			if	(stricmp(lpstrMethod,"scroll")	==	0)
				wWork	=	cMF_SetFocus::eScroll;
			else
			if	(stricmp(lpstrMethod,"set")	==	0)
				wWork	=	cMF_SetFocus::eSet;
			else
			if	(stricmp(lpstrMethod,"cancel")	==	0)
			{
				lpCurFrame->setSetFocus(cMF_SetFocus::eCancel,0,0,0);
				bSkipRemain	=	TRUE;
			}
			else
				ERRMSG("set_focus","%s가 뭐에요?",lpstrMethod);

			if	(bSkipRemain	==	FALSE)
			{
				char	*lpstrPos	=	decoder.getWord();
				int		iX			=	decoder.getNumber();
				int		iY			=	decoder.getNumber();

				char	*lpstrSpeed	=	decoder.getWord();
				int		iSpeed		=	decoder.getNumber();

				lpCurFrame->setSetFocus(wWork,iX,iY,iSpeed);	
			}
		}
		else
		if	(stricmp(lpstrKeyWord,"draw_map")	==	0)
		{
			char	*lpstrOnOff			=	decoder.getWord();

			int		iOnOff;

			if	(stricmp(lpstrOnOff,"on")	==	0)
				iOnOff	=	0;
			else
			if	(stricmp(lpstrOnOff,"off")	==	0)
				iOnOff	=	1;
			else
				ERRMSG("draw_map","%s가 뭐에요?",lpstrOnOff);

			lpCurFrame->setDrawMap(iOnOff);
		}
		else
		if	(stricmp(lpstrKeyWord,"end")	==	0)
		{
			lpCurFrame->setEnd();
		}
		else
		{
			break;
		}

		event.m_wWorkCount++;
	}

	return	TRUE;
}

