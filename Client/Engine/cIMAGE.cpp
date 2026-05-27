#include <stdio.h>
#include "cIMAGE.H"
#include "cDRAW.H"
#include "cTRANS.H"
#include "cANGLE.H"
#include "cBARMENU.H"
#include "CExceptionHandler.H"

#define	dMAX_IMAGE_SIZE	204800

tsSDHEADER	SDHEADER;
tsSADHEADER SADHEADER;

char	*l_lpLoadFileName;
BYTE	*l_pBuffer;

BOOL	l_bIsWantAutoConvert	=	FALSE;

/********************************************************************
	스프라이트 데이터
********************************************************************/

cSPRITE::cSPRITE()
{
	m_p16Sprite	=	NULL,	m_p8Sprite		=	NULL,	m_pSpriteOffset	=	NULL;
	m_pLayer	=	NULL,	m_pLayerOffset	=	NULL;
	m_pShadow	=	NULL,	m_pShadowOffset=	NULL;
	m_pPlt		=	NULL;
	m_iCount	=	0	,	m_iBpp			=	dSPRITE_BPP16;
	m_isControledByMcu		=	FALSE;
	m_iSerial	=	-1;
	m_isLoadedPlt	=	FALSE;
	m_lpPlt		=	NULL;
	m_iHeight	=	0;
}

cSPRITE::~cSPRITE()
{
	close();
}

void
cSPRITE::close()
{
	if (!m_isControledByMcu)
	{
		_Free(m_p16Sprite);
		_Free(m_p8Sprite);
		_Free(m_pLayer);
		_Free(m_pShadow);
		_Free(m_pSpriteOffset);
		_Free(m_pLayerOffset);
		_Free(m_pShadowOffset);

		if (m_isLoadedPlt)
			_Free(m_pPlt);
	}

	m_p16Sprite			=	NULL;
	m_p8Sprite			=	NULL;
	m_pLayer			=	NULL;
	m_pShadow			=	NULL;
	m_pSpriteOffset		=	NULL;
	m_pLayerOffset		=	NULL;
	m_pShadowOffset		=	NULL;
	m_pPlt				=	NULL;
	m_lpPlt				=	NULL;
	m_iSerial			=	-1;
}

void
cSPRITE::reset()
{
	m_iCount			=	0;
	m_iBpp				=	dSPRITE_BPP16;
	m_isControledByMcu	=	FALSE;
	m_iSerial			=	-1;
	m_isLoadedPlt		=	FALSE;
	m_iHeight			=	0;
}

void
cSPRITE::getMaxSize()
{
	m_iMaxSpriteWidth	=	0;
	m_iMaxSpriteHeight	=	0;

	m_iMaxShadowWidth	=	0;
	m_iMaxShadowHeight	=	0;

	getSpriteMaxSize(&m_iMaxSpriteWidth,&m_iMaxSpriteHeight);
	getShadowMaxSize(&m_iMaxShadowWidth,&m_iMaxShadowHeight);
}

void
cSPRITE::getSpriteMaxSize(int *_lpiMaxWidth,int *_lpiMaxHeight)
{
	GetSpriteMaxSize(m_p8Sprite,m_pSpriteOffset,m_iCount,_lpiMaxWidth,_lpiMaxHeight);
	GetSpriteMaxSize(m_p16Sprite,m_pSpriteOffset,m_iCount,_lpiMaxWidth,_lpiMaxHeight);
}

void
cSPRITE::getShadowMaxSize(int *_lpiMaxWidth,int *_lpiMaxHeight)
{
	GetSpriteMaxSize(m_pShadow,m_pShadowOffset,m_iCount,_lpiMaxWidth,_lpiMaxHeight);
}

void
cSPRITE::GetSpriteMaxSize(WORD *_lpSprite,int *_lpiOffset,int _iSpriteCount,int *_lpiMaxWidth,int *_lpiMaxHeight)
{
	int		iMX		=	0,iMY		=	0,i;
	int		iMaxDX	=	0,iMaxDY	=	0;
	int		iBP		=	0;
	cRECT	rectFrame;

	rectFrame.set(0xffff,0xffff,-0xffff,-0xffff);

	if	(!_lpSprite	||	!_lpiOffset)
		return;

	for	(i=0;i<_iSpriteCount;i++)
	{
		WORD	*lpSprite	=	_lpSprite+_lpiOffset[i];
		cRECT	rect;

		int		iXS		=	lpSprite[0];				//	스프라이트 사이즈
		int		iYS		=	lpSprite[1];
		int		iDX		=	lpSprite[2];
		int		iDY		=	lpSprite[3];

		rect.x1			=	-iDX;
		rect.y1			=	-iDY;
		rect.x2			=	iDX+iXS;
		rect.y2			=	iDY+iYS;

		iMaxDX			=	max(iMaxDX,iXS*2-iDX*2);
		iMaxDY			=	max(iMaxDY,iYS*2-iDY*2);
		iMaxDX			=	max(iMaxDX,iDX*2);
		iMaxDY			=	max(iMaxDY,iDY*2);

		rectFrame.x1	=	min(rectFrame.x1,rect.x1);
		rectFrame.y1	=	min(rectFrame.y1,rect.y1);
		rectFrame.x2	=	max(rectFrame.x2,rect.x2);
		rectFrame.y2	=	max(rectFrame.y2,rect.y2);
	}

	iMX				=	rectFrame.getWidth();
	iMY				=	rectFrame.getHeight();

	*_lpiMaxWidth	=	max(iMX,iMaxDX);
	*_lpiMaxHeight	=	max(iMY,iMaxDY);
}

void
cSPRITE::GetSpriteMaxSize(BYTE *_lpSprite,int *_lpiOffset,int _iSpriteCount,int *_lpiMaxWidth,int *_lpiMaxHeight)
{
	int		iMX		=	0,iMY		=	0,i;
	int		iMaxDX	=	0,iMaxDY	=	0;
	int		iBP		=	0;
	cRECT	rectFrame;

	rectFrame.set(0xffff,0xffff,-0xffff,-0xffff);

	if	(!_lpSprite	||	!_lpiOffset)
		return;

	for	(i=0;i<_iSpriteCount;i++)
	{
		BYTE	*lpSprite	=	_lpSprite+_lpiOffset[i];
		cRECT	rect;

		int		iXS		=	*(short *)(lpSprite		);				//	스프라이트 사이즈
		int		iYS		=	*(short *)(lpSprite+2	);
		int		iDX		=	*(short *)(lpSprite+4	);
		int		iDY		=	*(short *)(lpSprite+6	);

		rect.x1	=	-iDX;
		rect.y1	=	-iDY;
		rect.x2	=	iDX+iXS;
		rect.y2	=	iDY+iYS;

		iMaxDX	=	max(iMaxDX,iXS*2-iDX*2);
		iMaxDY	=	max(iMaxDY,iYS*2-iDY*2);
		iMaxDX	=	max(iMaxDX,iDX*2);
		iMaxDY	=	max(iMaxDY,iDY*2);

		rectFrame.x1	=	min(rectFrame.x1,rect.x1);
		rectFrame.y1	=	min(rectFrame.y1,rect.y1);
		rectFrame.x2	=	max(rectFrame.x2,rect.x2);
		rectFrame.y2	=	max(rectFrame.y2,rect.y2);
	}

	iMX				=	rectFrame.getWidth();
	iMY				=	rectFrame.getHeight();

	*_lpiMaxWidth	=	max(iMX,iMaxDX);
	*_lpiMaxHeight	=	max(iMY,iMaxDY);
}

BOOL
cSPRITE::isIn(int _iIndex,int _iX,int _iY,int _iPutX,int _iPutY)
{
	if	(m_iBpp==dSPRITE_BPP8)
	{
		BYTE	*lpSprite	=	Get8(_iIndex);

		if	(!lpSprite)
			return	FALSE;

		return	cDRAW::IsIn(lpSprite,_iX-_iPutX,_iY-_iPutY);
	}

	{
		WORD	*lpSprite	=	Get16(_iIndex);

		if	(!lpSprite)
			return	FALSE;

		return	cDRAW::IsIn(lpSprite,_iX-_iPutX,_iY-_iPutY);
	}
}

BOOL
cSPRITE::Load(char *fn,BOOL loadpalette,cNUX *nux,cMCU *mcu)
{
	cFILE	*file;

	if	(mcu)
		m_isControledByMcu	=	TRUE;
	else
		m_isControledByMcu	=	FALSE;

	if	(nux)
		file	=	nux->Open(fn);
	else
	{
		file	=	new cFILE();

		if	(!file->Open(fn,"rb"))
			KILL(file);
	}

	if	(!file)
		return ERRMSG("Error","Error in cSPRITE::Load - '%s' file not found",fn);

	file->Read(&SDHEADER,sizeof(SDHEADER));

	if	(strcmp(SDHEADER.Reg,REGSDHEADER)!=0	&&	strcmp(SDHEADER.Reg,REGSDHEADER2)	!=0	)
		return	ERRMSG("Error in cSPRITE::Load - header not match");

	m_iBpp		=	SDHEADER.BPP;
	m_isShadow	=	SDHEADER.bShadow;
	m_isLayer	=	SDHEADER.bOutline;
	m_iCount	=	SDHEADER.ImageCount;

	if	(m_iBpp	==	16)
		m_iBpp	=	dSPRITE_BPP16;
	else
		m_iBpp	=	dSPRITE_BPP8;

	BOOL result =	Load(file,loadpalette,mcu);

	KILL(file);

	if	(strcmp(SDHEADER.Reg,REGSDHEADER)	==	0)
	{
		getMaxSize();

		cFILE convertor;

		if	(l_bIsWantAutoConvert	&&	convertor.Open(fn,"rb+"))
		{
			strcpy(SDHEADER.Reg,REGSDHEADER2);

			SDHEADER.m_wMaxSpriteWidth		=	m_iMaxSpriteWidth	;
			SDHEADER.m_wMaxSpriteHeight		=	m_iMaxSpriteHeight	;
			SDHEADER.m_wMaxShadowWidth		=	m_iMaxShadowWidth	;
			SDHEADER.m_wMaxShadowHeight		=	m_iMaxShadowHeight	;

			convertor.Write(&SDHEADER,sizeof(SDHEADER));
			convertor.Close();
		}
	}
	else
	{
		m_iMaxSpriteWidth	=	SDHEADER.m_wMaxSpriteWidth;
		m_iMaxSpriteHeight	=	SDHEADER.m_wMaxSpriteHeight;
		m_iMaxShadowWidth	=	SDHEADER.m_wMaxShadowWidth;
		m_iMaxShadowHeight	=	SDHEADER.m_wMaxShadowHeight;
	}

	return result;
}

