#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
void HandleFunc(int sigNum);
pid_t pid1, pid2; //必须放到全局变量里，因为要给这两个子进程发送信号
int main(void) {
  int count = 1;     //发送次数
  int pipefd[2];     //用于两个子进程通信
  char sendBuf[100]; //实验中"i send you x times"，100够用了吧
  char readBuf[100]; //读缓冲
  pipe(pipefd);      //创建无名管道
  pid1 = fork();
  if (pid1 == 0) { //子进程1
    signal(SIGUSR1, HandleFunc);
    signal(SIGINT, SIG_IGN); //或者像下面这样写
    // signal(SIGINT,1);           //function=1指忽略该类信号
    while (1) {
      sprintf(sendBuf, "I send you %d times\n", count);
      write(pipefd[1], sendBuf, sizeof(sendBuf));
      sleep(1);
      count++;
    }
  } else {
    pid2 = fork();
    if (pid2 == 0) { //子进程2
      signal(SIGUSR2, HandleFunc);
      signal(SIGINT, SIG_IGN); //或者像下面这样写
      // signal(SIGINT,1);           //function=1指忽略该类信号
      while (1) {
        read(pipefd[0], readBuf, sizeof(readBuf));
        sleep(1);
        printf("%s", readBuf);
      }
    } else {                      //父进程
      signal(SIGINT, HandleFunc); //收到软中断信号，处理该信号
      wait(&pid1);
      wait(&pid2);
      close(pipefd[0]);
      close(pipefd[1]);
      printf("parent process is killed!\n");
      return 0;
    }
  }
}
void HandleFunc(int sigNum) {
  if (SIGUSR1 == sigNum) {
    printf("Child process 1 is killed by parent\n");
    exit(0);
  }
  if (SIGUSR2 == sigNum) {
    printf("Child process 2 is killed by parent\n");
    exit(0);
  }
  if (SIGINT == sigNum) { //收到软中断信号，发送sigusr1和sigusr2
    //    signal(SIGUSR1,HandleFunc);
    //    signal(SIGUSR2,HandleFunc);
    //函数写错了，signal是用于信号处理的，发送信号应该用kill
    kill(pid1, SIGUSR1);
    kill(pid2, SIGUSR2);
    //    exit(0);        //还不能exit退出程序，因为还要回到main函数
    return;
  }
}
