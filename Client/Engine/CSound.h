#ifndef SOUND_HEADER
#define SOUND_HEADER

/*

	sound.h

	sound °´¼.



				Fuzzface in 2003.

*/

#include <dsound.h>

#include "ogg/codec.h"
#include "ogg/vorbisfile.h"

#define SOUND_ERR_OK					0
#define SOUND_ERR_CANNOT_CREATE_DS		-1
#define SOUND_ERR_CANNOT_FIND_DS		-2
#define SOUND_ERR_CANNOT_SET_LEVEL		-3
#define SOUND_ERR_NO_FILE				-4
#define SOUND_ERR_OUT_OF_LIMIT			-5
#define SOUND_ERR_OUT_OF_INDEX			-6
#define SOUND_ERR_OUT_OF_COPY_INDEX		-7		// no use
#define SOUND_ERR_INDEX_COLLISION		-8
#define SOUND_ERR_NOT_READABLE			-9
#define SOUND_ERR_CANNOT_CREATE_BUFFER	-10
#define SOUND_ERR_CANNOT_DELETE_BUFFER	-11
#define SOUND_ERR_BUFFER_LOST			-12
#define SOUND_ERR_CANNOT_UNLOCK			-13
#define SOUND_ERR_CANNOT_DUPLICATE		-14
#define SOUND_ERR_CANNOT_SET_POSITION	-15
#define SOUND_ERR_CANNOT_PLAY			-16
#define SOUND_ERR_CANNOT_STOP			-17
#define SOUND_ERR_CANNOT_SET_PAN		-18
#define SOUND_ERR_CANNOT_SET_VOLUME		-19
#define SOUND_ERR_CANNOT_GET_INTERFACE	-20
#define SOUND_ERR_INVAILD_PAN			-21
#define SOUND_ERR_INVAILD_VOLUME		-22
#define SOUND_ERR_CANNOT_CREATE_STREAM	-23
#define SOUND_ERR_CANNOT_OPEN_STREAM	-24
#define SOUND_ERR_CANNOT_FIND_FOURCC	-25
#define SOUND_ERR_INVAILD_FILE_TYPE		-26
#define SOUND_ERR_CANNOT_OPEN_OGG		-27
#define SOUND_ERR_CANNOT_READ_OGG_INFO	-28
#define SOUND_ERR_CANNOT_CREATE_THREAD	-29
#define SOUND_ERR_CANNOT_CREATE_NOTIFY	-30
#define SOUND_ERR_CANNOT_MOVE			-31
#define SOUND_ERR_CANNOT_SET_FORMAT		-32
#define SOUND_ERR_NULL_BUFFER			-33

#define SOUND_RESTORE_MAX		3
#define SOUND_STREAM_POINT_MAX	4

#define SOUND_MAX				20
#define SOUND_COPY_MAX			1

#define SOUND_8BIT				8
#define SOUND_16BIT				16

#define SOUND_11K				11025
#define SOUND_22K				22050
#define SOUND_44K				44100

#define SOUND_MONO				1
#define SOUND_STEREO			2

#define SOUND_PLAY_ONCE			0
#define SOUND_PLAY_REPEAT		1

#define SOUND_PAN_LEFT			-10000
#define SOUND_PAN_CENTER		0
#define SOUND_PAN_RIGHT			10000

#define SOUND_VOLUME_MAX		0
#define SOUND_VOLUME_MIN		-10000
const int SOUND_VOLUME_RANGE		=	SOUND_VOLUME_MAX-SOUND_VOLUME_MIN;

#define SOUND_FREQ_MAX			100000
#define SOUND_FREQ_MIN			0

#define SOUND_FILE_TYPE_WAVE		0
#define SOUND_FILE_TYPE_OGG			1

#define SOUND_WAVE_FMT_PCM			0x01
#define SOUND_WAVE_FMT_MS_ADPCM		0x02
#define SOUND_WAVE_FMT_IMA_ADPCM	0x11

#define SOUND_OGG_BUFFER_MAX		512

#define SOUND_READ_FROM_CURRENT		DSBLOCK_FROMWRITECURSOR
#define SOUND_READ_ENTIRE			DSBLOCK_ENTIREBUFFER

#define	SOUND_LITTLE_ENDIAN			0
#define	SOUND_BIG_ENDIAN			1
#define	SOUND_ENDIAN				SOUND_LITTLE_ENDIAN

#define	SOUND_SIGN_TYPE_SIGNED		1
#define	SOUND_SIGN_TYPE_UNSIGNED	0
#define	SOUND_SIGN_TYPE				SOUND_SIGN_TYPE_SIGNED

