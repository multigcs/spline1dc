

all: spline

spline: main.c spline.c
	gcc -Os -o spline main.c spline.c -I ./ -lm
	./spline

clean:
	rm -rf spline

