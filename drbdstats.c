#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <glob.h>
#include <stdarg.h>
#include <stdbool.h>
#include "drbdgetstat.h"
#include "drbddigeststats.h"

int main(int argc, char *argv[] )
{
	FILE *fptr;
	char filename[255];
	char buff[2048];
	char *cur;
	char fndhost[100];
	char fndconn[100];
	char ch;
	char delim = ';';
	char *cvalue = NULL;
	char *obuff;
	bool newform=false;
	int json=1;
	int c;
	opterr = 0;
	char host[50];
	char connection[50];
	int i=0;
	glob_t globbuf;
	node arry=NULL;

	strcpy(host,"");
	strcpy(connection,"*");

	obuff=(char *) malloc(60000);
	obuff[0]=0;


  	while ((c = getopt (argc, argv, "ajtnd:Ar:c:")) != -1)
	switch (c)
	{
		case 'd':
			delim=optarg[0];		
			break;
		case 't':
			delim='\t';
			break;
		case 'n':
			delim='\n';
			break;
		case 'a':
			newform=true;
			json=1;
			break;
		case 'j':
			newform=true;
			json=-1;
		case 'A':
			strcpy(host,"*");
			strcpy(connection,"*");
			break;
		case 'r':
			strcpy(host,optarg);
			break;
		case 'c':
			strcpy(connection,optarg);
			break;
		case '?':
			if ((optopt == 'd')||(optopt == 'r')||(optopt == 'c'))
	            		fprintf (stderr, "Option -%c requires an argument.\n", optopt);
			else if (isprint (optopt))
				fprintf (stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
	 		return 1;
		default:
			abort ();
	}

	for(int s=0;connection[s]!=0;s++) if ((connection[s]=='/')||(connection[s]=='\\')) connection[s]='-';	
	for(int s=0;host[s]!=0;s++) if ((host[s]=='/')||(host[s]=='\\')) host[s]='-';	

	if (newform)
	{
		if (drbd_digest_stats(&arry,host,connection)>=0)
		{
			list_dump(&arry,obuff,json);
			printf("%s\n",obuff);
		}
		else
			fprintf(stderr,"Error retreiving list.\n");		
	}
	else
	{
		if (drbd_get_stats(obuff,delim,host,connection)>=0)
		{
			printf("%s\n",obuff);
			return(0);
		}
		else
		{
			fprintf(stderr,"Error retreiving list.\n");
			return(-1);
		}
	}

}

