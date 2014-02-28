/*
 * plant.cpp
 * ---------
 * Contains function callbacks and bookkeeping used by glut.  Starter code
 * for Project 2.
 *
 * Group Members: <FILL IN>
 */

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "common.h"
#include "mouse.h"
#include "drawplant.h"


#define THROTTLE_MAX 2.0F
#define THROTTLE_MIN 0.5F
#define DEPTH_MAX 7


/* The dimensions of the viewing frustum */
GLfloat fleft   = -400.0;
GLfloat fright  =  400.0;
GLfloat fbottom = -50.0;
GLfloat ftop    =  500.0;
GLfloat zNear   = 1000.0;
GLfloat zFar    = -1000.0;


/* Global zoom factor.  Modified by user input. Initially 1.0 */
GLfloat zoomFactor = 1.0; 


/* GLOBAL VARAIBLES */
/* (storage is actually allocated here) */
int W=800;		/* window width */
int H=600;		/* window height */
int X_OFF = 10;	/* window x offset */
int Y_OFF = 10;	/* window y offset */


GLfloat throttle = 0.5f;
bool ortho = true;



/* local function declarations */
void display(void);
void init(void);
void resetCamera(void);
int endPlant(int status);
void myKeyHandler (unsigned char, int, int);

void idle(void){
	rotation -=.5f;
	rotation = (rotation > 360) ? 0 : rotation;
	glutPostRedisplay();
}

int main (int argc, char** argv) {
  glutInit(&argc,argv);
  glutInitWindowSize(W, H);
  glutInitWindowPosition(X_OFF, Y_OFF);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("plant");
  init();

  glutKeyboardFunc(myKeyHandler);
  glutDisplayFunc(display);
  glutMouseFunc(myMouseButton);
  glutMotionFunc(myMouseMotion);
  //glutIdleFunc(idle);
  glutMainLoop();
  return 0;
}

void init() {
  glClearColor(0.0, 0.0, 0.0, 0.0);  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if(ortho) glOrtho(fleft, fright, fbottom, ftop, zFar, zNear);
  else{
    //Perspective
    gluPerspective(30,(fright-fleft)/(ftop-fbottom),1,zNear-zFar);
    glTranslatef(
    0,
    -(ftop+fbottom)/2,
    (-zNear));

  }
  
}

int endPlant(int status) {
  printf("\nQuitting Plant.\n\n");
  fflush(stdout);

  exit(status);
}



void display() {
	glEnable(GL_DEPTH_TEST);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	/* See drawplant.c for the definition of this routine */
	
	drawPlant();

    glFlush();  /* Flush all executed OpenGL ops finish */

    /*
     * Since we are using double buffers, we need to call the swap
     * function every time we are done drawing.
     */
    glutSwapBuffers();

}

void myKeyHandler(unsigned char ch, int x, int y) {
  switch(ch) {
    case '/':
      rotation-=throttle;
      rotation = (rotation < 0) ? 360 : rotation;
      break;
    case '?':
      rotation+=throttle;
      rotation = (rotation > 360) ? 0 : rotation;
      break;
    case ']':
      throttle+=0.1f;
      throttle = (throttle > THROTTLE_MAX) ? THROTTLE_MAX : throttle;
      break;
    case '[':
      throttle-=0.1f;
      throttle = (throttle < THROTTLE_MIN) ? THROTTLE_MIN : throttle;
      break;
    case 'a':
      depth+=1;
      depth = (depth > DEPTH_MAX) ? DEPTH_MAX : depth;
      printf("Depth increased to: %d\n", depth);
      break;
    case 's':
      depth-=1;
      depth = (depth < 0) ? 0 : depth; 
      printf("Depth decreased to: %d\n", depth);
      break;
    case 'p':
      ortho = ortho ^ true;
      init();
      printf("Viewing mode changed\n");
      break;
    case 'q':
      /* Quit with exit code 0 */
      endPlant(0);
      break;

    default:
      /* Unrecognized key press, just return */
      return;

      break;
  }

  /*
   * If control reaches here, the key press was recognized.  Refresh
   * the screen, since most key presses change the display in some way.
   */
  glutPostRedisplay();

  return;
}

/* end of plant.c */