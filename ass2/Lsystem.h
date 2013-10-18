#ifndef _LSYSTEM_H_
#define _LSYSTEM_H_

#include <cassert>
#include <iostream>
#include <cstring>
#include <cmath>
#include <stack>

#include "helper.h"
//#include "drawplant.h"

using namespace std;

class Lsystem
{
private:
  // ----------
  // Attributes
  // ----------
  
  /*
   * The order of rules
   */
  char* _sequence;
  
  /*
   * All valid rules
   */
  char* _rules;
  
  /*
   * The current matrix
   */
  float* _curMatrix;
  
  /*
   * The matrix stack
   */
  stack<float*> _matrixStack;
  
  
  
public:
  Lsystem (char*, char*);
  
  void execute (int);
  
  void loadcurMatrix ();
  
  void push ();
  void pop ();
  

  // ---------------
  // Transformations
  // ---------------

  /*
   * Translates _curMatrix by (x, y, z)
   */
  void translate (float, float, float);

  /*
   * Scales _curMatrix by (x, y, z)
   */
  void scale (float, float, float);

  /*
   * Rotate the _curMatrix by angle degrees
   * See enumeration for posible r values
   */
  void rotate (Rotation, float);
};
  
#endif
