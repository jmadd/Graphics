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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "common.h"
#include "drawplant.h"

/* GLOBAL VARAIBLES */
/* (storage is actually allocated here) */
int W=800;		/* window width */
int H=600;		/* window height */
int X_OFF = 10;	/* window x offset */
int Y_OFF = 10;	/* window y offset */

/* local function declarations */
void display(void);
void init(void);

int main (int argc, char** argv) {
  glutInit(&argc,argv);
  glutInitWindowSize(W, H);
  glutInitWindowPosition(X_OFF, Y_OFF);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("plant");
  init();

  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}

void init() {
  glClearColor(0.0, 0.0, 0.0, 0.0);  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-40.0, 40.0, -40.0, 40.0, -10.0, 10.0);
}


void display() {
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	/* See drawplant.c for the definition of this routine */
  GLfloat tx = 10, ty = 1, tz = 0;
  GLfloat A[16] = {1,0,0,tx,
                0,1,0,ty,
                0,0,1,tz,
                0,0,0,1};
  GLfloat R[16] = {0,-1,0,0,
                1,0,0,0,
                0,0,1,0,
                0,0,0,1};
  GLfloat I[16] = {1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                0,0,0,1};

  GLfloat* result = matByMat(A,R);
  
  load3DMatrix(result);

	drawLeaf();

  load3DMatrix(I);

  A[3]=-10;

  load3DMatrix(A);

  drawLeaf();

  load3DMatrix(I);


    glFlush();  /* Flush all executed OpenGL ops finish */

    /*
     * Since we are using double buffers, we need to call the swap
     * function every time we are done drawing.
     */
    glutSwapBuffers();

}

/* end of plant.c */
