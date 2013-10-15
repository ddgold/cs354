#include "Matrix.h"

int main()
{
  Matrix a = {2.0f, 3.0f, 2.0f, 3.2f};
  Matrix b = {1, 2, 1, 1};
  
  Scalar vector = {4, 4};
  
  Scalar c = a.vectorMultiply(vector);
  
  c.print();
}
