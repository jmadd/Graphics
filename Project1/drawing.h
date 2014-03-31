#ifndef _DRAWING_H_
#define _DRAWING_H_


/* All variables / contants declared in this file are global. */

/*
 * Different drawing modes.  The drawing modes should begin at 0.
 * and should be consecutive integers.
 * 
 * DM_MAX should be the total number of drawing modes.
 */
#define DM_CUBE_GLUT            0
#define DM_CUBE_QUAD            1
#define DM_CUBE_QUAD_ARRAYS     2
#define DM_CONE_GLUT            3
#define DM_CONE_TRI             4
#define DM_CONE_TRI_ARRAYS      5
#define DM_CONE_TRI_CALC        6
#define DM_VRML                 7
#define DM_FREE_SCENE           8
#define DM_MAX                  9

#define MAX_RECURSION 3

#include <vector>
#include <math.h>

using namespace std;
/* The current display mode */
extern int disp_mode;


/* Styles of drawing glut objects, either solid or wire-frame */
#define DS_SOLID        0
#define DS_WIRE         1

/* The current display style */
extern int disp_style;
extern int recursion_level;
extern int curr_rec; //selected_recursion_level
extern int vr_object;


class Vector3f{
public:
	GLfloat _x;
	GLfloat _y;
	GLfloat _z;

	Vector3f(){
		_x=0;_y=0;_z=0;
	}

	Vector3f(GLfloat x, GLfloat y, GLfloat z){
		_x=x;_y=y;_z=z;
	}
	GLfloat lengthSq(){
		return _x*_x+_y*_y+_z*_z;
	}
	GLfloat length(){
		return sqrtf(lengthSq());
	}
	void normalize(){
		GLfloat len = length();
		_x/=len;
		_y/=len;
		_z/=len;
	}
	void scale(GLfloat scl){
		_x*=scl;
		_y*=scl;
		_z*=scl;
	}
};

class Vector3ui{
public:
	GLuint _x;
	GLuint _y;
	GLuint _z;

	Vector3ui(){
		_x=0;_y=0;_z=0;
	}

	Vector3ui(GLuint x, GLuint y, GLuint z){
		_x=x;_y=y;_z=z;
	}
	GLuint lengthSq(){
		return _x*_x+_y*_y+_z*_z;
	}
	GLuint length(){
		return sqrt(lengthSq());
	}
	void normalize(){
		GLuint len = length();
		_x/=len;
		_y/=len;
		_z/=len;
	}
	void scale(GLuint scl){
		_x*=scl;
		_y*=scl;
		_z*=scl;
	}
};

extern vector<Vector3f> ico_vertsq;
extern vector<Vector3ui> ico_indicesq;

/* Function Declarations */
void init_shapes(void);
void draw_cube_glut(void);
void draw_cube_quad(void);
void draw_cube_quad_arrays(void);
void draw_cone_glut(void);
void draw_cone_tri(void);
void draw_cone_tri_arrays(void);
void draw_cone_tri_calc(double height, double radius, int base_tri);
void draw_vrml(void);
void draw_free_scene(void);
void print_disp_mode(void);
void print_disp_style(void);
void draw_sphere(int rec);
Vector3f getMidPoint(Vector3f v1, Vector3f v2);
#endif	/* _DRAWING_H_ */

