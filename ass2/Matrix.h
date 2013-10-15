// ----------------
// ----------------
// No Longer in Use
// ----------------
// ----------------

#include <iostream>
#include <cassert>
#include <cmath>
#include <initializer_list>

using namespace std;


class Scalar
{
  public:
    float X[4];

    Scalar()
    {
      int a;
       for(a = 0; a < 4; ++a)
       {
         X[a] = 0;
       }
    }
    
    Scalar(initializer_list<float> v)
    {
      auto begin = v.begin(), end = v.end();
      assert((end - begin) == 4);
      
      int a;
      for(a = 0; a < 4 ; ++a)
      {
         X[a] = *begin;
         ++begin;
      }
        
    }
    
    Scalar<i> copy ()
    {
      Scalar<i> results = Scalar<i>();
      
      int a;
      for (a = 0; a < 4; ++a)
      {
        results.X[a] = X[a];
      }
      
      return results;
    }
    
    void print()
    {
      int a;
      for (a = 0; a < 4; ++a)
      {
         cout << X[a] << endl;
      }
    }

  
};

class Matrix 
{
  public:
    float X[4][4];
    
  public:
    Matrix()
    {
      int a, b;
      for (a = 0; a < 4; ++a)
      {
        for (b = 0; b < 4; ++b)
        {
          X[a][b] = 0;
        }
      }
    }
    
    Matrix(initializer_list<float> v)
    {
      auto begin = v.begin(), end = v.end();
      assert (16 == (end - begin));
      
      int a, b;
      for (a = 0; a < 4; ++a)
      {
        for (b = 0; b < 4; ++b)
        {
          X[a][b] = *begin;
          ++begin;
        }
      }
    }
    
    
    Matrix matrixMultiply (Matrix rhs)
    {
      Matrix results = Matrix();
        
      int a, b, c;
      for (a = 0; a < 4; ++a)
      {
        for (b = 0; b < 4; ++b)
        {
          for (c = 0; c < 4; ++c)
          {
            results.X[a][b] += X[a][c] * rhs.X[c][b];
          }
        }
      }
      
      return results;
    }
    
    Scalar vectorMultiply (Scalar rhs)
    {
      //assert ((sizeof (rhs) / sizeof (float)) == i);
      Scalar results = Scalar();
        
      int a, b;
      for (a = 0; a < 4; ++a)
      {
        for (b = 0; b < 4; ++b)
        {
          results.X[a] += X[a][b] * rhs.X[b];
        }
      }
      
      return results;
    }
    
    Matrix copy ()
    {
      Matrix results = Matrix();
      
      int a, b;
      for (a = 0; a < 4; ++a)
      {
        for (b = 0; b < 4; ++b)
        {
          results.X[a][b] = X[a][b];
        }
      }
      
      return results;
    }
    
    void print()
    {
      int a, b;
      for (a = 0; a < 4; ++a)
      {
        for (b = 0; b < 3; ++b)
        {
          cout << X[a][b] << " ";
        }
        cout << X[a][b] << endl;
      }
    }
};




    Scalar translate (float x, float y, float z, Scalar that)
    {
      Matrix temp = {1,0,0,x,
                        0,1,0,y,
                        0,0,1,z,
                        0,0,0,1};
     return temp.vectorMultiply(that);  
    }
    
    //the point is a scalar
    
   
    
    Scalar rotateX(float degree, Scalar that)
    {
       //turn degree into radians 
       float rads = (degree/360.0)*(2*M_PI);
       
       Matrix temp = {1,0,0,0,
                        0,cos(rads),-1*sin(rads),0,
                        0,sin(rads),cos(rads),0,
                        0,0,0,1};
      return temp.vectorMultiply(that);   
        
    }
    
    Scalar rotateY(float degree, Scalar that)
    {
       //turn degree into radians 
       float rads = (degree/360.0)*(2*M_PI);
       
       Matrix temp = {cos(rads),0,sin(rads),0,
                         0,1,0,0,
                         -sin(rads),0,cos(rads),0,
                         0,0,0,1};
       return temp.vectorMultiply(that); 
        
    }
    
    Scalar rotateZ(float degree, Scalar that)
    {
       //turn degree into radians 
       float rads = (degree/360.0)*(2*M_PI);
       
       Matrix temp = {cos(rads),-1*sin(rads),0,0,
                         sin(rads),cos(rads),0,0,
                         0,0,1,0,
                         0,0,0,1};
        return temp.vectorMultiply(that); 
        
    }
