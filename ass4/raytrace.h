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
typedef enum {
  SPHERE,
  CYLINDER,
  PLANE
} OBJTYPE;


typedef struct point {
  GLfloat x;
  GLfloat y;
  GLfloat z;
  GLfloat w;
} point;

/* a vector is just a point */
typedef point vector;

/* a ray is a start point and a direction */
typedef struct ray {
  point* start;
  vector* dir;
} ray;

typedef struct material {
  /* color */
  GLfloat r;
  GLfloat g;
  GLfloat b; 
  
  GLfloat amb;
  GLfloat dif;
  GLfloat spec;
  GLfloat expo;

  GLfloat shine;
  GLfloat alpha;

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

typedef struct cylinder {
  point* c;
  GLfloat r;
  GLfloat hh;
  material* m;
} cylinder;

typedef struct inf_plane
{
  point* c;
  material* m;
} inf_plane;


typedef struct light {
  point* p;
  color* c;
} light;

/* functions in raytrace.cpp */
void traceRay(ray*, color*, int);
void firstHit(ray*,point*,vector*,material**);

/* functions in geometry.cpp */
sphere* makeSphere(GLfloat, GLfloat, GLfloat, GLfloat);
cylinder* makeCylinder(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
inf_plane* makePlane(GLfloat,GLfloat,GLfloat);
point* makePoint(GLfloat, GLfloat, GLfloat);
point* copyPoint(point *);
void freePoint(point *);
void calculateDirection(point*,point*,point*);
void findPointOnRay(ray*,double,point*);
int raySphereIntersect(ray*,sphere*,double*);
int rayCylinderIntersect(ray*,cylinder*,double*);
int rayPlaneIntersect(ray*,inf_plane*,double*);
void findSphereNormal(sphere*,point*,vector*);
void findCylinderNormal(cylinder*,point*,vector*);
void findPlaneNormal(inf_plane*,point*,vector*);
void normalize(vector*);
GLfloat dot(vector*, vector*);

/* functions in light.cpp */
material* makeMaterial(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
color* makeColor(GLfloat, GLfloat, GLfloat);
light* makeLight(GLfloat, GLfloat, GLfloat);
void shade(point*,vector*,material*,vector*,color*,int);
bool shadow(ray*);

/* global variables */
extern int width;
extern int height;

//extern sphere* spheres[3];
extern int* objects[3];
extern OBJTYPE types[3];

extern light* lights[2];

#endif	/* _RAYTRACE_H_ */
