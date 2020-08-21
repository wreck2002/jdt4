#!/bin/bash

SRC_CPP = test2.cpp test2_jdt2.cpp test2_jdt3.cpp test2_jdt4.cpp is_in_line.cpp is_in_line_collinear.cpp \
      is_in_line_cross.cpp util.cpp

SRC_H = test2_jdt2.h is_in_line.h util.h test2_jdt3.h test2_jdt4.h

test1 : ${SRC_CPP} ${SRC_H}
	g++ ${SRC_CPP} -O2 -std=c++11 -o test2 `pkg-config --libs --cflags gtk+-2.0`

clean :
	rm -rf test
run :
	./test2


