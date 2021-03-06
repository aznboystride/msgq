/**
 * CECS 326 OPERATING SYSTEM
 * receiver1.cpp
 * Purpose: Replies with acknowledgements whenever it receives messages from the senders
 *          sender251 and sender997. This program terminates when both sender251 and sender997
 *          programs have terminated.
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

#define SEND_997 1997

#define SEND_251 1251

#define RECV_997 9971

#define RECV_251 2511

using namespace std;

int main() {

  pid_t thisPid = getpid();

  cout << "This Process's PID: " << thisPid << endl;

  sleep(5);

  struct buf {
    long mtype;
    int num;
    char message[50];
  };

  buf msg;
  const char* message = "Ack from receiver # 1.";
  int qid = msgget(ftok(".", 'u'), IPC_EXCL | IPC_CREAT | 0600);
  int size = sizeof(buf) - sizeof(long);

  bool term997 = false;
  bool term251 = false;
  for(;;) {
    if(!term997) {
      msgrcv(qid, (struct msgbuf*) &msg, size, RECV_997, 0);
      cout << msg.message << " " << msg.num << endl;
      if(msg.num < 100) {
        cout << "Sender 997 terminated" << endl;
        term997 = true;
      }
      msg.mtype = SEND_997;
      strcpy(msg.message, message);
      msgsnd(qid, (struct msgbuf*) &msg, size, 0);
    }
    if(!term251) {
      msgrcv(qid, (struct msgbuf*) &msg, size, RECV_251, 0);
      cout << msg.message << " " << msg.num << endl;
      if(msg.num == -1) {
        cout << "Sender 251 terminated" << endl;
        term251 = true;
      }
      msg.mtype = SEND_251;
      strcpy(msg.message, message);
      msgsnd(qid, (struct msgbuf*) &msg, size, 0);
    }
    if(term251 && term997)
      break;
  }
  msgctl(qid, IPC_RMID, 0);
  return 0;
}

