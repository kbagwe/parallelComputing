#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>

#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif

float charToFloat(char* str){
  char *remaining;
  float number = strtof(str, &remaining);
  if (str == remaining){
    std::cerr<<"Non numeric character in input!"<<std::endl;
    exit(0);
  } 
  return number;
}
  
float calculateIntegral(float functionId, float a, float b, float n, float intensity){
  float firstPartOfEquation = (b-a)/n;
  float functionResponse = 0;
  for (int i=0;i<n;i++){
    float secondPartOfEquation = a + ((i + 0.5) * firstPartOfEquation);
    switch ((int) functionId){
    case 1:
      functionResponse += f1(secondPartOfEquation,intensity);
      break;
    case 2:
      functionResponse += f2(secondPartOfEquation,intensity);
      break;
    case 3:
      functionResponse += f3(secondPartOfEquation,intensity);
      break;
    case 4:
      functionResponse += f4(secondPartOfEquation,intensity);
      break;
    default:
      std::cerr<<"Function ID is incorrect."<<std::endl;
      exit(0);
    }
  }
  return (firstPartOfEquation*functionResponse);
}
int main (int argc, char* argv[]) {
  if (argc < 6) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }
  float functionId = charToFloat(argv[1]);;
  float a = charToFloat(argv[2]);
  float b = charToFloat(argv[3]);
  int n = charToFloat(argv[4]);
  int intensity = charToFloat(argv[5]);
  std::chrono::system_clock::time_point startTime = std::chrono::system_clock::now();
  float response = (float) calculateIntegral(functionId, a, b, n, intensity);
  std::chrono::system_clock::time_point endtime = std::chrono::system_clock::now();
  std::chrono::duration<double, std::milli> difference = endtime - startTime;
  std::cout<< response<<std::endl;
  std::cerr << difference.count() << std::endl;
  return 0;
}
