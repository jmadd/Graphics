/******************************************************************/
/*         Main raytracer file                                    */
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
#include "lowlevel.h"
#include "raytrace.h"

#define MAX_SPHERE 8
#define MAX_TRIANGLE 8
#define MAX_LIGHT 8

/* local functions */
void initScene(void);
void initCamera (int, int);
void display(void);
void init(int, int);
void traceRay(ray*,color*);
void drawScene(void);
void firstHit(ray*,point*,vector*,material**);

/* local data */

/* the scene: so far, just one sphere */
sphere** spheres;
int numSpheres = 0;

triangle** triangles;
int numTriangles = 0;


/* the viewing parameters: */
point* viewpoint;
GLfloat pnear;  /* distance from viewpoint to image plane */
GLfloat fovx;  /* x-angle of view frustum */
int width = 500;     /* width of window in pixels */
int height = 350;    /* height of window in pixels */

int main (int argc, char** argv) {
  int win;

  glutInit(&argc,argv);
  glutInitWindowSize(width,height);
  glutInitWindowPosition(100,100);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  win = glutCreateWindow("raytrace");
  glutSetWindow(win);
  init(width,height);
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}

void init(int w, int h) {

  /* OpenGL setup */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
  glClearColor(0.0, 0.0, 0.0, 0.0);  

  /* low-level graphics setup */
  initCanvas(w,h);

  /* raytracer setup */
  initCamera(w,h);
  initScene();
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  drawScene();  /* draws the picture in the canvas */
  flushCanvas();  /* draw the canvas to the OpenGL window */
  glFlush();
}

void initScene () {
  spheres = new sphere*[MAX_SPHERE];
  spheres[0] = makeSphere(-.5,0.0,-2.0,0.25);
  spheres[0]->m = makeMaterial(1.0,0.1,0.15,0.5, 0.9, 0.9);
  numSpheres++;
  spheres[1] = makeSphere(.5,0.0,-2.0,0.25);
  spheres[1]->m = makeMaterial(0.1,1.0,0.15,0.5, 0.9, 0.9);
  numSpheres++;
  spheres[2] = makeSphere(0.0,0.0,-4.0,0.25);
  spheres[2]->m = makeMaterial(0.15,1.0,1.0,0.5, 0.9, 0.9);
  numSpheres++;

  triangles = new triangle*[MAX_TRIANGLE];
  triangles[0] = makeTriangle(makePoint(-.25,-.15,-1.0),makePoint(0,0.25,-2.0),makePoint(0.5,0.0,-3.0));
  triangles[0]->m = makeMaterial(0.1,0.35,0.20,0.5, 1.0, 1.0);
  numTriangles++; 
}

void initCamera (int w, int h) {
  viewpoint = makePoint(0.0,0.0,0.0);
  pnear = 1.0;
  fovx = PI/6;
}

void drawScene () {
  int i,j;
  GLfloat imageWidth;
  /* declare data structures on stack to avoid dynamic allocation */
  point worldPix;  /* current pixel in world coordinates */
  point direction; 
  ray r;
  color c;
  light** ls = new light*[2];
  ls[0] = makeLight(makePoint(0.0,0.5,-1.5),1.0,1.0,1.0,0.5, 0.5, 0.5);
  ls[1] = makeLight(makePoint(0.5,2.0,-1.0),1.0,1.0,1.0,0.5, 0.5, 0.5);

  /* initialize */
  worldPix.w = 1.0;
  worldPix.z = -pnear;

  r.start = &worldPix;
  r.dir= &direction;
  r.ls = ls;

  imageWidth = 2*pnear*tan(fovx/2);

  /* trace a ray for every pixel */
  for (i=0; i<width; i++) {
    /* Refresh the display */
    /* Comment this line out after debugging */
    flushCanvas();

    for (j=0; j<height; j++) {

      /* find position of pixel in world coordinates */
      /* y position = (pixel height/middle) scaled to world coords */ 
      worldPix.y = (j-(height/2))*imageWidth/width;
      /* x position = (pixel width/middle) scaled to world coords */ 
      worldPix.x = (i-(width/2))*imageWidth/width;

      /* find direction */
      /* note: direction vector is NOT NORMALIZED */
      calculateDirection(viewpoint,&worldPix,&direction);

      /* trace the ray! */
      traceRay(&r,&c,0);
      /* write the pixel! */
      drawPixel(i,j,c.r,c.g,c.b);
    }
  }
}

/* returns the color seen by ray r in parameter c */
/* d is the recursive depth */
void traceRay(ray* r, color* c, int d) {
  point p;  /* first intersection point */
  vector n;
  material* m;

  p.w = 0.0;  /* inialize to "no intersection" */
  firstHit(r,&p,&n,&m);

  if (p.w != 0.0) {
    shade(&p,&n,m,r->dir,c,r->ls,2, viewpoint,d);  /* do the lighting calculations */
  } else {             /* nothing was hit */
    c->r = 0.0;
    c->g = 0.0;
    c->b = 0.0;
  }
}

/* firstHit */
/* If something is hit, returns the finite intersection point p, 
   the normal vector n to the surface at that point, and the surface
   material m. If no hit, returns an infinite point (p->w = 0.0) */
void firstHit(ray* r, point* p, vector* n, material* *m) {
  double t = 0;     /* parameter value at first hit */
  double T = 1.0e8;
  int hit = FALSE;
  int i = 0;
  
  
  while(i < numSpheres){
    hit = raySphereIntersect(r,spheres[i],&t);
    if (hit) {
	  if(t <= T){
		T=t;
		*m = spheres[i]->m;
		findPointOnRay(r,T,p);
		findSphereNormal(spheres[i],p,n);
	  }
    } else if(t == 1.0e8){
      //indicates no hit 
      p->w = 0.0;
    }
    i++;
	hit=FALSE;
  }

  i=0;
  while(i < numTriangles){
    hit = rayTriangleIntersect(r,triangles[i],&t);
    if (hit) {
	  if(t <= T){
		T=t;
		*m = triangles[i]->m;
		findPointOnRay(r,T,p);
		findTriangleNormal(triangles[i],n);
		if(dot(n,r->dir) > 0){
			scaleVec(n,-1,n);   //fixed lighting for triangle
		}
		//printVector(p);
	  }
    } else if(t == 1.0e8){
      /* indicates no hit */
      p->w = 0.0;
    }
    i++;
	hit=FALSE;
  }

}

