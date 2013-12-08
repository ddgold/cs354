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
  normalize(v);
  /* NOTE: v is not unit length currently, but probably should be */
}

/* given a vector, sets its contents to unit length */
void normalize(vector* v) {
	/* PUT YOUR CODE HERE */
  GLfloat denom = sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
  v->x /= denom;
  v->y /= denom;
  v->z /= denom;
}

/* dat dot doe*/
GLfloat dot(vector* lhs, vector* rhs)
{
  return lhs->x*rhs->x + lhs->y*rhs->y + lhs->z*rhs->z;
}



/* point on ray r parameterized by t is returned in p */
void findPointOnRay(ray* r,double t,point* p) {
  p->x = r->start->x + t * r->dir->x;
  p->y = r->start->y + t * r->dir->y;
  p->z = r->start->z + t * r->dir->z;
  p->w = 1.0;
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

cylinder* makeCylinder(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat hh) {
  cylinder* cyl;
  /* allocate memory */
  cyl = (cylinder*) malloc(sizeof(cylinder));

  /* put stuff in it */
  cyl->c = makePoint(x,y,z);   /* center */
  cyl->r = r;   /* radius */
  cyl->hh = hh;
  cyl->m = NULL;   /* material */
  return(cyl);
}
/* returns TRUE if ray r hits sphere s, with parameter value in t */
int rayCylinderIntersect(ray* r,cylinder* cyl,double* t) {
  point p;   /* start of transformed ray */
  double a,b,c;  /* coefficients of quadratic equation */
  double D;    /* discriminant */
  point* v;
  
  /* transform ray so that sphere center is at origin */
  /* don't use matrix, just translate! */
  p.x = r->start->x - cyl->c->x;
  p.y = r->start->y - cyl->c->y;
  p.z = r->start->z - cyl->c->z;
  v = r->dir; /* point to direction vector */


  a = v->x * v->x  + v->z * v->z;
  b = 2*( v->x * p.x +  v->z * p.z);
  c = p.x * p.x + p.z * p.z - cyl->r * cyl->r;

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
    if (*t < 0)
    {
      *t = (-b + D) / (2*a);
    }
    if (*t < 0)
    { 
      return(FALSE);
    }
    else
    {
      GLfloat y = r->start->y + *t * r->dir->y;
      if (y < cyl->c->y + cyl->hh && y > cyl->c->y - cyl->hh)
      {
        return(TRUE); 
      }
      else
      {
        double top, bottom;
        GLfloat x, z, distance;
        top = (cyl->c->y + cyl->hh - r->start->y) / r->dir->y;
        x = r->start->x + top * r->dir->x;
        z = r->start->z + top * r->dir->z;

        distance = (x - cyl->c->x) * (x - cyl->c->x) + (z - cyl->c->z) * (z - cyl->c->z);
        distance = sqrt(distance);

        if (distance <= cyl->r)
        {
          top = 99999999;
        }

        bottom = (cyl->c->y - cyl->hh - r->start->y) / r->dir->y;
        x = r->start->x + bottom * r->dir->x;
        z = r->start->z + bottom * r->dir->z;

        distance = (x - cyl->c->x) * (x - cyl->c->x) + (z - cyl->c->z) * (z - cyl->c->z);
        distance = sqrt(distance);

        if (distance <= cyl->r)
        {
          bottom = 99999999;
        }

        if (top > bottom)
        {
          *t = bottom;
        }
        else
        {
          *t = top;
        }

        return (*t != 99999999);

      }
      return(TRUE);
    }
  }
}

inf_plane* makePlane(GLfloat x,GLfloat y,GLfloat z ) {
  inf_plane* plane;
  /* allocate memory */
  plane = (inf_plane*) malloc(sizeof(inf_plane));

  /* put stuff in it */
  plane->c = makePoint(x,y,z);   /* center */
  plane->m = NULL;   /* material */
  return(plane);
}

int rayPlaneIntersect(ray* r,inf_plane* s,double* t) {
  //just check y components
  return FALSE;
}

/* normal vector of s at p is returned in n */
/* note: dividing by radius normalizes */
void findSphereNormal(sphere* s, point* p, vector* n) {
  n->x = (p->x - s->c->x) / s->r;  
  n->y = (p->y - s->c->y) / s->r;
  n->z = (p->z - s->c->z) / s->r;
  n->w = 0.0;
  normalize(n);
}

void findCylinderNormal(cylinder* cyl, point* p, vector* n) {
  if(p->y > cyl->c->y + cyl->hh)
  {
    n->x = 0.0;  
    n->y = 1.0;
    n->z = 0.0;
  }
  else if(p->y < cyl->c->y - cyl->hh)
  {
    n->x = 0.0;  
    n->y = -1.0;
    n->z = 0.0;
  }
  else
  {
    n->x = (p->x - cyl->c->x);  
    n->y = 0.0;
    n->z = (p->z - cyl->c->z);
  }
  n->w = 0.0;
  normalize(n);
}

void findPlaneNormal(inf_plane* s, point* p, vector* n) {
  n->x = 0.0;  
  n->y = 1.0;
  n->z = 0.0;
  n->w = 0.0;
  normalize(n);
}