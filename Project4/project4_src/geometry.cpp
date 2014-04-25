/******************************************************************/
/*         Geometry functions                                     */
/*                                                                */
/* Group Members: <FILL IN>                                       */
/******************************************************************/

#ifdef _WIN32
#include <windows.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "common.h"
#include "raytrace.h"

GLfloat clamp(GLfloat v, GLfloat l, GLfloat h){
  return (v < l) ? l : ((v > h) ? h : v);
}

void subtractPoint(point* a, point* b, vector* r){
  r->x = a->x-b->x;
  r->y = a->y-b->y;
  r->z = a->z-b->z;
  r->w = a->w-b->w;
}

GLfloat dot(vector* a, vector* b){
  return a->x*b->x + a->y*b->y + a->z*b->z;
}

GLfloat length(vector* a){
  return sqrt(a->x*a->x+a->y*a->y+a->z*a->z);
}

GLfloat cosAngBetween(vector* a, vector* b){
  return dot(a,b)/(length(a)*length(b));
}

void crossProduct(vector* a, vector* b, vector* r){
  r->x = a->y*b->z - a->z*b->y;
  r->y = a->z*b->x - a->x*b->z;
  r->z = a->x*b->y - a->y*b->x;
}

void scaleVec(vector* a, GLfloat scl, vector* r){
  r->x = a->x*scl;
  r->y = a->y*scl;
  r->z = a->z*scl;
}

void printVector(vector* a){
	printf("<%f, %f, %f>\n", a->x,a->y,a->z);
}

point* makePoint(GLfloat x, GLfloat y, GLfloat z) {
  point* p;
  /* allocate memory */
  p = (point*) malloc(sizeof(point));
  /* put stuff in it */
  p->x = x; p->y = y; p->z = z; 
  p->w = 1.0;
  return (p);
}

/* makes copy of point (or vector) */
point* copyPoint(point *p0) {
  point* p;
  /* allocate memory */
  p = (point*) malloc(sizeof(point));

  p->x = p0->x;
  p->y = p0->y;
  p->z = p0->z;
  p->w = p0->w;         /* copies over vector or point status */
  return (p);
}

/* unallocates a point */
void freePoint(point *p) {
  if (p != NULL) {
    free(p);
  }
}

/* vector from point p to point q is returned in v */
void calculateDirection(point* p, point* q, point* v) {
  v->x = q->x - p->x;
  v->y = q->y - p->y;
  v->z = q->z - p->z;
  /* a direction is a point at infinity */
  v->w = 0.0;

  /* NOTE: v is not unit length currently, but probably should be */
}

/* given a vector, sets its contents to unit length */
void normalize(vector* v) {
	/* PUT YOUR CODE HERE */
  scaleVec(v,1/length(v),v);
}

/* point on ray r parameterized by t is returned in p */
void findPointOnRay(ray* r,double t,point* p) {
  p->x = r->start->x + t * r->dir->x;
  p->y = r->start->y + t * r->dir->y;
  p->z = r->start->z + t * r->dir->z;
  p->w = 1.0;
}

/* TRIANGLES */

triangle* makeTriangle(point* a, point* b, point* c){
  triangle* t;

  t = (triangle*) malloc(sizeof(triangle));

  t->a=a;
  t->b=b;
  t->c=c;
  t->m = NULL;
  return t;
}

int rayTriangleIntersect(ray* r, triangle* tri, double* t){
  
  vector* n = makePoint(0,0,0);
  GLfloat d;
  GLfloat tmp;
  findTriangleNormal(tri,n);
  d=dot(n,tri->a);
  tmp=(d-dot(n,r->start))/dot(n,r->dir);
  point* q = makePoint(0,0,0);
  findPointOnRay(r,tmp,q);

  vector* v1 = makePoint(0,0,0);
  vector* v2 = makePoint(0,0,0);
  vector* cross = makePoint(0,0,0);
  subtractPoint(tri->b,tri->a,v1);
  subtractPoint(q,tri->a,v2);
  crossProduct(v1,v2,cross);
  if(dot(cross,n)<0){
    return false;
  }

  subtractPoint(tri->c,tri->b,v1);
  subtractPoint(q,tri->b,v2);
  crossProduct(v1,v2,cross);
  if(dot(cross,n)<0){
    return false;
  }

  subtractPoint(tri->a,tri->c,v1);
  subtractPoint(q,tri->c,v2);
  crossProduct(v1,v2,cross);
  if(dot(cross,n)<0){
    return false;
  }

  *t = tmp;
  if(tmp < 0)return false;
  return true;
  

}

void findTriangleNormal(triangle* tri, vector* n){
  vector* v1 = makePoint(0,0,0);
  vector* v2 = makePoint(0,0,0);
  subtractPoint(tri->b,tri->a,v1);
  subtractPoint(tri->c,tri->a,v2);
  crossProduct(v1,v2,n);
  scaleVec(n,1/length(n),n);
}

/* PLANES */

plane* makePlane(vector* normal, point* pt) {
  plane* p;
  p = (plane*) malloc(sizeof(plane));
  p->normal = normal;
  normalize(p->normal);
  p->p = pt;
  p->m = NULL;
  return p; 
}

int rayPlaneIntersect(ray* r, plane* p, double* t){
  
  GLfloat d;
  GLfloat tmp;
  d=dot(p->normal,p->p);
  tmp=(d-dot(p->normal,r->start))/dot(p->normal,r->dir);
  point* q = makePoint(0,0,0);
  findPointOnRay(r,tmp,q);

  *t = tmp;
  if(tmp < 0)return false;
  return true;
  

}

/* SPHERES */

sphere* makeSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat r) {
  sphere* s;
  /* allocate memory */
  s = (sphere*) malloc(sizeof(sphere));

  /* put stuff in it */
  s->c = makePoint(x,y,z);   /* center */
  s->r = r;   /* radius */
  s->m = NULL;   /* material */
  return(s);
}

/* returns TRUE if ray r hits sphere s, with parameter value in t */
int raySphereIntersect(ray* r,sphere* s,double* t) {
  point p;   /* start of transformed ray */
  double a,b,c;  /* coefficients of quadratic equation */
  double D;    /* discriminant */
  point* v;
  
  /* transform ray so that sphere center is at origin */
  /* don't use matrix, just translate! */
  p.x = r->start->x - s->c->x;
  p.y = r->start->y - s->c->y;
  p.z = r->start->z - s->c->z;
  v = r->dir; /* point to direction vector */


  a = v->x * v->x  +  v->y * v->y  +  v->z * v->z;
  b = 2*( v->x * p.x  +  v->y * p.y  +  v->z * p.z);
  c = p.x * p.x + p.y * p.y + p.z * p.z - s->r * s->r;

  D = b * b - 4 * a * c;
  
  if (D < 0) {  /* no intersection */
    return (FALSE);
  }
  else {
    D = sqrt(D);
    /* First check the root with the lower value of t: */
    /* this one, since D is positive */
    *t = (-b - D) / (2*a);
    /* ignore roots which are less than zero (behind viewpoint) */
    if (*t < 0) {
      *t = (-b + D) / (2*a);
    }
    if (*t < 0) { return(FALSE); }
    else return(TRUE);
  }
}

/* normal vector of s at p is returned in n */
/* note: dividing by radius normalizes */
void findSphereNormal(sphere* s, point* p, vector* n) {
  n->x = (p->x - s->c->x) / s->r;  
  n->y = (p->y - s->c->y) / s->r;
  n->z = (p->z - s->c->z) / s->r;
  n->w = 0.0;
}


