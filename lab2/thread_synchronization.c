#include <linux/sem.h> //包含信号灯操作
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
void *compute_thread(); //计算线程
void *print_thread();   //打印线程
int semid; //信号量
int a = 0; //全局变量，供两个线程使用
pthread_t idOne, idTwo; //两个子线程的线程号
void P(int semid, int index);
void V(int semid, int index);
int main(void) {
  semid = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666); //创建信号量
  semctl(semid, 0, SETVAL, 1);
  semctl(semid, 1, SETVAL, 0);
  pthread_create(&idOne, NULL, compute_thread, NULL);
  pthread_create(&idTwo, NULL, print_thread, NULL);
  pthread_join(idOne, NULL);
  pthread_join(idTwo, NULL);
  putchar('\n');
  semctl(semid, 0, IPC_RMID);
  return 0;
}
void *compute_thread() {
  int i;
  for (i = 1; i < 101; i++) {
    P(semid, 0);
    a = a + i;
    printf("thread 1 counting...\n");
    V(semid, 1);
  }
}
void *print_thread() {
  int i;
  for (i = 1; i < 101; i++) {
    P(semid, 1);
    printf("thread 2 print:%d\n", a);
    V(semid, 0);
  }
}
void P(int semid, int index) {
  struct sembuf sem;
  sem.sem_num = index;
  sem.sem_op = -1;
  sem.sem_flg = 0;       //操作标记：0或IPC_NOWAIT等
  semop(semid, &sem, 1); // 1:表示执行命令的个数
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