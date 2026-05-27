#ifndef	_classTILE_H
#define	_classTILE_H

#include "cNEMO.H"

#define	dTILE_XS				64
#define	dTILE_XS_H				32
#define	dTILE_BYTE_XS			128
#define	dTILE_BYTE_XS_H			64
#define	dTILE_YS				32
#define	dTILE_YS_H				16
#define	dTILE_SIZE_SHIFT		11
#define	dTILE_XSIZE_SHIFT		6
#define	dTILE_XSIZE_SHIFT_H		5
#define	dTILE_YSIZE_SHIFT		5
#define	dTILE_YSIZE_SHIFT_H		4

#define	dTILE_SIZE_SHIFT		11
#define	dTILE_SIZE_SHIFT_H		9


/*
#define	dTILE_XS				64
#define	dTILE_BYTE_XS			128
#define	dTILE_YS				32
#define	dTILE_SIZE_SHIFT		11
#define	dTILE_XSIZE_SHIFT		6
#define	dTILE_YSIZE_SHIFT		5
*/

const	DWORD	dTILE_HALF_XS	=	dTILE_XS/2;
const	DWORD	dTILE_HALF_YS	=	dTILE_YS/2;

const	DWORD	dTILE_SIZE		=	dTILE_XS*dTILE_YS;

class	cTILE_INFO
{
public:
	char	m_strHeader[40];
	WORD	m_wCount,m_wWidth,m_wHeight;

};

class cTILE	: public	cTILE_INFO
{
public:
	int					m_iTileSet;
	BOOL				m_isMcu;
	WORD				*m_pData;				//	16비트 타일
	BOOL				m_bIsHalfSize;

						cTILE();
						~cTILE();

	void				close();

	BOOL				load(int tileset,BOOL _bIsHalfSize,cNUX *nux=NULL,cMCU *mcu=NULL);

