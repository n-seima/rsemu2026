#include "cIMAGE.H"
#include "cDRAW.H"
#include "cTRANS.H"

BOOL 
cBMP::Save(char *fn,WORD *_lpData,int _iWidth,int _iHeight,BOOL _bIsOverWrite)
{
	if (_bIsOverWrite	==	FALSE)
		if (IsFile(fn))
			return FALSE;

	BYTE	*pBuffer	=	new BYTE [_iWidth*_iHeight*3];

	for (int i=0;i<_iHeight;i++)
		for (int j=0;j<_iWidth;j++)
		{
			int	iIndex	=	_iWidth*(_iHeight-i-1)+j;
			int	ii		=	_iWidth*i+j;

			RGB16To24(_lpData[iIndex+2],pBuffer[ii*3+2],pBuffer[ii*3+1],pBuffer[ii*3  ]);
		}

	Save(fn,pBuffer,_iWidth,_iHeight);

	pKILL(pBuffer);

	return	TRUE;
}

BOOL 
cBMP::Save(char *fn,BYTE *_lpData,int _iWidth,int _iHeight)
{
	FILE	*fp;

	fp	=	fopen(fn,"wb");

	BITMAPFILEHEADER	fileHeader;

	fileHeader.bfOffBits	=	sizeof(tagBITMAPINFOHEADER)+sizeof(fileHeader);
	fileHeader.bfType		=	0x4d42;
	fileHeader.bfSize		=	_iWidth*_iHeight*3+sizeof(tagBITMAPINFOHEADER)+sizeof(fileHeader);
	fileHeader.bfReserved1	=	0;
	fileHeader.bfReserved2	=	0;

	tagBITMAPINFOHEADER	bitMapInfo;

	bitMapInfo.biSize			=	40;
    bitMapInfo.biClrUsed		=	0;
    bitMapInfo.biBitCount		=	24;
	bitMapInfo.biWidth			=	_iWidth;
	bitMapInfo.biHeight			=	_iHeight;
	bitMapInfo.biPlanes			=	1;
	bitMapInfo.biCompression	=	0;
	bitMapInfo.biSizeImage		=	0;
	bitMapInfo.biXPelsPerMeter	=	2834;
	bitMapInfo.biYPelsPerMeter	=	2834;
	bitMapInfo.biClrImportant	=	0;

	fwrite(&fileHeader,sizeof(fileHeader),1,fp);
	fwrite(&bitMapInfo,sizeof(bitMapInfo),1,fp);

	fwrite(_lpData,_iWidth*_iHeight*3,1,fp);

	fclose(fp);

	return	TRUE;
}