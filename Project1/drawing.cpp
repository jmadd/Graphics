/*
 * drawing.c
 * ---------
 * Drawing subroutines for each of the various display modes in the canvas.
 * Also contains the quadrilateral information for a cube and the
 * triangulation information for a cone.
 *
 * Starter code for Project 1.
 *
 * Group Members: <FILL IN>
 */




#include <stdio.h>


#include "common.h"
#include "drawing.h"
#include "vrml.h"
#include <math.h> //addition

#define PI 3.14159265 //addition

int disp_style;
int recursion_level;
GLuint* ico_indices;
GLuint* ico_indices_tmp;
GLfloat* ico_verts;
int curr_rec; //selected_recursion_level
int vr_object;
int disp_mode;

vector<Vector3f> icovertsq;


/***********************************************************
 * Begin Cube Data
 ***********************************************************/
/*
 * Vertices used in the manually rendered cube.  This is cube
 * that is 1 x 1 x 1 centered at the origin.  Each group of
 * 3 numbers is a vertex, specified in Cartesian coordinates.
 */
GLfloat cube_vertices[] = {
    -0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
};

/*
 * The colors of each vertex in the low level cube.  The index
 * into this array corresponds to the index into cube_vert.
 */
GLfloat cube_colors[] = {
    0.5f, 0.5f, 0.5f,
    0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f,
};

/*
 * Indices into cube_vert, groups of 4 can be used to draw quadrilaterals
 * which represents a face of the cube.  For instance, the first
 * quad means that vertices 0, 1, 3, 2 of the cube_vertices[]
 * are used, in that order, to form the first face of the cube.
 *
 * Note that all front facing quads are specified in a counterclockwise order
 * (that is, if you are looking at the front of a quad, the vertices will
 * appear in counterclockwise order).
 */
GLuint cube_indices[] = {
    0, 2, 3, 1,
    2, 6, 7, 3,
    7, 6, 4, 5,
    4, 0, 1, 5,
    1, 3, 7, 5,
    0, 4, 6, 2,
};
/***********************************************************
 * End Cube Data
 ***********************************************************/


/***********************************************************
 * Begin Cone Data
 ***********************************************************/

/*
 * Data for the triangulation of the surface of a cone that is one
 * unit tall has a unit circle for its base.  The base is composed
 * of 8 equally sized triangles, and the lateral surface of the cone
 * is composed of a different set of 8 equally sized triangles.
 *
 * See documentation in the Cube Data section for information on
 * the meaning of the data in each array.
 */

GLfloat cone_vertices[] = {
    1.0, -0.5, 0.0,         /* begin unit circle at y = -0.5 */
    0.707, -0.5, 0.707,
    0.0, -0.5, 1.0,
    -0.707, -0.5, 0.707,
    -1.0, -0.5, 0.0,
    -0.707, -0.5, -0.707,
    0.0, -0.5, -1.0,
    0.707, -0.5, -0.707,    /* end unit circle at y = -0.5 */
    0.0, -0.5, 0.0,         /* center of the base */
    0.0, 0.5, 0.0,          /* top of the cone */
};

GLfloat cone_colors[] = {
    0.0f, 0.0f, 0.5f,
    0.0f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f,
};

/*
 * Each set of 3 indices represent the vertices of a triangle.
 * Each triangle faces to the outside of the cone.  The vertices
 * of these forward facing triangles are specified in
 * counterclockwise order.
 */
GLuint cone_indices[] = {
    0, 1, 8,
    1, 2, 8,
    2, 3, 8,
    3, 4, 8,
    4, 5, 8,
    5, 6, 8,
    6, 7, 8,
    7, 0, 8,
    1, 0, 9,
    2, 1, 9,
    3, 2, 9,
    4, 3, 9,
    5, 4, 9,
    6, 5, 9,
    7, 6, 9,
    0, 7, 9,
};
/***********************************************************
 * End Cone Data
 ***********************************************************/


GLfloat icosphere_vertices[] = {
	-1,1.6180339f,0,
	1,1.6180339f,0,
	-1,-1.6180339f,0,
	1,-1.6180339f,0,

	0,-1,1.6180339f,
	0,1,1.6180339f,
	0,-1,-1.6180339f,
	0,1,-1.6180339f,

	1.6180339f,0,-1,
	1.6180339f,0,1,
	-1.6180339f,0,-1,
	-1.6180339f,0,1
};

GLuint icosphere_indices[] = {
	0,11,5,
	0,5,1,
	0,1,7,
	0,7,10,
	0,10,11,

	1,5,9,
	5,11,4,
	11,10,2,
	10,7,6,
	7,1,8,

	3,9,4,
	3,4,2,
	3,2,6,
	3,6,8,
	3,8,9,

	4,9,5,
	2,4,11,
	6,2,10,
	8,6,7,
	9,8,1
};

