#include	"stdafx.h"
#include	"./file.h"
#include	"lnk_common.h"
#include	<io.h>
#include	<stdarg.h>


enum
{
	eFileType_file,
	eFileType_folder,
	eFileType_dot,
};

BOOL	//	폴더를 만든다. 
CreateFolder(TCHAR *_lpstrFolder,BOOL _bIsChangeFolder)
{	
	int		pos	=	0,count	=	0;
	TCHAR	strCurDir[MAX_PATH],strChangeFolder[MAX_PATH];

	GetCurrentDirectory(512,strCurDir);

	while(1)
	{	
		strChangeFolder[pos]	=	_lpstrFolder[count];

		if (strChangeFolder[pos]==	NULL	||	strChangeFolder[pos]==	'\\')
		{	
			strChangeFolder[pos+1]=	NULL;
			if (SetCurrentDirectory(strChangeFolder)==0)
			{	
				CreateDirectory(strChangeFolder,NULL);

				if (SetCurrentDirectory(strChangeFolder)==0)
				{
					SetCurrentDirectory(strCurDir);
					return FALSE;
				}
			}

			if	(_lpstrFolder[count]==	NULL)
				break;
			pos	=	0;
		}
		else
			pos++;

		count++;
	}

	if	(_bIsChangeFolder)
		SetCurrentDirectory(_lpstrFolder);
	else
		SetCurrentDirectory(strCurDir);

	return TRUE;
}

BOOL
DeleteFolder(TCHAR *_lpstrDelDir,BOOL _bIsWantRemoveRoot)			//	화일을 포함한 폴더를 지운다.
{	
	TCHAR	strCurDir[ MAX_PATH];									// 재귀 호출을 사용한다.

	GetCurrentDirectory( MAX_PATH, strCurDir);

	if	(SetCurrentDirectory( _lpstrDelDir) == 0) 
	{	
		SetCurrentDirectory( strCurDir);
		return FALSE;
	}

	HANDLE				hFind;
	BOOL				bRet	=	TRUE;
	WIN32_FIND_DATA		fd;
	int					fc		=	0;

	hFind	=	FindFirstFile(L"*.*",&fd);

	while(hFind != INVALID_HANDLE_VALUE && bRet)
	{
		if	(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			if	(fd.cFileName[0]	!=	'.')
				DeleteFolder(fd.cFileName,TRUE);
		}
		else
		{
			SetFileAttributes(fd.cFileName,FILE_ATTRIBUTE_ARCHIVE);
			DeleteFile(fd.cFileName);
		}

		bRet=	FindNextFile(hFind,&fd);
	}

	FindClose(hFind);

	SetCurrentDirectory( strCurDir);

	if	(_bIsWantRemoveRoot)
	{
		if	(RemoveDirectory( _lpstrDelDir) == 0)
			return FALSE;
	}

	return TRUE;
}

