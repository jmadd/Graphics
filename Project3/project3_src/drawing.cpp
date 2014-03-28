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



void drawPoints(void) {


    glColor3f(0.0f,0.0f,1.0f); //blue color
	glPointSize(3.0f);
	glBegin(GL_POINTS);
	for(int i = 0; i <= num_draw_pts; i++) {
		//Conversion of ratios between input and output
		int x = draw_x[i];
		int y = draw_y[i];
		//printf("(%d, %d) should be drawn\n", x, y);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();
}

void drawLines(void) {
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	for(int i = 0; i <= num_draw_pts; i++) {
		//Conversion of ratios between input and output
		int x = draw_x[i];
		int y = draw_y[i];
		//printf("(%d, %d) should be drawn\n", x, y);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();

}

void drawSurfaceSolid(void){
	int n = num_draw_pts+1;

	glColor3f(0.0f,0.0f,1.0f); //blue color
	int k = ((int)(3*pow(2.0f,subdiv_h)) - 1);
	for(int j = 0; j < (int)(3*pow(2.0f,subdiv_h)); j++){
		
		for(int i = 1; i < n; i++){
			glBegin(GL_POLYGON);
			GLfloat x1 = draw_x[i+k*n-1];
			GLfloat y1 = draw_y[i+k*n-1];
			GLfloat z1 = draw_z[i+k*n-1];
			
			GLfloat x2 = draw_x[i+j*n-1];
			GLfloat y2 = draw_y[i+j*n-1];
			GLfloat z2 = draw_z[i+j*n-1];

			GLfloat x3 = draw_x[i+j*n];
			GLfloat y3 = draw_y[i+j*n];
			GLfloat z3 = draw_z[i+j*n];

			GLfloat x4 = draw_x[i+k*n];
			GLfloat y4 = draw_y[i+k*n];
			GLfloat z4 = draw_z[i+k*n];
		
			glVertex3f(x1,y1,z1);
			glVertex3f(x2,y2,z2);
			glVertex3f(x3,y3,z3);
			glVertex3f(x4,y4,z4);

			glEnd();
		}
		k=j;
	}
}

void drawSurfaceWireframe(void){
	int n = num_draw_pts+1;

	glColor3f(0.0f,0.0f,1.0f); //blue color
	int k = ((int)(3*pow(2.0f,subdiv_h)) - 1);
	for(int j = 0; j < (int)(3*pow(2.0f,subdiv_h)); j++){
		
		for(int i = 1; i < n; i++){
			glBegin(GL_LINE_STRIP);
			GLfloat x1 = draw_x[i+k*n-1];
			GLfloat y1 = draw_y[i+k*n-1];
			GLfloat z1 = draw_z[i+k*n-1];
			
			GLfloat x2 = draw_x[i+j*n-1];
			GLfloat y2 = draw_y[i+j*n-1];
			GLfloat z2 = draw_z[i+j*n-1];

			GLfloat x3 = draw_x[i+j*n];
			GLfloat y3 = draw_y[i+j*n];
			GLfloat z3 = draw_z[i+j*n];

			GLfloat x4 = draw_x[i+k*n];
			GLfloat y4 = draw_y[i+k*n];
			GLfloat z4 = draw_z[i+k*n];
		
			glVertex3f(x1,y1,z1);
			glVertex3f(x2,y2,z2);
			glVertex3f(x3,y3,z3);
			glVertex3f(x4,y4,z4);
			glVertex3f(x1,y1,z1);
			glEnd();
		}
		k=j;
	}

}

void drawSurfacePoints(void) {
	int n = num_draw_pts+1;

	glColor3f(0.0f,0.0f,1.0f); //blue color
	
	glBegin(GL_POINTS);
	for(int j = 0; j < (int)(3*pow(2.0f,subdiv_h)); j++){
		for(int i = 0; i < n; i++){
		
			GLfloat x = draw_x[i+j*n];
			GLfloat y = draw_y[i+j*n];
			GLfloat z = draw_z[i+j*n];
		
			glVertex3f(x,y,z);
		
		}
	}
	glEnd();
}
/* end of drawing.cpp */
