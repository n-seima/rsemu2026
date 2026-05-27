#ifndef	_classLocalize_H
#define	_classLocalize_H

#include	"SFC.H"
#include	"CQuest.H"
#include	"CItem.H"
#include	"mainQuest.H"
#include	"book.H"

#define	dMAP_NAME_LENGTH		64

const	int	c_iLocalizeContentsCount	=	1024;

class	CReactionText
{
public:
	cSTRING				m_string;
	int					m_iCheckSum,m_iEvent,m_iKarma;
	BOOL				m_bIsChangeText;

	CReactionText()
	{
		reset();
	}
	void				reset()
	{
		m_string.Reset();
		m_iCheckSum		=	0x12345678;
		m_bIsChangeText	=	FALSE;
	}

	void				copy(CReactionText *_lpData)
	{
		reset();

		m_iCheckSum		=	_lpData->m_iCheckSum;
		m_string		=	_lpData->m_string;
		m_bIsChangeText	=	_lpData->m_bIsChangeText;
	}
};


class	cKarmaText
{
public:
	CReactionText		*m_apReaction[c_iLocalizeContentsCount];

	cKarmaText()
	{
		for (int i=0;i<c_iLocalizeContentsCount;i++)
			m_apReaction[i]	=	NULL;
	}

	~cKarmaText()
	{
		reset();
	}

	void				reset()
	{
		for (int i=0;i<c_iLocalizeContentsCount;i++)
			KILL(m_apReaction[i]);
	}

	void				copy(cKarmaText *_lpData)
	{
		reset();

		for (int i=0;i<c_iLocalizeContentsCount;i++)
		{
			if (_lpData->m_apReaction[i] == NULL)
				continue;

			m_apReaction[i]	=	new CReactionText();

			m_apReaction[i]->copy(_lpData->m_apReaction[i]);
		}
	}

};

class	CContentsText
{
public:
	cSTRING				m_string;
	int					m_iCheckSum;
	cKarmaText			*m_apKarma[c_iLocalizeContentsCount];

	CContentsText()
	{
		for (int i=0;i<c_iLocalizeContentsCount;i++)
			m_apKarma[i]	=	NULL;

		reset();
	}

	~CContentsText()
	{
		reset();
	}

	void				reset()
	{
		m_string.Reset();
		m_iCheckSum	=	0x12345678;

		for (int i=0;i<c_iLocalizeContentsCount;i++)
			KILL(m_apKarma[i]);
	}

	void				copy(CContentsText *_lpData)
	{
		reset();

		m_string	=	_lpData->m_string;
		m_iCheckSum	=	_lpData->m_iCheckSum;

		for (int i=0;i<c_iLocalizeContentsCount;i++)
		{
			if (_lpData->m_apKarma[i] == NULL)
				continue;

			m_apKarma[i]	=	new cKarmaText();

			m_apKarma[i]->copy(_lpData->m_apKarma[i]);
		}
	}
};

class	CConversationText
{
public:
	cSTRING				m_string;
	int					m_iCheckSum;
	CContentsText		*m_apContents[c_iLocalizeContentsCount];

	CConversationText()
	{
		for (int i=0;i<c_iLocalizeContentsCount;i++)
			m_apContents[i]	=	NULL;

		reset();
	}

	~CConversationText()
	{
		reset();
	}

	void				reset()
	{
		m_string.Reset();
		m_iCheckSum	=	0x12345678;

		for (int i=0;i<c_iLocalizeContentsCount;i++)
			KILL(m_apContents[i]);
	}

	void				copy(CConversationText *_lpData)
	{
		reset();

		m_string	=	_lpData->m_string;
		m_iCheckSum	=	_lpData->m_iCheckSum;

		for (int i=0;i<c_iLocalizeContentsCount;i++)
		{
			if (_lpData->m_apContents[i] == NULL)
				continue;

			m_apContents[i]	=	new CContentsText();

			m_apContents[i]->copy(_lpData->m_apContents[i]);
		}
	}
};


class	cCharacterText
{
public:
	int					m_iCheckSum;
	BOOL				m_bIsChangeText;
	cSTRING				m_strName;
	CContentsText		*m_apContents[c_iLocalizeContentsCount];

