#include <func.h>

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


int main()
{
    int fds[2];
    socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
    if(!fork())
    {
        close(fds[1]);//close write
        int fd;
        char buf[128] = {0};
        recv_fd(fds[0],&fd);
        printf("child fd = %d\n",fd);
        read(fd,buf,sizeof(buf)-1);
        printf("buf is %s\n",buf);
    }else {
        close(fds[0]);
        int fd = open("file1",O_RDWR);
        if(-1 == fd)
        {
            perror("open");
            return -1;
        }
        printf("parent fd = %d\n",fd);
        send_fd(fds[1],fd);
        wait(NULL);
    }
    return 0;
}

