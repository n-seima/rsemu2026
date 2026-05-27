#include "CBGPlay.h"
#include <tchar.h>
#include <malloc.h>

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

CBGPlay::CBGPlay()
{
	m_pGraphBuilder		=	NULL;
	m_pMediaControl		=	NULL;
	m_pMediaSeeking		=	NULL;
	m_pAudio			=	NULL;
	m_IsFileSet			=	false;
	m_iVolume			=	1000;
}

CBGPlay::~CBGPlay()
{
	Destroy();
}

void CBGPlay::Destroy()
{
	if(m_pMediaControl)	m_pMediaControl->Stop();
	SAFE_RELEASE(m_pAudio);
    SAFE_RELEASE(m_pMediaSeeking);
    SAFE_RELEASE(m_pMediaControl);
    SAFE_RELEASE(m_pGraphBuilder);
	m_IsFileSet = false;
}

HRESULT CBGPlay::Create(char *SoundFile)
{
	Destroy();

	HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, reinterpret_cast<void **>(&m_pGraphBuilder));

    if(FAILED(hr))    return hr;

    hr = m_pGraphBuilder->QueryInterface(IID_IMediaControl, reinterpret_cast<void **>(&m_pMediaControl));

	if(FAILED(hr))    return hr;

    hr = m_pGraphBuilder->QueryInterface(IID_IMediaSeeking, reinterpret_cast<void **>(&m_pMediaSeeking));

	if(FAILED(hr))    return hr;

	return SetSoundFile(SoundFile);
}

HRESULT CBGPlay::SetSoundFile(char *pstrFiles)
{
	if( (!m_pGraphBuilder) || (!m_pMediaControl) || (!m_pMediaSeeking) )
		return E_FAIL;

    IPin *pPin = NULL;
	IBaseFilter *pSourceNext = NULL;

    TCHAR szFilename[MAX_PATH];
    WCHAR wFileName[MAX_PATH];

    _tcscpy(szFilename, pstrFiles);

    if((DWORD)GetFileAttributes(szFilename) == (DWORD)-1)
		return ERROR_FILE_NOT_FOUND;

	MultiByteToWideChar(CP_ACP, 0, szFilename, -1, wFileName, MAX_PATH);

    HRESULT hr = m_pGraphBuilder->AddSourceFilter(wFileName, wFileName, &pSourceNext);

    if(SUCCEEDED(hr))	hr = pSourceNext->FindPin(L"Output", &pPin);

    if(SUCCEEDED(hr))   hr = m_pMediaControl->Stop();
    if(SUCCEEDED(hr))	hr = m_pGraphBuilder->Render(pPin);

    SAFE_RELEASE(pPin);
    SAFE_RELEASE(pSourceNext);

    if(SUCCEEDED(hr))
	{
        LONGLONG llPos = 0;
        hr = m_pMediaSeeking->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning, &llPos, AM_SEEKING_NoPositioning);
    }
	
	if(SUCCEEDED(hr))
		hr = m_pGraphBuilder->QueryInterface(IID_IBasicAudio, reinterpret_cast<void **>(&m_pAudio));

	m_IsFileSet = SUCCEEDED(hr);

    return S_OK;
}

BOOL
CBGPlay::PlayPosition(LONGLONG llPos)
{

	if(!m_IsFileSet)	return FALSE;

    m_pMediaSeeking->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning, &llPos, AM_SEEKING_NoPositioning);

	if (SUCCEEDED(m_pMediaControl->Run()))	return	TRUE;

	return	FALSE;
}

BOOL
CBGPlay::Play(char *fn,BOOL _bLoop)
{
	m_bLoop	=	_bLoop;

	if (Create(fn)	!=	S_OK)	return	FALSE;

	put_Volume(m_iVolume);

	if(!m_IsFileSet)	return FALSE;

	LONGLONG llPos	=	0;

    m_pMediaSeeking->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning, &llPos, AM_SEEKING_NoPositioning);

	if (SUCCEEDED(m_pMediaControl->Run()))	return	TRUE;

	return	FALSE;
}

HRESULT CBGPlay::Stop()
{
	if(!m_IsFileSet)	return E_FAIL;

	m_IsFileSet	=	FALSE;

	return m_pMediaControl->Stop();
}

BOOL CBGPlay::IsEndPosition()
{
	if(!m_IsFileSet)	return false;

	LONGLONG Current, Stop;

	m_pMediaSeeking->GetPositions(&Current, &Stop);

	return (Current>=Stop);
}

BOOL CBGPlay::IsPlaying()
{
	if(!m_IsFileSet)	return FALSE;

	if (!IsEndPosition())	return	TRUE;

	return	FALSE;
}

HRESULT CBGPlay::get_Balance(long *plBalance)
{
	if(!m_pAudio)	return E_FAIL;
	return m_pAudio->get_Balance(plBalance);
}

HRESULT CBGPlay::get_Volume(long *plVolume)
{
	if(!m_pAudio)	return E_FAIL;
	return m_pAudio->get_Volume(plVolume);
}

HRESULT CBGPlay::put_Balance(long lBalance)
{
	if(!m_pAudio)	return E_FAIL;
	return m_pAudio->put_Balance(lBalance);
}

HRESULT CBGPlay::put_Volume(long lVolume)
{
	lVolume			=	max(min(lVolume,1000),0);

	m_iVolume		=	lVolume;

	if(!m_pAudio)	return E_FAIL;

	return m_pAudio->put_Volume(DSBVOLUME_MIN + lVolume*2 + 8000);
}