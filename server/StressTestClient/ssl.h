#ifdef _WINDOWS

#elif _LINUX
typedef int SOCKET;

#endif


#define KEYLENGTH	128			//length of secret key used in encryption
#define KEYBYTE		KEYLENGTH / 8		//byte-length of secret key	
#define TYPELEN	2
#define LENGTHLEN 4
#define SEQLEN 4

#ifdef _WINDOWS
#define IMPORT __declspec(dllimport)
#elif _LINUX
#define IMPORT  
#endif

// error codes that can be returned
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


struct SSLSESSION {
	unsigned char myRnd[KEYBYTE];			// my random values
	unsigned char remoteRnd[KEYBYTE];		// random values of other side
	unsigned char masterSecret[KEYBYTE];
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



// **** API's

extern "C"
{
// Make a message for handshake
// After connection is established, this API should be called
IMPORT int gen_handshake(struct SSLSESSION *sock, char *msg, int alloclen);

// From the message of other side, process handshake
// After gen_handshake(), send it, receive a message from the other side, 
// this API should be performed
IMPORT int process_handshake(struct SSLSESSION *sock, char *msg, int msglen);

// General API to make send_message
// make encrypted message from the data to be sent
// ** flags **
// AES_ENCRYPT				AES encryption for the very important but slow data
// XOR_ENCRYPT				XOR encryption which is faster but weaker than AES
// ONLY_TIMESTAMP			Only add Timestamp to check server/client sync
// AES_TIMESTAMP			AES encryption + Timestamp
// XOR_TIMESTAMP			XOR encryption + Timestamp
IMPORT int encrypt_msg(struct SSLSESSION *sock, char *encdata, const char *buf, int alloclen, int len, int flag);

// General API to get real message from encrypted message
// get plaintext message from the data received
// 
// ** Message Type **
// AES_ENCRYPT				AES Decryption + MAC verification
// XOR_ENCRYPT				XOR Decryption + MAC verification
// ONLY_TIMESTAMP			Check time sync of server/client
// AES_TIMESTAMP			AES Decryption + Timestamp check
// XOR_TIMESTAMP			XOR Decryption + Timestamp check
IMPORT int decrypt_msg(struct SSLSESSION *sock, char *buf, char *encdata, int buflen, int encdatalen, int flag);

// Detailed explaination is in the manual
IMPORT int put_msg(struct SSLSESSION *sock, char *encdata, const char *buf, int alloclen, int len);
IMPORT int get_msg(struct SSLSESSION *sock, char *buf, char *encdata, int buflen, int encdatalen, int flag);
IMPORT int fast_put_msg(struct SSLSESSION *sock, char *encdata, const char *buf, int alloclen, int len);
IMPORT int fast_get_msg(struct SSLSESSION *sock, char *buf, char *encdata, int buflen, int encdatalen, int flag);
IMPORT int put_msg_time(struct SSLSESSION *sock, char *encdata, const char *buf, int alloclen, int len);
IMPORT int get_msg_time(struct SSLSESSION *sock, char *buf, char *encdata, int buflen, int encdatalen, int flag);
IMPORT int fast_put_msg_time(struct SSLSESSION *sock, char *encdata, const char *buf, int alloclen, int len);
IMPORT int fast_get_msg_time(struct SSLSESSION *sock, char *buf, char *encdata, int buflen, int encdatalen, int flag);
IMPORT int only_put_msg_time(struct SSLSESSION *sock, char *encdata, const char *buf, int alloclen, int len);
IMPORT int only_get_msg_time(struct SSLSESSION *sock, char *buf, char *encdata, int buflen, int encdatalen, int flag);


// Added APIs for encryption/decryption check
IMPORT int AES_decrypt(struct SSLSESSION *sock, char *buf, char *encdata, int buflen, int encdatalen);
IMPORT int AES_encrypt(struct SSLSESSION *sock, char *encdata, char *buf, int alloclen, int len);
}