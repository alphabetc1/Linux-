#include <func.h>

int main(int argc, char **argv)
{
    args_check(argc,3);
    char buf[8] = {0};
    int sfd = socket(AF_INET,SOCK_DGRAM,0);
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
    socklen_t addrlen = sizeof(struct sockaddr);
    int ret = bind(sfd,(struct sockaddr*)&ser,sizeof(ser));
    if(-1 == ret)
    {
        perror("bind");
        return -1;
    }
    memset(&client,0,sizeof(client));
    recvfrom(sfd,buf,sizeof(buf)-1,0,(struct sockaddr*)&client,&addrlen);
    printf("I'm a server,i receive%s\n",buf);
    fd_set fdrd;
    while(1)
    {
        FD_ZERO(&fdrd);
        FD_ISSET(STDIN_FILENO,&fdrd);
        FD_ISSET(sfd,&fdrd);
        ret = select(sfd+1,&fdrd,NULL,NULL,NULL);
        if(ret > 0)
        {
            if(FD_ISSET(STDIN_FILENO,&fdrd))
            {
                memset(buf,0,sizeof(buf));
                read(STDIN_FILENO,buf,sizeof(buf)-1);
                sendto(sfd,buf,strlen(buf)-1,0,(struct sockaddr*)&client,sizeof(client));
            }
            if(FD_ISSET(sfd,&fdrd))
            {
                memset(buf,0,sizeof(buf));
                recvfrom(sfd,buf,sizeof(buf)-1,0,(struct sockaddr*)&client,&addrlen);
                printf("%s\n",buf);
            }
        }
    }
    close(sfd);
    return 0;
}

