#include <func.h>

void handle(int num,siginfo_t *p,void *q)
{
    printf("%d is coming\n",num);
    sleep(3);
    printf("sleep over\n");
}

int main()
{
    struct sigaction act;
    act.sa_sigaction = handle;
    act.sa_flags = SA_SIGINFO;
    sigaddset(&act.sa_mask,SIGQUIT);
    sigaction(SIGINT,&act,NULL);
    while(1);

    return 0;
}

