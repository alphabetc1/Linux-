#include <func.h>
#define N 10000000

typedef struct{
    pthread_mutex_t t;
    long num;
}MUTEX;

void *threadfunc2(void *p)
{
    MUTEX *q = (MUTEX*)p;
    int i = 0;
    while(q->num > 0)
    {
        pthread_mutex_lock(&q->t);
        if(q->num > 0)
        {
            i++;
            q->num--;
            pthread_mutex_unlock(&q->t);
        }else {
            pthread_mutex_unlock(&q->t);
            printf("%d\n",i);
            return NULL;
        }
    }
    printf("%d\n",i);
    return NULL;

}
void *threadfunc(void *p)
{
    MUTEX *q = (MUTEX*)p;
    int i = 0;
    while(q->num > 0)
    {
        pthread_mutex_lock(&q->t);
        if(q->num > 0)
        {
            i++;
            q->num--;
            pthread_mutex_unlock(&q->t);
        }else {
            pthread_mutex_unlock(&q->t);
            printf("%d\n",i);
            return NULL;
        }
    }
    printf("%d\n",i);
    return NULL;
}

int main()
{
    MUTEX mux;
    mux.num = N;
    pthread_t pthid1,pthid2;
    pthread_mutex_init(&mux.t,NULL);
    pthread_create(&pthid1,NULL,threadfunc,&mux);
    pthread_create(&pthid2,NULL,threadfunc2,&mux);
    pthread_join(pthid1,NULL);
    pthread_join(pthid2,NULL);
    pthread_mutex_destroy(&mux.t);
    printf("%ld\n",mux.num);
    return 0;
}

