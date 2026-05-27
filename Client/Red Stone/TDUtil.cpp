#include "TDUtil.h"

/** 
	새로운 로그 파일을 생성한다. 만약 오류가 발생하면. 예외를 발생시킨다.
*/
debug_log::debug_log ( const char* str )
{
	fp = fopen ( str, "wt" );
	if ( fp == NULL )
		throw ( "log initialize error!!" );
}
/** 
	작업한 로그 파일을 닫는다.
*/
debug_log::~debug_log ( )
{
	fclose ( fp );
}
/** 
	문자열을 입력한다.
*/
void debug_log::write_string ( const char* str ) const
{
	fwrite ( str, strlen ( str ), 1, fp );
}
/** 
	문자열을 입력하고, 다음라인으로 넘어간다.
*/
void debug_log::writeln_string ( const char* str ) const
{
	fwrite ( str, strlen ( str ), 1, fp );
	write_return ( );
}
/**
	다음 라인으로 넘어간다.  LINE BREAK = CARRIAGE RETURN + LINE FEED;
*/
void debug_log::write_return ( ) const
{
	const char *str = "\r\n"; 

	fwrite ( str, strlen ( str ), 1, fp );
}
