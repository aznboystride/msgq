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

#define SEND_RECV1 9971

#define SEND_RECV2 9972

#define RECV_RECV1 1997

#define RECV_RECV2 2997

#define INT_MAX 2147483647

using namespace std;

int main() {
  sleep(5);
  pid_t thisPid = getpid();

  uniform_int_distribution<> rng(0, INT_MAX);
  mt19937 gen;
  gen.seed(random_device()());


  cout << "This Process's PID: " << thisPid << endl;

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
    random_num = rng(gen);
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
