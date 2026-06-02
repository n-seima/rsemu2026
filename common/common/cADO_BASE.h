#ifndef __C_ADO_BASE_H
#define __C_ADO_BASE_H

#pragma warning ( disable : 4786) 

#include <icrsint.h> 
#include "definePacketData.h"

//#import "msado20.tlb" no_namespace rename("EOF", "adoEOF") rename("BOF", "adoBOF")

#include "msado20.tlh"

#define dSQL_CREATETABLE(a,b)	sqlExecute("CREATE TABLE %s(%s)",a,b)
#define dSQL_DELETETABLE(a)		sqlExecute("DROP TABLE %s",a)

#define dSQL_INSERT(a,b)		sqlExecute("INSERT INTO %s VALUES(%s)",a,b)
#define dSQL_DELETE(a,b)		sqlExecute("DELETE FROM %s WHERE %s",a,b)
#define dSQL_UPDATE(a,b,c)		sqlExecute("UPDATE %s SET %s WHERE %s",a,b,c)

#define dSP_SUCCESS						0	//성공적인 실행
#define dSP_FAIL						-1	//실행 자체의 문제 
#define dSP_ERROR						-2	//SP 내부의 에러다.

#define dMAX_DB_CONNECTION				20

#define DEF_PARAM(cmd,x) cmd->Parameters->Item[ _variant_t(x) ]->Value 
#define RsITEM(rs,n) rs->Fields->Item[_variant_t(n)]->Value
#define CREATEiNSTANCE(sp,riid) { HRESULT _hr =sp .CreateInstance( __uuidof( riid ) ); \
if (FAILED(_hr)) _com_issue_error(_hr); }

#define CHECKhr(hr) checkHR(hr,__LINE__);
#define DMPcomERR(e) dmpCOMerr(e,__LINE__,__FILE__);
#define ZZ GlastLine = __LINE__ + 1
#define ZZR GlastLine =0;

typedef struct InitOle {
	InitOle()  { ::CoInitialize(NULL); }
	~InitOle() { ::CoUninitialize();   }
} _init_InitOle_;       // Global Instance to force load/unload of OLE


/*
adStateClosed					기본값. 개체가 닫혀 있음을 나타냅니다. 
adStateOpen						개체가 열려 있음을 나타냅니다. 
adStateConnecting Recordset		개체가 연결 중임을 나타냅니다. 
adStateExecuting Recordset		개체가 명령을 실행하는 중임을 나타냅니다. 
adStateFetching Recordset		개체의 행을 가져오는 중임을 나타냅니다. 

  conntion->GetState()로 얻어오며
윗 플래그들은 중복 가능 
*/

#define dDBCONNECT_COUNT		dMAX_DB_CONNECTION
#define dERROR_COUNT			5		//	5개 이상 연속으로 에러나면 다시 컨넥트 해서 사용해라.

class	cADO_BASE
{
public:
	int		iMaxCount;
	int		iLastIndex;

	DWORD	dwErrorTime;
	int		iErrorCount;
	int		iConnectIndex;		//	dDBCONNECT_COUNT / 3 -> 3의 배수로만 처리해라. 한번에 3개씩..
	int		iErrorProblem;
	char	mainPro[1024];
	char	pro[1024];
	char 	strDBIp[16];
	char 	strDBId[18];
	char 	strDBPw[32];
	char 	strDBDb[64];

	_ConnectionPtr		pConnection[dDBCONNECT_COUNT];
	_ConnectionPtr		GetConnection();	//	연결되어 있으나 사용중이지 않은 것을 보내준다.
	CRITICAL_SECTION	csConnectionPool;
	HANDLE				hThread;
	BOOL				continueThread;
	BOOL				isEnd;

public:
	cADO_BASE();
	virtual ~cADO_BASE();

	BOOL	Connect(char *ip,char *id,char *pw,char *db,int count = dMAX_DB_CONNECTION);
	BOOL	reConnect();
	void	Close();

//	for execute sql	
	BOOL	__cdecl sqlExecute(char *str,...);	//가능하면 이걸로 사용하지 말고 SP를 사용해라.
//	BOOL	_sqlChangeSkillData();
//	void	_sqlResetJob();
//	void	spSaveAva(cP_PLAYER_DATA * pData);
	void	getTimeToUTime(char *_lpstrDate,UTime *_lpUTime);

protected:
	
//	for Debug 
	int GlastLine;
	void dmpCOMerr( _com_error &e, int line, char *file, bool traceOnly=false );

//	for binary data 
	void	BlobToVariant(VARIANT &varChunk,void *pData, UINT nBytes);	//	data -> blob
	int		VariantToBlob(VARIANT &varChunk,void *pData);	//	blob -> data

	CRITICAL_SECTION	m_csAC;
};


#endif //	__C_ADO_BASE_H