#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <glob.h>
#include <stdarg.h>
#include "libutils.h"
#include "libll.h"

int main(int argc, char *argv[] )
{
	char tst[1024];
	char buf[8192];
	node joe=NULL;
	node top;
	int j=0;

	for(int i=1;i<20;i++) 
	{
		sprintf(tst,"This is a test of this : count=%d\n",i);
		if (j==0)
			joe=list_add(joe,"test",(void *)tst,sizeof(tst),'s');
		else
			joe=list_add(joe,"",(void *)tst,sizeof(tst),'s');
		j++;
		if (j>1) j=0;
	}
	list_rename(list_find(joe,"test"),"testnew");
	top=list_first(joe);
	while (top!=NULL)
	{
		list_name(top,tst);
		printf("Entry = %s\n",tst);
		top=top->next;
	}
	buf[0]=0;
	list_dump(joe,buf);
	printf("List:\n%s\n",buf);
	while (joe!=NULL)
	{
		joe=list_delete(joe);
		printf("Del Count = %d\n",list_count(joe));
	}
}

