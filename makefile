CXX = g++
# CXXFLAGS = -std=c++17 -Wall -O3 -shared -fPIC `python3 -m pybind11 --includes` `python3-config --includes` -Iextern/pybind11/include -I/home/lance/Dev/eigen
CXXFLAGS = -std=c++17 -Wall -O3 -I/home/lance/Dev/eigen -I/usr/include/mkl
PYTHONFLAGS = -std=c++11 -Wall -O3 -shared -fPIC $(shell python3 -m pybind11 --includes) $(shell python3-config --includes) -Iextern/pybind11/include -I/usr/include/mkl
LFLAGS = -L/usr/lib/x86_64-linux-gnu/mkl -lblas # $(shell python3.8-config --ldflags)
SRC_DIR = src
EXE = PCA$(shell python3-config --extension-suffix)
OBJ = main.o
HEADER = $(SRC_DIR)/matrix.h $(SRC_DIR)/PCA.h $(SRC_DIR)/wrapper.h
TEST_FILE = test_pca.py

all: $(EXE)
	@echo Compile successfully

$(OBJ): $(HEADER) main.cpp
	$(CXX) $(PYTHONFLAGS) -c -o $@ main.cpp

$(EXE): $(OBJ)
	$(CXX) $(PYTHONFLAGS) -o $@ $^ $(LFLAGS)

test: all
	@echo start to test
	@python3 -W ignore::DeprecationWarning -m pytest -q $(TEST_FILE)

ctest: $(HEADER) test_pca.cpp
	$(CXX) $(CXXFLAGS) -c test_pca.cpp
	$(CXX) -o test_pca.out test_pca.o $(LFLAGS)

clean:
	rm -rf *.out *.o
