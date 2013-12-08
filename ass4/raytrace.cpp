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
int* objects[3];
OBJTYPE types[3];

//sphere* spheres[3];

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
  
  objects[0] = (int*) makeSphere(0.2, 0.0, -1.5 ,0.1);
  ((sphere*) objects[0])->m = makeMaterial(1.0, 0.1, 0.1, 0.2, 0.3, 0.8, 1.0, 0.8, 0.3);
  types[0] = SPHERE;
  
  objects[1] = (int*) makeSphere(-0.2,0.0, -1.5,0.1);
  ((sphere*) objects[1])->m = makeMaterial(0.1, 0.1, 1.1, 0.2, 0.3, 0.8, 1.0, 0.8, 1.0);
  types[1] = SPHERE;
  
  objects[2] = (int*) makeSphere(0.2,-0.2, -2.5,0.15);
  ((sphere*) objects[2])->m = makeMaterial(0.1, 0.1, 0.1, 0.2, 1.0, 0.4, 1.0, 0.61, 1.0);
  types[2] = SPHERE;


  /*
  objects[0] = (int*) makeCylinder(0.0, 0.0, 0.0, 0.1, 0.1);
  ((cylinder*) objects[0])->m = makeMaterial(1.0, 0.1, 0.1, 0.2, 0.3, 0.8, 1.0, 0.8, 0.3);
  types[0] = CYLINDER;
  */

  lights[0] = makeLight(0.0, 0.0, -1.5);
  lights[0]->c = makeColor(0.5, 0.5, 0.5);

  lights[1] = makeLight(-1.0, 0.0, -1.0);
  lights[1]->c = makeColor(0.5, 0.5, 0.5);
}

void initCamera (int w, int h) {
  viewpoint = makePoint(0.0,0.0,0.0);
  pnear = 1.0;
  fovx = PI/6;
}

void drawScene () {
  int i,j,k;
  GLfloat imageWidth;
  /* declare data structures on stack to avoid dynamic allocation */
  point worldPix[4];  /* current pixel in world coordinates */
  point direction[4]; 
  ray r[4];
  color c[4];
  color avg;

  for (k = 0; k < 4; ++k)
  {
    /* initialize */
    worldPix[k].w = 1.0;
    worldPix[k].z = -pnear;

    r[k].start = &worldPix[k];
    r[k].dir= &direction[k];
  }

  imageWidth = 2*pnear*tan(fovx/2);

  /* trace a ray for every pixel */
  for (i=0; i<width; i++) {
    /* Refresh the display */
    /* Comment this line out after debugging */
    //flushCanvas();

    for (j=0; j<height; j++)
    {
      /* find position of pixel in world coordinates */
      /* y position = (pixel height/middle) scaled to world coords */
      /* x position = (pixel width/middle) scaled to world coords */

      worldPix[0].y = (j-(height/2))*imageWidth/width + 0.00045; 
      worldPix[0].x = (i-(width/2))*imageWidth/width + 0.00045;
      
      worldPix[1].y = (j-(height/2))*imageWidth/width + 0.00045; 
      worldPix[1].x = (i-(width/2))*imageWidth/width - 0.00045;

      //printf("point: (%f, %f) (%f, %f)\n", worldPix[0].x, worldPix[0].y, worldPix[1].x, worldPix[1].y);

      worldPix[2].y = (j-(height/2))*imageWidth/width - 0.00045; 
      worldPix[2].x = (i-(width/2))*imageWidth/width + 0.00045;

      worldPix[3].y = (j-(height/2))*imageWidth/width - 0.00045; 
      worldPix[3].x = (i-(width/2))*imageWidth/width - 0.00045;

      avg.r = 0;
      avg.g = 0;
      avg.b = 0;

      for (k = 0; k < 4; ++k)
      {
        /* find direction */
        /* note: direction vector is NOT NORMALIZED */
        calculateDirection(viewpoint, &worldPix[k], &direction[k]);

        /* trace the ray! */
        traceRay(&r[k], &c[k], 0);

        avg.r += c[k].r;
        avg.g += c[k].g;
        avg.b += c[k].b;
      }

      avg.r /= 4;
      avg.g /= 4;
      avg.b /= 4;

      /* write the pixel! */
      drawPixel(i, j, avg.r, avg.g, avg.b);
    }
  }
  printf("done..\n");
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
  }
  else {             /* nothing was hit */
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
  int hit = FALSE;
  double min = 999999.999;
  unsigned int minValue = -1;
  double distance = 0.0;

  //printf("a\n");
  for(unsigned int i = 0; i < sizeof(objects)/sizeof(int*); ++i)
  {
    switch(types[i])
    {
      case SPHERE:
        hit = raySphereIntersect(r, (sphere*) objects[i], &t);
        break;
      case CYLINDER:
        hit = rayCylinderIntersect(r, (cylinder*) objects[i], &t);
        break;
      case PLANE:
        hit = rayPlaneIntersect(r, (inf_plane*) objects[i], &t);
        break;
    }
   
    if (hit) 
    {
      //printf("d\n");
      findPointOnRay(r,t,p);

      //printf("e\n");
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
    //printf("z\n");
    p->w = 0.0;
  }
  else
  {
    findPointOnRay(r,t,p);
    
    switch(types[minValue])
    {
      case SPHERE:
        findSphereNormal((sphere*) objects[minValue],p,n);
        *m = ((sphere*) objects[minValue])->m;
        break;
      case CYLINDER:
        findCylinderNormal((cylinder*) objects[minValue],p,n);
        *m = ((cylinder*) objects[minValue])->m;
        break;
      case PLANE:
        findPlaneNormal((inf_plane*) objects[minValue],p,n);
        *m = ((inf_plane*) objects[minValue])->m;
        break;
    }
  }
}

