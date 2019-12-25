#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <glob.h>
#include <stdarg.h>
#include "libutils.h"
#include "libll.h"
#include "drbdgetstat.h"

int findvr(char * vr,char * buff,char * name)
{
	char fnd[200];

	for(int i=0;splitit(fnd,buff,i,'|')[0]!=0;i++)
	{
		if (strcmp(splitit(vr,fnd,0,':'),name)==0)
		{
			splitit(vr,fnd,1,':');
			return(0);
		}
	}
	return(-1);
}


int drbd_digest_stats(node * nn,char * host, char * connection)
{
	char * buff;
	char fnd[2048];
	char part[200];
	char vr[50];
	char dt[150];
	char res[150];
	char conn[150];
	node temp1,temp2;
	node * temp1p,temp2p;

	buff=(char *)malloc(20000);
	buff[0]=0;

	if (drbd_get_stats(buff,'|',host,connection)>=0)
	{
		for(int i=0;splitit(fnd,buff,i,'\n')[0]!=0;i++)
		{
			res[0]=0;
			conn[0]=0;
			if (findvr(res,fnd,"res")<0) return(-1);
			if (findvr(conn,fnd,"conn")<0) return(-1);
			temp1=list_array(nn,res,true);
			temp1p=(node *)&temp1->data;
			temp2=list_array(temp1p,conn,true);
			temp2p=(node *)&temp2->data;
			temp1=list_array(temp2p,"live",true);
			temp1p=(node *)&temp1->data;
			for(int j=0;splitit(part,fnd,j,'|')[0]!=0;j++)
			{
				splitit(vr,part,0,':');
				splitit(dt,part,1,':');
				list_add(temp1p,vr,dt,50,'s');
			}
		}
		return(0);
	}
	else
	{
		return(-1);
	}
}
