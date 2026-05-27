/********************************************************************
	created:	2004/07/14
	created:	14:7:2004   12:05
	filename: 	C:\project\company_code\csv_paser\TDUtil.h
	file path:	C:\project\company_code\csv_paser
	file base:	TDUtil
	file ext:	h
	author:		mightbe@hitel.net
	
	purpose:	basically utility set.
*********************************************************************/

#ifndef _TDUTIL_
#define _TDUTIL_

#pragma warning ( disable : 4786) 

#include < assert.h >
#include < crtdbg.h >
#include < vector >
#include < map >
#include < list >
#include < deque >
#include <queue>
#include < string >
#include < cassert >
#include <windows.h>
#include <malloc.h>
#include <stdlib.h>

#if !defined (STLPORT) || defined(_STLP_USE_NAMESPACES)
using namespace std;
#endif



//! TDASSERT 
#define TDASSERT( x ) _ASSERTE ( x )
//! 프로퍼티 매크로 READ용. 사용예) TDPropertyR( 참조시 실행할함수 ) 프로퍼티이름;
#define TDPropertyR( x )		_declspec ( property ( get=x ) )
//! 프로퍼티 매크로 WRITE용. 사용예) TDPropertyW( 입력시 실행할함수 ) 프로퍼티이름;
#define TDPropertyW( x )		_declspec ( property ( put=x ) )
//! 프로퍼티 매크로 READ/WRITE 겸용. 사용예) TDPropertyRW( 참조함수, 입력함수  ) 프로퍼티이름;
#define TDPropertyRW( x, y )	_declspec ( property ( get=x, put=y ) )

//* 너뭐야 -_-
typedef unsigned long DWORD;
 
class td_point
{
public:
	td_point ( )
	{
		x = 0;
		y = 0;
	}
	td_point ( int ix, int iy)
	{
		x = ix;
		y = iy;
	}
	td_point ( const td_point &pt )
	{
		 x =	pt.x;
		 y =	pt.y;
	}

	int x, y;
};

class td_rect
{
public:
	td_rect ( )
	{
		top = 0;
		bottom = 0;
		left = 0;
		right = 0;
	}
	td_rect ( const td_rect &rt )
	{
		top = rt.top;
		bottom = rt.bottom;
		left = rt.left;
		right = rt.right;
	}

	td_rect& operator= ( const td_rect &rt)
	{
		top = rt.top;
		bottom = rt.bottom;
		left = rt.left;
		right = rt.right;
		return *this;
	}

	int top, bottom;
	int left, right;
};


/**
********************************************************************************
class debug_log:           
  프로퍼티를 이용하여 만든 간단한 수준의 로그 클래스. 본 코드에 대한 개선사항
  이나 좋은 생각이 있는분들은 mightbe@hitel.net 으로 연락해주시기 바랍니다.

********************************************************************************
*/
//! 로그 클래스. 
class debug_log
{
public:
	debug_log ( const char* str );
	~debug_log ( );
	void write_string ( const char* str ) const ;
	void writeln_string ( const char* str ) const ;
	void write_return ( ) const ;

	TDPropertyW ( write_string ) char* write;
	TDPropertyW ( writeln_string ) char* writeln;
private:
	FILE* fp;
};


/**
******************************************************************************
td_graph \n
  자료구조의 대표적인 유형중 하나인 weighted graph이다. 
  본 코드에 대한 개선사항이나 좋은 생각이 있는분들은 mightbe@hitel.net으로
  연락해주시기 바랍니다.
******************************************************************************
*/
struct t_queue_value
{
	int id;
	int priority;

	t_queue_value ( )
	{
		id = 0;
		priority = 0;
	}

	t_queue_value ( const t_queue_value &value )
	{
		id = value.id;
		priority = value.priority;
	}

	bool operator== ( const t_queue_value &value )
	{
		return id == value.id;
	}
};


struct _sort
{
	bool operator () ( const t_queue_value &lv, const t_queue_value& rv )
	{
		return lv.priority < rv.priority; 
	}
};

