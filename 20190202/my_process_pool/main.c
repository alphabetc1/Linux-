#include "process_pool.h"

int main(int argc, char **argv)
{
    args_check(argc,4);
    int sfd;
    tcpInit(&sfd,argv[1],argv[2]);
    printf("sfd is %d\n",sfd);
    int processNum = atoi(argv[3]);
    printf("num:%d\n",processNum);
    pcData *pManage = (pcData*)calloc(processNum,sizeof(pcData));
    makeChild(pManage, processNum);
    struct epoll_event event, *evs;
    evs = (struct epoll_event*)calloc(processNum+1,sizeof(struct epoll_event));
    int epid = epoll_create(1);
    event.events = EPOLLIN;
    event.data.fd = sfd;
    epoll_ctl(epid,EPOLL_CTL_ADD,sfd,&event);
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
        memset(evs,0,(processNum+1)*sizeof(struct epoll_event));
        readyNum = epoll_wait(epid,evs,processNum+1,-1);
        for (i = 0; i < readyNum; i++)
        {
            if(sfd == evs[i].data.fd)
            {
                for(j = 0; j < processNum;j++)
                {
                    if(0 == pManage[j].busy)
                    {
                        new_fd = accept(sfd,NULL,NULL);
                        send_fd(pManage[j].fds,new_fd);
                        pManage[j].busy = 1;
                        close(new_fd);
                        printf("%d is buzy\n", pManage[j].pid);
                        break;
                    }
                }
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
        }

    }
    return 0;
}

