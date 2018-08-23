
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "spline.h"


int main(void) {
	float x[7] = {-1, 0, 1, 2, 3, 4, 5};
	float y[7] = { 0, 0, 8, 5, 2, 10, 10};
	SPLINE sp1 = spline_new(x, y, 7);
	for (float i = 0; i <= 4; i += .1) {
		float temp = spline_value(sp1, i);
		printf("%f", i);
		for (float j = 0; j <= temp; j += .2) {
			printf("*");
		}
		printf("   ");
		printf("%f\n", temp);
	}
	return 0;
};




