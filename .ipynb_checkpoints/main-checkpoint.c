#include "image.h"
#include "dir.h"

#include <stdio.h>
#include <time.h>
#include <string.h>

int main () {
	
    int i;
	clock_t begin;
	double average;
	IMAGE_T* image;
	DIR_T* dir;
	int count;
	char* entry;
	
    dir = Dir_select ("/documents/VSI_C", "Image");
	count = Dir_get_count(dir);

	average = 0;
	for ( i = 0; i < count; ++i) {
		entry = Dir_get_entry( dir, i );
		begin = clock();
		image = Image_new( entry );
		average+= (double) (clock()-begin) / CLOCKS_PER_SEC;
		Image_free(image);	
	}

	average = average / count * 1000;
	printf("------ Average -----\n");
	printf("Average time over %i reads is: %f [ms] \n", count, average );
	printf("--------------------\n\n");
	
	image = Image_new( "Image000002.dat" );
	Image_print_header(image);
	Image_print_data(image,0,0,10,10);
	
	Dir_free(dir);
	Image_free(image);

    return 0;
}