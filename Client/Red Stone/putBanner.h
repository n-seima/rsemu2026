#ifndef _classPutBanner_H_
#define	_classPutBanner_H_

#include	"CGamePlay.H"
#include	"bannerDefine.h"

enum
{
	ePUS_INPUT_TITLE,
	ePUS_INPUT_COMMENT,
};

class	cPutBannerWnd	:	public CGamePlay
{
public:
	int				m_iStatus;
	int				m_iBannerItem;
	char			m_strBannerText[dBANNER_TITLE_LENGTH];
	char			m_strBannerComment[dBANNER_COMMENT_LENGTH];

	CPos			m_pos;

					cPutBannerWnd();

	void			open(int _iBannerItem);//	¿
	void			close();
	void			update();

	BOOL			isOpened();

	void			draw();// ±×¸®±
	void			updateText(char *_lpstrText);

};

extern	cPutBannerWnd	g_gwPutBannerWnd;

#endif