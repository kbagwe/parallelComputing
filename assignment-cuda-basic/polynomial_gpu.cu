#include <iostream>
#include <chrono>

//Host Code
__global__ void polynomial_expansion (float* poly, int degree, int n, float* array) { 
  
     int index = blockIdx.x * blockDim.x + threadIdx.x;
    if( index < n ){
        float polynomial = 0.0;
        float power = 1.0;
        for ( int i = 0; i < degree+1; ++i){
        polynomial += power * poly[i];
        power *= array[index];
        }
        array[index] = polynomial;
    }
}

//
//Referred Github Code
//
int main (int argc, char* argv[]) {
    ////TODO: add usage

    if (argc < 3) {
        std::cerr<<"usage: "<<argv[0]<<" n degree"<<std::endl;
        return -1;
    }

    int n = atoi(argv[1]); 
    int degree = atoi(argv[2]);
    int nbiter = atoi(argv[3]);;

    float* array = new float[n];
    float* poly = new float[degree+1];
    for (int i=0; i<n; ++i) array[i] = 1.;

    for (int i=0; i<degree+1; ++i) poly[i] = 1.;

    float *dev_array, *dev_polynomial;
  
    std::chrono::time_point<std::chrono::system_clock> startTime, endTime;
    startTime = std::chrono::system_clock::now();
  	
	cudaMallocManaged(&dev_array, n*sizeof(float));
  	cudaMallocManaged(&dev_polynomial, (degree+1)*sizeof(float));
  
  	cudaMemcpy(dev_array, array, n*sizeof(float), cudaMemcpyHostToDevice);
  	cudaMemcpy(dev_polynomial, poly, (degree+1)*sizeof(float), cudaMemcpyHostToDevice);

  	polynomial_expansion<<<(n+255)/256, 256>>>(dev_polynomial, degree, n, dev_array);
  	cudaMemcpy(array, dev_array, n*sizeof(float), cudaMemcpyDeviceToHost);
       
    cudaFree(dev_array);
    cudaFree(dev_polynomial);

    cudaDeviceSynchronize();

    endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_time = (endTime-startTime)/nbiter;	 
    
    {
        bool finish = true;
        for (int i=0; i< n; ++i) {
        if (fabs(array[i]-(degree+1))>0.01) {
            finish = false;
        }
        }
        if (!finish) std::cerr<<"Incorrect Result."<<std::endl;
    }

    std::cerr<<array[0]<<std::endl;
    std::cout<<n<<" "<<degree<<" "<<elapsed_time.count()<<std::endl;

    delete[] array;
    delete[] poly;

    return 0;
}