template<class T>
class Queue 
{
public:
	Queue() { }
	Queue(const Queue& q) : v(q.v), w(q.w) { }
	~Queue() { }
	Queue& operator= (const Queue& q) {v = q.v; w = q.w; return *this;} // this overload could be omitted
	void pop() {v.erase(v.begin()); w.erase(w.begin());} // pop takes elements off the front
	void push(const T& x, int y = 0 ) 
	{ v.push_back(x); t_queue_value n; n.id = x; n.priority = y; w.push_back(n);
		queue_sort ( ); } // push() pushes elements on the back
	T& top ( ) { return v.front();}
	const T& top ( ) const { return v.front();}
	T& front() {return v.front();}
	const T& front() const {return v.front();}
	T& back() {return v.back();}
	const T& back() const {return v.back();}
	bool empty() const {return v.empty();}
	int size() const {return v.size();}
	void change_weight ( int index, int new_weight ) 
	{ w[index].priority = new_weight; queue_sort ( ); }
	void queue_sort ( )
	{	sort ( w.begin(), w.end(), _sort () ); for ( int i = 0; i < v.size(); i++)  v[i] = w[i].id; }
	bool operator== (const Queue& q) const {return v == q.v;}
	bool operator!= (const Queue& q) const {return v != q.v;}
	bool operator> (const Queue& q) const {return v > q.v;}
	bool operator< (const Queue& q) const {return v < q.v;}
	bool operator>= (const Queue& q) const {return v >= q.v;}
	bool operator<= (const Queue& q) const {return v <= q.v;}

	vector<T> v;
	vector<t_queue_value> w;
} ;


struct td_graph_link
{
	td_graph_link ( )
	{
		vertex_id = 0;
		weighted = 0;
	}
	td_graph_link ( const td_graph_link& link )
	{
		vertex_id = link.vertex_id;
		weighted = link.weighted;
	}

	int vertex_id;
	int weighted;
};

template < class t_value >
struct td_graph_value
{
	td_graph_value ( )
	{
		parent = -1;
		marked = 0;
		sum_distance = 0;
	}
	td_graph_value ( t_value& val )
	{
		parent = -1;
		marked = 0;
		sum_distance = 0;
		value = val;
	}

	td_graph_value ( const td_graph_value<t_value>& gv )
	{
		parent = gv.parent;
		marked = gv.marked;
		value = gv.value;
		sum_distance = gv.sum_distance;
	}

	vector < td_graph_link >		_links;

	int			parent;
	int			marked;
	int			sum_distance;
	t_value		value;
};

template < class t_value >
class td_graph
{
public:
	typedef int t_vertex_id;

	debug_log log;
	/**
	 td_graph 의 생성자
	 */
	td_graph ( )
		: log ( "tdlog.txt" )
	{
	}

	/**
	 td_graph 의 소멸자
	 */
	~td_graph ( )
	{
	}

	/**
		버텍스 추가 \n
		버텍스가 추가된다. 그리고. 
		 리턴값은 추가된 버텍스의 id이다.
	 */
	int add_vertex ( td_graph_value<t_value>& vertex )
	{
		_vertices.push_back( vertex );

		return _vertices.size ( ) - 1;
	}


	void resize_vertex ( int nsize )
	{
		_vertices.resize ( nsize );
	}

	/**
		버텍스 삭제
	 */
	void erase_vertex ( t_vertex_id vertex_id )
	{
		_vertices.erase(  _vertices[ vertex_id ] );
	}

	/**
		링크 추가
	 */
	void add_link ( t_vertex_id first_vertex_id, t_vertex_id second_vertex_id, int distance )
	{
		td_graph_link first_link, sencond_link;

		first_link.vertex_id = second_vertex_id;
		first_link.weighted = distance;

		sencond_link.vertex_id = first_vertex_id;
		sencond_link.weighted = distance;

		_vertices[first_vertex_id]._links.push_back( first_link );
		_vertices[second_vertex_id]._links.push_back( sencond_link );
	}

