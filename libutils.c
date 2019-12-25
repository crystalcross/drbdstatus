#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <glob.h>
#include <stdarg.h>

char * ltrim(char * ibuff)
{
	char bf[2048];
	char * strt;
	strncpy(bf,ibuff,2047);
	bf[2047]=0;
	for(strt=bf;(((strt[0]==' ')||(strt[0]=='\t'))&&(strt<(bf+2048)));strt++);
	strcpy(ibuff,strt);
}

char * rtrim(char * ibuff)
{
	char bf[2048];
	char * strt;
	strncpy(bf,ibuff,2047);
	bf[2047]=0;
	for(strt=bf;(((strt[0]!=0))&&(strt<(bf+2048)));strt++);
	strt--;
	for(;((strt!=bf)&&((strt[0]==' ')||(strt[0]=='\t')));strt--) strt[0]=0;
	strcpy(ibuff,bf);
}

char * trim(char * ibuff)
{
	char bf[2048];
	char * strt;
	strncpy(bf,ibuff,2047);
	bf[2047]=0;
	ltrim(bf);
	rtrim(bf);
	strcpy(ibuff,bf);
}


void catf( char * obuff, const char* format, ... ) 
{
	va_list args;
	char bf[2048];
	va_start( args, format );
	vsprintf(bf,format,args );
	strcat(obuff,bf);
	va_end( args );
}

char * splitit(char * find,char * pth, int part,char dlm)
{
	char * loc;
	int cnt,fnd;
	char found[100];

	loc=pth;
	fnd=0;
	found[0]=0;
	cnt=0;
	while((loc[0] != 0)&&(cnt<=part))
	{
		if (cnt==part)
		{
			if (loc[0]!=dlm)
			{
				found[fnd++]=loc[0];
				found[fnd]=0;
			}
		}
		if (loc[0]==dlm) cnt++;
		loc++;
	}
	strcpy(find,found);
	return(find);
}
