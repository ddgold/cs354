/*
 * subdiv.cpp
 * ----------
 * Contains function callbacks and bookkeeping for window management.
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

#include "common.h"
#include "drawing.h"
#include "data.h"

/* GLOBAL VARAIBLES */
/* (storage is actually allocated here) */
int W=400;		/* window width */
int H=400;		/* window height */
int X_OFF = 10;	/* window x offset */
int Y_OFF = 10;	/* window y offset */


/* The dimensions of the viewing frustum */
GLfloat fleft   = -40.0;
GLfloat fright  =  40.0;
GLfloat fbottom = -40.0;
GLfloat ftop    =  40.0;
GLfloat zNear   =  40.0;
GLfloat zFar    = -40.0;

/* local function declarations */
void init(void);
void display(void);
void myKeyHandler(unsigned char ch, int x, int y);
void myMouseButton(int button, int state, int x, int y);
void endSubdiv(int status);


/*From mouse.h*///---------------------------------------
#define MOUSE_ROTATE_YX		0
#define MOUSE_ROTATE_YZ		1
#define MOUSE_ZOOM			2

/* The frustum and zoom factor variables from canvas.c */
GLfloat zoomFactor = 1.0;

/* The current mode the mouse is in, based on what button(s) is pressed */
int mouse_mode;

/* The last position of the mouse since the last callback */
int m_last_x, m_last_y;

/* Function declarations */
void myMouseButton(int button, int state, int x, int y);
void myMouseMotion(int x, int y);

//----------------------------------------------------------


int main (int argc, char** argv) {
// Set display modes
  is3D = false;
  isWire = false;
  isPoints = false;
  isPhong = false;

  glutInit(&argc,argv);
  glutInitWindowSize(W, H);
  glutInitWindowPosition(X_OFF, Y_OFF);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("subdiv");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(myKeyHandler);
  glutMouseFunc(myMouseButton);
  glutMotionFunc(myMouseMotion);
  glutMainLoop();
  return 0;
}

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);  
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(fleft, fright, fbottom, ftop, -zNear, -zFar);
	
	glShadeModel (GL_SMOOTH);

	glEnable(GL_NORMALIZE);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
}

void display()
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	
	if (is3D)
	{
		glEnable(GL_LIGHTING);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		glEnable(GL_LIGHT0);

		if (isPhong)
		{
			drawSurfacePhong();
		}
		else
		{
			drawSurfaceGouraud();
		}
	}
	else
	{
		glDisable(GL_LIGHTING);
		drawSurface();
	}

    glFlush();  /* Flush all executed OpenGL ops finish */

    /*
     * Since we are using double buffers, we need to call the swap
     * function every time we are done drawing.
     */
    glutSwapBuffers();
}

void myKeyHandler(unsigned char ch, int x, int y) {
	switch(ch) {
		case 'q':
			endSubdiv(0);
			break;

		case 'w':
			if (originalCP < 5)
			{
				printf("Too Few Control Points, Min = 5\n");
				break;
			}

			if (!is3D)
			{
				generateRotationPoints(3);
				is3D = true;
				display();
			}
			break;

		case 'z':
			if (is3D)
			{
				is3D = false;

				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(fleft, fright, fbottom, ftop, -zNear, -zFar);

				display();	
			}
			break;

		case 'e':
			if (is3D)
			{
				isWire = !isWire;
				display();
			}
			break;

		case 'r':
			isPoints = !isPoints;
			display();
			break;
		case 'a':
			if(is3D && verticalSubLevel < 6)
			{
				verticalSubdivide();
				display();
			}
			break;

		case 'b':
			if(is3D && radialSubLevel < 6)
			{
				radialSubdivide();
				display();
			}
			break;

		case 'd':
			if (is3D)
			{
				isPhong = !isPhong;
				display();
			}
			break;

		case 's':
			if (is3D)
			{
				shininess[0] = ((int)shininess[0] + 10) % 128;
				//printf("shininess = %f\n", shininess[0]);
				display();
			}
			break;

		case 'i':
			if (is3D && lightPosition[1] > -10.0f)
			{
				lightPosition[1] -= 0.5f;
				//printf("lightPosition = (%g, %g, %g)\n", lightPosition[0], lightPosition[1], lightPosition[2]);
				display();
			}
			break;

		case 'k':
			if (is3D && lightPosition[1] < 10.0f)
			{
				lightPosition[1] += 0.5f;
				//printf("lightPosition = (%g, %g, %g)\n", lightPosition[0], lightPosition[1], lightPosition[2]);
				display();
			}
			break;

		case 'j':
			if (is3D && lightPosition[0] > -10.0f)
			{
				lightPosition[0] -= 0.5f;
				//printf("lightPosition = (%g, %g, %g)\n", lightPosition[0], lightPosition[1], lightPosition[2]);
				display();
			}
			break;

		case 'l':
			if (is3D && lightPosition[0] < 10.0f)
			{
				lightPosition[0] += 0.5f;
				//printf("lightPosition = (%g, %g, %g)\n", lightPosition[0], lightPosition[1], lightPosition[2]);
				display();
			}
			break;

		default:
			/* Unrecognized keypress */
		return;
	}

	glutPostRedisplay();
	return;
}

