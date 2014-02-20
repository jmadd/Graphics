/*
 * drawplant.cpp
 * -------------
 * Contains the drawing routine and related helper functions for the
 * L-system fractal plant.  Starter code for Project 2.
 *
 * Group Members: <FILL IN>
 */

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <assert.h>
#include <iostream>

#include "common.h"
#include "drawplant.h"
#include "readppm.h"

using namespace std;

/* Takes a 2D matrix in row-major order, and loads the 3D matrix which
   does the same trasformation into the OpenGL MODELVIEW matrix, in
   column-major order. */
void load2DMatrix(
		GLfloat m00, GLfloat m01, GLfloat m02,
		GLfloat m10, GLfloat m11, GLfloat m12,
		GLfloat m20, GLfloat m21, GLfloat m22) {

	GLfloat M3D [16];  /* three dimensional matrix doing same transform */

	M3D[0] = m00;  M3D[1] = m10; M3D[2] = 0.0; M3D[3] = m20;
	M3D[4] = m01;  M3D[5] = m11; M3D[6] = 0.0; M3D[7] = m21;
	M3D[8] = 0.0;  M3D[9] = 0.0; M3D[10] = 1.0; M3D[11] = 0.0;
	M3D[12] = m02; M3D[13] = m12; M3D[14] = 0.0; M3D[15] = m22;

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(M3D);
}

/* Takes a 3D matrix in row-major order, and loads the 3D matrix which
   does the same trasformation into the OpenGL MODELVIEW matrix, in
   column-major order. */
void load3DMatrix(
		GLfloat m00, GLfloat m01, GLfloat m02, GLfloat m03,
		GLfloat m10, GLfloat m11, GLfloat m12, GLfloat m13,
		GLfloat m20, GLfloat m21, GLfloat m22, GLfloat m23,
		GLfloat m30, GLfloat m31, GLfloat m32, GLfloat m33) {

	/* ADD YOUR CODE */
	GLfloat M3D [16];  /* three dimensional matrix doing same transform */

	M3D[0] = m00;  M3D[1] = m10; M3D[2] = m20; M3D[3] = m30;
	M3D[4] = m01;  M3D[5] = m11; M3D[6] = m21; M3D[7] = m31;
	M3D[8] = m02;  M3D[9] = m12; M3D[10] = m22; M3D[11] = m32;
	M3D[12] = m03; M3D[13] = m13; M3D[14] = m23; M3D[15] = m33;

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(M3D);
}

void load3DMatrix(GLfloat* m){
	load3DMatrix(m[0],m[1],m[2],m[3],
				m[4],m[5],m[6],m[7],
				m[8],m[9],m[10],m[11],
				m[12],m[13],m[14],m[15]);
}

//Passed in row-major order
GLfloat* vecByMat(GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3,
		GLfloat m00, GLfloat m01, GLfloat m02, GLfloat m03,
		GLfloat m10, GLfloat m11, GLfloat m12, GLfloat m13,
		GLfloat m20, GLfloat m21, GLfloat m22, GLfloat m23,
		GLfloat m30, GLfloat m31, GLfloat m32, GLfloat m33){

	GLfloat* result = new GLfloat[4];
	result[0] = v0*m00+v1*m01+v2*m02+v3*m03;
	result[1] = v0*m10+v1*m11+v2*m12+v3*m13;
	result[2] = v0*m20+v1*m21+v2*m22+v3*m23;
	result[3] = v0*m30+v1*m31+v2*m32+v3*m33;

	return result;
}

//Matrix is passed in row-major order
GLfloat* vecByMat(GLfloat* v, GLfloat* m){
	return vecByMat(v[0],v[1],v[2],v[3],
			m[0],m[1],m[2],m[3],
			m[4],m[5],m[6],m[7],
			m[8],m[9],m[10],m[11],
			m[12],m[13],m[14],m[15]);
}


GLfloat* matByMat(GLfloat* A, GLfloat* B){
	GLfloat* result = new GLfloat[16];

	for(int r = 0; r < 4; r++){
		for(int c = 0; c < 4; c++){
			int ri = r*4;
			int ci = c;
			result[ri+ci]=A[ri]*B[ci]+A[ri+1]*B[ci+4]+A[ri+2]*B[ci+8]+A[ri+3]*B[ci+12];
		}
	}

	return result;
}


void printMatrix(GLfloat* m){
	for(int i = 0; i < 4; i++){
		int I = i*4;
		printf("%f %f %f %f\n",m[I],m[I+1],m[I+2],m[I+3]);
	}
}



//------------------------------------------------------------------

void drawLeaf(void) {
	/* ADD YOUR CODE to make the 2D leaf a 3D extrusion */
	


	glColor3f(0.1,0.9,0.1); 
	glBegin(GL_POLYGON);
	glVertex2f(0.0,0.0);
	glVertex2f(1.0,0.7);
	glVertex2f(1.3,1.8);
	glVertex2f(1.0,2.8);
	glVertex2f(0.0,4.0);
	glVertex2f(-1.0,2.8);
	glVertex2f(-1.3,1.8);
	glVertex2f(-1.0,0.7);
	glEnd();

	
}

void drawBranch(void) {
	/* ADD YOUR CODE to make the 2D branch a 3D extrusion */
	glColor3f(0.54,0.27,0.07); 
	glBegin(GL_POLYGON);
	glVertex2f(1.0,0.0);
	glVertex2f(1.0,6.0);
	glVertex2f(-1.0,6.0);
	glVertex2f(-1.0,0.0);
	glEnd();
}

/*
 * Draws the plant.
 *
 * ADD YOUR CODE and modify the function to take an L-system depth and
 * any other necessary arguments.
 */
void drawPlant(void) {

	/* Load a hard-coded rotation matrix of -30 degrees about positive z */
	/* This matrix is only here as an example, and can be removed */
	load2DMatrix(
	       sqrt(3.0)/2.0, -1.0/2.0,      0.0,
		   1.0/2.0,       sqrt(3.0)/2.0, 0.0,
		   0.0,           0.0,           1.0);

	/*
	 * The location of the leaf and branch will not look right until
	 * transformation matrices are implmented.
	 */
	drawLeaf();

	drawBranch();
}

/* end of drawplant.c */
