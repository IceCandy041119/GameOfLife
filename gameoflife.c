/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
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
	Color *nextState = (Color *)malloc(sizeof(Color));
	int isAliveR,isAliveG,isAliveB;
	int aliveNeighourR = 0,aliveNeighourG = 0,aliveNeighourB = 0;
	int dx[8] = {0,0,1,-1,-1,1,1,-1};
	int dy[8] = {1,-1,0,0,-1,1,-1,1};

	isAliveR = (*(image->image + row * (image->cols) + col))->R != 0;
	isAliveG = (*(image->image + row * (image->cols) + col))->G != 0;
	isAliveB = (*(image->image + row * (image->cols) + col))->B != 0;

	for(int i = 0;i<8;i++){
		int nextRow = ((row + dx[i] + image->rows) % image->rows);	
		int nextCol = ((col + dy[i] + image->cols) % image->cols);	
		if((*(image->image + nextRow * (image->cols) + nextCol))->R != 0)
			aliveNeighourR++;
		if((*(image->image + nextRow * (image->cols) + nextCol))->G != 0)
			aliveNeighourG++;
		if((*(image->image + nextRow * (image->cols) + nextCol))->B != 0)
			aliveNeighourB++;
	}
	
	if((1<<(9*isAliveR + aliveNeighourR)) & rule)
		nextState->R = rand() % 256;
	else
		nextState->R = 0;
	if((1<<(9*isAliveG + aliveNeighourG)) & rule)
		nextState->G =	rand() % 256; 
	else
		nextState->G = 0;
	if( (1<<(9*isAliveB + aliveNeighourB)) & rule)
		nextState->B =	rand() % 256;
	else
		nextState->B = 0;

	return nextState;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	
	Image *iteration = (Image *)malloc(sizeof(Image));
	iteration->cols = image->cols;
	iteration->rows = image->rows;
	iteration->image = (Color **)malloc(sizeof(Color *) * image->cols *image->rows);
	Color **p = iteration->image;

	for(int i = 0;i < image->rows;i++){
		for(int j = 0;j < image->cols;j++){
			*p = evaluateOneCell(image,i,j,rule);
			p++;
		}
	}	
	return iteration;
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
	if(argc != 3){
		printf("usage: %s rule\n",argv[0]);
		printf("filename is an ASCII PPM file(type 3) with maximum value 255\n");
		printf("rule is a hex number beginning with 0x;Life is 0x1808\n");
		return -1;
	}
	Image *img = readData(argv[1]);
	uint32_t rule = strtol(argv[2],NULL,16);
	Image *nextState = life(img,rule);
	writeData(nextState);
	freeImage(nextState);
	freeImage(img);
}
