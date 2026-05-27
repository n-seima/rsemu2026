/*******************************************************************************************
	tdspring_motion.cpp
*******************************************************************************************/

#include "math.h"
#include "TDUtil.h"
#include "tdspring_motion.h"

#define TD_PI (3.1415926535897932384626433832795f)
#define TD_DEGREE_TO_RADIAN(x) ( (x)*TD_PI/180 )
#define TD_RADIAN_TO_DEGREE(x) ( (x)*(180/TD_PI) )

void tdswap ( float &a, float &b )
{
	float temp;

	temp = a;
	a = b;
	b = temp;
}

enum e_quadrant
{
	ec_original,
	ec_first,
	ec_second,
	ec_third,
	ec_forth,
	ec_first_second_on_xaxis,
	ec_second_third_on_yaxis,
	ec_third_forth_on_xaxis,
	ec_forth_first_on_yaxis,
};

// 정점이 몇사분면에 존재하는지 판별
e_quadrant	td_get_number_quadrant ( td2dvector in_vec )
{

	// 정점에 있을때
	if ( in_vec.x == 0.0f && in_vec.y == 0.0f ) 
	return ec_original;

	// 1사분면
	if ( in_vec.x > 0.0f && in_vec.y < 0.0f ) 
		return ec_first;
	// 2사분면
	if ( in_vec.x < 0.0f && in_vec.y < 0.0f )
		return ec_second;
	// 3사분면
	if ( in_vec.x < 0.0f && in_vec.y > 0.0f )
		return ec_third;
	// 4사분면
	if ( in_vec.x > 0.0f && in_vec.y > 0.0f )
		return ec_forth;

	// 1사분면과 2사분면 사이 x축에 잇을때
	if ( in_vec.x == 0.0f && in_vec.y < 0.0f ) 
		return ec_first_second_on_xaxis;
	// 2사분면과 3사분면 사이 y축에 잇을때
	if ( in_vec.x < 0.0f && in_vec.y == 0.0f )
		return ec_second_third_on_yaxis;
	// 3사분면과 4분면 사이 x축에 잇을때
	if ( in_vec.x == 0.0f && in_vec.y > 0.0f )
		return ec_third_forth_on_xaxis;
	// 4사분면r과 1사분면 사이 y축에 잇을때
	if ( in_vec.x > 0.0f && in_vec.y == 0.0f )
		return ec_forth_first_on_yaxis;

	return ec_original;
}

// 사분면을 이용해서 주어진 좌표가 시계방향/반시계 방향중 어느쪽으로
// 변화 하였는지 판별한다. 
bool td_get_clockwise ( td2dvector in_old, td2dvector in_new )
{
	e_quadrant quadrant_old, quadrant_new;

	quadrant_old = td_get_number_quadrant ( in_old );
	quadrant_new = td_get_number_quadrant ( in_new );

	switch ( quadrant_old )
	{
		case ec_original:
			return true;
		case ec_first:
			switch ( quadrant_new )
			{
			case ec_forth:
			case ec_forth_first_on_yaxis:
			case ec_third_forth_on_xaxis:
				return true;
			case ec_first_second_on_xaxis:
			case ec_second:
			case ec_second_third_on_yaxis:
				return false;

			default:
				return true;
			}
		case ec_second:
			switch ( quadrant_new )
			{
			case ec_first_second_on_xaxis:
			case ec_first:
			case ec_forth_first_on_yaxis:
				return true;
			case ec_third:
			case ec_second_third_on_yaxis:
			case ec_third_forth_on_xaxis:
				return false;
			default:
				return true;
			}
		case ec_third:
			switch ( quadrant_new )
			{
			case ec_second_third_on_yaxis:
			case ec_second:
			case ec_first_second_on_xaxis:
				return true;
			case ec_forth:
			case ec_third_forth_on_xaxis:
			case ec_forth_first_on_yaxis:
				return false;
			default:
				return true;
			}
		case ec_forth:
			switch ( quadrant_new )
			{
			case ec_second_third_on_yaxis:
			case ec_third:
			case ec_third_forth_on_xaxis:
				return true;
			case ec_first:
			case ec_forth_first_on_yaxis:
			case ec_first_second_on_xaxis:
				return false;
			default:
				return true;
			}
		case ec_first_second_on_xaxis:
			switch ( quadrant_new )
			{
			case ec_first:
			case ec_forth_first_on_yaxis:
				return true;
			case ec_second_third_on_yaxis:
			case ec_second:
				return false;
			default:
				return true;
			}
		case ec_second_third_on_yaxis:
			switch ( quadrant_new )
			{
			case ec_second:
			case ec_first_second_on_xaxis:
				return true;
			case ec_third:
			case ec_third_forth_on_xaxis:
				return false;
			default:
				return true;
			}
		case ec_third_forth_on_xaxis:
			switch ( quadrant_new )
			{
			case ec_third:
			case ec_second_third_on_yaxis:
				return true;
			case ec_forth:
			case ec_forth_first_on_yaxis:
				return false;
			default:
				return true;
			}
		case ec_forth_first_on_yaxis:
			switch ( quadrant_new )
			{
			case ec_forth:
			case ec_third_forth_on_xaxis:
				return true;
			case ec_first:
			case ec_first_second_on_xaxis:
				return false;
			default:
				return true;
			}
	}
	return true;
}

