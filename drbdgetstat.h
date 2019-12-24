/*
 * Definition for drbd_get_stats
 *
 */

void catf( char * obuff, const char* format, ... );
char * splitit(char * find,char * pth, int part,char dlm);
void parsedrbd(char * obuff,char * bf,char dlm);
int drbd_get_stats(char * obuff,char delim, char * host, char * connection);