vector<Vector3f> ico_vertsq;
vector<Vector3ui> ico_indicesq;

void init_shapes(void){
	int i;
	
	int num_indices = sizeof(icosphere_indices)/sizeof(int);
	ico_vertsq = vector<Vector3f>();
	ico_indicesq= vector<Vector3ui>();

	for(i = 0; i < 36; i+=3){
		Vector3f v(icosphere_vertices[i],icosphere_vertices[i+1],icosphere_vertices[i+2]);
		v.normalize();
		ico_vertsq.push_back(v);
	}
	for(i = 0; i < num_indices; i+=3){
		Vector3ui v(icosphere_indices[i],icosphere_indices[i+1],icosphere_indices[i+2]);
		ico_indicesq.push_back(v);
	}
}

/* Uses glut to draw a cube */
void draw_cube_glut(void) {
	/* Draw the cube using glut */

	glColor3f(1.0f, 0.0f, 0.0f);
	if (disp_style == DS_SOLID) {
		glutSolidCube(1.0f);
	} else if (disp_style == DS_WIRE) {
		glutWireCube(1.0f);
	}
}

/*
 * Draws a cube using the data arrays at the top of this file.
 * Iteratively draws each quad in the cube.
 */
void draw_cube_quad(void) {
	int num_indices;
	int i;
	int index1, index2, index3, index4;

	num_indices = sizeof(cube_indices) / sizeof(GLuint);

	/*
	 * Loop over all quads that need to be drawn.
	 * Step i by 4 because there are 4 vertices per quad.
	 */
	for (i = 0; i < num_indices; i += 4) {
		/*
		 * Find the index into the vertex array.  The value
		 * in the cube_indices array refers to the index
		 * of the ordered triples, not the index for the
		 * actual GLfloats that comprise the cube_vertices array.
		 * Thus, we need to multiple by 3 to get the real index.
		 */
		index1 = cube_indices[i] * 3;
		index2 = cube_indices[i+1] * 3;
		index3 = cube_indices[i+2] * 3;
		index4 = cube_indices[i+3] * 3;

		glBegin(GL_QUADS);

		/* All arguments here are pointers */
		glColor3fv(  &(cube_colors[index1]) );
		glVertex3fv( &(cube_vertices[index1]) );
		glColor3fv(  &(cube_colors[index2]) );
		glVertex3fv( &(cube_vertices[index2]) );
		glColor3fv(  &(cube_colors[index3]) );
		glVertex3fv( &(cube_vertices[index3]) );
		glColor3fv(  &(cube_colors[index4]) );
		glVertex3fv( &(cube_vertices[index4]) );

		glEnd();

	}
}

/*
 * Draws a cube using the data arrays at the top of this file.
 * Uses GL's vertex arrays, index arrays, color arrays, etc.
 */
void draw_cube_quad_arrays(void) {
	int num_indices;

	num_indices = sizeof(cube_indices) / sizeof(GLuint);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, cube_vertices);
	glColorPointer(3, GL_FLOAT, 0, cube_colors);
	glDrawElements(GL_QUADS, num_indices,
					GL_UNSIGNED_INT, cube_indices);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

/*
 * Uses glut to draw a cone.  Must render in either solid and wire
 * frame modes, based on the value of the variable disp_style.
 */
void draw_cone_glut(void) {
	/* ADD YOUR CODE HERE */
	glColor3f(0.0f, 0.0f, 1.0f);
	if (disp_style == DS_SOLID) {
		glutSolidCone(1,1,8,8);
	} else if (disp_style == DS_WIRE) {
		glutWireCone(1,1,8,8);
	}
}

/*
 * Draws a cone using the data arrays at the top of this file.
 * Iteratively draws each triangle in the cone.
 */
void draw_cone_tri(void) {
	/* ADD YOUR CODE HERE */
	int num_indices;
	int i;
	int index1, index2, index3;

	num_indices = sizeof(cone_indices) / sizeof(GLuint);

	/*
	 * Loop over all triangles that need to be draen.
	 * Step i by 3 because there are 3 vertices per triangles.
	 */
	for (i = 0; i < num_indices; i += 3) {
		/*
		 * Find the index into the vertex array.  The value
		 * in the cone_indices array refers to the index
		 * of the ordered triples, not the index for the
		 * actual GLfloats that comprise the cone_vertices array.
		 * Thus, we need to multiple by 3 to get the real index.
		 */
		index1 = cone_indices[i] * 3;
		index2 = cone_indices[i+1] * 3;
		index3 = cone_indices[i+2] * 3;

		glBegin(GL_TRIANGLES);

		/* All arguments here are pointers */
		glColor3fv(  &(cone_colors[index1]) );
		glVertex3fv( &(cone_vertices[index1]) );
		glColor3fv(  &(cone_colors[index2]) );
		glVertex3fv( &(cone_vertices[index2]) );
		glColor3fv(  &(cone_colors[index3]) );
		glVertex3fv( &(cone_vertices[index3]) );

		glEnd();

	}
}

