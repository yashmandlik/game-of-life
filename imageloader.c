/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				Yash Mandlik
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
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Error. File does not exist\n");
		exit(1);
	}
	Image *image = (Image*) malloc(sizeof(Image));
	if (image == NULL) {
		printf("Error. Memory allocation for image failed. \n");
		exit(1);
	}
	char imgFormat[2];
	uint8_t maxPixelVal;
	fscanf(fp, "%s %"PRIu32" %"PRIu32" %hhd", imgFormat, &image->cols, &image->rows, &maxPixelVal);
	if (strcmp(imgFormat, "P3")) {
		printf("Error. File format is not PPM.\n");
		exit(1);
	}
	if (maxPixelVal != 255) {
		printf("Error. File format is not PPM.\n");
		exit(1);
	}
	image->image = (Color**) malloc(image->rows * sizeof(Color*));
	if (image->image == NULL) {
		printf("Error. Memory allocation for pixel pointers failed. \n");
		exit(1);
	}
	for (int i = 0; i < image->rows; i++) {
		image->image[i] = (Color*) malloc(image->cols * sizeof(Color));
		for (int j = 0; j < image->cols; j++) {
			fscanf(fp, "%hhd %hhd %hhd", &((image->image[i][j]).R), &((image->image[i][j]).G), &((image->image[i][j]).B));
		}
	}
	fclose(fp);
	return image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	char imgFormat[2] = "P3";
	uint8_t pixelVal = 255;
	printf("%s\n%d %d\n%d\n", imgFormat, image->cols, image->rows, pixelVal);
	for (int i = 0; i < image->rows; i++) {
		for (int j = 0; j < image->cols; j++) {
			if (j == image->cols - 1) {
				printf("%3d %3d %3d\n", (image->image[i][j]).R, (image->image[i][j]).G, (image->image[i][j]).B);
			} else {
				printf("%3d %3d %3d   ", (image->image[i][j]).R, (image->image[i][j]).G, (image->image[i][j]).B);
			}
		}
	}
}

//Frees an image
void freeImage(Image *image)
{
	for (int i = 0; i < image->rows; i++) {
		free(image->image[i]);
	}
	free(image->image);
	free(image);
}