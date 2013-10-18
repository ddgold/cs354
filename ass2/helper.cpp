#include "helper.h"

using namespace std;
// ---------------
// Transformations
// ---------------

void translate (float* input, float x, float y, float z)
{
  float translate[MATRIX] = {1.0f, 0.0f, 0.0f, x,
                             0.0f, 1.0f, 0.0f, y,
                             0.0f, 0.0f, 1.0f, z,
                             0.0f, 0.0f, 0.0f, 1.0f};
  
  float results[MATRIX];
  
  //cout << "translate\n" << print (MATRIX, translate) << endl;
  
  multi (MATRIX, translate, input, results);
  copy (MATRIX, results, input);
  return;
}

void scale (float* input, float x, float y, float z)
{
  float scale[MATRIX] = {x,    0.0f, 0.0f, 0.0f,
                         0.0f, y,    0.0f, 0.0f,
                         0.0f, 0.0f, z,    0.0f,
                         0.0f, 0.0f, 0.0f, 1.0f};
  
  float results[MATRIX];
  multi (MATRIX, scale, input, results);
  copy (MATRIX, results, input);
  return;
}

void rotate (float* input, Rotation r, float angle)
{
  assert (r >= 0 && r <= 5);
  if (r % 2 == 1) // Rotate at origin
  {
   
   /*
     float translate[MATRIX] = {1.0f, 0.0f, 0.0f, 0.0f,
                               0.0f, 1.0f, 0.0f, 0.0f,
                               0.0f, 0.0f, 1.0f, 0.0f,
                               0.0f, 0.0f, 0.0f, 1.0f};
    
    translate[3] =  -input[3];
    translate[7] =  -input[7];
    translate[11] = -input[11];
    
    float before[MATRIX];
    multi (MATRIX, translate, input, before);
    
    rotate (before, (Rotation)(r - 1), angle);
    
    translate[3] =  -translate[3];
    translate[7] =  -translate[7];
    translate[11] = -translate[11];
    
    multi (MATRIX, translate, before, input);
    
    return;
    */  
   
    float before[MATRIX];
    copy (MATRIX, input, before);
    
    float identity[MATRIX] = {1.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 1.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 1.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 1.0f};
    copy (MATRIX, identity, input);
    
    
    rotate (input, (Rotation)(r - 1), angle);
    
    float after[MATRIX];
    copy (MATRIX, input, after);
    
    
    float results[MATRIX];
    multi (MATRIX, after, before,  results);
    copy (MATRIX, results, input);

    return;
  }
  else            // Rotate at location
  {
    angle = angle * M_PI / 180.0f;
    float rotate[MATRIX] = {1.0f, 0.0f, 0.0f, 0.0f,
                            0.0f, 1.0f, 0.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f};
    if (r == LOCATIONX)
    {
      rotate[5] =  cos (angle);
      rotate[6] = -sin (angle);
      rotate[9] =  sin (angle);
      rotate[10] = cos (angle);
    }
    else if (r == LOCATIONY)
    {
      rotate[0] =  cos (angle);
      rotate[2] =  sin (angle);
      rotate[8] = -sin (angle);
      rotate[10] = cos (angle);
    }
    else
    {
      rotate[0] =  cos (angle);
      rotate[1] = -sin (angle);
      rotate[4] =  sin (angle);
      rotate[5] =  cos (angle);
    }
    
    float results[MATRIX];
    //cout << "rotate\n" << print (MATRIX, rotate) << endl;
    multi (MATRIX, rotate, input, results);
    copy (MATRIX, results, input);
    return;
  }
}

// ----------------
// Matrix Functions
// ----------------

void copy (Type t, const float* from, float* to)
{
  assert (t == VECTOR || t == MATRIX);
  for (int i = 0; i < t; ++i)
  {
    to[i] = from[i];
  }
  return;
}

void multi (Type t, const float* lhs, const float* rhs, float* results)
{
  assert (t == VECTOR || t == MATRIX);
  clear (t, results);
  
  if (t == V) // VECTOR
  {
    for (int a = 0; a < VECTOR; ++a)
    {
      for (int b = 0; b < VECTOR; ++b)
      {
        results[a] += lhs[(a * VECTOR) + b] * rhs[b];
      }
    }
    return;
  }
  else        // MATRIX
  {
    for (int a = 0; a < VECTOR; ++a)
    {
      for (int b = 0; b < VECTOR; ++b)
      {
        for (int c = 0; c < VECTOR; ++c)
        {
          results[(a * VECTOR) + b] += lhs[(a * VECTOR) + c] * rhs[(c * VECTOR) + b];
        }
      }
    }
    return;
  }
}

void clear (Type t, float* empty)
{
  assert (t == VECTOR || t == MATRIX);
  for (int i = 0; i < t; ++i)
  {
    empty[i] = 0.0f;
  }
  return;
}

int equal (Type t, const float* lhs, const float* rhs)
{
  assert (t == VECTOR || t == MATRIX);
  for (int i = 0; i < t; ++i)
  {
    if (lhs[i] != rhs[i])
    {
      return i;
    }
  }
  return -1;
}

string print (Type t, const float* output)
{
  assert (t == VECTOR || t == MATRIX);
  stringstream ss;
  if (t == V) // VECTOR
  {
    for (int i = 0; i < VECTOR; ++i)
    {
      ss << output[i] << endl;
    }
  }
  else        // MATRIX
  {
    for (int i = 0; i < VECTOR; ++i)
    {
      for (int j = 0; j < VECTOR - 1; ++j)
      {
        ss << output[(i * VECTOR) + j] << " ";
      }
      ss << output[(i * VECTOR) + VECTOR - 1] << endl;
    }
  }
  return ss.str();
}
