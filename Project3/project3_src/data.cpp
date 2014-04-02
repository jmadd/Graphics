/*
 * data.cpp
 * --------
 * Contains routines to help manage the data for the Midpoint Subdivision.
 * Starter code for Project 3.
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
#include "data.h"
#include "drawing.h"

GLfloat i0_x[MAX_POINT];    // Iteration 0 control points, x
GLfloat i0_y[MAX_POINT];    // Iteration 0 control points, y
int num_i0_pts;                 // The number of iteration 0 control points

GLfloat *draw_x;     // Control points to be drawn, x
GLfloat *draw_y;     // Control points to be drawn, y
GLfloat *draw_z;
int num_draw_pts;    // The number of control points to draw



/**********************************************
 * Begin Routines for Midpoint Subdivsion
 **********************************************/

/*
 * Copies a static array into dynamically allocated space and
 * returns the copy.  Useful for making a copy of the static
 * iteration 0 control points, since the iteration 0 control
 * points should be kept in their static array for user-interface
 * purposes.
 *
 * Note: this function needs to be called twice, once for x and
 * once for y.
 */
GLfloat *copyPointsArray(GLfloat *orig, int size) {
	assert(size >= 0);
	if (size > 0) {
		assert(orig != NULL);
	}

	GLfloat *copy;
	copy = (GLfloat *)malloc(size * sizeof(GLfloat));
	for (int i = 0; i < size; i++) {
		copy[i] = orig[i];
	}

	return copy;
}

/*
 * Allocates an 1-D GLfloat array for the given number of elements.
 */
GLfloat *makePointsArray(int size) {
	assert(size >= 0);

	return (GLfloat *)malloc(size * sizeof(GLfloat));
}

/*
 * Given 2 equally sized points arrays, arr1 and arr2 in this form:
 *
 * arr1:    a0, a1, a2, a3, ...
 * arr2:    b0, b1, b2, b3, ... 
 *
 * ...returns a newly allocated array arr3 which contains:
 * arr3:    a0, b0, a1, b1, a2, b2, a3, b3, ...
 *
 * Note:
 * This function frees up the storage for arr1 and arr2, so the caller
 * of this function should not reference these arrays again.
 *
 * The two arrays should be the same size, and the argument "size"
 * is the size of one of these two arrays.
 */

GLfloat *mergePointsArrays(GLfloat *arr1, GLfloat *arr2, int size) {
	assert(size >= 0);
	if (size > 0) {
		assert(arr1 != NULL);
		assert(arr2 != NULL);
	}

	GLfloat *arr3;

	arr3 = (GLfloat *)malloc(2 * size * sizeof(GLfloat));
	
	for (int i = 0; i < size; i++) {
		arr3[2 * i] = arr1[i];
		arr3[(2 * i) + 1] = arr2[i];
	}

	free(arr1);
	free(arr2);

	return arr3;
}

void printArray(GLfloat* arr, int size){
	printf("[ ");
	for(int i = 0; i < size-1; i++){
		printf("%f, ",arr[i]);
	}
	printf("%f]\n",arr[size-1]);
}

/*
 * Using the iteration 0 control points (which are stored in the global
 * arrays "i0_x" and "i0_y") and the argument subdivision level, sets
 * the globals "GLfloat *draw_x" and "GLfloat *draw_y" to contain the x and
 * y coordinates for all points that need to be drawn at the given
 * subdivision level.  Also sets the number of value of global
 * "num_draw_pts" to be the size of one of these two drawing arrays.
 *
 * Note: The subdivision level should be greater than or equal to 0.  This
 * routine also frees the space used by the old "draw_x" and "draw_y" arrays,
 * if they exist.
 */
