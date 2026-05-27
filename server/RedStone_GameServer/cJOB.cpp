#include	"cJOB.H"
#include	"cGAME.H"
#include	"cDECODER.H"
#include	"debugCode.H"

cJOB				g_aJob[dMAX_JOB_COUNT];

//
//	최초의 초기화
BOOL
cJOB::Init()
{
	if (!LoadDefaultEquipmentData())
		return	FALSE;

	return	TRUE;
}	//	cJOB::Init()

enum
{
	eDEFAULT_DATA	=	101,
	eDATA_WEAPON	=	102,
	eDATA_SHIELD	=	103,
	eDATA_GLOVE		=	104,
	eDATA_BULLET	=	105,
	eDATA_BELT		=	106,
	eDATA_BASIC_SKILL		,
	eDATA_SLOT0ITEM		,
};

//
//	캐릭터 생성시 부터 착용하고 있는 아이템 로딩
BOOL
cJOB::LoadDefaultEquipmentData()
{
	cFILE	file;

#ifdef	_FOR_KOREA
	char	*strFileName	=	"data/jobData.dat";
#elif	_FOR_JAPAN
	char	*strFileName	=	"data/jobDataForJapan.dat";
#elif	_FOR_USA
	char	*strFileName	=	"data/jobDataForUsa.dat";
#elif	_FOR_CHINA
	char	*strFileName	=	"data/jobDataForChina.dat";
#elif	_FOR_INTER
	char	*strFileName	=	"data/jobDataForInter.dat";
#elif	_FOR_THAI
	char	*strFileName	=	"data/jobDataForThai.dat";
#endif

	if (file.Open(strFileName,"rb"))
	{
		int	iFileSize	=	file.Length;

		file.Read(g_aPlayer1LvSetting,sizeof(g_aPlayer1LvSetting));
		file.Close();

		for (int i=0;i<dPLAYER_JOB_COUNT;i++)
		{
			if	(i*sizeof(cDEFAULT_JOB_DATA) >= iFileSize)
			{
				g_aPlayer1LvSetting[i].m_wStatus	=	0;
			}
			else
			if	(g_aPlayer1LvSetting[i].m_wStatus	==	2)
			{
				if	(g_bIsTestServer)
					g_aPlayer1LvSetting[i].m_wStatus	=	1;
				else
					g_aPlayer1LvSetting[i].m_wStatus	=	0;
			}
		}

		return	TRUE;
	}

	return	ERRMSG("error!!","can not find '%s' file",strFileName);
}

//
//	제거
void
cJOB::Close()
{
}	//	cJOB::Close()