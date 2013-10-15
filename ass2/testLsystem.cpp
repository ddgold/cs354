#include "Lsystem.h"

void matrix_functions ()
{
  Lsystem system = Lsystem((char*)"ABC[]", (char*)"A");
  
  float results[MATRIX] = {0.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 0.0f};
                           
  float a[MATRIX] = {1.0f, 2.0f, 3.0f, 4.0f,
                     1.0f, 2.0f, 3.0f, 4.0f,
                     1.0f, 2.0f, 3.0f, 4.0f,
                     1.0f, 2.0f, 3.0f, 4.0f};
  
  float b[MATRIX] = {1.0f, 2.0f, 3.0f, 4.0f,
                     2.0f, 3.0f, 4.0f, 5.0f,
                     3.0f, 4.0f, 5.0f, 6.0f,
                     4.0f, 5.0f, 6.0f, 7.0f};
  
  float ab[MATRIX] = {30.0f, 40.0f, 50.0f, 60.0f,
                      30.0f, 40.0f, 50.0f, 60.0f,
                      30.0f, 40.0f, 50.0f, 60.0f,
                      30.0f, 40.0f, 50.0f, 60.0f};
  
  float ba[MATRIX] = {10.0f, 20.0f, 30.0f, 40.0f,
                      14.0f, 28.0f, 42.0f, 56.0f,
                      18.0f, 36.0f, 54.0f, 72.0f,
                      22.0f, 44.0f, 66.0f, 88.0f};
               
  float x[VECTOR] = {1.0f, 2.0f, 3.0f, 4.0f};
  
  float y[VECTOR] = {1.0f, 1.0f, 1.0f, 1.0f};
  
  float z[VECTOR] = {1.1f, 2.2f, 10.0f, 3.14159f};
  
  float ax[VECTOR] = {30.0f, 30.0f, 30.0f, 30.0f};
  
  float by[VECTOR] = {10.0f, 14.0f, 18.0f, 22.0f};
  
  assert (system.equal (MATRIX, a, a)  == -1);
  assert (system.equal (MATRIX, a, b)  ==  4);
  assert (system.equal (MATRIX, a, ab) ==  0);
  assert (system.equal (VECTOR, x, x)  == -1);
  assert (system.equal (VECTOR, x, y)  ==  1);
  assert (system.equal (VECTOR, y, z)  ==  0);
  cout << endl <<  "Equals Works" << endl;
  
  assert (system.print (MATRIX, a).compare ("1 2 3 4\n1 2 3 4\n1 2 3 4\n1 2 3 4\n") == 0);
  assert (system.print (MATRIX, b).compare ("1 2 3 4\n2 3 4 5\n3 4 5 6\n4 5 6 7\n") == 0);
  assert (system.print (VECTOR, x).compare ("1\n2\n3\n4\n") == 0);
  assert (system.print (VECTOR, z).compare ("1.1\n2.2\n10\n3.14159\n") == 0);
  cout << endl <<  "Print Works" << endl;
  
  system.multi (MATRIX, a, b, results);
  assert (system.equal (MATRIX, ab, results)  == -1);
  system.multi (MATRIX, b, a, results);
  assert (system.equal (MATRIX, ba, results)  == -1);
  system.multi (VECTOR, a, x, results);
  assert (system.equal (VECTOR, ax, results)  == -1);
  system.multi (VECTOR, b, y, results);
  assert (system.equal (VECTOR, by, results)  == -1);
  cout << endl <<  "Multi Works" << endl;
  
  system.copy (MATRIX, a, b);
  assert (system.equal (MATRIX, a, b)  == -1);
  assert (system.equal (MATRIX, b, a)  == -1);
  assert (system.print (MATRIX, b).compare ("1 2 3 4\n1 2 3 4\n1 2 3 4\n1 2 3 4\n") == 0);
  system.copy (VECTOR, x, y);
  assert (system.equal (VECTOR, x, y)  == -1);
  assert (system.equal (VECTOR, y, x)  == -1);
  assert (system.print (VECTOR, y).compare ("1\n2\n3\n4\n") == 0);
  cout << endl <<  "Copy Works" << endl;
  
  
  system.clear (MATRIX, a);
  system.clear (MATRIX, b);
  assert (system.equal (MATRIX, a, b)  == -1);
  assert (system.print (MATRIX, b).compare ("0 0 0 0\n0 0 0 0\n0 0 0 0\n0 0 0 0\n") == 0);
  system.clear (VECTOR, x);
  system.clear (VECTOR, y);
  assert (system.equal (VECTOR, x, y)  == -1);
  assert (system.print (VECTOR, x).compare ("0\n0\n0\n0\n") == 0);
  cout << endl << "Clear Works" << endl;
  
  cout << endl << "All Matrix Functions Work!" << endl << endl;
}

int main()
{
  //matrix_functions ();
  Lsystem system = Lsystem((char*)"ABC[]", (char*)"ABAAB[BC][BC][BC]AA");
      
  system.execute (5);
}