void myMouseButton(int button, int state, int x, int y) {
	if(!is3D)
	{
		if (state == GLUT_DOWN) {
			if (button == GLUT_LEFT_BUTTON) 
			{
				// Add a point, if there is room
				if(originalCP < 30)
				{
					printf("x: %3d, y: %3d Control Point: %d\n", x, y, originalCP);	
					add2dControlPoint(x,y);
					display();
				}
				else
				{
					printf("Exceeded Maximum Control Points of %d\n", MAX_POINT);
				}
			}
			if (button == GLUT_RIGHT_BUTTON)
			{
				if(originalCP > 0)
				{
					remove2dControlPoint();
					printf("Deleted Control Point: %d\n" , originalCP);
					display();
				}
				else
				{
					printf("No Control Points left to delete.\n");
				}
			}
		}
	}
	else // in 3D
	{
		if (state == GLUT_DOWN) 
		{
			m_last_x = x;
			m_last_y = y;

			if (button == GLUT_LEFT_BUTTON)
			{
				mouse_mode = MOUSE_ROTATE_YX;
			}
			else if (button == GLUT_MIDDLE_BUTTON)
			{
				mouse_mode = MOUSE_ROTATE_YZ;
			}
			else if (button == GLUT_RIGHT_BUTTON)
			{
				mouse_mode = MOUSE_ZOOM;
			}
		}
	}
}

void myMouseMotion(int x, int y) {
	if(is3D)
	{
		double d_x, d_y;	/* The change in x and y since the last callback */

		d_x = x - m_last_x;
		d_y = y - m_last_y;

		m_last_x = x;
		m_last_y = y;

		if (mouse_mode == MOUSE_ROTATE_YX) 
		{
			/* scaling factors */
			d_x /= 2.0;
			d_y /= 2.0;

			glRotatef(d_x, 0.0, 1.0, 0.0);	/* y-axis rotation */
			glRotatef(-d_y, 1.0, 0.0, 0.0);	/* x-axis rotation */

		} 
		else if (mouse_mode == MOUSE_ROTATE_YZ) 
		{
			/* scaling factors */
			d_x /= 2.0;
			d_y /= 2.0;

			glRotatef(d_x, 0.0, 1.0, 0.0);	/* y-axis rotation */
			glRotatef(-d_y, 0.0, 0.0, 1.0);	/* z-axis rotation */

		} 
		else if (mouse_mode == MOUSE_ZOOM) 
		{
			d_y /= 100.0;

			zoomFactor += d_y;

			if (zoomFactor <= 0.0) 
			{
				/* The zoom factor should be positive */
				zoomFactor = 0.001;
			}

			printf("zoomFactor: %f\n", zoomFactor);
			
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			/*
			 * glFrustum must receive positive values for the near and far
			 * clip planes ( arguments 5 and 6 ).
			 */
			glOrtho(fleft*zoomFactor, fright*zoomFactor,
				fbottom*zoomFactor, ftop*zoomFactor,
				-zNear, -zFar);
			/*
			printf("glFrustum( %f, %f, %f, %f, %f, %f )\n", fleft*zoomFactor, fright*zoomFactor,
				fbottom*zoomFactor, ftop*zoomFactor,
				-zNear, -zFar);
			*/
		}

		/* Redraw the screen */
		glutPostRedisplay();
	}
}

void endSubdiv(int status) {
  printf("\nQuitting subdivision program.\n\n");
  fflush(stdout);

  exit(status);
}


/* end of subdiv.cpp */
