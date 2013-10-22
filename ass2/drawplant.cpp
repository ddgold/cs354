/*
 * drawplant.cpp
 * -------------
 * Contains the drawing routine and related helper functions for the
 * L-system fractal plant.  Starter code for Project 2.
 *
 * Group Members: <FILL IN>
 */

#ifdef _WIN32
#include <windows.h>
#endif
#include <cmath>
#include <assert.h>
#include <iostream>

#include "helper.h"
#include "common.h"
#include "drawplant.h"
#include "readppm.h"

using namespace std;


/* Takes a 2D matrix in row-major order, and loads the 3D matrix which
   does the same trasformation into the OpenGL MODELVIEW matrix, in
   column-major order. */
void load2DMatrix(
		GLfloat m00, GLfloat m01, GLfloat m02,
		GLfloat m10, GLfloat m11, GLfloat m12,
		GLfloat m20, GLfloat m21, GLfloat m22) {

	GLfloat M3D [16];  /* three dimensional matrix doing same transform */

	 M3D[0] = m00;  M3D[1] = m10;  M3D[2] = 0.0;  M3D[3] = m20;
	 M3D[4] = m01;  M3D[5] = m11;  M3D[6] = 0.0;  M3D[7] = m21;
	 M3D[8] = 0.0;  M3D[9] = 0.0; M3D[10] = 1.0; M3D[11] = 0.0;
	M3D[12] = m02; M3D[13] = m12; M3D[14] = 0.0; M3D[15] = m22;

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(M3D);
}

/* Takes a 3D matrix in row-major order, and loads the 3D matrix which
   does the same trasformation into the OpenGL MODELVIEW matrix, in
   column-major order. */
void load3DMatrix(
		GLfloat m00, GLfloat m01, GLfloat m02, GLfloat m03,
		GLfloat m10, GLfloat m11, GLfloat m12, GLfloat m13,
		GLfloat m20, GLfloat m21, GLfloat m22, GLfloat m23,
		GLfloat m30, GLfloat m31, GLfloat m32, GLfloat m33) {

	GLfloat M3D [16];  /* three dimensional matrix doing same transform */
	
	 M3D[0] = m00;  M3D[1] = m10;  M3D[2] = m20;  M3D[3] = m30;
	 M3D[4] = m01;  M3D[5] = m11;  M3D[6] = m21;  M3D[7] = m31;
	 M3D[8] = m02;  M3D[9] = m12; M3D[10] = m22; M3D[11] = m32;
	M3D[12] = m03; M3D[13] = m13; M3D[14] = m23; M3D[15] = m33;

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(M3D);
}

void load3DMatrix (const float* mView)
{
  load3DMatrix (mView[0], mView[1], mView[2], mView[3], 
                mView[4], mView[5], mView[6], mView[7], 
                mView[8], mView[9], mView[10], mView[11],
                mView[12], mView[13], mView[14], mView[15]);
}

void drawLeaf (int i, float s)
{
if (i == 0)
  {   
    //drawActualLeaf ();
          push();
          float results[MATRIX];
          multi ( MATRIX, _cameraMulti, _curMatrix, results);
          copy (MATRIX, results, _curMatrix);
          load3DMatrix (_curMatrix);
          if(_iterations !=1)
              drawActualFruit ();
          pop();
  }
  else
  {
    drawBranch (i - 1, s);
    push ();
    if( _randArray[_counter%100] < 0.75  )
      {
        _counter++;
       turnLeft (i -1, s);
       drawLeaf (i - 1, s);
      }
    else if(i < _iterations)
      {
        _counter++;
        drawLeaf (0, s);
      }
    
    //turnLeft (i -1, s);
    //drawLeaf (i - 1, s)
    pop ();
    
    push ();
     if( _randArray[_counter%100] <0.75 )
      {
        _counter++;
        turnRight (s);
        drawLeaf (i - 1, s);
      }
    else if(i < _iterations)
      {
        _counter++;
       drawLeaf (0, s);
      }
    // turnRight (s);
    //drawLeaf (i - 1, s);
    pop ();
    
  }

}


