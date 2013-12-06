#ifndef _DATA_H_
#define _DATA_H_

/* Globals declared in data.cpp */

// The maximum number of points allowed in the 4 Point Scheme
#define MAX_POINT   30

extern GLfloat originalCP_x[MAX_POINT];
extern GLfloat originalCP_y[MAX_POINT];
extern int originalCP;


extern GLfloat *gouraudCP_x;
extern GLfloat *gouraudCP_y;
extern GLfloat *gouraudCP_z;

extern GLfloat *gouraudNorm_x;
extern GLfloat *gouraudNorm_y;
extern GLfloat *gouraudNorm_z;

extern int gouraudVerticalCP;
extern int gouraudRadialCP;


extern GLfloat *phongCP_x;
extern GLfloat *phongCP_y;
extern GLfloat *phongCP_z;

extern GLfloat *phongNorm_x;
extern GLfloat *phongNorm_y;
extern GLfloat *phongNorm_z;

extern int phongVerticalCP;
extern int phongRadialCP;


extern int verticalSubLevel;
extern int radialSubLevel;


extern GLfloat shininess[1];
extern GLfloat specular[4];
extern GLfloat lightPosition[4];


extern bool is3D;
extern bool isWire;
extern bool isPoints;
extern bool isPhong;



/* Functions implemented in data.cpp */
void subdividePointsArray(int subdiv_level);

void radialSubdivide(void);

void verticalSubdivide(void);

void add2dControlPoint(int,int); 

void remove2dControlPoint(void);

void generateRotationPoints(int n);

void generateGouraud(void);

void generatePhong(void);

#endif	/* _DATA_H_ */
