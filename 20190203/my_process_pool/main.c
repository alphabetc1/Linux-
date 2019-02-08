#include "process_pool.h"
int exitFds[2];
void sigfunc(int num)
{
    write(exitFds[1],&num,1);
}
int main(int argc, char **argv)
{
    pipe(exitFds);
    signal(SIGUSR1,sigfunc);
    args_check(argc,4);
    int sfd;
    tcpInit(&sfd,argv[1],argv[2]);
    printf("sfd is %d\n",sfd);
    int processNum = atoi(argv[3]);
    printf("num:%d\n",processNum);
    pcData *pManage = (pcData*)calloc(processNum,sizeof(pcData));
    makeChild(pManage, processNum);
    struct epoll_event event, *evs;
    evs = (struct epoll_event*)calloc(processNum+2,sizeof(struct epoll_event));
    int epid = epoll_create(1);
    event.events = EPOLLIN;
    event.data.fd = sfd;
    epoll_ctl(epid,EPOLL_CTL_ADD,sfd,&event);
    event.data.fd = exitFds[0];
    epoll_ctl(epid,EPOLL_CTL_ADD,exitFds[0],&event);
    int i;
    for (i = 0; i < processNum; i++)
    {
        event.data.fd = pManage[i].fds;
        epoll_ctl(epid,EPOLL_CTL_ADD,pManage[i].fds,&event);
    }
    int readyNum,new_fd,j;
    char flag = 1;
    listen(sfd,10);
    while(1)
    {
        memset(evs,0,(processNum+2)*sizeof(struct epoll_event));
        readyNum = epoll_wait(epid,evs,processNum+2,-1);
        for (i = 0; i < readyNum; i++)
        {
            if(sfd == evs[i].data.fd)
            {//有进程要进来
                for(j = 0; j < processNum;j++)
                {
                    if(0 == pManage[j].busy)
                    {
                        new_fd = accept(sfd,NULL,NULL);
                        send_fd(pManage[j].fds,new_fd);
                        pManage[j].busy = 1;
                        printf("%d is buzy\n", pManage[j].pid);
                        break;
                    }
                }
                close(new_fd);
            }
            for (j = 0; j < processNum;j++)
            {
                if(pManage[j].fds == evs[i].data.fd)
                {
                    read(pManage[j].fds,&flag,sizeof(char));
                    pManage[j].busy = 0;
                    printf("%d is not buzy\n", pManage[j].pid);
                    break;
                }
            }
            if(evs[i].data.fd == exitFds[0])
            {
                event.events = EPOLLIN;
                event.data.fd = sfd;
                epoll_ctl(epid,EPOLL_CTL_DEL,sfd,&event);
                for (j = 0; j < processNum;j++)
                {
                    send_fd(pManage[j].fds,-1);
                }
                for (j = 0; j < processNum;j++)
                {
                    wait(NULL);
                    printf("child exit success!\n");
                }
                exit(0);
            }
        }

    }
    return 0;
}

