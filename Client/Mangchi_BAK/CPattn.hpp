#ifndef CPATTN_H
#define CPATTN_H

#define dPATTN_YSIZE			32
#define dPATTN_XSIZE			64
#define dPATTN_IMAGE_PITCH		128
#define dPATTN_IMAGE_SIZE		4096

#define dPATTN_NORMAL			0
#define dPATTN_UNDER_BORDER		1
#define dPATTN_CLIFF_BORDER		2

#define dMaskLeft	1
#define dMaskLT		2
#define dMaskTop	4
#define dMaskRT		8
#define dMaskRight	16
#define dMaskRB		32
#define dMaskBottom	64
#define dMaskLB		128

#define dLandLT 0
#define dLandRT 1
#define dLandLB 2
#define dLandRB 3

#define MPRHEADER "MPR File Version0.99999999999999991"

extern BYTE		m_bFlatPattn;

typedef struct Pattn	{
	union	{
		struct	{		BYTE	bLandLT, bLandRT, bLandLB, bLandRB;		};
		DWORD	dwLand;
	};
	LPBYTE	lpImage;
}*LPPattn;

typedef class CPattn{
public:
	CPattn();
	~CPattn();

//	Header...
	char		m_bpNameStr[82];
	BYTE		m_bLowFlag1, m_bHighFlag1;
	RECT		m_range;
	BYTE		m_bForm, m_bHeight,	m_bFlag[2];

//***********************	Main Program에서 사용할	것...	*************************//
public:
	WORD		m_iImageHeight;
	WORD		m_iPattnCount;				//패턴의 갯수...

	LPBYTE		m_bpImage;
	LPPattn		m_cpPattn;					//인덱스에 대응되는 패턴들....

	BOOL		LoadPattn(char *filename);
//***********************************************************************************//

public:
	BOOL		InsertImage(BYTE* bitmap, int imageheight);

	BOOL		SavePattn(HANDLE &fp);
	BOOL		LoadPattn(HANDLE &fp);

	int			GetRandom(BYTE LT, BYTE RT, BYTE LB, BYTE RB);
	int			GetLast(BYTE LT, BYTE RT, BYTE LB, BYTE RB, int offset=0);

	BOOL		IsBorder()	{	return (m_bFlag[0]!=m_bFlag[1]);							}
	BOOL		IsUnder()	{	return (m_bHeight==0)&&(IsBorder())&&(m_range.bottom>0);	}

	void		BuildFlag();

	CPattn*		GetCPattn()	{	return this;												}

	BOOL		SavePattn(HANDLE &fp, int ImageOffset);

	void		DeletePattn(int index);

	void		SetRandom();

	BYTE		CullCliff(BYTE flag);

protected:
	BYTE		PattnCount[16], PattnIndex[16][256];
	BYTE		*ReadBuffer;	
}*LPCPattn;

#endif