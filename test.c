#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <glob.h>
#include <stdarg.h>
#include "libutils.h"
#include "libll.h"
#include "drbddigeststats.h"

int main(int argc, char *argv[] )
{
	node joe=NULL;
	node temp1;
	node * temp2;
	char * buff;

	buff=malloc(16000);
	buff[0]=0;

	drbd_digest_stats(&joe,"*","*");

	list_dump(&joe,buff,-1);
	printf("%s",buff);
}

