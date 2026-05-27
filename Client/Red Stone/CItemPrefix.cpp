#include "CItemPrefix.H"
#include "CJob.h"
#include "CMessage.h"

#define dPREFIX_FOR_ALL_SKILL_TYPE			103	//특정 케릭터의  all skill 접두사의 타입

cITEM_PREFIX			g_aItemPrefix[dMAX_ITEM_PREFIX_COUNT];
cITEM_PREFIX			g_aBackupItemPrefix[dMAX_ITEM_PREFIX_COUNT];
cITEM_PREFIX_MANAGER	g_itemPrefix;

//	로컬 작업 필요 없음.(아니 필요한데... 여기를 고칠 필요은 없다는 얘기)
//	itemPrefix.txt
//
//	아이템 효과들의 인덱스를 얻어 온다.(시리얼 참조)
int
cITEM_PREFIX_MANAGER::getIndex(int _iSerial)
{
	int	iIndex	=	0;

	while(1)
	{
		if	(g_aPrefixHyperText[iIndex].m_iSerial	==	0xffff	)	break;
		if	(g_aPrefixHyperText[iIndex].m_iSerial	==	_iSerial)	return	iIndex;

		iIndex++;
	}

	return	0xffff;
}

char*
cITEM_PREFIX_MANAGER::getComment(int _iPrefix)
{
	cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[_iPrefix];

	static	char	strText[512];

	CItemPrefixInfo	prefix;

	prefix.m_wPrefix	=	_iPrefix;
	prefix.m_aValue[0]	=	lpPrefix->m_aValue[0][1];
	prefix.m_aValue[1]	=	lpPrefix->m_aValue[1][1];

	strcpy(strText,getComment(&prefix,TRUE));

	return	strText;
}

//
//	아이템 부가 효과 설명
char*
cITEM_PREFIX_MANAGER::getComment(CItemPrefixInfo *_lpPrefixInfo,BOOL _bIsWantRange)
{
	cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[_lpPrefixInfo->m_wPrefix];

	int		iIndex	=	getIndex(lpPrefix->m_wType);
	if		(iIndex	==	0xffff)	return	NULL;

	char	*lpText	=	g_aPrefixHyperText[iIndex].m_strReferenceText;
	char	*lpStream;

	m_string.Reset();

	if	(lpPrefix->m_wType	==	eIP_JOB_SKILL_LEVEL)
	{
		if	(_bIsWantRange)
			m_string.Add("<c:LTYELLOW>%s<n> %s +<c:LTYELLOW>%d<n>~<c:LTYELLOW>%d<n>",g_pstrHeroJobName[_lpPrefixInfo->m_aValue[1]],dMSG_SKILL_LEVEL,lpPrefix->m_aValue[0][0],lpPrefix->m_aValue[0][1]);
		else
			m_string.Add("<c:LTYELLOW>%s<n> %s +<c:LTYELLOW>%d<n>",g_pstrHeroJobName[_lpPrefixInfo->m_aValue[1]],dMSG_SKILL_LEVEL,_lpPrefixInfo->m_aValue[0]);

		return	m_string.String;
	}

	int	aiCorrectValue[2]	=	{0,0};

	if	(lpPrefix->m_wType	==	eIP_POTION_HEAL_SPEED)
		aiCorrectValue[0]	=	50;

	m_decoder.UploadBuffer(lpText,NULL);

	BOOL	bFirst					=	TRUE;
	BOOL	bIsAcquireSymbol		=	FALSE;
	BOOL	bIsAcquireMinusSymbol	=	FALSE;
	int		iStreamCount			=	0;
	while(1)
	{
		bIsAcquireSymbol	=	FALSE;
		lpStream	=	m_decoder.getStream('[');

		if	(!lpStream && !bFirst)
			break;

		bFirst		=	FALSE;

		if	(lpStream)
			m_string.Add(lpStream);
		if	(m_decoder.m_isDone	)
			break;		//	다 읽어 들였다.

		if	(m_decoder.getFirstChar() == '+')
		{
			m_decoder.increaseBP();
			bIsAcquireSymbol		=	TRUE;
		}
		if	(m_decoder.getFirstChar() == '-')
		{
			m_decoder.increaseBP();
			bIsAcquireMinusSymbol	=	TRUE;
		}

		iIndex		=	m_decoder.getNumber();
		int	iValue	=	0;

		if	(iIndex	==	2)
			iValue	=	lpPrefix->m_sValue3;
		else
			iValue	=	_lpPrefixInfo->m_aValue[iIndex]+aiCorrectValue[iIndex];

		if	(bIsAcquireMinusSymbol)
			iValue	=	-iValue;

		lpStream	=	m_decoder.getStream(']');
		
		if	(_bIsWantRange && iIndex != 2)
		{
			if	(bIsAcquireSymbol && iValue>0)
				m_string.Add(_ms("<c:LTYELLOW>+%d<n>~<c:LTYELLOW>+%d<n>",
					lpPrefix->m_aValue[iIndex][0]+aiCorrectValue[iIndex],lpPrefix->m_aValue[iIndex][1]+aiCorrectValue[iIndex]));
			else
				m_string.Add(_ms("<c:LTYELLOW>%d<n>~<c:LTYELLOW>%d<n>",
					lpPrefix->m_aValue[iIndex][0]+aiCorrectValue[iIndex],lpPrefix->m_aValue[iIndex][1]+aiCorrectValue[iIndex]));
		}
		else
		{
			if	(bIsAcquireSymbol && iValue>0)
				m_string.Add(_ms("<c:LTYELLOW>+%d<n>",iValue));
			else if(iIndex == eIP_ACL_BUFF_SKILL_CASTING)
			{
				if(iStreamCount == 0)
					m_string.Add(g_aSkill[iValue].m_strName);
				else
					m_string.Add(_ms("<c:LTYELLOW>%d<n>",iValue));
			}
			else
				m_string.Add(_ms("<c:LTYELLOW>%d<n>",iValue));
		}
		++iStreamCount;
		if	(m_decoder.m_isDone	)
			break;		//	다 읽어 들였다.
	}

	return	m_string.String;
}


