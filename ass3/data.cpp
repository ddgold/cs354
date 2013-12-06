/*
 * data.cpp
 * --------
 * Contains routines to help manage the data for the Midpoint Subdivision.
 * Starter code for Project 3.
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
#include "data.h"

GLfloat originalCP_x[MAX_POINT];
GLfloat originalCP_y[MAX_POINT];
int originalCP;


GLfloat *gouraudCP_x;
GLfloat *gouraudCP_y;
GLfloat *gouraudCP_z;

GLfloat *gouraudNorm_x;
GLfloat *gouraudNorm_y;
GLfloat *gouraudNorm_z;

int gouraudVerticalCP;
int gouraudRadialCP;


GLfloat *phongCP_x;
GLfloat *phongCP_y;
GLfloat *phongCP_z;

GLfloat *phongNorm_x;
GLfloat *phongNorm_y;
GLfloat *phongNorm_z;

int phongVerticalCP;
int phongRadialCP;


int verticalSubLevel;
int radialSubLevel;


GLfloat shininess[1] = {55.0f};
GLfloat specular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat lightPosition[4] = {1.0f, 1.0f, 1.0f, 0.0f};


bool is3D;
bool isWire;
bool isPoints;
bool isPhong;

/**********************************************
 * Begin Routines for Midpoint Subdivsion
 **********************************************/

/*
 * Copies a static array into dynamically allocated space and
 * returns the copy.  Useful for making a copy of the static
 * iteration 0 control points, since the iteration 0 control
 * points should be kept in their static array for user-interface
 * purposes.
 *
 * Note: this function needs to be called twice, once for x and
 * once for y.
 */
GLfloat *copyPointsArray(GLfloat *orig, int size) {
	assert(size >= 0);
	if (size > 0) {
		assert(orig != NULL);
	}

	GLfloat *copy;
	copy = (GLfloat *)malloc(size * sizeof(GLfloat));
	for (int i = 0; i < size; i++) {
		copy[i] = orig[i];
	}

	return copy;
}

/*
 * Allocates an 1-D GLfloat array for the given number of elements.
 */
GLfloat *makePointsArray(int size) {
	assert(size >= 0);

	return (GLfloat *)malloc(size * sizeof(GLfloat));
}

/*
 * Given 2 equally sized points arrays, arr1 and arr2 in this form:
 *
 * arr1:    a0, a1, a2, a3, ...
 * arr2:    b0, b1, b2, b3, ... 
 *
 * ...returns a newly allocated array arr3 which contains:
 * arr3:    a0, b0, a1, b1, a2, b2, a3, b3, ...
 *
 * Note:
 * This function frees up the storage for arr1 and arr2, so the caller
 * of this function should not reference these arrays again.
 *
 * The two arrays should be the same size, and the argument "size"
 * is the size of one of these two arrays.
 */

GLfloat *mergePointsArrays(GLfloat *arr1, GLfloat *arr2, int size) {
	assert(size >= 0);
	if (size > 0) {
		assert(arr1 != NULL);
		assert(arr2 != NULL);
	}

	GLfloat *arr3;

	arr3 = (GLfloat *)malloc(2 * size * sizeof(GLfloat));

	for (int i = 0; i < size; i++) {
		arr3[2 * i] = arr1[i];
		arr3[(2 * i) + 1] = arr2[i];
	}

	free(arr1);
	free(arr2);

	return arr3;
}

void subdividePointsArray(int subdiv_level) 
{
	assert(subdiv_level >= 0);

	/* ADD YOUR CODE HERE */

	return;
}

