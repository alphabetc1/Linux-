#include  "process_pool.h"

int tcpInit(int *socketfd,char *ip, char*port)
{
    int reuse = 1;
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    if(-1==sfd)
    {
        perror("socket");
        return -1;
    }
    printf("sfd=%d\n",sfd);
    struct sockaddr_in ser;
    memset(&ser,0,sizeof(ser));
    ser.sin_family=AF_INET;
    ser.sin_port=htons(atoi(port));
    ser.sin_addr.s_addr=inet_addr(ip);
    int ret;
    ret = setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
    ret=bind(sfd,(struct sockaddr*)&ser,sizeof(ser));
    if(-1==ret)
    {
        perror("bind");
        return -1;
    }
    *socketfd = sfd;
    return 0;
}

void makeChild(pcData *pManage, int num)
{
    int i;
    pid_t pid;
    int fds[2];
    for (i = 0; i < num; i++)
    {
        socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
        pid = fork();
        if(!pid)
        {
            close(fds[1]);
            childHandle(fds[0]);
        }
        close(fds[0]);
        pManage[i].pid = pid;
        printf("pid = %d\n",pid);
        pManage[i].fds = fds[1];
    }
}

void childHandle(int fd)
{
    int new_fd;
    char flag = 1;
    while(1)
    {
        recv_fd(fd,&new_fd);
        if(-1 == new_fd)
        {
            exit(0);
        }
        transFile(new_fd);
        printf("I'm child,i'm ok\n");
        write(fd,&flag,sizeof(char));
    }
}


