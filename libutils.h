/*
 * Definition for a list of little utilities and functions I use
 *
 */
#ifndef LIBUTILS
#define LIBUTILS

void catf( char * obuff, const char* format, ... );
char * splitit(char * find,char * pth, int part,char dlm);
char * ltrim(char * ibuff);
char * rtrim(char * ibuff);
char * trim(char * ibuff);

#endif /* LIBUTILS */