// 벡터를 90도 회전시킨다. clockwise가 true 면 시계방향, false 면 반시계방향
td2dvector tdorthogonal_transform ( td2dvector in_vec, bool clockwise   )
{
	td2dvector sign_vec( 1.0f, 1.0f );
	e_quadrant quadrant;

	quadrant = td_get_number_quadrant ( in_vec );
	
	if ( clockwise == true )
		sign_vec.y = -sign_vec.y;
	else
		sign_vec.x = -sign_vec.x;

	tdswap ( in_vec.x, in_vec.y );

	in_vec.x = in_vec.x * sign_vec.x;
	in_vec.y = in_vec.y * sign_vec.y;

	return in_vec;
}

/*******************************************************************************************
	name:	td2dvector
	desc:	2d vector class. 

		all code by mightbe@hitel.net
*******************************************************************************************/
td2dvector::td2dvector ( )
{
	x = 0;
	y = 0;
}
td2dvector::td2dvector ( float in_x, float in_y )
{
	x = in_x;
	y = in_y;
}

// the length of vector. 
float td2dvector::length ( )
{
	return sqrtf ( (x*x) + (y*y) );
}

// a definition of vector divide by value
td2dvector td2dvector::operator / ( float fvalue )
{
	// if there are a warning here, it will be an error of divided by zero!!
	TDASSERT ( !(fvalue < dEPSILON && fvalue > -dEPSILON) );

	x = x / fvalue;
	y = y / fvalue;
	if ( x < dEPSILON && x > -dEPSILON ) x = 0.0f;
	if ( y < dEPSILON && y > -dEPSILON ) y = 0.0f;
	return  td2dvector 
		(
			x, y
		);
}

// a definition of vector multiplication by value
td2dvector td2dvector::operator * ( float fvalue )
{
	x = x * fvalue;
	y = y * fvalue;
	if ( x < dEPSILON && x > -dEPSILON ) x = 0.0f;
	if ( y < dEPSILON && y > -dEPSILON ) y = 0.0f;
	return  td2dvector 
		(
			x, y
		);
}

// the length of vector makes 1 as a unit vector.
td2dvector td2dvector::normalize ( )
{
	float flength;
	flength = length ( );

	if ( dEPSILON > flength && flength > -dEPSILON )
	{
		flength = 0.0f;
		return *this;
	}
	*this = (*this) / flength; 
	return *this;
}

// the direction of vector makes counter direction, themselves.
td2dvector td2dvector::operator - ( )
{
	if ( x < dEPSILON && x > -dEPSILON ) x = 0.0f;
	if ( y < dEPSILON && y > -dEPSILON ) y = 0.0f;
	return td2dvector 
		( 
			-(*this).x ,
			-(*this).y 
		);
}


// a definition of vector subtraction by value
td2dvector td2dvector::operator - ( td2dvector& in_cvec )
{
	if ( x < dEPSILON && x > -dEPSILON ) x = 0.0f;
	if ( y < dEPSILON && y > -dEPSILON ) y = 0.0f;
	return td2dvector 
		( 
			(*this).x - in_cvec.x,
			(*this).y - in_cvec.y
		);
}
// a definition of vector addition by value
td2dvector td2dvector::operator + ( td2dvector& in_cvec )
{
	td2dvector addition;
	addition.x = (*this).x + in_cvec.x;
	addition.y =(*this).y + in_cvec.y;

	if ( addition.x< dEPSILON && addition.x> -dEPSILON ) addition.x = 0.0f;
	if ( addition.y< dEPSILON && addition.y> -dEPSILON ) addition.y = 0.0f;

	return td2dvector 
		( 
			addition.x,
			addition.y
		);
}
/******************************************************************************************/