BOOL
cSPRITE::Load(cFILE *file,BOOL loadpalette,cMCU *mcu)
{
	close();

	if (mcu)
		m_isControledByMcu	=	TRUE;
	else
		m_isControledByMcu	=	FALSE;

	m_isLoadedPlt	=	loadpalette;

	if	(m_iBpp==dSPRITE_BPP8)
	{
		if	(loadpalette)
		{
			m_pPlt	=	_New(WORD,256,mcu,"cSPRITE::m_pPlt");

			if (!m_pPlt)	return FALSE;

			file->Read(m_pPlt,512);

			CONVERTPALETTE(m_pPlt);

			m_lpPlt	=	m_pPlt;
		}
		else
			file->Seek(512,SEEK_CUR);
	}
	//	스프라이트 데이터..
	m_pSpriteOffset	=	_New(int,m_iCount+1,mcu,"cSPRITE::m_pSpriteOffset");

	if	(!m_pSpriteOffset)
		return FALSE;

	file->Read(m_pSpriteOffset,(m_iCount+1),4);

	if (m_iBpp==dSPRITE_BPP16)
	{
		m_p16Sprite	=	_New(WORD,m_pSpriteOffset[m_iCount],mcu,"cSPRITE::m_p16Sprite");

		if (!m_p16Sprite)
			return FALSE;

		file->Read(m_p16Sprite	,m_pSpriteOffset[m_iCount],2);

		if (g_iPixelFormat != dRGB555)
		{
			for (int i=0;i<m_iCount;i++)
				CONVERTSPRITE(Get16(i));
		}
	}
	else
	{	
		m_p8Sprite	=	_New(BYTE,m_pSpriteOffset[m_iCount],mcu,"cSPRITE::m_p8Sprite");

		if (!m_p8Sprite)
			return FALSE;

		file->Read(m_p8Sprite	,m_pSpriteOffset[m_iCount]);
	}

	//	그림자 불러오기
	if (m_isShadow)
	{	
		m_pShadowOffset	=	_New(int,(m_iCount+1),mcu,"cSPRITE::m_pShadowOffset");

		if (!m_pShadowOffset) return FALSE;

		file->Read(m_pShadowOffset,m_iCount+1,4);

		if(m_pShadowOffset[m_iCount]>0)
		{
			m_pShadow		=	_New(BYTE,m_pShadowOffset[m_iCount]+8,mcu,"cSPRITE::m_pShadow");

			if (!m_pShadow)	return FALSE;

			file->Read(m_pShadow,m_pShadowOffset[m_iCount]);

			memset(m_pShadow+m_pShadowOffset[m_iCount],0,8);
		}
		else
		{
			if(m_pShadowOffset[m_iCount]<0)
			{
				g_eh.addStaticLog("cSPRITE::Load - m_pShadowOffset[m_iCount]<0");
			}
			m_pShadow		=	_New(BYTE,10,mcu,"cSPRITE::m_pShadow");
			memset(m_pShadow,0,10);
		}
	}

	//	외곽선 불러오기
	if (m_isLayer)
	{	m_pLayerOffset		=	_New(int,(m_iCount+1),mcu,"cSPRITE::m_pLayerOffset");
		if (!m_pLayerOffset) return FALSE;

		file->Read(m_pLayerOffset,m_iCount+1,4);

		m_pLayer	=	_New(BYTE,m_pLayerOffset[m_iCount],mcu,"cSPRITE::m_pLayer");
		if (!m_pLayer) return FALSE;

		file->Read(m_pLayer,m_pLayerOffset[m_iCount]);
	}
	else
	{	DWORD	size;

		file->Seek(m_iCount*4,SEEK_CUR);
		file->Read(&size,4);
		file->Seek(size,SEEK_CUR);
	}

	if (m_p16Sprite)
	{	WORD	*pdata	=	m_p16Sprite + m_pSpriteOffset[0];

		m_iWidth	=	pdata[0];
		m_iHeight	=	(pdata[1]-(short)pdata[3]);
	}
	if (m_p8Sprite)
	{	BYTE	*pdata	=	m_p8Sprite + m_pSpriteOffset[0];

		m_iWidth	=	*(WORD  *)(pdata  );
		m_iHeight	=	*(short*)(pdata+6);
	}

	return TRUE;
}

BOOL
cSPRITE::InstantLoad(cFILE *file,BOOL loadpalette,cMCU *mcu,cINSTANT_INFO *m_Info)
{
	close();

	if	(mcu)
		m_isControledByMcu	=	TRUE;
	else
		m_isControledByMcu	=	FALSE;

	m_isLoadedPlt	=	loadpalette;

	if	(m_iBpp==dSPRITE_BPP8)
	{	if	(loadpalette)
		{
			m_pPlt	=	_New(WORD,256,mcu,"cSPRITE::m_pPlt");

			if	(!m_pPlt)
				return FALSE;

			file->Read(m_pPlt,512);

			CONVERTPALETTE(m_pPlt);

			m_lpPlt	=	m_pPlt;
		}
		else
			file->Seek(512,SEEK_CUR);
	}
	//	스프라이트 데이터..
	m_pSpriteOffset	=	_New(int,m_iCount+1,mcu,"cSPRITE::m_pSpriteOffset");

	if	(!m_pSpriteOffset)
		return FALSE;

	file->Read(m_pSpriteOffset,(m_iCount+1),4);

	m_Info->m_iSpriteLocate	=	file->Locate();

	if (m_iBpp==dSPRITE_BPP16)
	{	
		m_p16Sprite	=	_New(WORD,m_pSpriteOffset[m_iCount],mcu,"cSPRITE::m_p16Sprite");

		if	(!m_p16Sprite)
			return FALSE;

		int	size	=	m_pSpriteOffset[1]-m_pSpriteOffset[0];

		file->Read(m_p16Sprite,size*2);
		file->Seek((m_pSpriteOffset[m_iCount]-size)*2,SEEK_CUR);
	}
	else
	{	m_p8Sprite	=	_New(BYTE,m_pSpriteOffset[m_iCount],mcu,"cSPRITE::m_p8Sprite");

		if	(!m_p8Sprite)
			return FALSE;

		int	size	=	m_pSpriteOffset[1]-m_pSpriteOffset[0];

		file->Read(m_p8Sprite,size);
		file->Seek(m_pSpriteOffset[m_iCount]-size,SEEK_CUR);
	}

	if	(g_iPixelFormat != dRGB555)
	{	
		if	(m_iBpp == dSPRITE_BPP16)
			for	(int i=0;i<m_iCount;i++)
				CONVERTSPRITE(Get16(i));
	}

	//	그림자 불러오기
	if (m_isShadow)
	{	
		m_pShadowOffset	=	_New(int,(m_iCount+1),mcu,"cSPRITE::m_pShadowOffset");

		if (!m_pShadowOffset)
			return FALSE;

		file->Read(m_pShadowOffset,m_iCount+1,4);

		if(m_pShadowOffset[m_iCount]>0)
		{	m_pShadow		=	_New(BYTE,m_pShadowOffset[m_iCount]+8,mcu,"cSPRITE::m_pShadow");

			memset(m_pShadow+m_pShadowOffset[m_iCount],0,8);

			if (!m_pShadow)	return FALSE;

			m_Info->m_iShadowLocate	=	file->Locate();

			file->Seek(m_pShadowOffset[m_iCount],SEEK_CUR);
		}
	}

	//	외곽선 불러오기
	if (m_isLayer)
	{
		m_pLayerOffset		=	_New(int,(m_iCount+1),mcu,"cSPRITE::m_pLayerOffset");

		if	(!m_pLayerOffset)
			return FALSE;

		file->Read(m_pLayerOffset,m_iCount+1,4);

		m_pLayer		=	_New(BYTE,m_pLayerOffset[m_iCount],mcu,"cSPRITE::m_pLayer");

		if	(!m_pLayer)
			return FALSE;

		m_Info->m_iLayerLocate	=	file->Locate();

		file->Seek(m_pLayerOffset[m_iCount],SEEK_CUR);
	}
	else
	{
		DWORD	size;

		file->Seek(m_iCount*4,SEEK_CUR);
		file->Read(&size,4);
		file->Seek(size,SEEK_CUR);
	}

	if (m_p16Sprite)
	{	WORD	*pdata	=	m_p16Sprite + m_pSpriteOffset[0];

		m_iWidth	=	pdata[0];
		m_iHeight	=	pdata[1];
	}

	if (m_p8Sprite)
	{	BYTE	*pdata	=	m_p8Sprite + m_pSpriteOffset[0];

		m_iWidth	=	*(WORD  *)(pdata  );
		m_iHeight	=	*(WORD  *)(pdata+2);
	}

	return TRUE;
}

BOOL
cSPRITE::LoadPlt(WORD *plt,char *fn,cNUX *nux)
{
	cFILE	*file;

	if	(nux)
		file	=	nux->Open(fn);
	else
	{	
		file=	new cFILE();

		if (!file->Open(fn,"rb"))
			KILL(file);
	}

	if	(!file)
	{
		ERRMSG("Error","Error in cSPRITE::Load - '%s' file not found",fn);
		return FALSE;
	}

	file->Seek(sizeof(SDHEADER),SEEK_CUR);

	LoadPlt(plt,file);

	KILL(file);

	return TRUE;
}

BOOL
cSPRITE::LoadPlt(WORD *plt,cFILE *file)
{
	file->Read(plt,512);
	CONVERTPALETTE(plt);

	return TRUE;
}

BOOL
cSPRITE::getRect(int index,cRECT *rect,int _iScale)
{
	if	(index	>=	m_iCount)
	{
		rect->set(-100000,-100000,-90000,-90000);
		return	FALSE;
	}

	if	(m_p16Sprite)
	{
		WORD	*pdata	=	m_p16Sprite + m_pSpriteOffset[index];

		rect->x1	=	-(short )(pdata[2]*_iScale/100);
		rect->y1	=	-(short )(pdata[3]*_iScale/100);
		rect->x2	=	rect->x1+(WORD )(pdata[0]*_iScale/100);
		rect->y2	=	rect->y1+(WORD )(pdata[1]*_iScale/100);

		return	TRUE;
	}
	if (m_p8Sprite)
	{
		BYTE	*pdata	=	m_p8Sprite + m_pSpriteOffset[index];

		rect->x1	=	-(*(short *)(pdata+4))*_iScale/100;
		rect->y1	=	-(*(short *)(pdata+6))*_iScale/100;
		rect->x2	=	rect->x1+(*(WORD  *)(pdata  ))*_iScale/100;
		rect->y2	=	rect->y1+(*(WORD  *)(pdata+2))*_iScale/100;

		return	TRUE;
	}

	return	FALSE;
}

void
cSPRITE::setPalette(WORD *plt)
{	
	if	(plt)
		m_lpPlt	=	plt;
}

void
cSPRITE::restorePalette()
{	
	m_lpPlt	=	m_pPlt;
}

void
cSPRITE::Put(int x,int y,int index,int xrate,int yrate,int effect,int alpha,int flip)
{	
	if (effect	<	dPUT_NORMAL)	alpha	=	effect;

	if (!m_p8Sprite && !m_p16Sprite)
		return;

	TEST(!m_p8Sprite && !m_p16Sprite,ERRMSG("Error in cSPRITE::Put - pointer pDATA are valid"));

	_PutSprite(x,y,m_iBpp,flip,m_p16Sprite+m_pSpriteOffset[index],m_p8Sprite+m_pSpriteOffset[index],m_lpPlt,xrate,yrate,effect,alpha,m_iMaxSpriteWidth,m_iMaxSpriteHeight);
}

//
//	레퍼런스 이미지를 이용해서 출력
void
cSPRITE::putReferenceImage(int _iX,int _iY,int _iIndex,WORD *_lpReferenceImage)
{
	cDRAW::PutReferenceSprite(_iX,_iY,m_p16Sprite+m_pSpriteOffset[_iIndex],_lpReferenceImage );
}

void
cSPRITE::putClipedImage(int x,int y,int index,int xs,int xrate,int yrate,int effect,int alpha,int flip)
{	
	if	(effect	<	dPUT_NORMAL)
		alpha	=	effect;

	if	(!m_p8Sprite	&&	!m_p16Sprite)
		return;

	int	iDX		=	0;

	if	(m_p16Sprite)
	{
		WORD	*pdata	=	m_p16Sprite + m_pSpriteOffset[index];

		iDX			=	-(short )(pdata[2]*xrate/100);
	}
	if (m_p8Sprite)
	{
		BYTE	*pdata	=	m_p8Sprite + m_pSpriteOffset[index];

		iDX			=	-(*(short *)(pdata+4))*xrate/100;
	}

	cDRAW::SetClippginArea(0,0,x+xs+iDX,cDRAW::HEIGHT);

	TEST(!m_p8Sprite	&&	!m_p16Sprite,ERRMSG("Error in cSPRITE::Put - pointer pDATA are valid"));

	_PutSprite(x,y,m_iBpp,flip,m_p16Sprite+m_pSpriteOffset[index],m_p8Sprite+m_pSpriteOffset[index],m_lpPlt,xrate,yrate,effect,alpha,m_iMaxSpriteWidth,m_iMaxSpriteHeight);

	cDRAW::FreeClippginArea();
}

void
cSPRITE::put3PieceBar(int _iX,int _iY,int _iFirstImage,int _iSize,int xrate,int yrate,int effect,int alpha)
{
	Put(_iX,_iY,_iFirstImage,xrate,yrate,effect,alpha);
	putClipedImage(_iX,_iY,_iFirstImage+1,_iSize,xrate,yrate,effect,alpha);
	Put(_iX+_iSize,_iY,_iFirstImage+2,xrate,yrate,effect,alpha);
}

//
//	직선으로 _iGap간격으로 _iRange까지 _iTimes만큼 출력
void
cSPRITE::linerPut(int _iX,int _iY,int _iIndex,int _iAngle,int _iRange,int _iGap,int _iTimes)
{
	int		iRange	=	0;
	CPos	pos;

	pos.x	=	_iX;
	pos.y	=	_iY;

	while(_iTimes)
	{
		Put(pos.x,pos.y,_iIndex);

		GetAnglePos(&pos,_iAngle,_iGap);
		iRange	+=	_iGap;

		if	(iRange	>	_iRange)
			break;

		_iTimes--;
	}
}

