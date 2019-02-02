#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <sys/sem.h>
#include <signal.h>
#include <sys/msg.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <errno.h>
#include <sys/uio.h>
typedef struct {
    pid_t pid;
    short busy;
    int fds;
}pcData;

#define args_check(argc,num) {if(argc!=num) {printf("error args\n");return -1;}}
#define max(a,b) (a)>(b)?(a):(b)

void makeChild(pcData*,int);
void childHandle(int);
int tcpInit(int*,char*,char*);
void recv_fd(int,int*);
void send_fd(int,int);
