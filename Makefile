CC=gcc
CPP=g++
CFLAGS=-g -Wall
SPECLIBS=-lcairo -lm
# NOTE: beware cairomm, it can be a little devil to compile
# prob due to its being part of a bigger project
SPECINC2=`pkg-config --cflags cairomm-1.0`
SPECLIBS2=`pkg-config --libs cairomm-1.0`

EXECUTABLES=ra0

ra0: ra0.c
	${CC} ${CFLAGS} -o $@ $^ ${SPECLIBS}

.PHONY: clean

clean:
	rm -f ${EXECUTABLES}
