#include "process_pool.h"

int transFile(int fds)
{
    train t;
    int ret;
    struct stat statbuf;
    strcpy(t.buf,FILENAME);
    t.dataLen = strlen(FILENAME);
    ret = send_n(fds,&t,4+t.dataLen);
    if(-1 == ret)
    {
        close(fds);
        printf("child close\n");
        return -1;
    }
    int fp = open(FILENAME,O_RDONLY);
    fstat(fp,&statbuf);
    memcpy(t.buf,&statbuf.st_size,sizeof(t.buf));
    t.dataLen = sizeof(statbuf.st_size);
    ret = send_n(fds,&t,4+t.dataLen);
    if(-1 == ret)
    {
        close(fds);
        printf("child close\n");
        return -1;
    }
    while((t.dataLen = read(fp,t.buf,sizeof(t.buf))))
    {
        ret = send_n(fds,&t,4+t.dataLen);
        if(-1 == ret)
        {
            close(fds);
            printf("child close\n");
            return -1;
        }
    }
    send_n(fds,&t,4+t.dataLen);
    close(fds);
    return 0;
}