	/**
		링크 삭제
	 */
	void erase_link ( t_vertex_id vertex_id, t_vertex_id link_id )
	{
		_vertices[ vertex_id ]._links.erase( _vertices[ vertex_id ]._links [ link_id ] );
	}

	/**
	******************************************************************************
	 최단거리를 찾는다. \n
	 최단거리의 가중치를 리턴한다.
	******************************************************************************
	*/
	int search_shortest_way ( t_vertex_id start_vertex_id, t_vertex_id find_vertex_id )
	{
		Queue < int > 	_queue ;
/*
		1. 시작점을 주변정점으로 만든다. ( 우선순위 큐에 넣는다. )
		2. 가중치가 가장 작은 것을 찾아서 나무 정점으로 만든다.
		3. 그것과 연결된 모든 보이지 않는 정점을 주변정점으로 만든다. ( 큐에 넣는다. 가중치와 함께 )
		4. 나무정점 과 연결된 주변정점이 있을경우. 또 간선이 더 짧을경우. 새롭게 값을 갱신한다.
		*/

		int tree_id;
		int pre_tree_id;
		int _node;
	
		clear_marked ( );
		// 시작점을 주변정점으로 설정 그리고 큐에 저장.
		_vertices[ start_vertex_id ].marked = 1;
		_vertices[ start_vertex_id ].parent = -1;
		_node = start_vertex_id;
		_queue.push( _node, 0 );

		tree_id = -1;
		// 모든 정점이 나무 정점이 될 때까지.
		while ( _queue.empty() == false )
		{

			int index;
			tree_id = _queue.top ( );
			pre_tree_id = _vertices [ tree_id ].parent;
			//큐에서 빠져나오는 데이터는 나무정점이 되며 그 주변 정점 사이는 간선이 된다.
			_queue.pop();

#ifdef _DEBUG
			static int c = 0;
			char str[100], str2[100];
			sprintf ( str, "%d: tree_id: %d", c++, tree_id);
			sprintf ( str2, "tree_link: %d to %d", pre_tree_id, tree_id);
			int a = _queue.top ( );;
			log.writeln = str;
			log.writeln = str2;
			log.writeln = "";
#endif

			// 나무정점 지정.
			_vertices[ tree_id ].marked = 2;


			// 링크된 정점들을 주변정점으로 만든다. 
			for ( index = 0; index < _vertices [ tree_id ]._links.size(); index ++ )
			{
				int link_id;
				int weight;
				link_id = _vertices[ tree_id ]._links [ index ].vertex_id;
				weight  = _vertices[ tree_id ]._links [ index ].weighted;

				// 만약 나무정점이면 스킵
				if ( _vertices [ link_id ].marked == 2 )
				{
					continue;
				}

				// 이미 큐안에 있는 버텍스면.
				if ( _vertices [ link_id ].marked == 1 )
				{
					// 가중치가 더 작으면 그것으로 갱신.
					for ( int i = 0; i < _queue.w.size() ; i ++)
					{
						if ( _queue.w[i].id == link_id )
						{
							// 기존에 있는 현재가중치가 weight + 이전가중치값 보다 크면
							if ( _queue.w[i].priority > weight + _vertices [tree_id].sum_distance )
							{
								_vertices [ link_id ].parent = tree_id;
								_vertices [ link_id ].sum_distance = weight + _vertices [tree_id].sum_distance;
								_queue.change_weight( i, weight + _vertices [tree_id].sum_distance );
							}
						}
					}
					continue;
				}

				_vertices [ link_id ].parent = tree_id;
				_vertices [ link_id ].marked = 1;
				_vertices [ link_id ].sum_distance = weight + _vertices [tree_id].sum_distance;
				_node = link_id;
				_queue.push( _node, weight + _vertices [tree_id].sum_distance );
			}
		}


		// 스패닝 트리에서 내가 원하는 최단 경로만 가지치기 한다.   
		int cur_pos;
		cur_pos = find_vertex_id;
		_way_list.clear();
		while ( 1 )
		{
			if ( cur_pos == -1 )
				TDASSERT ( !"에러" );
			_way_list.push_front( cur_pos );
			if ( cur_pos == start_vertex_id)
				break;
			cur_pos = _vertices [ cur_pos ].parent;
		}

		return _vertices[find_vertex_id].sum_distance;
	}
	deque < int >								_way_list;

protected:

