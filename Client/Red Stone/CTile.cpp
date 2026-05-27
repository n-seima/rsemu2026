#include "cTILE.H"
#include "CGame.H"

char	*g_strTileName[]	=
{
	"Grassland"			,
	"Mountains"			,
	"Desert"			,
	"Savana"			,
	"Cave"				,
	"Dungeon"			,
	"Tower"				,
	"Mine"				,
	"Hell"				,
	"Heaven"			,
	"Brunenstig"		,
	"Bigaple"			,
	"Augusta"			,
	"Bridgehead"		,
	"Mountains Village"	,
	"Arian"				,
	"Ruined City"		,
	"FarmHouse"			,
	"Gypsy"				,
	"Room"				,
	"GuildHall"			,
	"end"				,
};


cTILE::cTILE()
{
	m_pData		=	NULL;		//	16비트 타일
	m_wCount	=	0;
	m_wWidth	=	0;
	m_wHeight	=	0;
	m_isMcu		=	FALSE;
	m_iTileSet	=	0xffff;
}

cTILE::~cTILE()
{	close();
}

//
//
void
cTILE::close()
{
	if (!m_isMcu)
		_Free(m_pData);

	m_pData		=	NULL;		//	16비트 타일
	m_isMcu		=	FALSE;
	m_iTileSet	=	0xffff;
}	//	cTILE::close()

//
//	타일셋에 맞춰 타일을 로딩한다.
BOOL
cTILE::load(int tileSet,BOOL _bIsHalfSize,cNUX *_lpNux,cMCU *_lpMcu)
{
	cFILE	*file;

	if (_lpNux)
		file	=	_lpNux->Open(_ms("data/mapset/%s/tile.mpr",g_strTileName[tileSet]));
	else
	{	
		file	=	new cFILE();
		if (!file->Open(_ms("data/mapset/%s/tile.mpr",g_strTileName[tileSet]),"rb"))	KILL(file);
	}

	if (!file)
		return ERRMSG("Error in cTILE::Load - '%s' file not found","data/mapset/%s/tile.mpr",g_strTileName[tileSet]);

	close();

	if (_lpMcu)
		m_isMcu	=	TRUE;
	else
		m_isMcu	=	FALSE;

	m_iTileSet	=	tileSet;

	file->Read(this,sizeof(cTILE_INFO));		//	타일 정보

	m_pData		=	_New(WORD,m_wCount*m_wWidth*m_wHeight,_lpMcu,"cTILE::m_pData");

	file->Read(m_pData,m_wCount*m_wWidth*m_wHeight*2);

	if (g_iPixelFormat	==	dRGB565)
		for (int i=0;i<m_wCount*m_wWidth*m_wHeight;i++)	
			m_pData[i]	=	ToRGB565(m_pData[i]);
	if (g_iPixelFormat	==	dBGR565)
		for (int i=0;i<m_wCount*m_wWidth*m_wHeight;i++)	
			m_pData[i]	=	ToBGR565(m_pData[i]);

	if (_bIsHalfSize)
	{
		int	iPoint	=	0;

		for (int i=0;i<m_wCount;i++)
		{
			int	iAddress	=	i*m_wHeight*m_wWidth;

			for (int y=0;y<m_wHeight;y+=2)
			{
				for (int x=0;x<m_wWidth;x+=2)
				{
					m_pData[iPoint]	=	GetMixColor(m_pData[iAddress+x+y*m_wWidth],m_pData[iAddress+x+y*m_wWidth+1]);
					iPoint++;
				}
			}
		}
	}

	KILL(file);

	return TRUE;
}