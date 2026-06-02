#pragma once

#ifdef _WINDOWS
#include <windows.h>

#elif _LINUX
typedef int SOCKET;

#endif

#include "stdio.h"
#include "ICU_rand.h"			//Header of pseudo-random-number generator

#define KEYLENGTH 128			//length of secret key used in encryption
#define KEYBYTE KEYLENGTH/8		//byte-length of secret key
#define MACBYTE 16				//byte-length of MAC

#define TYPELEN	2				//length of TYPE Field
#define SEQLEN 4				//length of SEQUENCE Field
#define LENGTHLEN 4				//length of LENGTH Field

/*
#ifdef _WINDOWS


#ifdef _cplusplus
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif


#elif _LINUX
#define EXPORT  
#endif
*/

#define EXPORT

// ***** Definitions *****

// error codes
#define	SSL_SEND_FAIL				-2			// error occurs while sending
#define SSL_RECV_FAIL				-3			// error occurs while receiving
#define SSL_MAC_CREATE_FAIL			-4			// error occurs when MAC is created
#define SSL_MAC_VERIFY_FAIL			-5			// invalid MAC value
#define SSL_ENCRYPT_FAIL			-6			// fail to encrypt
#define SSL_DECRYPT_FAIL			-7			// fail to decrypt
#define SSL_PADDING_NOTEXIST		-8			// In AES, padding does not exist
#define SSL_BUF_SMALL				-9			// the size of buffer to store data is too small
#define SSL_QUEUE_SMALL				-10
#define SSL_REMOTE_HANDSHAKE_ERROR	-11			// received handshake message has some problem	
#define SSL_INVALID_ENCRYPT_TYPE	-12			// not matched to anything among message type below
#define SSL_INVALID_SEQUENCE		-13			// invalid sequence number of current session
#define SPEEDHACK_FOUND				-14			// the attempt such as speedhack was found

// message types, used to flags
#define AES_ENCRYPT				65531			// encryption only, AES
#define XOR_ENCRYPT				65532			// encryption only, XOR
#define ONLY_TIMESTAMP			65533			// timestamp only
#define AES_TIMESTAMP			65534			// AES encryption with timestamp
#define XOR_TIMESTAMP			65535			// XOR encryption with timestamp


//char master[KEYBYTE] = { 0x72, 0x32, 0xa2, 0x81, 0x93, 0x12, 0x34, 0x56, 0xb2, 0x2c, 0x83, 0x97, 0x85, 0xf3, 0xcc, 0xe1};

struct SSLSESSION {
	unsigned char myRnd[KEYBYTE];			// my random values
	unsigned char remoteRnd[KEYBYTE];		// random values of other side
	unsigned char masterSecret[KEYBYTE];	// master secret key
	unsigned char key[KEYBYTE];				// secret key used in this session
	int mySeq;								// my sequence number
	int remoteSeq;							// sequence number of other side
	int error;
	unsigned long serverTic;	// server's previous tick
	unsigned long clientTic;	// client's previous tick
#ifdef _WINDOWS
	LARGE_INTEGER query;		// previous QueryPerformanceCounter value
	LARGE_INTEGER freq;			// QueryPerformanceFrequency value
	DWORD gtc;					// previous GetTimeCount value
	DWORD tgt;					// previous timeGetTime value
#endif
	int checkRule;				// indicates current rule
};




// ***** rules of checking speedhack *****

// # of rules
#define NUM_RULE	3

// time interval (sec)
//float times[NUM_RULE] = { 5,   5,   5 };

// Fractions of each time interval.
// For example, if client's time(7.1 sec) > 5 * 1.4 then the next rule is applied
// If all rules are applied, return SPEEDHACK_FOUND
//float fracs[NUM_RULE] = { 1.4, 1.6, 1.8};




// ***** APIs *****



// Make a message for handshake
// After connection is established, this API should be called
// 
// Parameters : 
//		struct SSLSESSION *sock 
//					[IN/OUT]세션 정보를 저장하는 SSLSESSION. 
//					이 함수를 수행한 후에는 자신의 난수와 master secret, 자신의 sequence번호가 저장된다
//		char *msg 
//					[OUT]함수를 통해 생성된 메시지를 저장할 공간으로 36바이트 이상 할당되어야 한다.
//		int alloclen
//					[IN] msg에 할당된 길이
//
//	Return values :
//		general case : 
//					Total length of message. Sequence(4) + Random value(16) + MAC(16) = 36
//		SSL_BUF_SMALL :
//					Too small msg buffer


