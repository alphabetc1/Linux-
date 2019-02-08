#include <func.h>
void* threadfunc(void* p)
{
    time_t t=time(NULL);
    char ptime[128]={0};
    ctime_r(&t,ptime);
    printf("child %s\n",ptime);
    sleep(5);
    printf("child %s\n",ptime);
}

int main()
{
    pthread_t pthid;
    pthread_create(&pthid,NULL,threadfunc,NULL);
    sleep(2);
    time_t t=time(NULL);
    char mtime[128]={0};
    ctime_r(&t,mtime);
    printf("main %s\n",mtime);
    pthread_join(pthid,NULL);
    return 0;
}

