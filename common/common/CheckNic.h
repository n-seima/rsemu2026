#pragma once

#define APNIC_FTP	L"ftp.apnic.net"
#define APNIC_LOCAL	L"local"
#define APNIC_PATH	L"pub/stats/apnic/delegated-apnic-latest"

#include <string>

class cCheckNic
{
public:
	enum {reg_afrinic,reg_apnic,reg_arin,reg_iana,reg_lacnic,reg_ripencc, reg_none, reg_num};

	struct stHeader {
		int		m_iVersion;
		std::string		m_strRegistry;
		std::string		m_strSerial;
		int		m_iRecords;
//		SysDate startdate;
//		SysDate enddate;
//		String UTCoffset;
	};

	struct stRecord {
		int		m_iRegistry;
		std::string		m_strCC;
//		SockAddr start;
		int		m_iCount;
//		SysDate date;
	};	

	bool	addStat( const wchar_t * ftp, const wchar_t* path );

};
/*
#include <string>
#include <set>
#include <vector>
#include <winsock2.h>


#define Int4	int
#define UInt4	unsigned int
#define String	std::string
#define Byte	int
#define SockAddr sockaddr

typedef struct
{
	int		yy;
	short	mm;
	short	dd;
}SysDate;

class cCheckNic
{
public:
	enum {reg_afrinic,reg_apnic,reg_arin,reg_iana,reg_lacnic,reg_ripencc, reg_none, reg_num};

	struct Header {
		Int4 version;
		String registry;
		String serial;
		Int4 records;
		SysDate startdate;
		SysDate enddate;
		String UTCoffset;
	};

	struct Record {
		Byte registry;
		String cc;
		SockAddr start;
		UInt4 count;
		SysDate date;
	};	
private:
	// set functor 
	struct RIRSort
	{
		bool operator()(const Record& l,const Record& r) const
		{
			return l.start > r.start ;
		}
	};
	typedef std::set< Record, RIRSort > Records;
	typedef std::vector< Header> Headers;

	Records		_records;	//< 저장된 레코드
	Headers		_headers;	//< 헤더 정보
public:
	bool		addStat( const wchar_t * ftp, const wchar_t* path );
	bool		getCC( const SockAddr& test, String& cc);
	bool		getHeader(Byte registry, Header& header);
	void		clear();
private:
	bool		parse_rirstat(String& str);
	bool		inRangeSockAddr( const SockAddr& start, UInt4 range, const SockAddr& test);
	Byte		getRegCode(const wchar_t* str);
	SysDate		strtosysdate(const wchar_t* date);
};
*/
/*
void test()
{
	loadWinsock();

	RIR	rir;
	//if( rir.init(APNIC_FTP, APNIC_PATH) == false )
	if( rir.addStat(APNIC_LOCAL, L"d:\\delegated-apnic-latest.txt") == false )
	{
		std::cout << "rir.addStat failed!\n";
	}

	if( rir.addStat(L"ftp.apnic.net", L"pub/stats/afrinic/delegated-afrinic-latest") == false )
	{
		std::cout << "rir.addStat failed!\n";
	}

	String cc;
	if( rir.getCC( L"222.106.57.99", cc ) == true )
	{
		std::wcout << "222.106.57.99 - " << cc.c_str() << std::endl;
	}else{
		std::cout << "222.106.57.99 - unknown cc\n";
	}
}

*/

