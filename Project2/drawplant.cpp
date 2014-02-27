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
#include <algorithm>
#include <vector>

#include "common.h"
#include "drawplant.h"
#include "readppm.h"

#define PI 3.14159

using namespace std;

extern GLfloat rotation=0;
int depth = 6;

vector<GLfloat> I(16);
vector<vector<GLfloat>> state(20,vector<GLfloat>(16));
int index=0;
vector<GLfloat> moved(16);



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

void load3DMatrix(vector<GLfloat>& m){
	load3DMatrix(m[0],m[1],m[2],m[3],
				m[4],m[5],m[6],m[7],
				m[8],m[9],m[10],m[11],
				m[12],m[13],m[14],m[15]);
}

//Passed in row-major order
vector<GLfloat> vecByMat(GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3,
		GLfloat m00, GLfloat m01, GLfloat m02, GLfloat m03,
		GLfloat m10, GLfloat m11, GLfloat m12, GLfloat m13,
		GLfloat m20, GLfloat m21, GLfloat m22, GLfloat m23,
		GLfloat m30, GLfloat m31, GLfloat m32, GLfloat m33){

	vector<GLfloat> result(4);
	result[0] = v0*m00+v1*m01+v2*m02+v3*m03;
	result[1] = v0*m10+v1*m11+v2*m12+v3*m13;
	result[2] = v0*m20+v1*m21+v2*m22+v3*m23;
	result[3] = v0*m30+v1*m31+v2*m32+v3*m33;

	return result;
}

//Matrix is passed in row-major order
void vecByMat(vector<GLfloat>& v, const vector<GLfloat>& m){
	v.swap(vecByMat(v[0],v[1],v[2],v[3],
			m[0],m[1],m[2],m[3],
			m[4],m[5],m[6],m[7],
			m[8],m[9],m[10],m[11],
			m[12],m[13],m[14],m[15]));
}


void matByMat(const vector<GLfloat>& A, vector<GLfloat>& B){
	vector<GLfloat> result(16);

	for(int r = 0; r < 4; r++){
		for(int c = 0; c < 4; c++){
			int ri = r*4;
			int ci = c;
			result[ri+ci]=A[ri]*B[ci]+A[ri+1]*B[ci+4]+A[ri+2]*B[ci+8]+A[ri+3]*B[ci+12];
		}
	}

	B.swap(result);
}

void rotateXY(GLfloat angle, vector<GLfloat>& out){ //takes degrees
	
	out[10]=1;out[15]=1;
	out[0]=cos(PI*angle/180);out[1]=-sin(PI*angle/180);
	out[4]=sin(PI*angle/180);out[5]=cos(PI*angle/180);
}

void rotateXZ(GLfloat angle, vector<GLfloat>& out){ //takes degrees
	
	out[5]=1;out[15]=1;
	out[0]=cos(PI*angle/180);out[2]=-sin(PI*angle/180);
	out[8]=sin(PI*angle/180);out[10]=cos(PI*angle/180);
}

void translate(GLfloat x, GLfloat y,GLfloat z, vector<GLfloat>& out){ 
	out[0]=1;out[5]=1;out[10]=1;out[15]=1;
	out[3]=x;out[7]=y;out[11]=z;
}


void printMatrix(vector<GLfloat>& m){
	for(int i = 0; i < 4; i++){
		int I = i*4;
		printf("%f %f %f %f\n",m[I],m[I+1],m[I+2],m[I+3]);
	}
}



void turn(GLfloat deg){
	vector<GLfloat> R(16);
	rotateXY(deg,R);
	
	vector<GLfloat> Tn(16);
	translate(moved[3],moved[7],moved[11],Tn);
	moved[3]=0;moved[7]=0;moved[11]=0;
	matByMat(R,moved);
	matByMat(Tn,moved);
	
}



void move(GLfloat num){
	moved[3]+= num*moved[1];
	moved[7]+= num*moved[0];
}

void pushState(){
	
	copy(&moved[0],&moved[0]+16,&(state[index][0]));
	
	index++;
}

void popState(){
	index--;
	moved.swap(state[index]);
	
}




//------------------------------------------------------------------

