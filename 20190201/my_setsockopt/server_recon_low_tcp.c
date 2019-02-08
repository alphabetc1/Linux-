#include <func.h>

int main(int argc, char **argv)
{
    args_check(argc,3);
    char buf[128];
    int new_fd,ret,sfd,value = 10,reuse = 1;
    sfd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == sfd)
    {
        perror("socket");
        return -1;
    }
    ret = setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
    if(-1 == ret)
    {
        perror("setsockopt");
        return -1;
    }
    struct sockaddr_in ser,client;
    memset(&ser,0,sizeof(ser));
    ser.sin_family = AF_INET;
    ser.sin_port = htons(atoi(argv[2]));
    ser.sin_addr.s_addr = inet_addr(argv[1]);
    ret = bind(sfd,(struct sockaddr *)&ser,sizeof(ser));
    if(-1 == ret)
    {
        perror("bind");
        return -1;
    }
    listen(sfd,10);
    socklen_t addrlen = sizeof(client);
    fd_set rdset,recordset;
    FD_SET(STDIN_FILENO,&recordset);
    FD_SET(sfd,&recordset);
    
    while(1)
    {
        FD_ZERO(&rdset);
        memcpy(&rdset,&recordset,sizeof(fd_set));
        ret = select(20,&rdset,NULL,NULL,NULL);
        if(ret > 0)
        {
            if(FD_ISSET(sfd,&rdset))
            {
                memset(&client,0,sizeof(client));
                new_fd = accept(sfd,(struct sockaddr*)&client,&addrlen);
                if(-1 == new_fd)
                {
                    perror("accept");
                    return -1;
                }
                //ret = setsockopt(new_fd,SOL_SOCKET,SO_RCVLOWAT,&value,sizeof(int));
                if(-1 == ret)
                {
                    perror("setsockopt");
                    return -1;
                }
                printf("new_fd:%d, client ip = %s, port = %d\n",new_fd,inet_ntoa(client.sin_addr),ntohs(client.sin_port));
                FD_SET(new_fd,&recordset);
            }
            if(FD_ISSET(STDIN_FILENO,&rdset))
            {
                memset(buf,0,sizeof(buf));
                read(STDIN_FILENO,buf,sizeof(buf)-1);
                send(new_fd,buf,strlen(buf)-1,0);
            }
            if(FD_ISSET(new_fd,&rdset))
            {
                memset(buf,0,sizeof(buf));
                ret = recv(new_fd,buf,sizeof(buf)-1,0);
                if(0 == ret)
                {
                    printf("Byebye\n");
                    FD_CLR(new_fd,&recordset);
                    close(new_fd);
                }
                printf("%s\n",buf );
            }
        }
    }
    close(sfd);
    close(new_fd);
    return 0;
}