	int get_link_id ( t_vertex_id start_vertex_id, t_vertex_id end_vertex_id )
	{
		int n;
		for ( n = 0; n < _vertices[start_vertex_id]._links.size(); ++n  )
		{
			if ( _vertices[start_vertex_id]._links[n].vertex_id == end_vertex_id )
				return n;
		}
		return -1;
	}

	void clear_marked ( )
	{

		for ( int i = 0; i < _vertices.size(); i++  )
		{
			_vertices[i].marked = 0;
			_vertices[i].parent = -1;
			_vertices[i].sum_distance = 0;
		}
	}
	
protected:
	vector < td_graph_value<t_value> >		_vertices;
};


/**
******************************************************************************
	Name: td_string

	Desc: 간단한 스트링 클래스. 10분동안 뚝딱 만든거라 중복되는 코드가 매우많다.
	앞으로 계속해서 추가해나서 이것을 쓰도록한다.


 % 확인할점 : strlen이 0베이스인지 1베이스인지 찾아볼것.

	Auth: 이선우 ( mightbe@hitel.net )
******************************************************************************
*/ 
//! 스트링 클래스. 
class td_string
{
public:

	td_string ( const td_string& str )
	{
		int nstrlen;
		nstrlen = strlen ( str._string );
		_string = new char [ nstrlen + 1 ];
		strcpy ( _string, str._string );		
	}
	td_string ( )
		:	_string (NULL)
	{
	}
	
	td_string ( char* str )
	{
		int nstrlen;

		nstrlen = strlen ( str );
		_string = new char [ nstrlen + 1 ];
		strcpy ( _string, str );
	}
	td_string ( int nvalue )
	{
		char temp[256];
		int nstrlen;

		itoa ( nvalue, temp, 10 );

		nstrlen = strlen ( temp );
		_string = new char [ nstrlen + 1 ];
		strcpy ( _string, temp );		
	}

	~td_string ( )
	{
		if ( _string )
			delete [] _string;
	}

	td_string& operator= ( char* str )
	{
		int nstrlen;

		nstrlen = strlen ( str );
		realloc ( nstrlen + 1 );
		strcpy ( _string, str );		
		return *this;
	}
	 
	td_string& operator= ( const td_string& str )
	{
		int nstrlen;
		if ( str._string == NULL )
			return *this;
		nstrlen = strlen ( str._string );
		realloc ( nstrlen + 1 );
		strcpy ( _string, str._string );
		return *this;
	}

	td_string& operator+ ( const td_string& str )
	{
		int nstrlen;
		td_string temp_string;
		temp_string = _string;
		nstrlen = strlen ( str._string ) + strlen ( _string );
		realloc ( nstrlen + 1 );
		strcpy ( _string, temp_string );
		strcat ( _string + strlen ( _string ), str._string );
		return *this;
	}

	td_string& operator= ( int nvalue )
	{
		char temp[256];
		int nstrlen;

		itoa ( nvalue, temp, 10 );

		nstrlen = strlen ( temp );
		realloc ( nstrlen + 1 );
		strcpy ( _string, temp );		

		return *this;
	}

	char* ToChar ( )
	{
		return _string;
	}

	operator const td_string ( )
	{
		return *this;
	}
	operator const char* ( )
	{
		return _string;
	}

	operator int ( )
	{
		return atoi ( _string );
	}

	
	operator char* ( )
	{
		return _string;
	}

	int ToInt ( )
	{
		return atoi ( _string );
	}

	td_string& replace ( char target, char change )
	{
		int i;
		for ( i = 0 ; i < (int)strlen ( _string ) ; i++ )
		{
			if ( _string [ i ] == target ) _string [ i ] = change;
		}

		return *this;
	}

