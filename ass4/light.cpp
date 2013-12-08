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

material* makeMaterial(GLfloat r, GLfloat g, GLfloat b,
                       GLfloat amb, GLfloat dif, GLfloat spec, GLfloat expo,
                       GLfloat shine, GLfloat alpha) {
  material* m;
  
  /* allocate memory */
  m = (material*) malloc(sizeof(material));
  /* put stuff in it */
  m->r = r;
  m->g = g;
  m->b = b;

  m->amb  = amb;
  m->dif  = dif;
  m->spec = spec;
  m->expo = expo;

  m->shine = shine;
  m->alpha = alpha;
  return(m);
}

color* makeColor(GLfloat r, GLfloat g, GLfloat b) {
  color* c;

  c = (color*) malloc(sizeof(color));
  c->r = r;
  c->g = g;
  c->b = b;
  return(c);
}

/* LIGHT */

light* makeLight(GLfloat x, GLfloat y, GLfloat z) {
  light* l;
  l = (light*) malloc(sizeof(light));

  l->p = makePoint(x, y, z);
  l->c = NULL;
  return(l);
}





/* LIGHTING CALCULATIONS */
int test = 0;
/* shade */
/* color of point p with normal vector n and material m returned in c */
/* in is the direction of the incoming ray and d is the recusive depth */
void shade(point* p, vector* n, material* m, vector* in, color* c, int d) {
  vector* s = makePoint(0, 0, 0);
  vector* v = makePoint( in->x * -1,  in->y * -1,  in->z * -1);
  vector* h = makePoint(0,0,0);

  ray* shadowFeeler = (ray*)malloc(sizeof(ray));
  shadowFeeler->start = makePoint(p->x + .0005 * v->x,p->y + .0005 * v->y,p->z + .0005 * v->z);
  shadowFeeler->dir   = makePoint(0,0,0); 

  GLfloat lambert = 0.0;
  GLfloat phong = 0.0;

  // Ambient Portion
  c->r =  m->amb * m->r;
  c->g =  m->amb * m->g;
  c->b =  m->amb * m->b;

  for(unsigned int i = 0; i < sizeof(lights)/sizeof(light*); ++i)
  {
    calculateDirection(p, lights[i]->p, s);
    calculateDirection(shadowFeeler->start,lights[i]->p, shadowFeeler->dir);

    //printf("entering shadow\n");
    if(!shadow(shadowFeeler))
    {
      h = makePoint(v->x + s->x, v->y + s->y, v->z + s->z);

      lambert = dot(s,n);
      phong = dot(h,n)/ sqrt(h->x*h->x + h->y*h->y + h->z*h->z);

      if (test == 500)
      {
        printf("point: (%f, %f, %f, %f)\n", p->x, p->y, p->z, p->w);
        printf("normal: (%f, %f, %f, %f)\n", n->x, n->y, n->z, n->w);
        printf("view: (%f, %f, %f, %f)\n", in->x, in->y, in->z, in->w);
        printf("light: (%f, %f, %f, %f)\n", s->x, s->y, s->z, s->w);

        printf("lambert %f\n", lambert);
        printf("phong %f\n", phong);
        
      }
      ++test; 

      // Difustion Portion
      if(lambert > 0 )
      {
        c->r += lambert * lights[i]->c->r * m->dif * m->r;
        c->g += lambert * lights[i]->c->g * m->dif * m->g;
        c->b += lambert * lights[i]->c->b * m->dif * m->b;
      }

      // Spectrial Portion
      if(phong > 0)
      {
        phong = pow(phong,m->expo);
        c->r += phong * lights[i]->c->r * m->spec * m->r;
        c->g += phong * lights[i]->c->g * m->spec * m->g;
        c->b += phong * lights[i]->c->b * m->spec * m->b;
      }
    }
  }

  if (d < 4)
  {
    if(m->shine > 0.6)
    {
      GLfloat reflect = dot(in, n);
      color* c2 = makeColor(0,0,0);

      ray* r = (ray*)malloc(sizeof(ray));
      r->start = makePoint(p->x + 0.0005 * v->x, p->y + 0.0005 * v->y, p->z + 0.0005 * v->z);
      r->dir = makePoint(in->x - 2 * reflect * n->x,
                         in->y - 2 * reflect * n->y,
                         in->z - 2 * reflect * n->z);
      traceRay(r, c2, d + 1);

      //if (c2->r > 0.0 || c2->g > 0.0 || c2->b > 0.0)
        //printf("color @ (%f, %f, %f) = (%f, %f, %f)\n", p->x, p->y, p->z, c2->r, c2->g, c2->b);
      
      c->r += m->shine * c2->r;
      c->g += m->shine * c2->g;
      c->b += m->shine * c2->b;
    }

    if(m->alpha < 0.9)
    {

      color* c3 = makeColor(0,0,0);

      ray* t = (ray*)malloc(sizeof(ray));
      t->start = makePoint(p->x - 0.0005 * v->x, p->y - 0.0005 * v->y, p->z - 0.0005 * v->z);
      t->dir = makePoint((.8 * v->x + .2 * n->x)* -1,
                         (.8 * v->y + .2 * n->y)* -1,
                         (.8 * v->z + .2 * n->z)* -1);

      traceRay(t, c3, d + 1);

      c->r += (1 - m->alpha) * c3->r;
      c->g += (1 - m->alpha) * c3->g;
      c->b += (1 - m->alpha) * c3->b;


    }
  }
  
  /* clamp color values to 1.0 */
  if (c->r > 1.0) c->r = 1.0;
  if (c->g > 1.0) c->g = 1.0;
  if (c->b > 1.0) c->b = 1.0;

}

bool shadow(ray *r)
{
  point p;  /* first intersection point */
  vector n;
  material* m;

  p.w = 0.0;  /* inialize to "no intersection" */
  //printf("point: (%f, %f, %f)\n", r->start->x,  r->start->y,  r->start->z);
  firstHit(r,&p,&n,&m);  

  if (p.w != 0.0 && r->start->x <= p.x && (r->start->x + r->dir->x) >= p.x
                 && r->start->y <= p.y && (r->start->y + r->dir->y) >= p.y
                 && r->start->z <= p.z && (r->start->z + r->dir->z) >= p.z) 
  {
    return true;
  } 
  else 
  {
    return false;
  }
}