#ifdef __cplusplus
extern "C" {
#endif

EXPORT int gen_handshake(struct SSLSESSION *sock, char *msg, int alloclen);




// From the message of other side, process handshake
// After gen_handshake(), send it, receive a message from the other side, 
// this API should be performed
//
//	Parameters :
//		struct SSLSESSION *sock 
//					[IN/OUT] 세션 정보를 저장하는 SSLSESSION
//		char *msg
//					[IN]상대방으로부터 전송받은 메시지
//		int msglen
//					[IN] msg의 길이. 36바이트여야 한다
//	Return Values :
//		general case : 	
//					0. It means that Handshake is performed.
//		SSL_REMOTE_HANDSHAKE_ERROR : 
//					메시지의 길이가 일치하지 않는 경우
//		SSL_MAC_VERIFY_FAIL :
//					서버로부터 받은 난수의 MAC이 일치하지 않는 경우

EXPORT int process_handshake(struct SSLSESSION *sock, char *msg, int msglen);




// General API to make send_message
// make encrypted message from the data to be sent
// ** flags **
// AES_ENCRYPT				AES encryption for the very important but slow data
// XOR_ENCRYPT				XOR encryption which is faster but weaker than AES
// ONLY_TIMESTAMP			Only add Timestamp to check server/client sync
// AES_TIMESTAMP			AES encryption + Timestamp
// XOR_TIMESTAMP			XOR encryption + Timestamp
//
//	Parameters :
//		struct SSLSESSION *sock
//					[IN, OUT] 현재 연결되어 있는 SSLSESSION
//		char *encdata
//					[OUT] 암호화된 메시지를 저장할 공간. 암호화된 메시지에는 패딩과 
//					MAC이 추가되고 헤더가 추가되기 때문에, 최소한 메시지 길이+41바이트가 할당되어야 한다.
//		const char *buf
//					[IN] 암호화할 메시지가 저장되어 있는 공간
//		int alloclen
//					[IN] encdata 파라미터에 할당된 공간의 크기
//		int len
//					[IN] 암호화할 메시지의 길이
//		int flag
//					[IN] 위 설명 참조
//
//	Return values :
//		general case :
//					Length of encrypted message
//		SSL_MAC_CREATE_FAIL
//					메시지의 MAC 생성에 실패
//		SSL_BUF_SMALL 
//					암호화된 메시지를 저장하기에는 할당된 encdata가 너무 작은 경우
//		SSL_ENCRYPT_FAIL 
//					암호화에 실패

EXPORT int encrypt_msg(struct SSLSESSION *sock, char *encdata, const char *buf, int alloclen, int len, int flag);




// General API to get real message from encrypted message
// get plaintext message from the data received
// 
// ** Message Type **
// AES_ENCRYPT				AES Decryption + MAC verification
// XOR_ENCRYPT				XOR Decryption + MAC verification
// ONLY_TIMESTAMP			Check time sync of server/client
// AES_TIMESTAMP			AES Decryption + Timestamp check
// XOR_TIMESTAMP			XOR Decryption + Timestamp check
//
//	Parameters :
//		struct SSLSESSION *sock
//					[IN, OUT] 현재 연결되어 있는 SSLSESSION
//		char *buf
//					[OUT] 실제 메시지를 저장할 공간. 최소한 암호화된 메시지-25바이트가 할당되어야 한다
//
//		char *encdata
//					[IN] 암호화된 메시지가 저장되어 있는 공간
//		int buflen
//					[IN] buf 파라미터에 할당된 공간의 크기
//		int encdatalen
//					[IN] 암호화된 메시지의 길이
//		int flag
//					[IN] sequence number check flag (TRUE = check)
//
//	Return values :
//		general case :
//					Length of decrypted message
//		SSL_INVALID_SEQUENCE : 
//					상대방의 sequence번호가 맞지 않음
//		SSL_RECV_FAIL : 
//					암호화된 메시지의 길이가 맞지 않은 경우 발생한다.
//		SSL_DECRYPT_FAIL : 
//					복호화에 실패
//		SSL_PADDING_NOTEXIST : 
//					복호화된 메시지에서 패딩 찾기 실패. 잘못된 키를 복호화에 사용했을 가능성이 높다.
//		SSL_BUF_SMALL : 
//					복호화된 메시지를 저장하기에는 할당된 buf가 너무 작은 경우
//		SSL_MAC_CREATE_FAIL : 
//					MAC확인을 위한 MAC 생성에 실패
//		SSL_MAC_VERIFY_FAIL : 
//					메시지의 MAC이 일치하지 않음

EXPORT int decrypt_msg(struct SSLSESSION *sock, char *buf, char *encdata, int buflen, int encdatalen, int flag);


// Old version APIs.
// Detailed explaination is in the manual
EXPORT int put_msg(struct SSLSESSION *sock, char *encdata, const char *buf, int alloclen, int len);
EXPORT int get_msg(struct SSLSESSION *sock, char *buf, char *encdata, int buflen, int encdatalen, int flag);
EXPORT int fast_put_msg(struct SSLSESSION *sock, char *encdata, const char *buf, int alloclen, int len);
EXPORT int fast_get_msg(struct SSLSESSION *sock, char *buf, char *encdata, int buflen, int encdatalen, int flag);
EXPORT int put_msg_time(struct SSLSESSION *sock, char *encdata, const char *buf, int alloclen, int len);
EXPORT int get_msg_time(struct SSLSESSION *sock, char *buf, char *encdata, int buflen, int encdatalen, int flag);
EXPORT int fast_put_msg_time(struct SSLSESSION *sock, char *encdata, const char *buf, int alloclen, int len);
EXPORT int fast_get_msg_time(struct SSLSESSION *sock, char *buf, char *encdata, int buflen, int encdatalen, int flag);
EXPORT int only_put_msg_time(struct SSLSESSION *sock, char *encdata, const char *buf, int alloclen, int len);
EXPORT int only_get_msg_time(struct SSLSESSION *sock, char *buf, char *encdata, int buflen, int encdatalen, int flag);

// Added APIs for encryption/decryption check
EXPORT int AES_decrypt(struct SSLSESSION *sock, char *buf, char *encdata, int buflen, int encdatalen);
EXPORT int AES_encrypt(struct SSLSESSION *sock, char *encdata, char *buf, int alloclen, int len);

EXPORT int getBuildNo();
EXPORT void sslInit();

#ifdef __cplusplus
}
#endif
