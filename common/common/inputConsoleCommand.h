#pragma	once

class	cInputConsoleCommand
{
public:
	int		m_iCommandPos;
	char	m_strCommand[256];
	int		m_abValidKey[256];

	cInputConsoleCommand();

	char*	getCommand(int _bIsWantLowcase=0);
};