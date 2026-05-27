#include "stdafx.h"
#include "cpattn.hpp"
#include <mmsystem.h>

BYTE *ReadBuffer;

BYTE m_bFlatPattn = true;

BYTE CPattn::CullCliff(BYTE flag)
{
	return (m_bFlatPattn) ? flag : flag&0x0f;
}

CPattn::CPattn()
{
	memset(this, 0, sizeof(CPattn));
}

CPattn::~CPattn()
{
	if(ReadBuffer)
		delete []ReadBuffer;
	else
	{
		if(m_cpPattn)		free(m_cpPattn);
		if(m_bpImage)		free(m_bpImage);
	}
	memset(this, 0, sizeof(CPattn));
}

void CPattn::BuildFlag()
{
	m_bFlag[1] = m_bFlag[0] = (m_iPattnCount) ? m_cpPattn[0].bLandLT : 0;

	for(int i=0; i<m_iPattnCount; i++)
	{
		if(m_bFlag[0]!=m_cpPattn[i].bLandLT)	{
			m_bFlag[1] = m_cpPattn[i].bLandLT;
			break;
		}
		if(m_bFlag[0]!=m_cpPattn[i].bLandRT)	{
			m_bFlag[1] = m_cpPattn[i].bLandRT;
			break;
		}
		if(m_bFlag[0]!=m_cpPattn[i].bLandLB)	{
			m_bFlag[1] = m_cpPattn[i].bLandLB;
			break;
		}
		if(m_bFlag[0]!=m_cpPattn[i].bLandRB)	{
			m_bFlag[1] = m_cpPattn[i].bLandRB;
			break;
		}
	}

	if(m_bFlag[1]<m_bFlag[0])
	{
		BYTE flagtemp = m_bFlag[0];
		m_bFlag[0] = m_bFlag[1];
		m_bFlag[1] = flagtemp;
	}
}

BOOL CPattn::InsertImage(BYTE* bitmap, int imageheight)
{
	int AddPattnCount	= imageheight / dPATTN_YSIZE;
	int OldImageSize	= m_iPattnCount * dPATTN_IMAGE_SIZE;
	int NewImageSize	= OldImageSize + dPATTN_IMAGE_PITCH * imageheight;
	int NewPattnSize	= sizeof(Pattn) * m_iPattnCount + sizeof(Pattn)*AddPattnCount;

	m_bpImage = (LPBYTE) ( (!m_bpImage) ? malloc(NewImageSize) : realloc(m_bpImage, NewImageSize) );
	m_cpPattn = (LPPattn) ( (!m_cpPattn) ? malloc(NewPattnSize) : realloc(m_cpPattn, NewPattnSize) );

	if( (!m_bpImage) || (!m_cpPattn) )	return FALSE;

	memcpy(m_bpImage+OldImageSize, bitmap, dPATTN_IMAGE_PITCH*imageheight);
	memset(m_cpPattn+m_iPattnCount, 0, sizeof(Pattn)*AddPattnCount);

	m_iPattnCount	= m_iPattnCount + AddPattnCount;
	m_iImageHeight	= m_iPattnCount * dPATTN_YSIZE;

	m_bFlag[0] = m_cpPattn[0].bLandLT;

	BYTE *ImageTemp	= m_bpImage;
	for(int i=0; i<m_iPattnCount; i++, ImageTemp+=dPATTN_IMAGE_SIZE)
		m_cpPattn[i].lpImage	= ImageTemp;

	return TRUE;
}

BOOL CPattn::SavePattn(HANDLE &fp)
{
	DWORD aWrite;
	WriteFile(fp, this, sizeof(CPattn), &aWrite, NULL);
	WriteFile(fp, m_cpPattn, sizeof(Pattn)*m_iPattnCount, &aWrite, NULL);
	WriteFile(fp, m_bpImage, m_iImageHeight*dPATTN_IMAGE_PITCH, &aWrite, NULL);
	return TRUE;
}