inline	int
_getFileType(WIN32_FIND_DATA *fd)
{
	if	(fd->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
	{
		if	(fd->cFileName[0]	==	'.')
			return eFileType_dot;

		return	eFileType_folder;
	}

	return eFileType_file;
}


int
__GetFolderList(tstring _strFolder,std::vector<tstring> *_lpList,int *_lpiCount,int (*_lpfnOperateFilter)(BOOL,TCHAR*,void*),void *_lpData)
{
	HANDLE				hFind;
	BOOL				bRet		=	TRUE;
	WIN32_FIND_DATA		fd;
	tstring				strFilter	=	_strFolder+_T("\\*.*");

	hFind		=	FindFirstFile(strFilter.c_str(),&fd);

	while(hFind != INVALID_HANDLE_VALUE && bRet)
	{	
		if	(_getFileType(&fd) == eFileType_folder)
		{
			if	(!_lpfnOperateFilter || !_lpfnOperateFilter(TRUE,fd.cFileName,_lpData))
			{
				tstring	strFolderFullPath	=	_strFolder+_T("\\")+fd.cFileName;

				_lpList->push_back(strFolderFullPath.c_str());

				(*_lpiCount)++;

				__GetFolderList(strFolderFullPath,_lpList,_lpiCount,_lpfnOperateFilter,_lpData);
			}
		}

		bRet	=	FindNextFile(hFind,&fd);
	}

	FindClose(hFind);

	return	*_lpiCount;
}

int
GetFolderList(TCHAR *_lpstrRoot,std::vector<tstring> *_lpList,int (*_lpfnOperateFilter)(BOOL,TCHAR*,void*),void *_lpData)
{
	int		iFolderCount	=	0;
	tstring	strFolder		=	_lpstrRoot;

	__GetFolderList(strFolder,_lpList,&iFolderCount,_lpfnOperateFilter,_lpData);

	return	iFolderCount;
}

int
__GetFileList(tstring _strFolder,TCHAR *_lpstrFilter,std::vector<tstring> *_lpList,BOOL _bIsIncludeSubFolder,int *_lpiCount,int (*_lpfnOperateFilter)(BOOL,TCHAR*,void*),void *_lpData)
{
	HANDLE				hFind;
	BOOL				bRet		=	TRUE;
	WIN32_FIND_DATA		fd;
	tstring				strFilter	=	_strFolder+_T("\\")+_lpstrFilter;

	hFind		=	FindFirstFile(strFilter.c_str(),&fd);

	while(hFind != INVALID_HANDLE_VALUE && bRet)
	{	
		int	iFileType	=	_getFileType(&fd);

		if	(iFileType == eFileType_folder)
		{
			if	(_bIsIncludeSubFolder)
			{
				if	(!_lpfnOperateFilter || !_lpfnOperateFilter(TRUE,fd.cFileName,_lpData))
				{
					tstring	strFolderFullPath	=	_strFolder+_T("\\")+fd.cFileName;

					__GetFileList(strFolderFullPath,_lpstrFilter,_lpList,_bIsIncludeSubFolder,_lpiCount,_lpfnOperateFilter,_lpData);
				}
			}
		}
		else
		if	(iFileType == eFileType_file)
		{
			if	(!_lpfnOperateFilter || !_lpfnOperateFilter(FALSE,fd.cFileName,_lpData))
			{
				tstring	strFileName	=	_strFolder+_T("\\")+fd.cFileName;

				_lpList->push_back(strFileName.c_str());

				(*_lpiCount)++;
			}
		}

		bRet	=	FindNextFile(hFind,&fd);
	}

	FindClose(hFind);

	return	*_lpiCount;
}

int
GetFileList(TCHAR *_lpstrRoot,TCHAR *_lpstrFilter,std::vector<tstring> *_lpList,BOOL _bIsIncludeSubFolder,int (*_lpfnOperateFilter)(BOOL,TCHAR*,void*),void *_lpData)
{
	int		iFileCount	=	0;
	tstring	strFolder	=	_lpstrRoot;

	__GetFileList(strFolder,_lpstrFilter,_lpList,_bIsIncludeSubFolder,&iFileCount,_lpfnOperateFilter,_lpData);

	return	iFileCount;
}

bool
isFolder(WIN32_FIND_DATA *fd)
{
	if	(fd->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY && fd->cFileName[0] != '.')
		return true;

	return false;
}

bool
IsFolder(const TCHAR *fn)
{
	HANDLE				hFind;
	BOOL				bRet	=	TRUE;
	WIN32_FIND_DATA		fd;
	int					fc		=	0;

	hFind	=	FindFirstFile(fn,&fd);

	if	(hFind == INVALID_HANDLE_VALUE)
		return FALSE;

	FindClose(hFind);

	return isFolder(&fd);
}

bool
isFile(WIN32_FIND_DATA *fd)
{	
	if	(fd->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		return false;

	return true;
}

bool
IsFile(const TCHAR *fn)
{
	HANDLE				hFind;
	BOOL				bRet	=	TRUE;
	WIN32_FIND_DATA		fd;
	int					fc		=	0;

	hFind	=	FindFirstFile(fn,&fd);

	if	(hFind == INVALID_HANDLE_VALUE)
		return FALSE;

	FindClose(hFind);

	return isFile(&fd);
}

int		
GetFileList(std::vector<tstring> *_lpList,const TCHAR *_lpstrFullPathFilter)
{
	_lpList->clear();

	HANDLE				hFind;
	BOOL				bRet		=	TRUE;
	WIN32_FIND_DATA		fd;
	int					iFileCount	=	0;

	hFind	=	FindFirstFile(_lpstrFullPathFilter,&fd);

	if	(hFind == INVALID_HANDLE_VALUE)
		return	0;

	while(hFind != INVALID_HANDLE_VALUE && bRet)
	{
		if	(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			continue;

		_lpList->push_back(fd.cFileName);

		bRet	=	FindNextFile(hFind,&fd);

		iFileCount++;
	}

	FindClose(hFind);

	return	iFileCount;
}



//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

cFile::cFile()
{
	m_pFile			=	NULL;
	m_bfIsRef		=	FALSE;
	m_uiFrontPos	=	0;
	m_uiEndPos		=	0;
	m_uiLength		=	0;
	m_bIsMergeFile	=	false;

	memset(m_auiPushLocateInfo,0,sizeof(m_auiPushLocateInfo));
}

cFile::cFile(FILE *_lpFP)
{
	m_pFile			=	NULL;
	m_bfIsRef		=	FALSE;
	m_uiFrontPos	=	0;
	m_uiEndPos		=	0;
	m_uiLength		=	0;
	m_bIsMergeFile	=	false;

	memset(m_auiPushLocateInfo,0,sizeof(m_auiPushLocateInfo));

	set(_lpFP);
}

cFile::~cFile()
{	
	close();
}
	

bool cFile::open( const TCHAR* _strFullFileName, const TCHAR* _lpstrMode, BOOL _bIsFFlush )
{	
	close();

	{
		_tfopen_s( &m_pFile, _strFullFileName, _lpstrMode );

		if( !m_pFile )
			return false;

		//
		m_uiEndPos = cFile::GetSize(m_pFile);
		m_uiLength = m_uiEndPos;
		//m_uiDataSize = m_uiLength;		
	}

	// 
	m_bIsMergeFile = false;
	m_uiFrontPos = 0;
	m_bIsFFlush = (_bIsFFlush) ? true : false;

	return true;
}




bool cFile::close()
{
	if( !m_pFile )
		return false;

	{
		if( m_bIsMergeFile	|| m_bfIsRef )
			return true;

		::fclose( m_pFile );
	}

	m_pFile			=	NULL;
	m_uiFrontPos	=	0;
	m_uiEndPos		=	0;
	m_uiLength		=	0;
	m_bIsMergeFile	=	false;

	return true;
}

int cFile::read( void* _lpBuffer, UINT _uiSize, UINT _uiItemSize )
{
	if( !m_pFile )
		return 0;

	if( _uiSize*_uiItemSize <= 0 )
		return	0;

	return (int)::fread( _lpBuffer, _uiSize*_uiItemSize, 1, m_pFile );
}

int cFile::write( void* _lpBuffer, UINT _uiSize, UINT _uiItemSize )
{
	if( !m_pFile )
		return 0;

	if( _uiSize*_uiItemSize <= 0 )
		return 0;

	int iResult	= (int)::fwrite( _lpBuffer, _uiSize*_uiItemSize, 1, m_pFile );
	if( iResult )
	{
		if( m_bIsFFlush )
			::fflush( m_pFile );

		return iResult;
	}

	return 0;
}

int cFile::write( int _iLocate, void* _lpData, int _iSize, UINT _uiItemSize )
{
	int	iPos = getCurrentLocate();
	if( _iLocate == -1 )
		return write( _lpData, _iSize, _uiItemSize );

	seek( _iLocate, SEEK_SET );
	int ireturnValue = write( _lpData, _iSize, _uiItemSize );
	seek( iPos, SEEK_SET );

	return	ireturnValue;
}

bool cFile::seek( int _iPos, int _iSeekTerm )
{	
	if( !m_pFile )
		return false;

	if( m_bIsMergeFile )
	{
		switch( _iSeekTerm )
		{
		case SEEK_SET :
			if( ::fseek(m_pFile, _iPos+m_uiFrontPos, _iSeekTerm) )
				return false;
			break;

		case SEEK_CUR :
			if( ::fseek(m_pFile, _iPos, _iSeekTerm) )
				return false;
			break;

		case SEEK_END :
			if( ::fseek(m_pFile, _iPos+m_uiEndPos, SEEK_SET) )
				return false;
			break;
		}
	}
	else
	{
		if( ::fseek(m_pFile,_iPos,_iSeekTerm) )
			return false;
	}

	return false;
}

bool cFile::skip( int _iSkip )
{
	return seek( _iSkip, SEEK_CUR );
}

bool cFile::rewind()
{	
	return seek( m_uiFrontPos, SEEK_SET );
}

bool cFile::end()
{	
	return seek( m_uiEndPos, SEEK_SET );
}


UINT cFile::getCurrentLocate()
{
	if( !m_pFile )
		return 0;

	return ::ftell( m_pFile );
}

bool cFile::set( FILE* _pFile )
{
	m_pFile = _pFile;
	m_bIsMergeFile = false;

	if( m_pFile != NULL )
	{
		m_bfIsRef		=	TRUE;
		m_uiFrontPos	=	0;
		m_uiEndPos		=	cFile::GetSize(m_pFile);
		m_uiLength		=	m_uiEndPos;		

		return true;
	}
	else
	{
		m_uiFrontPos	=	0;
		m_uiEndPos		=	0;
		m_uiLength		=	0;

		return false;
	}

	return false;
}

bool cFile::set( cFile *_lpFile, UINT _uiSize )
{
	m_pFile			=	_lpFile->m_pFile;
	m_uiLength		=	_uiSize;
	m_uiFrontPos	=	::ftell(_lpFile->m_pFile);
	m_uiEndPos		=	m_uiFrontPos+_uiSize;
	m_bfIsRef		=	TRUE;

	return true;
}



bool cFile::skipStringAndSize()
{
	if( !m_pFile )
		return false;
	{
		int	iLen;
		::fread( &iLen, 1, 4, m_pFile );

		if( iLen )
			skip( iLen*sizeof(TCHAR) );	//	널문자 스킵
	}

	return	true;
}

bool cFile::writeString(const TCHAR *_lpstrText,...)
{
	if	(!m_pFile)
		return false;

	TCHAR	strMessage[1024];
	char	strMessageBuffer[1024];

	va_list		argptr;

	va_start(argptr, _lpstrText);
	_vstprintf_s(strMessage,_lpstrText,argptr);
	va_end(argptr);

	if	(!U2A(strMessage,strMessageBuffer,1024))
		return	false;

	fprintf(m_pFile,strMessageBuffer);

	if	(m_bIsFFlush)
		fflush(m_pFile);

	return true;
}

bool cFile::writeStringNP(const TCHAR *_lpstrText)
{
	if	(!m_pFile)
		return false;

	char	strMessageBuffer[1024];
	if	(!U2A(_lpstrText,strMessageBuffer,1024))
		return	false;

	fprintf(m_pFile,strMessageBuffer);

	if	(m_bIsFFlush)
		fflush(m_pFile);

	return true;
}

bool
cFile::writeVecInt(VecInt *_lpVecInt)
{
	if	(!m_pFile)
		return false;

	int	iSize	=	(int)_lpVecInt->size();

	write(&iSize,sizeof(iSize));

	IterInt	iterValue	=	_lpVecInt->begin();

	for (;iterValue!=_lpVecInt->end();iterValue++)
		write(&*iterValue,sizeof(int));

	return	true;
}

//	readStringAndSize과 쌍이다.
//	문자열을 널문자 포함해서 저장 시킨다.
bool cFile::writeStringAndSize(const TCHAR *_lpstrText)
{

	if	(!m_pFile)
		return false;

	int	iLen	=	0;

	if	(_lpstrText && STRLEN(_lpstrText))
		iLen	=	(int)STRLEN(_lpstrText)+1;

	fwrite(&iLen,1,4,m_pFile);

	if	(iLen)
		fwrite(_lpstrText,iLen*sizeof(TCHAR),1,m_pFile);

	if	(m_bIsFFlush)
		fflush(m_pFile);

	return	true;
}


bool
cFile::readVecInt(VecInt *_lpVecInt)
{
	if	(!m_pFile)
		return false;

	_lpVecInt->clear();

	{
		int	iCount;

		::fread(&iCount,1,4,m_pFile);

		for(int i=0;i<iCount;i++)
		{
			int	iValue;

			::fread(&iValue,1,4,m_pFile);

			_lpVecInt->push_back(iValue);
		}
	}

	return	true;
}

//	writeStringAndSize과 쌍이다.
bool cFile::readStringAndSize( tstring* _lpstrString )
{
	if	(!m_pFile)
		return false;

	{
		int	iLen;

		::fread(&iLen,1,4,m_pFile);

		if	(iLen)
		{
			_lpstrString->resize(iLen-1);	//	NULL 문자제외하고 읽음

			::fread(&(*_lpstrString)[0],(iLen-1)*sizeof(TCHAR),1,m_pFile);
			skip(sizeof(TCHAR));	//	널문자 스킵
		}
		else
		{
			*_lpstrString	=	_T("");
		}
	}

	return	true;
}

void cFile::pushCurrentLocate(int _iSlot)
{
	m_auiPushLocateInfo[_iSlot]	=	getCurrentLocate();
}

bool cFile::popCurrentLocate(int _iSlot)
{
	if	(_iSlot	>=	c_iPushFileLocateInfoCount)
		return	false;

	seek(m_auiPushLocateInfo[_iSlot],SEEK_SET);

	return	true;
}

bool cFile::writeCurrentPos(int _iLocate)
{
	int	iPos	=	getCurrentLocate();
	if	(_iLocate	==	-1)
	{
		write(&iPos,4);
		return	true;
	}

	seek(_iLocate,SEEK_SET);
	write(&iPos,4);
	seek(iPos,SEEK_SET);

	return	true;
}



//	정적 멤버
UINT cFile::GetSize(const TCHAR *_lpstrFn)
{
//	assert( !m_bUsingPacker && _T("지원 안 함") );

	FILE *tfp;

	fopen_s(&tfp,IU2A(_lpstrFn),"rb");

	if	(!tfp)
		return 0;

	UINT size = _filelength(_fileno(tfp));

	::fclose(tfp);

	return size;
}

UINT cFile::GetSize(FILE *_lpFP)
{	
	//assert( !m_bUsingPacker && _T("지원 안 함") );

	return _filelength(_fileno(_lpFP));
}

//	_lpOrgFile을 안전하게 저장하기 위해 사용
TCHAR* cFile::ReadyToSafeSave(const TCHAR *_lpOrgFile)
{
//	assert( !m_bUsingPacker && _T("지원 안 함") );

	static	TCHAR		strTempFileName[512];
	TCHAR	strBackupTempFileName[512];

	SPRINTF(strBackupTempFileName,_T("%s_backup"),_lpOrgFile);

	SetFileAttributes(strBackupTempFileName,FILE_ATTRIBUTE_ARCHIVE);

	CopyFile(strBackupTempFileName,_lpOrgFile,FALSE);	//	기존 파일 백업

	SPRINTF(strTempFileName,_T("%s_temp"),_lpOrgFile);
	SetFileAttributes(strTempFileName,FILE_ATTRIBUTE_ARCHIVE);

	return	strTempFileName;
}

bool cFile::EndSafeSave(const TCHAR *_lpOrgFile)
{
//	assert( !m_bUsingPacker && _T("지원 안 함") );


	TCHAR	strTempFileName[512];
	TCHAR	strBackupTempFileName[512];
	SPRINTF(strTempFileName,_T("%s_temp"),_lpOrgFile);
	SPRINTF(strBackupTempFileName,_T("%s_backup"),_lpOrgFile);

	SetFileAttributes(_lpOrgFile,FILE_ATTRIBUTE_ARCHIVE);
	CopyFile(strTempFileName,_lpOrgFile,FALSE);
	DeleteFile(strBackupTempFileName);
	DeleteFile(strTempFileName);

	return	true;
}




#ifdef	WIN32

bool cFile::SaveBox(TCHAR *fname,TCHAR *filter,TCHAR *defaultExt,TCHAR *folder,TCHAR *title,UINT _uiStyle)
{
	OPENFILENAME	ofn;

	TCHAR	dir[512];

	if	(!folder)
		GetCurrentDirectory(511,dir);
	else
		_tcscpy_s(dir,folder);

	memset(&ofn,0,sizeof(OPENFILENAME));

	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	GetActiveWindow();
	ofn.lpstrFilter		=	filter;
	ofn.lpstrTitle		=	title;
	ofn.lpstrFile		=	fname;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLESIZING | _uiStyle;
	ofn.lpstrDefExt		=	defaultExt;
	ofn.lpstrInitialDir	=	dir;

	if	(!GetSaveFileName(&ofn)) 
		return false;

	return true;
}

bool cFile::LoadBox(TCHAR *fname,TCHAR *filter,TCHAR *defaultExt,TCHAR *folder,TCHAR *title,UINT _uiStyle)
{
	OPENFILENAME	ofn;

	TCHAR	dir[512];

	if	(!folder)
		GetCurrentDirectory(511,dir);
	else
		_tcscpy_s(dir,folder);

	memset(&ofn,0,sizeof(OPENFILENAME));
	ofn.lStructSize		=	sizeof(OPENFILENAME);
	ofn.hwndOwner		=	GetActiveWindow();
	ofn.lpstrFilter		=	filter;
	ofn.lpstrTitle		=	title;
	ofn.lpstrFile		=	fname;
	ofn.nMaxFile		=	256;
	ofn.Flags			=	OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLESIZING | _uiStyle;
	ofn.lpstrDefExt		=	defaultExt;
	ofn.lpstrInitialDir	=	dir;

	if	(!GetOpenFileName(&ofn))
		return false;

	return true;
}


#endif