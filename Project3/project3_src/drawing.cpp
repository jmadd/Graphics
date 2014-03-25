/*
 * drawing.cpp
 * -----------
 * Contains the drawing routines and related helper functions for the
 * subdivision surface
 *
 * Group Members: <FILL IN>
 */

#ifdef _WIN32
#include <windows.h>
#endif
#include <math.h>
#include <assert.h>
#include <stdio.h>


#include "common.h"
#include "drawing.h"
#include "data.h"

/* Globals */
int subdiv_v; // The user-specified subdivision level, vertical
int subdiv_h; // The user-specified subdivision level, horizontal

/* The parameter list may need to be changed for the functions in this file */

/*void drawSurface(void) {
	GLfloat rot120x [num_i0_pts+1];
	GLfloat rot120z [num_i0_pts+1];
	GLfloat rot240x [num_i0_pts+1];
	GLfloat rot240z [num_i0_pts+1];

	for(int i = 0; i <= num_i0_pts; i++) {
		rot120x[i] = -1/2 * i0_x[i];
		rot120z[i] = .86603 * i0_x[i];
		rot240x[i] = rot120x[i];
		rot240z[i] = rot120z[i] * -1;
	}

	glColor3f(0.0f,0.0f,1.0f); //blue color
	glPointSize(3.0f);
	glBegin(GL_POINTS);
	for(int i = 0; i <= num_i0_pts; i++) {
		//Conversion of ratios between input and output
		int x = i0_x[i];
		int y = i0_y[i];
		//printf("(%d, %d) should be drawn\n", x, y);
		glVertex3f(x, y, 0.0f);
	}

	for(int i = 0; i <= num_i0_pts; i++) {
		//Conversion of ratios between input and output
		int x = rot120x[i];
		int y = i0_y[i];
		int z = rot120z[i];
		//printf("(%d, %d) should be drawn\n", x, y);
		glVertex3f(x, y, z);
	}

	glEnd();
}
*/
void drawPoints(void) {


    glColor3f(0.0f,0.0f,1.0f); //blue color
	glPointSize(3.0f);
	glBegin(GL_POINTS);
	for(int i = 0; i <= num_i0_pts; i++) {
		//Conversion of ratios between input and output
		int x = i0_x[i];
		int y = i0_y[i];
		//printf("(%d, %d) should be drawn\n", x, y);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();
}

void drawLines(void) {
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	for(int i = 0; i <= num_i0_pts; i++) {
		//Conversion of ratios between input and output
		int x = i0_x[i];
		int y = i0_y[i];
		//printf("(%d, %d) should be drawn\n", x, y);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();

}
/* end of drawing.cpp */