void radialSubdivide()
{
	++radialSubLevel;

	
	GLfloat *temp_x = copyPointsArray(gouraudCP_x, gouraudRadialCP * gouraudVerticalCP);
	GLfloat *temp_y = copyPointsArray(gouraudCP_y, gouraudRadialCP * gouraudVerticalCP);
	
	gouraudRadialCP *= 2;

	gouraudCP_x = makePointsArray( gouraudRadialCP * gouraudVerticalCP);
	gouraudCP_y = makePointsArray( gouraudRadialCP * gouraudVerticalCP);
	gouraudCP_z = makePointsArray( gouraudRadialCP * gouraudVerticalCP);

	

	int i = 0;
	int j = 0;
	float theta = 0;
	for(j = 0; j < gouraudRadialCP; ++j)
	{
		theta = (360.0f / gouraudRadialCP * j) * M_PI / 180.0f; 

		//printf("theta: %f\n",theta);

		for(i = 0; i < gouraudVerticalCP; ++i)
		{
			gouraudCP_x[j * gouraudVerticalCP + i] = temp_x[i]*cos(theta);
			gouraudCP_y[j * gouraudVerticalCP + i] = temp_y[i];
			gouraudCP_z[j * gouraudVerticalCP + i] = temp_x[i]*sin(theta);
		}
	}
	free(temp_x);
	free(temp_y);

	generateGouraud();
	generatePhong();
}

	void verticalSubdivide()
	{
		++verticalSubLevel;

		GLfloat *temp_x = copyPointsArray(gouraudCP_x, gouraudRadialCP * gouraudVerticalCP);
		GLfloat *temp_y = copyPointsArray(gouraudCP_y, gouraudRadialCP * gouraudVerticalCP);
		GLfloat *temp_z = copyPointsArray(gouraudCP_z, gouraudRadialCP * gouraudVerticalCP);
	
		GLfloat *steady_x = copyPointsArray(gouraudCP_x, gouraudRadialCP * gouraudVerticalCP);
		GLfloat *steady_y = copyPointsArray(gouraudCP_y, gouraudRadialCP * gouraudVerticalCP);
		GLfloat *steady_z = copyPointsArray(gouraudCP_z, gouraudRadialCP * gouraudVerticalCP);
	
		GLfloat *sub_x = makePointsArray( gouraudRadialCP * gouraudVerticalCP);
		GLfloat *sub_y = makePointsArray( gouraudRadialCP * gouraudVerticalCP);
		GLfloat *sub_z = makePointsArray( gouraudRadialCP * gouraudVerticalCP);


		int i = 0;
		int j = 0;
		//int theta = 0;
		int curSpot = 0;
		//repeat for all vertical slices
		for(j = 0; j < gouraudRadialCP; ++j)
		{
			//for each vertical point
			for(i = 0; i < gouraudVerticalCP - 1; ++i)
			{
				//make sub_xyz first
				curSpot = j * gouraudVerticalCP + i;
				sub_x[curSpot] = (steady_x[curSpot] + steady_x[curSpot+1])/2.0;
				sub_y[curSpot] = (steady_y[curSpot] + steady_y[curSpot+1])/2.0;
				sub_z[curSpot] = (steady_z[curSpot] + steady_z[curSpot+1])/2.0;

				if(i > 0)
				{
					temp_x[curSpot] = (steady_x[curSpot-1] + 6 * steady_x[curSpot] + steady_x[curSpot+1])/8.0;
					temp_y[curSpot] = (steady_y[curSpot-1] + 6 * steady_y[curSpot] + steady_y[curSpot+1])/8.0;
					temp_z[curSpot] = (steady_z[curSpot-1] + 6 * steady_z[curSpot] + steady_z[curSpot+1])/8.0;
				}
			}
		}

	gouraudCP_x = makePointsArray( gouraudRadialCP * (2 * gouraudVerticalCP - 1));
	gouraudCP_y = makePointsArray( gouraudRadialCP * (2 * gouraudVerticalCP - 1));
	gouraudCP_z = makePointsArray( gouraudRadialCP * (2 * gouraudVerticalCP - 1));

	int fillPos = 0;
	for(j = 0; j< gouraudRadialCP; ++j)
	{
		for(i = 0; i < gouraudVerticalCP; ++i)
		{
			curSpot = j * gouraudVerticalCP + i;
			gouraudCP_x[fillPos] = temp_x[curSpot];
			gouraudCP_y[fillPos] = temp_y[curSpot];
			gouraudCP_z[fillPos] = temp_z[curSpot];
			++fillPos;

			if(i != gouraudVerticalCP -1)
			{
				gouraudCP_x[fillPos] = sub_x[curSpot];
				gouraudCP_y[fillPos] = sub_y[curSpot];
				gouraudCP_z[fillPos] = sub_z[curSpot];
				++fillPos;
			}
		}
	}
		free(temp_x);
		free(temp_y);
		free(temp_z);

		free(steady_x);
		free(steady_y);
		free(steady_z);

		free(sub_x);
		free(sub_y);
		free(sub_z);

		gouraudVerticalCP = 2 * gouraudVerticalCP - 1;


		generateGouraud();
		generatePhong();
	}

