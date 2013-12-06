/*
 * drawing.cpp
 * -----------
 * Contains the drawing routines and related helper functions for the
 * subdivision surface
 *
 * Group Members: <FILL IN>
 */

#ifdef _WIN32
#include <windows.h>
#endif
#include <math.h>
#include <assert.h>

#include <stdio.h>
#include "common.h"
#include "drawing.h"
#include "data.h"

/* Globals */
int subdiv_v; // The user-specified subdivision level, vertical
int subdiv_h; // The user-specified subdivision level, horizontal

/* The parameter list may need to be changed for the functions in this file */

void drawSurface(void) {
	int i;
	//Vertical line
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0f,1.0f,1.0f);
	glVertex3f(0 , 35.0f , 0.0f);
	glVertex3f(0 , -35.0f , 0.0f);
	glEnd();
	glPointSize(4.0f);
	glBegin(GL_POINTS);
	glVertex3f(0 , 35.0f , 0.0f);
	glVertex3f(0 , -35.0f , 0.0f);
	glEnd();


	//line curve
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0f,0.0f,1.0f);
	for(i = 0; i < originalCP; ++i)
	{
		glVertex3f(originalCP_x[i] , originalCP_y[i] , 0.0f);
	}
	glEnd();


	//points
	glPointSize(4.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f,0.0f,0.0f);
	for(i = 0; i < originalCP; ++i)
	{
		glVertex3f(originalCP_x[i] , originalCP_y[i], 0.0f);
	}
	glEnd();

}

void drawSurfaceGouraud()
{	
  int i = 0;
  int j = 0;
  for(i = 0; i < gouraudRadialCP; ++i)
  {
	  for(j = 0; j < gouraudVerticalCP - 1; ++j)
	  {
      if(isPoints)
      {
        glPointSize(4.0f);
        glBegin(GL_POINTS);
      }
      else
      {
        if (isWire)
     		  glBegin(GL_LINE_STRIP);
        else
      		glBegin(GL_QUADS);
      }
      
      glColor3f(205/255.0f,127/255.0f,50/255.0f);
      
      glNormal3f(gouraudNorm_x[i * gouraudVerticalCP + j], 
                 gouraudNorm_y[i * gouraudVerticalCP + j], 
                 gouraudNorm_z[i * gouraudVerticalCP + j]);
      glVertex3f(gouraudCP_x[i * gouraudVerticalCP + j], 
                 gouraudCP_y[i * gouraudVerticalCP + j], 
                 gouraudCP_z[i * gouraudVerticalCP + j]);
      
      
      glNormal3f(gouraudNorm_x[i * gouraudVerticalCP + j + 1], 
                 gouraudNorm_y[i * gouraudVerticalCP + j + 1], 
                 gouraudNorm_z[i * gouraudVerticalCP + j + 1]);
      glVertex3f(gouraudCP_x[i * gouraudVerticalCP + j + 1], 
                 gouraudCP_y[i * gouraudVerticalCP + j + 1], 
                 gouraudCP_z[i * gouraudVerticalCP + j + 1]);
      
      
      if(i != gouraudRadialCP - 1)
      {
        glNormal3f(gouraudNorm_x[(i + 1) * gouraudVerticalCP + j + 1], 
                   gouraudNorm_y[(i + 1) * gouraudVerticalCP + j + 1], 
                   gouraudNorm_z[(i + 1) * gouraudVerticalCP + j + 1]);
        glVertex3f(gouraudCP_x[(i + 1) * gouraudVerticalCP + j + 1], 
                   gouraudCP_y[(i + 1) * gouraudVerticalCP + j + 1], 
                   gouraudCP_z[(i + 1) * gouraudVerticalCP + j + 1]);
        

        glNormal3f(gouraudNorm_x[(i +1) * gouraudVerticalCP + j], 
                   gouraudNorm_y[(i +1) * gouraudVerticalCP + j], 
                   gouraudNorm_z[(i +1) * gouraudVerticalCP + j]); 
        glVertex3f(gouraudCP_x[(i +1) * gouraudVerticalCP + j], 
                   gouraudCP_y[(i +1) * gouraudVerticalCP + j], 
                   gouraudCP_z[(i +1) * gouraudVerticalCP + j]);
      }
      else
      {
        glNormal3f(gouraudNorm_x[j + 1], 
                   gouraudNorm_y[j + 1], 
                   gouraudNorm_z[j + 1]);
        glVertex3f(gouraudCP_x[j + 1], 
                   gouraudCP_y[j + 1], 
                   gouraudCP_z[j + 1]);
        

        glNormal3f(gouraudNorm_x[j], 
                   gouraudNorm_y[j], 
                   gouraudNorm_z[j]);
        glVertex3f(gouraudCP_x[j], 
                   gouraudCP_y[j], 
                   gouraudCP_z[j]);   
      }
      glEnd();
	  }
  }
}

