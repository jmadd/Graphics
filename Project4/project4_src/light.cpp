/******************************************************************/
/*         Lighting functions                                     */
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

material* makeMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat amb, GLfloat dif, GLfloat spc) {
  material* m;
  
  /* allocate memory */
  m = (material*) malloc(sizeof(material));
  /* put stuff in it */
  m->r = r;
  m->g = g;
  m->b = b;
  m->amb = amb;
  m->dif = dif;
  m->spc = spc;
  return(m);
}

light* makeLight(point* origin, GLfloat r, GLfloat g, GLfloat b, GLfloat amb, GLfloat dif, GLfloat spc) {
  light* m;
  
  /* allocate memory */
  m = (light*) malloc(sizeof(light));
  /* put stuff in it */
  m->origin = origin;
  m->r = r;
  m->g = g;
  m->b = b;
  m->amb = amb;
  m->dif = dif;
  m->spc = spc;
  return(m);
}

/* LIGHTING CALCULATIONS */

/* shade */
/* color of point p with normal vector n and material m returned in c */
/* in is the direction of the incoming ray and d is the recusive depth */
void shade(point* p, vector* n, material* m, ray* r, vector* in, color* c, light** ls, int numLights, point* vp, GLfloat weight, int d,void* last) {

  GLfloat amb=0;
  GLfloat dif=0;
  GLfloat spc=0;
  color rc;
  c->r=0;
  c->g=0;
  c->b=0;
  rc.r=0;
  rc.g=0;
  rc.b=0;

  for(int i = 0; i < numLights; i++){
    light* l = ls[i];
    //calculate ambient light
    amb += m->amb * l->amb;

    //calculate diffuse light
    vector* L = makePoint(0,0,0);
    subtractPoint(l->origin,p,L);
	  normalize(L);
	  GLfloat cosAng = dot(n,L);

    dif += m->dif * l->dif * clamp(cosAng, 0, 1);
	

    //calculate specular light
    vector* D = makePoint(0,0,0);
    scaleVec(L,-1.0,D);
    vector* N = makePoint(0,0,0);
    scaleVec(n,1/length(n),N);
    vector* tmp = makePoint(0,0,0);
    scaleVec(N,2*dot(D,N),tmp);
    vector* R = makePoint(0,0,0);
    subtractPoint(D,tmp,R);
    vector* V = makePoint(0,0,0);
    subtractPoint(vp,p,V);
    spc += m->spc * l->spc * pow(clamp(cosAngBetween(R,V),0.0,1.0),8);
  }

  
  /* so far, just finds ambient component of color */
  c->r = (amb+dif+spc) * m->r;
  c->g = (amb+dif+spc) * m->g;
  c->b = (amb+dif+spc) * m->b;

  GLfloat krg = 1.0;
  ray reflect;
  calculateReflection(r,n,p,&reflect);
  traceRay(&reflect,&rc,krg*weight,d+1,p->shape);

  c->r += krg*rc.r;
  c->g += krg*rc.g;
  c->b += krg*rc.b;
  
  /* clamp color values to 1.0 */
  if (c->r > 1.0) c->r = 1.0;
  if (c->g > 1.0) c->g = 1.0;
  if (c->b > 1.0) c->b = 1.0;

}