	void push_back ( char ch )
	{
		int ncursize;
		int nstrsize;
		
		if ( _string == NULL )
		{
			ncursize = 1;
			_string = new char [ ncursize + 1 ];
			_string[0] = '\0';
		}
		else
		{
			td_string temp_str;

			temp_str = _string; 
			ncursize = _msize ( _string );
			realloc ( ncursize + 1 );
			strcpy ( _string, temp_str._string );
		}
		nstrsize = strlen ( _string ) ;
		_string [ nstrsize ] = ch;
		_string [ nstrsize + 1  ] = '\0';
	}

	void pop_back ( )
	{
		int ncursize;
		int nstrsize;
		
		if ( _string == NULL )
		{
			ncursize = 1;
			_string = new char [ ncursize + 1 ];
			_string[0] = '\0';
		}
		else
		{
			td_string temp_str;

			temp_str = _string; 
			ncursize = _msize ( _string );
			realloc ( ncursize - 1 );
			strcpy ( _string, temp_str._string );
		}
		nstrsize = strlen ( _string ) ;
		_string [ nstrsize - 1] = '\0';
	}

	td_string& left_trim ( )
	{
		int noffset;
		td_string temp_str;
		
		if ( _string == NULL ) return *this;


		temp_str = _string; 
		for ( noffset = 0 ; noffset <(int)strlen ( _string ) ;  )
		{
			// 공백문자이거나 탭문자가 아닐경우.
			if ( _string [ noffset ] != 0x20 &&	_string [ noffset ] != 0x09  ) break;
			noffset ++;
		}

		realloc ( strlen ( _string ) + 1 - noffset );	
		strcpy ( _string, temp_str._string + noffset ); 
		return *this;
	}

	td_string& right_trim ( )
	{
		int noffset;
		int nsize;
		td_string temp_str;
		
		if ( _string == NULL ) return *this;

		temp_str = _string; 
		nsize = strlen ( _string ) -  1;
		for ( noffset = 0 ; noffset < nsize + 1 ;  )
		{
			// 공백문자이거나 탭문자가 아닐경우.
			if ( _string [ nsize - noffset ] != 0x20 &&	_string [ nsize - noffset ] != 0x09  ) break;
			noffset ++;
		}

		realloc ( strlen ( _string ) + 1 - noffset );	
		temp_str._string [ nsize - noffset + 1 ] = '\0';
		strcpy ( _string, temp_str._string  ); 
		return *this;
	}

private:
	void realloc ( int nsize )
	{
		if ( _string == NULL )
		{
			_string = new char [ nsize ];
		}
		else if ( nsize > (int)_msize ( _string ) )
		{
			delete [] _string;
			_string = new char [ nsize ];
		}
	}
	char* _string;

};

/**
******************************************************************************
	Name: TDCommand

	Desc: 커맨드 유틸리티

	Auth: 이선우 ( mightbe@hitel.net )

    (*참조문헌: GPG스터디 그래픽 게시판 
				http://www.gpgstudy.com/forum/viewtopic.php?t=3218 )
******************************************************************************
*/
class TDCommand
{
public:
	virtual bool	execute ( DWORD dwParam1 = 0, DWORD dwParam2 = 0 ) = 0;
};

/*******************************************************************************
*	Name: TDCommandBind

	Desc: 커맨드 유틸리티

	Auth: 이선우 ( mightbe@hitel.net )

    (*참조문헌: GPG스터디 그래픽 게시판 
				http://www.gpgstudy.com/forum/viewtopic.php?t=3218 )
*******************************************************************************/
template < typename T > class TDCommandBind : public TDCommand
{
public:
	typedef bool ( T::*linked_function ) ( DWORD dwParam1, DWORD dwParam2 );
	void	bind ( T* in_owner, linked_function in_linked_func )
	{
		TDASSERT ( in_owner			!= NULL );
		TDASSERT ( in_linked_func	!= NULL );

		owner = in_owner;
		linked_func = in_linked_func;
	}
	bool	execute ( DWORD dwParam1 = 0, DWORD dwParam2 = 0 )
	{
		return ( owner->*linked_func) ( dwParam1, dwParam2 );
	}
private:
	linked_function	linked_func;
	T*				owner;
};


