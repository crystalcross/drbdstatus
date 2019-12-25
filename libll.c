/*
 * Linked List Library - for creating named arrays
 */

#include "libll.h"
#include "libutils.h"

node list_add(node * nn,char * name,void * data,size_t size,char kind)
{
	node temp;
	node top;
	node fnd;
	char tmp[20];
	char nam[20];
	int i;
	temp=list_find(nn,name);
	if (temp!=NULL)
		*nn=list_delete(&temp);
	top=*nn;
	strcpy(nam,name);
	if (*nn==NULL)
	{
		*nn=(node)malloc(sizeof(struct LinkedList));
		(**nn).next = NULL;
		(**nn).prev = NULL;
		temp=*nn;
		if (nam[0]==0)
		{
			strcpy(nam,"#0");
		}
	}
	else
	{
		top=list_first(nn);
		if (nam[0]==0)
		{
			i=list_count(&top);
			list_renumber(&top);
			strcpy(nam,"#");
			sprintf(tmp,"%d",i);
			strcat(nam,tmp);
		}
		temp=list_last(nn);
		temp->next=(node)malloc(sizeof(struct LinkedList));
		temp->next->prev=temp;
		temp->next->next=NULL;
		temp=temp->next;
	}
	temp->kind=kind;
	if (kind=='a')
	{
		temp->data=data;
		temp->size=0;
	}
	else
	{
		temp->size=size;
		temp->data=malloc(size);
		memcpy(temp->data,data,size);
	}
	strcpy(temp->name,nam);
	return(temp);
}

node list_array(node * nn,char * name,bool crt)
{
	node temp;

	if (*nn==NULL)
	{
		if(!crt) return(NULL);
		temp=list_add(nn,name,NULL,0,'a');
		return(temp);
	}
	temp=list_find(nn,name);
	if (temp!=NULL)
	{
		if (temp->kind=='a')
			return(temp);
		if (!crt) return(NULL);
		temp=list_delete(&temp);
		temp=list_add(&temp,name,NULL,0,'a');
		return(temp);
	}
	if (crt) temp=list_add(nn,name,NULL,0,'a');
	return(temp);
}

node list_find(node * nn,char * name)
{
	node top;

	if (nn==NULL) return(NULL);
	top=list_first(nn);
	while (top!=NULL)
	{
		if (strcmp(top->name,name)==0)
			return(top);
		top=top->next;
	}
	return(NULL);
}

void list_renumber(node * nn)
{
	node temp;
	int i;
	char tmp[20];

	temp=list_first(nn);
	if (temp == NULL) 
		return;
	i=0;
	while (temp != NULL)
	{
		if (temp->name[0]=='#')
		{
			strcpy(temp->name,"#");
			sprintf(tmp,"%d",i);
			strcat(temp->name,tmp);
		}
		temp=temp->next;
		i++;
	}
}

void list_rename(node * nn,char * name)
{
	node temp;

	if (*nn==NULL) return;
	temp=*nn;
	if (name[0]==0)
		strcpy(temp->name,"#0");
	else
	    	strcpy(temp->name,name);
	list_renumber(nn);
}

void list_name(node * nn,char * name)
{
	node temp;
	temp=*nn;
	if (temp!=NULL) 
		strcpy(name,temp->name);
	else
		name[0]=0;
}

node list_delete(node * nn)
{
	node temp;
	node top;

	top=*nn;
	if (top==NULL)
		return(NULL);
	else
	{
		if (top->data != NULL)
		{
			if (top->kind=='a')
				top->data=list_clear((node *)&(top->data));
			else
				free(top->data);
		}
		if (top->prev != NULL)
		{
			top->prev->next=top->next;
			temp=top->prev;
		}
		else
			temp=top->next;
		if (top->next != NULL)
			top->next->prev=top->prev;
		free(top);
		*nn=temp;
		list_renumber(nn);
		return(*nn);
	}
}

node list_clear(node * nn)
{
	node temp;

	if (*nn==NULL)
		return(NULL);
	temp=list_last(nn);
	while(temp!=NULL)
	{
		temp=list_delete(&temp);
	}
	return(temp);
}

int list_count(node * nn)
{
	node temp;
	int i;

	temp=list_first(nn);
	if (temp == NULL) 
		return(0);
	i=1;
	while (temp->next != NULL)
	{
		temp=temp->next;
		i++;
	}
	return(i);
}

node list_first(node * nn)
{
	node top;

	if (*nn==NULL)
		return(NULL);
	else
	{
		for(top=*nn;top->prev!=NULL;top=top->prev);
		return(top);
	}
}

node list_last(node * nn)
{
	node temp;

	if (nn==NULL)
		return(NULL);
	else
	{
		for(temp=*nn;temp->next!=NULL;temp=temp->next);
		return(temp);
	}
}

void list_dump(node * nn,char * obuff,int indent)
{
	node temp;
	char del[50];
	int dnext=-1;
	char fdelim='\n';
	char adelim='\n';

	memset(del,'\t',49);
	if (indent>=0)
	       	del[indent]=0;
	else
	{
		del[0]=0;
		fdelim=',';
		adelim=' ';
	}
	if (indent>=0) dnext=indent+1;

	if (*nn==NULL) return;
	temp=list_first(nn);
	catf(obuff,"%c%c",'{',adelim);
	while (temp!=NULL)
	{
		catf(obuff,"%s",del);
		catf(obuff,"%s:",temp->name);
		switch (temp->kind)
		{
			case 's':
				catf(obuff,"\"%s\"",(char *)temp->data);
				if (fdelim!=',') catf(obuff,"%c",fdelim);
				break;
			case 'a':
				list_dump((node *)&temp->data,obuff,dnext);
				break;
		}
		temp=temp->next;
		if ((temp!=NULL)&&(fdelim==',')) catf(obuff,"%c",fdelim);
	}
	if (indent>0) del[indent-1]=0;
	catf(obuff,"%s",del);
	catf(obuff,"%c%c",'}',adelim);
}


