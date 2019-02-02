#include <func.h>

int main(int argc, char **argv)
{
    args_check(argc,3);
    char buf[5] = {0};
    int new_fd = -1;
    int sfd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == sfd)
    {
        perror("socket");
        return -1;
    }
    struct sockaddr_in ser,client;
    memset(&ser,0,sizeof(ser));
    ser.sin_family = AF_INET;
    ser.sin_port = htons(atoi(argv[2]));
    ser.sin_addr.s_addr = inet_addr(argv[1]);
    socklen_t addrlen = sizeof(client);
    int ret = bind(sfd,(struct sockaddr*)&ser,sizeof(ser));
    if(-1 == ret)
    {
        perror("bind");
        return -1;
    }
    listen(sfd,10);
    memset(&client,0,sizeof(client));
    printf("I'm a server\n");
    int epid = epoll_create(1);
    struct epoll_event event,eve[3];
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = sfd;
    ret = epoll_ctl(epid,EPOLL_CTL_ADD,sfd,&event);
    if(-1 == ret)
    {
        perror("epoll_ctl1");
        return -1;
    }
    event.data.fd = STDIN_FILENO;
    ret = epoll_ctl(epid,EPOLL_CTL_ADD,STDIN_FILENO,&event);
    if(-1 == ret)
    {
        perror("epoll_ctl1");
        return -1;
    }
    int i, readNum;
    while(1)
    {
        memset(eve,0,sizeof(eve));
        readNum = epoll_wait(epid,eve,2,-1);
        for (i = 0; i < readNum; i++)
        {
            if(sfd == eve[i].data.fd)
            {
                new_fd = accept(sfd,(struct sockaddr*)&client,&addrlen);
                if(-1 == new_fd)
                {
                    perror("accept");
                    return -1;
                }
                event.events = EPOLLIN | EPOLLET;
                event.data.fd = new_fd;
                ret = epoll_ctl(epid,EPOLL_CTL_ADD,new_fd,&event);
                if(-1 == ret)
                {
                    perror("epoll_ctl2");
                    return -1;
                }

            }
            if(STDIN_FILENO == eve[i].data.fd)
            {
                memset(buf,0,sizeof(buf));
                ret = read(STDIN_FILENO,buf,sizeof(buf)-1);
                if(0 == ret)
                {
                    printf("Close\n");
                    goto end;
                }
                send(new_fd,buf,strlen(buf)-1,0);
            }
            if(new_fd == eve[i].data.fd)
            {
                memset(buf,0,sizeof(buf));
                ret =recv(new_fd,buf,sizeof(buf)-1,0);
                if(0 == ret)
                {
                    printf("Client close,byebye\n");
                    epoll_ctl(epid,EPOLL_CTL_DEL,new_fd,&event );
                    break;
                }
                printf("%s\n",buf);
            }
        }
    }
end:
    close(epid);
    close(new_fd);
    close(sfd);
    return 0;
}

