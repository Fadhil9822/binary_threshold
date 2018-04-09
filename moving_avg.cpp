#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

#define _USE_MATH_DEFINES
#include <math.h>
#include "morph_function_list.h"
#include <time.h>

static double* arr_scan;
static double* arr_mov_left;
static double* arr_mov_top;
static double* arr_mov_avg;

IplImage* moving_avg(IplImage* img)
{
	IplImage* filtered = img;
	int i, j, nIdx, mIdx;
	int k = 0; // Gray color
	int t;
	int size;
	double k_const;

	double mov_left;
	double mov_top;
	double mov_avg;

	printf(" Local Thresholding using Moving Average \n\n");

	double min = (double)min_of_img(img);
	double max = (double)max_of_img(img);

	printf(" Maximum of the image : %d\n", (unsigned char)max);
	printf(" minimum of the image : %d\n", (unsigned char)min);

	printf(" Input the number of moving N : \n");
	scanf("%d", &size);
	printf(" Input constant k : \n");
	scanf("%lf", &k_const);

	int m = filtered->width;
	int n = filtered->height;
	int sizeofimg = m*n;

	arr_scan = (double*)malloc(sizeof(double) * size);
	arr_mov_left = (double*)malloc(sizeof(double) * sizeofimg);
	arr_mov_top = (double*)malloc(sizeof(double) * sizeofimg);
	arr_mov_avg = (double*)malloc(sizeof(double) * sizeofimg);

	nIdx = filtered->widthStep; // y position increment -- transform space
	mIdx = filtered->nChannels; // x position increment


	clock_t before;
	double result;
	before = clock();

	// scan from left to right
	for (j = 0 ; j < filtered->height; j++)
	{
		for (i = 0, mov_left = 0; i < filtered->width; i++)
		{
			unsigned char p = img->imageData[j*nIdx + i*mIdx + k];
			
			if (i < size) // for zero padding from left
			{
				mov_left = mov_left + (double)p / (double)size;
				*(arr_mov_left + j*nIdx + i*mIdx + k) = mov_left;
			}
			else
			{
				unsigned char p20 = img->imageData[j*nIdx + (i - size + 2)*mIdx + k];
				mov_left = mov_left + (double)p/(double)size - (double)p20/(double)size;
				*(arr_mov_left + j*nIdx + i*mIdx + k) = mov_left;

			}
		}
	}

	// scan from top to bottom
	for (i = 0; i < filtered->width; i++)
	{
		for (j = 0, mov_top = 0; j < filtered->height; j++)
		{
			unsigned char p = img->imageData[j*nIdx + i*mIdx + k];
			if (j < size) // for zero padding from left
			{
				mov_top = mov_top + (double)p / (double)size;
				*(arr_mov_top + j*nIdx + i*mIdx + k) = mov_top;
			}
			else
			{
				unsigned char p20 = img->imageData[(j - size + 2)*nIdx + i*mIdx + k];
				mov_top = mov_top + (double)p / (double)size - (double)p20 /(double)size;
				*(arr_mov_top + j*nIdx + i*mIdx + k) = mov_top;
			}
		}
	}

	for (j = 0; j < filtered->height; j++)
	{
		for (i = 0; i < filtered->width; i++)
		{
			double p_left = *(arr_mov_left + j*nIdx + i*mIdx + k);
			double p_top = *(arr_mov_top + j*nIdx + i*mIdx + k);
			double p_avg = 0.5*(p_top + p_left);
			*(arr_mov_avg + j*nIdx + i*mIdx + k) = p_avg;
		}
	}

	for (j = 0; j < filtered->height; j++)
	{
		for (i = 0; i < filtered->width; i++)
		{
			double p_ref = *(arr_mov_avg + j*nIdx + i*mIdx + k);
			double p_tot = p_ref * k_const;
			
			unsigned char p_char = (unsigned char)p_tot;
			//if ( i == 200 && j == 200)
			//printf(" %d", p_char);

			unsigned char p_dic = (unsigned char)img->imageData[j*nIdx + i*mIdx + k];
			if ( p_dic > p_char)
				filtered->imageData[j*nIdx + i*mIdx + k] = 255;
			else
				filtered->imageData[j*nIdx + i*mIdx + k] = 0;
		}
	}

	result = (double)(clock() - before) / CLOCKS_PER_SEC;
	printf(" Time elapsed : %lf \n", result);

	cvNamedWindow("moving_average", CV_WINDOW_AUTOSIZE);
	cvShowImage("moving_average", filtered);
	cvSaveImage("moving_average.bmp", filtered);
	printf("\n Saved Successfully \n\n");

	display_img_info(filtered);

	return filtered;
}
	