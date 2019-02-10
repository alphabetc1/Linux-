#ifndef __TRANFILE_H__
#define __TRANFILE_H__
#include "head.h"
typedef struct{
    int dataLen;
    char buf[1000];
}train;
#define DOWNFILE "file"
int tranFile(int);
int send_n(int,char*,int);
int recv_n(int,char*,int);
#endif
