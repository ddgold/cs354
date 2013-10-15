//
//  Lsystem.cpp
//  
//
//  Created by Doug Goldstein on 10/15/13.
//
//

#include "Lsystem.h"

using namespace std;



// -----------
// Constructor
// -----------

Lsystem::Lsystem (char* rules, char* sequence)
{
  assert (strlen (rules) >= 0);
  _rules =  rules;
  
  assert (strlen (sequence) >= 0);
  // Must verify all characters in sequence are valid rules
  for (int i = 0; i < strlen(sequence); ++i)
  {
    if (strchr(_rules, sequence[i]) == NULL)
    {
      cout << sequence[i] << " is not a valid rule, double check input" << endl;
      assert (0);
    }
  }
  _sequence = sequence;
  _curMatrix = new float[MATRIX];
}



// -------
// Execute
// -------
void Lsystem::execute (int i)
{
  assert (i >= 0);
  if (i == 0)
  {
    // End Case
  }
  else
  {
    // Loop through each rule in _sequence
    for (int c = 0; c < strlen(_sequence); ++c)
    {
      switch (_sequence[c])
      {
        case 'A':
          ruleA();
          break;
          
        case 'B':
          ruleB();
          break;
          
        case 'C':
          ruleC();
          break;
        case '[':
          push();
          break;
          
        case ']':
          pop();
          break;
          
        default:
          cout << "Something wrong happend, you shouldn't be here." << endl;
          assert(0);
          break;
      }
      
    }
  }
}



// -----
// Rules
// -----
void Lsystem::ruleA ()
{
  cout << "Rule A" << endl;
  return;
}

void Lsystem::ruleB ()
{
  cout << "Rule B" << endl;
  return;
}

void Lsystem::ruleC ()
{
  cout << "Rule C" << endl;
  return;
}

void Lsystem::push()
{
  cout << "Push" << endl;
  float* temp = new float[16];
  copy (MATRIX, _curMatrix, temp);
  _matrixStack.push (temp);
}

void Lsystem::pop()
{
  cout << "Pop" << endl;
  float* temp = _matrixStack.top();
  copy (MATRIX, temp, _curMatrix);
  _matrixStack.pop ();
  delete temp;
}



// ---------------
// Transformations
// ---------------

void Lsystem::translate (float x, float y, float z)
{
  float scale[MATRIX] = {0.0f, 0.0f, 0.0f, x,
                         0.0f, 0.0f, 0.0f, y,
                         0.0f, 0.0f, 0.0f, z,
                         0.0f, 0.0f, 0.0f, 1.0f};
  
  float results[MATRIX];
  multi (MATRIX, scale, _curMatrix, results);
  copy (MATRIX, results, _curMatrix);
  //load3DMatrix (_curMatrix);
  return;
}

void Lsystem::scale (float x, float y, float z)
{
  float scale[MATRIX] = {x,    0.0f, 0.0f, 0.0f,
                         0.0f, y,    0.0f, 0.0f,
                         0.0f, 0.0f, z,    0.0f,
                         0.0f, 0.0f, 0.0f, 1.0f};
  
  float results[MATRIX];
  multi (MATRIX, scale, _curMatrix, results);
  copy (MATRIX, results, _curMatrix);
  //load3DMatrix (_curMatrix);
  return;
}

void Lsystem::rotate (Rotation r, float angle)
{
  assert (r >= 0 && r <= 5);
  if (r % 2 == 1) // Rotate at origin
  {
    float before[MATRIX];
    copy (MATRIX, _curMatrix, before);
    
    float identity[MATRIX] = {1.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 1.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 1.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 1.0f};
    copy (MATRIX, identity, _curMatrix);
    //load3DMatrix (_curMatrix);
    
    rotate ((Rotation)(r - 1), angle);
    
    float after[MATRIX];
    copy (MATRIX, _curMatrix, after);
    
    
    float results[MATRIX];
    multi (MATRIX, after, before, results);
    copy (MATRIX, results, _curMatrix);
    //load3DMatrix (_curMatrix);
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
    multi (MATRIX, rotate, _curMatrix, results);
    copy (MATRIX, results, _curMatrix);
    //load3DMatrix(_curMatrix);
    return;
  }
  
}



// ----------------
// Matrix Functions
// ----------------

void Lsystem::copy (Type t, const float* from, float* to)
{
  assert (t == VECTOR || t == MATRIX);
  for (int i = 0; i < t; ++i)
  {
    to[i] = from[i];
  }
  return;
}

void Lsystem::multi (Type t, const float* lhs, const float* rhs, float* results)
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

void Lsystem::clear (Type t, float* empty)
{
  assert (t == VECTOR || t == MATRIX);
  for (int i = 0; i < t; ++i)
  {
    empty[i] = 0.0f;
  }
  return;
}

int Lsystem::equal (Type t, const float* lhs, const float* rhs)
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

string Lsystem::print (Type t, const float* output)
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
