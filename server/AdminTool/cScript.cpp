// cScript.cpp: implementation of the cScript class.
//
//////////////////////////////////////////////////////////////////////

#include "cScript.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cScript::cScript()
{

}

cScript::~cScript()
{

}

int cScript::Read(char	*filename)
{
	//	초기화 
	memset(scp,0,sizeof(SCP)*dMAXSCPSIZE);
	if( (fp = fopen( filename, "r" )) == NULL )
	{
		MessageBox(NULL,filename,"NOT FOUND FILE",MB_OK);
		return dNOTFILE;
	}
	scpcount=0;
	totcount=0;

	while(TRUE)
	{
		if(readLine(fp)!=TRUE)
		{
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return 0;
}

BOOL	cScript::readLine(FILE *f)
{
	int		tmp=0;
	int		count=0;

	memset(_readline,0,sizeof(_readline));

	tmp = fgetc(f);
	while(tmp!=EOF && tmp!=10)
	{
		_readline[count++] = (char)tmp;
		tmp = fgetc(f);
	}
	_readline[count]=0;
	if(tmp==EOF)	return FALSE;
	if( count==0)	return TRUE;

	return parseLine(_readline, count);
}

BOOL	cScript::parseLine(char *line,int size)
{
	int		parse		=	0;
	int		cnt			=	0;
	int		count		=	0;
	char	tmp[dMAXNAMESIZE];			//	임시 저장 공간.
	tmp[0]				=	0;
	BOOL	isTXT		=	FALSE;
	BOOL	isData		=	FALSE;
	//	이름...
	memset(tmp,0,dMAXNAMESIZE);
	for(count=0;count<size; count++)
	{
		//	지나쳐야 할 것들 
		if(!isTXT && line[count]==' ' || line[count]==9 || line[count]==13)
		{
			continue;
		}

		//	특수 기호들
		if(!isTXT && line[count]=='/')
		{
			if( line[count-1]=='/')
			{
				return TRUE;
			}
		}
		if(line[count]=='\"')
		{
			if(isTXT)	isTXT = FALSE;
			else		isTXT = TRUE;
		}

		if(line[count]==dDATAENDCHAR || line[count]==dPARSECHAR || line[count]==dDATAPARSECHAR)
		{
			if(tmp[0]!=0)
			{
				memset(&scp[scpcount].name[parse],0,sizeof(scp[scpcount].name[parse]));
				memcpy(scp[scpcount].name[parse],tmp,cnt);
				scp[scpcount].name[parse][cnt] = 0;
				scp[scpcount].count++;
				++parse;
				cnt			=	0;
				tmp[0]		=	0;
				if(line[count]==dDATAENDCHAR)
				{
					++totcount;
					++scpcount;
					parse=0;
					return TRUE;
				}
				continue;
			}else{
				return TRUE;
			}
		}
		if(line[count]==EOF)
			return FALSE;
		if(line[count]=='\\')
		{
			tmp[cnt++] = '\r';
			tmp[cnt++] = '\n';
		}else if(line[count]=='\"')
		{
			//	텍스트에 안넣어 버린다.
		}
		else
			tmp[cnt++] = line[count];
		isData = TRUE;
	}
	if(isData)	return FALSE;
	else		return TRUE;
}

char*	cScript::getData(char *name,int count)
{
	static char	tt[10];

	tt[0] = 0;

	int i=0;

	for(i=0;i<totcount;i++)
	{	
		if( stricmp(scp[i].name[0],name)==0)		
			break;	
	}

	if(i>=totcount)		
		return tt;

	return scp[i].name[count];
}

int		cScript::getDataCount(char *name)
{
	for(int i=0;i<totcount;i++)
	{	
		if( stricmp(scp[i].name[0],name)==0)		
			break;	
	}
	
	if(i>=totcount)		
		return 0;
	
	return scp[i].count;
}

/****************************************************************
	동적할당 가능한 Script 읽기
****************************************************************/

cDynamicScript::cDynamicScript()
{
	scp = NULL;
}

cDynamicScript::~cDynamicScript()
{
	if(scp)
	{
		delete [] scp;
		scp = NULL;
	}
}

int cDynamicScript::Read(char	*filename,int _MAX)
{
	MAX = _MAX+4;
	//	초기화 
	if(scp)
	{
		delete [] scp;
		scp = NULL;
	}
	scp = new SCP[MAX];
	memset(scp,0,sizeof(SCP)*MAX);
	if( (fp = fopen( filename, "r" )) == NULL )
	{
		MessageBox(NULL,filename,"NOT FOUND FILE",MB_OK);
		return dNOTFILE;
	}
	scpcount=0;
	totcount=0;

	while(TRUE)
	{
		if(readLine(fp)!=TRUE)
		{
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return 0;
}

BOOL	cDynamicScript::readLine(FILE *f)
{
	int		tmp=0;
	int		count=0;

	memset(_readline,0,sizeof(_readline));

	tmp = fgetc(f);
	while(tmp!=EOF && tmp!=10)
	{
		_readline[count++] = (char)tmp;
		tmp = fgetc(f);
	}
	_readline[count]=0;
	if(tmp==EOF)	return FALSE;
	if( count==0)	return TRUE;

	return parseLine(_readline, count);
}

BOOL	cDynamicScript::parseLine(char *line,int size)
{
	int		parse		=	0;
	int		cnt			=	0;
	int		count		=	0;
	char	tmp[dMAXNAMESIZE];			//	임시 저장 공간.
	tmp[0]				=	0;
	BOOL	isTXT		=	FALSE;
	BOOL	isData		=	FALSE;
	//	이름...
	memset(tmp,0,dMAXNAMESIZE);
	for(count=0;count<size; count++)
	{
		//	지나쳐야 할 것들 
		if(!isTXT && line[count]==' ' || line[count]==9 || line[count]==13)
		{
			continue;
		}

		//	특수 기호들
		if(!isTXT && line[count]=='/')
		{
			if( line[count-1]=='/')
			{
				return TRUE;
			}
		}
		if(line[count]=='\"')
		{
			if(isTXT)	isTXT = FALSE;
			else		isTXT = TRUE;
		}

		if(line[count]==dDATAENDCHAR || line[count]==dPARSECHAR || line[count]==dDATAPARSECHAR)
		{
			if(tmp[0]!=0)
			{
				memset(&scp[scpcount].name[parse],0,sizeof(scp[scpcount].name[parse]));
				memcpy(scp[scpcount].name[parse],tmp,cnt);
				scp[scpcount].name[parse][cnt] = 0;
				scp[scpcount].count++;
				++parse;
				cnt			=	0;
				tmp[0]		=	0;
				if(line[count]==dDATAENDCHAR)
				{
					++totcount;
					++scpcount;
					parse=0;
					return TRUE;
				}
				continue;
			}else{
				return TRUE;
			}
		}
		if(line[count]==EOF)
			return FALSE;
		if(line[count]=='\\')
		{
			tmp[cnt++] = '\r';
			tmp[cnt++] = '\n';
		}else if(line[count]=='\"')
		{
			//	텍스트에 안넣어 버린다.
		}
		else
			tmp[cnt++] = line[count];
		isData = TRUE;
	}
	if(isData)	return FALSE;
	else		return TRUE;
}

char*	cDynamicScript::getData(char *name,int count)
{
	static char	tt[10];

	tt[0] = 0;

	int i=0;

	for(i=0;i<totcount;i++)
	{	
		if( stricmp(scp[i].name[0],name)==0)		
			break;	
	}

	if(i>=totcount)		
		return tt;

	return scp[i].name[count];
}

int		cDynamicScript::getDataCount(char *name)
{
	for(int i=0;i<totcount;i++)
	{	
		if( stricmp(scp[i].name[0],name)==0)		
			break;	
	}
	
	if(i>=totcount)		
		return 0;
	
	return scp[i].count;
}