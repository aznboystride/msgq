#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <string.h>
#include <random> 
#include <signal.h>
#include <ctime>
#include <cerrno>
#include <thread>
#include "get_info.h"

#define SEND_RECV1 2511

#define RECV_RECV1 1251

using namespace std;

int main() {
  
  pid_t thisPid = getpid();

  uniform_int_distribution<> rng(0, 2147483647);
  mt19937 gen;
  gen.seed(random_device()());
  sleep(5);
  cout << "This Process's PID: " << thisPid << endl;

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
  get_info(qid, (msgbuf*) &msg, size, (long) SEND_RECV1);
  for(;;) {
    random_num = rng(gen);
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

