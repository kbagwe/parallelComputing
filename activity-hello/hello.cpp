/*
* Date: 01/30/2021
* Assignment done by,
*       Mukesh Dasari (801208218)
*       Kiran Bagwe (801223392)
*/

#include<iostream>
#include<unistd.h>
using namespace std;
int main(){
  char hostname[256];
  int result = gethostname(hostname, 256);
  cout << hostname;
  return 0;
}
