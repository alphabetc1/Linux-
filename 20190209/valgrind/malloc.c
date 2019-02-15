#include <stdlib.h>

int main()
{
	void *p;
	p=malloc(20);
	free(p);
	p=NULL;
	return 0;
}