void add2dControlPoint(int x, int y)
{
	if(x <= 205 && x >= 195)
		originalCP_x[originalCP] = 0;
	else
		originalCP_x[originalCP] = (x - 200) /  5.0;

	originalCP_y[originalCP] = (y - 200) / -5.0 ;
	++originalCP;
}

void remove2dControlPoint()
{
	--originalCP;
}

void generateRotationPoints(int n)
{
	radialSubLevel = 0;
	verticalSubLevel = 0;
	gouraudRadialCP = n;
	gouraudVerticalCP = originalCP + 2;

	gouraudCP_x = (GLfloat *)malloc(gouraudRadialCP * gouraudVerticalCP * sizeof(GLfloat));
	gouraudCP_y = (GLfloat *)malloc(gouraudRadialCP * gouraudVerticalCP * sizeof(GLfloat));
	gouraudCP_z = (GLfloat *)malloc(gouraudRadialCP * gouraudVerticalCP * sizeof(GLfloat));
	
	int i = 0;
	int j = 0;
	float theta = 0;
	
	for(j = 0; j < gouraudRadialCP; ++j)
	{
		theta = (360.0/gouraudRadialCP * j) * M_PI / 180.0f; 

		gouraudCP_x[j * gouraudVerticalCP] = 0;
		gouraudCP_y[j * gouraudVerticalCP] = originalCP_y[0];
		gouraudCP_z[j * gouraudVerticalCP] = 0;

		for(i = 0; i < originalCP; ++i)
		{
			gouraudCP_x[j * gouraudVerticalCP + i + 1] = originalCP_x[i] * cos(theta);
			gouraudCP_y[j * gouraudVerticalCP + i + 1] = originalCP_y[i];
			gouraudCP_z[j * gouraudVerticalCP + i + 1] = originalCP_x[i] * sin(theta);

		}

		gouraudCP_x[(j + 1) * gouraudVerticalCP - 1] = 0;
		gouraudCP_y[(j + 1) * gouraudVerticalCP - 1] = originalCP_y[originalCP - 1];
		gouraudCP_z[(j + 1) * gouraudVerticalCP - 1] = 0;


	}

	generateGouraud();
	generatePhong();
}

