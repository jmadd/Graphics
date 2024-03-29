/*
 * drawing.cpp
 * -----------
 * Contains the drawing routines and related helper functions for the
 * subdivision surface
 *
 * Group Members: Nick Crum (nrc523) Jared Maddox (jam7732)
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
		GLfloat x = draw_x[i];
		GLfloat y = draw_y[i];
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
		GLfloat x = draw_x[i];
		GLfloat y = draw_y[i];
		//printf("(%d, %d) should be drawn\n", x, y);
		glVertex3f(x, y, 0.0f);
	}
	glEnd();

}

void drawSurfaceSolid(void){

	//NEWLY ADDED
/*   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 50.0 };
   GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);

   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);

 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
 	glEnable(GL_DEPTH_TEST);
	// glLightModeli(GL_LIGHT_MODEL_TWO_SIDED, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);*/

	GLfloat ambient[] = {1.0, 0.0, 0.0, 1.0};

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	//END NEWLY ADDED


	int n = num_draw_pts+1;

	//glColor3f(0.0f,0.0f,1.0f); //blue color
	int k = ((int)(3*pow(2.0f,subdiv_h)) - 1);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(gouraud) {
		for(int j = 0; j < (int)(3*pow(2.0f,subdiv_h)); j++){
			for(int i = 1; i < n; i++){
				glBegin(GL_POLYGON);

				GLfloat* v1 = (GLfloat*)malloc(sizeof(GLfloat) * 3);
				v1[0] = draw_x[i+k*n-1];
				v1[1] = draw_y[i+k*n-1];
				v1[2] = draw_z[i+k*n-1];
				
				GLfloat* v2 = (GLfloat*)malloc(sizeof(GLfloat) * 3);
				v2[0] = draw_x[i+j*n-1];
				v2[1] = draw_y[i+j*n-1];
				v2[2] = draw_z[i+j*n-1];

				GLfloat* v3 = (GLfloat*)malloc(sizeof(GLfloat) * 3);
				v3[0] = draw_x[i+j*n];
				v3[1] = draw_y[i+j*n];
				v3[2] = draw_z[i+j*n];

				GLfloat* v4 = (GLfloat*)malloc(sizeof(GLfloat) * 3);
				v4[0] = draw_x[i+k*n];
				v4[1] = draw_y[i+k*n];
				v4[2] = draw_z[i+k*n];

				GLfloat* n1 = (GLfloat*)malloc(sizeof(GLfloat) * 3);
				n1[0] = vertices[i+k*n-1].normal[0];
				n1[1] = vertices[i+k*n-1].normal[1];
				n1[2] = vertices[i+k*n-1].normal[2];

				GLfloat* n2 = (GLfloat*)malloc(sizeof(GLfloat) * 3);
				n2[0] = vertices[i+j*n-1].normal[0];
				n2[1] = vertices[i+j*n-1].normal[1];
				n2[2] = vertices[i+j*n-1].normal[2];

				GLfloat* n3 = (GLfloat*)malloc(sizeof(GLfloat) * 3);
				n3[0] = vertices[i+j*n].normal[0];
				n3[1] = vertices[i+j*n].normal[1];
				n3[2] = vertices[i+j*n].normal[2];

				GLfloat* n4 = (GLfloat*)malloc(sizeof(GLfloat) * 3);
				n4[0] = vertices[i+k*n].normal[0];
				n4[1] = vertices[i+k*n].normal[1];
				n4[2] = vertices[i+k*n].normal[2];
				
				glNormal3fv(n1);
				glVertex3fv(v1);
				glNormal3fv(n2);
				glVertex3fv(v2);
				glNormal3fv(n3);
				glVertex3fv(v3);
				glNormal3fv(n4);
				glVertex3fv(v4);

				glEnd();
				glFlush();
			}
			k=j;
		}
	}

	else {
			for(int j = 0; j < (int)(3*pow(2.0f,subdiv_h)); j++){
				for(int i = 1; i < n; i++){
					glBegin(GL_POLYGON);

					GLfloat* v1 = (GLfloat*)malloc(sizeof(GLfloat) * 3);
					v1[0] = draw_x[i+k*n-1];
					v1[1] = draw_y[i+k*n-1];
					v1[2] = draw_z[i+k*n-1];

					GLfloat* v2 = (GLfloat*)malloc(sizeof(GLfloat) * 3);
					v2[0] = draw_x[i+j*n-1];
					v2[1] = draw_y[i+j*n-1];
					v2[2] = draw_z[i+j*n-1];

					GLfloat* v3 = (GLfloat*)malloc(sizeof(GLfloat) * 3);
					v3[0] = draw_x[i+j*n];
					v3[1] = draw_y[i+j*n];
					v3[2] = draw_z[i+j*n];

					GLfloat* v4 = (GLfloat*)malloc(sizeof(GLfloat) * 3);
					v4[0] = draw_x[i+k*n];
					v4[1] = draw_y[i+k*n];
					v4[2] = draw_z[i+k*n];

					GLfloat* n1 = crossProduct(subtractPoints(v1, v2), subtractPoints(v1, v3));

					glNormal3fv(n1);
					glVertex3fv(v1);
					glNormal3fv(n1);
					glVertex3fv(v2);
					glNormal3fv(n1);
					glVertex3fv(v3);
					glNormal3fv(n1);
					glVertex3fv(v4);

					glEnd();
					glFlush();
				}
				k=j;
			}
	}
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
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

GLfloat* crossProduct(GLfloat* v1, GLfloat* v2) {
	GLfloat* result = (GLfloat*)malloc(sizeof(GLfloat) * 3);
    result[0] = (v1[1]*v2[2])-(v2[1]*v1[2]);
    result[1] = -(v1[0]*v2[2])+(v2[0]*v1[2]);
    result[2] = (v1[0]*v2[1])-(v1[1]*v2[0]);
    GLfloat r = result[0]*result[0]+result[1]*result[1]+result[2]*result[2];
    r = sqrt(r);
    result[0]/=r;
    result[1]/=r;
    result[2]/=r;
    return result;
}

GLfloat* subtractPoints(GLfloat* v1, GLfloat* v2) {
	GLfloat* result = (GLfloat*)malloc(sizeof(GLfloat) * 3);
    result[0] = v1[0]-v2[0];
    result[1] = v1[1]-v2[1];
    result[2] = v1[2]-v2[2];
    return result;
}
/* end of drawing.cpp */
