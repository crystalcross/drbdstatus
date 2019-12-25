#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <glob.h>
#include <stdarg.h>
#include "libutils.h"

void parsedrbd(char * obuff,char * bf,char dlm)
{
	int line=0;
	int chr=0;
	int pt=0;
	int adtl=2;
	int spst=1;
	char tmp[50];
	char * loc;
	char parts[10][20][100];
	for (int i=0;i<10;i++)
		for (int j=0;j<20;j++) parts[i][j][0]=0;

	loc=bf;
	while (*loc!=0)
	{
		if ((spst==0)&&((*loc==' ')||(*loc=='\t')||(*loc=='(')||(*loc==')')))		// Start of blank
		{
			spst=1;
			pt++;
			loc++;
			chr=0;

		}
		else if ((spst==1)&&((*loc==' ')||(*loc=='\t')||(*loc=='(')||(*loc==')')))	// Continued blank
		{
			loc++;
		}
		else if ((*loc=='\n'))					// Line Ender
		{
			spst=1;
			line++;
			chr=00;
			pt=0;
			loc++;
		}
		else if ((spst==1))
		{
			spst=0;
			chr=0;
			parts[line][pt][chr]=*loc;
			parts[line][pt][chr+1]=0;
			loc++;
		}
		else
		{
			if (*loc!=',')
			{
				chr++;		
				parts[line][pt][chr]=*loc;
				parts[line][pt][chr+1]=0;
			}
			loc++;
		}
	}
	splitit(tmp,parts[0][0],0,':');
	catf(obuff,"resnum:%s%c",tmp,dlm);
	catf(obuff,"protocol:%s%c",parts[0][4],dlm);
	catf(obuff,"flags:%s%c",parts[0][5],dlm);
	for(int j=1;j<4;j++)
		catf(obuff,"%s%c",parts[0][j],dlm);
	for(int j=0;parts[1][j][0]!=0;j++)
		catf(obuff,"%s%c",parts[1][j],dlm);
	if (line==8)
	{
		long int tt=0;
		adtl=5;
		catf(obuff,"syncing:%s%c","yes",dlm);
		catf(obuff,"progress:%s%c",parts[2][0],dlm);
		catf(obuff,"synced:%s%c",parts[2][2],dlm);
		splitit(tmp,parts[2][3],0,'/');
		catf(obuff,"left:%s%s%c",tmp,parts[2][4],dlm);
		splitit(tmp,parts[2][3],1,'/');
		catf(obuff,"total:%s%s%c",tmp,parts[2][4],dlm);
		splitit(tmp,parts[3][1],0,':');
		tt=atol(tmp)*3600;
		splitit(tmp,parts[3][1],1,':');
		tt=tt+(atol(tmp)*60);
		splitit(tmp,parts[3][1],2,':');
		tt=tt+atol(tmp);
		int w=7;
		if (strcmp(parts[3][7],"want:")==0)
		{
			w=9;
			catf(obuff,"swant:%s %s%c",parts[3][8],parts[3][w],dlm);
		}
		else
			catf(obuff,"swant:0 %s%c",parts[3][w],dlm);
		catf(obuff,"timeleft:%07ld%c",tt,dlm);
		catf(obuff,"sspeed:%s %s%c",parts[3][3],parts[3][w],dlm);
		catf(obuff,"ssmin:%s %s%c",parts[3][4],parts[3][w],dlm);
		catf(obuff,"ssmax:%s %s%c",parts[3][6],parts[3][w],dlm);
		catf(obuff,"sposperc:%s%c",parts[4][0],dlm);
		splitit(tmp,parts[4][3],0,'/');
		catf(obuff,"sposc:%s%c",tmp,dlm);
		splitit(tmp,parts[4][3],1,'/');
		catf(obuff,"spost:%s%c",tmp,dlm);
	}
	else
	{
		catf(obuff,"syncing:no%c",dlm);
		catf(obuff,"progress:none%c",dlm);
		catf(obuff,"synced:100%%%c",dlm);
		catf(obuff,"left:0%c",dlm);
		catf(obuff,"total:0%c",dlm);
		catf(obuff,"timeleft:0%c",dlm);
		catf(obuff,"sspeed:0 K/sec%c",dlm);
		catf(obuff,"ssmin:0 K/sec%c",dlm);
		catf(obuff,"ssmax:0 K/sec%c",dlm);
		catf(obuff,"sposperc:0%%%c",dlm);
		catf(obuff,"sposc:0%c",dlm);
		catf(obuff,"spost:0%c",dlm);
	}
	splitit(tmp,parts[adtl][1],0,'/');
	catf(obuff,"re-%s%c",tmp,dlm);
	splitit(tmp,parts[adtl][1],1,'/');
	catf(obuff,"re-total:%s%c",tmp,dlm);
	for(int j=2;parts[adtl][j][0]!=0;j++)
		catf(obuff,"re-%s%c",parts[adtl][j],dlm);
	adtl++;
	splitit(tmp,parts[adtl][1],0,'/');
	catf(obuff,"al-%s%c",tmp,dlm);
	splitit(tmp,parts[adtl][1],1,'/');
	catf(obuff,"al-total:%s%c",tmp,dlm);
	for(int j=2;parts[adtl][j][0]!=0;j++)
		catf(obuff,"al-%s%c",parts[adtl][j],dlm);
	adtl++;
	splitit(tmp,parts[adtl][4],0,'/');
	catf(obuff,"boal1:%s%c",tmp,dlm);
	splitit(tmp,parts[adtl][4],1,'/');
	catf(obuff,"boal2:%s%c",tmp,dlm);
	splitit(tmp,parts[adtl][4],2,'/');
	catf(obuff,"boal3:%s%c",tmp,dlm);
	catf(obuff,"\n");
}

int drbd_get_stats(char * obuff,char delim, char * host, char * connection)
{
	FILE *fptr;
	char filename[255];
	char buff[2048];
	char *cur;
	char fndhost[100];
	char fndconn[100];
	char ch;
	char *cvalue = NULL;
	int c;
	opterr = 0;
	int i=0;
	glob_t globbuf;

	strcpy(filename,"/sys/kernel/debug/drbd/resources/");
	strcat(filename,host);
	strcat(filename,"/connections/");
	strcat(filename,connection);
	strcat(filename,"/0/proc_drbd");

	obuff[0]=0;

	if (!glob(filename, 0, NULL, &globbuf))
       	{
		for (i=0;  i <globbuf.gl_pathc; i++)
		{ 
			splitit(fndhost,globbuf.gl_pathv[i],6,'/');
			splitit(fndconn,globbuf.gl_pathv[i],8,'/');
			catf(obuff,"res:%s",fndhost);
			catf(obuff,"%c",delim);
			catf(obuff,"conn:%s",fndconn);
			catf(obuff,"%c",delim);
			fptr=fopen(globbuf.gl_pathv[i],"r");
			buff[0]=0;
			cur=buff;

			if (fptr == NULL)
			{
				return(-1);
			}
			ch=fgetc(fptr);
			while (ch != EOF)
			{
				cur[0]=ch;
				cur++;
				cur[0]=0;
				ch=fgetc(fptr);
			}
			fclose(fptr);
			parsedrbd(obuff,buff,delim);
			
       		}
		globfree(&globbuf);
		return(0);
	}
       	else 
	        return(-1);
}
