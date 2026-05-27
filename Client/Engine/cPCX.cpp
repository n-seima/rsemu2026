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
decode(BYTE *buffer, int fullsize,int bpl,int width, cFILE *file)
{   int index, data, num,count = 0;

    for(index = 0; index < fullsize;) 
	{	data = fgetc(file->fp);

        if (data >= 0xc0)
		{	num	=	data & 0x3f;
			data = fgetc(file->fp);
			while(num--)
			{	index++;
				if ((index % bpl) < width)	buffer[count++] = data;
			}
		}
        else
		{	index++;
			if ((index % bpl) < width)	buffer[count++] = data;
		}
    }
}

BOOL
cPCX::Load(char * fname,BOOL change16bit)
{	_Free(m_p16IMAGE);
	_Free(m_p8IMAGE);
	_Free(m_pPlt);

	cFILE	file;

	if (!file.Open(fname,"rb"))	return ERRMSG("Error in cPCX::Load","can't find '%s' file",fname);

	file.Read(&PCX_HEADER, sizeof(PCX_HEADER));

	m_iWidth	= PCX_HEADER.ex - PCX_HEADER.sx + 1;
	m_iHeight	= PCX_HEADER.ey - PCX_HEADER.sy + 1;

	if ( (PCX_HEADER.manufacturer != 10) || (PCX_HEADER.version != 5) )
		return ERRMSG("Error in cPCX::Load","It's not a PCX File '%s'",fname);

	if ( PCX_HEADER.color_planes == 3 )
	{	BYTE	*buffer	=	_New(BYTE,m_iWidth * m_iHeight * 3,NULL,"cPCX::Load buffer");

		decode(buffer,PCX_HEADER.bytes_per_line*m_iHeight*3,PCX_HEADER.bytes_per_line, m_iWidth, &file);

		m_p16IMAGE	=	PCX24TO16(buffer,m_iWidth,m_iHeight);			//	이미지 변환 24bit -> 16bit
		_Free(buffer);

		m_iBpp		=	16;
	}
	else
	{	m_p8IMAGE	=	_New(BYTE,m_iWidth * m_iHeight+4,NULL,"cPCX::m_p8IMAGE");
		*(WORD *)(m_p8IMAGE	)	=	m_iWidth;
		*(WORD *)(m_p8IMAGE+2	)	=	m_iHeight;

		decode(m_p8IMAGE+4,PCX_HEADER.bytes_per_line*m_iHeight,PCX_HEADER.bytes_per_line, m_iWidth, &file);

		file.Skip(1);
		file.Read(palette, 256 * 3);
		m_pPlt	=	CONVERTPALETTE(palette);
		m_iBpp		=	8;
	}

	file.Close();

	if (change16bit && m_p8IMAGE)
	{	m_p16IMAGE	=	_New(WORD,m_iWidth*m_iHeight+2,NULL,"cPCX::m_p16IMAGE");
		m_p16IMAGE[0]	=	(WORD)m_iWidth;
		m_p16IMAGE[1]	=	(WORD)m_iHeight;

		for (DWORD i = 0 ; i < (DWORD)m_iWidth*m_iHeight; i++)	m_p16IMAGE[i+2]	=	m_pPlt[m_p8IMAGE[i+4]];

		_Free(m_p8IMAGE);
	}

	return TRUE;
}