//
//
void
cSPRITE::clipPut(int x,int y,int index,int xs,int ys)
{
	if	(xs ==	0xffff)
		xs	=	cDRAW::WIDTH;
	else
		xs	=	x+xs;
	
	if	(ys	==	0xffff)
		ys	=	cDRAW::HEIGHT;
	else
		ys	=	y+ys;

	cDRAW::SetClippginArea(0,0,xs,ys);

	TEST(!m_p8Sprite && !m_p16Sprite,ERRMSG("Error in cSPRITE::Put - pointer pDATA are valid"));

	_PutSprite(x,y,m_iBpp,FALSE,m_p16Sprite+m_pSpriteOffset[index],m_p8Sprite+m_pSpriteOffset[index],m_lpPlt,100,100,dPUT_NORMAL,32,m_iMaxSpriteWidth,m_iMaxSpriteHeight);

	cDRAW::FreeClippginArea();
}

void
cSPRITE::PutLayer(int x,int y,int index,WORD color,int xrate,int yrate,int effect,int alpha,int flip)
{	
	if	(effect	<	dPUT_NORMAL)
		alpha	=	effect;

	TEST(!m_pLayer,ERRMSG("Error in cSPRITE::PutLayer - pointer m_pLayer are valid"));

	_PutLayer(x,y,color,flip,m_pLayer+m_pLayerOffset[index],xrate,yrate,effect,alpha);
}

void
cSPRITE::PutShadow(int x,int y,int index,int xrate,int yrate,int _iAlpha)
{	
	TEST(!m_pShadow,ERRMSG("Error in cSPRITE::PutShadow - pointer m_pShadow are valid"));

	if	(m_pShadowOffset[index] >= m_pShadowOffset[m_iCount])
	{
//		cMSG::Put("뷁!!","뷁!!");
	}

	cSHADOW::PutShadow(x,y,m_pShadow+m_pShadowOffset[index],xrate,yrate,_iAlpha,m_iMaxShadowWidth,m_iMaxShadowHeight);
}

// void
// cSPRITE::ShadowMask(int x,int y,int index)
// {
// 	TEST(!m_pShadow,ERRMSG("Error in cSPRITE::PutShadow - pointer m_pShadow are valid"));
// 
// 	cSHADOW::ShadowMask(x,y,m_pShadow+m_pShadowOffset[index]);
// }


/********************************************************************
	퀵 로드 스프라이트(리얼타임으로 불러서 찍는다.)
********************************************************************/

cSPRITEQ::cSPRITEQ()
		:cSPRITE()
{	
	m_lpFILE	=	NULL;
	m_pFile		=	NULL;
}

cSPRITEQ::~cSPRITEQ()
{	
	KILL(m_pFile);
	close();
}

BOOL
cSPRITEQ::Load(char *fn,BOOL loadpalette,cNUX *nux,cMCU *mcu)
{
	cFILE *file;

	if (nux)
		file=	nux->Open(fn);
	else
	{	
		m_pFile	=	new cFILE();
		file	=	m_pFile;

		if	(!file->Open(fn,"rb"))
		{
			KILL(m_pFile);
			file	=	NULL;
		}
	}

	if (!file)
		return ERRMSG("Error in cSPRITEQ::Load - '%s' file not found",fn);

	file->Read(&SDHEADER,sizeof(SDHEADER));

	if	(strcmp(SDHEADER.Reg,REGSDHEADER)	!=0	&&	strcmp(SDHEADER.Reg,REGSDHEADER2)	!=0	)
		return ERRMSG("Error in cSPRITEQ::Load - '%s' file header not match",m_strFName);

	m_iBpp		=	SDHEADER.BPP;
	m_isShadow	=	SDHEADER.bShadow;
	m_isLayer	=	SDHEADER.bOutline;
	m_iCount	=	SDHEADER.ImageCount;

	if (m_iBpp	==	16)
		m_iBpp	=	dSPRITE_BPP16;
	else
		m_iBpp	=	dSPRITE_BPP8;

	BOOL result =	Load(file,loadpalette,mcu);

/*	if (nux)	m_lpFILE	=	file;
	else
	{	KILL(file);
		strcpy(m_strFName,fn);
	}
*/
	m_lpFILE	=	file;

	cRECT	rect;

	getRect(0,&rect);

	m_iHeight	=	rect.getHeight();

	if	(strcmp(SDHEADER.Reg,REGSDHEADER2)	==	0)
	{
		m_iMaxSpriteWidth	=	SDHEADER.m_wMaxSpriteWidth;
		m_iMaxSpriteHeight	=	SDHEADER.m_wMaxSpriteHeight;
		m_iMaxShadowWidth	=	SDHEADER.m_wMaxShadowWidth;
		m_iMaxShadowHeight	=	SDHEADER.m_wMaxShadowHeight;
	}
	else
	if	(strcmp(SDHEADER.Reg,REGSDHEADER)	==	0)
	{
		m_iMaxSpriteWidth	=	0;
		m_iMaxSpriteHeight	=	0;
		m_iMaxShadowWidth	=	0;
		m_iMaxShadowHeight	=	0;
	}

	return result;
}

BOOL
cSPRITEQ::Load(cFILE *file,BOOL loadpalette,cMCU *mcu)
{	
	close();

	if	(mcu)
		m_isControledByMcu	=	TRUE;
	else
		m_isControledByMcu	=	FALSE;

	m_isLoadedPlt	=	loadpalette;

	if	(m_iBpp	==	dSPRITE_BPP8)
	{	
		if	(loadpalette)
		{	
			m_pPlt	=	_New(WORD,256,mcu,"cSPRITEQ::m_pPlt");

			if	(!m_pPlt)
				return FALSE;

			file->Read(m_pPlt,512);

			CONVERTPALETTE(m_pPlt);

			m_lpPlt	=	m_pPlt;
		}
		else
			file->Seek(512,SEEK_CUR);
	}

	//	여기부터 스프라이트 데이터..
	m_pSpriteOffset	=	_New(int,(m_iCount+1),mcu,"cSPRITEQ::m_pSpriteOffset");

	if	(!m_pSpriteOffset)
		return FALSE;

	file->Read(m_pSpriteOffset,sizeof(int)*(m_iCount+1));

	m_iSpriteLocate	=	file->Locate();

	if	(m_iBpp	==	dSPRITE_BPP8)
		file->Seek(m_pSpriteOffset[m_iCount]  ,SEEK_CUR);
	else
		file->Seek(m_pSpriteOffset[m_iCount]*2,SEEK_CUR);

	//	그림자 불러오기
	if	(m_isShadow)
	{
		m_pShadowOffset		=	_New(int,(m_iCount+1),mcu,"cSPRITEQ::m_pShadowOffset");

		if	(!m_pShadowOffset)
			return FALSE;

		file->Read(m_pShadowOffset,sizeof(int),m_iCount+1);

		m_iShadowLocate	=	file->Locate();

		file->Seek(m_pShadowOffset[m_iCount],SEEK_CUR);
	}

	//	외곽선 불러오기
	DWORD	size;

	file->Seek(m_iCount*4,SEEK_CUR);
	file->Read(&size,4);
	file->Seek(size,SEEK_CUR);

	return TRUE;
}

BOOL
cSPRITEQ::Put(int x,int y,int index,int xrate,int yrate,int effect,int alpha,int flip)
{
	if	(effect	<	dPUT_NORMAL)	
		alpha	=	effect;

	if	(m_pSpriteOffset == NULL)
	{
		return ERRMSG("Error in cSPRITEQ::Put - data's are not initialized");
	}

	cFILE	*file;

/*	if (m_lpFILE)	file	=	m_lpFILE;
	else
	{	file	=	new cFILE();
		file->Open(m_strFName,"rb");
	}*/
	file	=	m_lpFILE;

	TEST(!file,return ERRMSG("Error in cSPRITEQ::Put - '%s' file not found",m_strFName));

	DWORD size	=	m_pSpriteOffset[index+1]-m_pSpriteOffset[index];

	if (m_iBpp	==	dSPRITE_BPP8)
	{
		AllocTemplateBuffer(size);
		file->Seek(m_iSpriteLocate+m_pSpriteOffset[index],SEEK_SET);
		file->Read(g_pTemplateBuffer,size);
		_PutSprite(x,y,m_iBpp,flip,NULL,g_pTemplateBuffer,m_lpPlt,xrate,yrate,effect,alpha,m_iMaxSpriteWidth,m_iMaxSpriteHeight);
	}
	else
	{
		AllocTemplateBuffer(size*2);
		file->Seek(m_iSpriteLocate+m_pSpriteOffset[index]*2,SEEK_SET);
		
		file->Read(g_pTemplateBuffer,size*2);

		CONVERTSPRITE((WORD *)g_pTemplateBuffer);

		_PutSprite(x,y,m_iBpp,flip,(WORD *)g_pTemplateBuffer,NULL,NULL,xrate,yrate,effect,alpha,m_iMaxSpriteWidth,m_iMaxSpriteHeight);
	}

//	if (!m_lpFILE) KILL(file);

	return TRUE;
}

BOOL
cSPRITEQ::PutLayer(int x,int y,int index,WORD color,int xrate,int yrate,int effect,int alpha,int flip)
{
	if	(effect	<	dPUT_NORMAL)	
		alpha	=	effect;

	TEST(m_pLayerOffset == NULL,return ERRMSG("Error in cSPRITEQ::Put - data's are not initialized"));

	cFILE	*file;

	file	=	m_lpFILE;
//	if (m_lpFILE)	file	=	m_lpFILE;
//	else
//	{	file	=	new cFILE();
//		file->Open(m_strFName,"rb");
//	}

	TEST(!file,return ERRMSG("Error in cSPRITEQ::PutLayer - '%s' file not found",m_strFName));

	DWORD size	=	m_pLayerOffset[index+1]-m_pLayerOffset[index];

	file->Seek(m_iLayerLocate+m_pLayerOffset[index],SEEK_SET);

	AllocTemplateBuffer(size);
	file->Read(g_pTemplateBuffer,size);

	_PutLayer(x,y,color,flip,g_pTemplateBuffer,xrate,yrate,effect,alpha);

	if (!m_lpFILE) KILL(file);

	return TRUE;
}

WORD*
cSPRITEQ::Get16(int index)
{
	if (index>=m_iCount)
		return NULL;

	if (m_iBpp	!=	dSPRITE_BPP16)
		return	NULL;

	DWORD size	=	m_pSpriteOffset[index+1]-m_pSpriteOffset[index];

	AllocTemplateBuffer(size*2);
	m_lpFILE->Seek(m_iSpriteLocate+m_pSpriteOffset[index]*2,SEEK_SET);
	m_lpFILE->Read(g_pTemplateBuffer,size*2);

	CONVERTSPRITE((WORD *)g_pTemplateBuffer);

	return (WORD *)g_pTemplateBuffer;
}

BYTE*
cSPRITEQ::Get8(int index)
{
	if	(index>=m_iCount)
		return NULL;

	if	(m_iBpp	!=	dSPRITE_BPP8)
		return	NULL;

	DWORD size	=	m_pSpriteOffset[index+1]-m_pSpriteOffset[index];

	AllocTemplateBuffer(size);
	m_lpFILE->Seek(m_iSpriteLocate+m_pSpriteOffset[index],SEEK_SET);
	m_lpFILE->Read(g_pTemplateBuffer,size);

	return g_pTemplateBuffer;
}

int
cSPRITEQ::getSpriteWidth(int index)
{
	WORD	*lpwData	=	Get16(index);

	if	(lpwData)
		return	lpwData[0];

	BYTE	*lpbData	=	Get8(index);

	if	(lpbData)
		return	*(WORD *)lpbData;

	return 0;
}

int
cSPRITEQ::getSpriteHeight(int index)
{
	WORD	*lpwData	=	Get16(index);

	if	(lpwData)
		return	lpwData[1];

	BYTE	*lpbData	=	Get8(index);

	if	(lpbData)
		return	*(WORD *)(lpbData+2);

	return 0;
}

BOOL
cSPRITEQ::PutShadow(int x,int y,int index,int xrate,int yrate,int _iAlpha)
{
	if	(m_pShadowOffset == NULL)
		return FALSE;

	cFILE	*file;

	if (m_lpFILE)
		file	=	m_lpFILE;
	else
	{
		file	=	new cFILE();
		file->Open(m_strFName,"rb");
	}

	TEST(!file,return ERRMSG("Error in cSPRITEQ::PutShadow - '%s' file not found",m_strFName));

	DWORD size	=	m_pShadowOffset[index+1]-m_pShadowOffset[index];

	if	(size	==	0)
		return	FALSE;

	file->Seek(m_iShadowLocate+m_pShadowOffset[index],SEEK_SET);

	AllocTemplateBuffer(size);

	file->Read(g_pTemplateBuffer,size);

	cSHADOW::PutShadow(x,y,g_pTemplateBuffer,xrate,yrate,_iAlpha,m_iMaxShadowWidth,m_iMaxShadowHeight);

	if	(!m_lpFILE)
		KILL(file);

	return TRUE;
}

