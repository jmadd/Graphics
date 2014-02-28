#ifndef _DRAWPLANT_H_
#define _DRAWPLANT_H_

/* Functions implemented in drawplant.cpp */



void drawPlant(void);
void drawLeaf(void);
void load3DMatrix(GLfloat* m);
GLfloat* vecByMat(GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3,
		GLfloat m00, GLfloat m01, GLfloat m02, GLfloat m03,
		GLfloat m10, GLfloat m11, GLfloat m12, GLfloat m13,
		GLfloat m20, GLfloat m21, GLfloat m22, GLfloat m23,
		GLfloat m30, GLfloat m31, GLfloat m32, GLfloat m33);
GLfloat* vecByMat(const GLfloat* v,const  GLfloat* m);

void pushState(const GLfloat*);
GLfloat* popState(void);
GLfloat* translate(GLfloat x, GLfloat y=0,GLfloat z=0);
void drawLeaf(int);
void drawBranch(int);
void drawTree(int);


GLfloat* matByMat(const GLfloat* m,const  GLfloat* n);

void printMatrix(GLfloat* m);



#endif	/* _DRAWPLANT_H_ */