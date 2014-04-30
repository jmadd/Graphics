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
#define MAX_PLANE 8

/* local functions */
void initScene(void);
void initCamera (int, int);
void display(void);
void init(int, int);
void traceRay(ray*,color*);
void drawScene(void);
void firstHit(ray*,point*,vector*,material**,void**);

/* local data */

/* the scene: so far, just one sphere */
sphere** spheres;
int numSpheres = 0;

triangle** triangles;
int numTriangles = 0;

plane** planes;
int numPlanes = 0;

int max_depth = 3;
bool antialias = TRUE;


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
  spheres[0] = makeSphere(0.05,0.00,-1.0,0.05);
  spheres[0]->m = makeMaterial(0.5,0.5,0.5,0.5, 0.6, 0.7,0.8,0.2,1.2);
  numSpheres++;
  spheres[1] = makeSphere(.250,0.0,-1.5,0.15);
  spheres[1]->m = makeMaterial(0.7,0.7,0.7,0.5, 0.9, 0.9,1.0,0,1.3);
  numSpheres++;
  spheres[2] = makeSphere(-0.25,0.0,-1.2,0.15);
  spheres[2]->m = makeMaterial(0.15,0.0,0.3,0.5, 0.9, 0.9,0.1,0,1.0);
  numSpheres++;

  triangles = new triangle*[MAX_TRIANGLE];
  triangles[0] = makeTriangle(makePoint(-0.25,0.5,-1.5),makePoint(0.1,0.0,-1.5),makePoint(-0.2,-0.4,-1.3));
  triangles[0]->m = makeMaterial(0.4,0.4,0.4,0.5, 0.9, 0.8,1.0,0,1.0);
  numTriangles++; 

  planes = new plane*[MAX_PLANE];
  planes[0] = makePlane(makePoint(0,0,-2), makePoint(0,0,1));
  planes[0]->m = makeMaterial(1.0,1.0,1.0,0.25, .35, .3,1.0,0,1.0);
  //numPlanes++;
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
  point wp1,wp2,wp3,wp4;  
		  point dir1,dir2,dir3,dir4; 
		  ray r1,r2,r3,r4;
	      color c1,c2,c3,c4;

  light** ls = new light*[2];
  ls[0] = makeLight(makePoint(0.0,0.0,0),1.0,1.0,1.0,0.4, 0.7, 0.6);
  ls[1] = makeLight(makePoint(0.5,1.0,-1.0),1.0,1.0,1.0,0.4, 0.7, 0.6);

  /* initialize */
  worldPix.w = 1.0;
  worldPix.z = -pnear;

  r.start = &worldPix;
  r.dir= &direction;
  r.ls = ls;
  c.r=0;
  c.g=0;
  c.b=0;

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

	  if(antialias){
		  

		  r1.start=&wp1;
		  r2.start=&wp2;
		  r3.start=&wp3;
		  r4.start=&wp4;
		  r1.dir=&dir1;
		  r2.dir=&dir2;
		  r3.dir=&dir3;
		  r4.dir=&dir4;
		  r1.ls = ls;
		  r2.ls = ls;
		  r3.ls = ls;
		  r4.ls = ls;

		  wp1.w=1.0;
		  wp1.z=-pnear;
		  wp2.w=1.0;
		  wp2.z=-pnear;
		  wp3.w=1.0;
		  wp3.z=-pnear;
		  wp4.w=1.0;
		  wp4.z=-pnear;

		  wp1.y=(j-(height/2))*imageWidth/width;
		  wp1.x=(i-(width/2))*imageWidth/width;
		  wp2.y=(j-(height/2)+0.5)*imageWidth/width;
		  wp2.x=(i-(width/2))*imageWidth/width;
		  wp3.y=(j-(height/2))*imageWidth/width;
		  wp3.x=(i-(width/2)+0.5)*imageWidth/width;
		  wp4.y=(j-(height/2)+0.5)*imageWidth/width;
		  wp4.x=(i-(width/2)+0.5)*imageWidth/width;

		  calculateDirection(viewpoint,&wp1,&dir1);
		  calculateDirection(viewpoint,&wp2,&dir2);
		  calculateDirection(viewpoint,&wp3,&dir3);
		  calculateDirection(viewpoint,&wp4,&dir4);

		  traceRay(&r1,&c1,0,NULL);
		  traceRay(&r2,&c2,0,NULL);
		  traceRay(&r3,&c3,0,NULL);
		  traceRay(&r4,&c4,0,NULL);

		  c.r=(c1.r+c2.r+c3.r+c4.r)/4;
		  c.g=(c1.g+c2.g+c3.g+c4.g)/4;
		  c.b=(c1.b+c2.b+c3.b+c4.b)/4;

		  drawPixel(i,j,c.r,c.g,c.b);
	  } else{
		/* trace the ray! */
		traceRay(&r,&c,0,NULL);
		/* write the pixel! */
		drawPixel(i,j,c.r,c.g,c.b);
	  }
    }
  }
}


/* returns the color seen by ray r in parameter c */
/* d is the recursive depth */
void traceRay(ray* r, color* c, int d, void* last) {
  if(d > max_depth){
    c->r=0.0;
    c->g=0.0;
    c->b=0.0;
    return;
  }
  point p;  /* first intersection point */
  vector n;
  material* m;
  

  p.w = 0.0;  /* inialize to "no intersection" */
  firstHit(r,&p,&n,&m,&last);

  if (p.w != 0.0) {
    
    shade(&p,&n,m,r,r->dir,c,r->ls,2, viewpoint,d,last);  /* do the lighting calculations */
  } else {             /* nothing was hit */
    c->r = 0.0;
    c->g = 0.0;
    c->b = 0.0;
  }
}

