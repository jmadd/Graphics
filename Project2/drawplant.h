#ifndef _DRAWPLANT_H_
#define _DRAWPLANT_H_

/* Functions implemented in drawplant.cpp */
#include <vector>

using namespace std;

void initialize(void);
void drawPlant(void);
void drawLeaf(void);
void load3DMatrix(vector<GLfloat>& m);
vector<GLfloat> vecByMat(GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3,
		GLfloat m00, GLfloat m01, GLfloat m02, GLfloat m03,
		GLfloat m10, GLfloat m11, GLfloat m12, GLfloat m13,
		GLfloat m20, GLfloat m21, GLfloat m22, GLfloat m23,
		GLfloat m30, GLfloat m31, GLfloat m32, GLfloat m33);
void vecByMat(vector<GLfloat>* v,const  vector<GLfloat>* m);

void pushState(void);
void popState(void);
void translate(GLfloat x, GLfloat y,GLfloat z, vector<GLfloat>* out);
void drawLeaf(int);
void drawBranch(int);
void drawTree(int);


void matByMat(const vector<GLfloat>* m,  vector<GLfloat>* n);

void printMatrix(vector<GLfloat>& m);



#endif	/* _DRAWPLANT_H_ */