void generateGouraud(void)
{
	gouraudNorm_x = makePointsArray( gouraudRadialCP * gouraudVerticalCP);
	gouraudNorm_y = makePointsArray( gouraudRadialCP * gouraudVerticalCP);
	gouraudNorm_z = makePointsArray( gouraudRadialCP * gouraudVerticalCP);

	int i, j;
	int curSpot;
	int leftIndex, rightIndex;

	float right[3]      = {0, 0, 0};
	float top[3]        = {0, 0, 0};
	float left[3]       = {0, 0, 0};
	float bottom[3]     = {0, 0, 0};
	float normal[3]     = {0, 0, 0};
	float un;

	for(j = 0; j < gouraudRadialCP; ++j)
	{
		gouraudNorm_x[j * gouraudVerticalCP] = 0.0f;
		gouraudNorm_y[j * gouraudVerticalCP] = 1.0f;
		gouraudNorm_z[j * gouraudVerticalCP] = 0.0f;


		//for each vertical point
		for(i = 1; i < gouraudVerticalCP - 1; ++i)
		{
			// Splice
			curSpot = j * gouraudVerticalCP + i;
			leftIndex  = curSpot - gouraudVerticalCP;
			rightIndex = curSpot + gouraudVerticalCP;
			
			if (j == 0)
			{
				leftIndex = (gouraudRadialCP - 1) * gouraudVerticalCP + i;
			}
			
			if (j == gouraudRadialCP - 1)
			{
				rightIndex = i;
			}

			right[0] = gouraudCP_x[rightIndex] - gouraudCP_x[curSpot];
			right[1] = gouraudCP_y[rightIndex] - gouraudCP_y[curSpot];
			right[2] = gouraudCP_z[rightIndex] - gouraudCP_z[curSpot];

			top[0] = gouraudCP_x[curSpot - 1] - gouraudCP_x[curSpot];
			top[1] = gouraudCP_y[curSpot - 1] - gouraudCP_y[curSpot];
			top[2] = gouraudCP_z[curSpot - 1] - gouraudCP_z[curSpot];

			left[0] = gouraudCP_x[leftIndex] - gouraudCP_x[curSpot];
			left[1] = gouraudCP_y[leftIndex] - gouraudCP_y[curSpot];
			left[2] = gouraudCP_z[leftIndex] - gouraudCP_z[curSpot];

			bottom[0] = gouraudCP_x[curSpot + 1] - gouraudCP_x[curSpot];
			bottom[1] = gouraudCP_y[curSpot + 1] - gouraudCP_y[curSpot];
			bottom[2] = gouraudCP_z[curSpot + 1] - gouraudCP_z[curSpot];
			


			//normal
			normal[0] += (right[1] * top[2] - right[2] * top[1]);
			normal[1] += (right[2] * top[0] - right[0] * top[2]);
			normal[2] += (right[0] * top[1] - right[1] * top[0]);

			normal[0] += (top[1] * left[2] - top[2] * left[1]);
			normal[1] += (top[2] * left[0] - top[0] * left[2]);
			normal[2] += (top[0] * left[1] - top[1] * left[0]);

			normal[0] += (left[1] * bottom[2] - left[2] * bottom[1]);
			normal[1] += (left[2] * bottom[0] - left[0] * bottom[2]);
			normal[2] += (left[0] * bottom[1] - left[1] * bottom[0]);

			normal[0] += (bottom[1] * right[2] - bottom[2] * right[1]);
			normal[1] += (bottom[2] * right[0] - bottom[0] * right[2]);
			normal[2] += (bottom[0] * right[1] - bottom[1] * right[0]);
			

			//normalize
			un = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);

			normal[0] /= (un);
			normal[1] /= (un);
			normal[2] /= (un);
			
			gouraudNorm_x[curSpot] = normal[0];
			gouraudNorm_y[curSpot] = normal[1];
			gouraudNorm_z[curSpot] = normal[2];

			normal[0] = 0;
			normal[1] = 0;
			normal[2] = 0;
		}

		gouraudNorm_x[(j + 1) * gouraudVerticalCP - 1] =  0.0f;
		gouraudNorm_y[(j + 1) * gouraudVerticalCP - 1] = -1.0f;
		gouraudNorm_z[(j + 1) * gouraudVerticalCP - 1] =  0.0f;
	}
}