	cCharacterText()
	{
		for (int i=0;i<c_iLocalizeContentsCount;i++)
			m_apContents[i]	=	NULL;

		reset();
	}

	~cCharacterText()
	{
		reset();
	}

	void				reset()
	{
		m_iCheckSum	=	0x12345678;
		
		for (int i=0;i<c_iLocalizeContentsCount;i++)
			KILL(m_apContents[i]);

		m_bIsChangeText	=	FALSE;
	}

	void				copy(cCharacterText *_lpData)
	{
		reset();

		m_strName	=	_lpData->m_strName;

		for (int i=0;i<c_iLocalizeContentsCount;i++)
		{
			if (_lpData->m_apContents[i])
			{
				m_apContents[i]	=	new CContentsText();

				m_apContents[i]->copy(_lpData->m_apContents[i]);
			}
		}
	}
};

class	CKarmaText
{
public:
	BOOL				m_bIsChangeText;
	int					m_iCheckSum;
	CReactionText		*m_apReaction[c_iLocalizeContentsCount];

	CKarmaText()
	{
		for (int i=0;i<c_iLocalizeContentsCount;i++)
			m_apReaction[i]	=	NULL;

		reset();
	}

	~CKarmaText()
	{
		reset();
	}

	void				reset()
	{
		m_bIsChangeText	=	FALSE;
		m_iCheckSum		=	0x12345678;

		for (int i=0;i<c_iLocalizeContentsCount;i++)
			KILL(m_apReaction[i]);
	}

	void				copy(CKarmaText *_lpData)
	{
		reset();

		for (int i=0;i<c_iLocalizeContentsCount;i++)
		{
			if (_lpData->m_apReaction[i])
			{
				m_apReaction[i]	=	new CReactionText();

				m_apReaction[i]->copy(_lpData->m_apReaction[i]);
			}
		}

		m_iCheckSum		=	_lpData->m_iCheckSum;
		m_bIsChangeText	=	_lpData->m_bIsChangeText;
	}

};

class	CActorText
{
public:
	cSTRING				m_string;
	int					m_iCheckSum;
	BOOL				m_bIsChangeText;
	CConversationText	*m_apConversation[c_iLocalizeContentsCount];

	CActorText()
	{
		for (int i=0;i<c_iLocalizeContentsCount;i++)
			m_apConversation[i]	=	NULL;

		reset();
	}

	~CActorText()
	{
		reset();
	}

	void				reset()
	{
		m_string.Reset();

		for (int i=0;i<c_iLocalizeContentsCount;i++)
			KILL(m_apConversation[i]);

		m_iCheckSum		=	0x12345678;
		m_bIsChangeText	=	FALSE;
	}

	void				copy(CActorText *_lpData)
	{
		reset();
		m_string	=	_lpData->m_string;
		m_iCheckSum	=	_lpData->m_iCheckSum;

		for (int i=0;i<c_iLocalizeContentsCount;i++)
		{
			if (_lpData->m_apConversation[i])
			{
				m_apConversation[i]	=	new CConversationText();

				m_apConversation[i]->copy(_lpData->m_apConversation[i]);
			}
		}
	}
};

class	CAreaText
{
public:
	cSTRING				m_string;
	int					m_iCheckSum;

	void				reset()
	{
		m_string.Reset();
		m_iCheckSum	=	0x12345678;
	}
};

class	CShopText
{
public:
	cSTRING				m_string;
	int					m_iCheckSum;

	void				reset()
	{
		m_string.Reset();
		m_iCheckSum	=	0x12345678;
	}
};

class	CQuestTextItem : public CQuestItem
{
public:
	int					m_iNameCheckSum;
	int					m_iClientCheckSum;
	int					m_iBeginClientCheckSum;
	int					m_iInfoCheckSum;
	int					m_iExplainCheckSum[dQUEST_STATUS_COUNT];