/*******************************************************************************************
	name:	tdspring_motion
	desc:	it makes the object position that 
    		moves look like a coiled spring motion ( in physical science ).

		all code by mightbe@hitel.net
*******************************************************************************************/
tdspring_motion::tdspring_motion ( )
	: cur_pos ( 0, 0 ), target_pos ( 0, 0 ), 
	accelerated_velocity ( 0, 0 ), velocity ( 0, 0 ), 
	mass ( 100 ), friction ( 1.0f ) , elasticity ( 1.0f ),
	interrupt_friction ( 1.0f ) ,spring_velocity ( 0.0f, 0.0f ) ,interrupt_velocity ( 0.0f, 0.0f ),
	interrupt_elasticity ( 1.0f )

{
};
tdspring_motion::~tdspring_motion ( )
{};

// get the currently position!! 
td2dvector tdspring_motion::get_current_pos ( )
{
	return cur_pos;
}
// get the target position!!
td2dvector tdspring_motion::get_target_pos ( )
{
	return target_pos;
}
int		tdspring_motion::get_mass ( )
{
	return mass;
}
float	tdspring_motion::get_elasticity ( )
{
	return elasticity;
}
float	tdspring_motion::get_friction ( )
{
	return friction;
}
void tdspring_motion::set_friction ( float fvalue )
{
	TDASSERT ( 0.0f < fvalue && fvalue <= 1.0f);
	friction = fvalue;
}
void tdspring_motion::set_elasticity ( float fvalue )
{
	TDASSERT ( fvalue >= 1.0f);
	elasticity = fvalue;
}
void tdspring_motion::set_mass ( int nvalue )
{
	TDASSERT ( nvalue > 0 );
	mass = nvalue;
}
void tdspring_motion::set_current_pos ( int x, int y )
{
	cur_pos.x = ( float ) x; 
	cur_pos.y = ( float ) y; 
}
void tdspring_motion::set_target_pos ( int x, int y )
{
	target_pos.x = ( float ) x; 
	target_pos.y = ( float ) y; 
}
void tdspring_motion::set_interrupt( td2dvector& in_force, float fpower )
{
	interrupt_accelerated_velocity = in_force.normalize() * fpower * interrupt_elasticity;
}
void tdspring_motion::update ( )
{
	update_spring_motion ( );
	update_interrupt_motion ( );

	// applying to the friction to the velocity.
	velocity = spring_velocity + interrupt_velocity;
	// update current position.
	cur_pos = cur_pos + velocity;
}
void tdspring_motion::update_spring_motion ( )
{
	td2dvector force;

	// get the force. it is called by the law of coiled spring motion. 
	// the force = kx and the x is distance, the k is fixed by elasticity here.
	// so, the force is equal to the elasticity*distance. 
	// [force = elasticity * ( target position - current position) ] ; 
	force = ( target_pos - cur_pos ) * elasticity;

	// this is prevent for the error of divided by zero 
	// and get the accelerated_velocity
	if ( mass != 0 )
		accelerated_velocity = force / ( mass );
	else
		accelerated_velocity = force;

	spring_velocity = spring_velocity +accelerated_velocity ;
	spring_velocity = spring_velocity * friction;
}
void tdspring_motion::update_interrupt_motion ( )
{
	interrupt_velocity = interrupt_velocity + interrupt_accelerated_velocity;
	interrupt_accelerated_velocity = interrupt_accelerated_velocity * interrupt_friction;
	interrupt_velocity = interrupt_velocity * interrupt_friction;
}

void tdspring_motion::set_interrupt_friction ( float fvalue )
{
	interrupt_friction = fvalue;
}
void tdspring_motion::set_interrupt_elasticity ( float fvalue )
{
	interrupt_elasticity = fvalue;
}



/******************************************************************************************/




/*******************************************************************************************
	name:	the_fallowing_motion
	desc:	
		all code by mightbe@hitel.net
*******************************************************************************************/
the_fallowing_motion::the_fallowing_motion ( )
	: radius ( 0.0f ) , current_radian ( 0.0f ), target_radian ( 0.0f ),
	radian_accelerated_velocity( 0.0f ), radian_velocity ( 0.0f ),
	radian_friction ( 0.0f ), radian_elasticity ( 0.0f ), radian_mass ( 0.0f ), child_pos ( 0.0f, 0.0f ),
	dressup_delay( 0 ), dressup_angle ( 0 ), mode(0), flydir(0), dressup_dir(0)
{
}
the_fallowing_motion::~the_fallowing_motion ( )
{

}
// get the currently parent position. 
td2dvector the_fallowing_motion::get_parent_pos ( )
{
	return parent_pos;
}
// get the currently child position. 
td2dvector the_fallowing_motion::get_child_pos ( )
{

	switch ( mode )
	{
	case 0:
		return child_pos;
	case 1:
		return child_pos + dressup_pos;
	case 2:
		return child_pos + dressup_pos;
	}
	return child_pos;
}

