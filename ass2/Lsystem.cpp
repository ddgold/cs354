#include "Lsystem.h"
#include "helper.h"
#include "drawplant.h"

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
  for (unsigned int i = 0; i < strlen(sequence); ++i)
  {
    if (strchr(_rules, sequence[i]) == NULL)
    {
      cout << sequence[i] << " is not a valid rule, double check input" << endl;
      assert (0);
    }
  }
  _sequence = sequence;
  _curMatrix = new float[MATRIX];
  _curMatrix[0]  = 1.0f; 
  _curMatrix[5]  = 1.0f;
  _curMatrix[10] = 1.0f;
  _curMatrix[15] = 1.0f;
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
    for (unsigned int c = 0; c < strlen(_sequence); ++c)
    {
      switch (_sequence[c])
      {
        case 'A':
          break;
          
        case 'B':   //rotate 90 counter clockwise 90 around the z axis
          rotate(ORIGINZ, 90.0f);
          break;
          
        case 'F':   // Draw branch
          drawBranch ();
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


void Lsystem::loadcurMatrix ()
{
  //load3DMatrix (_curMatrix);
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
  loadcurMatrix ();
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
  loadcurMatrix ();
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
    loadcurMatrix ();
    
    rotate ((Rotation)(r - 1), angle);
    
    float after[MATRIX];
    copy (MATRIX, _curMatrix, after);
    
    
    float results[MATRIX];
    multi (MATRIX, after, before, results);
    copy (MATRIX, results, _curMatrix);
    loadcurMatrix ();
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
    loadcurMatrix ();
    return;
  }
}