//
//	아이템 부가 효과 설명
char*
cITEM_PREFIX_MANAGER::getComment(cUniqueData *_lpUnique)
{
	int		iPrefixEffectIndex	=	getIndex(_lpUnique->m_wEffect);

	if	(iPrefixEffectIndex	==	0xffff)
		return	NULL;

	char	*lpText	=	g_aPrefixHyperText[iPrefixEffectIndex].m_strReferenceText;
	char	*lpStream;

	m_string.Reset();

	if (_lpUnique->m_wEffect	==	eIP_JOB_SKILL_LEVEL)
	{
		m_string.Add("<c:LTYELLOW>%s<n> %s +<c:LTYELLOW>%d<n>",g_pstrHeroJobName[_lpUnique->m_aValue[1]],dMSG_SKILL_LEVEL,_lpUnique->m_aValue[0]);

		return	m_string.String;
	}

	m_decoder.UploadBuffer(lpText,NULL);

	BOOL	bFirst					=	TRUE;
	BOOL	bIsAcquireSymbol		=	FALSE;
	BOOL	bIsAcquireMinusSymbol	=	FALSE;
	int		iStreamCount			=	0;
	while(1)
	{
		bIsAcquireSymbol	=	FALSE;
		lpStream	=	m_decoder.getStream('[');

		if	(!lpStream && !bFirst)
			break;

		bFirst		=	FALSE;

		if	(lpStream)
			m_string.Add(lpStream);
		if	(m_decoder.m_isDone	)
			break;		//	다 읽어 들였다.

		if (m_decoder.getFirstChar() == '+')
		{
			m_decoder.increaseBP();
			bIsAcquireSymbol	=	TRUE;
		}
		if (m_decoder.getFirstChar() == '-')
		{
			m_decoder.increaseBP();
			bIsAcquireMinusSymbol	=	TRUE;
		}

		int	iIndex	=	m_decoder.getNumber();
		int	iValue	=	_lpUnique->m_aValue[iIndex];

		if	(_lpUnique->m_wEffect	==	eIP_POTION_HEAL_SPEED)
			iValue	+=	50;

		if	(bIsAcquireMinusSymbol)
			iValue	=	-iValue;

		lpStream	=	m_decoder.getStream(']');

		if	(bIsAcquireSymbol && iValue > 0 )
			m_string.Add(_ms("<c:LTYELLOW>+%d<n>",iValue));
		else if(iPrefixEffectIndex == eIP_ACL_BUFF_SKILL_CASTING)
			{
				if(iStreamCount == 0)
					m_string.Add(g_aSkill[iValue].m_strName);
				else
					m_string.Add(_ms("<c:LTYELLOW>%d<n>",iValue));
			}
		else
			m_string.Add(_ms("<c:LTYELLOW>%d<n>",iValue));
		++iStreamCount;
		if	(m_decoder.m_isDone	)
			break;		//	다 읽어 들였다.
	}

	return	m_string.String;
}