BOOL CPattn::LoadPattn(HANDLE &fp)
{
	DWORD aRead;
	ReadFile(fp, this, sizeof(CPattn), &aRead, NULL);

	LPBYTE	wpStartImage	= m_bpImage;

	m_cpPattn	= (LPPattn)malloc(sizeof(Pattn)*m_iPattnCount);
	m_bpImage	= (LPBYTE)malloc(dPATTN_IMAGE_PITCH*m_iImageHeight);

	ReadFile(fp, m_cpPattn, sizeof(Pattn)*m_iPattnCount, &aRead, NULL);
	ReadFile(fp, m_bpImage, m_iImageHeight*dPATTN_IMAGE_PITCH, &aRead, NULL);

	for(int i=0; i<m_iPattnCount; i++)
		m_cpPattn[i].lpImage	= m_bpImage + (m_cpPattn[i].lpImage-wpStartImage);

	return TRUE;
}

BOOL CPattn::LoadPattn(char *filename)
{
	CPattn::~CPattn();

	HANDLE fp = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	BOOL isSuccess = (fp!=INVALID_HANDLE_VALUE);
	DWORD aRead;

	char buf[40];
	if(isSuccess)
		isSuccess &= ( ReadFile(fp, buf, 40, &aRead, NULL) !=0 );

	if(isSuccess)
		isSuccess &= ( ReadFile(fp, &m_iPattnCount, 2, &aRead, NULL) !=0 );

	if(isSuccess)
		isSuccess &= ( ReadFile(fp, buf, 4, &aRead, NULL) !=0 );

	DWORD readsize = m_iPattnCount*dPATTN_IMAGE_SIZE;
	DWORD datasize = readsize + m_iPattnCount*sizeof(Pattn);

	if(isSuccess)
		ReadBuffer = new BYTE[datasize];

	if(ReadBuffer==NULL)
		isSuccess = false;

	if(isSuccess)
		isSuccess &= ( ReadFile(fp, ReadBuffer, readsize, &aRead, NULL) !=0 );

	CloseHandle(fp);

	if(!isSuccess)
		return FALSE;

	LPBYTE bpImage = m_bpImage	= ReadBuffer;

	for(int i=0; i<m_iPattnCount; i++, bpImage+dPATTN_IMAGE_SIZE)
		m_cpPattn[i].lpImage = bpImage;

	m_cpPattn	= (LPPattn)(m_bpImage+m_iPattnCount*dPATTN_IMAGE_SIZE);

	return TRUE;
}

BOOL CPattn::SavePattn(HANDLE &fp, int ImageOffset)
{
	Pattn sample;
	DWORD aWrite;

	BOOL is = TRUE;

	for(int i=0; i<m_iPattnCount; i++)
	{
		sample				= m_cpPattn[i];
		sample.lpImage		= m_cpPattn[i].lpImage-(int)m_bpImage+ImageOffset;
		is &= WriteFile(fp, &sample, sizeof(Pattn), &aWrite, NULL);
	}
	return is;
}

void CPattn::SetRandom()
{	
	memset(PattnCount, 0, 16);
	memset(PattnIndex, 0, 16*256);

	int n;

	LPPattn lpPattn;

	for(int i=0; i<m_iPattnCount; i++)
	{
		lpPattn = m_cpPattn+i;
		n = (lpPattn->bLandLT==m_bFlag[1]) | ((lpPattn->bLandRT==m_bFlag[1])<<1) |
			((lpPattn->bLandLB==m_bFlag[1])<<2) | ((lpPattn->bLandRB==m_bFlag[1])<<3);

		PattnIndex[n][ PattnCount[n] ] = i;

		PattnCount[n]++;
	}
}

