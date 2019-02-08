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
    struct sockaddr_in ser,client;
    memset(&ser,0,sizeof(ser));
    ser.sin_family = AF_INET;
    ser.sin_port = htons(atoi(argv[2]));
    ser.sin_addr.s_addr = inet_addr(argv[1]);
    int ret = bind(sfd,(struct sockaddr*)&ser,sizeof(ser));
    if(-1 == ret)
    {
        perror("bind");
        return -1;
    }
    listen(sfd,10);
    int new_fd;
    memset(&client,0,sizeof(client));
    socklen_t addrlen = sizeof(client);
    new_fd =accept(sfd,(struct sockaddr*)&client,&addrlen);
    if(-1 == new_fd)
    {
        perror("accept");
        return -1;
    }
    ret = recv(new_fd,buf,sizeof(buf),0);
    if(-1 == ret)
    {
        perror("recv");
        return -1;
    }
    printf("i'm servicer,i recv %s\n",buf);
    send(new_fd,"world",5,0);
    close(new_fd);
    close(sfd);

    return 0;
}