void calculateReflection(ray* r, vector* n, point* p, ray* reflect){
  reflect->ls = r->ls;
  reflect->start = makePoint(0,0,0);
  scaleVec(p,1,reflect->start);
  reflect->dir = makePoint(0,0,0);
  vector tmp;
  scaleVec(n,2*dot(r->dir,n),&tmp);
  subtractPoint(r->dir,&tmp,reflect->dir);

}

void calculateRefraction(ray* r, vector* n, point* p, GLfloat refAng, ray* refract) {
  refract->ls = r->ls;
  refract->start = makePoint(0,0,0);
  refract->dir = makePoint(0,0,0);
  scaleVec(p,1,refract->start);
  vector dir;
  vector tmp;
  GLfloat c1 = - dot(n, r->dir);
  GLfloat N = 1 / refAng;
  GLfloat c2 = sqrt(1 - N*N * (1 - c1*c1));
  scaleVec(r->dir,N,&tmp);
  scaleVec(n,N*c1-c2,&dir);
  addPoint(&tmp,&dir,refract->dir);
  snellIntersect(refract,p,&(p->shape),&tmp);
  scaleVec(&tmp,1,refract->start);
  scaleVec(r->dir,1,refract->dir);

}

bool checkShadow(point* p, light* l, void* last){
  ray r;
  r.start = p;
  r.dir = makePoint(0,0,0);
  subtractPoint(l->origin,p,r.dir);

  return checkHit(&r,&last);
}

bool checkHit(ray* r, void** last){
  int hit = FALSE;
  double t = 0;
  int i = 0;
  
  
  while(i < numSpheres){
    hit = raySphereIntersect(r,spheres[i],&t);
    //printf("%d   %d\n", *last, spheres+1);

    if (hit && ((spheres+i)!=*last)){
      return TRUE;
    }
    i++;
  }

  i=0;
  while(i < numTriangles){
    hit = rayTriangleIntersect(r,triangles[i],&t);
    if (hit && ((triangles+i)!=*last)) {
      return TRUE;
    }
    i++;
  }


  i=0;
  while(i < numPlanes){
    hit = rayPlaneIntersect(r,planes[i],&t);
    if (hit && ((planes+i)!=*last)) {
      return TRUE;
    }
    i++;
  }  
  return FALSE;
}

void snellIntersect(ray* r, point* p, void** last, point* result){
  int hit = FALSE;
  double t = 0;
  int i = 0;
  ray in;
  in.start=p;
  in.dir=r->dir;
  in.ls=r->ls;
  
  while(i < numSpheres){
    

    if (((spheres+i)==*last)){
      //intersect
	  raySphereIntersectLast(&in,spheres[i],&t);
	  findPointOnRay(&in,t,result);
	  return;
    }
    i++;
  }

  i=0;
  while(i < numTriangles){
    
    if (((triangles+i)==*last)) {
	  rayTriangleIntersect(&in,triangles[i],&t);
	  findPointOnRay(&in,t,result);
      return;
    }
    i++;
  }


  i=0;
  while(i < numPlanes){
    
    if (((planes+i)==*last)) {
	  rayPlaneIntersect(&in,planes[i],&t);
	  findPointOnRay(&in,t,result);
      return;
    }
    i++;
  }  
  
}

/* firstHit */
/* If something is hit, returns the finite intersection point p, 
   the normal vector n to the surface at that point, and the surface
   material m. If no hit, returns an infinite point (p->w = 0.0) */
void firstHit(ray* r, point* p, vector* n, material* *m,void**last) {
  double t = 0;     /* parameter value at first hit */
  double T = 1.0e8;
  int hit = FALSE;
  int i = 0;
  
  
  while(i < numSpheres){
	if((spheres+i)==*last){
		i++;
		continue;
	}
    hit = raySphereIntersect(r,spheres[i],&t);

    if (hit) {
  	  if(t <= T){
    		T=t;
    		*m = spheres[i]->m;
			p->shape = spheres + i;
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
	if(((triangles+i)==*last)){
		i++;
		continue;
	}
    hit = rayTriangleIntersect(r,triangles[i],&t);
    if (hit) {
  	  if(t <= T){
    		T=t;
    		*m = triangles[i]->m;
			p->shape = triangles + i;
    		findPointOnRay(r,T,p);
    		findTriangleNormal(triangles[i],n);
    		if(dot(n,r->dir) > 0){
    			scaleVec(n,-1,n);   //fixed lighting for triangle
    		}
  	  }
    } else if(t == 1.0e8){
        /* indicates no hit */
        p->w = 0.0;
    }
    i++;
  	hit=FALSE;
  }


  i=0;
  while(i < numPlanes){
	if(((planes+i)!=*last)){
		i++;
		continue;
	}
    hit = rayPlaneIntersect(r,planes[i],&t);
    if (hit) {
		if(t <= T){
			T=t;
			*m = planes[i]->m;
			p->shape = planes + i;
			findPointOnRay(r,T,p);
			scaleVec(planes[i]->normal, 1, n);  //store normal values in n
			if(dot(n,r->dir) > 0){
			  scaleVec(n,-1,n);   //fixed lighting for triangle
			}
		
		}
    } else if(t == 1.0e8){
      /* indicates no hit */
      p->w = 0.0;
    }
    i++;
	hit=FALSE;
  }  

}

