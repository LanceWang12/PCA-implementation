CXX = g++
# CXXFLAGS = -std=c++17 -Wall -O3 -shared -fPIC `python3 -m pybind11 --includes` `python3-config --includes` -Iextern/pybind11/include -I/home/lance/Dev/eigen
CXXFLAGS = -std=c++17 -Wall -O3 -I/home/lance/Dev/eigen -I/usr/include/mkl
PYTHONFLAGS = -std=c++17 -Wall -O3 -shared -fPIC $(shell python3 -m pybind11 --includes) $(shell python3-config --includes) -Iextern/pybind11/include
LFLAGS = -L/usr/lib/x86_64-linux-gnu/mkl -lblas
SRC_DIR = src

all: compile link
	@echo Compile successfully

test: $(SRC_DIR)/matrix.h $(SRC_DIR)/PCA.h test.cpp
	$(CXX) $(CXXFLAGS) -c test.cpp
	$(CXX) -o test.out test.o $(LFLAGS)

compile: $(SRC_DIR)/matrix.h $(SRC_DIR)/PCA.h main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

link: main.o
	$(CXX) -o main.out main.o $(LFLAGS)

clean:
	rm -rf *.out *.o