void drawActualLeaf (void)
{
  
  if(_season != WINTER)
  {
     if(_season == SPRING)
     {
      glColor3f(0.5, 0.1+ _randArray[_counter%100] ,0.5); 
     }
     else if(_season == SUMMER)
     {
      glColor3f(0.1, _randArray[_counter%100] ,0.1); 
     }
     else
     {
      glColor3f(_randArray[_counter%100] ,0.1, 0.1); 
     }
    //cout << "leaf\n" << print(MATRIX, _curMatrix) << endl;
    load3DMatrix (_curMatrix);
    //glColor3f(0.1, _randArray[_counter%100] ,0.1); 
    

    GLfloat vertices[] = {
         0.0, 0.0, 0.0,
         1.0, 0.7, 0.0,
         1.3, 1.8, 0.0,
         1.0, 2.8, 0.0,
         0.0, 4.0, 0.0,
        -1.0, 2.8, 0.0,
        -1.3, 1.8, 0.0,
        -1.0, 0.7, 0.0,

         0.0, 0.0, 0.3,
         1.0, 0.7, 0.3,
         1.3, 1.8, 0.3,
         1.0, 2.8, 0.3,
         0.0, 4.0, 0.3,
        -1.0, 2.8, 0.3,
        -1.3, 1.8, 0.3,
        -1.0, 0.7, 0.3};

    GLfloat indices[] = {
        0, 1, 2 , 3 , 4 , 5 , 6 , 7 ,
        8, 9, 10, 11, 12, 13, 14, 15,
        0, 1, 9 , 8 ,
        1, 2, 10, 9 , 
        2, 3, 11, 10,
        3, 4, 12, 11,
        4, 5, 13, 12,
        5, 6, 14, 13,
        6, 7, 15, 14,
        7, 0, 8 , 15};
    

  int i, index1, index2, index3, index4, index5, index6, index7, index8;

    for ( i = 0; i < 16; i+=8)
    { 
      index1 = indices[i] * 3;
      index2 = indices[i+1] * 3;
      index3 = indices[i+2] * 3;
      index4 = indices[i+3] * 3;
      index5 = indices[i+4] * 3;
      index6 = indices[i+5] * 3;
      index7 = indices[i+6] * 3;
      index8 = indices[i+7] * 3;
       glBegin(GL_POLYGON);
       glVertex3fv( &(vertices[index1]) );
      glVertex3fv( &(vertices[index2]) );
      glVertex3fv( &(vertices[index3]) );
      glVertex3fv( &(vertices[index4]) );
      glVertex3fv( &(vertices[index5]) );
      glVertex3fv( &(vertices[index6]) );
      glVertex3fv( &(vertices[index7]) );
      glVertex3fv( &(vertices[index8]) );
       glEnd();
    }
    
    for (i = 16; i < 48; i+=4)
    { 
      index1 = indices[i] * 3;
      index2 = indices[i+1] * 3;
      index3 = indices[i+2] * 3;
      index4 = indices[i+3] * 3;

      glBegin(GL_QUADS);

      glVertex3fv( &(vertices[index1]) );
      glVertex3fv( &(vertices[index2]) );
      glVertex3fv( &(vertices[index3]) );
      glVertex3fv( &(vertices[index4]) );

      glEnd();
    }
  }
  _counter++;
}