	BOOL				isDifferentCheckSum(CQuestTextItem *_lpCompare)
	{
		if	(m_iNameCheckSum	==	0x12345678)
			return	FALSE;
		if	(m_iNameCheckSum	!=	_lpCompare->m_iNameCheckSum)
			return	TRUE;
		if	(m_iInfoCheckSum	!=	_lpCompare->m_iInfoCheckSum)
			return	TRUE;
		if	(m_iClientCheckSum	!=	_lpCompare->m_iClientCheckSum)
			return	TRUE;
		if	(m_iBeginClientCheckSum	!=	_lpCompare->m_iBeginClientCheckSum)
			return	TRUE;

		for (int i=0;i<dQUEST_STATUS_COUNT;i++)
		{
			if (m_iExplainCheckSum[i]	!=	_lpCompare->m_iExplainCheckSum[i])
				return	TRUE;
		}

		return	FALSE;
	}

	void				reset()
	{
		m_iNameCheckSum		=	0x12345678;
		m_iClientCheckSum	=	0x12345678;
		m_iInfoCheckSum		=	0x12345678;
		m_iBeginClientCheckSum=	0x12345678;

		for (int i=0;i<dQUEST_STATUS_COUNT;i++)
			m_iExplainCheckSum[i]	=	0x12345678;
	}
};

class	CQuestText
{
public:
	CQuestTextItem		m_aQuest[dQUEST_COUNT],m_aArbeit[dQUEST_COUNT];

	void				reset()
	{
		for (int i=0;i<dQUEST_COUNT;i++)
		{
			m_aQuest[i].reset();
			m_aArbeit[i].reset();
		}
	}
};

class	cMainQuestChapterText
{
public:
	int		m_iNameCheckSum,m_iCommentCheckSum,m_iEpilogCommentCheckSum;
	int		m_aiHiQuestNameChecksum[c_iMaxHighQuestCount];

	char	m_strName[c_iChapterNameLength];
	char	m_strComment[c_iChapterCommentLength];
	char	m_strEpilogComment[c_iChapterEpilogCommentLength];
	char	m_astrHighQuest[c_iMaxHighQuestCount][c_iHighQuestNameLength];

	void	reset()
	{
		m_iNameCheckSum		=	0x12345678;
		m_iCommentCheckSum	=	0x12345678;
		m_iEpilogCommentCheckSum	=	0x12345678;

		for (int i=0;i<c_iMaxHighQuestCount;i++)
			m_aiHiQuestNameChecksum[i]	=	0x12345678;
	}
};

class	cMainQuestText
{
public:
	cMainQuestChapterText	m_aChapterList[c_iMaxChapterCount];

	void					reset()
	{
		for (int i=0;i<c_iMaxChapterCount;i++)
			m_aChapterList[i].reset();
	}
};

class	cBookPageText
{
public:
	int					m_iCheckSum;
	cSTRING				m_strText;

	void				reset()
	{
		m_iCheckSum	=	0x12345678;
		m_strText.Reset();
	}
};

class	cBookCategoryText
{
public:
	int					m_iCategoryCheckSum;
	char				m_strCategoryName[c_iBookCategoryNameLength];

	cBookPageText		m_aPage[c_iMaxBookPageCount];

	void				reset()
	{
		for	 (int i=0;i<c_iMaxBookPageCount;i++)
			m_aPage[i].reset();

		m_iCategoryCheckSum	=	0x12345678;
	}
};

class	cBookText
{
public:
	int					m_iCheckSum,m_iCommentCheckSum;
	char				m_strBookName[c_iBookNameLength];
	cSTRING				m_strItemComment;
	
	cBookCategoryText	m_aCategory[c_iMaxBookCategoryCount];

	void				reset()
	{
		for	 (int i=0;i<c_iMaxBookCategoryCount;i++)
			m_aCategory[i].reset();

		m_iCheckSum			=	0x12345678;
		m_iCommentCheckSum	=	0x12345678;
	}
};

class	cBookTextPack
{
public:
	cBookText		m_aBook[c_iBookCount];

	void			reset()
	{
		for	 (int i=0;i<c_iBookCount;i++)
			m_aBook[i].reset();
	}
};

#define	dMAX_COMPARE_TEXT_COUNT	4096

class	C1KindTextItem
{
public:
	int					m_iCheckSum;
	cSTRING				m_strText;

						C1KindTextItem()
						{
							reset();
						}

	void				reset()
	{
		m_iCheckSum	=	0x12345678;
	}

	BOOL				isDifferentCheckSum(C1KindTextItem *_lpCompare)
	{
		if (m_iCheckSum	==	0x12345678)
			return	FALSE;

		if (m_iCheckSum		!=	_lpCompare->m_iCheckSum)
			return	TRUE;
		
		return	FALSE;
	}
};

