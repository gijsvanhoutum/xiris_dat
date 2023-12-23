#include "image.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _IMAGE_T {
	short int fSize;
	char* file_name;
    char schema;
    short int header_len;
    short int aoi_left;
    short int aoi_right;
    short int aoi_top;
    short int aoi_bottom;
    short int img_width;
    short int img_height;
    char bits_pp;
    char pixel_fmt;
	short int** data;
};

IMAGE_T* Image_new ( char* file ) {
	
	// local variables
	FILE* file_ptr;
	IMAGE_T* image_ptr;
	long file_size;
	char* buffer;
	size_t read_result;
	long buffer_position;
	int row;
	int column;
	
	 // allocate memory for the image struct
	image_ptr = (IMAGE_T*) malloc ( sizeof(IMAGE_T) );
	
	// Open file and check
    file_ptr = fopen ( file , "rb" );
    if ( file_ptr == NULL ) {fputs ("File Open Error\n",stderr); exit (1);}

	// Allocate and set file name
    image_ptr->file_name = (char*) malloc ( sizeof(char) * (strlen(file)+1) );
	if ( image_ptr->file_name == NULL) {fputs ("File Name Allocation Error\n",stderr); exit (2);}
	strcpy (image_ptr->file_name, file);
	
    // obtain file size:
    fseek (file_ptr , 0 , SEEK_END);
	file_size = ftell ( file_ptr );
    rewind ( file_ptr );

	// allocate memory to contain the whole file
	buffer = (char*) malloc ( sizeof(char) * file_size );
	if ( buffer == NULL) {fputs ("Buffer Allocation Error\n",stderr); exit (2);}

    // copy the file into the buffer  and close the file
    read_result = fread( buffer,1, file_size, file_ptr );
    if ( read_result != file_size ) {fputs ("File Reading error\n",stderr); exit (3);}
	fclose( file_ptr );

    // header
    if ( buffer[0] == 0 && buffer[1] == 0) { // schema 1
        image_ptr->schema = 1;
        memcpy(&image_ptr->header_len, buffer+4,4);
        memcpy(&image_ptr->aoi_left, buffer+8,4);
        memcpy(&image_ptr->aoi_top, buffer+12,4);
        memcpy(&image_ptr->aoi_right,buffer+16,4);
        memcpy(&image_ptr->aoi_bottom,buffer+20,4);
        memcpy(&image_ptr->img_width, buffer+24,4);
        memcpy(&image_ptr->bits_pp,buffer+28,4);
        memcpy(&image_ptr->pixel_fmt,buffer+32,4);
        image_ptr->img_height = image_ptr->aoi_bottom - image_ptr->aoi_top;
        buffer_position = 36;
    } else { // schema 0
        image_ptr->schema = 0;
        memcpy(&image_ptr->img_height,buffer,2);
        memcpy(&image_ptr->img_width,buffer+2,2);
        memcpy(&image_ptr->bits_pp,buffer+4,2);
        memcpy(&image_ptr->pixel_fmt,buffer+6,2);
        buffer_position = 9;
    }
	
    // allocate 2D array for the DATA in the IMAGE struct:
	// allocate memory for rows
    image_ptr->data = (short int**) malloc ( image_ptr->img_height * sizeof( short int* ) );
    if ( image_ptr->data == NULL ) {fputs ("Data Rows Allocation Error\n",stderr); exit (2);}
    //Allocate memory for columns
    for (row=0 ; row<image_ptr->img_height ; ++row) {
        image_ptr->data[row] = (short int*) malloc ( image_ptr->img_width * sizeof(short int) );
        if ( image_ptr->data[row] == NULL ) {fputs ("Data Column Allocation Error\n",stderr); exit (2);}
    }
	
    // data
	switch (image_ptr->pixel_fmt) {
		
		case 0: // monochrome 8 bits
	        for ( row=0; row<image_ptr->img_height; ++row ) {
				for ( column=0; column<image_ptr->img_width; ++column ) {
				image_ptr->data[row][column] = buffer[buffer_position];
                ++buffer_position;
				}
			}
			break;
			
		case 1: // monochrome 12 bits
			for (row=0; row<image_ptr->img_height; ++row) {
				// do for every 2 pixels ( 3 bytes at a time )
				for (column=0; column<image_ptr->img_width; column=column+2) {
					image_ptr->data[row][column] = (buffer[buffer_position]  << 4 )| ( (buffer[buffer_position+1] & 240 ) >> 4);
					image_ptr->data[row][column+1] = (buffer[buffer_position+2] << 4) | (buffer[buffer_position+1] & 15);
					buffer_position=buffer_position+3;
				}
			}			
			break;
			
		default: // future use
			fputs ("Pixel Format Error\n",stderr); exit (2);		
	}
	
	// De allocate the buffer
	free(buffer);
	
    return image_ptr;
}

void Image_print_header(IMAGE_T* image) {
	printf("------ Header ------\n");
	printf("%-15s%s\n","Filename", image->file_name);
	printf("%-15s%i\n","Schema", image->schema);
	printf("%-15s%i\n","Headler Length", image->header_len);
	printf("%-15s%i\n","AOI Left", image->aoi_left);
	printf("%-15s%i\n","AOI Right", image->aoi_right);
	printf("%-15s%i\n","AOI Top", image->aoi_top);
	printf("%-15s%i\n","AOI Bottom", image->aoi_bottom);
	printf("%-15s%i\n","Image Width", image->img_width);
	printf("%-15s%i\n","Image Height", image->img_height);
	printf("%-15s%i\n","Bits per Pixel", image->bits_pp);
	printf("%-15s%i\n","Pixel Format", image->pixel_fmt);
	printf("--------------------\n\n");
}

void Image_print_data(IMAGE_T* image, int start_row, int start_col, int nr_rows, int nr_cols) {
	int row;
	int column;
	printf("------- Data -------\n");
	for (row=start_row; row<start_row+nr_rows; row++) {
		for (column=start_col; column<start_col+nr_cols; column++) {
			printf("%i ", image->data[row][column]);
		}
		printf("\n");
	}
	printf("--------------------\n\n");
}

int Image_get_pixel(IMAGE_T* image, int row, int column) {
    return image->data[row][column];
}

void Image_free(IMAGE_T* image) {
	int row;
    for (row=0 ; row<image->img_height ; row++) {
        free(image->data[row]);
    }
	free(image->data);
    free (image);
}
