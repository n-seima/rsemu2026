#include "message.H"

#ifdef	_FOR_ENG
char	*g_strMessage[]	=	
{
	//	0~5
	"Remove %s",
	"Uninstall",
	"Removes %s.",
	"An error has occurred during the uninstallation process.",
	"Cannot find the registry information.",
	"Cannot find the folder in which the program is installed:\n\n%s",
	
	//	6~10
	"All files were removed.",
	"Stop the process",
	"The process is not complete. If you quit now, the program will not be removed properly.\n\n\
To complete the uninstallation properly, you need to continue the process.\n\n\
Do you want to quit the uninstallation?",
	"Uninstallation complete",
	"The uninstallation process is complete.",

	" ",
	"This process will remove all the data in the following folder and the folder itself.\n\nDo you want to continue?\n\n\n[%s]",
	"The registry is corrupt.",
	" various registry data have been deleted.\n\nYou need to remove the rest of the data manually.",
	"Cannot find the following file which is necessary for the uninstallation:\n\n[%s]",

	" This program will remove %s from your computer.",
	" Click the Remove button to continue or click the Cancel button to stop the process.",
	"Removing %s file",
	"Error uninstalling the program!!",
	"Cannot find the 'Uninstall.dat' file.",

	"Red Stone"
};

#elif	_FOR_INTER
char	*g_strMessage[]	=	
{
	//	0~5
	"Remove %s",
	"Uninstall",
	"Removes %s.",
	"An error has occurred during the uninstallation process.",
	"Cannot find the registry information.",
	"Cannot find the folder in which the program is installed:\n\n%s",
	
	//	6~10
	"All files were removed.",
	"Stop the process",
	"The process is not complete. If you quit now, the program will not be removed properly.\n\n\
To complete the uninstallation properly, you need to continue the process.\n\n\
Do you want to quit the uninstallation?",
	"Uninstallation complete",
	"The uninstallation process is complete.",

	" ",
	"This process will remove all the data in the following folder and the folder itself.\n\nDo you want to continue?\n\n\n[%s]",
	"The registry is corrupt.",
	" various registry data have been deleted.\n\nYou need to remove the rest of the data manually.",
	"Cannot find the following file which is necessary for the uninstallation:\n\n[%s]",

	" This program will remove %s from your computer.",
	" Click the Remove button to continue or click the Cancel button to stop the process.",
	"Removing %s file",
	"Error uninstalling the program!!",
	"Cannot find the 'Uninstall.dat' file.",

	"Red Stone"
};

#endif
