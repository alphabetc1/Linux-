#include <func.h>

void sigfunc(int num)
{
    printf("%d is coming\n",num);
}

int main()
{
    signal(SIGALRM,sigfunc);
    alarm(3);
    printf("Before pause\n");
    pause();
    pause();
    printf("After pause\n");
    return 0;
}

