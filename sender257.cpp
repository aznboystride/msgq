/**
 * CECS 326 OPERATING SYSTEM
 * sender257.cpp
 * Purpose: Sends random 32-bit unsigned integers to the second receiver program
 *          and terminates when the receiver program terminates.
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

#define SEND_RECV2 2572

#define RECV_RECV2 2257

using namespace std;

int main() {
  
  pid_t thisPid = getpid();

  uniform_int_distribution<int> distribution(0, UINT_MAX); // Distribution Range: [0 - 2^32-1)
  mt19937 generator(random_device()()); // Mersenne Twister Pseudorandom Generator

  cout << "This Process's PID: " << thisPid << endl;

  sleep(5);

  struct buf {
    long mtype;
    int num;
    char message[50];
  };

  buf msg;
  const char* message = "Hello from sender 257.";
  int qid = msgget(ftok(".", 'u'), 0);
  int size = sizeof(buf) - sizeof(long);

  int random_num;
  for(;;) {
    random_num = distribution(generator);
    if(random_num % 257 == 0) {
      msg.mtype = SEND_RECV2;
      msg.num = random_num;
      strcpy(msg.message, message);
      msgsnd(qid, (struct msgbuf*) &msg, size, 0);
      msgrcv(qid, (struct msgbuf*) &msg, size, RECV_RECV2, 0);
      cout << msg.message << " " << msg.num << endl;
      if(msg.num == -1) {
          break;
      }
    } 
  }
  
  return 0;
}

