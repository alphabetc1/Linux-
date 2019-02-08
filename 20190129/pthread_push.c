#include <func.h>

void pthclean1(void *p)
{
    printf("i am cleanup1\n");
}

void pthclean2(void *p)
{
    printf("i am cleanup2\n");
}

void *threadfunc(void *p)
{
    pthread_cleanup_push(pthclean1,NULL);
    pthread_cleanup_push(pthclean2,NULL);
    printf("hello world\n");
    pthread_cleanup_pop(1);
    pthread_cleanup_pop(1);
    return NULL;
}

int main()
{
    pthread_t pthid;
    pthread_create(&pthid,NULL,threadfunc,NULL);
    pthread_cancel(pthid);
    sleep(3);
    return 0;
}