void subdividePointsArray(int subdiv_level) {
	assert(subdiv_level >= 0);

	/* ADD YOUR CODE HERE */
	if (subdiv_level == 0) {
		int n = num_i0_pts+1;
		for(int i = 0; i <= num_i0_pts; i++) {
			//0
			draw_x[i] = i0_x[i];
			draw_y[i] = i0_y[i];
			draw_z[i] = 0;
		}
		num_draw_pts = num_i0_pts;
		return;
	}
	subdividePointsArray(subdiv_level-1);
	
	GLfloat* tmp_draw_x = makePointsArray(num_draw_pts+1);
	GLfloat* new_draw_x = makePointsArray(num_draw_pts+1);
	GLfloat* tmp_draw_y = makePointsArray(num_draw_pts+1);
	GLfloat* new_draw_y = makePointsArray(num_draw_pts+1);
	
	for(int i = 0; i <= num_draw_pts; i++){
		tmp_draw_x[i] = draw_x[i];
		tmp_draw_y[i] = draw_y[i];
		
	}
	
	for(int i = 1; i < num_draw_pts; i++){
		tmp_draw_x[i] = .125f * (draw_x[i-1] + 6 * draw_x[i] + draw_x[i+1]);
		tmp_draw_y[i] = .125f * (draw_y[i-1] + 6 * draw_y[i] + draw_y[i+1]);
	}
	for(int i = 0; i < num_draw_pts; i++) {
		new_draw_x[i] = .125f * (4 * draw_x[i] + 4 * draw_x[i+1]);
		new_draw_y[i] = .125f * (4 * draw_y[i] + 4 * draw_y[i+1]);
	}
	new_draw_x = mergePointsArrays(tmp_draw_x, new_draw_x, num_draw_pts);
	new_draw_y = mergePointsArrays(tmp_draw_y, new_draw_y, num_draw_pts);
	new_draw_x[num_draw_pts*2] = draw_x[num_draw_pts];
	new_draw_y[num_draw_pts*2] = draw_y[num_draw_pts];
	
	num_draw_pts = num_draw_pts * 2;
	free(draw_x);
	free(draw_y);
	free(draw_z);
	draw_x = (GLfloat *)malloc((num_draw_pts+1)* 3 * (int) pow(2.0f,subdiv_h) * sizeof(GLfloat));
	draw_y = (GLfloat *)malloc((num_draw_pts+1)* 3 * (int) pow(2.0f,subdiv_h) * sizeof(GLfloat));
	draw_z = (GLfloat *)malloc((num_draw_pts+1)* 3 * (int) pow(2.0f,subdiv_h) * sizeof(GLfloat));
	for(int i = 0; i <= num_draw_pts; i++){
		draw_x[i]=new_draw_x[i];
		draw_y[i]=new_draw_y[i];
		draw_z[i]=0;
	}

	
	return;
}

void subdivideStack(GLfloat* stack_x, GLfloat* stack_y, GLfloat* stack_z, int stack_size){
	GLfloat* old_x = makePointsArray(stack_size);
	GLfloat* old_y = makePointsArray(stack_size);
	GLfloat* old_z = makePointsArray(stack_size);
	GLfloat* new_x = makePointsArray(stack_size);
	GLfloat* new_y = makePointsArray(stack_size);
	GLfloat* new_z = makePointsArray(stack_size);

	for(int i = 1; i < stack_size-1; i++){
		old_x[i] = .125f * (stack_x[i-1] + 6 * stack_x[i] + stack_x[i+1]);
		old_y[i] = stack_y[i];
		old_z[i] = .125f * (stack_z[i-1] + 6 * stack_z[i] + stack_z[i+1]);
	}
	old_x[0] = .125f * (stack_x[stack_size-1] + 6 * stack_x[0] + stack_x[1]);
	old_y[0] = stack_y[0];
	old_z[0] = .125f * (stack_z[stack_size-1] + 6 * stack_z[0] + stack_z[1]);

	old_x[stack_size-1] = .125f * (stack_x[stack_size-2] + 6 * stack_x[stack_size-1] + stack_x[0]);
	old_y[stack_size-1] = stack_y[stack_size-1];
	old_z[stack_size-1] = .125f * (stack_z[stack_size-2] + 6 * stack_z[stack_size-1] + stack_z[0]);


	for(int i = 0; i < stack_size-1; i++) {
		new_x[i] = .125f * (4 * stack_x[i] + 4 * stack_x[i+1]);
		new_y[i] = stack_y[i];
		new_z[i] = .125f * (4 * stack_z[i] + 4 * stack_z[i+1]);
	}
	new_x[stack_size-1]= .125f * (4 * stack_x[0] + 4* stack_x[stack_size-1]);
	new_y[stack_size-1] = stack_y[stack_size-1];
	new_z[stack_size-1]= .125f * (4 * stack_z[0] + 4* stack_z[stack_size-1]);


	new_x=mergePointsArrays(old_x,new_x,stack_size);
	new_y=mergePointsArrays(old_y,new_y,stack_size);
	new_z=mergePointsArrays(old_z,new_z,stack_size);

	for(int i = 0; i < stack_size*2; i++){
		stack_x[i]=new_x[i];
		stack_y[i]=new_y[i];
		stack_z[i]=new_z[i];
	}

}