td2dvector the_fallowing_motion::get_fly_pos ( )
{
	return dressup_pos;
}

// set the currently parent position
void the_fallowing_motion::set_current_parent_pos ( int x, int y )
{
	parent_pos.x = x;
	parent_pos.y = y;
}
void the_fallowing_motion::set_mode ( int ntype )
{
	mode = ntype;
}
void the_fallowing_motion::set_mass ( int nvalue )
{
	motion.set_mass( nvalue );
}
void the_fallowing_motion::set_friction ( float fvalue )
{
	motion.set_friction( fvalue );
}
void the_fallowing_motion::set_elasticity ( float fvalue )
{
	motion.set_elasticity( fvalue );
}
void the_fallowing_motion::set_rotate_mass ( float fvalue )
{
	radian_mass = fvalue;
}
void the_fallowing_motion::set_rotate_friction ( float fvalue )
{
	radian_friction = fvalue;
}
void the_fallowing_motion::set_rotate_elasticity ( float fvalue )
{
	radian_elasticity = fvalue;
}
void the_fallowing_motion::set_radius ( float fvalue )
{
	radius = fvalue;
}
void the_fallowing_motion::set_target_degree ( float fvalue )
{
	float value;
	float target_degree, current_degree;

	if ( abs ( (int)fvalue ) > 360 )
	{
		value = (float) ( (int) fvalue % 360 );
	}
	else 
		value = fvalue;

	current_degree = TD_RADIAN_TO_DEGREE ( current_radian );
	target_degree = value;

	target_radian = TD_DEGREE_TO_RADIAN ( target_degree );
	if ( current_degree > target_degree ) 
        tdswap(current_degree,target_degree);

	if ( abs( target_degree   - current_degree) >= 180 )
	{
		if ( current_radian < target_radian ) 
            target_radian -= TD_DEGREE_TO_RADIAN( (float)360);
		else
			current_radian -= TD_DEGREE_TO_RADIAN( (float)360);
	}
}
void the_fallowing_motion::update_radian( )
{
	float force;

	force = ( target_radian - current_radian ) * radian_elasticity;

	if ( radian_mass != 0 )
		radian_accelerated_velocity = force / ( radian_mass );
	else
		radian_accelerated_velocity = force;

	radian_velocity = radian_velocity + radian_accelerated_velocity ;
	radian_velocity = radian_velocity * radian_friction;

	current_radian = current_radian + radian_velocity;
}
void the_fallowing_motion::update_dressup()
{
	switch ( mode )
	{
	case 0:
		return;
	case 1:

		if ( flydir == 0 )
		{
			dressup_pos.y --;
		}
		else if ( flydir == 1 )
		{
			dressup_pos.y ++;
		}
		if ( abs(dressup_pos.y ) > 10 ) 
		{
			flydir = !flydir ;
		}
		return;
	case 2:
		
		if ( flydir == 0 )
		{
			dressup_pos.y --;
		}
		else if ( flydir == 1 )
		{
			dressup_pos.y ++;
		}
		if ( abs(dressup_pos.y ) > 10 ) 
		{
			flydir = !flydir ;
		}  

		{
			const int ndgree = 20;
			td2dvector pos;

			if ( dressup_dir==0 )
				dressup_angle +=0.5;
			else
				dressup_angle -=0.5;

			if ( dressup_angle > ndgree ) 
			{
				dressup_dir = 1;
			}
			if ( dressup_angle < -ndgree )
			{
				dressup_dir = 0;
			}

			/*
			dressup_radian = TD_DEGREE_TO_RADIAN ( dressup_angle  );
			pos.x = radius * cosf ( dressup_radian );
			pos.y = radius * sinf ( dressup_radian );
			//dressup_pos  = dressup_pos+  pos;
//			dressup_pos  = pos;
			*/
		}

		return;
	}
}

void the_fallowing_motion::stickToParent()	//	위치를 부모 위치에 맞춘다.
{
	motion.set_current_pos ( parent_pos.x, parent_pos.y);
}