/*
 * Draws a cone using the data arrays at the top of this file.
 * Uses GL's vertex arrays, index arrays, color arrays, etc.
 */
void draw_cone_tri_arrays(void) {
	/* ADD YOUR CODE HERE */
	int num_indices;

	num_indices = sizeof(cone_indices) / sizeof(GLuint);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, cone_vertices);
	glColorPointer(3, GL_FLOAT, 0, cone_colors);
	glDrawElements(GL_TRIANGLES, num_indices,
					GL_UNSIGNED_INT, cone_indices);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

/*
 * Draws a cone using a calculated triangulation of the cone surface.
 *
 * Args
 * ----
 * The HEIGHT and RADIUS of the cone.
 *
 * BASE_TRI refers to the number of triangles used to represent
 * the base of the cone.  Each of these triangles should share a
 * common vertex, namely, the center of the base.
 *
 * The final triangulation of the cone surface should include
 * exactly 2 * BASE_TRI.
 */
void draw_cone_tri_calc(double height, double radius, int base_tri) {
	/* ADD YOUR CODE HERE */
	int numVerts=(base_tri+2);
	
	int indexBase=3*(numVerts-2);
	int indexTop=3*(numVerts-1);
	GLfloat half = (0.5f)*height;
	GLfloat rad = radius;
	GLfloat *verts = (GLfloat*)malloc(sizeof(GLfloat)*3*numVerts);
	int *indices = (int*)malloc(sizeof(int)*6*base_tri);
	int num_indices = 6*base_tri;

	int index1;
	int index2;
	int index3;

	GLfloat inc = (2.0f*PI)/base_tri;
	GLfloat theta = 0;

	int i = 0;

	verts[indexBase]=0.0f;
	verts[indexBase+1]=-half;
	verts[indexBase+2]=0.0f;
	verts[indexTop]=0.0f;
	verts[indexTop+1]=half;
	verts[indexTop+2]=0.0f;
	for(i=0; i < base_tri; i++, theta+=inc){
		GLfloat sine = sin(theta);
		GLfloat cosine = cos(theta);
		GLfloat x = cosine*rad;
		GLfloat y = sine*rad;
		verts[i*3]=x;
		verts[i*3+1]=-half;
		verts[i*3+2]=y;
		

		indices[i*3]=i; //vert1
		indices[i*3+1]= (i+1 >= base_tri) ? 0 : i+1; //vert2
		indices[i*3+2]= base_tri; //base
		indices[3*(base_tri+i)]= indices[3*i+1]; //vert2
		indices[3*(base_tri+i)+1]= indices[3*i]; //vert1
		indices[3*(base_tri+i)+2]= base_tri+1; //top
	}
	
	
	glColor3f( 0.0f,0.0f,1.0f );
	for (i = 0; i < num_indices; i += 3) {
		
		index1 = indices[i] * 3;
		index2 = indices[i+1] * 3;
		index3 = indices[i+2] * 3;

		glBegin(GL_TRIANGLES);
		
		 //All arguments here are pointers
		glVertex3fv( &(verts[index1]) );
		glVertex3fv( &(verts[index2]) );
		glVertex3fv( &(verts[index3]) );

		glEnd();

	}
}


/* Draw the various vrml scenes */
void draw_vrml(void) {
	/* ADD YOUR CODE HERE */
	/* NOTE: you should be calling a function or functions in vrml.c */
	glColor3f(0,0,1.0f);
	switch(vr_object){
	case VR_CUBE:
		draw_vrml_cube();
		break;
	case VR_DODECAHEDRON:
		draw_vrml_dodecahedron();
		break;
	case VR_ICOSAHEDRON:
		draw_vrml_icosahedron();
		break;
	case VR_PYRAMID:
		draw_vrml_pyramid();
		break;
	case VR_MAX:
		vr_object = VR_CUBE;
		draw_vrml();
		break;
	default:
		break;
	}
}