#define	dMAX_SOUND_VOLUME			10000
#define	dMIN_SOUND_VOLUME			0

#pragma pack(1)

typedef struct tag_sound_handle
{

	int				type;
	unsigned long	size;
	FILE			*pt;
	unsigned long	start_position;
	OggVorbis_File	vf;
	vorbis_info		*vi;
	int				ogg_current_section;
	WAVEFORMATEX	fmt;
	DSBUFFERDESC	desc;

} sound_handle;

typedef struct tag_wav_fourcc
{

	unsigned char	fourcc[4];
	unsigned int	size;

} wave_fourcc;

typedef struct tag_wave_pcm_header {
  short FormatTag;          // WAVE_FORMAT_PCM
  short Channels;           // # of channels				2
  long  SampleRate;         // sampling rate				4
  long  BytesPerSec;        // bytes per second				4
  short BlockAlign;         // sample block alignment		2
  short BitsPerSample;      // bits per second				2
} wave_pcm_header;

typedef struct tag_wave_ima_adpcm_header {
  short FormatTag;          // WAVE_FORMAT_PCM
  short Channels;           // # of channels				2
  long  SampleRate;         // sampling rate				4
  long  BytesPerSec;        // bytes per second				4
  short BlockAlign;         // sample block alignment		2
  short BitsPerSample;      // bits per second				2
  short	cbSize;
  short	wSamplesPerBlock;
} wave_ima_adpcm_header;

#pragma pack()

class CSound
{
public:
	LPDIRECTSOUND8		m_lpDS;
	IDirectSoundBuffer	*m_lpBuffer;
	IDirectSoundBuffer8	*m_pList[SOUND_MAX][SOUND_COPY_MAX];

	BOOL				m_bIsStreamStop;
	int					m_iVolume,m_iPanValue;

	HRESULT				init(HWND _hWwnd, unsigned int _uiStereo, unsigned int _uiFreq, unsigned int _uiBitrate);
	HRESULT				destroy(void);

	HRESULT				getFourCC(FILE *_lpFp, unsigned char *_byFourcc);
	HRESULT				findFourCC(FILE *_lpFp, wave_fourcc *_lpChunk, const unsigned char *_byFourcc);
	HRESULT				createBuffer (IDirectSoundBuffer **_lpDSBuffers, DSBUFFERDESC *_lpDSBufferDesc);
	HRESULT				duplicateBuffer(IDirectSoundBuffer *_lpDSBuffer, IDirectSoundBuffer **_lpTargetDSBuffer);
	HRESULT				deleteBuffer(IDirectSoundBuffer *_lpDSBuffer);
	HRESULT				openFromFile (FILE *_lpFp, sound_handle *_lpHandle);
	HRESULT				openFromMem (unsigned char *_lpSource, WAVEFORMATEX *_wfeFormat,DSBUFFERDESC *_lpDesc);
	HRESULT				readFromFile (IDirectSoundBuffer *_lpDSBuffer, sound_handle *_lpHandle, unsigned int _uiPos, unsigned int _uiSize, DWORD _dwFlag, DWORD *_dwRead);
	HRESULT				readFromMem (IDirectSoundBuffer *_lpDSBuffer, unsigned char *_lpSource, unsigned int _uiPos, unsigned int _uiSize, DWORD _dwFlag, DWORD *_dwRead);
	HRESULT				moveFromFile (sound_handle *_handle, long _lPosition);
	HRESULT				moveFromMem (sound_handle *_handle, long _lPosition);
	HRESULT				closeFromFile (sound_handle *_handle);
	HRESULT				closeFromMem (sound_handle *_handle);

						CSound();
						~CSound(void);

	int					load(FILE *_lpFp, int _iIndex);
	int					getFreeIndex();

	HRESULT				play(int _iSoundIndex, int _iSoundSubIndex, unsigned char _byOption, long _lPanValue, long _lVolumeValue);
	HRESULT				stop(int _iSoundIndex, int _iSoundSubIndex);
	HRESULT				allStop(void);
	HRESULT				close(int _iSoundIndex);

	HRESULT				isPlaying(int _iSoundIndex, int _iSoundSubIndex);
	HRESULT				setPan(int _iSoundIndex, int _iSoundSubIndex,long _lPanValue);
	HRESULT				setVolume(int _iSoundIndex, int _iSoundSubIndex, long _lVolumeValue);
	HRESULT				setFreq(int _iSoundIndex,int _iSoundSubIndex, DWORD _dwFrequencyValue);
	void				setDefaultVolume(int _lVolume);

	BOOL				play(char *_lpstrFileName,int _iVolume=0xffffffff,int _iPanValue=0xffffffff);
};

#endif