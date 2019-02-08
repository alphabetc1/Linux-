#include <func.h>
#define N 20

typedef struct {
    pthread_cond_t cond;
    pthread_mutex_t t;
    long num;
}MUTEX;

void* threadfunc1(void *p)
{
    int num = 0;
    MUTEX *q = (MUTEX*)p;
    while(q->num >0)
    {
        num++;
        printf("i'm windows1,going to sell tickets%ld\n",q->num);
        pthread_mutex_lock(&q->t);
        if(q->num>0)
        {
            q->num--;
            printf("Now left%ld\n",q->num);
            if(0 == q->num)
                pthread_cond_signal(&q->cond);
            pthread_mutex_unlock(&q->t);
        }else {
            pthread_mutex_unlock(&q->t);
            break;
        }
        sleep(1);
    }
    printf("Windows1 sold%d\n",num);
    pthread_exit(NULL);
}

void* threadfunc2(void *p)
{
    int num = 0;
    MUTEX *q = (MUTEX*)p;
    while(q->num >0)
    {
        num++;
        printf("i'm windows2,going to sell tickets%ld\n",q->num);
        pthread_mutex_lock(&q->t);
        if(q->num>0)
        {
            q->num--;
            printf("Now left%ld\n",q->num);
            if(0 == q->num)
                pthread_cond_signal(&q->cond);
            pthread_mutex_unlock(&q->t);
        }else {
            pthread_mutex_unlock(&q->t);
            break;
        }
        sleep(1);
    }
    printf("Windows2 sold%d\n",num);
    pthread_exit(NULL);
}

void* soldtickets(void *p)
{
    MUTEX *q = (MUTEX*)p;
    if(q->num > 0)
    {
        pthread_mutex_lock(&q->t);
        pthread_cond_wait(&q->cond,&q->t);
        q->num = 10;
        pthread_mutex_unlock(&q->t);
    }
    return NULL;
}

int main()
{
    MUTEX mux;
    mux.num = N;
    pthread_t pthid1,pthid2,pthid3;
    pthread_mutex_init(&mux.t,NULL);
    pthread_cond_init(&mux.cond,NULL);
    pthread_create(&pthid1,NULL,threadfunc1,&mux);
    pthread_create(&pthid2,NULL,threadfunc2,&mux);
    pthread_create(&pthid3,NULL,soldtickets,&mux);
    pthread_join(pthid1,NULL);
    pthread_join(pthid2,NULL);
    pthread_join(pthid3,NULL);
    pthread_mutex_destroy(&mux.t);
    pthread_cond_destroy(&mux.cond);
    return 0;
}

