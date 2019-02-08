#include <func.h>

int main(int argc, char** argv)
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
    int ret = connect(sfd,(struct sockaddr*)&ser,sizeof(ser));
    if(-1 == ret)
    {
        perror("connect");
        return -1;
    }
    send(sfd,"hello",5,0);
    recv(sfd,buf,sizeof(buf),0);
    printf("I'm client,i recv %s\n",buf);
    close(sfd);
    return 0;
}