void the_fallowing_motion::update ( )
{
	td2dvector pos;

	pos.x = radius * cosf ( current_radian+TD_DEGREE_TO_RADIAN ( dressup_angle  ) );
	pos.y = radius * sinf ( current_radian+TD_DEGREE_TO_RADIAN ( dressup_angle  ) );

	child_pos = motion.get_current_pos() + pos;

	motion.set_target_pos( parent_pos.x, parent_pos.y );
	motion.update();
//	update_radian ( );
//	update_dressup();
	
}
/******************************************************************************************/
	




/*******************************************************************************************
	name:	the_matrix_rotate_fallowing_motion
	desc:	
		all code by mightbe@hitel.net
*******************************************************************************************/

/*
the_matrix_rotate_fallowing_motion::the_matrix_rotate_fallowing_motion ( )
	:parent_pos ( 0,0 ), interrupt_friction ( 0.0f ), interrupt_power ( 0.0f )
{
	const int intv = 100;

	float cornerv =  (float)intv* cosf( 45 * TD_PI / 180);

	set_child_lacal_pos ( ec_upper_left, -cornerv, -cornerv  );
	set_child_lacal_pos ( ec_up, 0, -intv );
	set_child_lacal_pos ( ec_upper_right,  cornerv, -cornerv );
	set_child_lacal_pos ( ec_left,  -intv, 0 );
	set_child_lacal_pos ( ec_center,  0, 0 );
	set_child_lacal_pos ( ec_right,  intv, 0 );
	set_child_lacal_pos ( ec_down_left, -cornerv, cornerv );
	set_child_lacal_pos ( ec_down,  0, intv );
	set_child_lacal_pos ( ec_down_right,  cornerv, cornerv );
	set_direction ( ec_upper_left );
}
the_matrix_rotate_fallowing_motion::~the_matrix_rotate_fallowing_motion ( )
{
}

// get the currently parent position. 
td2dvector the_matrix_rotate_fallowing_motion::get_parent_pos ( )
{
	return parent_pos;
}
// get the currently child position. 
td2dvector the_matrix_rotate_fallowing_motion::get_child_pos ( )
{
	return motion.get_current_pos();
}
void the_matrix_rotate_fallowing_motion::set_interrupt_friction ( float fvalue )
{
	interrupt_friction = fvalue;
}
void the_matrix_rotate_fallowing_motion::set_interrupt_power ( float fvalue )
{
	interrupt_power = fvalue;
}
void the_matrix_rotate_fallowing_motion::set_interrupt_elasticity ( float fvalue )
{
	motion.set_interrupt_elasticity( fvalue );
}
void the_matrix_rotate_fallowing_motion::set_mass ( int nvalue )
{
	motion.set_mass ( nvalue );
}
void the_matrix_rotate_fallowing_motion::set_friction ( float fvalue )
{
	motion.set_friction ( fvalue );
}
void the_matrix_rotate_fallowing_motion::set_elasticity( float fvalue )
{
	motion.set_elasticity ( fvalue );
}
void the_matrix_rotate_fallowing_motion::set_direction ( e_motion_direction in_cdirect )
{
	td2dvector child_pos, parent_pos;
	td2dvector interrupt_vec;
	float		flength;
	bool clockwise;

	direct = in_cdirect;
	parent_pos = get_parent_pos();

	child_pos = parent_pos + matrix.arrays [ direct ];

	flength = (motion.get_target_pos() - child_pos).length ( );
	clockwise = td_get_clockwise ( motion.get_target_pos() - parent_pos, matrix.arrays [ direct ] );
	if ( clockwise== 0 )
		clockwise = 0;
	interrupt_vec = tdorthogonal_transform ( child_pos - motion.get_target_pos(), clockwise );

	set_current_parent_pos ( child_pos.x, child_pos.y );
	motion.set_interrupt(interrupt_vec, interrupt_power *flength );
	motion.set_interrupt_friction( interrupt_friction );

}

// the local axes of the child object
void the_matrix_rotate_fallowing_motion::set_child_lacal_pos ( e_motion_direction dir, int x, int y )
{
	matrix.arrays [ dir ].x = x;
	matrix.arrays [ dir ].y = y;
}

// set the currently parent position
void the_matrix_rotate_fallowing_motion::set_current_parent_pos ( int x, int y )
{
	td2dvector child_pos;
	child_pos  = matrix.arrays [ direct ];
	motion.set_target_pos( x + child_pos.x, y + child_pos.y );
	parent_pos.x = x;
	parent_pos.y = y;
}
// set the currently child position
void the_matrix_rotate_fallowing_motion::set_current_child_pos ( int x, int y )
{
	motion.set_current_pos( x, y );
}
void the_matrix_rotate_fallowing_motion::update ( )
{
	motion.update() ;
}
*/
/******************************************************************************************/
