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
	WORD	Width;
	WORD	Height;
	char	Bits;
	char	Descriptor;
}	TGA_HEADER;
#pragma pack()


BOOL
cTGA::Load(FILE *fp)
{	pKILL(p16IMAGE);
	pKILL(p8IMAGE);
	pKILL(pPLT);

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
	MSGOUT("TGA_HEADER.Width        ->%d",TGA_HEADER.Width);
	MSGOUT("TGA_HEADER.Height       ->%d",TGA_HEADER.Height);
	MSGOUT("TGA_HEADER.Bits         ->%d",TGA_HEADER.Bits);
	MSGOUT("TGA_HEADER.Descriptor   ->%d",TGA_HEADER.Descriptor);
*/
	Width	=	TGA_HEADER.Width;
	Height	=	TGA_HEADER.Height;
	BPP		=	TGA_HEADER.Bits;

	if(TGA_HEADER.Bits == 16)
	{	p16IMAGE	=	new WORD [(Width * Height)+2];
		p16IMAGE[0]	=	(WORD) Width;
		p16IMAGE[1]	=	(WORD) Height;

		for(int line = Height-1; line >=0; line--)	fread(p16IMAGE+line*Width+2,Width*2,1,fp);
	}
	else
	if(TGA_HEADER.Bits == 8)
	{	p8IMAGE				=	new BYTE [(Width * Height)+4];
		*(WORD *)(p8IMAGE)	=	(WORD) Width;
		*(WORD *)(p8IMAGE+2)=	(WORD) Height;

		int		colors		=	TGA_HEADER.ColorMapLast-TGA_HEADER.ColorMapFirst;

		BYTE	PALETTE[768];
		memset(PALETTE,0,768);
		fread(PALETTE,colors*3,1,fp);

		for (int i=0;i<256;i++)	swap(PALETTE[i*3],PALETTE[i*3+2]);

//		pPLT	=	PALETTE24TO15(PALETTE);

		for(int line = Height-1; line >=0; line--)	fread(p8IMAGE+line*Width+4,Width,1,fp);
	}
	else	return MSGOUT("Error!! this .TGA File '%d' Bits Format",TGA_HEADER.Bits);

	return TRUE;
}

BOOL
cTGA::Load(char *fn,BOOL change16bit)
{	FILE	*fp;
	fp	=	fopen(fn,"rb");

	if(!fp) return MSGOUT("'%s' file not Found!!",fn);

	BOOL result	=	Load(fp);

	fclose(fp);

	if (change16bit && p8IMAGE)
	{	p16IMAGE	=	new WORD [Width*Height+2];
		p16IMAGE[0]	=	(WORD)Width;
		p16IMAGE[1]	=	(WORD)Height;

		for (int i = 0 ; i < Width*Height; i++)	p16IMAGE[i+2]	=	pPLT[p8IMAGE[i+4]];
		pKILL(p8IMAGE);
	}

	return result;
}

BOOL
cTGA::Save(char *fn,WORD *image,BOOL overWrite)
{	if (!overWrite)	if (IsFile(fn)) return FALSE;

	if (!image	) return MSGOUT("Error in cTGA::Save - value 'image' are valid!!",fn);

	FILE *fp;
	fp	=	fopen(fn,"wb");

	if (!fp		) return MSGOUT("Error in cTGA::Save - '%s' file open(write) error!!",fn);

	int		xs	=	image[0];
	int		ys	=	image[1];

	TGA_HEADER.idLength     =0;
	TGA_HEADER.ColorMapType =0;
	TGA_HEADER.ImageType    =2;
	TGA_HEADER.ColorMapFirst=0;
	TGA_HEADER.ColorMapLast =0;
	TGA_HEADER.ColorMapBits =0;
	TGA_HEADER.FirstX       =0;
	TGA_HEADER.FirstY       =0;
	TGA_HEADER.Width        =xs;
	TGA_HEADER.Height       =ys;
	TGA_HEADER.Bits         =16;
	TGA_HEADER.Descriptor   =1;

	fwrite(&TGA_HEADER,sizeof(TGA_HEADER),1,fp);

	for(int line = ys-1; line >=0; line--)	fwrite(image+line*xs+2,xs*2,1,fp);

	fclose(fp);

	return TRUE;
}
