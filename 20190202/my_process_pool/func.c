#include  "process_pool.h"

int tcpInit(int *socketfd,char *ip, char*port)
{
    int reuse = 1;
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
    ser.sin_port=htons(atoi(port));
    ser.sin_addr.s_addr=inet_addr(ip);
    int ret;
    ret = setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
    ret=bind(sfd,(struct sockaddr*)&ser,sizeof(ser));
    if(-1==ret)
    {
        perror("bind");
        return -1;
    }
    *socketfd = sfd;
    return 0;
}

void makeChild(pcData *pManage, int num)
{
    int i;
    pid_t pid;
    int fds[2];
    for (i = 0; i < num; i++)
    {
        socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
        pid = fork();
        if(!pid)
        {
            close(fds[1]);
            childHandle(fds[0]);
        }
        close(fds[0]);
        pManage[i].pid = pid;
        printf("pid = %d\n",pid);
        pManage[i].fds = fds[1];
    }
}

void childHandle(int fd)
{
    int new_fd;
    char flag = 1;
    while(1)
    {
        recv_fd(fd,&new_fd);
        printf("I'm child,i'm ok\n");
        close(new_fd);
        write(fd,&flag,sizeof(char));
    }
}


void send_fd(int sockpipew,int fd)
{
    struct msghdr msg;
    memset(&msg,0,sizeof(msg));
    char buf1[10] = "hello";
    char buf2[10] = "world";
    struct iovec iov[2];
    iov[0].iov_base = buf1;
    iov[0].iov_len = 5;
    iov[1].iov_base = buf2;
    iov[1].iov_len = 5;
    msg.msg_iov = iov;
    msg.msg_iovlen = 2;
    struct cmsghdr *cmsg;
    int len = CMSG_LEN(sizeof(int));
    cmsg = (struct cmsghdr*)calloc(1,len);
    cmsg->cmsg_len = len;
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    *(int*)CMSG_DATA(cmsg) = fd;
    msg.msg_control = cmsg;
    msg.msg_controllen = len;
    int ret = sendmsg(sockpipew,&msg,0);
    if(-1 == ret)
    {
        perror("sendmsg");
    }
}

void recv_fd(int sockpipew,int *fd)
{
    struct msghdr msg;
    memset(&msg,0,sizeof(msg));
    char buf1[10] = {0};
    char buf2[10] = {0};
    struct iovec iov[2];
    iov[0].iov_base = buf1;
    iov[0].iov_len = 5;
    iov[1].iov_base = buf2;
    iov[1].iov_len = 5;
    msg.msg_iov = iov;
    msg.msg_iovlen = 2;
    struct cmsghdr *cmsg;
    int len = CMSG_LEN(sizeof(int));
    cmsg = (struct cmsghdr*)calloc(1,len);
    cmsg->cmsg_len = len;
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    msg.msg_control = cmsg;
    msg.msg_controllen = len;
    int ret = recvmsg(sockpipew,&msg,0);
    if(-1 == ret)
    {
        perror("recvmsg");
    }
    *fd = *(int*)CMSG_DATA(cmsg);
}


//int main()
//{
//    int fds[2];
//    socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
//    if(!fork())
//    {
//        close(fds[1]);//close write
//        int fd;
//        char buf[128] = {0};
//        recv_fd(fds[0],&fd);
//        printf("child fd = %d\n",fd);
//        read(fd,buf,sizeof(buf)-1);
//        printf("buf is %s\n",buf);
//    }else {
//        close(fds[0]);
//        int fd = open("file1",O_RDWR);
//        if(-1 == fd)
//        {
//            perror("open");
//            return -1;
//        }
//        printf("parent fd = %d\n",fd);
//        send_fd(fds[1],fd);
//        wait(NULL);
//    }
//    return 0;
//}

