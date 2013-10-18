/*
 * plant.cpp
 * ---------
 * Contains function callbacks and bookkeeping used by glut.  Starter code
 * for Project 2.
 *
 * Group Members: <FILL IN>
 */


#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "helper.h"
#include "common.h"
#include "drawplant.h"


/* GLOBAL VARAIBLES */
/* (storage is actually allocated here) */
int W=800;		/* window width */
int H=600;		/* window height */
int X_OFF = 10;	/* window x offset */
int Y_OFF = 10;	/* window y offset */

/* local function declarations */
void display(void);
void init(void);
int endCanvas(int);


void myKeyHandler(unsigned char ch, int x, int y)
{
  switch (ch)
  {
    case '/':
      rotateCamera(5, Y_AXIS);
      break;
      
    case '?':
      rotateCamera(-5, Y_AXIS);
      break;
      
    case 'q':
      endCanvas(0);
      break;
      
    case 'a':
      if (_iterations < 10)
      {
        ++_iterations;
      }
      break;
      
    case 's':
      if (_iterations > 0)
      {
        --_iterations;
      }
      break;
      
    case 'p':
    
      break;
    
    default:
      return;
      break;
  }
  
  display ();
}

int endCanvas(int status) {
  printf("\nQuitting canvas.\n\n");
  fflush(stdout);

  exit(status);
}





int main (int argc, char** argv) {
  fillRandom();
  glutInit(&argc,argv);
  glutInitWindowSize(W, H);
  glutInitWindowPosition(X_OFF, Y_OFF);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("plant");
  init();
  //glFrustum(-100.0, 100.0, -50.0,50.0,-50.0,50.0  );
  glutDisplayFunc(display);
	glutKeyboardFunc(myKeyHandler);
  glutMainLoop();
  return 0;
}

void init() {
  glClearColor(0.0, 0.0, 0.0, 0.0);  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-40.0, 40.0, -10.0, 70.0, -50.0, 50.0);
}

void display() {
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   cout << "display\n" << print(MATRIX, _curMatrix) << endl;
	/* See drawplant.c for the definition of this routine */
	drawPlant(_iterations, 10.0f);

    glFlush();  /* Flush all executed OpenGL ops finish */

    /*
     * Since we are using double buffers, we need to call the swap
     * function every time we are done drawing.
     */
    glutSwapBuffers();
}

/* end of plant.c */
