#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <omp.h>
#include <chrono>

#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (const int* arr, size_t n);

#ifdef __cplusplus
}
#endif

void parBubbleSort(int *arr, int n, int nbthread){
   omp_set_num_threads(nbthread);
   for(int i=0; i<n;i++){
    int startIndex = i%2;
    #pragma omp parallel for default(none) shared(arr, n, startIndex)
    for(int j=startIndex; j<n-1; j+=2){
      if(arr[j]>arr[j+1]){
        int temp = arr[j];
        arr[j] = arr[j+1];
        arr[j+1] = temp;
      }
    }
   }
}

int main (int argc, char* argv[]) {

  if (argc < 3) { std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int nbthread = atoi(argv[2]);
  
  // get arr data
  int * arr = new int [n];
  generateMergeSortData (arr, n);

  //insert sorting code here.
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  parBubbleSort(arr, n, nbthread);

  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;

  
  checkMergeSortResult (arr, n);
  
  std::cerr<<elapsed_seconds.count()<<std::endl;

  delete[] arr;

  return 0;
}
