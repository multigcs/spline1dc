/*
  Library for 1-d splines
  Copyright Ryan Michael (original cpp version)
  Copyright Oliver Dippel (port to c)
  Licensed under the LGPLv3 
*/

#define Hermite 10
#define Catmull 11

typedef struct {
	float *_x;
	float *_y;
	float *_m;
	int _degree;
	int _length;
	int _prev_point;
} SPLINE;

SPLINE spline_new(float *x, float *y, int numPoints);
float spline_value(SPLINE sp, float x);

