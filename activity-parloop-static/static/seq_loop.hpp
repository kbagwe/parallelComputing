#ifndef __SEQ_LOOP_H
#define __SEQ_LOOP_H

#include <functional>
#include <vector>
#include <thread>
#include <iostream>

class SeqLoop {
private:
  int n;
  int nbthread;

public:
  SeqLoop(int n, int nbthread){
    this->n = n;
    this->nbthread = nbthread;
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

    for(int i=0; i<nbthread; i++){
      TLS tls;
      tlsVec.push_back(tls);
    }
    int nbItr = n / nbthread;

    for(int t=0; t<nbthread; t++){
      before(tlsVec[t]);  
      threadVec.push_back(std::thread([&](size_t beg1, size_t end1, int increment1, TLS& tls) {
              for (size_t i=beg1; i<end1; i+= increment1){					   
                f(i, tls);
              }
     
            }, t * nbItr, (nbthread - 1) == t ? n : (t * nbItr) + nbItr, 1, std::ref(tlsVec[t]))
      );
    }
    for (auto& thread : threadVec){
      thread.join();
    }
    // TLS sum;
    for(auto& s : tlsVec){
      after(s);
    }
  }
  
};

#endif