BOOL
cSPRITEQ::getRect(int index,cRECT *rect,int _iScale)
{
	if	(m_iBpp	==	dSPRITE_BPP16)
	{
		WORD	*pdata	=	Get16(index);

		if	(!pdata)
			return	FALSE;

		rect->x1	=	-(short )(pdata[2]*_iScale/100);
		rect->y1	=	-(short )(pdata[3]*_iScale/100);
		rect->x2	=	rect->x1+(WORD )(pdata[0]*_iScale/100);
		rect->y2	=	rect->y1+(WORD )(pdata[1]*_iScale/100);
	}
	else
	{
		BYTE	*pdata	=	Get8(index);

		if	(!pdata)
			return	FALSE;

		rect->x1	=	-(*(short *)(pdata+4))*_iScale/100;
		rect->y1	=	-(*(short *)(pdata+6))*_iScale/100;
		rect->x2	=	rect->x1+(*(WORD  *)(pdata  ))*_iScale/100;
		rect->y2	=	rect->y1+(*(WORD  *)(pdata+2))*_iScale/100;
	}

	return	TRUE;
}


/********************************************************************
	에니메이션 스프라이트 데이터
********************************************************************/
cANMDATA::cANMDATA()
{	
	m_pReleasePos	=	NULL;
	m_pPos			=	NULL;
	m_pSprite		=	NULL;
	m_pIsEvent		=	NULL;
	m_pBackFrame	=	NULL;

	m_iFrameCount	=	0;
	m_iDirectCount	=	0;
	m_isControledByMcu			=	FALSE;

	m_iLoopBegin	=	0xffff;
	m_iLoopEnd		=	0xffff;
}

cANMDATA::~cANMDATA()
{	Reset();
}

void
cANMDATA::Reset()
{	
	if (!m_isControledByMcu)
	{	_Free(m_pPos);
		_Free(m_pSprite);
		_Free(m_pBackFrame);
		_Free(m_pIsEvent);
		_Free(m_pReleasePos);
	}

	m_pPos			=	NULL;
	m_pSprite		=	NULL;
	m_pBackFrame	=	NULL;
	m_pIsEvent		=	NULL;
	m_pReleasePos	=	NULL;
	m_iLinkAnm		=	0xffff;
}

BOOL
cANMDATA::Init(cFILE *file,cMCU *mcu)
{	
	Reset();

	if	(mcu)
		m_isControledByMcu	=	TRUE;
	else
		m_isControledByMcu	=	FALSE;

	file->Read(&m_iFrameCount	,4);

	if	(m_iFrameCount > 0)
	{
		file->Read(this	,sizeof(cANM_INFO));

		m_pIsEvent		=	_New(BYTE,m_iFrameCount,mcu,"cANMDATA::m_pIsEvent");
		m_pSprite		=	_New(WORD,m_iFrameCount*m_iDirectCount,mcu,"cANMDATA::m_pSprite");
		m_pBackFrame	=	_New(BYTE,m_iFrameCount*m_iDirectCount,mcu,"cANMDATA::m_pBackFrame");

		file->Read(m_pIsEvent	,m_iFrameCount);						//	이벤트 정보
		file->Read(m_pSprite	,m_iFrameCount*m_iDirectCount,2);		//	스프라이트 정보
		file->Read(m_pBackFrame	,m_iFrameCount*m_iDirectCount,1);		//	백 프레임

		if (m_isRELEASE)	//	릴리즈 포인트
		{	m_pReleasePos	=	_New(POINTS,m_iDirectCount,mcu,"cANMDATA::m_pReleasePos");
			if (!m_pReleasePos)	return FALSE;

			file->Read(m_pReleasePos	,m_iDirectCount,4);
		}

		if (m_isREFITFRAME)//	프레임 보정 위치
		{	m_pPos	=	_New(POINTS,m_iFrameCount*m_iDirectCount,mcu,"cANMDATA::m_pPos");
			if (!m_pPos)	return FALSE;

			file->Read(m_pPos	,m_iFrameCount*m_iDirectCount,4);
		}
	}

	for (int i=0;i<m_iFrameCount;i++)
	{
		if (m_pIsEvent[i]	&	dEVENT_LOOP_START	)	m_iLoopBegin	=	i;
		if (m_pIsEvent[i]	&	dEVENT_LOOP_END		)	m_iLoopEnd		=	i;
	}

/*	if (m_iLoopBegin	!=	0xffff	&&	m_iLoopEnd	==	0xffff)
		ERRMSG("Fatal DATA Error!!","Can not find End Loop Frame in \"%s\"",l_lpLoadFileName);

	if (m_iLoopBegin	==	0xffff	&&	m_iLoopEnd	!=	0xffff)
		ERRMSG("Fatal DATA Error!!","Can not find Begin Loop Frame in \"%s\"",l_lpLoadFileName);
*/

	return TRUE;
}

/********************************************************************
	에니메이션 스프라이트 데이터
********************************************************************/

cANM::cANM()
	 :cANM_BASE()
{
}

cANM::~cANM()
{
	Destroy();
}

void
cANM::Destroy()
{	
	Reset();
}


void
cANM::Reset()
{	
	m_sprite.close();

	pKILL(m_pAnmData);
}

BOOL
cANM::convert051115(char *fn)
{
	cFILE	file,file2;

	if	(!Load(fn,TRUE,NULL,NULL))
	{
		Reset();
		return	FALSE;
	}

	if	(strcmp(SADHEADER.Reg,REGSADHEADER2)	==	0)
	{
		Reset();
		return TRUE;
	}

	m_sprite.getMaxSize();

	SADHEADER.m_wMaxSpriteWidth		=	m_sprite.m_iMaxSpriteWidth;
	SADHEADER.m_wMaxSpriteHeight	=	m_sprite.m_iMaxSpriteHeight;
	SADHEADER.m_wMaxShadowWidth		=	m_sprite.m_iMaxShadowWidth;
	SADHEADER.m_wMaxShadowHeight	=	m_sprite.m_iMaxShadowWidth;

	strcpy(SADHEADER.Reg,REGSADHEADER2);

	SetFileAttributes(fn,FILE_ATTRIBUTE_ARCHIVE);

	if	(!file.Open(fn,"rb+",TRUE))
		return	FALSE;

	file.Write(&SADHEADER,sizeof(SADHEADER));
	file.Close();

	Reset();

	return	TRUE;
}

BOOL
cANM::Load(char *fn,BOOL loadpalette,cNUX *nux,cMCU *mcu)
{
	cFILE	*file;

	l_lpLoadFileName	=	fn;

	if	(nux)
		file	=	nux->Open(fn);
	else
	{	
		file	=	new cFILE();
		
		if	(!file->Open(fn,"rb"))
			KILL(file);
	}

	if	(!file)
		return g_eh.addStaticLog("Error in cANM::Load - '%s' file not found",fn);

	file->Read(&SADHEADER,sizeof(SADHEADER));

	if	(strcmp(SADHEADER.Reg,REGSADHEADER)	!=	0	&&	strcmp(SADHEADER.Reg,REGSADHEADER2)	!=	0)
		return ERRMSG("Error in cANM::Load - header not match!!");

	m_iAnmCount			=	SADHEADER.AnmCount;
	m_isFlip			=	SADHEADER.bHalf;
	m_sprite.m_iBpp		=	SADHEADER.BPP;
	m_sprite.m_isShadow	=	SADHEADER.bShadow;
	m_sprite.m_isLayer	=	SADHEADER.bOutline;
	m_sprite.m_iCount	=	SADHEADER.ImageCount;

	if	(m_sprite.m_iBpp==	16)
		m_sprite.m_iBpp	=	dSPRITE_BPP16;
	else
		m_sprite.m_iBpp	=	dSPRITE_BPP8;

	BOOL	result		=	Load(file,loadpalette,mcu);

	KILL(file);

	if	(strcmp(SADHEADER.Reg,REGSADHEADER2)	==	0)
	{
		m_sprite.m_iMaxSpriteWidth	=	SADHEADER.m_wMaxSpriteWidth;
		m_sprite.m_iMaxSpriteHeight	=	SADHEADER.m_wMaxSpriteHeight;
		m_sprite.m_iMaxShadowWidth	=	SADHEADER.m_wMaxShadowWidth;
		m_sprite.m_iMaxShadowHeight	=	SADHEADER.m_wMaxShadowHeight;
	}
	else
	{
		m_sprite.getMaxSize();

		cFILE	convertor;

		if	(l_bIsWantAutoConvert	&&	convertor.Open(fn,"rb+",TRUE))
		{
			strcpy(SADHEADER.Reg,REGSADHEADER2);

			SADHEADER.m_wMaxSpriteWidth		=	m_sprite.m_iMaxSpriteWidth	;
			SADHEADER.m_wMaxSpriteHeight	=	m_sprite.m_iMaxSpriteHeight	;
			SADHEADER.m_wMaxShadowWidth		=	m_sprite.m_iMaxShadowWidth	;
			SADHEADER.m_wMaxShadowHeight	=	m_sprite.m_iMaxShadowHeight	;

			convertor.Write(&SADHEADER,sizeof(SADHEADER));
			convertor.Close();
		}
	}

	return result;
}

BOOL
cANM::Load(cFILE *file,BOOL loadpalette,cMCU *mcu)
{
	int i;

	Reset();	//	몽땅 초기화

	if (mcu)
		m_sprite.m_isControledByMcu	=	TRUE;
	else
		m_sprite.m_isControledByMcu	=	FALSE;

	//	스프라이트 데이터 로딩

	m_sprite.Load(file,loadpalette,mcu);

	//	에니메이션 데이터 로딩
	m_pAnmData	=	new cANMDATA [m_iAnmCount];

	file->Read(&m_iMoveOval,4);
	file->Read(&m_rectCrash,16);
	file->Read(&m_rectSelect,16);
	file->Read(&m_iKind,4);
	file->Read(&m_posRefit,sizeof(CPos));
	file->Read(&m_iCrashSize,4);
	file->Read(&m_iDefaultAttack,4);

	WORD	wDefaultMagic;
	file->Read(&wDefaultMagic,2);
	m_iDefaultMagic	=	wDefaultMagic;
	file->Read(&m_wIsOccasionallyRestAction,2);

	for (i=0;i<m_iAnmCount;i++)
		m_pAnmData[i].Init(file,mcu);

	if (m_iAnmCount	&&	m_pAnmData[0].m_iLinkAnm	>=	0	&& m_pAnmData[0].m_iLinkAnm	<	m_iAnmCount)
	{
		cRECT	rect;

		getRect(m_pAnmData[0].m_iLinkAnm,0,0,&rect);

		m_sprite.m_iHeight	=	max(m_sprite.m_iHeight,(int)rect.getHeight());
	}

	m_sprite.m_iHeight	+=	m_posRefit.y;

	return TRUE;
}

BOOL
cANM::LoadPlt(WORD *plt,char *fn,cNUX *nux)
{
	cFILE	*file;

	if (nux)	file	=	nux->Open(fn);
	else
	{	file	=	new cFILE();
		if (!file->Open(fn,"rb"))	KILL(file);
	}

	if (!file)
	{	ERRMSG("Error in cANM::LoadPlt(char *fn,cNUX *nux) - '%s' file not found",fn);
		return	FALSE;
	}

	file->Seek(sizeof(SADHEADER),SEEK_CUR);

	cSPRITE::LoadPlt(plt,file);

	KILL(file);

	return TRUE;
}

void
cANM::Put(int x,int y,int anm,int direct,int frame,int xrate,int yrate,int effect,int alpha)
{
	if	(anm	>= m_iAnmCount						)	return;
	if	(direct >= m_pAnmData[anm].m_iDirectCount	)	return;
	if	(frame	>= m_pAnmData[anm].m_iFrameCount	)	return;

	int		fliped	=	c_aIsFlip[m_isFlip][m_pAnmData[anm].m_iAnmType][direct];
	int		index	=	m_pAnmData[anm].GetSprite(direct,frame,m_isFlip);

// 	y	-=	(m_posRefit.y*yrate/100);
// 	x	-=	(m_posRefit.x*xrate/100);
	y	-=	m_posRefit.y;
	x	-=	m_posRefit.x;

	if (m_pAnmData[anm].m_isREFITFRAME)
	{
		x	-=	(m_pAnmData[anm].m_pPos[direct*m_pAnmData[anm].m_iFrameCount + frame].x*xrate)/100;
		y	-=	(m_pAnmData[anm].m_pPos[direct*m_pAnmData[anm].m_iFrameCount + frame].y*yrate)/100;
	}

	_PutSprite(x,y,m_sprite.m_iBpp,fliped,
				m_sprite.m_p16Sprite+m_sprite.m_pSpriteOffset[index],
				m_sprite.m_p8Sprite+m_sprite.m_pSpriteOffset[index],
				m_sprite.m_lpPlt,xrate,yrate,effect,alpha,m_sprite.m_iMaxSpriteWidth,m_sprite.m_iMaxSpriteHeight);
}

