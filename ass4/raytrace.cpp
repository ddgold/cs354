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
sphere* spheres[2];

light* lights[2];


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
  glOrtho(0.0, 1.0, 0.0, 1.0, -10.0, 10.0);
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
  spheres[0] = makeSphere(0.0,0.0,-2.0,0.1);
  spheres[0]->m = makeMaterial(1.0,0.1,0.15,0.2,0.3,0.8,1.0);

  spheres[1] = makeSphere(0.1, 0.1,-2.0,0.1);
  spheres[1]->m = makeMaterial(1.0,0.1,0.15,0.5,0.6,0.8,1.0);

  lights[0] = makeLight(8.0, 8.0, -1.0);
  lights[0]->c = makeColor(1.0, 1.0, 1.0);

  lights[1] = makeLight(-2.0, -2.0, 0.0);
  lights[1]->c = makeColor(0.0, 0.0, 1.0);
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

  /* initialize */
  worldPix.w = 1.0;
  worldPix.z = -pnear;

  r.start = &worldPix;
  r.dir= &direction;

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
    shade(&p,&n,m,r->dir,c,d);  /* do the lighting calculations */
  } else {             /* nothing was hit */
    c->r = 0.0;
    c->g = 0.0;
    c->b = 0.0;
  }

 // printf("color: (%f, %f, %f)\n", c->r, c->g, c->b);

}

/* firstHit */
/* If something is hit, returns the finite intersection point p, 
   the normal vector n to the surface at that point, and the surface
   material m. If no hit, returns an infinite point (p->w = 0.0) */
void firstHit(ray* r, point* p, vector* n, material* *m) {
  double t = 0;     /* parameter value at first hit */
  int hit = FALSE;
  double min = 999999.999;
  unsigned int minValue = -1;
  double distance = 0.0;

  for(unsigned int i = 0; i < sizeof(spheres)/sizeof(sphere*); ++i)
  {
    hit = raySphereIntersect(r,spheres[i],&t);
    if (hit) 
    {
      findPointOnRay(r,t,p);

      distance =  (r->start->x - p->x) * (r->start->x - p->x);
      distance += (r->start->y - p->y) * (r->start->y - p->y);
      distance += (r->start->z - p->z) * (r->start->z - p->z);
      distance = sqrt(distance);

      if (distance < min)
      {
        min = distance;
        minValue = i;
      }
    } 
  }

  if (minValue == -1)
  {
    p->w = 0.0;
  }
  else
  {
    *m = spheres[minValue]->m;
    findPointOnRay(r,t,p);
    findSphereNormal(spheres[minValue],p,n);
  }
}
