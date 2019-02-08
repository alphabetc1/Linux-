#include <func.h>
void* threadfunc(void* p)
{
    time_t t=time(NULL);
    char *ptime=ctime(&t);
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
    char *mtime=ctime(&t);
    printf("main %s\n",mtime);
    pthread_join(pthid,NULL);
    return 0;
}

