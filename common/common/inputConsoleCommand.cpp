#include "inputConsoleCommand.h"
#include <conio.h>
#include <stdio.h>
#include <memory.h>

cInputConsoleCommand::cInputConsoleCommand()
{
	m_iCommandPos	=	0;

	memset(m_strCommand,0,sizeof(m_strCommand));
	memset(m_abValidKey,0,sizeof(m_abValidKey));

	int iKey;

	for (iKey='a';iKey<='z';iKey++)
		m_abValidKey[iKey]	=	1;

	for (iKey='A';iKey<='Z';iKey++)
		m_abValidKey[iKey]	=	1;

	for (iKey='0';iKey<='9';iKey++)
		m_abValidKey[iKey]	=	1;

	m_abValidKey['-']	=	1;
	m_abValidKey['_']	=	1;
	m_abValidKey[' ']	=	1;
}

char*
cInputConsoleCommand::getCommand(int _bIsWantLowcase)
{
	if	(!kbhit())
		return	0;

	int	chKey	=	getch();

	if	(chKey	==	10	||	chKey	==	13	)
	{
		m_strCommand[m_iCommandPos]	=	NULL;

		if	(_bIsWantLowcase)
		{
			for (int i=m_iCommandPos;i;)
			{
				--i;

				if	(m_strCommand[i]	>=	'A'	&&	m_strCommand[i]	<=	'Z'	)
				{
					m_strCommand[i]	-=	'A';
					m_strCommand[i]	+=	'a';
				}
			}
		}

		m_iCommandPos	=	0;
		printf("\n");
		return	m_strCommand;
	}

	if	(m_abValidKey[chKey])
	{
		if	(m_iCommandPos	<	sizeof(m_strCommand))
		{
			m_strCommand[m_iCommandPos++]	=	chKey;

			printf("%c",chKey);
		}
	}

	return	0;
}