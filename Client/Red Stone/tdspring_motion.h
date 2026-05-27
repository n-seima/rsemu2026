/*******************************************************************************************
	tdspring_motion.h
*******************************************************************************************/


/*******************************************************************************************
	name:	td2dvector
	desc:	2d vector class. 

		all code by mightbe@hitel.net
*******************************************************************************************/
#define dEPSILON 0.0001f
class td2dvector
{
public:
	td2dvector ( );
	td2dvector ( float in_x, float in_y );
	
	// the length of vector. 
	float length ( );

	// a definition of vector divide by value
	td2dvector operator / ( float fvalue );

	// a definition of vector multiplication by value
	td2dvector operator * ( float fvalue );

	// the length of vector makes 1 as a unit vector.
	td2dvector normalize ( );

	// the direction of vector makes counter direction, themselves.
	td2dvector operator - ( );

	// a definition of vector subtraction by value
	td2dvector operator - ( td2dvector& in_cvec );

	// a definition of vector addition by value
	td2dvector operator + ( td2dvector& in_cvec );

public:
	float x;
	float y;
};
/******************************************************************************************/


/*******************************************************************************************
	name:	tdspring_motion
	desc:	it makes the object position that 
    		moves look like a coiled spring motion ( in physical science ).

		all code by mightbe@hitel.net
*******************************************************************************************/
class tdspring_motion
{
public:
	tdspring_motion ( );
	~tdspring_motion ( );

	// get the currently position!! 
	td2dvector get_current_pos ( );

	// get the target position!!
	td2dvector get_target_pos ( );

	int			get_mass ( );
	float		get_elasticity ( );
	float		get_friction ( );

	void set_friction ( float fvalue );
	void set_elasticity ( float fvalue );

	void set_mass ( int nvalue );

	void set_current_pos ( int x, int y );

	void set_target_pos ( int x, int y );

	void set_interrupt ( td2dvector& in_force, float fpower );
	void set_interrupt_elasticity ( float fvalue );
	void set_interrupt_friction ( float fvalue );

	void update ( );

private:
	void update_spring_motion ( );
	void update_interrupt_motion ( );

private:
	int		   mass;	
	float	   elasticity;
	float      friction;
	float	   interrupt_friction ;
	float	   interrupt_elasticity;
	td2dvector interrupt_accelerated_velocity;
	td2dvector accelerated_velocity;
	td2dvector velocity;
	td2dvector spring_velocity;
	td2dvector interrupt_velocity;

	// the current position.
	td2dvector cur_pos;
	// the target position.
	td2dvector target_pos;
};
/******************************************************************************************/



/*******************************************************************************************
	name:	the_fallowing_motion
	desc:	
		all code by mightbe@hitel.net
*******************************************************************************************/
class the_fallowing_motion
{
public:
	the_fallowing_motion ( );
	~the_fallowing_motion ( );
	
	// get the currently parent position. 
	td2dvector get_parent_pos ( );

	// get the currently child position. 
	td2dvector get_child_pos ( );

	// get the shadow pos
	td2dvector get_fly_pos ( );

	// set the currently parent position
	void set_current_parent_pos ( int x, int y );
	void stickToParent();	//	위치를 부무 위치에 맞춘다.

	void set_mode ( int ntype );
	void set_mass ( int nvalue );
	void set_friction ( float fvalue );
	void set_elasticity ( float fvalue );

	void set_rotate_mass ( float fvalue );
	void set_rotate_friction ( float fvalue );
	void set_rotate_elasticity ( float fvalue );

	void set_radius ( float fvalue );
	void set_target_degree ( float fvalue );

	void update_radian ( );
	void update_dressup();
	void update ( );
protected:
	float						radian_mass;
	float						radian_friction;
	float						radian_elasticity;
	float						radian_accelerated_velocity;
	float						radian_velocity;
	float						current_radian;
	float						target_radian;
	float						radius;
	tdspring_motion				motion;
	td2dvector					parent_pos;
	td2dvector					child_pos;
	td2dvector					dressup_pos;
	int							dressup_delay;
	float						dressup_angle;

	int							mode;
	int flydir;
	int dressup_dir;

};
/******************************************************************************************/


/*******************************************************************************************
	name:	the_matrix_rotate_fallowing_motion
	desc:	
		all code by mightbe@hitel.net
*******************************************************************************************/
/*
struct the_pos_matrix
{
	union
	{
		struct 
		{
			td2dvector _11, _12, _13;
			td2dvector _21, _22, _23;
			td2dvector _31, _32, _33;
		} ;
		struct 
		{
			td2dvector arrays[9];
		};
	}; 
};

enum e_motion_direction
{
	ec_upper_left = 0,
	ec_up,
	ec_upper_right,
	ec_left,
	ec_center,
	ec_right,
	ec_down_left,
	ec_down,
	ec_down_right
};

class the_matrix_rotate_fallowing_motion
{
public:

	the_matrix_rotate_fallowing_motion ( );
	~the_matrix_rotate_fallowing_motion ( );

	// get the currently parent position. 
	td2dvector get_parent_pos ( );

	// get the currently child position. 
	td2dvector get_child_pos ( );

	void set_interrupt_friction ( float fvalue );
	void set_interrupt_power ( float fvalue );
	void set_interrupt_elasticity ( float fvalue );

	void set_mass ( int nvalue );
	void set_friction ( float fvalue );
	void set_elasticity ( float fvalue );

	void set_direction ( e_motion_direction in_cdirect );

	// the local axes of the child object
	void set_child_lacal_pos ( e_motion_direction dir, int x, int y );
	// set the currently parent position
	void set_current_parent_pos ( int x, int y );
	// set the currently child position
	void set_current_child_pos ( int x, int y );
	void update ( );
	
private:
	float						interrupt_friction;
	float						interrupt_power;
	e_motion_direction			direct;
	tdspring_motion				motion;
	the_pos_matrix				matrix;
	td2dvector					parent_pos;
	
};
*/
/******************************************************************************************/
