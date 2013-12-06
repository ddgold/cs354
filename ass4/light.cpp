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
                       GLfloat amb, GLfloat dif, GLfloat spec, GLfloat expo) {
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

  GLfloat lambert = 0.0;
  GLfloat phong = 0.0;

  // Ambient Portion
  c->r =  m->amb * m->r;
  c->g =  m->amb * m->g;
  c->b =  m->amb * m->b;

  for(unsigned int i = 0; i < sizeof(lights)/sizeof(light*); ++i)
  {
    calculateDirection(p, lights[i]->p, s);

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


  
  
  /* clamp color values to 1.0 */
  if (c->r > 1.0) c->r = 1.0;
  if (c->g > 1.0) c->g = 1.0;
  if (c->b > 1.0) c->b = 1.0;

}

bool shadow(vector *v)
{
  return false;
}