	void	inline		put16(int index,WORD *lpDestScr)
						{
							WORD	*lpScr	=	lpDestScr;
							WORD	*lpTile	=	m_pData	+	(index <<dTILE_SIZE_SHIFT);

							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);lpTile	+=	dTILE_XS;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS);

						}

	void	inline		put16Clip(int x,int y,int index,WORD *lpDestScr)
						{
							WORD	*lpTile	=	m_pData	+	(index <<dTILE_SIZE_SHIFT);

							int		offset=0,addr=0,i;
							int		xs	=	dTILE_XS,ys	=	dTILE_YS;

							if (x			< 0					) {offset+=-x			;xs+=x;x=0;}
							if (y			< 0					) {offset+=-y*dTILE_XS	;ys+=y;y=0;}
							if (x+dTILE_XS	> cDRAW::WIDTH		) xs	=	cDRAW::WIDTH		-x;
							if (y+dTILE_YS	> g_iScreenHeight	) ys	=	g_iScreenHeight	-y;

							if (xs <= 0 || ys <= 0) return;

							addr	=	x+y*cDRAW::WIDTH;
							xs		*=	2;

							for (i=0;i<ys;i++,addr+=cDRAW::WIDTH,offset+=dTILE_XS)
								memcpy(lpDestScr+addr,lpTile+offset,xs);
						}

	void	inline		put16NW(int dx,int dy,int index,WORD *lpDestScr)
						{
							WORD	*lpTile	=	m_pData	+	(index <<dTILE_SIZE_SHIFT)	+	dx + (dy<<dTILE_XSIZE_SHIFT);

							dx		=	(dTILE_XS	-	dx)*2;
							dy		=	dTILE_YS	-	dy;

							Label_put16NW:
								memcpy(lpDestScr,lpTile,dx);

								dy--;
								if (dy	<=	0) return;

								lpDestScr	+=	cDRAW::WIDTH;
								lpTile		+=	dTILE_XS;
							goto	Label_put16NW;
						}

	void	inline		put16N(int dy,int index,WORD *lpDestScr)
						{
							WORD	*lpTile	=	m_pData	+	(index <<dTILE_SIZE_SHIFT)	+	(dy<<dTILE_XSIZE_SHIFT);

							dy		=	dTILE_YS	-	dy;

							Label_put16N:
								memcpy(lpDestScr,lpTile,dTILE_BYTE_XS);

								dy--;
								if (dy	<=	0) return;

								lpDestScr	+=	cDRAW::WIDTH;
								lpTile		+=	dTILE_XS;
							goto	Label_put16N;
						}

	void	inline		put16NE(int dx,int dy,int index,WORD *lpDestScr)
						{
							WORD	*lpTile	=	m_pData	+	(index <<dTILE_SIZE_SHIFT)	+	(dy<<dTILE_XSIZE_SHIFT);

							dx		*=	2;
							dy		=	dTILE_YS	-	dy;

							Label_put16NE:
								memcpy(lpDestScr,lpTile,dx);

								dy--;
								if (dy	<=	0) return;

								lpDestScr	+=	cDRAW::WIDTH;
								lpTile		+=	dTILE_XS;
							goto	Label_put16NE;
						}

	void	inline		put16W(int dx,int index,WORD *lpDestScr)
						{
							WORD	*lpTile	=	m_pData	+	(index <<dTILE_SIZE_SHIFT)	+	dx;

							dx		=	(dTILE_XS	-	dx)*2;

							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);
						}

	void	inline		put16E(int dx,int index,WORD *lpDestScr)
						{
							WORD	*lpTile	=	m_pData	+	(index <<dTILE_SIZE_SHIFT);

							dx		*=	2;

							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS;
							memcpy(lpDestScr,lpTile,dx);
						}


	void	inline		put16SW(int dx,int sy,int index,WORD *lpDestScr)
						{
							WORD	*lpTile	=	m_pData	+	(index <<dTILE_SIZE_SHIFT) + dx;

							dx		=	(dTILE_XS	-	dx)*2;

							Label_put16SW:
								memcpy(lpDestScr,lpTile,dx);
								sy	--;
								if (sy	<=	0) return;

								lpDestScr	+=	cDRAW::WIDTH;
								lpTile		+=	dTILE_XS;
							goto Label_put16SW;
						}

	void	inline		put16S(int sy,int index,WORD *lpDestScr)
						{
							WORD	*lpTile	=	m_pData	+	(index <<dTILE_SIZE_SHIFT);

							Label_put16S:

								memcpy(lpDestScr,lpTile,dTILE_BYTE_XS);
								sy	--;
								if (sy	<=	0) return;

								lpDestScr	+=	cDRAW::WIDTH;
								lpTile		+=	dTILE_XS;

							goto Label_put16S;
						}

	void	inline		put16SE(int sx,int sy,int index,WORD *lpDestScr)
						{
							WORD	*lpTile	=	m_pData	+	(index <<dTILE_SIZE_SHIFT);

							sx	*=	2;

							Label_put16SE:

								memcpy(lpDestScr,lpTile,sx);
								sy	--;
								if (sy	<=	0) return;

								lpDestScr	+=	cDRAW::WIDTH;
								lpTile		+=	dTILE_XS;

							goto Label_put16SE;
						}

	void	inline		put16h(int index,WORD *lpDestScr)
						{
							WORD	*lpScr	=	lpDestScr;
							WORD	*lpTile	=	m_pData	+	(index <<dTILE_SIZE_SHIFT_H);

							memcpy(lpScr,lpTile	,dTILE_BYTE_XS_H);lpTile	+=	dTILE_XS_H;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS_H);lpTile	+=	dTILE_XS_H;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS_H);lpTile	+=	dTILE_XS_H;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS_H);lpTile	+=	dTILE_XS_H;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS_H);lpTile	+=	dTILE_XS_H;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS_H);lpTile	+=	dTILE_XS_H;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS_H);lpTile	+=	dTILE_XS_H;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS_H);lpTile	+=	dTILE_XS_H;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS_H);lpTile	+=	dTILE_XS_H;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS_H);lpTile	+=	dTILE_XS_H;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS_H);lpTile	+=	dTILE_XS_H;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS_H);lpTile	+=	dTILE_XS_H;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS_H);lpTile	+=	dTILE_XS_H;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS_H);lpTile	+=	dTILE_XS_H;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS_H);lpTile	+=	dTILE_XS_H;lpScr	+=	cDRAW::WIDTH;
							memcpy(lpScr,lpTile	,dTILE_BYTE_XS_H);

						}

	void	inline		put16Cliph(int x,int y,int index,WORD *lpDestScr)
						{
							WORD	*lpTile	=	m_pData	+	(index <<dTILE_SIZE_SHIFT_H);

							int		offset=0,addr=0,i;
							int		xs	=	dTILE_XS_H,ys	=	dTILE_YS_H;

							if (x			< 0					)
							{
								offset+=-x			;
								xs+=x;
								x=0;
							}
							if (y			< 0					)
							{
								offset+=-y*dTILE_XS_H;
								ys+=y;
								y=0;
							}
							if (x+dTILE_XS_H> cDRAW::WIDTH		)
								xs	=	cDRAW::WIDTH		-x;
							if (y+dTILE_YS_H> g_iScreenHeight	)
								ys	=	g_iScreenHeight	-y;

							if (xs <= 0 || ys <= 0) return;

							addr	=	x+y*cDRAW::WIDTH;
							xs		*=	2;

							for (i=0;i<ys;i++,addr+=cDRAW::WIDTH,offset+=dTILE_XS_H)
								memcpy(lpDestScr+addr,lpTile+offset,xs);
						}

	void	inline		put16NWh(int dx,int dy,int index,WORD *lpDestScr)
						{
							WORD	*lpTile	=	m_pData	+	(index <<dTILE_SIZE_SHIFT_H)	+	dx + (dy<<dTILE_XSIZE_SHIFT_H);

							dx		=	(dTILE_XS_H	-	dx)*2;
							dy		=	dTILE_YS_H	-	dy;

							Label_put16NW:
								memcpy(lpDestScr,lpTile,dx);

								dy--;
								if (dy	<=	0) return;

								lpDestScr	+=	cDRAW::WIDTH;
								lpTile		+=	dTILE_XS_H;
							goto	Label_put16NW;
						}

	void	inline		put16Nh(int dy,int index,WORD *lpDestScr)
						{
							WORD	*lpTile	=	m_pData	+	(index <<dTILE_SIZE_SHIFT_H)	+	(dy<<dTILE_XSIZE_SHIFT_H);

							dy		=	dTILE_YS_H	-	dy;

							Label_put16N:
								memcpy(lpDestScr,lpTile,dTILE_BYTE_XS_H);

								dy--;
								if (dy	<=	0) return;

								lpDestScr	+=	cDRAW::WIDTH;
								lpTile		+=	dTILE_XS_H;
							goto	Label_put16N;
						}

	void	inline		put16NEh(int dx,int dy,int index,WORD *lpDestScr)
						{
							WORD	*lpTile	=	m_pData	+	(index <<dTILE_SIZE_SHIFT_H)	+	(dy<<dTILE_XSIZE_SHIFT_H);

							dx		*=	2;
							dy		=	dTILE_YS_H	-	dy;

							Label_put16NE:
								memcpy(lpDestScr,lpTile,dx);

								dy--;
								if (dy	<=	0) return;

								lpDestScr	+=	cDRAW::WIDTH;
								lpTile		+=	dTILE_XS_H;
							goto	Label_put16NE;
						}

	void	inline		put16Wh(int dx,int index,WORD *lpDestScr)
						{
							WORD	*lpTile	=	m_pData	+	(index <<dTILE_SIZE_SHIFT_H)	+	dx;

							dx		=	(dTILE_XS_H	-	dx)*2;

							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);
						}

	void	inline		put16Eh(int dx,int index,WORD *lpDestScr)
						{
							WORD	*lpTile	=	m_pData	+	(index <<dTILE_SIZE_SHIFT_H);

							dx		*=	2;

							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);lpDestScr	+=	cDRAW::WIDTH;lpTile		+=	dTILE_XS_H;
							memcpy(lpDestScr,lpTile,dx);
						}


	void	inline		put16SWh(int dx,int sy,int index,WORD *lpDestScr)
						{
							WORD	*lpTile	=	m_pData	+	(index <<dTILE_SIZE_SHIFT_H) + dx;

							dx		=	(dTILE_XS_H	-	dx)*2;

							Label_put16SW:
								memcpy(lpDestScr,lpTile,dx);
								sy	--;
								if (sy	<=	0) return;

								lpDestScr	+=	cDRAW::WIDTH;
								lpTile		+=	dTILE_XS_H;
							goto Label_put16SW;
						}

	void	inline		put16Sh(int sy,int index,WORD *lpDestScr)
						{
							WORD	*lpTile	=	m_pData	+	(index <<dTILE_SIZE_SHIFT_H);

							Label_put16S:

								memcpy(lpDestScr,lpTile,dTILE_BYTE_XS_H);
								sy	--;
								if (sy	<=	0) return;

								lpDestScr	+=	cDRAW::WIDTH;
								lpTile		+=	dTILE_XS_H;

							goto Label_put16S;
						}

	void	inline		put16SEh(int sx,int sy,int index,WORD *lpDestScr)
						{
							WORD	*lpTile	=	m_pData	+	(index <<dTILE_SIZE_SHIFT_H);

							sx	*=	2;

							Label_put16SE:

								memcpy(lpDestScr,lpTile,sx);
								sy	--;
								if (sy	<=	0) return;

								lpDestScr	+=	cDRAW::WIDTH;
								lpTile		+=	dTILE_XS_H;

							goto Label_put16SE;
						}

};

extern	char	*g_strTileName[];

#endif