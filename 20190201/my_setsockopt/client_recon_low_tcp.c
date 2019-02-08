#include <func.h>

int main(int argc, char** argv)
{
    args_check(argc,3);
    char buf[128] = {0};
    int ret,value = 10;
    fd_set fdrd;
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
    ret = setsockopt(sfd,SOL_SOCKET,SO_RCVLOWAT,&value,sizeof(int));
    if(-1 == ret)
    {
        perror("setsockopt");
        return -1;
    }
    ret = connect(sfd,(struct sockaddr*)&ser,sizeof(ser));
    if(-1 == ret)
    {
        perror("connect");
        return -1;
    }
    printf("I'm a client\n");
    while(1)
    {
        FD_ZERO(&fdrd);
        FD_SET(STDIN_FILENO,&fdrd);
        FD_SET(sfd,&fdrd);
        ret = select(sfd+1,&fdrd,NULL,NULL,NULL);
        if(ret > 0)
        {
            if(FD_ISSET(STDIN_FILENO,&fdrd))
            {
                memset(buf,0,sizeof(buf));
                read(STDIN_FILENO,buf,sizeof(buf));
                send(sfd,buf,strlen(buf)-1,0);
            }if(FD_ISSET(sfd,&fdrd))
            {
                memset(buf,0,sizeof(buf));
                ret = recv(sfd,buf,sizeof(buf),0);
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

