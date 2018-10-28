/**
 * CECS 326 OPERATING SYSTEM
 * receiver2.cpp
 * Purpose: Replies with acknowledgements whenever it receives messages from the senders
 *          sender257 and sender997. This program terminates after receiving a total
 *          of 5000 messages.
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

#define SEND_997 2997

#define SEND_257 2257

#define RECV_997 9972

#define RECV_257 2572

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
  const char* message = "Ack from receiver # 2.";
  int qid = msgget(ftok(".", 'u'), 0);
  int size = sizeof(buf) - sizeof(long);

  bool term997 = false;
  for(int msgcount = 0; msgcount < 5000; msgcount++) {
    if(!term997) {
      msgrcv(qid, (struct msgbuf*) &msg, size, RECV_997, 0);
      cout << msg.message << " " << msg.num << endl;
      if(msg.num < 100) {
        cout << "Sender 997 terminated" << endl;
        term997 = true;
      }
      msg.mtype = SEND_997;
      msg.num = msgcount == 4999 ? -1 : msg.num;
      strcpy(msg.message, message);
      msgsnd(qid, (struct msgbuf*) &msg, size, 0);
    }
    msgrcv(qid, (struct msgbuf*) &msg, size, RECV_257, 0);
    cout << msg.message << " " << msg.num << endl;
    msg.mtype = SEND_257;
    msg.num = (msgcount == 4999 ? -1 : msg.num);
    strcpy(msg.message, message);
    msgsnd(qid, (struct msgbuf*) &msg, size, 0);
  }
  cout << "Received 5000 messages" << endl;
  
  return 0;
}


