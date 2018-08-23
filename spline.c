/*
  Library for 1-d splines
  Copyright Ryan Michael (original cpp version)
  Copyright Oliver Dippel (port to c)
  Licensed under the LGPLv3 
*/

#include <math.h>
#include "spline.h"


SPLINE spline_new(float *x, float *y, int numPoints) {
	SPLINE sp;
	sp._prev_point = 0;
	sp._x = x;
	sp._y = y;
	sp._length = numPoints;
	sp._degree = Catmull;
	return sp;
}

float spline_hermite_00(float t) {
	return (2 * pow(t, 3)) - (3 * pow(t, 2)) + 1;
}

float spline_hermite_10(float t) {
	return pow(t, 3) - (2 * pow(t, 2)) + t;
}

float spline_hermite_01(float t) {
	return (3 * pow(t, 2)) - (2 * pow(t, 3));
}

float spline_hermite_11(float t) {
	return pow(t, 3) - pow(t, 2);
}

float spline_catmull_tangent(SPLINE sp,  int i)  {
	if (sp._x[i + 1] == sp._x[i - 1]) {
		return 0;
	} else {
		return (sp._y[i + 1] - sp._y[i - 1]) / (sp._x[i + 1] - sp._x[i - 1]);
	}
}

float spline_hermite(float t, float p0, float p1, float m0, float m1, float x0, float x1) {
	return (spline_hermite_00(t) * p0) + (spline_hermite_10(t) * (x1 - x0) * m0) + (spline_hermite_01(t) * p1) + (spline_hermite_11(t) * (x1 - x0) * m1);
}

float spline_calc(SPLINE sp, float x, int i) {
	switch (sp._degree) {
		case 0:
			return sp._y[i];
		case 1:
			if (sp._x[i] == sp._x[i + 1]) {
				// Avoids division by 0
				return sp._y[i];
			} else {
				return sp._y[i] + (sp._y[i + 1] - sp._y[i]) * (x - sp._x[i]) / (sp._x[i + 1] - sp._x[i]);
			}
		case Hermite:
			return spline_hermite(((x - sp._x[i]) / (sp._x[i + 1] - sp._x[i])), sp._y[i], sp._y[i + 1], sp._m[i], sp._m[i + 1], sp._x[i], sp._x[i + 1]);
		case Catmull:
			if (i == 0) {
				// x prior to spline start - first point used to determine tangent
				return sp._y[1];
			} else if (i == sp._length - 2) {
				// x after spline end - last point used to determine tangent
				return sp._y[sp._length - 2];
			} else {
				float t = (x - sp._x[i]) / (sp._x[i + 1] - sp._x[i]);
				float m0 = (i == 0 ? 0 : spline_catmull_tangent(sp, i));
				float m1 = (i == sp._length - 1 ? 0 : spline_catmull_tangent(sp,  i + 1));
				return spline_hermite(t, sp._y[i], sp._y[i + 1], m0, m1, sp._x[i], sp._x[i + 1]);
			}
	}
}

float spline_value(SPLINE sp, float x) {
	if (sp._x[0] > x) {
		return sp._y[0];
	} else if (sp._x[sp._length - 1] < x) {
		return sp._y[sp._length - 1];
	} else {
		for (int i = 0; i < sp._length; i++) {
			int index = (i + sp._prev_point) % sp._length;
			if (sp._x[index] == x) {
				sp._prev_point = index;
				return sp._y[index];
			} else if ((sp._x[index] < x) && (x < sp._x[index + 1])) {
				sp._prev_point = index;
				return spline_calc(sp, x, index);
			}
		}
	}
}

