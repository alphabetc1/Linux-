#include <assert.h>
#include <string.h>
#include <stdlib.h>
int main(int argc,char* argv[])
{
	int ret;
	ret=strcmp(argv[1],"hello");
	assert(ret);//ret为零，程序就崩溃
	exit(0);
}
