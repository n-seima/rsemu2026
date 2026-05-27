#ifndef	_class_field_operator_command
#define	_class_field_operator_command

#include	"cFIELD.h"

class	cField_OC	:	public cFIELD
{
public:
	class	cCommand
	{
	public:
		char	strCommand[2][32];
		WORD	m_wCheckMethod,m_wIsOnlyTestServer,m_wRequireOperatorLevel,m_wCommand;
	};
	int		analOC(cACTOR *_lpChatter,char *_lpstrText,char **_lpstrCommand);
	BOOL	operateSpecialChat(CG_CHAT *_lpChatPacket,cACTOR *_lpChatter);
	inline void		setMessageKORorENG(char* _strBuffer , char* _strkor , char* _strEng)
	{
#ifdef	_FOR_KOREA
		strcpy(_strBuffer , _strkor);
#else
		strcpy(_strBuffer , _strEng);
#endif
	}
};

extern BOOL g_bIsMinipetActiveChaceTest;


#endif