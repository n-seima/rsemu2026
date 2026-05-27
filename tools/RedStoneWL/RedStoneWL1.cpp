// RedStoneWL1.cpp : Implementation of CRedStoneWL

#include "stdafx.h"
#include "RedStoneWL.h"
#include "RedStoneWL1.h"

/////////////////////////////////////////////////////////////////////////////
// CRedStoneWL


STDMETHODIMP CRedStoneWL::get_getID(BSTR *pVal)
{
	// TODO: Add your implementation code here

	*pVal	=	m_bstrId.Copy();

	return S_OK;
}

STDMETHODIMP CRedStoneWL::put_getID(BSTR newVal)
{
	// TODO: Add your implementation code here

	m_bstrId	= newVal; 

	return S_OK;
}

STDMETHODIMP CRedStoneWL::get_UrlPath(BSTR *pVal)
{
	// TODO: Add your implementation code here

	*pVal	=	m_bstrUrlPath.Copy();

	return S_OK;
}

STDMETHODIMP CRedStoneWL::put_UrlPath(BSTR newVal)
{
	// TODO: Add your implementation code here

	m_bstrUrlPath	= newVal; 

	return S_OK;
}

STDMETHODIMP CRedStoneWL::get_PortalID(long *pVal)
{
	*pVal		=	m_iPortalID;
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CRedStoneWL::put_PortalID(long newVal)
{
	m_iPortalID	=	newVal;
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CRedStoneWL::get_Password(BSTR *pVal)
{
	*pVal			=	m_bstrPassword; 

	return S_OK;
}

STDMETHODIMP CRedStoneWL::put_Password(BSTR newVal)
{
	m_bstrPassword	= newVal; 
	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CRedStoneWL::Run()
{
	char	strUserId[ 64]		=	{0,};		//	사용자 ID
	char	strPassword[ 64]	=	{0,};		//	사용자 비번
	char	strUrlPath[ 512]	=	{0,};		//	설치본 패스
	char	parStr[ 512];

	::WideCharToMultiByte( CP_ACP, WC_DEFAULTCHAR | WC_COMPOSITECHECK, m_bstrId.operator BSTR()			,-1	, strUserId		,64		,NULL	,NULL);
	::WideCharToMultiByte( CP_ACP, WC_DEFAULTCHAR | WC_COMPOSITECHECK, m_bstrPassword.operator BSTR()	,-1	, strPassword	,64		,NULL	,NULL);
	::WideCharToMultiByte( CP_ACP, WC_DEFAULTCHAR | WC_COMPOSITECHECK, m_bstrUrlPath.operator BSTR()	,-1	, strUrlPath	,512	,NULL	,NULL);

	if	(m_iPortalID	!=	ePI_RedStone)
	{
		if	( ::strlen(strUserId)		==	0)
		{
			::MessageBox(NULL, "실행 인자가 부족합니다", "에러", MB_OK);
			return S_OK;
		}
		if	( ::strlen(strPassword)		==	0)
		{
			::MessageBox(NULL, "실행 인자가 부족합니다", "에러", MB_OK);
			return S_OK;
		}
	}

	if	( ::strlen(strUrlPath)	==	0)
	{
		::MessageBox(NULL, "실행 인자가 부족합니다", "에러", MB_OK);
		return S_OK;
	}

	switch(m_iPortalID)
	{
		case	ePI_RedStone	:
			::wsprintf( parStr, "begin_rs_parameter%03d%s",strlen(strUrlPath),strUrlPath);
			break;
// 		case	ePI_Yahoo		:
// 			::wsprintf( parStr, "begin_rs_parameter%03d%s%03d%s%03d%sisYahoo1",strlen(strUserId),strUserId,strlen(strPassword),strPassword,strlen(strUrlPath),strUrlPath);
// 			break;
// 		case	ePI_Paran		:
// 			::wsprintf( parStr, "begin_rs_parameter%03d%s%03d%s%03d%sisParan",strlen(strUserId),strUserId,strlen(strPassword),strPassword,strlen(strUrlPath),strUrlPath);
// 			break;
		case	ePI_AnycallLand	:
			::wsprintf( parStr, "begin_rs_parameter%03d%s%03d%s%03d%sisAnycall",strlen(strUserId),strUserId,strlen(strPassword),strPassword,strlen(strUrlPath),strUrlPath);
			break;
		case	ePI_PlayNet		:
			::wsprintf( parStr, "begin_rs_parameter%03d%s%03d%s%03d%sisPlayNet",strlen(strUserId),strUserId,strlen(strPassword),strPassword,strlen(strUrlPath),strUrlPath);
			break;
	}

	char	fileNameBuf[ 512];
	fileNameBuf[ 0] = 0;
	GetWindowsDirectory( fileNameBuf, 512);

#ifdef	_FOR_INTER
	::ShellExecute( NULL, "open", "redstoneWebInstaller_I.exe", parStr, fileNameBuf, SW_SHOWNORMAL);
#else
	::ShellExecute( NULL, "open", "redstoneWebInstaller.exe", parStr, fileNameBuf, SW_SHOWNORMAL);
#endif

	return S_OK;
}