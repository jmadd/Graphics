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
	GLfloat* rot120x = (GLfloat *)malloc((num_draw_pts+1) * sizeof(GLfloat));
	GLfloat* rot120z = (GLfloat *)malloc((num_draw_pts+1) * sizeof(GLfloat));
	GLfloat* rot240x = (GLfloat *)malloc((num_draw_pts+1) * sizeof(GLfloat));
	GLfloat* rot240z = (GLfloat *)malloc((num_draw_pts+1) * sizeof(GLfloat));
	
	for(int i = 0; i <= num_draw_pts; i++) {
		rot120x[i] = -0.5f * draw_x[i];
		rot120z[i] = .86603 * draw_x[i];
		rot240x[i] = rot120x[i];
		rot240z[i] = rot120z[i] * -1;
	}

	glColor3f(0.0f,0.0f,1.0f); //blue color
	int i = 0;
	GLfloat x1 = draw_x[i];
	GLfloat y = draw_y[i];
	GLfloat z1 = 0;
	GLfloat x2 = rot120x[i];
	GLfloat z2 = rot120z[i];
	for(i = 1; i <= num_draw_pts; i++){
		glBegin(GL_POLYGON);
		
		glVertex3f(x1,y,z1);
		glVertex3f(x2,y,z2);
		x1 = draw_x[i];
		y = draw_y[i];
		z1 = 0;
		x2 = rot120x[i];
		z2 = rot120z[i];
		
		glVertex3f(x2,y,z2);
		glVertex3f(x1,y,z1);

		glEnd();
	}

	i = 0;
	x1 = draw_x[i];
	y = draw_y[i];
	z1 = 0;
	GLfloat x3 = rot120x[i];
	GLfloat z3 = rot120z[i];
	for(i = 1; i <= num_draw_pts; i++){
		glBegin(GL_POLYGON);
		
		glVertex3f(x1,y,z1);
		glVertex3f(x3,y,z3);
		x1 = draw_x[i];
		y = draw_y[i];
		z1 = 0;
		x2 = rot240x[i];
		z2 = rot240z[i];
		
		glVertex3f(x3,y,z3);
		glVertex3f(x1,y,z1);

		glEnd();
	}

	i = 0;
	x3 = rot240x[i];
	y = draw_y[i];
	z3 = rot240z[i];
	x2 = rot120x[i];
	z2 = rot120z[i];
	for(i = 1; i <= num_draw_pts; i++){
		glBegin(GL_POLYGON);
		
		glVertex3f(x3,y,z3);
		glVertex3f(x2,y,z2);
		x3 = rot240x[i];
		y = draw_y[i];
		z3 = rot240z[i];
		x2 = rot120x[i];
		z2 = rot120z[i];
		
		glVertex3f(x2,y,z2);
		glVertex3f(x3,y,z3);

		glEnd();
	}

	free(rot120x);
	free(rot120z);
	free(rot240x);
	free(rot240z);
}

void drawSurfaceWireframe(void){
	GLfloat* rot120x = (GLfloat *)malloc((num_draw_pts+1) * sizeof(GLfloat));
	GLfloat* rot120z = (GLfloat *)malloc((num_draw_pts+1) * sizeof(GLfloat));
	GLfloat* rot240x = (GLfloat *)malloc((num_draw_pts+1) * sizeof(GLfloat));
	GLfloat* rot240z = (GLfloat *)malloc((num_draw_pts+1) * sizeof(GLfloat));

	for(int i = 0; i <= num_draw_pts; i++) {
		rot120x[i] = -0.5f * draw_x[i];
		rot120z[i] = .86603 * draw_x[i];
		rot240x[i] = rot120x[i];
		rot240z[i] = rot120z[i] * -1;
	}

	glColor3f(0.0f,0.0f,1.0f); //blue color
	int i = 0;
	GLfloat x1 = draw_x[i];
	GLfloat y = draw_y[i];
	GLfloat z1 = 0;
	GLfloat x2 = rot120x[i];
	GLfloat z2 = rot120z[i];
	for(i = 1; i <= num_draw_pts; i++){
		glBegin(GL_LINE_STRIP);
		
		glVertex3f(x1,y,z1);
		glVertex3f(x2,y,z2);
		x1 = draw_x[i];
		y = draw_y[i];
		z1 = 0;
		x2 = rot120x[i];
		z2 = rot120z[i];
		
		glVertex3f(x2,y,z2);
		glVertex3f(x1,y,z1);
		glVertex3f(draw_x[i-1],draw_y[i-1],0);
		glEnd();
	}

	i = 0;
	x1 = draw_x[i];
	y = draw_y[i];
	z1 = 0;
	GLfloat x3 = rot120x[i];
	GLfloat z3 = rot120z[i];
	for(i = 1; i <= num_draw_pts; i++){
		glBegin(GL_LINE_STRIP);
		
		glVertex3f(x1,y,z1);
		glVertex3f(x3,y,z3);
		x1 = draw_x[i];
		y = draw_y[i];
		z1 = 0;
		x2 = rot240x[i];
		z2 = rot240z[i];
		
		glVertex3f(x3,y,z3);
		glVertex3f(x1,y,z1);
		glVertex3f(draw_x[i-1],draw_y[i-1],0);
		glEnd();
	}

	i = 0;
	x3 = rot240x[i];
	y = draw_y[i];
	z3 = rot240z[i];
	x2 = rot120x[i];
	z2 = rot120z[i];
	for(i = 1; i <= num_draw_pts; i++){
		glBegin(GL_LINE_STRIP);
		
		glVertex3f(x3,y,z3);
		glVertex3f(x2,y,z2);
		x3 = rot240x[i];
		y = draw_y[i];
		z3 = rot240z[i];
		x2 = rot120x[i];
		z2 = rot120z[i];
		
		glVertex3f(x2,y,z2);
		glVertex3f(x3,y,z3);
		glVertex3f(rot240x[i-1],draw_y[i-1],rot240z[i-1]);
		glEnd();
	}

	free(rot120x);
	free(rot120z);
	free(rot240x);
	free(rot240z);
}

void drawSurfacePoints(void) {
	

	GLfloat* rot120x = (GLfloat *)malloc((num_draw_pts+1) * sizeof(GLfloat));
	GLfloat* rot120z = (GLfloat *)malloc((num_draw_pts+1) * sizeof(GLfloat));
	GLfloat* rot240x = (GLfloat *)malloc((num_draw_pts+1) * sizeof(GLfloat));
	GLfloat* rot240z = (GLfloat *)malloc((num_draw_pts+1) * sizeof(GLfloat));

	for(int i = 0; i <= num_draw_pts; i++) {
		rot120x[i] = -0.5f * draw_x[i];
		rot120z[i] = .86603 * draw_x[i];
		rot240x[i] = rot120x[i];
		rot240z[i] = rot120z[i] * -1;
	}

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

	for(int i = 0; i <= num_draw_pts; i++) {
		//Conversion of ratios between input and output
		int x = rot120x[i];
		int y = draw_y[i];
		int z = rot120z[i];
		//printf("(%d, %d) should be drawn\n", x, y);
		glVertex3f(x, y, z);
	}

	for(int i = 0; i <= num_draw_pts; i++) {
		//Conversion of ratios between input and output
		int x = rot240x[i];
		int y = draw_y[i];
		int z = rot240z[i];
		//printf("(%d, %d) should be drawn\n", x, y);
		glVertex3f(x, y, z);
	}

	glEnd();
	free(rot120x);
	free(rot120z);
	free(rot240x);
	free(rot240z);
}
/* end of drawing.cpp */
