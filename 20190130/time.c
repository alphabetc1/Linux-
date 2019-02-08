#include <func.h>


int main()
{
    time_t tt;
    time(&tt);
    char *ptime = ctime(&tt);
    printf("now is %s\n",ptime);
    return 0;
}

