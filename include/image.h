#ifndef _IMAGE_H
#define _IMAGE_H

struct _IMAGE_T;
typedef struct _IMAGE_T IMAGE_T;

IMAGE_T* Image_new ( char* file );
int Image_get_pixel(IMAGE_T* image, int row, int column);
void Image_print_header(IMAGE_T* image);
void Image_print_data(IMAGE_T* image, int start_row, int start_col, int nr_rows, int nr_cols);
void Image_free(IMAGE_T* image);

#endif // _IMAGE_H
