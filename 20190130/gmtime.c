#include <func.h>

void *sigfunc(void *q)
{
    time_t tt;
    struct tm *p;
    time(&tt);
    p = gmtime(&tt);
    printf("chind:%04d-%02d-%02d %02d:%02d:%02d\n",1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min,p->tm_sec);
    sleep(5);
    printf("child:%04d-%02d-%02d %02d:%02d:%02d\n",1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min,p->tm_sec);
    pthread_exit(NULL);
}

int main()
{
    pthread_t pthid;
    time_t tt;
    struct tm *p;
    pthread_create(&pthid,NULL,sigfunc,NULL);
    sleep(2);
    time(&tt);
    p = gmtime(&tt);
    printf("main:%04d-%02d-%02d %02d:%02d:%02d\n",1900+p->tm_year,1+p->tm_mon,p->tm_mday,8+p->tm_hour,p->tm_min,p->tm_sec);

    pthread_join(pthid,NULL);
    return 0;
}