void drawBranch (int i, float s)
{
  if (i == 0)
  {
  
          push();
          float results[MATRIX];
          multi ( MATRIX, _cameraMulti, _curMatrix, results);
          copy (MATRIX, results, _curMatrix);
          load3DMatrix (_curMatrix);
    drawActualBranch (s);
          pop();
    push();
          float results2[MATRIX];
          if(_iterations > 1)
          {
            float results2[MATRIX];
            int alt = -1;
            multi ( MATRIX, _cameraMulti, _curMatrix, results2);
            copy (MATRIX, results2, _curMatrix);
            for(int q = 0; q < 5; q++)
            {
               float results3[MATRIX];
               alt *= -1;
               push();
               translate (_curMatrix, 0, s * 1.1 * _randArray[_counter%100], 0);
               _counter++; 
               rotate (_curMatrix, LOCATIONY, alt * 180.0f * _randArray[_counter%100]);
               _counter++;
               rotate (_curMatrix, LOCATIONZ, alt * 30.0f * (_randArray[_counter%100] - 0.4));
               _counter++;
              
               multi ( MATRIX, _cameraMulti, _curMatrix, results3);
               copy (MATRIX, results3, _curMatrix);
               
               drawActualLeaf ();
               pop();
                            
            }
          }
          else
          {
              multi ( MATRIX, _cameraMulti, _curMatrix, results2);
              copy (MATRIX, results2, _curMatrix);
              translate (_curMatrix, 0, s, 0);
              drawActualLeaf ();
          }
          
    pop ();
  }
  else
  {
    drawBranch (i - 1, s * 1.25);
  }
}

void drawActualBranch (float s)
{
	//cout << "branch\n" << print(MATRIX, _curMatrix) << endl;
  glColor3f(_randArray[_counter%100] , _randArray[(_counter+1)%100]/2 ,0.07); 
  _counter+=2;
  //glColor3f(0.54,0.27,0.07); 
	
	GLfloat vertices[] = {0.02 * s * sin(0*M_PI/180)  , 0.0, 0.02 * s * cos(0*M_PI/180),
                        0.02 * s * sin(45*M_PI/180) , 0.0, 0.02 * s * cos(45*M_PI/180),
                        0.02 * s * sin(90*M_PI/180) , 0.0, 0.02 * s * cos(90*M_PI/180),
                        0.02 * s * sin(135*M_PI/180), 0.0, 0.02 * s * cos(135*M_PI/180),
                        0.02 * s * sin(180*M_PI/180), 0.0, 0.02 * s * cos(180*M_PI/180),
                        0.02 * s * sin(225*M_PI/180), 0.0, 0.02 * s * cos(215*M_PI/180),
                        0.02 * s * sin(270*M_PI/180), 0.0, 0.02 * s * cos(270*M_PI/180),
                        0.02 * s * sin(315*M_PI/180), 0.0, 0.02 * s * cos(315*M_PI/180),

                        0.02 * s * sin(0*M_PI/180)  , s  , 0.02 * s * cos(0*M_PI/180),
                        0.02 * s * sin(45*M_PI/180) , s  , 0.02 * s * cos(45*M_PI/180),
                        0.02 * s * sin(90*M_PI/180) , s  , 0.02 * s * cos(90*M_PI/180),
                        0.02 * s * sin(135*M_PI/180), s  , 0.02 * s * cos(135*M_PI/180),
                        0.02 * s * sin(180*M_PI/180), s  , 0.02 * s * cos(180*M_PI/180),
                        0.02 * s * sin(225*M_PI/180), s  , 0.02 * s * cos(215*M_PI/180),
                        0.02 * s * sin(270*M_PI/180), s  , 0.02 * s * cos(270*M_PI/180),
                        0.02 * s * sin(315*M_PI/180), s  , 0.02 * s * cos(315*M_PI/180)};

 GLfloat indices[] = {
      0, 1, 2 , 3 , 4 , 5 , 6 , 7 ,
      8, 9, 10, 11, 12, 13, 14, 15,
      0, 1, 9 , 8 ,
      1, 2, 10, 9 , 
      2, 3, 11, 10,
      3, 4, 12, 11,
      4, 5, 13, 12,
      5, 6, 14, 13,
      6, 7, 15, 14,
      7, 0, 8 , 15};
  
int i, index1, index2, index3, index4, index5, index6, index7, index8;

  for ( i = 0; i < 16; i+=8)
  { 
    index1 = indices[i] * 3;
    index2 = indices[i+1] * 3;
    index3 = indices[i+2] * 3;
    index4 = indices[i+3] * 3;
    index5 = indices[i+4] * 3;
    index6 = indices[i+5] * 3;
    index7 = indices[i+6] * 3;
    index8 = indices[i+7] * 3;
     glBegin(GL_POLYGON);
     glVertex3fv( &(vertices[index1]) );
    glVertex3fv( &(vertices[index2]) );
    glVertex3fv( &(vertices[index3]) );
    glVertex3fv( &(vertices[index4]) );
    glVertex3fv( &(vertices[index5]) );
    glVertex3fv( &(vertices[index6]) );
    glVertex3fv( &(vertices[index7]) );
    glVertex3fv( &(vertices[index8]) );
     glEnd();
  }
  
  for (i = 16; i < 48; i+=4)
  { 
    index1 = indices[i] * 3;
    index2 = indices[i+1] * 3;
    index3 = indices[i+2] * 3;
    index4 = indices[i+3] * 3;

    glBegin(GL_QUADS);

    glVertex3fv( &(vertices[index1]) );
    glVertex3fv( &(vertices[index2]) );
    glVertex3fv( &(vertices[index3]) );
    glVertex3fv( &(vertices[index4]) );

    glEnd();
  }
}

