#include "cIMAGE.H"
#include "cTRANS.H"

#pragma pack(1)
struct PCX_HEADER_TYPE 
{	char	manufacturer;
	char	version;
	char	encoding;
	char	bits_per_pixel;
	short	sx, sy, ex, ey;
	short	hres, vres;
	char	pallete[48];
	char	reserved;
	char	color_planes;
	short	bytes_per_line;
	short	pallete_type;
	char	filler[58];
} PCX_HEADER;
#pragma pack()

BYTE	palette[768];

void
cPCX::decode(BYTE *buffer, int fullsize,int bpl,int width, cFILE *file)
{   int index, data, num,count = 0;

    for(index = 0; index < fullsize;) 
	{	data = fgetc(file->fp);

        if (data >= 0xc0)
		{	num	=	data & 0x3f;
			data = fgetc(file->fp);
			while(num--)
			{
				if ((index % bpl) < width)	buffer[count++] = data;
				index++;
			
			}
		}
        else
		{	
			if ((index % bpl) < width)	buffer[count++] = data;
			index++;
		}
    }
}

BOOL
cPCX::Load(char * fname,BOOL change16bit)
{	pKILL(p16IMAGE);
	pKILL(p8IMAGE);
	pKILL(pPLT);

	cFILE	file;

	if (!file.Open(fname,"rb"))	return ERRMSG("Error in cPCX::Load","can't find '%s' file",fname);

	file.Read(&PCX_HEADER, sizeof(PCX_HEADER));
/*
	MSGOUT("PCX file : %s", fname);
	MSGOUT("- HEADER INFO -");
	MSGOUT("  manufacturer: %d", PCX_HEADER.manufacturer);
	MSGOUT("       version: %d", PCX_HEADER.version);
	MSGOUT("      encoding: %d", PCX_HEADER.encoding);
	MSGOUT("bits_per_pixel: %d", PCX_HEADER.bits_per_pixel);
	MSGOUT("sx, sy, ex, ey: %d, %d, %d, %d", PCX_HEADER.sx, PCX_HEADER.sy, PCX_HEADER.ex, PCX_HEADER.ey);
	MSGOUT("    hres, vres: %d, %d", PCX_HEADER.hres, PCX_HEADER.vres);
	MSGOUT("   pallete[48]:");
	MSGOUT("      reserved:");
	MSGOUT("  color_planes: %d", PCX_HEADER.color_planes);
	MSGOUT("bytes_per_line: %d", PCX_HEADER.bytes_per_line);
	MSGOUT("  pallete_type: %d", PCX_HEADER.pallete_type);
	MSGOUT("    filler[58]:");
*/
	Width	= PCX_HEADER.ex - PCX_HEADER.sx + 1;
	Height	= PCX_HEADER.ey - PCX_HEADER.sy + 1;

	if ( (PCX_HEADER.manufacturer != 10) || (PCX_HEADER.version != 5) )
		return ERRMSG("Error in cPCX::Load","It's not a PCX File '%s'",fname);

	if ( PCX_HEADER.color_planes == 3 )
	{	BYTE	*buffer	=	new BYTE [Width * Height * 3];

		decode(buffer, Height*PCX_HEADER.bytes_per_line*3,PCX_HEADER.bytes_per_line,Width , &file);

		p16IMAGE	=	PCX24TO16(buffer,Width,Height);			//	이미지 변환 24bit -> 16bit
		pKILL(buffer);

		BPP		=	16;
	}
	else
	{	p8IMAGE	=	new BYTE [Width * Height+4];
		*(WORD *)(p8IMAGE	)	=	Width;
		*(WORD *)(p8IMAGE+2	)	=	Height;

		decode(p8IMAGE+4,Height*PCX_HEADER.bytes_per_line,PCX_HEADER.bytes_per_line, Width , &file);

		file.Seek(1,SEEK_CUR);
		file.Read(palette, 256 * 3);
		pPLT	=	CONVERTPALETTE(palette);
		BPP		=	8;
	}

	file.Close();

	if (change16bit && p8IMAGE)
	{	p16IMAGE	=	new WORD [Width*Height+2];
		p16IMAGE[0]	=	(WORD)Width;
		p16IMAGE[1]	=	(WORD)Height;

		for (DWORD i = 0 ; i < (DWORD)Width*Height; i++)	p16IMAGE[i+2]	=	pPLT[p8IMAGE[i+4]];

		pKILL(p8IMAGE);
	}

	return TRUE;
}