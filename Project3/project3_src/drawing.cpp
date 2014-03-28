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
	int i = 0;
	GLfloat x1 = draw_x[i];
	GLfloat y1 = draw_y[i];
	GLfloat z1 = draw_z[i];
	GLfloat x2 = draw_x[i+n];
	GLfloat y2 = draw_y[i+n];
	GLfloat z2 = draw_z[i+n];
	for(i = 1; i <= num_draw_pts; i++){
		glBegin(GL_POLYGON);
		
		glVertex3f(x1,y1,z1);
		glVertex3f(x2,y2,z2);
		x1 = draw_x[i];
		y1 = draw_y[i];
		z1 = draw_z[i];
		x2 = draw_x[i+n];
		y2 = draw_y[i+n];
		z2 = draw_z[i+n];
		
		glVertex3f(x2,y2,z2);
		glVertex3f(x1,y2,z1);
		glEnd();
	}

	i = 0;
	x1 = draw_x[i];
	y1 = draw_y[i];
	z1 = draw_z[i];
	GLfloat x3 = draw_x[i+n*2];
	GLfloat y3 = draw_y[i+n*2];
	GLfloat z3 = draw_z[i+n*2];
	for(i = 1; i <= num_draw_pts; i++){
		glBegin(GL_POLYGON);
		
		glVertex3f(x1,y1,z1);
		glVertex3f(x3,y3,z3);
		x1 = draw_x[i];
		y1 = draw_y[i];
		z1 = draw_z[i];
		x3 = draw_x[i+n*2];
		y3 = draw_y[i+n*2];
		z3 = draw_z[i+n*2];
		
		glVertex3f(x3,y3,z3);
		glVertex3f(x1,y1,z1);
		glEnd();
	}

	i = 0;
	x3 = draw_x[i+n*2];
	y3 = draw_y[i+n*2];
	z3 = draw_z[i+n*2];
	x2 = draw_x[i+n];
	y2 = draw_y[i+n];
	z2 = draw_z[i+n];
	for(i = 1; i <= num_draw_pts; i++){
		glBegin(GL_POLYGON);
		
		glVertex3f(x3,y3,z3);
		glVertex3f(x2,y2,z2);
		x3 = draw_x[i+n*2];
		y3 = draw_y[i+n*2];
		z3 = draw_z[i+n*2];
		x2 = draw_x[i+n];
		y2 = draw_y[i+n];
		z2 = draw_z[i+n];
		
		glVertex3f(x2,y2,z2);
		glVertex3f(x3,y3,z3);
		glEnd();
	}
}

void drawSurfaceWireframe(void){
	int n = num_draw_pts+1;

	glColor3f(0.0f,0.0f,1.0f); //blue color
	int i = 0;
	GLfloat x1 = draw_x[i];
	GLfloat y1 = draw_y[i];
	GLfloat z1 = draw_z[i];
	GLfloat x2 = draw_x[i+n];
	GLfloat y2 = draw_y[i+n];
	GLfloat z2 = draw_z[i+n];
	for(i = 1; i <= num_draw_pts; i++){
		glBegin(GL_LINE_STRIP);
		
		glVertex3f(x1,y1,z1);
		glVertex3f(x2,y2,z2);
		x1 = draw_x[i];
		y1 = draw_y[i];
		z1 = draw_z[i];
		x2 = draw_x[i+n];
		y2 = draw_y[i+n];
		z2 = draw_z[i+n];
		
		glVertex3f(x2,y2,z2);
		glVertex3f(x1,y2,z1);
		glVertex3f(draw_x[i-1],draw_y[i-1],draw_z[i-1]);
		glEnd();
	}

	i = 0;
	x1 = draw_x[i];
	y1 = draw_y[i];
	z1 = draw_z[i];
	GLfloat x3 = draw_x[i+n*2];
	GLfloat y3 = draw_y[i+n*2];
	GLfloat z3 = draw_z[i+n*2];
	for(i = 1; i <= num_draw_pts; i++){
		glBegin(GL_LINE_STRIP);
		
		glVertex3f(x1,y1,z1);
		glVertex3f(x3,y3,z3);
		x1 = draw_x[i];
		y1 = draw_y[i];
		z1 = draw_z[i];
		x3 = draw_x[i+n*2];
		y3 = draw_y[i+n*2];
		z3 = draw_z[i+n*2];
		
		glVertex3f(x3,y3,z3);
		glVertex3f(x1,y1,z1);
		glVertex3f(draw_x[i-1],draw_y[i-1],draw_z[i-1]);
		glEnd();
	}

	i = 0;
	x3 = draw_x[i+n*2];
	y3 = draw_y[i+n*2];
	z3 = draw_z[i+n*2];
	x2 = draw_x[i+n];
	y2 = draw_y[i+n];
	z2 = draw_z[i+n];
	for(i = 1; i <= num_draw_pts; i++){
		glBegin(GL_LINE_STRIP);
		
		glVertex3f(x3,y3,z3);
		glVertex3f(x2,y2,z2);
		x3 = draw_x[i+n*2];
		y3 = draw_y[i+n*2];
		z3 = draw_z[i+n*2];
		x2 = draw_x[i+n];
		y2 = draw_y[i+n];
		z2 = draw_z[i+n];
		
		glVertex3f(x2,y2,z2);
		glVertex3f(x3,y3,z3);
		glVertex3f(draw_x[(i+n*2)-1],draw_y[(i+n*2)-1],draw_z[(i+n*2)-1]);
		glEnd();
	}
}

void drawSurfacePoints(void) {
	int n = num_draw_pts+1;

	glColor3f(0.0f,0.0f,1.0f); //blue color
	int i = 0;
	GLfloat x1 = draw_x[i];
	GLfloat y1 = draw_y[i];
	GLfloat z1 = draw_z[i];
	GLfloat x2 = draw_x[i+n];
	GLfloat y2 = draw_y[i+n];
	GLfloat z2 = draw_z[i+n];
	glBegin(GL_POINTS);
	for(i = 0; i <= num_draw_pts; i++){
		
		x2 = draw_x[i+n];
		y2 = draw_y[i+n];
		z2 = draw_z[i+n];
		
		glVertex3f(x2,y2,z2);
		
	}

	i = 0;
	
	for(i = 0; i <= num_draw_pts; i++){
		
		x1 = draw_x[i];
		y1 = draw_y[i];
		z1 = draw_z[i];
		glVertex3f(x1,y1,z1);
		
	}

	i = 0;
	GLfloat x3 = draw_x[i+n*2];
	GLfloat y3 = draw_y[i+n*2];
	GLfloat z3 = draw_z[i+n*2];
	for(i = 0; i <= num_draw_pts; i++){
	
		x3 = draw_x[i+n*2];
		y3 = draw_y[i+n*2];
		z3 = draw_z[i+n*2];
		glVertex3f(x3,y3,z3);
		
	}
	glEnd();
}
/* end of drawing.cpp */