void
cANM::PutSprite(int x,int y,int index,int xrate,int yrate,int effect,int alpha)
{
	_PutSprite(x,y,m_sprite.m_iBpp,FALSE,
				m_sprite.m_p16Sprite+m_sprite.m_pSpriteOffset[index],
				m_sprite.m_p8Sprite+m_sprite.m_pSpriteOffset[index],
				m_sprite.m_lpPlt,xrate,yrate,effect,alpha,m_sprite.m_iMaxSpriteWidth,m_sprite.m_iMaxSpriteHeight);
}

void
cANM::PutAlpha(int x,int y,int anm,int direct,int frame,int xrate,int yrate,int alpha)
{
	if	(anm	>= m_iAnmCount						)	return;
	if	(direct >= m_pAnmData[anm].m_iDirectCount	)	return;
	if	(frame	>= m_pAnmData[anm].m_iFrameCount	)	return;

	int		fliped	=	c_aIsFlip[m_isFlip][m_pAnmData[anm].m_iAnmType][direct];
	int		index	=	m_pAnmData[anm].GetSprite(direct,frame,m_isFlip);

// 	y	-=	(m_posRefit.y*yrate/100);
// 	x	-=	(m_posRefit.x*xrate/100);
	y	-=	m_posRefit.y;
	x	-=	m_posRefit.x;

	if (m_pAnmData[anm].m_isREFITFRAME)
	{
		x	-=	(m_pAnmData[anm].m_pPos[direct*m_pAnmData[anm].m_iFrameCount + frame].x*xrate)/100;
		y	-=	(m_pAnmData[anm].m_pPos[direct*m_pAnmData[anm].m_iFrameCount + frame].y*yrate)/100;
	}

	_PutSprite(x,y,m_sprite.m_iBpp,fliped,
				m_sprite.m_p16Sprite+m_sprite.m_pSpriteOffset[index],
				m_sprite.m_p8Sprite+m_sprite.m_pSpriteOffset[index],
				m_sprite.m_lpPlt,xrate,yrate,dPUT_ALPHA_BLENDING,alpha,m_sprite.m_iMaxSpriteWidth,m_sprite.m_iMaxSpriteHeight);
}

void
cANM::PutShadow(int x,int y,int anm,int direct,int frame,int xrate,int yrate,int _iAlpha)
{
	if	(!m_pAnmData	||	anm	>= m_iAnmCount	||	anm	<	0	)
		return;
	if	(direct >= m_pAnmData[anm].m_iDirectCount	||	direct	<	0)
		return;
	if	(frame	>= m_pAnmData[anm].m_iFrameCount	||	frame	<	0)
		return;

	if	(m_sprite.m_pShadow	==	NULL	|| m_sprite.m_pShadowOffset == NULL)
		return;

	int		index	=	m_pAnmData[anm].GetSprite(direct,frame,FALSE);

// 	y	-=	(m_posRefit.y*yrate/100);
// 	x	-=	(m_posRefit.x*xrate/100);
	y	-=	m_posRefit.y;
	x	-=	m_posRefit.x;

	if (m_pAnmData[anm].m_isREFITFRAME)
	{
		x	-=	(m_pAnmData[anm].m_pPos[direct*m_pAnmData[anm].m_iFrameCount + frame].x*xrate)/100;
		y	-=	(m_pAnmData[anm].m_pPos[direct*m_pAnmData[anm].m_iFrameCount + frame].y*yrate)/100;
	}

	cSHADOW::PutShadow(x,y,m_sprite.m_pShadow+m_sprite.m_pShadowOffset[index],xrate,yrate,_iAlpha,m_sprite.m_iMaxShadowWidth,m_sprite.m_iMaxShadowHeight);

	return;

	anm		=	m_pAnmData[anm].m_iLinkAnm;

	if (anm	==	0xffff)
		return;
	
	index	=	m_pAnmData[anm].GetSprite(direct,frame,m_isFlip);

	cSHADOW::PutShadow(x,y,m_sprite.m_pShadow+m_sprite.m_pShadowOffset[index]);
}

// void
// cANM::ShadowMask(int x,int y,int anm,int direct,int frame)
// {
// 	if	(anm	>= m_iAnmCount						)	return;
// 	if	(direct >= m_pAnmData[anm].m_iDirectCount	)	return;
// 	if	(frame	>= m_pAnmData[anm].m_iFrameCount	)	return;
// 
// 	TEST(!m_sprite.m_pShadow,ERRMSG("Error in cANM::ShadowMask - pointer m_pShadow are valid"));
// 
// 	int		index	=	m_pAnmData[anm].GetSprite(direct,frame,FALSE);
// 
// 	y	-=	m_posRefit.y;
// 	x	-=	m_posRefit.x;
// 
// 	cSHADOW::ShadowMask(x,y,m_sprite.m_pShadow+m_sprite.m_pShadowOffset[index]);
// }

void
cANM::PutLayer(int x,int y,int anm,int direct,int frame,WORD color,int xrate,int yrate,int effect,int alpha)
{
	if	(anm	>= m_iAnmCount						)	return;
	if	(direct >= m_pAnmData[anm].m_iDirectCount	)	return;
	if	(frame	>= m_pAnmData[anm].m_iFrameCount	)	return;

	TEST(!m_sprite.m_pLayer,ERRMSG("Error in cANM::PutLayer - pointer m_pLayer are valid"));

	int		fliped	=	c_aIsFlip[m_isFlip][m_pAnmData[anm].m_iAnmType][direct];
	int		index	=	m_pAnmData[anm].GetSprite(direct,frame,m_isFlip);

	y	-=	m_posRefit.y;
	x	-=	m_posRefit.x;

	if (m_pAnmData[anm].m_isREFITFRAME)
	{
		x	-=	(m_pAnmData[anm].m_pPos[direct*m_pAnmData[anm].m_iFrameCount + frame].x*xrate)/100;
		y	-=	(m_pAnmData[anm].m_pPos[direct*m_pAnmData[anm].m_iFrameCount + frame].y*yrate)/100;
	}


	if (effect	<	dPUT_NORMAL)	alpha	=	effect;

	_PutLayer(x,y,color,fliped,m_sprite.m_pLayer+m_sprite.m_pLayerOffset[index],xrate,yrate,effect,alpha);
}

void
cANM::PutReg(int x,int y,int anm,int direct,int frame,int xrate,int yrate)
{
	if	(anm	>= m_iAnmCount	||	anm	<	0		)
		return;
	if	(direct >= m_pAnmData[anm].m_iDirectCount	)
		return;
	if	(frame	>= m_pAnmData[anm].m_iFrameCount	)
		return;

	TEST(!m_sprite.m_p8Sprite && !m_sprite.m_p16Sprite,ERRMSG("Error in cANM::PutReg - pointer m_p16Sprite are valid"));
	TEST(anm	>= m_iAnmCount,ERRMSG("Error in cANM::PutReg","valid anm %d",anm));
	TEST(direct	>= m_pAnmData[anm].m_iDirectCount,ERRMSG("Error in cANM::PutReg" , "valid direct %d",direct));
	TEST(frame  >= m_pAnmData[anm].m_iFrameCount ,ERRMSG("Error in cANM::PutReg" , "valid frame %d",frame));

	int		fliped	=	c_aIsFlip[m_isFlip][m_pAnmData[anm].m_iAnmType][direct];
	int		index	=	m_pAnmData[anm].GetSprite(direct,frame,m_isFlip);
	int		alpha	=	m_pAnmData[anm].m_iALPHA;

	y	-=	m_posRefit.y;
	x	-=	m_posRefit.x;
// 	y	-=	(m_posRefit.y*yrate/100);
//	x	-=	(m_posRefit.x*xrate/100);

	if	(m_pAnmData[anm].m_isREFITFRAME)
	{
		x	-=	m_pAnmData[anm].m_pPos[direct*m_pAnmData[anm].m_iFrameCount + frame].x;
		y	-=	m_pAnmData[anm].m_pPos[direct*m_pAnmData[anm].m_iFrameCount + frame].y;
	}

	
	_PutSprite	(x,y,m_sprite.m_iBpp,fliped,
				m_sprite.m_p16Sprite+m_sprite.m_pSpriteOffset[index],
				m_sprite.m_p8Sprite+m_sprite.m_pSpriteOffset[index],
				m_sprite.m_lpPlt,xrate,yrate,alpha,alpha,m_sprite.m_iMaxSpriteWidth,m_sprite.m_iMaxSpriteHeight);

	anm		=	m_pAnmData[anm].m_iLinkAnm;

	if	(anm	==	0xffff	||	anm	>=	m_iAnmCount	||	m_pAnmData[anm].m_iFrameCount	==	0)
		return;

	frame	=	frame%m_pAnmData[anm].m_iFrameCount;
	fliped	=	c_aIsFlip[m_isFlip][m_pAnmData[anm].m_iAnmType][direct];
	index	=	m_pAnmData[anm].GetSprite(direct,frame,m_isFlip);
	alpha	=	m_pAnmData[anm].m_iALPHA;

	_PutSprite	(x,y,m_sprite.m_iBpp,fliped,
				m_sprite.m_p16Sprite+m_sprite.m_pSpriteOffset[index],
				m_sprite.m_p8Sprite+m_sprite.m_pSpriteOffset[index],
				m_sprite.m_lpPlt,xrate,yrate,alpha,alpha,m_sprite.m_iMaxSpriteWidth,m_sprite.m_iMaxSpriteHeight);

}

int
cANM::getSpriteHeight(int anm,int direct,int frame)
{
	int		iIndex	=	GetSpriteIndex(anm,direct,frame);

	WORD	*lpwData	=	m_sprite.Get16(iIndex);

	if	(lpwData)
		return	lpwData[1];

	BYTE	*lpbData	=	m_sprite.Get8(iIndex);

	if	(lpbData)
		return	*(WORD *)(lpbData+2);

	return 0;
}

int
cANM::getSpriteWidth(int anm,int direct,int frame)
{
	int		iIndex	=	GetSpriteIndex(anm,direct,frame);

	WORD	*lpwData	=	m_sprite.Get16(iIndex);

	if	(lpwData)
		return	lpwData[0];

	BYTE	*lpbData	=	m_sprite.Get8(iIndex);

	if	(lpbData)
		return	*(WORD *)lpbData;

	return 0;
}

int
cANM::getSpriteDX(int anm,int direct,int frame)
{
	int		iIndex	=	GetSpriteIndex(anm,direct,frame);

	WORD	*lpwData	=	m_sprite.Get16(iIndex);

	if	(lpwData)
		return	(short)lpwData[2];

	BYTE	*lpbData	=	m_sprite.Get8(iIndex);

	if	(lpbData)
		return	*(short *)(lpbData+4);

	return 0;
}

int
cANM::getSpriteDY(int anm,int direct,int frame)
{
	int		iIndex	=	GetSpriteIndex(anm,direct,frame);

	WORD	*lpwData	=	m_sprite.Get16(iIndex);

	if	(lpwData)
		return	(short)lpwData[3];

	BYTE	*lpbData	=	m_sprite.Get8(iIndex);

	if	(lpbData)
		return	*(short *)(lpbData+6);

	return 0;
}

