/*
 * Linked List Libary header file
 */

#ifndef LIBLL
#define LIBLL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct LinkedList {
	char kind;
	void * data;
	char name[50];
	size_t size;
	struct LinkedList *prev;
	struct LinkedList *next;
};

typedef struct LinkedList *node;

node list_add(node * top,char * name,void * data,size_t size,char kind);
node list_delete(node * nn);
node list_first(node * nn);
node list_last(node * nn);
int list_count(node * nn);
void list_renumber(node * nn);
node list_clear(node * nn);
void list_name(node * nn,char * name);
node list_find(node * nn,char * name);
void list_rename(node * nn,char * name);
void list_dump(node * nn,char * obuff,int indent);
node list_array(node * nn,char * name,bool crt);


#endif /* LIBLL */
