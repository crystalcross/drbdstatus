#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <glob.h>

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

void parsedrbd(char * bf,char dlm)
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
	printf("resnum:%s%c",tmp,dlm);
	printf("protocol:%s%c",parts[0][4],dlm);
	printf("flags:%s%c",parts[0][5],dlm);
	for(int j=1;j<4;j++)
		printf("%s%c",parts[0][j],dlm);
	for(int j=0;parts[1][j][0]!=0;j++)
		printf("%s%c",parts[1][j],dlm);
	if (line==8)
	{
		long int tt=0;
		adtl=5;
		printf("syncing:%s%c","yes",dlm);
		printf("progress:%s%c",parts[2][0],dlm);
		printf("synced:%s%c",parts[2][2],dlm);
		splitit(tmp,parts[2][3],0,'/');
		printf("left:%s%s%c",tmp,parts[2][4],dlm);
		splitit(tmp,parts[2][3],1,'/');
		printf("total:%s%s%c",tmp,parts[2][4],dlm);
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
			printf("swant:%s %s%c",parts[3][8],parts[3][w],dlm);
		}
		else
			printf("swant:0 %s%c",parts[3][w],dlm);
		printf("timeleft:%07ld%c",tt,dlm);
		printf("sspeed:%s %s%c",parts[3][3],parts[3][w],dlm);
		printf("ssmin:%s %s%c",parts[3][4],parts[3][w],dlm);
		printf("ssmax:%s %s%c",parts[3][6],parts[3][w],dlm);
		printf("sposperc:%s%c",parts[4][0],dlm);
		splitit(tmp,parts[4][3],0,'/');
		printf("sposc:%s%c",tmp,dlm);
		splitit(tmp,parts[4][3],1,'/');
		printf("spost:%s%c",tmp,dlm);
	}
	else
	{
		printf("syncing:no%c",dlm);
		printf("progress:none%c",dlm);
		printf("synced:100%%%c",dlm);
		printf("left:0%c",dlm);
		printf("total:0%c",dlm);
		printf("timeleft:0%c",dlm);
		printf("sspeed:0 K/sec%c",dlm);
		printf("ssmin:0 K/sec%c",dlm);
		printf("ssmax:0 K/sec%c",dlm);
		printf("sposperc:0%%%c",dlm);
		printf("sposc:0%c",dlm);
		printf("spost:0%c",dlm);
	}
	splitit(tmp,parts[adtl][1],0,'/');
	printf("re-%s%c",tmp,dlm);
	splitit(tmp,parts[adtl][1],1,'/');
	printf("re-total:%s%c",tmp,dlm);
	for(int j=2;parts[adtl][j][0]!=0;j++)
		printf("re-%s%c",parts[adtl][j],dlm);
	adtl++;
	splitit(tmp,parts[adtl][1],0,'/');
	printf("al-%s%c",tmp,dlm);
	splitit(tmp,parts[adtl][1],1,'/');
	printf("al-total:%s%c",tmp,dlm);
	for(int j=2;parts[adtl][j][0]!=0;j++)
		printf("al-%s%c",parts[adtl][j],dlm);
	adtl++;
	splitit(tmp,parts[adtl][4],0,'/');
	printf("boal1:%s%c",tmp,dlm);
	splitit(tmp,parts[adtl][4],1,'/');
	printf("boal2:%s%c",tmp,dlm);
	splitit(tmp,parts[adtl][4],2,'/');
	printf("boal3:%s%c",tmp,dlm);
	printf("\n");
	//printf("%s\n%s\nlines=%d\n",parts[0][0],bf,line);
}

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
	int c;
	opterr = 0;
	char host[50];
	char connection[50];
	int i=0;
	glob_t globbuf;

	strcpy(host,"");
	strcpy(connection,"*");


  	while ((c = getopt (argc, argv, "tnd:Ar:c:")) != -1)
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

	strcpy(filename,"/sys/kernel/debug/drbd/resources/");
	strcat(filename,host);
	strcat(filename,"/connections/");
	strcat(filename,connection);
	strcat(filename,"/0/proc_drbd");

	if (!glob(filename, 0, NULL, &globbuf))
       	{
		for (i=0;  i <globbuf.gl_pathc; i++)
		{ 
			splitit(fndhost,globbuf.gl_pathv[i],6,'/');
			splitit(fndconn,globbuf.gl_pathv[i],8,'/');
			printf("res:%s",fndhost);
			printf("%c",delim);
			printf("conn:%s",fndconn);
			printf("%c",delim);
			//printf("%s\n",globbuf.gl_pathv[i]);
			fptr=fopen(globbuf.gl_pathv[i],"r");
			buff[0]=0;
			cur=buff;

			if (fptr == NULL)
			{
	        		fprintf(stderr,"Error: no entries found.\n");
				exit(-1);
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
			parsedrbd(buff,delim);
			
       		}
		globfree(&globbuf);
	}
       	else 
	        fprintf(stderr,"Error: no entries found.\n");

	

}