//
//	직선 찍기
void
cANM::linerPut(int _iX,int _iY,int _iAnm,int _iDirect,int _iFrame,int _iAngle,int _iRange,int _iGap,int _iTimes,
			   int _iSecondImageAlphaDepth,int _iDecreaseAlphaDepth,int _iScale,BOOL _bIsIncreaseFrame)
{
	int		iRange	=	0;
	CPos	pos;

	pos.x	=	_iX;
	pos.y	=	_iY;

	while(_iTimes)
	{
		if (_iSecondImageAlphaDepth	>	0)
		{
			if (_iSecondImageAlphaDepth	!=	32)
				Put(pos.x,pos.y,_iAnm,_iDirect,_iFrame,_iScale,_iScale,dPUT_ALPHA_BLENDING,_iSecondImageAlphaDepth);
			else
				PutReg(pos.x,pos.y,_iAnm,_iDirect,_iFrame,_iScale,_iScale);
		}

		_iSecondImageAlphaDepth	-=	_iDecreaseAlphaDepth;

		GetAnglePos(&pos,_iAngle,_iGap);
		iRange	+=	_iGap;

		if	(iRange	>	_iRange)
			break;
		_iTimes--;

		if	(_bIsIncreaseFrame)
			_iFrame++;

		if	(_iFrame>=	m_pAnmData[_iAnm].m_iFrameCount)
			_iFrame	=	0;
	}
}

void
cANM::linerPutByDestPos(int _iX,int _iY,int _iDestX,int _iDestY,int _iAnm,int _iDirect,int _iFrame,int _iRange,int _iGap,int _iScale)
{
	CPos	aPos[100];

	int	iCount	=	GetLinerPath(_iX,_iY,_iDestX,_iDestY,aPos,_iGap,100);

	for	(int i=0;i<iCount;i++)
		PutReg(aPos[i].x,aPos[i].y,_iAnm,_iDirect,_iFrame,_iScale,_iScale);
}

BOOL
cANM::getRect(int anm,int direct,int frame,cRECT *rect,int _iScale)
{
	rect->set(0,0,0,0);

	if	(m_pAnmData[anm].m_iFrameCount	==	0)
		return	FALSE;

	int	index	=	m_pAnmData[anm].GetSprite(direct,frame,m_isFlip);

	if	(!m_sprite.getRect(index,rect,_iScale))
		return	FALSE;

	rect->add(-(m_posRefit.x*_iScale/100),-(m_posRefit.y*_iScale/100));

	if (m_pAnmData[anm].m_isREFITFRAME)
	{
		rect->add(	-(m_pAnmData[anm].m_pPos[direct*m_pAnmData[anm].m_iFrameCount + frame].x*_iScale)/100,
					-(m_pAnmData[anm].m_pPos[direct*m_pAnmData[anm].m_iFrameCount + frame].y*_iScale)/100);
	}

	return	TRUE;
}


/********************************************************************
	퀵 로드 에니메이션 스프라이트(리얼타임으로 불러서 찍는다.)
********************************************************************/

cANMQ::cANMQ()
	  :cANM_BASE()
{	m_pAnmData		=	NULL;
	m_iAnmCount	=	0	;
	m_pFile		=	NULL;
}

cANMQ::~cANMQ()
{	
	Destroy();
}

void
cANMQ::Destroy()
{	
	Reset();
	KILL(m_pFile);
}

void
cANMQ::Reset()
{	
	m_sprite.close();

	pKILL(m_pAnmData);
}

BOOL
cANMQ::Load(char *fn,BOOL loadpalette,cNUX *nux,cMCU *mcu)
{
	cFILE	*file=	NULL;

	if	(nux)
		file	=	nux->Open(fn);
	else
	{
		m_pFile	=	new cFILE();
		file	=	m_pFile;

		if (!file->Open(fn,"rb"))
		{
			KILL(m_pFile);

			file=	NULL;
		}
	}

	if	(!file)	
		return g_eh.addStaticLog("Error in cANMQ::Load - '%s' file not found",fn);

	file->Read(&SADHEADER,sizeof(SADHEADER));

	if	(strcmp(SADHEADER.Reg,REGSADHEADER)	!=	0	&&	strcmp(SADHEADER.Reg,REGSADHEADER2)	!=	0)
		return ERRMSG("Error in cANMQ::Load - header not match!!");

	m_iAnmCount			=	SADHEADER.AnmCount;
	m_sprite.m_iBpp		=	SADHEADER.BPP;
	m_sprite.m_isShadow	=	SADHEADER.bShadow;
	m_sprite.m_isLayer	=	SADHEADER.bOutline;
	m_sprite.m_iCount	=	SADHEADER.ImageCount;

	if	(m_sprite.m_iBpp	==	16)
		m_sprite.m_iBpp	=	dSPRITE_BPP16;
	else
		m_sprite.m_iBpp	=	dSPRITE_BPP8;

	if	(strcmp(SADHEADER.Reg,REGSADHEADER2)	==	0)
	{
		m_sprite.m_iMaxSpriteWidth	=	SADHEADER.m_wMaxSpriteWidth;
		m_sprite.m_iMaxSpriteHeight	=	SADHEADER.m_wMaxSpriteHeight;
		m_sprite.m_iMaxShadowWidth	=	SADHEADER.m_wMaxShadowWidth;
		m_sprite.m_iMaxShadowHeight	=	SADHEADER.m_wMaxShadowHeight;
	}
	else
	{
		m_sprite.m_iMaxSpriteWidth	=	0;
		m_sprite.m_iMaxSpriteHeight	=	0;
		m_sprite.m_iMaxShadowWidth	=	0;
		m_sprite.m_iMaxShadowHeight	=	0;
	}

	return	Load(file,loadpalette,mcu);
}


BOOL
cANMQ::Load(cFILE *file,BOOL loadpalette,cMCU *mcu)
{	int i;

	Reset();	//	몽땅 초기화

	if	(mcu)
		m_sprite.m_isControledByMcu	=	TRUE;
	else
		m_sprite.m_isControledByMcu	=	FALSE;

	//	스프라이트 데이터 로딩

	m_sprite.Load(file,loadpalette,mcu);

	//	에니메이션 데이터 로딩
	m_pAnmData	=	new cANMDATA [m_iAnmCount];

	file->Read(&m_iMoveOval,4);
	file->Read(&m_rectCrash,16);
	file->Read(&m_rectSelect,16);
	file->Read(&m_iKind,4);
	file->Read(&m_posRefit,sizeof(CPos));
	file->Read(&m_iCrashSize,4);
	file->Read(&m_iDefaultAttack,4);

	WORD	wDefaultMagic;
	file->Read(&wDefaultMagic,2);
	m_iDefaultMagic	=	wDefaultMagic;
	file->Read(&m_wIsOccasionallyRestAction,2);

	for (i=0;i<m_iAnmCount;i++)
		m_pAnmData[i].Init(file,mcu);

	{
		m_sprite.m_lpFILE	=	file;

		cRECT	rect;

		m_sprite.getRect(0,&rect);

		m_sprite.m_iHeight	=	-rect.y1;
		m_sprite.m_iHeight	+=	m_posRefit.y;
	}

	return TRUE;
}

void
cANMQ::Put(int x,int y,int anm,int direct,int frame,int xrate,int yrate,int effect,int alpha)
{
	if	(anm	>= m_iAnmCount						)
		return;
	if	(direct >= m_pAnmData[anm].m_iDirectCount	)
		return;
	if	(frame	>= m_pAnmData[anm].m_iFrameCount	)
		return;

	int		fliped	=	c_aIsFlip[m_isFlip][m_pAnmData[anm].m_iAnmType][direct];
	int		index	=	m_pAnmData[anm].GetSprite(direct,frame,m_isFlip);

// 	y	-=	(m_posRefit.y*yrate/100);
// 	x	-=	(m_posRefit.x*xrate/100);
	y	-=	m_posRefit.y;
	x	-=	m_posRefit.x;

	if (m_pAnmData[anm].m_isREFITFRAME)
	{
		x	-=	(m_pAnmData[anm].m_pPos[direct*m_pAnmData[anm].m_iFrameCount + frame].x*xrate)/100;
		y	-=	(m_pAnmData[anm].m_pPos[direct*m_pAnmData[anm].m_iFrameCount + frame].y*yrate)/100;
	}

	m_sprite.Put(x,y,index,xrate,yrate,effect,alpha);
}

void
cANMQ::PutSprite(int x,int y,int index,int xrate,int yrate,int effect,int alpha)
{
// 	y	-=	(m_posRefit.y*yrate/100);
// 	x	-=	(m_posRefit.x*xrate/100);
	y	-=	m_posRefit.y;
	x	-=	m_posRefit.x;

	m_sprite.Put(x,y,index,xrate,yrate,effect,alpha);
}

void
cANMQ::PutAlpha(int x,int y,int anm,int direct,int frame,int xrate,int yrate,int alpha)
{
	int index	=	m_pAnmData[anm].GetSprite(direct,frame,0);

// 	y	-=	(m_posRefit.y*yrate/100);
// 	x	-=	(m_posRefit.x*xrate/100);
	y	-=	m_posRefit.y;
	x	-=	m_posRefit.x;

	if (m_pAnmData[anm].m_isREFITFRAME)
	{
		x	-=	(m_pAnmData[anm].m_pPos[direct*m_pAnmData[anm].m_iFrameCount + frame].x*xrate)/100;
		y	-=	(m_pAnmData[anm].m_pPos[direct*m_pAnmData[anm].m_iFrameCount + frame].y*yrate)/100;
	}

	m_sprite.Put(x,y,index,xrate,yrate,dPUT_ALPHA_BLENDING,alpha);
}

void
cANMQ::PutReg(int x,int y,int anm,int direct,int frame,int xrate,int yrate)
{
	if	(anm	>= m_iAnmCount						)
		return;
	if	(direct >= m_pAnmData[anm].m_iDirectCount	)
		return;
	if	(frame	>= m_pAnmData[anm].m_iFrameCount	)
		return;

	int		index	=	m_pAnmData[anm].GetSprite(direct,frame,0);
	int		alpha	=	m_pAnmData[anm].m_iALPHA;
	
// 	x	-=	(m_posRefit.x*xrate/100);
//	y	-=	(m_posRefit.y*yrate/100);
	y	-=	m_posRefit.y;
	x	-=	m_posRefit.x;

	if (m_pAnmData[anm].m_isREFITFRAME)
	{
		x	-=	(m_pAnmData[anm].m_pPos[direct*m_pAnmData[anm].m_iFrameCount + frame].x*xrate)/100;
		y	-=	(m_pAnmData[anm].m_pPos[direct*m_pAnmData[anm].m_iFrameCount + frame].y*yrate)/100;
	}

	m_sprite.Put(x,y,index,xrate,yrate,alpha,alpha,0);

	anm		=	m_pAnmData[anm].m_iLinkAnm;

	if	(anm	==	0xffff	||	anm	>=	m_iAnmCount	||	m_pAnmData[anm].m_iFrameCount	==	0)
		return;

	frame	=	frame%m_pAnmData[anm].m_iFrameCount;
	index	=	m_pAnmData[anm].GetSprite(direct,frame,m_isFlip);
	alpha	=	m_pAnmData[anm].m_iALPHA;

	m_sprite.Put(x,y,index,xrate,yrate,alpha,alpha,0);
}

void
cANMQ::PutShadow(int x,int y,int anm,int direct,int frame,int xrate,int yrate,int _iAlpha)
{
	if	(anm	>= m_iAnmCount						)
		return;
	if	(direct >= m_pAnmData[anm].m_iDirectCount	)
		return;
	if	(frame	>= m_pAnmData[anm].m_iFrameCount	)
		return;

	int		index	=	m_pAnmData[anm].GetSprite(direct,frame,FALSE);

// 	y	-=	(m_posRefit.y*yrate/100);
// 	x	-=	(m_posRefit.x*xrate/100);
	y	-=	m_posRefit.y;
	x	-=	m_posRefit.x;

	if (m_pAnmData[anm].m_isREFITFRAME)
	{
		x	-=	(m_pAnmData[anm].m_pPos[direct*m_pAnmData[anm].m_iFrameCount + frame].x*xrate)/100;
		y	-=	(m_pAnmData[anm].m_pPos[direct*m_pAnmData[anm].m_iFrameCount + frame].y*yrate)/100;
	}

	m_sprite.PutShadow(x,y,index,xrate,yrate,_iAlpha);
}

//
//	직선 찍기
void
cANMQ::linerPut(int _iX,int _iY,int _iAnm,int _iDirect,int _iFrame,int _iAngle,int _iRange,int _iGap,int _iTimes)
{
	int		iRange	=	0;
	CPos	pos;

	pos.x	=	_iX;
	pos.y	=	_iY;

	while(_iTimes)
	{
		PutReg(pos.x,pos.y,_iAnm,_iDirect,_iFrame);

		GetAnglePos(&pos,_iAngle,_iGap);
		iRange	+=	_iGap;

		if (iRange	>	_iRange)	break;
		_iTimes--;
	}
}


