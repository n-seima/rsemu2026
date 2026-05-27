#ifndef	_classLoadTextData_H
#define	_classLoadTextData_H

#include	"SFC.H"
#include	"cDECODER.h"

class	CLoadTextData
{
public:
	static	BOOL	s_bIsExist;	//	Ω±€

	cDECODER		m_decoder;
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
	int				m_iTipCount;
	int				m_iMonsterGradeCount;
	int				m_iMonsterTypeCount;
	int				m_iSummonBeastCount;
	int				m_iNameFilterCount;

	int				m_iDebugCommandCount,m_iItemExplainCount,m_iItemPrefixCount,m_iTitleCount,m_iHelpCount;
	int				m_iExtraItemGradeCount,m_iServerCommentCount,m_iStatusTooltipCount,m_iEventMobCount,m_iMonsterDicCount , m_iGGGWordCount;

					CLoadTextData();
					~CLoadTextData();

	void			close();
	BOOL			run(char *_lpstrFodler);
	BOOL			save(char *_lpstrFodler);
	BOOL			loadTextScriptFile(char *_lpstrFodler);
	BOOL			exportText(char *_lpstrFodler);
	BOOL			writeSimpleText(char *_lpstrFileName,int _iTextCount,char **_lpstrText);

	BOOL			decodeFontScript();
	BOOL			decodeDebugScript();
	BOOL			decodeItemExplainScript();
	BOOL			decodeItemPrefixScript();
	BOOL			decodeTitleScript();
	BOOL			decodeHelpScript();
	BOOL			decodeEventMobScript();

	
	char**			decodeSimpleTextScriptFile(char *_lpstrFn,int *_lpiCount);
	char**			decodeSimpleTextScriptFile2(char *_lpstrFn,int *_lpiCount);

	void			saveSimpleTextData(cFILE *_lpFile,char **_lpTextData,int _iCount);
	char**			loadSimpleTextData(cFILE *_lpFile,int *_lpiCount);
	BOOL			load();
};

extern	CLoadTextData	g_loaderText;

#endif