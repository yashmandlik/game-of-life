/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				Yash Mandlik
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	uint32_t totalRows = image->rows;
	uint32_t totalCols = image->cols;
	int neighbors[8][2] = {{-1,1},{0,1},{1,1},{-1,0},{1,0},{-1,-1},{0,-1},{1,-1}};
	int liveNeighbors = 0;
	
	uint8_t colors[3] = {0,0,0};
	int maxIntColor = 0;

	if ((image->image[row][col]).R > 0 || (image->image[row][col]).G > 0 || (image->image[row][col]).B > 0) {
		rule >>= 9;
	}
	int tempRow = row;
	int tempCol = col;
	for (int i = 0; i < 8; i += 1) {
		tempRow = ((row + neighbors[i][0]) + totalRows) % totalRows;
		tempCol = ((col + neighbors[i][1]) + totalCols) % totalCols;
		if ((image->image[tempRow][tempCol]).R > 0 || (image->image[tempRow][tempCol]).G > 0 || (image->image[tempRow][tempCol]).B > 0) {
			liveNeighbors += 1;
			int tempSum = image->image[tempRow][tempCol].R + image->image[tempRow][tempCol].G + image->image[tempRow][tempCol].B;
			if (tempSum > maxIntColor) {
				colors[0] = image->image[tempRow][tempCol].R;
				colors[1] = image->image[tempRow][tempCol].G;
				colors[2] = image->image[tempRow][tempCol].B;
				maxIntColor = tempSum;
			}
		}
	}
	rule = (rule >> liveNeighbors) & 1;
	Color *newState = (Color*) malloc(sizeof(Color));
	if (newState == NULL) {
		printf("Memory allocation for new Color failed. \n");
		exit(-1);
	}
	if (rule) {
		newState->R = colors[0];
		newState->G = colors[1];
		newState->B = colors[2];
	} else {
		newState->R = 0;
		newState->G = 0;
		newState->B = 0;
	}
	
	return newState;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	Image *newImage = (Image*) malloc(sizeof(Image));
	if (newImage == NULL) {
		printf("Memory allocation for new image failed. \n");
		exit(-1);
	}
	newImage->rows = image->rows;
	newImage->cols = image->cols;
	newImage->image = (Color**) malloc(newImage->rows * sizeof(Color*));
	for (int i = 0; i < newImage->rows; i++) {
		newImage->image[i] = (Color*) malloc(newImage->cols * sizeof(Color));
		for (int j = 0; j < newImage->cols; j++) {
			Color *temp = evaluateOneCell(image, i, j, rule);
			newImage->image[i][j] = *temp;
			free(temp);
		}
	}
	return newImage;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	if (argc != 3) {
		printf("usage: ./gameOfLife filename rule \n");
		printf("filename is an ASCII PPM file (type P3) with maximum value 255. \n");
		printf("rule is a hex number beginning with 0x; Life is 0x1808. \n");
		exit(-1);
	}
	char *fileName = argv[1];
	char *hex = argv[2];
	Image *image = readData(fileName);
	uint32_t rule = strtol(hex, NULL, 16);
	Image *newImage = life(image, rule);
	writeData(newImage);
	freeImage(image);
	freeImage(newImage);
	
}