void drawActualFruit (void)
{
  if(_season == SUMMER)
  {
    glColor3f(_randArray[_counter%100] + 0.05, 0.0f, 0.0f);
    glutSolidSphere(1.0, 30, 30); 
  }
  else if(_season == SPRING)
  {
    glColor3f(_randArray[_counter%100] - 0.1, 1.0, 0.0f);
    glutSolidSphere(0.6, 30, 30); 
  }
  _counter++;
}

void turnLeft (int i, float s)
{
  rotate (_curMatrix, LOCATIONZ, 35.0f * _randArray[_counter%100]);
  load3DMatrix (_curMatrix);
  _counter++;
  rotate (_curMatrix, LOCATIONY, 90.0f * _randArray[_counter%100]);
  _counter++;
  load3DMatrix (_curMatrix);
  translate (_curMatrix, 0, s / 3, 0);  /* * pow(1.25f, i)*/
  load3DMatrix (_curMatrix);
}

void turnRight (float s)
{
  rotate (_curMatrix, LOCATIONZ, -35.0f  * _randArray[_counter%100]);
  _counter++;
  load3DMatrix (_curMatrix);
   rotate (_curMatrix, LOCATIONY, 90.0f * _randArray[_counter%100]);
  _counter++;
  load3DMatrix (_curMatrix);
  translate (_curMatrix, 0, s * 2 / 3, 0);
  load3DMatrix (_curMatrix);
}

void push ()
{
  float* temp = new float[16];
 //cout << "push\n" << print(MATRIX, _curMatrix) << endl;
  copy (MATRIX, _curMatrix, temp);
  _matrixStack.push (temp);
}

void pop ()
{
   
  float* temp = _matrixStack.top();
  copy (MATRIX, temp, _curMatrix);
  load3DMatrix (_curMatrix);
  //cout << "pop\n" << print(MATRIX, _curMatrix) << endl;
  _matrixStack.pop ();
  
  delete temp;
}


void rotateCamera(double deg, int axis)
{

  if (axis == X_AXIS) {
		rotate (_cameraMulti, ORIGINX, deg);
	} else if (axis == Y_AXIS) {
		rotate (_cameraMulti, ORIGINY, deg);
	} else if (axis == Z_AXIS) {
		rotate (_cameraMulti, ORIGINZ, deg);
	}
}

void setSeason()
{
  _season = (Season)((_season+1)%4);
}

void fillRandom()
{
  srand (time(NULL));
  for(int i = 0; i < 100; i++)
  {
    _randArray[i] = (float)(rand() % 10)/30 + .5;
  }
  //assert(0);
}

void drawPlant (int i, float s)
{
  _counter = 0;
  _iterations = i;
  push ();
  drawLeaf (i, s);
  pop ();
}

/* end of drawplant.c */
