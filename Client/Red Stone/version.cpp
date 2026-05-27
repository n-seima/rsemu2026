#include	"version.h"
#include	"stdio.h"

int		g_iRevision			=	1216;
char	g_strBuildDate[64]	=	{0,};

void	
SetVersion()
{
	sprintf(g_strBuildDate,__TIMESTAMP__);

}