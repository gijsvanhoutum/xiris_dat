#ifndef _DIR_H
#define _DIR_H

#include <string.h>

struct _DIR_T;
typedef struct _DIR_T DIR_T;

DIR_T* Dir_all( const char *path );
DIR_T* Dir_select( const char *path, char* hint );
int Dir_get_count( DIR_T* dir);
char* Dir_get_entry( DIR_T* dir, int index );
void Dir_free( DIR_T* dir );

#endif // _DIR_H