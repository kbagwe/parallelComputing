CXXFLAGS=-std=c++14 -O3 -fopenmp
LDFLAGS=-fopenmp
CUDAFLAGS=-O3 -arch=sm_37 -std=c++14
LD=g++

polynomial: polynomial.cpp

polynomial_gpu: polynomial_gpu.cu
	nvcc $(CUDAFLAGS) polynomial_gpu.cu -o polynomial_gpu

test_cpu: polynomial
	./test_cpu.sh

test_gpu: polynomial_gpu
	./test_gpu.sh

clean:
	-rm polynomial polynomial_gpu


