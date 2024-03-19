/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				Yash Mandlik
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	uint8_t blueChannel = (image->image[row][col]).B;
	unsigned val = blueChannel & 1;
	Color *newColor = (Color*) malloc(sizeof(Color));
	newColor->R = newColor->G = newColor->B = val * 255;
	return newColor;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	Image *newImage = (Image*) malloc(sizeof(Image));
	if (newImage == NULL) {
		printf("Memory allocation for newImage failed.\n");
	}
	newImage->rows = image->rows;
	newImage->cols = image->cols;
	newImage->image = (Color**) malloc(newImage->rows * sizeof(Color*));
	for (int i = 0; i < newImage->rows; i++) {
		newImage->image[i] = (Color*) malloc(newImage->cols * sizeof(Color));
		for (int j = 0; j < newImage->cols; j++) {
			Color *temp = evaluateOnePixel(image, i, j);
			newImage->image[i][j] = *temp;
			free(temp);
		}
	}
	return newImage;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
		exit(-1);
	}
	char *filename = argv[1];
	Image *image = readData(filename);
	Image *newImage = steganography(image);
	writeData(newImage);
	freeImage(image);
	freeImage(newImage);
	return 0;
}