GLfloat th=0,th2=0,th3=0;
GLfloat pulse=1,mod=0.1f;
/* Draws a freeform scene */
void draw_free_scene(void) {
	/* ADD YOUR CODE HERE */
	/* NOTE: Modify or remove the existing code in this func, as necessary */
	
	


	glPushMatrix();
	glColor3f((1-pulse),0,pulse);
	glRotatef(th+=0.2f,0,1,1);
	glScalef(3*pulse,3*pulse,3*pulse);
	draw_sphere(curr_rec);
	glPopMatrix();
	
	glPushMatrix();
	glColor3f((1-pulse),pulse,pulse);
	glRotatef(th2-=0.4f,-1,-.5,0);
	glScalef(1.5f*pulse,1.5f*pulse,1.5f*pulse);
	draw_sphere(curr_rec);
	glPopMatrix();

	glPushMatrix();
	glColor3f(pulse,(1-pulse),(1-pulse));
	glRotatef(th3+=1.0f,.25f,-.5,1.5f);
	glScalef(pulse,pulse,pulse);
	draw_sphere(curr_rec);
	glPopMatrix();

	pulse+=mod;
	if(pulse > 1) mod = -0.005f;
	if(pulse < 0.05f) mod = 0.005f;
	
}



void draw_sphere(int rec){
	int num_indices;
	
	int j;
	int t;

	num_indices = sizeof(icosphere_indices) / sizeof(GLuint);
	rec = (rec > MAX_RECURSION) ? MAX_RECURSION : rec;
	rec = (rec < 0) ? 0 : rec;
	
	t = 12;
	if(recursion_level != rec){
		
		init_shapes();
		
		
		for(j = 0; j < rec; j++){
			vector<Vector3ui> ico_indicesq_tmp;
			for (Vector3ui indexv: ico_indicesq) {
				Vector3f a;
				Vector3f b;
				Vector3f c;

				/*
				ico_indicesq = {
				Vector3ui (i1,i2,i3) Triangle 1,
				Vector3ui (i4,i5,i6) Triangle 2,
				Vector3ui (i7,i8,i9) Triangle 3 }
				*/
			

				a = getMidPoint((ico_vertsq[indexv._x]),(ico_vertsq[indexv._y]));
				b = getMidPoint((ico_vertsq[indexv._y]),(ico_vertsq[indexv._z]));
				c = getMidPoint((ico_vertsq[indexv._z]),(ico_vertsq[indexv._x]));
			
				ico_vertsq.push_back(a);
				ico_vertsq.push_back(b);
				ico_vertsq.push_back(c);


				ico_indicesq_tmp.push_back(Vector3ui(indexv._x,t,t+2));

				ico_indicesq_tmp.push_back(Vector3ui(indexv._y,t+1,t));

				ico_indicesq_tmp.push_back(Vector3ui(indexv._z,t+2,t+1));

				ico_indicesq_tmp.push_back(Vector3ui(t,t+1,t+2));

				t+=3;
			
			}
			ico_indicesq.clear();
			for(Vector3ui v: ico_indicesq_tmp){
				ico_indicesq.push_back(v);
			}
		}
	}
	
	for (Vector3ui v: ico_indicesq) {
		
		
		glBegin(GL_LINE_LOOP);

		
		glVertex3fv( &(ico_vertsq[v._x]._x) );
		glVertex3fv( &(ico_vertsq[v._y]._x) );
		glVertex3fv( &(ico_vertsq[v._z]._x) );

		glEnd();

	}

	recursion_level = rec;

}

Vector3f getMidPoint(Vector3f v1, Vector3f v2){
	Vector3f v((v1._x+v2._x)/2.0f,
			   (v1._y+v2._y)/2.0f,
			   (v1._z+v2._z)/2.0f);
	v.normalize();
	return v;
}

/* Prints to stdout the current display mode */
void print_disp_mode( void ) {
    switch (disp_mode) {
        case DM_CUBE_GLUT:
            printf("Display Mode: Cube using glut\n");
            break;
        case DM_CUBE_QUAD:
            printf("Display Mode: Cube using quadrilaterals\n");
            break;
        case DM_CUBE_QUAD_ARRAYS:
            printf("Display Mode: Cube using quadrilateral arrays\n");
            break;
        case DM_CONE_GLUT:
            printf("Display Mode: Cone using glut\n");
            break;
        case DM_CONE_TRI:
            printf("Display Mode: Cone using triangles\n");
            break;
        case DM_CONE_TRI_ARRAYS:
            printf("Display Mode: Cone using triangle arrays\n");
            break;
        case DM_CONE_TRI_CALC:
            printf("Display Mode: Cone using calculated triangles\n");
			break;
        case DM_VRML:
            printf("Display Mode: VRML objects\n");
            break;
        case DM_FREE_SCENE:
            printf("Display Mode: Freeform scene\n");
            break;
        default:
			printf("Warning: unknown display mode\n");
            break;
    }
}


/* Prints to stdout the current display style */
void print_disp_style( void ) {
	if (disp_style == DS_SOLID) {
		printf("Display Style: solid (for glut modes only)\n");
	} else if (disp_style == DS_WIRE) {  
		printf("Display Style: wire (for glut modes only)\n");
	} else {
		printf("Warning: unknown display style\n");
	}
}

/* end of drawing.c */

