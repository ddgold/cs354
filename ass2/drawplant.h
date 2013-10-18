#ifndef _DRAWPLANT_H_
#define _DRAWPLANT_H_

using namespace std;

void load2DMatrix(GLfloat, GLfloat, GLfloat, GLfloat,
		              GLfloat, GLfloat, GLfloat, GLfloat,
		              GLfloat, GLfloat, GLfloat, GLfloat,
		              GLfloat, GLfloat, GLfloat, GLfloat);

void load3DMatrix(GLfloat, GLfloat, GLfloat, GLfloat,
		              GLfloat, GLfloat, GLfloat, GLfloat,
		              GLfloat, GLfloat, GLfloat, GLfloat,
		              GLfloat, GLfloat, GLfloat, GLfloat);

void load3DMatrix(const float*);

void drawLeaf (int, float);

void drawActualLeaf (void);

void drawBranch (int, float);

void drawActualBranch (float);

void push (void);

void pop (void);

void turnRight(float);

void turnLeft(int, float);

void rotateCamera(double, int);


#define X_AXIS			0
#define Y_AXIS			1
#define Z_AXIS			2


void initialize (void);

void drawPlant (int, float);

#endif	/* _DRAWPLANT_H_ */
