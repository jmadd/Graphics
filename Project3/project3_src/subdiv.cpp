/*
 * subdiv.cpp
 * ----------
 * Contains function callbacks and bookkeeping for window management.
 *
 * Group Members: <FILL IN>
 */

#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "common.h"
#include "drawing.h"
#include "data.h"

/* GLOBAL VARAIBLES */
/* (storage is actually allocated here) */
int W=400;		/* window width */
int H=400;		/* window height */
int X_OFF = 10;	/* window x offset */
int Y_OFF = 10;	/* window y offset */

/* The dimensions of the viewing frustum */
GLfloat fleft   = -40.0;
GLfloat fright  =  40.0;
GLfloat fbottom = -40.0;
GLfloat ftop    =  40.0;
GLfloat zNear   =  40.0;
GLfloat zFar    = -40.0;

bool mode3d = false;
bool wireframe = false;
bool disp_points = false;

/* local function declarations */
void init(void);
void display(void);
void myKeyHandler(unsigned char ch, int x, int y);
void myMouseButton(int button, int state, int x, int y);
void endSubdiv(int status);

int main (int argc, char** argv) {
  glutInit(&argc,argv);
  glutInitWindowSize(W, H);
  glutInitWindowPosition(X_OFF, Y_OFF);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("subdiv");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(myKeyHandler);
  glutMouseFunc(myMouseButton);
  glutMainLoop();
  return 0;
}

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);  
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(fleft, fright, fbottom, ftop, -zNear, -zFar);
	num_i0_pts = -1;
	
}

void display() {
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	/*
	 * See drawing.c for the definition of these routines.
	 *
	 * Note: Only one should be called at a time (based on the
	 * display mode).
	 */
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, -40.0f, 3.2f);
	glVertex3f(0.0f, 40.0f, 3.2f);
	glEnd();


	if(mode3d){
		if(disp_points) {
			drawSurfacePoints();
		}
		else if(wireframe)
			drawSurfaceWireframe();
		else 
			drawSurfaceSolid();
	}else{
		drawPoints();
		drawLines();
	}	
    glFlush();  /* Flush all executed OpenGL ops finish */

    /*
     * Since we are using double buffers, we need to call the swap
     * function every time we are done drawing.
     */
    glutSwapBuffers();
}

void myKeyHandler(unsigned char ch, int x, int y) {
	switch(ch) {
		case 'z':
			mode3d = (mode3d) ? false : true;
			break;
		case 'e':
			wireframe = (wireframe) ? false: true;
			break;
		case 'r':
			disp_points = (disp_points) ? false: true;
			break;
		case 'q':
			endSubdiv(0);
			break;
		default:
			/* Unrecognized keypress */
		return;
	}

	glutPostRedisplay();

	return;
}

void myMouseButton(int button, int state, int x, int y) {
	if (state == GLUT_DOWN && mode3d == false) {
		if (button == GLUT_LEFT_BUTTON) {
			// Add a point, if there is room
			if(x >= 195 && x <= 205) {
				x = 200;
			}

			if(num_i0_pts < MAX_POINT && x >= 200) {
				printf("Point %d added at x: %3d, y: %3d\n", num_i0_pts, x, y);
				num_i0_pts++;
				i0_x[num_i0_pts] = x/5 - 40;
				i0_y[num_i0_pts] = (y/5 - 40) * -1;
			}
			else {
				printf("No more points can be added.\n");
			}
		}
		if (button == GLUT_RIGHT_BUTTON) {
			if(num_i0_pts >= 0) {
				printf("Point %d deleted\n", num_i0_pts);
				i0_x[num_i0_pts] = 0;
				i0_y[num_i0_pts] = 0;
				num_i0_pts--;
			}
		}
	}
	display();
}

void endSubdiv(int status) {
  printf("\nQuitting subdivision program.\n\n");
  fflush(stdout);

  exit(status);
}


/* end of subdiv.cpp */