/*******************************************************************************
*	Name: TDSingleton

	Desc: 단일체 유틸리티

	Auth: 이선우 ( mightbe@hitel.net )

	(*참조문헌: 게임 프로그래밍1권 1.3 자동적인 단일체 유틸리티 )
*******************************************************************************/
template < typename T > class TDSingleton 
{
	static T* _singleton;

public:

	TDSingleton ( )
	{
		const T* nOne = ( const T* )( 1 );
		int nOffset;

		_ASSERTE ( _singleton == NULL );
		nOffset = ( int )( nOne ) - ( int ) ( ( TDSingleton < T >* )( nOne) );
		_singleton = ( T* )( ( int )( this ) + nOffset );
	}

	~TDSingleton ( )
	{
		_ASSERTE ( _singleton != NULL );
		_singleton = 0;
	}

	static T& getSingleton ( )
	{
			_ASSERTE ( _singleton != NULL );
			return ( *_singleton ) ;
	}

	static T* getSingletonPtr ( )
	{
			return ( _singleton );
	}
};

template < typename T > T* TDSingleton < T >::_singleton = NULL;



/*******************************************************************************
*	Name: TDHandle, TDHandleMgr

	Desc: 범용 핸들 기반 자원 관리자.

	Auth: 이선우 ( mightbe@hitel.net )

	(*참조문헌: 게임 프로그래밍1권 1.6 범용 핸들 기반 자원 관리자 )
*******************************************************************************/

//! 핸들
template < typename T > class TDHandle
{
private:
	union 
	{
		enum
		{
			MAX_BITS_INDEX = 16,
			MAX_BITS_MAGIC = 16,

			MAX_INDEX = ( 1 << MAX_BITS_INDEX ) - 1,
			MAX_MAGIC = ( 1 << MAX_BITS_MAGIC ) - 1,
		};

		struct 
		{
			unsigned _Index : MAX_BITS_INDEX; 
			unsigned _Magic : MAX_BITS_MAGIC; 
		};

		unsigned int _Handle;
	};

public:
	TDHandle ( void ) : _Handle ( 0 ) { }
	void Init ( unsigned int in_nIndex );

	unsigned int getIndex ( void ) const { return ( _Index ); }
	unsigned int getMagic ( void ) const { return ( _Magic ); }
	unsigned int getHandle ( void ) const { return ( _Handle ); }
	bool IsNull ( void ) const { return ( !_Handle ); }

	//! 핸들을 얻을때 캐스팅이 일어나지 않도록 명시해줌.
	operator unsigned int ( void ) const { return ( _Handle ); }
};

//! 핸들 
template < typename T >
void TDHandle < T >::Init ( unsigned int in_nIndex )
{
	static unsigned int nAutoMagic = 0;

	assert ( IsNull ( ) );				// 재배치 허용 안함.
	assert ( in_nIndex <= MAX_INDEX );	// 검증 범위

	if ( ++nAutoMagic > MAX_MAGIC )
	{
		nAutoMagic = 1;		// 0은 널 핸들에 쓰인다.
	}

	_Index = in_nIndex;
	_Magic = nAutoMagic;

}

template < typename T >
inline bool operator != ( TDHandle < T > l, TDHandle < T > r )
{
	return ( l.getHandle ( ) != r.getHandle ( ) );
}

