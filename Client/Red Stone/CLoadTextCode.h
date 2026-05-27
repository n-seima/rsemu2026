#ifndef	_classLoadTextData_H
#define	_classLoadTextData_H

#include "cNEMO.H"

class	CLoadTextData
{
public:
	static	BOOL	s_bIsExist;	//	싱글톤

	int				m_iCommonMessageCount;
	int				m_iEquipmentAllignmentTextCount;
	int				m_iEquipUseLimitContentsTextCount;
	int				m_iEquipmentNameTextCount;
	int				m_iGuildRankTextCount;
	int				m_iHeroJobTextCount;
	int				m_iCorrectMagicResistanceMessageCount;
	int				m_iNpcNameTextCount;
	int				m_iRegistServerMessageCount;
	int				m_iSlanderTextCount;
	int				m_iNameFilterCount;
	int				m_iTipCount;
	int				m_iMonsterGradeCount;
	int				m_iMonsterTypeCount;
	int				m_iSummonBeastCount;
	int				m_iEventMobCount;
	int				m_iMonsterDicCount;			//몬스터 도감 관련
	int				m_iGreateGodGraceCount;		//거신의 은총 관련

	int				m_iDebugCommandCount,m_iItemExplainCount,m_iItemPrefixCount,m_iTitleCount,m_iHelpCount;
	int				m_iExtraItemGradeCount,m_iServerCommentCount,m_iStatusTooltipCount;

					CLoadTextData();
					~CLoadTextData();
	
	BOOL			init();
	BOOL			save();
	BOOL			load();

	BOOL			decodeFontScript();
	BOOL			decodeDebugScript();
	BOOL			decodeItemExplainScript();
	BOOL			decodeItemPrefixScript();
	BOOL			decodeHelpScript();
	BOOL			decodeTitleScript();
	BOOL			decodeEventMobScript();
	
	char**			decodeSimpleTextScriptFile(char *_lpstrFn,int *_lpiCount);
	BOOL			loadTextScriptFile();

	void			saveSimpleTextData(cFILE *_lpFile,char **_lpTextData,int _iCount);
	char**			loadSimpleTextData(cFILE *_lpFile,int *_lpiCount);
};

extern	CLoadTextData	g_loaderText;

inline	GetTextCheckSum(char *_lpstrText)
{
	int	iCheckSum	=	0;
	int	iStrsize	=	strlen(_lpstrText);

	for (int i=0;i<iStrsize;i++)
		iCheckSum	+=	_lpstrText[i];

	return	iCheckSum;	
}

#endif