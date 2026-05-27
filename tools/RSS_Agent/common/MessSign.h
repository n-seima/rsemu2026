#pragma once

class	cMessSign
{
public:
	static	void	EncodeBuffer(void *_lpBuffer,int _iBufferSize);	//	¹öÆÛ
	static	void	DecodeBuffer(void *_lpBuffer,void *_lpReturnBuffer,int _iReturnBufferSize);
	static	int		DecodeBufferToInt(void *_lpBuffer,int _iBufferSize);
	static	int		XorInt(int _iInput);
	static	void*	ReadMessSignFile(const void *_lpstrFile,int *_lpiDataSize);
	static	int		MakeMessSignFile(const void *_lpstrFile,void *_lpData,int _iDataSize);
};