int
cITEM_PREFIX_MANAGER::getPrefixByType(int _iType,int _iLevel)
{
	int	iCount	=	0,i;

	if(_iType == dPREFIX_FOR_ALL_SKILL_TYPE)
	{
		if(_iLevel % 3 == 0)
			return	0xffff;

		//_iLevel = (_iLevel) % 3;
	}

	for (i=0;i<dMAX_ITEM_PREFIX_COUNT;i++)
	{
		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[i];

		if	(lpPrefix->m_wDiscernmentCode	>=	10000)	//	꼬맹이 접두사 무시
			continue;

		if	(lpPrefix->m_wType	!=	_iType)
			continue;

		if	(_iLevel	==	iCount)
			return	i;

		iCount++;
	}

	return	0xffff;
}

int
cITEM_PREFIX_MANAGER::getPrefixLevel(int _iPrefix)
{
	int	iCount	=	0,i;
	int	iType	=	g_aItemPrefix[_iPrefix].m_wType;

	for (i=0;i<dMAX_ITEM_PREFIX_COUNT;i++)
	{
		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[i];

		if	(lpPrefix->m_wDiscernmentCode	>=	10000)	//	꼬맹이 접두사 무시
			continue;

		if	(_iPrefix			==	i		)
			return	iCount;

		if	(lpPrefix->m_wType	==	iType	)
			iCount++;
	}

	return	0xffff;
}

int
cITEM_PREFIX_MANAGER::getPrefixCountByType(int _iType)
{
	int	iCount	=	0,i;

	for (i=0;i<dMAX_ITEM_PREFIX_COUNT;i++)
	{
		cITEM_PREFIX	*lpPrefix	=	&g_aItemPrefix[i];

		if	(lpPrefix->m_wDiscernmentCode	>=	10000)
			continue;

		if	(lpPrefix->m_wType	==	_iType)
			iCount++;
	}

	return	iCount;
}

BOOL
cITEM_PREFIX_MANAGER::isLevelUpAble(int _iItemKind,int _iPrefix,int *_lpiNextPrefix)
{
	if	(g_aItemPrefix[_iPrefix].m_wDiscernmentCode	>=	10000)
		return	FALSE;

	int	iType	=	g_aItemPrefix[_iPrefix].m_wType;
	int	iLevel	=	getPrefixLevel(_iPrefix);

	if	(iLevel	==	0xffff)
		return	FALSE;

	iLevel++;
	int	iNextPrefix	=	getPrefixByType(iType,iLevel);

	if	(iNextPrefix	==	0xffff)
		return	FALSE;

	if	(!g_aItemPrefix[iNextPrefix].isStickAbleItem(_iItemKind))
		return	FALSE;

	int	iOldKind	=	0;
	int	iCurrentKind=	0;

	if	(g_aItemPrefix[_iPrefix].m_wIsDXPrefix)
		iOldKind	=	1;
	if	(g_aItemPrefix[_iPrefix].m_wIsUltimatePrefix)
		iOldKind	=	2;
	if	(g_aItemPrefix[iNextPrefix].m_wIsDXPrefix)
		iCurrentKind=	1;
	if	(g_aItemPrefix[iNextPrefix].m_wIsUltimatePrefix)
		iCurrentKind=	2;

	if	(iOldKind	!=	iCurrentKind)
		return	FALSE;

	*_lpiNextPrefix	=	iNextPrefix;

	return	TRUE;
}