template < typename T >
inline bool operator == ( TDHandle < T > l, TDHandle < T > r )
{
	return ( l.getHandle ( ) == r.getHandle ( ) );
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// 
//! 핸들 관리자.
template < typename DATA, typename HANDLE >
class TDHandleMgr
{
private:
// private 데이터형들
	typedef std::vector < DATA >	UserVec;
	typedef std::vector < DWORD >	MagicVec;
	typedef std::vector < DWORD >	FreeVec;

// private 데이터
	UserVec			_userVec;	// 자원들을 담을 벡터
	MagicVec		_magicVec;	// 매직 넘버들을 담을 벡터
	FreeVec			_freeVec;	// DB 안의 빈 슬롯들을 관리

public:
// 생성자와 소멸자
	TDHandleMgr ( void ) { }
	~TDHandleMgr ( void ) { }

// Handle 메서드들

	//! 핸들 관리
	DATA*	Acquire ( HANDLE&	in_cHandle );
	void	Release ( HANDLE	in_cHandle );

	//! 역참조
	DATA*	Dereference ( HANDLE in_cHandle );
	const DATA* Dereference ( HANDLE in_cHandle ) const;

	//! 기타조회
	unsigned int getUsedHandleCount ( void ) const
	{
		return ( _magicVec.size ( ) - m_freeVec.size ( ) );
	}

	bool HasUsedHandles ( void ) const
	{
		return ( !!getUsedHandleCount ( ) ); 
	}
};

//! 핸들 관리자.
template < typename DATA, typename HANDLE >
DATA* TDHandleMgr < DATA, HANDLE >::Acquire ( HANDLE& in_cHandle )
{
	// 해제 목록이 비어 있으면 새 데이터를 추가. 아니면 기존의
	// 것들 중 첫번째 것을 돌려준다.

	unsigned int nIndex;

	if ( _freeVec.empty ( ) )
	{
		nIndex = _magicVec.size ( ) ;
		in_cHandle.Init ( nIndex );
		_userVec.push_back ( DATA ( ) );
		_magicVec.push_back ( in_cHandle.getMagic ( ) );
	}
	else
	{
		nIndex = _freeVec.back ( );
		in_cHandle.Init ( nIndex );
		_freeVec.pop_back ( );
		_magicVec [ nIndex ] = in_cHandle.getMagic ( );
	}
	return ( _userVec.begin ( ) + nIndex );
}

template < typename DATA, typename HANDLE > 
void TDHandleMgr < DATA, HANDLE >::Release ( HANDLE in_cHandle )
{
	//색인을 얻는다.
	unsigned int nIndex;
	
	
	nIndex = in_cHandle.getIndex ( );

	// 유효성을 점검
	assert ( nIndex < _userVec.size ( ) );
	assert ( _magicVec [ nIndex ] == in_cHandle.getMagic ( ) );

	// 문제가 없으면 제거 - 더 이상 쓰이지 않음을 표시하고
	// 해제 목록에 추가한다.
	_magicVec [ nIndex ] = 0;
	_freeVec.push_back ( nIndex );
}

template < typename DATA, typename HANDLE >
inline DATA* TDHandleMgr < DATA, HANDLE > ::Dereference ( HANDLE in_cHandle )
{
	unsigned int nIndex;

	if ( in_cHandle.IsNull ( ) ) return ( 0 );

	// 핸들 유효성 점검 - 모든 핸들 참조들이 항상 유효하다고
	// 가정되는 경우라면 속도를 위해서 생략할 수도 있다.
	nIndex = in_cHandle.getIndex ( );

	if ( ( nIndex >= _userVec.size ( ) ) || ( _magicVec [ nIndex ] != in_cHandle.getMagic ( ) ) )
	{
		// 유효하지 못한 핸들 - 클라이언트 쪽의 오류이다.
		assert ( 0 );
		return ( 0 );
	}

	return ( _userVec.begin ( ) + nIndex );
}

template < typename DATA, typename HANDLE >
inline const DATA* TDHandleMgr < DATA, HANDLE >::Dereference ( HANDLE in_cHandle ) const
{
	//이러한 느슨한 형변환도 문제가 없다. 비 const 버전은 어떠한
	//것도 수정하지 않는다.

	typedef HandleMgr < DATA, HANDLE > ThisType;
	return ( const_cast < ThisType* > ( this )->Dereference ( in_cHandle ) );
}


///////////////////////////////////////////////////////////////////////////////////////////////////


#endif