class	C1KindText
{
public:
	C1KindTextItem		m_aItems[dMAX_COMPARE_TEXT_COUNT];

	void				reset()
	{
		for (int i=0;i<dMAX_COMPARE_TEXT_COUNT;i++)
			m_aItems[i].reset();
	}
};

class	C2KindTextItem
{
public:
	int					m_iNameCheckSum,m_iExplainCheckSum;
	cSTRING				m_strName,m_strExplain;

						C2KindTextItem()
						{
							reset();
						}

	void				reset()
	{
		m_iNameCheckSum	=	0x12345678;
	}

	BOOL				isDifferentCheckSum(C2KindTextItem *_lpCompare)
	{
		if (m_iNameCheckSum	==	0x12345678)
			return	FALSE;

		if (m_iNameCheckSum		!=	_lpCompare->m_iNameCheckSum)
			return	TRUE;
		if (m_iExplainCheckSum	!=	_lpCompare->m_iExplainCheckSum)
			return	TRUE;
		
		return	FALSE;
	}
};


class	C2KindText
{
public:
	C2KindTextItem		m_aItems[dBASIC_ITEM_COUNT];

	void				reset()
	{
		for (int i=0;i<dBASIC_ITEM_COUNT;i++)
			m_aItems[i].reset();
	}
};

class	cSetItemTextItem
{
public:
	int					m_iNameCheckSum,m_iExplainCheckSum;
	cSTRING				m_strName,m_strExplain;

						cSetItemTextItem()
						{
							reset();
						}

	void				reset()
	{
		m_iNameCheckSum		=	0x12345678;
		m_iExplainCheckSum	=	0x12345678;
	}

	BOOL				isDifferentCheckSum(cSetItemTextItem *_lpCompare)
	{
		if	(m_iNameCheckSum	==	0x12345678)
			return	FALSE;

		if	(m_iNameCheckSum	!=	_lpCompare->m_iNameCheckSum)
			return	TRUE;
		if	(m_iExplainCheckSum	!=	_lpCompare->m_iExplainCheckSum)
			return	TRUE;

		return	FALSE;
	}
};

#define	dMAX_ITEM_COUNT_IN_SET	10

class	cItemSetTextItem
{
public:
	int					m_iNameCheckSum,m_iExplainCheckSum;
	cSTRING				m_strName,m_strExplain;

	cSetItemTextItem	m_aData[dMAX_ITEM_COUNT_IN_SET];

						cItemSetTextItem()
						{
							reset();
						}

	void				reset()
	{
		m_iNameCheckSum		=	0x12345678;
		m_iExplainCheckSum	=	0x12345678;
	}

	BOOL				isDifferentCheckSum(cItemSetTextItem *_lpCompare)
	{
		if	(m_iNameCheckSum	==	0x12345678)
			return	FALSE;

		if	(m_iNameCheckSum	!=	_lpCompare->m_iNameCheckSum)
			return	TRUE;
		if	(m_iExplainCheckSum	!=	_lpCompare->m_iExplainCheckSum)
			return	TRUE;

		for	(int	i=0;i<dMAX_ITEM_COUNT_IN_SET;i++)
		{
			if	(m_aData[i].isDifferentCheckSum(&_lpCompare->m_aData[i]))
				return	TRUE;
		}
		
		return	FALSE;
	}

};

class	cItemSetText
{
public:
	cItemSetTextItem	m_aSetItem[dMAX_SET_ITEM_GROUP_COUNT];

	void				reset()
	{
		for (int i=0;i<dMAX_SET_ITEM_GROUP_COUNT;i++)
			m_aSetItem[i].reset();
	}
};

class	CSkillTextItem
{
public:
	int					m_iNameCheckSum,m_iPowerUpCheckSum,m_iExplainCheckSum;
	cSTRING				m_strName,m_strPowerUp,m_strExplain;

						CSkillTextItem()
						{
							reset();
						}

	void				reset()
	{
		m_iNameCheckSum	=	0x12345678;
	}

