#include "CheckNic.h"
//#include "StringWrapper.h"

bool
cCheckNic::addStat( const wchar_t * ftp, const wchar_t* path )
{
/*	FILE * fp = NULL;
	if( wcscmp(ftp,APNIC_LOCAL) == 0)
	{
		fp = fopen(path);
		if(fp == NULL)
			return false;
	}else
	{
		DynamicBufferStream bus;
	}
*/	return true;
}

/*
void cCheckNic::clear()
{
	_records.clear();
	_headers.clear();
}

bool cCheckNic::addStat( const wchar_t* ftp, const wchar_t* path )
{
	WideString s;
	FilePtr file;

	if( String(APNIC_LOCAL) == ftp )
	{
		file = openWin32File( path );
		if( file ==null )
			return false;
	}
	else
	{
		DynamicBufferStream bus;

		StringVector addrs;
		Socket::getHostAddr(ftp, addrs);
		if(addrs.empty())
			return false;
		SockAddr sock( addrs[0] );
		sock.setPort( 21 );

		String sFilename;
		sFilename += path;
		OutputStream<DynamicBufferStream> os( &bus );
		if( FtpFileDownload( String( sock.getIP() ).c_str(), sock.getPort(), L"anonymous", L"test@test.test", sFilename.c_str(), os ) == false )		
			return false;
		file = openMemFile( bus.data(), bus.getSize(), true);
		if( file == null )
			return false;
	}
	
	if( loadTextDocument( file, s ) == false )
		return false;

	if( parse_rirstat( s ) == false ) 
		return false;

	return true;
}

bool cCheckNic::getCC( const SockAddr& test, String& cc)
{
	Record tmp;
	tmp.start = test;
	Records::iterator i = _records.lower_bound( tmp );
	if( i==_records.end() )
		return false;

	Record& r = *i;
	if( inRangeSockAddr( r.start, r.count, test ) == true )
	{
		cc = r.cc;
		return true;
	}
	return false;
}

bool cCheckNic::getHeader(Byte reg, cCheckNic::Header& header)
{
	for(UInt4 i=0;i<_headers.size();++i)
	{
		if ( _headers[i].registry == reg )
		{
			header =  _headers[i];
			return true;
		}
	}
	return false;
}

#undef DELIMITOR
#define DELIMITOR  L"|"

bool cCheckNic::parse_rirstat(String& str)
{
	bool ret;
	String out;
	UInt4 idx = 0;
	UInt4 i=0;

	int nRecord = 0;

	do {
		String out;
		ret = readLine( str, out, idx ); 	
		if( out.empty() )
			break;
		if( out[0] == L'#')
			continue;
		
		if( i == 0 ) {
			StringVector tokens;
			tokenize( out, DELIMITOR, back_inserter(tokens) );
			if( tokens.size() != 7 )
				return false;

			Header header;
			header.version = getRegCode( tokens[0].c_str() );
			header.registry = tokens[1];
			header.serial = tokens[2];
			header.records = _wtoi(tokens[3].c_str() );
			header.startdate = strtosysdate( tokens[4].c_str() );
			header.enddate = strtosysdate( tokens[5].c_str() );
			header.UTCoffset = tokens[6];

			_headers.push_back( header );
		}else{
			StringVector tokens;

			tokenize( out, DELIMITOR, back_inserter(tokens) );
			if( tokens.size() != 7 ) {			
				continue;
			}

			Record r;
			r.registry = getRegCode( tokens[0].c_str() );
			r.cc = tokens[1];
			r.count = _wtoi(tokens[4].c_str() );
			r.date = strtosysdate( tokens[5] );			
			if ( tokens[2] == L"asn" ) {
				IN_ADDR inaddr;
				inaddr.s_addr = _wtoi( tokens[3]);
				r.start.setIP(inaddr);;
				_records.insert(r);
			}else if( tokens[2]== L"ipv4" )
			{				
				r.start.setIP( tokens[3].c_str() );	
				_records.insert(r);
			}
		}
		i++;
	} while( ret );

	return true;
}

bool cCheckNic::inRangeSockAddr( const SockAddr& start, UInt4 range, const SockAddr& test)
{
	SockAddr end;
	end.sin_addr.s_addr = htonl(ntohl( start.sin_addr.s_addr ) + range);
	return ( start == test) || (test ==end) || ( start < test && test < end );
}

Byte cCheckNic::getRegCode(const wchar_t* str)
{
	if ( _wcsicmp( L"afrinic", str ) == 0 )
		return reg_afrinic;
	else if ( _wcsicmp( L"apnic", str ) == 0 )
		return reg_apnic;
	else if ( _wcsicmp( L"arin", str ) == 0 )
		return reg_arin;
	else if ( _wcsicmp( L"iana", str ) == 0 )
		return reg_iana;
	else if ( _wcsicmp( L"lacnic", str ) == 0 )
		return reg_lacnic;
	else if ( _wcsicmp( L"ripencc", str ) == 0 )
		return reg_ripencc;
	return reg_none;
}

SysDate cCheckNic::strtosysdate(const wchar_t* date)
{
	String s(date);
	Int4 y = _wtoi( mid(s, 0, 4).c_str() );
	Int4 m = _wtoi( mid(s, 4, 2).c_str() );
	Int4 d = _wtoi( mid(s, 6, 2).c_str() );
	return SysDate( y, m, d );
}

*/