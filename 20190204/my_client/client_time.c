#include <func.h>
//按时间打印进度条
int recv_n(int, void*, int);
int main(int argc,char *argv[])
{
    args_check(argc,3);
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
    ser.sin_port=htons(atoi(argv[2]));
    ser.sin_addr.s_addr=inet_addr(argv[1]);
    int ret;
    ret=connect(sfd,(struct sockaddr*)&ser,sizeof(struct sockaddr));
    if(-1==ret)
    {
        perror("connect");
        return -1;
    }
    int dataLen;
    char buf[1000] = {0};
    recv_n(sfd,&dataLen,sizeof(int));
    recv_n(sfd,buf,dataLen);
    int fp = open(buf,O_WRONLY|O_CREAT,0666);
    if(-1 == fp)
    {
        perror("open");
        return -1;
    }
    time_t start,now;
    start = time(NULL);
    off_t totalSize,loadSize = 0;
    recv_n(sfd,&dataLen,sizeof(int));
    recv_n(sfd,&totalSize,dataLen);
    while(1)
    {
        ret = recv_n(sfd,&dataLen,sizeof(int));
        if(-1 == ret)
        {
            printf("child crash\n");
            goto end;
        }
        loadSize += dataLen;
        if(dataLen>0)
        {
            ret = recv_n(sfd,buf,dataLen);
            if(-1 == ret)
            {
                printf("child crash\n");
                goto end;
            }
            write(fp,buf,dataLen);
            now = time(NULL);
            if(now > start)
            {
                printf("%5.2f%s\r",(double)loadSize/totalSize*100,"%");
                fflush(stdout);
                start = now;
            }
        }
        else
        {
            close(fp);
            printf("100.00%s","%");
            printf("Transfer succeed!\n");
            break;
        }
    }
end:
    close(sfd);
    return 0;
}
