/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	FILE *fp = fopen(filename,"r");
	if(!fp){
		printf("open failed");
		return NULL;
	}

	Image *image = (Image *)malloc(sizeof(Image));
	
	char format[3];
	int maxColor;

	fscanf(fp,"%s",format);
	if(format[0]!='P' && format[1] != '3'){
		printf("format error\n");
		return NULL;
	}

	fscanf(fp,"%u %u",&image->cols,&image->rows);

	fscanf(fp,"%u",&maxColor);
	if(image->cols < 0 || image->rows < 0 ||maxColor != 255){
		printf("wrong format\n");
		return NULL;
	}

	int totpixels = image->cols * image->rows;

	image->image = (Color **)malloc(totpixels * sizeof(Color *));

	
	for(int i = 0;i < totpixels;i++){
		*(image->image + i) = (Color *)malloc(sizeof(Color));	
		Color *pixel = *(image->image+i);
		fscanf(fp,"%hhu %hhu %hhu",&pixel->R,&pixel->G,&pixel->B);
	}
	fclose(fp);
	return image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	uint32_t rows;
	uint32_t cols;

	int size = image->rows * image->cols;

	rows=image->rows;
	cols=image->cols;

	printf("P3\n%u %u\n255\n",cols,rows);

	for(int i = 0;i < size;i++){
		if((i+1) % (image->cols) != 0)
			printf("%3hhu %3hhu %3hhu   ",(*(image->image + i))->R,(*(image->image + i))->G,(*(image->image + i))->B);
		else
			printf("%3hhu %3hhu %3hhu\n",(*(image->image + i))->R,(*(image->image + i))->G,(*(image->image + i))->B);
	}


//	Color **pixel = image->image;
	
/*	for(int i = 0;i < rows;i++){
		for(int j = 0;j < cols-1;j++){
			printf("%3hhu %3hhu %3hhu   ",(*pixel)->R,(*pixel)->G,(*pixel)->B);
			pixel++;
		}
			printf("%3hhu %3hhu %3hhu\n",(*pixel)->R,(*pixel)->G,(*pixel)->B);
			pixel++;
	}	
*/

}

//Frees an image
void freeImage(Image *image)
{
	int size = image->rows * image->cols;
	for(int i = 0;i < size;i++){
		free(*(image->image + i));
	}
	free(image->image);
	free(image);
}


