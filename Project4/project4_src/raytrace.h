#ifndef _RAYTRACE_H_
#define _RAYTRACE_H_

/******************************************************************/
/*         Raytracer declarations                                 */
/******************************************************************/


/* constants */
#define TRUE 1
#define FALSE 0

#define PI 3.14159265358979323846264338327

/* data structures */

typedef struct point {
  GLfloat x;
  GLfloat y;
  GLfloat z;
  GLfloat w;
} point;

/* a vector is just a point */
typedef point vector;

/* a light has an origin, a color, and parameters for ambient, diffuse, and specular lighting */
typedef struct light {
  point* origin;

  GLfloat r;
  GLfloat g;
  GLfloat b;

  GLfloat amb;
  GLfloat dif;
  GLfloat spc;
} light;

/* a ray is a start point and a direction */
typedef struct ray {
  point* start;
  vector* dir;
  light** ls;
} ray;

typedef struct material {
  /* color */
  GLfloat r;
  GLfloat g;
  GLfloat b; 
  /* ambient reflectivity */
  GLfloat amb;
  GLfloat dif;
  GLfloat spc;
} material;



typedef struct color {
  GLfloat r;
  GLfloat g;
  GLfloat b; 
  /* these should be between 0 and 1 */
} color;

typedef struct sphere {
  point* c;  /* center */
  GLfloat r;  /* radius */
  material* m;
} sphere;

typedef struct triangle {
  point* a;
  point* b;
  point* c;
  material* m;
} triangle;

/* functions in raytrace.cpp */
void traceRay(ray*, color*, int);

/* functions in geometry.cpp */
sphere* makeSphere(GLfloat, GLfloat, GLfloat, GLfloat);
triangle* makeTriangle(point*,point*,point*);
point* makePoint(GLfloat, GLfloat, GLfloat);
point* copyPoint(point *);
void printVector(vector*);
void freePoint(point *);
void calculateDirection(point*,point*,point*);
void findPointOnRay(ray*,double,point*);
int rayTriangleIntersect(ray*,triangle*,double*);
void findTriangleNormal(triangle*,vector*);
int raySphereIntersect(ray*,sphere*,double*);
void findSphereNormal(sphere*,point*,vector*);
void subtractPoint(point*,point*,vector*);
void scaleVec(vector*,GLfloat,vector*);
GLfloat cosAngBetween(vector*,vector*);
GLfloat dot(vector*,vector*);
GLfloat length(vector*);
void normalize(vector*);
GLfloat clamp(GLfloat,GLfloat,GLfloat);

/* functions in light.cpp */
material* makeMaterial(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
light* makeLight(point*,GLfloat,GLfloat,GLfloat,GLfloat, GLfloat, GLfloat);
void shade(point*,vector*,material*,vector*,color*,light**, int, point*,int);

/* global variables */
extern int width;
extern int height;

#endif	/* _RAYTRACE_H_ */
