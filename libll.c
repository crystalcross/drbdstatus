/*
 * Linked List Library - for creating named arrays
 */

#include "libll.h"
#include "libutils.h"

node list_add(node nn,char * name,void * data,size_t size,char kind)
{
	node temp;
	node top;
	node fnd;
	char tmp[20];
	char nam[20];
	int i;
	temp=list_find(nn,name);
	if (temp!=NULL)
		nn=list_delete(temp);
	top=nn;
	strcpy(nam,name);
	if (top==NULL)
	{
		top=(node)malloc(sizeof(struct LinkedList));
		top->next = NULL;
		top->prev = NULL;
		temp=top;
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
			i=list_count(top);
			list_renumber(top);
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
	return(top);
}

node list_find(node nn,char * name)
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

void list_renumber(node nn)
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

void list_rename(node nn,char * name)
{
	if (nn==NULL) return;
	if (name[0]==0)
		strcpy(nn->name,"#0");
	else
	    	strcpy(nn->name,name);
	list_renumber(nn);
}

void list_name(node nn,char * name)
{
	if (nn!=NULL) 
		strcpy(name,nn->name);
	else
		name[0]=0;
}

node list_delete(node nn)
{
	node temp;

	if (nn==NULL)
		return(NULL);
	else
	{
		if (nn->data != NULL)
		{
			if (nn->kind=='a')
				nn->data=list_clear(nn->data);
			else
				free(nn->data);
		}
		if (nn->prev != NULL)
		{
			nn->prev->next=nn->next;
			temp=nn->prev;
		}
		else
			temp=nn->next;
		if (nn->next != NULL)
			nn->next->prev=nn->prev;
		free(nn);
		nn=temp;
		list_renumber(nn);
		return(nn);
	}
}

node list_clear(node nn)
{
	node temp;

	if (nn==NULL)
		return(NULL);
	temp=list_last(nn);
	while(temp!=NULL)
	{
		temp=list_delete(temp);
	}
	return(temp);
}

int list_count(node nn)
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

node list_first(node nn)
{
	node top;

	if (nn==NULL)
		return(NULL);
	else
	{
		for(top=nn;top->prev!=NULL;top=top->prev);
		return(top);
	}
}

node list_last(node nn)
{
	node temp;

	if (nn==NULL)
		return(NULL);
	else
	{
		for(temp=nn;temp->next!=NULL;temp=temp->next);
		return(temp);
	}
}

void list_dump(node nn,char * obuff)
{
	node temp;

	if (nn==NULL) return;
	temp=list_first(nn);
	catf(obuff,"%c\n",'{');
	while (temp!=NULL)
	{
		catf(obuff,"%s:",temp->name);
		switch (temp->kind)
		{
			case 's':
				catf(obuff,"\"%s\"\n",(char *)temp->data);
				break;
			case 'a':
				list_dump((node)temp->data,obuff);
				break;
		}
		temp=temp->next;
	}
	catf(obuff,"%c\n",'}');
}


