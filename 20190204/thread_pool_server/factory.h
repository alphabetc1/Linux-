#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "head.h"
#include "work_que.h"
#include "tranfile.h"
typedef void* (*threadfunc_t)(void*);
typedef struct{
    pthread_t *pthid;
    int threadNum;
    Que_t que;
    pthread_cond_t cond;
    threadfunc_t downFileFunc;
    short startFlag;
}Factory,*pFactory;
void factoryInit(pFactory pf,int threadNum,int capacity,threadfunc_t threadfunc);
void factoryStart(pFactory);
int tcpInit(int *sfd,char* ip,char* port);
#endif
