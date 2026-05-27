#ifdef SPEEDHACKCHECKERDLL_EXPORTS
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API __declspec(dllimport)
#pragma comment(lib, "ngpBlue")
#endif // SPEEDHACKCHECKERDLL_EXPORTS


#define MAXPLIST 100	// max. # of list
#define MAXPNAME 256	// max. length of process name
#define DEF_TIMERID	7

// Error code definition
#define FILE_NOT_FOUND	-1
#define INVALID_MAC		-2
#define INVALID_VERSION	-3
#define DECRYPTION_FAIL	-4


// Check Method
#define PATTERN_METHOD		1
#define CLOCK_METHOD		2
#define BOTH_METHOD			3


typedef BOOL (CALLBACK *CHECKPROC)(int);
typedef BOOL (CALLBACK *CHECKWITHINFOPROC)(struct PList);

// Process list
typedef struct PList {
	int num;			// # of processes
	int pid[MAXPLIST];	// process ID
	char pname[MAXPLIST][MAXPNAME];	// process name
	char mname[MAXPLIST][MAXPNAME];
} PList;


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

EXPORT_API int InitChecker(char *version, int method);
EXPORT_API PList WINAPI EnumProcs();
EXPORT_API BOOL KillProcs(struct PList pList);
EXPORT_API int LoadCheckList();
EXPORT_API int SetCallbackWithInfo(CHECKWITHINFOPROC func);
EXPORT_API int SetCallback(CHECKPROC func);
EXPORT_API int SetChecker(HWND hWnd, UINT_PTR timerID, UINT uElapse);
EXPORT_API int ReleaseChecker(HWND hWnd);

#ifdef __cplusplus
}
#endif // __cplusplus
