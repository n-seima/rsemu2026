#define MAXLIST 1024
#define MAXNAMELENGTH 256
#define MAXFILELENGTH 1024
#define MAXBUFLIST 1024

#define FILE_NOT_FOUND	-1
#define INCORRECT_MAC	-2
#define EMULATION_FAIL -3
#define GETDEVICELIST_FAIL	-4

#pragma comment(lib, "ngpBrown.lib")

typedef struct _KeyLoggerInfo
{
	char Name[MAXNAMELENGTH];
	char FileName[MAXFILELENGTH];
} KeyLoggerInfo;

typedef struct _KeyLoggerList
{
	int Num;
	KeyLoggerInfo keyloggers[MAXLIST];
} KeyLoggerList;


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	// Initializing function
	int ngpBrown_Init(char *dir);

	// Checking function
	int ngpBrown_CheckKeyLogger(KeyLoggerList *result);
	
#ifdef __cplusplus
}

#endif // __cplusplus


