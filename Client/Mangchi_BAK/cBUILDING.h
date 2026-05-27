#ifndef _classBUILDING_H
#define _classBUILDING_H

#include "SFC.H"
#include "cIMAGE.H"
#include "cOBJECT.H"

#define	dMAX_ADDON_IMAGE_COUNT				20
#define	dMAX_ADDON_OBJECT_COUNT_ON_BUILDING	4
#define	dMAX_BUILDING_COUNT_IN_MAP			255

class	cSAVE_BUILDING_INFO
{
public:
	WORD			m_wSerial;
	cPOINT			m_pos;
	WORD			m_wBuilding;
	WORD			m_isPutShadow;
	WORD			m_wAddonBlockCount,m_wAddonObjectCount;	//	덧붙는 이미지의 수
	WORD			m_aAddonBlock[dMAX_ADDON_IMAGE_COUNT];
	cADD_ON_OBJECT	m_aAddonObject[dMAX_ADDON_OBJECT_COUNT_ON_BUILDING];
	BOOL			isSame(cSAVE_BUILDING_INFO* _lpSaveBuildInfo)
	{
		if(m_wSerial != _lpSaveBuildInfo->m_wSerial)
			return FALSE;

		if(memcmp(&m_pos, &_lpSaveBuildInfo->m_pos,sizeof(cPOINT)) != 0)
			return FALSE;

		if(m_wBuilding != _lpSaveBuildInfo->m_wBuilding)
			return FALSE;

		if(m_isPutShadow != _lpSaveBuildInfo->m_isPutShadow)
			return FALSE;

		if(m_wAddonBlockCount != _lpSaveBuildInfo->m_wAddonBlockCount)
			return FALSE;

		if(m_wAddonObjectCount != m_wAddonObjectCount)
			return FALSE;

		if(memcmp(m_aAddonBlock,m_aAddonBlock, sizeof(WORD) * dMAX_ADDON_IMAGE_COUNT) != 0)
			return FALSE;

		for(int i=0;i<dMAX_ADDON_OBJECT_COUNT_ON_BUILDING;++i)
		{
			if(!m_aAddonObject[i].isSame(&_lpSaveBuildInfo->m_aAddonObject[i]))
				return FALSE;
		}

		return TRUE;
	}
};

class	cBUILDING_INFO	:	public cSAVE_BUILDING_INFO
{
public:
	BOOL			m_isSelected;

					cBUILDING_INFO()	{	reset();}

	inline	void	copy(cBUILDING_INFO *_lpObject)
	{
		memcpy(this,_lpObject,sizeof(cBUILDING_INFO));
	}
	void			reset();
	BOOL			isFocus(int cx,int cy,BYTE *_lpCrash);
	BOOL			isCheckedBlock(int _iIndex);
	int				getBlockIndex(int _iIndex);
	void			toggleBlock(int _iBlock);
	void			toggleShadow(){m_isPutShadow=1-m_isPutShadow;}
	void			sortAddonBuildingBlock();
	
	BOOL			addAddonObject(int _iObject,int _iX,int _iY);
	void			removeAddonObject(int _iIndex);
	void			moveAddonObject(int _iIndex,int _iDx,int _iDy);

	static	int _cdecl	Compare( const void *a, const void *b);

};	//	class	cBUILDING_INFO


class	cBUILDING
{
public:
	int						m_iDataCount,m_iSelectAddonObject;
	cOBJ_ANM				*m_pData;
	cDEN<cBUILDING_INFO>	m_den;

	
	inline	cBUILDING_INFO	*get(int _iIndex){return m_den.get(_iIndex);}
	inline	BYTE*			getCrashInfo(int _iIndex)	{return	m_pData[_iIndex].pCRASH;}
	inline	BYTE*			getBlankInfo(int _iIndex)	{return	m_pData[_iIndex].pBLOCK;}
	inline	BYTE*			getOverlapInfo(int _iIndex)	{return	m_pData[_iIndex].pOVERLAPBLOCK;}

	inline	int				getMaxCount(){return	m_den.getMaxCount();}// 최대 카운트
	inline	int				getCount(){return	m_den.getCount();}// 카운트

							cBUILDING();
							~cBUILDING();

	void					close();
	BOOL					init(int _iTileSet,cNUX *_lpNux=NULL);						//	초기화
	void					reset();									//	데이터 리셋
	int						add(int _iX,int _iY,int _iBuilding);//	건물 추가
	int						getFocusBuilding(int _iMx,int _iMy);

	cOBJ_ANM*				getBuildingImage(int _iIndex){return	&m_pData[_iIndex];}

	void					putRegisteredBuilding(int _iIndex,int _iX,int _iY,cOBJ_ANM *_lpObjectImage);
	void					putRegisteredBuildingScale(int _iIndex,int _iX,int _iY,cOBJ_ANM *_lpObjectImage,int _iScale);

	void					put(int _iBuilding,int _iIndex,int _iX,int _iY,int _iXs,int _iYs);	//	찍어주기(사이즈에 맞춤)
	void					put(int index,int x,int y,int xs,int ys);	//	찍어주기(사이즈에 맞춤)
	void					put(int index,int x,int y,BOOL red=FALSE);	//	찍기
	void					putLayer(int index,int x,int y);//	레이어 찍기
	void					select(cRECT *rect);//	rect안에 건물 선택


	void					freeSelectedAll();
	void					removeSelectedAll();//	선택된 건물들 제거
	BOOL					remove(int _iIndex){return	m_den.remove(_iIndex);}//_iIndex번째 건물 제거
};	//	class	cBUILDING_INFO


#endif