/**
 * CECS 326 OPERATING SYSTEM
 * sender997.cpp
 * Purpose: Sends random 32-bit unsigned integers to the first receiver program and the second receiver program
 *          and terminates after observing a random number less than 100.
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

#define SEND_RECV1 9971

#define SEND_RECV2 9972

#define RECV_RECV1 1997

#define RECV_RECV2 2997

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
  const char* message = "Hello from sender 997.";
  int qid = msgget(ftok(".", 'u'), 0);
  int size = sizeof(buf) - sizeof(long);

  int random_num;
  bool term2 = false;

  for(;;) {
    random_num = distribution(generator);
    if(random_num % 997 == 0 || random_num < 100) {
      msg.mtype = SEND_RECV1;
      msg.num = random_num;
      strcpy(msg.message, message);
      msgsnd(qid, (struct msgbuf*) &msg, size, 0);
      msgrcv(qid, (struct msgbuf*) &msg, size, RECV_RECV1, 0);
      cout << msg.message << " " << msg.num << endl;
      if(!term2) {
        msg.mtype = SEND_RECV2;
        msg.num = random_num;
        strcpy(msg.message, message);
        msgsnd(qid, (struct msgbuf*) &msg, size, 0);
        msgrcv(qid, (struct msgbuf*) &msg, size, RECV_RECV2, 0);
        if(msg.num == -1)
          term2 = true;
        cout << msg.message << " " << msg.num << endl;
      }    
      if(random_num < 100) {
        cout << "Observed number less than 100: " << random_num << endl;
        break;
      }
    }
  }
  
  return 0;
}
