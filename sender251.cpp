/**
 * CECS 326 OPERATING SYSTEM
 * sender251.cpp
 * Purpose: Sends random 32-bit unsigned numbers to the first receiver program
 *          and terminates after receiving an SIGUSR1 signal.
 */

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>
#include <random> 
#include <ctime>
#include <climits>
#include "get_info.h"

#define SEND_RECV1 2511

#define RECV_RECV1 1251

using namespace std;

int main() {
  
  pid_t thisPid = getpid();

  random_device rd;
  uniform_int_distribution<int> distribution(0, INT_MAX); // Distribution Range: [0 - 2^31-1)
  mt19937 generator(rd()); // Mersenne Twister Pseudorandom Generator
  
  cout << "This Process's PID: " << thisPid << endl;

  sleep(5);

  struct buf {
    long mtype;
    int num;
    char message[50];
  };

  buf msg;
  const char* message = "Hello from sender 251.";
  int qid = msgget(ftok(".", 'u'), 0);
  int size = sizeof(buf) - sizeof(long);

  int random_num;
  msg.num = -1;
  strcpy(msg.message, message);
  get_info(qid, (msgbuf*) &msg, size, SEND_RECV1);
  for(;;) {
    random_num = distribution(generator);
    if(random_num % 251 == 0) {
      msg.mtype = SEND_RECV1;
      msg.num = random_num;
      strcpy(msg.message, message);
      msgsnd(qid, (struct msgbuf*) &msg, size, 0);
      msgrcv(qid, (struct msgbuf*) &msg, size, RECV_RECV1, 0);
      cout << msg.message << " " << msg.num << endl;
    } 
  }
  
  return 0;
}