void drawSurfacePhong()
{ 
 

  int i = 0;
  int j = 0;
  for(i = 0; i < phongRadialCP; ++i)
  {
    for(j = 0; j < phongVerticalCP - 1; ++j)
    {
      if(isPoints)
      {
        glPointSize(4.0f);
        glBegin(GL_POINTS);
      }
      else
      {
        if (isWire)
          glBegin(GL_LINE_STRIP);
        else
          glBegin(GL_TRIANGLES);
      }
      
      glColor3f(205/255.0f,127/255.0f,50/255.0f);
      //glColor3f(1.0f, 0.0f, 1.0f);
      
      
      glNormal3f(phongNorm_x[i * phongVerticalCP + j], 
                 phongNorm_y[i * phongVerticalCP + j], 
                 phongNorm_z[i * phongVerticalCP + j]);
      glVertex3f(phongCP_x[i * phongVerticalCP + j], 
                 phongCP_y[i * phongVerticalCP + j], 
                 phongCP_z[i * phongVerticalCP + j]);
      
      
      glNormal3f(phongNorm_x[i * phongVerticalCP + j + 1], 
                 phongNorm_y[i * phongVerticalCP + j + 1], 
                 phongNorm_z[i * phongVerticalCP + j + 1]);
      glVertex3f(phongCP_x[i * phongVerticalCP + j + 1], 
                 phongCP_y[i * phongVerticalCP + j + 1], 
                 phongCP_z[i * phongVerticalCP + j + 1]);
      
      if(i != phongRadialCP - 1)
      {
        
        glNormal3f(phongNorm_x[(i +1) * phongVerticalCP + j], 
                   phongNorm_y[(i +1) * phongVerticalCP + j], 
                   phongNorm_z[(i +1) * phongVerticalCP + j]);
        glVertex3f(phongCP_x[(i +1) * phongVerticalCP + j], 
                   phongCP_y[(i +1) * phongVerticalCP + j], 
                   phongCP_z[(i +1) * phongVerticalCP + j]); 
      }
      else
      {
        
        glNormal3f(phongNorm_x[j], 
                   phongNorm_y[j], 
                   phongNorm_z[j]);  
        glVertex3f(phongCP_x[j], 
                   phongCP_y[j], 
                   phongCP_z[j]); 
      }
      glEnd();

      glColor3f(205/255.0f,127/255.0f,50/255.0f);
      //glColor3f(0.0f, 1.0f, 0.0f);
      if(isPoints)
      {
        glPointSize(4.0f);
        glBegin(GL_POINTS);
      }
      else
      {
        if (isWire)
          glBegin(GL_LINE_STRIP);
        else
          glBegin(GL_TRIANGLES);
      }
    
      glNormal3f(phongNorm_x[i * phongVerticalCP + j + 1], 
                 phongNorm_y[i * phongVerticalCP + j + 1], 
                 phongNorm_z[i * phongVerticalCP + j + 1]);
      glVertex3f(phongCP_x[i * phongVerticalCP + j + 1], 
                 phongCP_y[i * phongVerticalCP + j + 1], 
                 phongCP_z[i * phongVerticalCP + j + 1]);
      
      
      if(i != phongRadialCP - 1)
      {
        
        glNormal3f(phongNorm_x[(i + 1) * phongVerticalCP + j + 1], 
                   phongNorm_y[(i + 1) * phongVerticalCP + j + 1], 
                   phongNorm_z[(i + 1) * phongVerticalCP + j + 1]);
        glVertex3f(phongCP_x[(i + 1) * phongVerticalCP + j + 1], 
                   phongCP_y[(i + 1) * phongVerticalCP + j + 1], 
                   phongCP_z[(i + 1) * phongVerticalCP + j + 1]);
        

        
        glNormal3f(phongNorm_x[(i +1) * phongVerticalCP + j], 
                   phongNorm_y[(i +1) * phongVerticalCP + j], 
                   phongNorm_z[(i +1) * phongVerticalCP + j]);
        glVertex3f(phongCP_x[(i +1) * phongVerticalCP + j], 
                   phongCP_y[(i +1) * phongVerticalCP + j], 
                   phongCP_z[(i +1) * phongVerticalCP + j]);
      }
      else
      {
        
        glNormal3f(phongNorm_x[j + 1], 
                   phongNorm_y[j + 1], 
                   phongNorm_z[j + 1]);
        glVertex3f(phongCP_x[j + 1], 
                   phongCP_y[j + 1], 
                   phongCP_z[j + 1]);
        

        
        glNormal3f(phongNorm_x[j], 
                   phongNorm_y[j], 
                   phongNorm_z[j]);
        glVertex3f(phongCP_x[j], 
                   phongCP_y[j], 
                   phongCP_z[j]);   
      }
      glEnd();
    }
  }
}