void generatePhong(void)
{
	phongVerticalCP = 2 * gouraudVerticalCP - 1;
	phongRadialCP = 2 * gouraudRadialCP;

	phongCP_x =  makePointsArray( phongRadialCP * phongVerticalCP);
	phongCP_y =  makePointsArray( phongRadialCP * phongVerticalCP);
	phongCP_z =  makePointsArray( phongRadialCP * phongVerticalCP);

	int i, j;
  int curSpot;
  int nextCol;

	//THIS FOR LOOP LOOK THOUGH GOURAUD
	//DO NO CHANGE gouraudRadialCP or vGCP
	//NOT GENERATING THE PINTS RIGHT, here lays our problems
	for(j = 0; j < gouraudRadialCP; ++j)
	{
		for(i = 0; i < gouraudVerticalCP ; ++i)
		{
			//Vertical points
			curSpot = j * gouraudVerticalCP + i; 
			phongCP_x[2 * j * phongVerticalCP + 2 * i ] = gouraudCP_x[curSpot]; 
			phongCP_y[2 * j * phongVerticalCP + 2 * i ] = gouraudCP_y[curSpot]; 
			phongCP_z[2 * j * phongVerticalCP + 2 * i ] = gouraudCP_z[curSpot]; 

			if(i != gouraudVerticalCP - 1)
			{
				phongCP_x[2 * j * phongVerticalCP + 2 * i  + 1] = (gouraudCP_x[curSpot] + gouraudCP_x[curSpot + 1])/2.0; 
				phongCP_y[2 * j * phongVerticalCP + 2 * i  + 1] = (gouraudCP_y[curSpot] + gouraudCP_y[curSpot + 1])/2.0; 
				phongCP_z[2 * j * phongVerticalCP + 2 * i  + 1] = (gouraudCP_z[curSpot] + gouraudCP_z[curSpot + 1])/2.0; 
			}

			//horrizontal
			if(j != gouraudRadialCP - 1)
				nextCol = (j + 1) * gouraudVerticalCP + i;
			else
				nextCol = i;

			phongCP_x[(2 * j + 1) * phongVerticalCP + i * 2] = (gouraudCP_x[curSpot] + gouraudCP_x[nextCol])/2.0;
			phongCP_y[(2 * j + 1) * phongVerticalCP + i * 2] = (gouraudCP_y[curSpot] + gouraudCP_y[nextCol])/2.0;
			phongCP_z[(2 * j + 1) * phongVerticalCP + i * 2] = (gouraudCP_z[curSpot] + gouraudCP_z[nextCol])/2.0;

			if(i !=gouraudVerticalCP - 1)
			{
				phongCP_x[(2 * j + 1) * phongVerticalCP + i * 2 + 1] = (gouraudCP_x[curSpot] + gouraudCP_x[nextCol + 1])/2.0;
				phongCP_y[(2 * j + 1) * phongVerticalCP + i * 2 + 1] = (gouraudCP_y[curSpot] + gouraudCP_y[nextCol + 1])/2.0;
				phongCP_z[(2 * j + 1) * phongVerticalCP + i * 2 + 1] = (gouraudCP_z[curSpot] + gouraudCP_z[nextCol + 1])/2.0;

			}
		}
	}

	phongNorm_x = makePointsArray( phongRadialCP * phongVerticalCP);
	phongNorm_y = makePointsArray( phongRadialCP * phongVerticalCP);
	phongNorm_z = makePointsArray( phongRadialCP * phongVerticalCP);

	
	
	int leftIndex, rightIndex;

	float right[3]      = {0, 0, 0};
	float topRight[3]   = {0, 0, 0};
	float top[3]        = {0, 0, 0};
	float left[3]       = {0, 0, 0};
	float bottomLeft[3] = {0, 0, 0};
	float bottom[3]     = {0, 0, 0};
	float normal[3]     = {0, 0, 0};
	float un;

	for(j = 0; j < phongRadialCP; ++j)
	{
		phongNorm_x[j * phongVerticalCP] = 0.0f;
		phongNorm_y[j * phongVerticalCP] = 1.0f;
		phongNorm_z[j * phongVerticalCP] = 0.0f;

		//for each vertical point
		for(i = 1; i < phongVerticalCP - 1; ++i)
		{
			// Splice
			curSpot = j * phongVerticalCP + i;
			leftIndex  = curSpot - phongVerticalCP;
			rightIndex = curSpot + phongVerticalCP;
			
			if (j == 0)
			{
				leftIndex = (phongRadialCP - 1) * phongVerticalCP + i;
			}
			
			if (j == phongRadialCP - 1)
			{
				rightIndex = i;
			}

			right[0] = phongCP_x[rightIndex] - phongCP_x[curSpot];
			right[1] = phongCP_y[rightIndex] - phongCP_y[curSpot];
			right[2] = phongCP_z[rightIndex] - phongCP_z[curSpot];

			topRight[0] = phongCP_x[rightIndex - 1] - phongCP_x[curSpot];
			topRight[1] = phongCP_y[rightIndex - 1] - phongCP_y[curSpot];
			topRight[2] = phongCP_z[rightIndex - 1] - phongCP_z[curSpot];

			top[0] = phongCP_x[curSpot - 1] - phongCP_x[curSpot];
			top[1] = phongCP_y[curSpot - 1] - phongCP_y[curSpot];
			top[2] = phongCP_z[curSpot - 1] - phongCP_z[curSpot];

			left[0] = phongCP_x[leftIndex] - phongCP_x[curSpot];
			left[1] = phongCP_y[leftIndex] - phongCP_y[curSpot];
			left[2] = phongCP_z[leftIndex] - phongCP_z[curSpot];

			bottomLeft[0] = phongCP_x[leftIndex + 1] - phongCP_x[curSpot];
			bottomLeft[1] = phongCP_y[leftIndex + 1] - phongCP_y[curSpot];
			bottomLeft[2] = phongCP_z[leftIndex + 1] - phongCP_z[curSpot];

			bottom[0] = phongCP_x[curSpot + 1] - phongCP_x[curSpot];
			bottom[1] = phongCP_y[curSpot + 1] - phongCP_y[curSpot];
			bottom[2] = phongCP_z[curSpot + 1] - phongCP_z[curSpot];
			


			//normal
			normal[0] += (right[1] * topRight[2] - right[2] * topRight[1]);
			normal[1] += (right[2] * topRight[0] - right[0] * topRight[2]);
			normal[2] += (right[0] * topRight[1] - right[1] * topRight[0]);

			normal[0] += (topRight[1] * top[2] - topRight[2] * top[1]);
			normal[1] += (topRight[2] * top[0] - topRight[0] * top[2]);
			normal[2] += (topRight[0] * top[1] - topRight[1] * top[0]);

			normal[0] += (top[1] * left[2] - top[2] * left[1]);
			normal[1] += (top[2] * left[0] - top[0] * left[2]);
			normal[2] += (top[0] * left[1] - top[1] * left[0]);

			normal[0] += (left[1] * bottomLeft[2] - left[2] * bottomLeft[1]);
			normal[1] += (left[2] * bottomLeft[0] - left[0] * bottomLeft[2]);
			normal[2] += (left[0] * bottomLeft[1] - left[1] * bottomLeft[0]);

			normal[0] += (bottomLeft[1] * bottom[2] - bottomLeft[2] * bottom[1]);
			normal[1] += (bottomLeft[2] * bottom[0] - bottomLeft[0] * bottom[2]);
			normal[2] += (bottomLeft[0] * bottom[1] - bottomLeft[1] * bottom[0]);

			normal[0] += (bottom[1] * right[2] - bottom[2] * right[1]);
			normal[1] += (bottom[2] * right[0] - bottom[0] * right[2]);
			normal[2] += (bottom[0] * right[1] - bottom[1] * right[0]);
			

			//normalize - divide first then normalizE? think 
			un = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);

			normal[0] /= (un);
			normal[1] /= (un);
			normal[2] /= (un);
			
			phongNorm_x[curSpot] = normal[0];
			phongNorm_y[curSpot] = normal[1];
			phongNorm_z[curSpot] = normal[2];

			normal[0] = 0;
			normal[1] = 0;
			normal[2] = 0;
		}

		phongNorm_x[(j + 1) * phongVerticalCP - 1] =  0.0f;
		phongNorm_y[(j + 1) * phongVerticalCP - 1] = -1.0f;
		phongNorm_z[(j + 1) * phongVerticalCP - 1] =  0.0f;
	}
}





/**********************************************
 * End Routines for Midpoint Subdivision 
 **********************************************/

/* End of data.cpp */
