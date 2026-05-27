#include "CGuild.h"
#include "CGuildSkill.h"
#include "CMessage.h"
#include "CTitle.h"
#include "CHero.h"

CGuildMarkPackManager	g_guildMarkPackManager;

BOOL
CGuildMarkPackManager::initForDev()
{
#ifdef	_IS_DEV_CLIENT
	CDecoder	reader;
	BOOL		bIsLooping	=	TRUE;

	if (!reader.Upload("dev data/scripter/guild_mark_info.txt",NULL))
		return	load();

	while(bIsLooping)
	{
		char	*str	=	NULL;
		char	*strKeyword	=	reader.getWord(TRUE);
		BOOL	bIsCorrect;
		int		iValue	=	0;

		switch(reader.analCode(strKeyword)) 
		{
			case	dCODE_CLOSE		:
				continue;

			case	dCODE_END		:
				bIsLooping	=	FALSE;
				continue;
				
			case	dCODE_FINISH	:
			case	dCODE_OPEN		:
				continue;
		}

		if (STRICMP(strKeyword,"베이스BasePackBegin")		==	0)	
		{
			m_base.m_wBasePackBegin				=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"베이스BasePackEnd")			==	0)	
		{
			m_base.m_wBasePackEnd				=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"베이스ExpandPackBegin")		==	0)	
		{
			m_base.m_wExpandPackBegin			=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"베이스ExpandPackEnd")		==	0)	
		{
			m_base.m_wExpandPackEnd				=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"베이스SpecialPackBegin")	==	0)	
		{
			m_base.m_wSpecialPackBegin			=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"베이스SpecialPackEnd")		==	0)	
		{
			m_base.m_wSpecialPackEnd			=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"베이스LuxuryPackBegin")		==	0)	
		{
			m_base.m_wLuxuryPackBegin			=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"베이스LuxuryPackEnd")		==	0)	
		{
			m_base.m_wLuxuryPackEnd				=	reader.getNumber(&bIsCorrect);
			continue;
		}

		if (STRICMP(strKeyword,"베이스분할BasePackBegin")	==	0)	
		{
			m_division.m_wBasePackBegin			=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"베이스분할BasePackEnd")		==	0)	
		{
			m_division.m_wBasePackEnd			=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"베이스분할ExpandPackBegin")	==	0)	
		{
			m_division.m_wExpandPackBegin		=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"베이스분할ExpandPackEnd")	==	0)	
		{
			m_division.m_wExpandPackEnd			=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"베이스분할SpecialPackBegin")==	0)	
		{
			m_division.m_wSpecialPackBegin		=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"베이스분할SpecialPackEnd")	==	0)	
		{
			m_division.m_wSpecialPackEnd		=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"베이스분할LuxuryPackBegin")	==	0)	
		{
			m_division.m_wLuxuryPackBegin		=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"베이스분할LuxuryPackEnd")	==	0)	
		{
			m_division.m_wLuxuryPackEnd			=	reader.getNumber(&bIsCorrect);
			continue;
		}

		if (STRICMP(strKeyword,"한글심볼BasePackBegin")		==	0)	
		{
			m_symbolHangul.m_wBasePackBegin		=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"한글심볼BasePackEnd")		==	0)	
		{
			m_symbolHangul.m_wBasePackEnd		=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"한글심볼ExpandPackBegin")	==	0)	
		{
			m_symbolHangul.m_wExpandPackBegin	=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"한글심볼ExpandPackEnd")		==	0)	
		{
			m_symbolHangul.m_wExpandPackEnd		=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"한글심볼SpecialPackBegin")	==	0)	
		{
			m_symbolHangul.m_wSpecialPackBegin	=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"한글심볼SpecialPackEnd")	==	0)	
		{
			m_symbolHangul.m_wSpecialPackEnd	=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"한글심볼LuxuryPackBegin")	==	0)	
		{
			m_symbolHangul.m_wLuxuryPackBegin	=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"한글심볼LuxuryPackEnd")		==	0)	
		{
			m_symbolHangul.m_wLuxuryPackEnd		=	reader.getNumber(&bIsCorrect);
			continue;
		}

		if (STRICMP(strKeyword,"영어심볼BasePackBegin")		==	0)	
		{
			m_symbolEnglish.m_wBasePackBegin	=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"영어심볼BasePackEnd")		==	0)	
		{
			m_symbolEnglish.m_wBasePackEnd		=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"영어심볼ExpandPackBegin")	==	0)	
		{
			m_symbolEnglish.m_wExpandPackBegin	=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"영어심볼ExpandPackEnd")		==	0)	
		{
			m_symbolEnglish.m_wExpandPackEnd	=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"영어심볼SpecialPackBegin")	==	0)	
		{
			m_symbolEnglish.m_wSpecialPackBegin	=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"영어심볼SpecialPackEnd")	==	0)	
		{
			m_symbolEnglish.m_wSpecialPackEnd	=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"영어심볼LuxuryPackBegin")	==	0)	
		{
			m_symbolEnglish.m_wLuxuryPackBegin	=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"영어심볼LuxuryPackEnd")		==	0)	
		{
			m_symbolEnglish.m_wLuxuryPackEnd	=	reader.getNumber(&bIsCorrect);
			continue;
		}

		if (STRICMP(strKeyword,"한문심볼BasePackBegin")		==	0)	
		{
			m_symbolHanmun.m_wBasePackBegin		=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"한문심볼BasePackEnd")		==	0)	
		{
			m_symbolHanmun.m_wBasePackEnd		=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"한문심볼ExpandPackBegin")	==	0)	
		{
			m_symbolHanmun.m_wExpandPackBegin	=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"한문심볼ExpandPackEnd")		==	0)	
		{
			m_symbolHanmun.m_wExpandPackEnd		=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"한문심볼SpecialPackBegin")	==	0)	
		{
			m_symbolHanmun.m_wSpecialPackBegin	=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"한문심볼SpecialPackEnd")	==	0)	
		{
			m_symbolHanmun.m_wSpecialPackEnd	=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"한문심볼LuxuryPackBegin")	==	0)	
		{
			m_symbolHanmun.m_wLuxuryPackBegin	=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"한문심볼LuxuryPackEnd")		==	0)	
		{
			m_symbolHanmun.m_wLuxuryPackEnd		=	reader.getNumber(&bIsCorrect);
			continue;
		}

		if (STRICMP(strKeyword,"아이템심볼BasePackBegin")	==	0)	
		{
			m_symbolItem.m_wBasePackBegin		=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"아이템심볼BasePackEnd")		==	0)	
		{
			m_symbolItem.m_wBasePackEnd			=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"아이템심볼ExpandPackBegin")	==	0)	
		{
			m_symbolItem.m_wExpandPackBegin		=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"아이템심볼ExpandPackEnd")	==	0)	
		{
			m_symbolItem.m_wExpandPackEnd		=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"아이템심볼SpecialPackBegin")==	0)	
		{
			m_symbolItem.m_wSpecialPackBegin	=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"아이템심볼SpecialPackEnd")	==	0)	
		{
			m_symbolItem.m_wSpecialPackEnd		=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"아이템심볼LuxuryPackBegin")	==	0)	
		{
			m_symbolItem.m_wLuxuryPackBegin		=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"아이템심볼LuxuryPackEnd")	==	0)	
		{
			m_symbolItem.m_wLuxuryPackEnd		=	reader.getNumber(&bIsCorrect);
			continue;
		}

		if (STRICMP(strKeyword,"동물심볼BasePackBegin")		==	0)	
		{
			m_symbolAnimal.m_wBasePackBegin		=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"동물심볼BasePackEnd")		==	0)	
		{
			m_symbolAnimal.m_wBasePackEnd		=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"동물심볼ExpandPackBegin")	==	0)	
		{
			m_symbolAnimal.m_wExpandPackBegin	=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"동물심볼ExpandPackEnd")		==	0)	
		{
			m_symbolAnimal.m_wExpandPackEnd		=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"동물심볼SpecialPackBegin")	==	0)	
		{
			m_symbolAnimal.m_wSpecialPackBegin	=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"동물심볼SpecialPackEnd")	==	0)	
		{
			m_symbolAnimal.m_wSpecialPackEnd	=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"동물심볼LuxuryPackBegin")	==	0)	
		{
			m_symbolAnimal.m_wLuxuryPackBegin	=	reader.getNumber(&bIsCorrect);
			continue;
		}
		if (STRICMP(strKeyword,"동물심볼LuxuryPackEnd")		==	0)	
		{
			m_symbolAnimal.m_wLuxuryPackEnd		=	reader.getNumber(&bIsCorrect);
			continue;
		}

	}

	return	save();
#endif
	return	TRUE;
}

BOOL
CGuildMarkPackManager::init()
{
	return	load();
}


//	길드 마크 아이템 정보 
BOOL
CGuildMarkPackManager::save()
{
	cFILE	file;

	if	(!file.Open("data/guildMarkInfo.dat","wb"))
		return	FALSE;

	file.Write(this,sizeof(CGuildMarkPackManager));
	file.Close();

	return	TRUE;
}

//	길드 마크 아이템 정보 불러오기
BOOL
CGuildMarkPackManager::load()
{
	cFILE	file;

	if (!file.Open("data/guildMarkInfo.dat","rb"))	return	FALSE;

	file.Read(this,sizeof(CGuildMarkPackManager));
	file.Close();

	return	TRUE;
}