#include "Lsystem.h"

using namespace std;

int main ()
{
  Lsystem system = Lsystem((char*)"ABC[]", (char*)"A");
  
  float test[MATRIX] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  
  cout << print (MATRIX, test);
  
}