void subdividePointsArrayH(int subdiv_level){
	assert(subdiv_level >= 0);
	
	subdividePointsArray(subdiv_v);
	int n = num_draw_pts+1;

	if (subdiv_level == 0) {
		
		for(int i = 0; i <= num_draw_pts; i++) {
			
			draw_x[i+n] = -0.5f * draw_x[i];
			draw_y[i+n] = draw_y[i];
			draw_z[i+n] = .86603 * draw_x[i];
			draw_x[i+n*2] = draw_x[i+n];
			draw_y[i+n*2] = draw_y[i];
			draw_z[i+n*2] = draw_z[i+n] * -1;
		}
		
		return;
	}
	subdividePointsArrayH(subdiv_level-1);

	/*
	IMPORTANT: ALWAYS APPLY VERTICAL SUBDIVISION FIRST OR THIS WONT WORK
	So we are subdividing by stacks,
	so stack 1 is indices: 0, n, n*2
	where n is num_draw_pts+1
	stack 2 is indices: 1, n+1, n*2+1
	After the first subdivision, each stack should contain 3 more indices, and we should have 3 more slices
	So stack 1 will be indices: 0, n, n*2, n*3, n*4, n*5
	So draw_x,draw_y and draw_z in effect hold (num_draw_pts+1)*3*(2^subdiv_h) points
	To index only one stack to make the first subdivision, you take i, i+n, i+2*n
	But what about for future subdivisions?
	Index i+j*n for all j < 3*(2^(subdiv_h-1))
	Add those points into a new array representing the stack.
	Apply subdivision to that array, and arrive with an old and new array
	Change old values for the past index i+j*n, and add new ones in for i+j*n where 3*(2^(subdiv_h-1) < j < 3*(2^subdiv_h)
	*/
	GLfloat* new_x = (GLfloat *)malloc((num_draw_pts+1)* 3 * (int) pow(2.0f,subdiv_h) * sizeof(GLfloat));
	GLfloat* new_y = (GLfloat *)malloc((num_draw_pts+1)* 3 * (int) pow(2.0f,subdiv_h) * sizeof(GLfloat));
	GLfloat* new_z = (GLfloat *)malloc((num_draw_pts+1)* 3 * (int) pow(2.0f,subdiv_h) * sizeof(GLfloat));

	for(int i = 0; i < n; i++){ //i indicates the stack index


		int stack_size = (int)(3*pow(2.0f,subdiv_level-1));
		/*
			This loop creates the stack array
		*/
		GLfloat* stack_x = makePointsArray(stack_size*2);
		GLfloat* stack_y = makePointsArray(stack_size*2);
		GLfloat* stack_z = makePointsArray(stack_size*2);
		for(int s = 0; s < stack_size; s++){ //s indicates the slice index
			int index = i+s*n; //index for the vertex
			stack_x[s]=draw_x[index];
			stack_y[s]=draw_y[index];
			stack_z[s]=draw_z[index];
		}
		//At this points, stack array should be created for x,y,z
		/*
			Next we need to perform subdivision on this array
			Since it is a loop, we do not have to exclude the first and last vertices like in the vertical subdivision
		*/
		subdivideStack(stack_x,stack_y,stack_z,stack_size);

		//Now stack_x,stack_y,stack_z contain the new stacks


		for(int s = 0; s < stack_size*2; s++){ //s indicates the slice index
			int index = i+s*n; //index for the vertex
			new_x[index]=stack_x[s];
			new_y[index]=stack_y[s];
			new_z[index]=stack_z[s];
		}


	}

	free(draw_x);
	free(draw_y);
	free(draw_z);

	draw_x = new_x;
	draw_y = new_y;
	draw_z = new_z;
}


void createEnvironment(void){
	int n = num_draw_pts+1;
	int m = (int)(3*pow(2.0f,subdiv_h));
	struct vertex* verts = new vertex[n];
	for(int i = 0; i < n*m; i++){
		verts[i].location = new GLfloat[3];
		verts[i].normal = new GLfloat[3];
		verts[i].color = new GLfloat[3];
		verts[i].location[0] = draw_x[i];
		verts[i].location[1] = draw_y[i];
		verts[i].location[2] = draw_z[i];
	}

	//now we have a single array of vertices
	
	//next we need to take this array, and create an array of poly's made of four edges
	

	//glColor3f(0.0f,0.0f,1.0f); //blue color
	int k = m - 1;
	struct poly* polys = new poly[m*(n-1)];
	for(int j = 0; j < m; j++){
		for(int i = 1; i < n; i++){
			polys[(i-1)+j*n].verts = new vertex[4];
			polys[(i-1)+j*n].verts[0] = verts[i+k*n-1];
			polys[(i-1)+j*n].verts[1] = verts[i+j*n-1];
			polys[(i-1)+j*n].verts[2] = verts[i+j*n];
			polys[(i-1)+j*n].verts[3] = verts[i+k*n];
			polys[(i-1)+j*n].normal = crossProduct(subtractPoints(verts[i+k*n-1].location, verts[i+j*n-1].location), subtractPoints(verts[i+k*n-1].location, verts[i+j*n].location));
			
			
			
		}
		k=j;
	}

	//at this point we have all the polygons and their normals in the polys array


}


/**********************************************
 * End Routines for Midpoint Subdivision 
 **********************************************/

/* End of data.cpp */
