TAO_DIR=/home/zlebnik/practice/tao-2.2.0
PETSC_DIR=/home/zlebnik/practice/petsc-3.4.3
PETSC_ARCH=arch-linux2-cxx-debug


include ${TAO_DIR}/conf/tao_base

preparation:
	mpic++ -c shape.cpp
	mpic++ -c rectshape.cpp
	mpic++ -c rect.cpp
	mpic++ -c placement.cpp
	mpic++ -c functions.cpp
	mpic++ -c output.cpp

test: preparation
	mpic++ test.cpp rect.o shape.o rectshape.o placement.o functions.o output.o

visualise: preparation
	-${CLINKER} -o visualise visualise.cpp rect.o shape.o rectshape.o placement.o functions.o output.o

all: main.o visualise preparation tao_chkopts
	-${CLINKER} -o main main.o rect.o shape.o rectshape.o placement.o functions.o output.o ${TAO_LIB} ${PETSC_LIB}
	${RM} main.o
	${RM} rect.o shape.o rectshape.o placement.o functions.o output.o

