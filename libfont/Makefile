CC := g++

SRC_DIR := .
BUILD_DIR := build
STD := c++11
OUTPUT := output 

build_dir:
	mkdir -p ${BUILD_DIR}

compile: build_dir \
		${BUILD_DIR}/main.o ${BUILD_DIR}/json.o

${BUILD_DIR}/json.o:
	${CC} -std=${STD} -c ../libjson/json.cpp -o ${BUILD_DIR}/json.o

${BUILD_DIR}/main.o:${SRC_DIR}/main.cc
	${CC} -std=${STD} -c ${SRC_DIR}/main.cc -o ${BUILD_DIR}/main.o -I .
	
link:compile
	${CC} ${BUILD_DIR}/*.o -o ${BUILD_DIR}/main.exe -Llib -lfreetypedll
	
build_font:link
	${BUILD_DIR}/main

clean:
	rm -f ${BUILD_DIR}/*.o 
	rm -f ${BUILD_DIR}/main.exe
	rm -f *.png
	rm -f *.json