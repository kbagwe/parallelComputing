#ifndef __SEQ_LOOP_H
#define __SEQ_LOOP_H

#include <functional>
#include <vector>
#include <thread>
#include <iostream>
#include <mutex>

#include <fstream>
#include <string>
#include <condition_variable>


class SeqLoop {
private:
  int n;
  int nbthread;
  int granularity;
  int nbItr;
  bool done = false;

public:
  SeqLoop(int n, int nbthread, int granularity){
    this->n = n;
    this->nbthread = nbthread;
    this->granularity = granularity;
    this->nbItr = n / granularity;
  }

  /// @brief execute the function f multiple times with different
  /// parameters possibly in parallel
  ///
  /// f will be executed multiple times with parameters starting at
  /// beg, no greater than end, in inc increment. These execution may
  /// be in parallel
  void parfor (size_t beg, size_t end, size_t inc,
	       std::function<void(int)> f) {
    for (size_t i=beg; i<end; i+= inc) {
      f(i);
    }
  }

  /// @brief execute the function f multiple times with different
  /// parameters possibly in parallel
  ///
  /// f will be executed multiple times with parameters starting at
  /// beg, no greater than end, in inc increment. These execution may
  /// be in parallel.
  ///
  /// Each thread that participate in the calculation will have its
  /// own TLS object.
  ///
  /// Each thread will execute function before prior to any
  /// calculation on a TLS object.
  ///
  /// Each thread will be passed the TLS object it executed before on.
  ///
  /// Once the iterations are complete, each thread will execute after
  /// on the TLS object. No two thread can execute after at the same time.
  template<typename TLS>
  void parfor (size_t beg, size_t end, size_t increment,
	       std::function<void(TLS&)> before,
	       std::function<void(int, TLS&)> f,
	       std::function<void(TLS&)> after
	       ) {

    std::vector<TLS> tlsVec;
    std::vector<std::thread> threadVec;
    std::vector<bool> taskFlags(nbItr, false);
    // std::vector<std::mutex> mut;
    std::mutex mut;

    int taskComplete = 0;
    for(int i=0; i<nbItr; i++){
      TLS tls;
      tlsVec.push_back(tls);
      // std::mutex m;
      // mut.push_back(m);
    }

    for(int k=0; k<nbItr; k++){
      before(tlsVec[k]);
    }

    for(int t=0; t<nbthread; t++){
      threadVec.push_back(std::thread([&](int &taskComplete) {   
            do{
              
              bool taskStatus = false;
              auto glambda =  [&](std::vector<bool> &taskFlags, int& taskComplete) -> int{
                  std::lock_guard<std::mutex> lg(mut);

                  if(taskFlags[taskComplete] == false){
                    taskFlags[taskComplete] == true;
                    taskComplete++;
                    return taskComplete-1;
                  } else{
                    return -1;
                  }
              };  
              int startIndex =  glambda(std::ref(taskFlags), std::ref(taskComplete));
              int start = startIndex * granularity;
              bool flag = taskPool(std::ref(start), tlsVec[startIndex], f, increment, std::ref(taskStatus), std::ref(taskComplete));
              while (!taskStatus);
            }while(taskComplete != nbItr && done!= true);
            
					 }, std::ref(taskComplete)));

    }
    for (auto& thread : threadVec){
      thread.join();
    }
       
    for(int l=0; l<nbItr;l++){
      after(tlsVec[l]);
    }
  }

  template<typename TLS>
  bool taskPool(int &start, TLS &tls, std::function<void(int, TLS&)> f, int increment, bool &taskStatus, int& taskComplete){
    if(start == -1 * granularity) return true;
    int end = start + granularity;
    for (size_t i=start; i<end; i+= increment){	                  
        f(i, tls);
    }
    taskStatus = true;
    if(taskComplete == nbItr) {
      done = true;
    }
    return false;
  }
  
};

#endif
