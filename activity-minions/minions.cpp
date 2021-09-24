#include <iostream>
#include <vector>
#include <thread>
#include <string>

void helloMinion(int minionId){
  minionId>0? std::cout<<"Hello! I am minion "<< minionId << std::endl : std::cout<<"Hello Minions! I am the Overload!"<< std::endl;
}

int main (int argc, char** argv) {
  if (argc < 2) {
    std::cerr<<"usage: "<<argv[0]<<" <nbminions>\n";
    return -1;
  }
  //Your code goes here
  int minionNum = std::stoi(argv[1]);
  std::vector<std::thread> threadVec;
  for(int i=1; i<=minionNum; i++){
    std::thread minionThread (helloMinion, i);
    threadVec.push_back(std::move(minionThread));
  }

  for(auto &j : threadVec){
    if(j.joinable()){
      j.join();
    } else{
      std::cout<<"Thread is not joinable."<<std::endl;
    }
  }

  std::thread lastThread (helloMinion, 0);
  lastThread.join();

  return 0;
}
