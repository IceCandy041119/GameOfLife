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
	int isAliveR[8],isAliveG[8],isAliveB[8];
	int aliveNeighoursR[8] = {0},aliveNeighoursG[8] = {0},aliveNeighoursB[8] = {0};
	int dx[8] = {0,0,1,-1,-1,1,1,-1};
	int dy[8] = {1,-1,0,0,-1,1,-1,1};
	
	for(int i = 7;i >= 0;i--){
		isAliveR[i] = ((*(image->image + row * (image->cols) + col))->R & (1<<i)) != 0;
		isAliveG[i] = ((*(image->image + row * (image->cols) + col))->G & (1<<i)) != 0;
		isAliveB[i] = ((*(image->image + row * (image->cols) + col))->B & (1<<i)) != 0;
	}

	for(int i = 7;i >= 0;i--){
		int nextRow = ((row + dx[i] + image->rows) % image->rows);	
		int nextCol = ((col + dy[i] + image->cols) % image->cols);	
		for(int R = 7;R >= 0;R--){
			if((*(image->image + nextRow * (image->cols) + nextCol))->R & (1 << R)){
				aliveNeighoursR[R]++;
			}
		}
		for(int G = 7;G >= 0;G--){
			if((*(image->image + nextRow * (image->cols) + nextCol))->G & (1 << G)){
				aliveNeighoursG[G]++;
			}
		}
		for(int B = 7;B >= 0;B--){
			if((*(image->image + nextRow * (image->cols) + nextCol))->B & (1 << B)){
				aliveNeighoursB[B]++;
			}
		}
	}
	
	for(int i = 7;i >= 0;i--){
		if((1 << (aliveNeighoursR[i] + (9*isAliveR[i]))) & rule){
			nextState->R |= 1 << i;
		}else{
			nextState->R &= 0 << i;
		}
		if((1 << (aliveNeighoursG[i] + (9*isAliveG[i]))) & rule){
			nextState->G |= 1 << i;
		}else{
			nextState->G &= 0 << i;
		}
		if((1 << (aliveNeighoursB[i] + (9*isAliveB[i]))) & rule){
			nextState->B |= 1 << i;
		}else{
			nextState->B &= 0 << i;
		}
	}
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