int
cANMQ::getSpriteHeight(int anm,int direct,int frame)
{
	int		iIndex		=	GetSpriteIndex(anm,direct,frame);
	WORD	*lpwData	=	m_sprite.Get16(iIndex);

	if	(lpwData)
		return	lpwData[1];

	BYTE	*lpbData	=	m_sprite.Get8(iIndex);

	if	(lpbData)
		return	*(WORD *)(lpbData+2);

	return 0;
}

int
cANMQ::getSpriteWidth(int anm,int direct,int frame)
{
	int		iIndex		=	GetSpriteIndex(anm,direct,frame);
	WORD	*lpwData	=	m_sprite.Get16(iIndex);

	if	(lpwData)
		return	lpwData[0];

	BYTE	*lpbData	=	m_sprite.Get8(iIndex);

	if	(lpbData)
		return	*(WORD *)lpbData;

	return 0;
}

int
cANMQ::getSpriteDX(int anm,int direct,int frame)
{
	int		iIndex		=	GetSpriteIndex(anm,direct,frame);
	WORD	*lpwData	=	m_sprite.Get16(iIndex);

	if	(lpwData)
		return	(short)lpwData[2];

	BYTE	*lpbData	=	m_sprite.Get8(iIndex);

	if	(lpbData)
		return	*(short *)(lpbData+4);

	return 0;
}

int
cANMQ::getSpriteDY(int anm,int direct,int frame)
{
	int		iIndex		=	GetSpriteIndex(anm,direct,frame);
	WORD	*lpwData	=	m_sprite.Get16(iIndex);

	if	(lpwData)
		return	(short)lpwData[3];

	BYTE	*lpbData	=	m_sprite.Get8(iIndex);

	if	(lpbData)
		return	*(short *)(lpbData+6);

	return 0;
}

BOOL
cANMQ::getRect(int anm,int direct,int frame,cRECT *rect,int _iScale)
{	
	int	index	=	m_pAnmData[anm].GetSprite(direct,frame,m_isFlip);

	if	(!m_sprite.getRect(index,rect,_iScale))
		return	FALSE;

	rect->add(-(m_posRefit.x*_iScale/100),-(m_posRefit.y*_iScale/100));

	if (m_pAnmData[anm].m_isREFITFRAME)
	{
		rect->add(	-(m_pAnmData[anm].m_pPos[direct*m_pAnmData[anm].m_iFrameCount + frame].x*_iScale)/100,
					-(m_pAnmData[anm].m_pPos[direct*m_pAnmData[anm].m_iFrameCount + frame].y*_iScale)/100);
	}

	return	TRUE;
}

/********************************************************************
	오브젝트/건물 이미지 데이터
********************************************************************/

int		g_iImageFrameCounter;	//	오브젝트 에니메이션을 위한 프레임 카운터

cOBJ_ANM_INFO::cOBJ_ANM_INFO()
{	
	m_pPos			=	NULL;
	m_pSprite		=	NULL;
	m_wFrameCount	=	0;
	m_bAlpha		=	dPUT_NORMAL;
	m_isControledByMcu			=	FALSE;
}

cOBJ_ANM_INFO::~cOBJ_ANM_INFO()
{	Reset();
}

void
cOBJ_ANM_INFO::Reset()
{	
	if (!m_isControledByMcu)
	{
		_Free(m_pPos);
		_Free(m_pSprite);
	}

	m_pPos		=	NULL;
	m_pSprite	=	NULL;
}

BOOL
cOBJ_ANM_INFO::Init(cFILE *file,cMCU *mcu)
{	
	Reset();

	file->Read(&m_wFrameCount	,2);

	if (mcu)	m_isControledByMcu	=	TRUE;
	else		m_isControledByMcu	=	FALSE;
	
	if (m_wFrameCount > 0)
	{	m_pSprite	=	_New(WORD,m_wFrameCount,mcu,"cOBJ_ANM_INFO::Sprte");
		m_pPos		=	_New(POINTS,m_wFrameCount,mcu,"cOBJ_ANM_INFO::m_pPos");

		file->Read(&m_bAlpha	,1);
		file->Read(m_pPos		,4,m_wFrameCount);
		file->Read(m_pSprite	,2,m_wFrameCount);
	}

	return TRUE;
}

int
cOBJ_ANM_INFO::GetSprite(int frame)
{	return m_pSprite[frame];
}


cOBJ_ANM::cOBJ_ANM()
{	
	m_pAnmData	=	NULL;
	m_pBLOCK	=	NULL;
	m_pCRASH	=	NULL;
	m_wIsLoaded	=	FALSE;
}


cOBJ_ANM::~cOBJ_ANM()
{
	Reset();
}

void
cOBJ_ANM::Reset()
{
	pKILL(m_pBLOCK);
	pKILL(m_pCRASH);
	pKILL(m_pAnmData);
	
	cSPRITE::close();

	m_wIsLoaded	=	FALSE;
}

BOOL
cOBJ_ANM::Load(char *fn,BOOL loadpalette,cNUX *nux,cMCU *mcu)
{	
	cFILE *file;

	if	(strstr(fn,"Ghost.sad"))
		fn[0]	=	fn[0];

	if	(nux)
		file=	nux->Open(fn);
	else
	{	
		file	=	new cFILE();

		if (!file->Open(fn,"rb"))
			KILL(file);
	}

	if	(!file)
		return ERRMSG("Error in cOBJ_ANM::Load","'%s' file not found",fn);

	file->Read(&SADHEADER,sizeof(SADHEADER));

	if	(	strcmp(SADHEADER.Reg,REGRODHEADER)!=0 && strcmp(SADHEADER.Reg,REGRBDHEADER)!=0 &&	
			strcmp(SADHEADER.Reg,REGRODHEADER2)!=0 && strcmp(SADHEADER.Reg,REGRBDHEADER2)!=0 )
			return ERRMSG("Error in cOBJ_ANM::Load - header not match!!");

	m_wAnmCount			=	SADHEADER.AnmCount;
	m_iAlpha			=	SADHEADER.byAlpha;

	if	(m_iAlpha	==	0)
		m_iAlpha	=	dPUT_NORMAL;
	else
		m_iAlpha--;

	m_iBpp				=	SADHEADER.BPP;
	m_isShadow			=	SADHEADER.bShadow;
	m_isLayer			=	FALSE;					//	레이어를 불러 올 필요는 없다.

	cSPRITE::m_iCount	=	SADHEADER.ImageCount;

	if	(m_iBpp!=8)
		return ERRMSG("Error in cOBJ_ANM::Load - object bpp not 8");

	m_iBpp				=	dSPRITE_BPP8;

	BOOL result			=	Load(file,loadpalette,mcu);

	KILL(file);

	m_wIsLoaded	=	TRUE;
	m_wScale	=	100;

	if	(strcmp(SADHEADER.Reg,REGRODHEADER2)==	0	||	strcmp(SADHEADER.Reg,REGRBDHEADER2)	==	0 )
	{
		m_iMaxSpriteWidth	=	SADHEADER.m_wMaxSpriteWidth;
		m_iMaxSpriteHeight	=	SADHEADER.m_wMaxSpriteHeight;
		m_iMaxShadowWidth	=	SADHEADER.m_wMaxShadowWidth;
		m_iMaxShadowHeight	=	SADHEADER.m_wMaxShadowHeight;
	}
	else
		getMaxSize();

	if	(strcmp(SADHEADER.Reg,REGRODHEADER)	==	0	||	strcmp(SADHEADER.Reg,REGRBDHEADER)	==	0 )
	{
		cFILE convertor;

		if	(l_bIsWantAutoConvert	&&	convertor.Open(fn,"rb+"))
		{
			if	(strcmp(SADHEADER.Reg,REGRODHEADER)	==	0)
				strcpy(SADHEADER.Reg,REGRODHEADER2);
			if	(strcmp(SADHEADER.Reg,REGRBDHEADER)	==	0)
				strcpy(SADHEADER.Reg,REGRBDHEADER2);

			SADHEADER.m_wMaxSpriteWidth		=	m_iMaxSpriteWidth	;
			SADHEADER.m_wMaxSpriteHeight	=	m_iMaxSpriteHeight	;
			SADHEADER.m_wMaxShadowWidth		=	m_iMaxShadowWidth	;
			SADHEADER.m_wMaxShadowHeight	=	m_iMaxShadowHeight	;

			convertor.Write(&SADHEADER,sizeof(SADHEADER));
			convertor.Close();
		}
	}

	return result;
}

int
cOBJ_ANM::GetDoorCrashMap(char *_lpstrFn,BYTE *_lpBuffer,cNUX *nux)
{
	cFILE *file;

	if	(nux)
		file	=	nux->Open(_lpstrFn);
	else
	{
		file	=	new cFILE();

		if (!file->Open(_lpstrFn,"rb"))
			KILL(file);
	}

	if (!file)
		return ERRMSG("Error in cOBJ_ANM::Load","'%s' file not found",_lpstrFn);

	file->Read(&SADHEADER,sizeof(SADHEADER));

	if (SADHEADER.BPP==8)
		file->Seek(512,SEEK_CUR);

	int	iSize,iAttr,iCount	=	SADHEADER.ImageCount;

	file->Seek(iCount*4,SEEK_CUR);
	file->Read(&iSize,4);

	if (SADHEADER.BPP	==	16)
		file->Seek(iSize*2,SEEK_CUR);
	else
		file->Seek(iSize,SEEK_CUR);

	//	그림자 불러오기
	if (SADHEADER.bShadow)
	{	
		file->Seek(iCount*4);
		file->Read(&iSize,4);

		if (iSize)
			file->Seek(iSize,SEEK_CUR);
	}

	file->Seek(iCount*4,SEEK_CUR);
	file->Read(&iSize,4);
	file->Seek(iSize,SEEK_CUR);

	file->Seek(4,SEEK_CUR);

	for (int i=0;i<SADHEADER.AnmCount;i++)
	{
		WORD	wFrameCount;

		file->Read(&wFrameCount	,2);

		if (wFrameCount > 0)
			file->Seek(1+4*wFrameCount+2*wFrameCount,SEEK_CUR);
	}

//	스킵 데이터
	int	iCrashMapSize	=	0;

	{
		file->Read(&iCrashMapSize,4);

		iCrashMapSize	-=	8;

		file->Read(_lpBuffer,8);
		file->Read(_lpBuffer,iCrashMapSize);

		file->Read(&iSize,4);
		file->Seek(iSize,SEEK_CUR);	//	블럭 정보
		file->Read(&iSize,4);
		file->Seek(iSize,SEEK_CUR);	//	오버랩 정보
		file->Seek(sizeof(cRECT),SEEK_CUR);	//	충돌 박스
	}

	file->Read(&iAttr,4);

	KILL(file);

	if (iAttr	==	dOBJECT_TYPE_DOOR)
		return	iCrashMapSize;

	return 0;
}

BOOL
cOBJ_ANM::Load(cFILE *file,BOOL loadpalette,cMCU *mcu)
{
	int i;

	Reset();									//	초기화

	if (mcu)
		m_isControledByMcu	=	TRUE;
	else
		m_isControledByMcu	=	FALSE;

	cSPRITE::Load(file,loadpalette,mcu);		//	스프라이트 데이터 로딩

	//	에니메이션 데이터..

	m_pAnmData	=	new cOBJ_ANM_INFO [m_wAnmCount];

	file->Read(&m_iPutMethod,4);

	for (i=0;i<m_wAnmCount;i++)
		m_pAnmData[i].Init(file,mcu);

	int	iCrashInfoPos	=	file->Locate();
	int	iSize;

//	스킵 데이터
	{
		file->Read(&iSize,4);file->Seek(iSize,SEEK_CUR);	//	충돌 정보
		file->Read(&iSize,4);file->Seek(iSize,SEEK_CUR);	//	블럭 정보
		file->Read(&iSize,4);file->Seek(iSize,SEEK_CUR);	//	오버랩 정보
		file->Seek(sizeof(cRECT),SEEK_CUR);	//	충돌 박스
	}

	file->Read(&m_iAttr,4);
	file->Seek(-4,SEEK_END);
	file->Read(&m_iFPS,4);

//	충돌 박스
	if (m_iAttr	==	dOBJECT_TYPE_DOOR)
	{
		file->Seek(iCrashInfoPos,SEEK_SET);

		file->Read(&iSize,4);
		iSize		-=	8;

		pKILL(m_pCRASH);
		m_pCRASH	=	new	BYTE [iSize];

		file->Read(m_pCRASH,8);
		file->Read(m_pCRASH,iSize);
	}


	if (m_iFPS	<=	2	||	m_iFPS	>=	32	)	
		m_iFPS	=	8;

	m_wIsLoaded	=	TRUE;

	return TRUE;
}

