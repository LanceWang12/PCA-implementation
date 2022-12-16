CXX = g++
# CXXFLAGS = -std=c++17 -Wall -O3 -shared -fPIC `python3 -m pybind11 --includes` `python3-config --includes` -Iextern/pybind11/include -I/home/lance/Dev/eigen
CXXFLAGS = -std=c++17 -Wall -O3 -I/home/lance/Dev/eigen -I/usr/include/mkl
PYTHONFLAGS = -std=c++17 -Wall -O3 -shared -fPIC -I/usr/include/mkl $(shell python3 -m pybind11 --includes) $(shell python3-config --includes) -Iextern/pybind11/include
LFLAGS = -L/usr/lib/x86_64-linux-gnu/mkl -lblas $(shell python3.8-config --ldflags)
SRC_DIR = src
EXE = PCA$(shell python3-config --extension-suffix)


all: compile $(EXE)
	@echo Compile successfully

test: $(SRC_DIR)/matrix.h $(SRC_DIR)/PCA.h test.cpp
	$(CXX) $(CXXFLAGS) -c test.cpp
	$(CXX) -o test.out test.o $(LFLAGS)

compile: $(SRC_DIR)/matrix.h $(SRC_DIR)/PCA.h $(SRC_DIR)/wrapper.h main.cpp
	$(CXX) $(PYTHONFLAGS) -c main.cpp

$(EXE): main.o
	$(CXX) -o $@ $^ $(LFLAGS)

clean:
	rm -rf *.out *.o
