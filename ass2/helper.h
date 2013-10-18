#ifndef _HELPER_H_
#define _HELPER_H_

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <cmath>

using namespace std;

/*
 * Choose between vector or matrix version of functions
 * Used in copy, multi, clear, and print
 */
enum Type
{
  V      = 4,
  VECTOR = 4,
  M      = 16,
  MATRIX = 16
};

/*
 * Choose the correct type of rotaion
 * LOCATIONX, LOCATIONY, LOCATIONZ are mod 2 = 0
 * ORIGINX, ORIGINY, ORIGINZ are mod 2 = 1
 */
enum Rotation
{
  LOCATIONX,
  ORIGINX,
  LOCATIONY,
  ORIGINY,
  LOCATIONZ,
  ORIGINZ
};

static float _curMatrix[MATRIX] = {1.0f, 0.0f, 0.0f, 0.0f,
                                   0.0f, 1.0f, 0.0f, 0.0f,
                                   0.0f, 0.0f, 1.0f, 0.0f,
                                   0.0f, 0.0f, 0.0f, 1.0f};
                                   
static float _cameraMulti[] = {1.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 1.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 1.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 1.0f};

static stack<float*> _matrixStack;

static int _test = 1;

static int _iterations = 1;

// ---------------
// Transformations
// ---------------

/*
 * Translates _curMatrix by (x, y, z)
 */
void translate (float*, float, float, float);

/*
 * Scales _curMatrix by (x, y, z)
 */
void scale (float*, float, float, float);

/*
 * Rotate the _curMatrix by angle degrees
 * See enumeration for posible r values
 */
void rotate (float*, Rotation, float);



// ----------------
// Matrix Functions
// ----------------

/*
 * Copies content from first to second
 * Amount copied is dependent on type t
 */
void copy (Type, const float*, float*);

/*
 * Multiply a matrix with either a vector or matrix, dependent on t
 * lhs must always be a matrix
 */
void multi (Type, const float*, const float*, float*);

/*
 * Clears content from empty (sets all values to 0.0f)
 * Amount cleared is dependent on type t
 */
void clear (Type, float*);

/*
 * Compares two matri or two vectors, dependent on t
 * if lhs == lhs, returns -1
 * else returns index of first difference
 */
int equal (Type, const float*, const float*);

/*
 * Returns the output in a string
 */
string print (Type, const float*);

#endif
