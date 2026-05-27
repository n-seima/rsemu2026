#ifndef _classGameSaveOption_h
#define _classGameSaveOption_h

#include "SFC.H"

#ifdef	_FOR_INTER
#define	dID_LENGTH		64
#else
#define	dID_LENGTH		20
#endif

#define	dNAME_LENGTH	18

enum
{
	eCFV_FIRST,
	eCFV_ADD_MINIMAP_DISPLAY_OPTION,
	eCFV_ADD_MUSIC_ON,
	eCFV_ADD_OUTPUT_DEVICE,
	eCFV_ADD_WINDOW_MODE,
	eCFV_CURRENT_VERSION,
};
#pragma pack(2)
class	CGameOption
{
public:
	DWORD			m_dwCheckCode;
	DWORD			m_dwConfigVersion;
	char			m_strLastId[dID_LENGTH];
	int				m_iSelectAvatar;
	DWORD			m_dwStaticMemorySize;
	char			m_strLastSelectServerName[dNAME_LENGTH];

	int				m_iSoundVolume;	//	사운드 볼률 조절
	int				m_iMusicVolume;	//	음악 볼륨 조절

	DWORD			m_bf1IsViewSkillRange		:	1;	//1
	DWORD			m_bf1IsRememberId			:	1;	//2
	DWORD			m_bf1IsUseOldStyleShotKey	:	1;	//3
	DWORD			m_bf1IsHideTip				:	1;	//4
	DWORD			m_bf1IsBattleChat			:	1;	//5
	DWORD			m_bf1IsHidePartyItemMessage	:	1;	//6
	DWORD			m_bf1IsWindowMode			:	1;	//7
	DWORD			m_bf1IsSlimMemoryMode		:	1;	//8
	DWORD			m_bf1IsShowMiniPetWindow	:	1;	//9
	DWORD			m_bf2TitleViewType			:	2;	//11		// 10.02.18
	DWORD			m_bf1IsMiniPetSoundOff		:	1;	//12			// 미니펫 효과음끄기 
	DWORD			m_bf1IsShakeOff				:	1;	//13			// 화면 흔들림 끄기 
	DWORD			m_bf1IsShowDropItem			:	1;	//14			// 드랍 아이템 보기
	DWORD			m_bf1IsUse1024X768			:	1;	//15			// 1024 768 해상도 사용
	DWORD			m_bf1IsBodyEffectOff		:	1;	//16			//	몸에 붙는 이펙트 끄기
	DWORD			:	0;
	WORD			m_wIsTooltipOn;	//	툴팁 표시 (켜기, 끄기)
	WORD			m_wIsViewLargeSkillIcon;	//	스킬 아이콘 표시툴팁 표시 (켜기, 끄기)
	BOOL			m_bIsInfoWindowHalfblending;//	정보창 표시 방법 (반투명, 불투명)
	BOOL			m_bIsChatWindowHalfblending;//	채팅창 표시 방법 (반투명, 불투명)
	CPos			m_posChattingBox;
	int				m_iChattingBoxScale,m_iChattingBoxHeight;
	int				m_iMinimapWindowScale;
	int				m_iMinimapOutputMethod;//	미니맵 표시 방법 (반투명, 불투명)
	BOOL			m_bIsMusicOn;
	BOOL			m_bIsSoundOn;
	BOOL			m_bIsDisplayShotKey;
	WORD			m_wIsAllowWhisper,m_wIsAllowShout,m_wIsExclusiveMode,m_wIsAllowPartyChat;
	int				m_iOutputDevice;
	BOOL			m_bIsDoubleSizeTargetting;
	// insu add
	BOOL			b_bIsNewCarrotShop;
	// insu add end
					CGameOption()
					{
						memset(this,0,sizeof(CGameOption));

						m_dwStaticMemorySize		=	100*1000;
						m_iSoundVolume				=	1000;	//	사운드 볼률 조절
						m_iMusicVolume				=	1000;	//	음악 볼륨 조절
						m_wIsTooltipOn				=	TRUE;	//	툴팁 표시 (켜기, 끄기)
						m_bIsInfoWindowHalfblending	=	TRUE;	//	정보창 표시 방법 (반투명, 불투명)
						m_bIsChatWindowHalfblending	=	TRUE;	//	채팅창 표시 방법 (반투명, 불투명)
						m_wIsViewLargeSkillIcon		=	TRUE;
						m_bIsMusicOn				=	TRUE;
						m_bIsSoundOn				=	TRUE;
						m_bIsDisplayShotKey			=	TRUE;

						m_wIsAllowWhisper			=	TRUE;
						m_wIsAllowShout				=	TRUE;
						m_wIsAllowPartyChat			=	TRUE;
						b_bIsNewCarrotShop			=	FALSE;	// insu add ■■■■■■■■■■■■■■■
					}
};	//	class	CGameOption
#pragma pack()

extern	CGameOption	g_config;

#endif