void
cOBJ_ANM::Put(int index,int x,int y)
{
	if (m_wIsLoaded	==	FALSE)
		return;

	_PutSprite(x,y,m_iBpp,0,NULL,m_p8Sprite+m_pSpriteOffset[index],m_lpPlt,m_wScale,m_wScale,m_iAlpha,0,m_iMaxSpriteWidth,m_iMaxSpriteHeight);
}

void
cOBJ_ANM::Put(int x,int y)
{
	if	(m_wIsLoaded	==	FALSE)
		return;

	Put(0,x,y);

	if	(m_iCount	>	1)
		if (m_iAttr	==	dOBJECT_TYPE_ANM || m_iAttr	==	dOBJECT_TYPE_BRIDGE)
		{
			int	iFrame	=	(g_iImageFrameCounter*m_iFPS/64)%(m_iCount-1)+1;

			_PutSprite(x,y,m_iBpp,0,NULL,m_p8Sprite+m_pSpriteOffset[iFrame],m_lpPlt,m_wScale,m_wScale,m_pAnmData[0].m_bAlpha,m_pAnmData[0].m_bAlpha,m_iMaxSpriteWidth,m_iMaxSpriteHeight);
		}
}

void
cOBJ_ANM::PutShadow(int x,int y)
{
	if (m_wIsLoaded	==	FALSE)
		return;

	if (!m_pShadow)
		return;

	cSHADOW::PutShadow(x,y,m_pShadow+m_pShadowOffset[0],m_wScale,m_wScale,m_iMaxShadowWidth,m_iMaxShadowHeight);
}

// void
// cOBJ_ANM::ShadowMask(int x,int y)
// {
// 	if (m_wIsLoaded	==	FALSE)
// 		return;
// 
// 	if (!m_pShadow)
// 		return;
// 
// 	cSHADOW::ShadowMask(x,y,m_pShadow+m_pShadowOffset[0]);
// }

/********************************************************************
	cIMAGE
********************************************************************/

BOOL
cIMAGE::Put(int x,int y)
{	if (m_p16IMAGE)
	{	cDRAW::PutImage(x,y,m_p16IMAGE);
		return TRUE;
	}

	if (m_p8IMAGE)
	{	cDRAW::PutImage(x,y,m_p8IMAGE,m_pPlt);
		return TRUE;
	}

	return TRUE;
}





//┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//┃이미지 클래스... 그런데... 왜 SMI지.. -_-;;
cSMI::cSMI()
{
	m_iCount	=	0;	//	오프셋,이미지 수
	m_pOffset	=	NULL;
	m_pwImage	=	NULL;	//	이미지
	m_pFile		=	NULL;
	m_lpMcu		=	NULL;//	Mcu
	m_pbImage	=	NULL;
	m_pPlt		=	NULL;
}

//
//	디스트럭터
cSMI::~cSMI()
{
	close();
}	//	cSMI::~cSMI()

//
//	닫기
void
cSMI::close()
{
	KILL(m_pFile);

	if (!m_lpMcu)
	{
		_Free(m_pwImage);
		_Free(m_pbImage);
		_Free(m_pPlt);
		_Free(m_pOffset);
	}

	m_iCount	=	0;	//	오프셋,이미지 수
	m_lpMcu		=	NULL;//	Mcu
	m_pOffset	=	NULL;
	m_pwImage	=	NULL;	//	이미지
}	//	cSMI::close()

//
//	걍 로드
BOOL
cSMI::load(char *fn,cNUX *_lpNux,cMCU *_lpMcu)
{
	cFILE	*pFile;

	if (_lpNux)	pFile	=	_lpNux->Open(fn);
	else
	{
		pFile	=	new cFILE();
		if (!pFile->Open(fn,"rb"))	KILL(pFile);
	}

	if (!pFile)	return ERRMSG("cSMI::load","'%s' file not found",fn);

	close();

	m_lpMcu	=	_lpMcu;

	pFile->Skip(60);
	pFile->Read(&m_iBPP,4);

	if (m_iBPP != 16 && m_iBPP != 8)
		m_iBPP = 16;

	pFile->Read(&m_iCount,4);

	m_pOffset	=	_New(int,m_iCount+1,_lpMcu,"cSMI::m_pOffset");

	pFile->Read(m_pOffset,(m_iCount+1)*sizeof(int));

	if (m_iBPP	==	16)
	{
		m_pwImage	=	_New(WORD,m_pOffset[m_iCount],_lpMcu,"cSMI::m_pwImage");
		pFile->Read(m_pwImage,m_pOffset[m_iCount],2);

		for (int i=0;i<m_iCount;i++)
			CONVERTIMAGE(m_pwImage+m_pOffset[i]);
	}
	else
	{
		m_pbImage	=	_New(BYTE,m_pOffset[m_iCount],_lpMcu,"cSMI::m_pbImage");
		m_pPlt		=	_New(WORD,256,_lpMcu,"cSMI::m_pPlt");
		pFile->Read(m_pbImage,m_pOffset[m_iCount]);
		pFile->Read(m_pPlt,512);

		if (g_iPixelFormat != dRGB565)
			for (int i=0;i<256;i++)	
				m_pPlt[i]	=	RGB565ToRGB555(m_pPlt[i]);

		setPalette(m_pPlt);
	}

	KILL(pFile);

	return	TRUE;
}	//	load(char *fn,cNUX *_lpNux=NULL,cMCU *_lpMcu=NULL)

//
//	인스턴트 출력을 생각하고 로딩-_-a
BOOL
cSMI::iLoad(char *fn,cNUX *_lpNux,cMCU *_lpMcu)
{
	cFILE	*pFile;

	if (_lpNux)	pFile	=	_lpNux->Open(fn);
	else
	{	pFile	=	new cFILE();
		if (!pFile->Open(fn,"rb"))	KILL(pFile);
	}

	if (!pFile)	return ERRMSG("cSMI::iLoad","'%s' file not found",fn);

	close();

	m_lpMcu	=	_lpMcu;

	pFile->Skip(60);
	pFile->Read(&m_iBPP,4);
	pFile->Read(&m_iCount,4);

	m_pOffset	=	_New(int,m_iCount+1,_lpMcu,"cSMI::m_pOffset");

	pFile->Read(m_pOffset,(m_iCount+1)*sizeof(int));

	m_iImageLocate	=	pFile->Locate();
	m_pFile			=	pFile;

	return	TRUE;
}	//	cSMI::iLoad(char *fn,cNUX *_lpNux=NULL,cMCU *_lpMcu=NULL)

void
cSMI::put(int _iIndex,int _iX,int _iY,int _iAlpha)
{
	CRY(_iIndex >= m_iCount,_ms("Error in cSMI::put - _iIndex >= m_iCount - %d",_iIndex));

	if (m_pwImage)
	{
		cDRAW::PutImage(_iX,_iY,m_pwImage+m_pOffset[_iIndex]);
		return;
	}

	if (m_pbImage)
	{
		cDRAW::PutImage(_iX,_iY,m_pbImage+m_pOffset[_iIndex],m_lpPlt);
		return;
	}

	int	iSize	=	(m_pOffset[_iIndex+1]-m_pOffset[_iIndex])*sizeof(WORD);

	AllocTemplateBuffer(iSize);

	m_pFile->Seek(m_iImageLocate+m_pOffset[_iIndex]*2,SEEK_SET);
	m_pFile->Read(g_pTemplateBuffer,iSize);

	CONVERTIMAGE((WORD *)g_pTemplateBuffer);

	cDRAW::PutImage(_iX,_iY,(WORD *)g_pTemplateBuffer);
}	//	cSMI::put(int _iIndex,int _iX=0,int _iY=0,int _iAlpha=32)

//
//	확대/축소해서 출력
void
cSMI::putScale(int _iIndex,int _iX,int _iY,int _iScale)
{
	if (m_pwImage)
	{
		cDRAW::PutImageScale(_iX,_iY,m_pwImage+m_pOffset[_iIndex],_iScale);
		return;
	}

	if (m_pbImage)
	{
		cDRAW::PutImageScale(_iX,_iY,m_pbImage+m_pOffset[_iIndex],m_lpPlt,_iScale);
		return;
	}

	if (m_iBPP	==	16)
	{
		int	iSize	=	(m_pOffset[_iIndex+1]-m_pOffset[_iIndex])*sizeof(WORD);

		AllocTemplateBuffer(iSize);

		m_pFile->Seek(m_iImageLocate+m_pOffset[_iIndex]*2,SEEK_SET);
		m_pFile->Read(g_pTemplateBuffer,iSize);

		CONVERTIMAGE((WORD *)g_pTemplateBuffer);

		cDRAW::PutImageScale(_iX,_iY,(WORD *)g_pTemplateBuffer,_iScale);

		return;
	}

	if (m_iBPP	==	8)
	{
		int	iSize	=	(m_pOffset[_iIndex+1]-m_pOffset[_iIndex]);

		AllocTemplateBuffer(iSize);

		m_pFile->Seek(m_iImageLocate+m_pOffset[_iIndex],SEEK_SET);
		m_pFile->Read(g_pTemplateBuffer,iSize);

		cDRAW::PutImageScale(_iX,_iY,g_pTemplateBuffer,m_lpPlt,_iScale);
	}

}	//	cSMI::putScale(int _iIndex,int _iX,int _iY,int _iScale)


WORD*
cSMI::get16Image(int _iIndex)
{
	CRY(_iIndex >= m_iCount,"Error in cSMI::put - _iIndex >= m_iCount");

	if (m_pwImage)	return	m_pwImage+m_pOffset[_iIndex];

	int	iSize	=	(m_pOffset[_iIndex+1]-m_pOffset[_iIndex])*sizeof(WORD);

	AllocTemplateBuffer(iSize);

	m_pFile->Seek(m_iImageLocate+m_pOffset[_iIndex]*2,SEEK_SET);
	m_pFile->Read(g_pTemplateBuffer,iSize);

	CONVERTIMAGE((WORD *)g_pTemplateBuffer);

	return	(WORD *)g_pTemplateBuffer;
}

BYTE*
cSMI::get8Image(int _iIndex)
{
	CRY(_iIndex >= m_iCount,"Error in cSMI::put - _iIndex >= m_iCount");

	if (m_pbImage)	return	m_pbImage+m_pOffset[_iIndex];

	int	iSize	=	(m_pOffset[_iIndex+1]-m_pOffset[_iIndex]);

	AllocTemplateBuffer(iSize);

	m_pFile->Seek(m_iImageLocate+m_pOffset[_iIndex],SEEK_SET);
	m_pFile->Read(g_pTemplateBuffer,iSize);

	return	g_pTemplateBuffer;
}

void
cSMI::setPalette(WORD *_lpPalette)
{
	if (_lpPalette)
		m_lpPlt	=	_lpPalette;
}

void
cSMI::restorePalette()
{
	setPalette(m_pPlt);
}

void
cSMI::putBySpriteFilter(int _iX,int _iY,int _iIndex,BYTE *_lpBaseShape,int _iScale)
{
	if (m_pwImage)
	{
		cDRAW::PutSpriteByReferenceImage(_iX,_iY,m_pwImage+m_pOffset[_iIndex],_lpBaseShape,_iScale);
		return;
	}

	if (m_pbImage)
	{
		cDRAW::PutSpriteByReferenceImage(_iX,_iY,m_pbImage+m_pOffset[_iIndex],m_lpPlt,_lpBaseShape,_iScale);
		return;
	}

	if (m_iBPP	==	16)
	{
		int	iSize	=	(m_pOffset[_iIndex+1]-m_pOffset[_iIndex])*sizeof(WORD);

		AllocTemplateBuffer(iSize);

		m_pFile->Seek(m_iImageLocate+m_pOffset[_iIndex]*2,SEEK_SET);
		m_pFile->Read(g_pTemplateBuffer,iSize);

		CONVERTIMAGE((WORD *)g_pTemplateBuffer);

		cDRAW::PutSpriteByReferenceImage(_iX,_iY,(WORD *)g_pTemplateBuffer,_lpBaseShape,_iScale);

		return;
	}

	if (m_iBPP	==	8)
	{
		int	iSize	=	(m_pOffset[_iIndex+1]-m_pOffset[_iIndex]);

		AllocTemplateBuffer(iSize);

		m_pFile->Seek(m_iImageLocate+m_pOffset[_iIndex],SEEK_SET);
		m_pFile->Read(g_pTemplateBuffer,iSize);

		cDRAW::PutSpriteByReferenceImage(_iX,_iY,g_pTemplateBuffer,m_lpPlt,_lpBaseShape,_iScale);
	}
}