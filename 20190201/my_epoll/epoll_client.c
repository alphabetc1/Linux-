#include <func.h>

int main(int argc, char **argv)
{
    args_check(argc,3);
    char buf[128] = {0};
    int sfd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == sfd)
    {
        perror("socket");
        return -1;
    }
    struct sockaddr_in ser;
    memset(&ser,0,sizeof(ser));
    ser.sin_family = AF_INET;
    ser.sin_port = htons(atoi(argv[2]));
    ser.sin_addr.s_addr = inet_addr(argv[1]);
    int ret = connect(sfd,(struct sockaddr*)&ser,sizeof(struct sockaddr));
    if(-1 == ret)
    {
        perror("connect");
        return -1;
    }
    printf("I'm a client\n");
    fd_set fdrd;
    while(1)
    {
        FD_ZERO(&fdrd);
        FD_SET(STDIN_FILENO,&fdrd);
        FD_SET(sfd,&fdrd);
        int ret = select(sfd+1,&fdrd,NULL,NULL,NULL);
        if(ret > 0)
        {
            if(FD_ISSET(STDIN_FILENO,&fdrd))
            {
                memset(buf,0,sizeof(buf));
                ret = read(STDIN_FILENO,buf,sizeof(buf)-1);
                if(0 == ret)
                {
                    printf("close\n");
                }
                send(sfd,buf,strlen(buf)-1,0);
            }
            if(FD_ISSET(sfd,&fdrd))
            {
                memset(buf,0,sizeof(buf));
                ret = recv(sfd,buf,sizeof(buf)-1,0);
                if(0 == ret)
                {
                    printf("Byebye\n");
                    break;
                }
                printf("%s\n",buf);
            }
        }
    }
    close(sfd);
    return 0;
}

