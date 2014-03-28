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
	draw_x = (GLfloat *)malloc((num_draw_pts+1) * sizeof(GLfloat));
	draw_y = (GLfloat *)malloc((num_draw_pts+1) * sizeof(GLfloat));
	for(int i = 0; i <= num_draw_pts; i++){
		draw_x[i]=new_draw_x[i];
		draw_y[i]=new_draw_y[i];
	}

	
	return;
}

void subdividePointsArrayH(int subdiv_level){
	assert(subdiv_level >= 0);
	subdividePointsArray(0);
	/* ADD YOUR CODE HERE */
	if (subdiv_level == 0) {
		int n = num_draw_pts+1;
		for(int i = 0; i <= num_draw_pts; i++) {
			draw_x[i+n] = -0.5f * draw_x[i];
			draw_z[i+n] = .86603 * draw_x[i];
			draw_x[i+n*2] = draw_x[i+n];
			draw_z[i+n*2] = draw_z[i+n] * -1;
		}
		
		return;
	}
	subdividePointsArray(subdiv_level-1);

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


}


/**********************************************
 * End Routines for Midpoint Subdivision 
 **********************************************/

/* End of data.cpp */
