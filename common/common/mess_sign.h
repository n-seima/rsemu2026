#pragma once

#ifdef	_DEBUG
	#ifdef	_FOR_JAPAN
		#pragma comment(lib,"../../common/common/MessSignJD.lib")
	#else
		#pragma comment(lib,"../../common/common/MessSignD.lib")
	#endif
#else
	#ifdef	_FOR_JAPAN
		#pragma comment(lib,"../../common/common/MessSignJR.lib")
	#else
		#pragma comment(lib,"../../common/common/MessSignR.lib")
	#endif
#endif

class	cMessSigeDoumi
{
	int	m_iValue;

public:
	cMessSigeDoumi();
	~cMessSigeDoumi();
};	

class	cMessSign
{
public:
	char	m_strTemplateFileName[512];
	int		m_iIsSuccess;

	cMessSign();
	~cMessSign();

	static	void	SetDataEncodeTable(int _iTableIndex);
	static	void	EncodeDataBuffer2(void *_lpBuffer,int _iBufferSize);	//	ｹﾛ
	static	void	DecodeDataBuffer2(void *_lpBuffer,int _iBufferSize);	//	ｹﾛ

	static	void	EncodeBuffer(void *_lpBuffer,int _iBufferSize);	//	ｹﾛ
	static	void	DecodeBuffer(void *_lpBuffer,void *_lpReturnBuffer,int _iReturnBufferSize);
	static	void	DecodeBuffer(void *_lpBuffer,int _iBufferSize);
	static	int		DecodeBufferToInt(void *_lpBuffer,int _iBufferSize);
	static	int		XorInt(int _iInput);
	static	int		MakeCheckTableResult(char *_lpstrFolder);
};

extern	int		EncryptData(void *_lpData,int _iSize,int _iSeed);
extern	int		DecryptData(void *_lpData,int _iSize,int _iSeed);

#define	dENCRYPT_TABLE_COUNT	73
#define	dENCRYPT_TABLE_SIZE		93
#define	dMAX_PACKET_SERIAL		0x7fff

class	cEncryptGameServerPacketTable
{
	inline	void	_build();
	inline	void	_reset();
	
public:
	
	int				m_iTableCount,m_iTableSize;
	unsigned char	m_abTable[dENCRYPT_TABLE_COUNT][dENCRYPT_TABLE_SIZE];

	void	build();
	void	encrypt(int _iTable,void *_lpData,int _iDataSize);
	void	encrypt(void *_lpData,int _iDataSize,unsigned char *_lpbTable);
	void	decrypt(int _iTable,void *_lpData,int _iDataSize);
	void	decrypt(void *_lpData,int _iDataSize,unsigned char *_lpbTable);
	void	setTable(int _iTable,unsigned char* _lpiTable);

	unsigned short makeProtectCode(int _iSerial,int _iEncTableIndex,unsigned short _wXorMask);
	void	decodeProtectCode(unsigned short _wProtectCode,int *_lpiEncTableIndex,int *_lpiProtectCodeSerial,unsigned short _wXorMask);

	unsigned short makeXorCode();

	int		getNextPacketSerial(int _iCurrentPacketSerial);
};


extern	cEncryptGameServerPacketTable	g_gsEncTable;	//	ﾅｬｶ・>ｰﾔﾀﾓｼｭｹ・ﾆﾐﾅｶ ﾀﾎﾄﾚｵ・ﾅﾗﾀﾌｺ・
extern	void		BuildMessSign();
extern	int			_IsTestID(char *_lpstId);
extern	void		GetMessSignEncodeSign(unsigned int *_uiValue);

extern	int			GetLoginServerEncodeCode1();
extern	int			GetLoginServerEncodeCode2();

extern	unsigned	int	EncodeAddrBuffer(int _iSeedValue,unsigned char *_lpBuffer,int _iBufferSize);
extern	unsigned	int	DecodeAddrBuffer(int _iSeedValue,unsigned char *_lpBuffer,int _iBufferSize);