	BOOL				isDifferentCheckSum(CSkillTextItem *_lpCompare)
	{
		if (m_iNameCheckSum		==	0x12345678)
			return	FALSE;

		if (m_iNameCheckSum		!=	_lpCompare->m_iNameCheckSum)
			return	TRUE;
		if (m_iExplainCheckSum	!=	_lpCompare->m_iExplainCheckSum)
			return	TRUE;
		if (m_iPowerUpCheckSum	!=	_lpCompare->m_iPowerUpCheckSum)
			return	TRUE;
		
		return	FALSE;
	}
};

class	CSkillText
{
public:
	CSkillTextItem		m_aSkill[dMAX_SKILL];

	void				reset()
	{
		for (int i=0;i<dMAX_SKILL;i++)
			m_aSkill[i].reset();
	}
};


class	CProjectKarmaText
{
public:
	CKarmaText			m_aMD_Karma[1024];
	CKarmaText			m_aMG_Karma[1024];

	void				reset()
	{
		for (int i=0;i<sizeof(m_aMD_Karma)/sizeof(CKarmaText);i++)
		{
			m_aMD_Karma[i].reset();
			m_aMG_Karma[i].reset();
		}
	}
};

class	CFieldText
{
public:
	int					m_iFieldNameCheckSum;
	cSTRING				m_string;
	CActorText			m_aActor[1024];
	CActorText			m_aArea[1024];
	CShopText			m_aShop[1024];
	cCharacterText		m_aCharacter[1024];
	CKarmaText			m_aKarma[1024];

	void				reset()
	{
		int	i;

		m_string.Reset();
		m_iFieldNameCheckSum	=	0x12345678;

		for (i=0;i<sizeof(m_aActor)/sizeof(CActorText);i++)
			m_aActor[i].reset();
		for (i=0;i<sizeof(m_aArea)/sizeof(CActorText);i++)
			m_aArea[i].reset();
		for (i=0;i<sizeof(m_aShop)/sizeof(CShopText);i++)
			m_aShop[i].reset();
		for (i=0;i<sizeof(m_aCharacter)/sizeof(cCharacterText);i++)
			m_aCharacter[i].reset();
		for (i=0;i<sizeof(m_aKarma)/sizeof(CKarmaText);i++)
			m_aKarma[i].reset();
	}
};

#define	dMAX_MAP_COUNT	2048

class	CMapSimpleInfo
{
public:
	WORD				m_wSerial;
	WORD				m_wWidth,m_wHeight;
	WORD				m_bf8Type			:	8;
	WORD				m_bf1IsPremiumZone	:	1;
	char				m_strName[dMAP_NAME_LENGTH];
	char				m_strFileName[dMAP_NAME_LENGTH];
};

extern	CQuestText		l_srcQuest,l_destQuest;
extern	C1KindText		l_src1KindText,l_dest1KindText;
extern	C2KindText		l_srcItem,l_destItem;
extern	CSkillText		l_srcSkill,l_destSkill;
extern	cItemSetText	l_srcItemSet,l_destItemSet;
extern	cMainQuestText	l_srcMainQuestText,l_destMainQuestText;
extern	cBookTextPack	l_srcBookText,l_destBookText;
extern	CProjectKarmaText	l_srcProjectKarma,l_destProjectKarma;

extern	CQuest			l_localWorkQuest,l_localWorkarbeit;
extern	cBASIC_ITEM		l_aBasicItem[dBASIC_ITEM_COUNT];
extern	cSKILL			l_aSkill[dMAX_SKILL];
extern	CMapSimpleInfo	l_aMapSimpleInfo[dMAX_MAP_COUNT];
extern	cJOB_DEN		jobDen;

extern	int			l_aConvertMapList[dMAX_MAP_COUNT];

inline	BOOL	IsConvertMap(int _iIndex)
{
	if (l_aConvertMapList[0]	==	-1)
		return	TRUE;

	for (int i=0;;i++)
	{
		if(l_aConvertMapList[i]	==	-1)
			break;

		if (l_aConvertMapList[i]==	_iIndex)
			return	TRUE;
	}

	return	FALSE;
}

extern	BOOL			g_bIsJapan;
extern	int				g_iLocalizeNation;

enum
{
	eNationKorea	= 0,
	eNationJapan,
	eNationChina,
	eNationKoreaNotUsed,
	eNationInter,
	eNationUsa,
	eNationThai,
};

#endif