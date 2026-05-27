#include	"./file.h"
#include	<io.h>
#include	<stdarg.h>

cFile::cFile()
{
	m_pFile			=	NULL;
	m_u4FrontPos	=	0;
	m_u4EndPos		=	0;
	m_u4Length		=	0;
	m_bIsMergeFile	=	false;
}

cFile::~cFile()
{	
	close();
}

bool
cFile::open(char *_lpstrFn,char *_lpstrMode,bool _bIsFFlush)
{	
	close();

	m_pFile			=	fopen(_lpstrFn,_lpstrMode);
	
	if	(!m_pFile)
		return false;

	m_bIsMergeFile	=	false;
	m_u4FrontPos	=	0;
	m_u4EndPos		=	cFile::GetSize(m_pFile);
	m_u4Length		=	m_u4EndPos;
	m_bIsFFlush		=	_bIsFFlush;

	return true;
}

bool
cFile::close()
{	
	if	(!m_pFile		)
		return false;
	if	(m_bIsMergeFile	)
		return true;

	fclose(m_pFile);

	m_pFile			=	NULL;
	m_u4FrontPos	=	0;
	m_u4EndPos		=	0;
	m_u4Length		=	0;
	m_bIsMergeFile	=	false;

	return true;
}

bool
cFile::seek(s4 _s4Pos,s4 _s4SeekTerm)
{	
	if	(!m_pFile)
		return false;

	if (m_bIsMergeFile)
	{
		switch(_s4SeekTerm)
		{
			case	SEEK_SET	:
				if	(fseek(m_pFile,_s4Pos+m_u4FrontPos,_s4SeekTerm))
					return false;
				break;

			case	SEEK_CUR	:
				if	(fseek(m_pFile,_s4Pos,_s4SeekTerm))
					return false;
				break;

			case	SEEK_END	:
				if	(fseek(m_pFile,_s4Pos+m_u4EndPos,SEEK_SET))
					return false;
				break;
		}
	}
	else
	{
		if	(fseek(m_pFile,_s4Pos,_s4SeekTerm))
			return false;
	}

	return true;
}

u4
cFile::getCurrentLocate()
{	
	return ftell(m_pFile);
}

s4
cFile::read(void *buffer,u4 size,u4 itemSize)
{
	if	(!m_pFile)
		return 0;

	if	(size*itemSize	<=	0)
		return	0;

	return	fread(buffer,size*itemSize,1,m_pFile);
}

s4
cFile::write(void *buffer,u4 size,u4 itemSize)
{	
	if	(!m_pFile)
		return 0;

	if	(size*itemSize	<=	0)
		return	0;

	s4	iResult	=	fwrite(buffer,size*itemSize,1,m_pFile);

	if	(iResult)
	{
		if	(m_bIsFFlush)
			fflush(m_pFile);

		return	iResult;
	}

	return 0;
}

bool
cFile::writeString(char *_lpstrText,...)
{
	if	(!m_pFile)
		return false;

	static	char	strMessage[1024];

	va_list		argptr;

	va_start(argptr, _lpstrText);
	vsprintf(strMessage,_lpstrText,argptr);
	va_end(argptr);

	fprintf(m_pFile,strMessage);

	if	(m_bIsFFlush)
		fflush(m_pFile);

	return true;
}

bool
cFile::writeStringNP(char *_lpstrText)
{
	if	(!m_pFile)
		return false;

	fprintf(m_pFile,_lpstrText);

	if	(m_bIsFFlush)
		fflush(m_pFile);

	return true;
}

bool
cFile::rewind()
{	
	return seek(m_u4FrontPos,SEEK_SET);
}

bool
cFile::end()
{	
	return seek(m_u4EndPos,SEEK_SET);
}

bool
cFile::skip(s4 skip)
{	
	return	seek(skip,SEEK_CUR);
}

bool
cFile::set(cFile *_lpFile,u4 _u4Size)
{
	m_pFile			=	_lpFile->m_pFile;
	m_u4Length		=	_u4Size;
	m_u4FrontPos	=	ftell(_lpFile->m_pFile);
	m_u4EndPos		=	m_u4FrontPos+_u4Size;

	return true;
}

//	¤ ¸¹
u4
cFile::GetSize(char *_lpstrFn)
{
	FILE *tfp;

	tfp	=	fopen(_lpstrFn,"rb");

	if	(!tfp)
		return 0;

	u4 size = filelength(fileno(tfp));

	fclose(tfp);

	return size;
}

u4
cFile::GetSize(FILE *_lpFP)
{	
	return filelength(fileno(_lpFP));
}

#ifdef	WIN32

bool
cFile::SaveBox(char *fname,char *filter,char *defaultExt,char *folder,char *title,u4 _u4Style)
{
	OPENFILENAME	ofn;

	char	dir[512];

	if	(!folder)
		GetCurrentDirectory(511,dir);
	else
		strcpy(dir,folder);

	memset(&ofn,0,sizeof(OPENFILENAME));

	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	GetActiveWindow();
	ofn.lpstrFilter		=	filter;
	ofn.lpstrTitle		=	title;
	ofn.lpstrFile		=	fname;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLESIZING | _u4Style;
	ofn.lpstrDefExt		=	defaultExt;
	ofn.lpstrInitialDir	=	dir;

	if	(!GetSaveFileName(&ofn)) 
		return false;

	return true;
}

bool
cFile::LoadBox(char *fname,char *filter,char *defaultExt,char *folder,char *title,u4 _u4Style)
{	
	OPENFILENAME	ofn;

	char	dir[512];

	if	(!folder)
		GetCurrentDirectory(511,dir);
	else
		strcpy(dir,folder);

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	GetActiveWindow();
	ofn.lpstrFilter		=	filter;
	ofn.lpstrTitle		=	title;
	ofn.lpstrFile		=	fname;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLESIZING | _u4Style;
	ofn.lpstrDefExt		=	defaultExt;
	ofn.lpstrInitialDir	=	dir;

	if	(!GetOpenFileName(&ofn))
		return false;

	return true;
}

#endif