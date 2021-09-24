#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <vector>
#include<omp.h>

#ifdef __cplusplus
extern "C" {
#endif

  void generateLCS(char* X, int m, char* Y, int n);
  void checkLCS(char* X, int m, char* Y, int n, int result);

#ifdef __cplusplus
}
#endif


int LCS(char* X, int m, char* Y, int n, int nbthread) {
  omp_set_num_threads(nbthread);
  int high = std::max(n,m);
  int C[high+1][high+1];

  #pragma omp parallel
  {
    #pragma omp for
    for (int i=0; i<=high; ++i) {
      C[i][0] = 0;
    }

    #pragma omp for
    for (int j=0; j<=high; ++j) {
      C[0][j] = 0;
    }

    #pragma omp parallel
    {
      #pragma omp single
      {
        int grain = n/nbthread;
        for(int k=1; k<=m;k++) {
          if (X[k-1] == Y[k-1]){
            C[k][k] = C[k-1][k-1] + 1;
          }else{
              C[k][k] = std::max(C[k][k-1],C[k-1][k]);
          }
          #pragma omp task shared(X , Y, C, k, high)
          {
            #pragma omp parallel for schedule(guided,grain)
            for(int j = k; j<=high;j++){
              if (X[k-1] == Y[j]){
                      C[k][j] = C[k-1][j-1] + 1;
              }else{
                  C[k][j] = std::max(C[k][j-1],C[k-1][j]);
              }
            }
          }
          #pragma omp task shared(X, Y, C, k, high) 
          { 
            #pragma omp parallel for schedule(guided,grain)
            for(int i = k;i<=high;i++){
                if (X[i] == Y[k-1]){
                        C[i][k] = C[i-1][k-1] + 1;
                }else{
                    C[i][k] = std::max(C[i][k-1],C[i-1][k]);
                }
            }
          }
          #pragma omp taskwait
        }
      }
    }
  }

  return C[m][n];
}


int main (int argc, char* argv[]) {

  if (argc < 4) { std::cerr<<"usage: "<<argv[0]<<" <m> <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int m = atoi(argv[1]);
  int n = atoi(argv[2]);
  int nbthread = atof(argv[3]);

  // get string data 
  char *X = new char[m];
  char *Y = new char[n];
  generateLCS(X, m, Y, n);

  
  //insert LCS code here.
  // int result = -1; // length of common subsequence

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  int lcs = LCS(X, m, Y, n, nbthread);

  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elpased_seconds = end-start;

  checkLCS(X, m, Y, n, lcs);
  std::cerr<<elpased_seconds.count()<<std::endl;

  return 0;
}
