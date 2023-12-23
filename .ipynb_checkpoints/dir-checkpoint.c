#include "dir.h"
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

struct _DIR_T {
	char** entries;
	size_t count;
};

DIR_T* Dir_select( const char *path, char* hint ) {
	
    DIR *dp = NULL;
    struct dirent *ep = NULL;
	int i;
	DIR_T* dir_ptr;
	
    dp = opendir(path);
    if(NULL == dp) {
        fprintf(stderr, "no such directory: '%s \n'", path);
        return 0;
    }

	 // allocate memory for the image struct
	dir_ptr = (DIR_T*) malloc ( sizeof(DIR_T) );
	
    // count number of items in directory
	dir_ptr->count = 0;
    ep = readdir(dp);
	
	if ( hint == NULL) {
		while(NULL != ep){
			dir_ptr->count++;
			ep = readdir(dp);
		}		
	} else {
		while(NULL != ep ){
			if ( strncmp( ep->d_name , hint, strlen(hint) ) == 0 ) {
				dir_ptr->count++;
			}
			ep = readdir(dp);
		}	
	}
    rewinddir(dp);

	// allocate memory to hold the count
	dir_ptr->entries = (char**) malloc ( dir_ptr->count* sizeof(char*) );

	// fill the array
	i= 0;
    ep = readdir(dp);
	if ( hint == NULL) {
		while(NULL != ep){
			dir_ptr->entries[i] = strdup(ep->d_name);
			++i;
			ep = readdir(dp);
		}		
	} else {
		while(NULL != ep) {
			if ( strncmp( ep->d_name , hint, strlen(hint) ) == 0 ) {
				dir_ptr->entries[i] = strdup(ep->d_name);
				++i;
			}
			ep = readdir(dp);
		}	
	}

	// close directory
    closedir(dp);
	
    return dir_ptr;
}

DIR_T* Dir_all( const char *path ) {
	char* t = NULL;
	DIR_T* dir = Dir_select( path, t);
	return dir;
}

int Dir_get_count( DIR_T* dir) {
	return dir->count;
}

char* Dir_get_entry( DIR_T* dir, int index ) {
	return dir->entries[index];
}

void Dir_free( DIR_T* d ) {
	int i;
    for (i=0 ; i<d->count ; ++i) {
        free(d->entries[i]);
    }
	free(d->entries);
    free (d);
}