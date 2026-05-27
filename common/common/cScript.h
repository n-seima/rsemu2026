#ifndef __CSCRIPT_H
#define __CSCRIPT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _WINDOWS_
#include <windows.h>
#endif


//////////////////////////////////////////////////////////
//	cScript : Text Script - 각 환경설정등등의 화일을 읽어올때 사용
//////////////////////////////////////////////////////////

#define dMAXSCPSIZE		100
#define dMAXNAMESIZE	512
#define dPARSECHAR		'='
#define dDATAPARSECHAR	'|'
#define dDATAENDCHAR	';'
#define dCOMMENTS		"//"
#define	dNOTFILE		-1	//	파일을 찾을수가 없다.
#define dENDMESSAGE		"END"

class SCP{
public:
	int		count;
	char	name[10][dMAXNAMESIZE];	//	name , data1,data2,...

	SCP()
	{
		count = 0;
	}
};

class cScript  
{
public:
	FILE	*fp;
	char	_readline[512];		//	한줄 읽어들여서 돌려줄...
	SCP		scp[dMAXSCPSIZE];
	int		totcount;			//	읽어들인 총 데이타 종류의 수
	int		scpcount;
	int		running;

	cScript();
	virtual ~cScript();

	int	Read(char	*filename);
	char*	getData(char *name,int count=1);	//	해당 값을 가져온다.
	int		getDataCount(char *name);

private:
	BOOL	readLine(FILE *f);
	BOOL	parseLine(char *_readline,int size);

	int		getTotSize(){	return totcount;	}

};

class cDynamicScript
{
public:
	FILE	*fp;
	char	_readline[512];		//	한줄 읽어들여서 돌려줄...
	SCP		* scp;
	int		totcount;			//	읽어들인 총 데이타 종류의 수
	int		scpcount;
	int		running;
	int		MAX;

	cDynamicScript();
	virtual ~cDynamicScript();

	int	Read(char	*filename,int _MAX);
	char*	getData(char *name,int count=1);	//	해당 값을 가져온다.
	int		getDataCount(char *name);
private:
	BOOL	readLine(FILE *f);
	BOOL	parseLine(char *_readline,int size);

	int		getTotSize(){	return totcount;	}
};

#endif //  __CSCRIPT_H
