#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#define SHMKEY 7777
#define SEMKEY 9999
int i = 0;
int shmid[20];
int sid = 0;
union semunarg {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *_buf;
} arg;
int main() {
  sid = semget((key_t)SEMKEY, 2, IPC_CREAT | 0666);
  if (sid == -1)
    return -1;
  arg.val = 0;
  semctl(sid, 0, SETVAL, arg);
  arg.val = 1;
  semctl(sid, 1, SETVAL, arg);
  for (i = 0; i < 10; i++) {
    shmid[i] = shmget((key_t)(SHMKEY + i), 1024, 0666 | IPC_CREAT);
  }
  for (i = 10; i < 20; i++) {
    shmid[i] = shmget((key_t)(SHMKEY + i), sizeof(int), 0666 | IPC_CREAT);
  }
  int p1, p2;
  if ((p1 = fork()) == 0) {
    puts("Create readbuf!\n");
    execv("./readbuf", NULL);
  } else if ((p2 = fork()) == 0) {
    puts("Create writebuf!\n");
    execv("./writebuf", NULL);
  }
  wait(0);
  wait(0);
  semctl(sid, 0, IPC_RMID);
  for (i = 0; i < 20; i++) {
    shmctl(shmid[i], IPC_RMID, 0);
  }
  return 0;
}