int	CPattn::GetRandom(BYTE LT, BYTE RT, BYTE LB, BYTE RB)
{
	if(m_bForm!=dPATTN_CLIFF_BORDER)
	{
		LT = CullCliff(LT);	RT = CullCliff(RT);	LB = CullCliff(LB);	RB = CullCliff(RB);
	}
	else
	{
		BYTE min = LT;
		if(RT<min)	min = RT;
		if(LB<min)	min = LB;
		if(RB<min)	min = RB;
		min>>=4;

		LT = ( LT&0x0f ) | ( ( (LT>>4)-min) <<4 );
		RT = ( RT&0x0f ) | ( ( (RT>>4)-min) <<4 );
		LB = ( LB&0x0f ) | ( ( (LB>>4)-min) <<4 );
		RB = ( RB&0x0f ) | ( ( (RB>>4)-min) <<4 );
	}

	int ind = (LT==m_bFlag[1]) | ((RT==m_bFlag[1])<<1) | ((LB==m_bFlag[1])<<2) | ((RB==m_bFlag[1])<<3);
	int randommax	= PattnCount[ind];
	BYTE *temp		= PattnIndex[ind];

	int randindex = (randommax>0) ? ((timeGetTime()+rand())%randommax) : 0;

	if(m_bForm!=dPATTN_NORMAL)
	{
		if( (LT!=RT) && (LT==LB) && (RT==RB) )
		{
			randommax -= (m_range.left==3) ? 3 : 1;
			randindex = (randommax>0) ? ((timeGetTime()+rand())%randommax) : 0;
			randindex = randindex/m_range.left*m_range.left;
		}

		if( (LB==RB) && (LT!=RT) && (LB==m_bLowFlag1))
		{
			randommax -= 3;
			randindex = (randommax>0) ? rand()%randommax : 0;
		}
	}

	return temp[randindex];
}

int CPattn::GetLast(BYTE LT, BYTE RT, BYTE LB, BYTE RB, int offset/*=0*/)
{
	if(m_bForm!=dPATTN_CLIFF_BORDER)
	{
		LT = CullCliff(LT);	RT = CullCliff(RT);	LB = CullCliff(LB);	RB = CullCliff(RB);
	}

	int randommax = 0;
	static BYTE temp[256];

	for(int i=0; i<m_iPattnCount; i++)
	{
		if((m_cpPattn[i].bLandLT!=LT)||(m_cpPattn[i].bLandRT!=RT)||(m_cpPattn[i].bLandLB!=LB)||(m_cpPattn[i].bLandRB!=RB))
			continue;

		temp[randommax] = i;
		randommax++;
	}

	return temp[randommax-1+offset];
}

void CPattn::DeletePattn(int index)
{
	if(index>=m_iPattnCount)	return;

	LPBYTE newImage = (LPBYTE)malloc(dPATTN_IMAGE_SIZE*(m_iPattnCount-1));
	LPPattn newPattn = (LPPattn)malloc(sizeof(Pattn)*(m_iPattnCount-1));

	int front = index*dPATTN_IMAGE_SIZE;
	memcpy(newImage, m_bpImage, front);
	memcpy(newImage+front, m_bpImage+front+dPATTN_IMAGE_SIZE, (m_iPattnCount-index-1)*dPATTN_IMAGE_SIZE);

	front = sizeof(Pattn)*index;
	memcpy(newPattn, m_cpPattn, front);
	memcpy(newPattn+index, m_cpPattn+index+1, (m_iPattnCount-index-1)*sizeof(Pattn));

	m_iPattnCount--;
	LPBYTE ImageTemp = newImage;
	for(int i=0; i<m_iPattnCount; i++, ImageTemp+=dPATTN_IMAGE_SIZE)
		newPattn[i].lpImage		= ImageTemp;

	if(m_cpPattn)		free(m_cpPattn);
	if(m_bpImage)		free(m_bpImage);

	m_cpPattn = newPattn;
	m_bpImage = newImage;
	m_iImageHeight = m_iPattnCount*dPATTN_YSIZE;
}