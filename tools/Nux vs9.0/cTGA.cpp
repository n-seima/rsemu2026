#include "cIMAGE.H"
#include "cDRAW.H"

#pragma pack(1)

struct TGA_HEADER_TYPE
{	char	idLength;
	char	ColorMapType;
	char	ImageType;
	WORD	ColorMapFirst;
	WORD	ColorMapLast;
	char	ColorMapBits;
	WORD	FirstX;
	WORD	FirstY;
	WORD	m_iWidth;
	WORD	m_iHeight;
	char	Bits;
	char	Descriptor;
}	TGA_HEADER;
#pragma pack()


BOOL
cTGA::Load(FILE *fp)
{	_free(m_p16IMAGE);
	_free(m_p8IMAGE);
	_free(m_pPLT);

	memset(&TGA_HEADER	,0,sizeof(TGA_HEADER));

	fread(&TGA_HEADER	,sizeof(TGA_HEADER),1,fp);
/*
	MSGOUT("TGA_HEADER.idLength     ->%d",TGA_HEADER.idLength);
	MSGOUT("TGA_HEADER.ColorMapType ->%d",TGA_HEADER.ColorMapType);
	MSGOUT("TGA_HEADER.ImageType    ->%d",TGA_HEADER.ImageType);
	MSGOUT("TGA_HEADER.ColorMapFirst->%d",TGA_HEADER.ColorMapFirst);
	MSGOUT("TGA_HEADER.ColorMapLast ->%d",TGA_HEADER.ColorMapLast);
	MSGOUT("TGA_HEADER.ColorMapBits ->%d",TGA_HEADER.ColorMapBits);
	MSGOUT("TGA_HEADER.FirstX       ->%d",TGA_HEADER.FirstX);
	MSGOUT("TGA_HEADER.FirstY       ->%d",TGA_HEADER.FirstY);
	MSGOUT("TGA_HEADER.m_iWidth        ->%d",TGA_HEADER.m_iWidth);
	MSGOUT("TGA_HEADER.m_iHeight       ->%d",TGA_HEADER.m_iHeight);
	MSGOUT("TGA_HEADER.Bits         ->%d",TGA_HEADER.Bits);
	MSGOUT("TGA_HEADER.Descriptor   ->%d",TGA_HEADER.Descriptor);
*/
	m_iWidth	=	TGA_HEADER.m_iWidth;
	m_iHeight	=	TGA_HEADER.m_iHeight;

	if(TGA_HEADER.Bits == 16)
	{	m_p16IMAGE	=	_new(WORD,m_iWidth * m_iHeight+2,NULL, "cTGA::m_p16IMAGE");
		m_p16IMAGE[0]	=	(WORD) m_iWidth;
		m_p16IMAGE[1]	=	(WORD) m_iHeight;

		for(int line = m_iHeight-1; line >=0; line--)	fread(m_p16IMAGE+line*m_iWidth+2,m_iWidth*2,1,fp);
	}
	else
	if(TGA_HEADER.Bits == 8)
	{	m_p8IMAGE				=	_new(BYTE,(m_iWidth * m_iHeight)+4,NULL,"cTGA::m_p8IMAGE");
		*(WORD *)(m_p8IMAGE)	=	(WORD) m_iWidth;
		*(WORD *)(m_p8IMAGE+2)=	(WORD) m_iHeight;

		int		colors		=	TGA_HEADER.ColorMapLast-TGA_HEADER.ColorMapFirst;

		BYTE	PALETTE[768];
		memset(PALETTE,0,768);
		fread(PALETTE,colors*3,1,fp);

		for (int i=0;i<256;i++)	swap(PALETTE[i*3],PALETTE[i*3+2]);

		m_pPLT	=	Correct8BitPalette(PALETTE);

		for(int line = m_iHeight-1; line >=0; line--)	fread(m_p8IMAGE+line*m_iWidth+4,m_iWidth,1,fp);
	}
	else	return MSGOUT("Error!! this .TGA File '%d' Bits Format",TGA_HEADER.Bits);

	return TRUE;
}

BOOL
cTGA::Load(char *fn,BOOL change16bit)
{	FILE	*fp;
	fp	=	fopen(fn,"rb");

	if(fp == NULL) return ERRMSG("'%s' file not Found!!");

	BOOL result	=	Load(fp);

	fclose(fp);

	if (change16bit && m_p8IMAGE)
	{	m_p16IMAGE	=	_new(WORD,m_iWidth*m_iHeight+2,NULL,"cTGA::m_p16IMAGE");
		m_p16IMAGE[0]	=	(WORD)m_iWidth;
		m_p16IMAGE[1]	=	(WORD)m_iHeight;

		for (DWORD i = 0 ; i < (DWORD)m_iWidth*m_iHeight; i++)	m_p16IMAGE[i+2]	=	m_pPLT[m_p8IMAGE[i+4]];

		_free(m_p8IMAGE);
	}

	return result;
}

BOOL
cTGA::Save(char *fn,WORD *image,BOOL overWrite)
{	if (!overWrite)	if (cFILE::IsFile(fn)) return FALSE;

	if (!image	) return MSGOUT("Error in cTGA::Save - value 'image' are valid!!",fn);

	cFILE	file;

	if (!file.open(fn,"wb")) return MSGOUT("Error in cTGA::Save - '%s' file open(write) error!!",fn);

	int	xs	=	image[0];
	int	ys	=	image[1];

	TGA_HEADER.idLength     =0;
	TGA_HEADER.ColorMapType =0;
	TGA_HEADER.ImageType    =2;
	TGA_HEADER.ColorMapFirst=0;
	TGA_HEADER.ColorMapLast =0;
	TGA_HEADER.ColorMapBits =0;
	TGA_HEADER.FirstX		=0;
	TGA_HEADER.FirstY		=0;
	TGA_HEADER.m_iWidth		=xs;
	TGA_HEADER.m_iHeight	=ys;
	TGA_HEADER.Bits			=16;
	TGA_HEADER.Descriptor	=1;

	file.write(&TGA_HEADER,sizeof(TGA_HEADER));

	for(int line = ys-1; line >=0; line--)	file.write(image+line*xs+2,xs*2);

	file.close();

	return TRUE;
}

BOOL
cTGA::Save(char *fn,WORD *image,int width,int height,BOOL overWrite)
{	if (!overWrite)	if (cFILE::IsFile(fn)) return FALSE;

	if (!image	) return MSGOUT("Error in cTGA::Save - value 'image' are valid!!",fn);

	cFILE	file;

	if (!file.open(fn,"wb")) return MSGOUT("Error in cTGA::Save - '%s' file open(write) error!!",fn);

	TGA_HEADER.idLength     =0;
	TGA_HEADER.ColorMapType =0;
	TGA_HEADER.ImageType    =2;
	TGA_HEADER.ColorMapFirst=0;
	TGA_HEADER.ColorMapLast =0;
	TGA_HEADER.ColorMapBits =0;
	TGA_HEADER.FirstX       =0;
	TGA_HEADER.FirstY       =0;
	TGA_HEADER.m_iWidth		=width;
	TGA_HEADER.m_iHeight	=height;
	TGA_HEADER.Bits         =16;
	TGA_HEADER.Descriptor   =1;

	file.write(&TGA_HEADER,sizeof(TGA_HEADER));

	for(int line = height-1; line >=0; line--)	file.write(image+line*width,width*2);

	file.close();

	return TRUE;
}