#ifndef _DATA_H_
#define _DATA_H_

/* Globals declared in data.cpp */

// The maximum number of points allowed in the 4 Point Scheme
#define MAX_POINT   30

extern GLfloat i0_x[MAX_POINT];	// Iteration 0 control points, x
extern GLfloat i0_y[MAX_POINT];	// Iteration 0 control points, y
extern int num_i0_pts;			// The number of iteration 0 control points

extern GLfloat *draw_x;		// Control points to be drawn, x
extern GLfloat *draw_y;		// Control points to be drawn, y
extern GLfloat *draw_z;
extern int num_draw_pts;    // The number of control points to draw

extern bool mode3d;
extern bool wireframe;
extern bool disp_points;

struct vertex {
	GLfloat* location;
	GLfloat* normal;
	GLfloat* color;
};

struct poly{
	vertex* verts;
	GLfloat* normal;
};

/* Functions implemented in data.cpp */
void subdividePointsArray(int subdiv_level);
void subdividePointsArrayH(int subdiv_level);
void createEnvironment(void);

#endif	/* _DATA_H_ */
