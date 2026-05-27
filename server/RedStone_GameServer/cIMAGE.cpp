#include <stdio.h>
#include "cIMAGE.H"

tsSADHEADER SADHEADER;

/********************************************************************
	에니메이션 스프라이트 데이터
********************************************************************/
cANMDATA::cANMDATA()
{	m_pReleasePos	=	NULL;
	m_pPos			=	NULL;
	m_pIsEvent		=	NULL;

	m_iFrameCount	=	0;
	m_iDirectCount	=	0;
	m_isControledByMcu			=	FALSE;

	m_iLoopBegin	=	0xffff;
	m_iLoopEnd		=	0xffff;
}

cANMDATA::~cANMDATA()
{	reset();
}

void
cANMDATA::reset()
{	if (!m_isControledByMcu)
	{	Free(m_pPos);
		Free(m_pIsEvent);
		Free(m_pReleasePos);
	}

	m_pPos			=	NULL;
	m_pIsEvent		=	NULL;
	m_pReleasePos	=	NULL;
}

BOOL
cANMDATA::init(cFILE *file)
{	reset();

	file->Read(&m_iFrameCount	,4);

	if (m_iFrameCount > 0)
	{
		cANM_INFO	anmInfo;

		file->Read(&anmInfo,sizeof(cANM_INFO));

		m_iFPS	=	anmInfo.m_iFPS;
		m_iPPS	=	anmInfo.m_iPPS;

		m_pIsEvent		=	New(BYTE,m_iFrameCount,NULL,"event");

		file->Read(m_pIsEvent	,m_iFrameCount);						//	이벤트 정보
		file->Skip(m_iFrameCount*anmInfo.m_iDirectCount*2);		//	프레임별 스프라이트 정보
		file->Skip(m_iFrameCount*anmInfo.m_iDirectCount);		//	뒤에 깔리는 스프라이트

		if (anmInfo.m_isRELEASE)
		{
			m_pReleasePos	=	New(POINTS,anmInfo.m_iDirectCount,NULL,"POINTS");
			file->Read(m_pReleasePos	,anmInfo.m_iDirectCount,4);
		}

		if (anmInfo.m_isREFITFRAME)//	프레임 보정 위치
		{	m_pPos	=	New(POINTS,m_iFrameCount*anmInfo.m_iDirectCount,NULL,"cANMDATA::m_pPos");
			if (!m_pPos)	return FALSE;

			file->Read(m_pPos	,m_iFrameCount*anmInfo.m_iDirectCount,4);
		}

		for (int i=0;i<m_iFrameCount;i++)
		{
			if (m_pIsEvent[i]	&	dEVENT_LOOP_START	)	m_iLoopBegin	=	i;
			if (m_pIsEvent[i]	&	dEVENT_LOOP_END		)	m_iLoopEnd		=	i;
		}
	}

	return TRUE;
}

/********************************************************************
	퀵 로드 에니메이션 스프라이트(리얼타임으로 불러서 찍는다.)
********************************************************************/

//
//
cBODY::cBODY()
{	m_pANM		=	NULL;
	m_iAnmCount	=	0	;
}	//	cBODY::cBODY()

//
//
cBODY::~cBODY()
{	reset();
}	//	cBODY::~cBODY()

//
//	데이터 초기화
void
cBODY::reset()
{
	pKILL(m_pANM);
}	//	cBODY::Reset()

//
//	
BOOL
cBODY::load(char *fn,cNUX *nux)
{	cFILE	*file;
	int		i;

	if (nux)	file	=	nux->Open(fn);
	else
	{	file	=	new cFILE();
		if (!file->Open(fn,"rb"))	KILL(file);
	}

	if (!file)	return MSGOUT("Error in cANM::Load - '%s' file not found",fn);

	file->Read(&SADHEADER,sizeof(SADHEADER));

	if	(strcmp(SADHEADER.Reg,REGSADHEADER)!=0	&&	strcmp(SADHEADER.Reg,REGSADHEADER2)!=0)
		return MSGOUT("Error in cANM::Load - header not match!!");

	m_iAnmCount			=	SADHEADER.AnmCount;
	m_iSpriteCount		=	SADHEADER.ImageCount;

	reset();	//	몽땅 초기화

//	스프라이트/그림자/외곽선 데이터 스킵
	{
		int		size;

		if (SADHEADER.BPP	==	8)	file->Skip(512);	//	팔레트 스킵

//	스프라이트	스킵
		{
			file->Skip(m_iSpriteCount*4);
			file->Read(&size,4);
			if (SADHEADER.BPP	!=	8)	size*=2;
			file->Skip(size);
		}	//	스프라이트	스킵

//	그림자	스킵
		if (SADHEADER.bShadow)
		{
			file->Skip(m_iSpriteCount*4);
			file->Read(&size,4);
			file->Skip(size);	//	팔레트 스킵
		}//	그림자	스킵에

//	외곽선 스킵
//		if (SADHEADER.bOutline)
//		{
			file->Skip(m_iSpriteCount*4);
			file->Read(&size,4);
			file->Skip(size);	//	팔레트 스킵
//		}//	외곽선 스킵
	}	//	스프라이트/그림자/외곽선 데이터 스킵

	//	에니메이션 데이터 로딩
	m_pANM	=	new cANMDATA [m_iAnmCount];

	file->Skip(4+16+16+4+8);	//	각종 데이터 스킵

	file->Read(&m_iCrashSize,4);
	file->Read(&m_iDefaultAttack,4);
	
	WORD	wDefaultMagic;
	file->Read(&wDefaultMagic,2);
	m_iDefaultMagic	=	wDefaultMagic;
	file->Read(&wDefaultMagic,2);	//	클라이언트

	for (i=0;i<m_iAnmCount;i++)	m_pANM[i].init(file);
	
	if (!nux)	KILL(file);

	return TRUE;
}	//	cBODY::load(char *fn,BOOL loadpalette,cNUX *nux,cMCU *mcu)