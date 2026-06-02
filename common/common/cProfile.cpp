// cProfile.cpp: implementation of the cProfile class.
//
//////////////////////////////////////////////////////////////////////

#include "cProfile.h"

#include <mmsystem.h>
#include <time.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void	cProfileItem::Init(FILE *f,char *n,BOOL isCCount)
{
	strcpy(name,n);
	isCheckCount = isCCount;
	count = 0;
	fp = f;
}

void	cProfileItem::Begin()
{
	starttime = timeGetTime();
	if(isCheckCount)
		++count;
}

void	cProfileItem::End()
{
	//	자기 이름 / 걸린 시간 / 호출된 카운트
	fprintf(fp,"%s|%d\n",name,timeGetTime()-starttime);
}

cProfile::cProfile(char * n,char *path)
{
	fp = NULL;
	sprintf(name,"%s\\%s.prof",path,n);
	fp = fopen(name,"a+");
	if(fp==NULL)
	{
		printf("헉.. 파일을 만들지 못했습니다. \n");
		close();
	}

	time_t tTime;	struct tm *tmTime;
	tTime = time(NULL);	tmTime = localtime(&tTime);
	fprintf(fp,"- START %4d년%2d월%2d일 - %2d:%2d:%2d-----------------------\n",tmTime->tm_year+1900,tmTime->tm_mon+1,tmTime->tm_mday,tmTime->tm_hour,tmTime->tm_min,tmTime->tm_sec);
}

void	cProfile::close()
{
	time_t tTime;	struct tm *tmTime;
	tTime = time(NULL);	tmTime = localtime(&tTime);
	fprintf(fp,"- RESULT %4d년%2d월%2d일 - %2d:%2d:%2d-----------------------\n",tmTime->tm_year+1900,tmTime->tm_mon+1,tmTime->tm_mday,tmTime->tm_hour,tmTime->tm_min,tmTime->tm_sec);

	for(int i =0 ;i<dMAXPROFILE_COUNT; i++)
	{
		if(strcmp(pItem[i].GetName(),"")!=0)
			fprintf(fp,"%s|%d\n",pItem[i].GetName(),pItem[i].GetCount());
	}
	memset(&name,0,128);
	if(fp!=NULL)
	{
		fclose(fp);
		fp = NULL;
	}
}

void	cProfile::Begin(char *n)
{
	if(strcmp(name,"")==0)
		return;
	for(int i = 0; i < dMAXPROFILE_COUNT;i++)
		if(strcmp(pItem[i].GetName(),n)==0)
			break;
	if(i<dMAXPROFILE_COUNT)
		pItem[i].Begin();
	else{	
		//	새로 등록하고 시작한다.
		for(int j = 0; j < dMAXPROFILE_COUNT;j++)
			if(strcmp(pItem[j].GetName(),"")==0)
				break;
		if(j>=dMAXPROFILE_COUNT)
			return;
		pItem[j].Init(fp,n,TRUE);
		pItem[j].Begin();
	}
}

void	cProfile::End(char *n)
{
	if(strcmp(name,"")==0)
		return;
	for(int i = 0; i < dMAXPROFILE_COUNT;i++)
		if(strcmp(pItem[i].GetName(),n)==0)
			break;
	if(i<dMAXPROFILE_COUNT)
		pItem[i].End();
}
