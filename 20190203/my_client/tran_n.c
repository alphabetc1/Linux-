#include <func.h>

int send_n(int fd, void *vbuf,int len)
{
    char *pbuf = (char*)vbuf;
    int total= 0, ret;
    while(total < len)
    {
        ret = send(fd,pbuf+total,len - total,0);
        total += ret;
    }
    return total;
}

int recv_n(int fd, void *vbuf,int len)
{
    char *pbuf = (char*)vbuf;
    int total= 0, ret;
    while(total < len)
    {
        ret = recv(fd,pbuf+total,len - total,0);
        if(0 == ret)
        {
            return -1;
        }
        total += ret;
    }
    return total;
}

