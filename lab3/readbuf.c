#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define SHMKEY 7777
#define SEMKEY 9999
int i = 0;
int shmid[20];
int sid = 0;
char *addr[10];
int *len[10];
void P(int semid, int index) {
  struct sembuf sem;
  sem.sem_num = index;
  sem.sem_op = -1;
  sem.sem_flg = 0;
  semop(semid, &sem, 1);
  return;
}
void V(int semid, int index) {
  struct sembuf sem;
  sem.sem_num = index;
  sem.sem_op = 1;
  sem.sem_flg = 0;
  semop(semid, &sem, 1);
  return;
}
void readbuf() {
  int fp = open("./input", O_RDONLY);
  P(sid, 1);
  puts("read begin");
  for (i = 0; (*(len[i]) = read(fp, addr[i], 1024)) != 0;) {
    puts("read!");
    V(sid, 0);
    if (i == 9) {
      P(sid, 1);
      i = 0;
    } else
      i++;
  }
  puts("read over!");
  close(fp);
  return;
}
int main() {
  puts("readbuf begin!");
  for (i = 0; i < 10; i++) {
    shmid[i] = shmget((key_t)(SHMKEY + i), 1024, 0666);
    addr[i] = shmat(shmid[i], 0, 0);
  }
  for (i = 10; i < 20; i++) {
    shmid[i] = shmget((key_t)(SHMKEY + i), sizeof(int), 0666);
    len[i - 10] = shmat(shmid[i], 0, 0);
  }
  sid = semget((key_t)SEMKEY, 2, 0666);
  readbuf();
  for (i = 0; i < 20; i++) {
    shmdt(addr[i]);
  }
  return 0;
}