void drawLeaf(void) {
	/* ADD YOUR CODE to make the 2D leaf a 3D extrusion */
	pushState();
	vector<GLfloat> R(16);
	rotateXZ(rotation,R);
	matByMat(R,moved);
	
	load3DMatrix(moved);
	popState();
	
	glColor3f(0.1,0.9,0.1); 
	

	GLfloat t = 2.0;

	GLfloat verts[] = {
		0.0,0.0,0, //0
		t*1.0,t*0.7,0, //1
		t*1.3,t*1.8,0, //2
		t*1.0,t*2.8,0, //3
		0.0,t*4.0,0, //4
		-t*1.0,t*2.8,0, //5
		-t*1.3,t*1.8,0, //6
		-t*1.3,t*1.8,0, //7
		0.0,t*2.0,t*1, //8 Peak
		0.0,t*2.0,-t*1 //9 Valley
	};

	GLuint indices[] = {
		0,1,8,
		1,2,8,
		2,3,8,
		3,4,8,
		4,5,8,
		5,6,8,
		6,7,8,
		7,0,8,
		1,0,9,
		2,1,9,
		3,2,9,
		4,3,9,
		5,4,9,
		6,5,9,
		7,6,9,
		0,7,9
	};

	int num_indices;

	num_indices = sizeof(indices) / sizeof(GLuint);

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, verts);
	glDrawElements(GL_TRIANGLES, num_indices,
					GL_UNSIGNED_INT, indices);

	glDisableClientState(GL_VERTEX_ARRAY);

	
}

void drawBranch(void) {
	/* ADD YOUR CODE to make the 2D branch a 3D extrusion */
	

	pushState();
	vector<GLfloat> R(16);
	rotateXZ(rotation,R);
	matByMat(R,moved);
	load3DMatrix(moved);
	popState();


	GLfloat th = 2.0;

	GLfloat verts[24]={
		th/2,0.0,th/2, //0
		th/2,5.0,th/2, //1
		-th/2,5.0,th/2, //2
		-th/2,0.0,th/2, //3
		th/2,0.0,-th/2, //4
		th/2,5.0,-th/2, //5
		-th/2,5.0,-th/2, //6
		-th/2,0.0,-th/2}; //7

	GLuint indices[24] = {
		0,1,2,3,
		0,1,5,4,
		2,3,7,6,
		4,5,6,7,
		0,3,7,4,
		1,2,6,5};

	glColor3f(0.54,0.27,0.07); 
	int num_indices;

	num_indices = sizeof(indices) / sizeof(GLuint);

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, verts);
	glDrawElements(GL_QUADS, num_indices,
					GL_UNSIGNED_INT, indices);

	glDisableClientState(GL_VERTEX_ARRAY);

}

int branch_len = 1;

void grow(void){
	branch_len++;
}

void drawActualBranch(void){
	for(int i = 0; i < branch_len;i++){
		drawBranch();
		move(5.0f);
	}
}


void drawLeftLeaf(int i){
	if(i==0){
		drawLeaf();
	}else{
		drawBranch(i-1); 
		pushState(); 
			turn(30); 
			drawBranch(i-1); 
			drawLeftLeaf(i-1); 
		popState(); 
		drawBranch(i-1);
		pushState(); 
			turn(-30); 
			drawBranch(i-1); 
			drawLeaf(); 
		popState();
		drawBranch(i-1); 
		drawLeftLeaf(i-1);
	}
}

void drawRightLeaf(int i){
	if(i==0){
		drawLeaf();
	}else{
		drawBranch(i-1); 
		pushState(); 
			turn(30); 
			drawBranch(i-1); 
			drawLeaf(); 
		popState(); 
		drawBranch(i-1);
		pushState(); 
			turn(-30); 
			drawBranch(i-1); 
			drawRightLeaf(i-1); 
		popState();
		drawBranch(i-1); 
		drawRightLeaf(i-1);
	}
}

void drawLeaf(int i){
	if(i==0) {drawLeaf();}
	else{
		drawBranch(i-1); 
		pushState(); 
			turn(30); 
			drawBranch(i-1); 
			drawLeftLeaf(i-1); 
		popState(); 
		drawBranch(i-1);
		pushState(); 
			turn(-30); 
			drawBranch(i-1); 
			drawRightLeaf(i-1); 
		popState();
		drawBranch(i-1); 
		drawLeaf(i-1);
	}
}

void drawBranch(int i) {
	if(i==0){ drawActualBranch(); }
	else {
		 grow(); drawBranch(i-1); branch_len--;
	}
}

void initialize(){
	branch_len = 1;
	
	//state.clear();
	translate(0,0,0,I);
	copy(&I[0],&I[0]+16,&moved[0]);
	index=0;
}

void drawTree(int i) {
	//initialize?
	initialize();
	drawLeaf(i);
}

/*
 * Draws the plant.
 *
 * ADD YOUR CODE and modify the function to take an L-system depth and
 * any other necessary arguments.
 */
void drawPlant(void) {

	drawTree(depth);
	
	

}





/* end of drawplant.c */
