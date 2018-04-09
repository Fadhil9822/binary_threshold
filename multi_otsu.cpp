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

static int* arr;
static double *arr2; // for within class variance
static double* arr3; // for inter class variance
static double* arr4;
static double* arr5;

IplImage* otsu(IplImage* img)
{
	IplImage* filtered = img;
	int i, j, nIdx, mIdx;
	int k = 0; // Gray color
	double p_avg_b, p_avg_f;
	double p_wgt_b, p_wgt_f;
	double p_var_b, p_var_f;
	int p_num_b, p_num_f;
	int p;
	int t1, t2;

	printf(" Otsu's Method \n\n");

	double min = (double)min_of_img(img);
	double max = (double)max_of_img(img);

	printf(" Maximum of the image : %d\n", (unsigned char)max);
	printf(" minimum of the image : %d\n", (unsigned char)min);

	arr = (int*)malloc(sizeof(int) * 256); // intensity histogram
	arr2 = (double*)malloc(sizeof(double) * 256); // for within class variance
	arr3 = (double*)malloc(sizeof(double) * 256); // for inter class variance

	arr4 = (double*)malloc(sizeof(double) * 256); // for within class variance SORTED
	arr5 = (double*)malloc(sizeof(double) * 256); // for inter class variance SORTED

	int m = filtered->width;
	int n = filtered->height;

	nIdx = filtered->widthStep; // y position increment -- transform space
	mIdx = filtered->nChannels; // x position increment

	clock_t before;
	double result;
	before = clock();

	for (p = 0; p < 256; p++)
	{
		*(arr + p) = 0;
	}
	for (j = 0; j < filtered->height; j++)
	{
		for (i = 0; i < filtered->width; i++)
		{
			unsigned char p_char = filtered->imageData[j*nIdx + i*mIdx + k];
			p = (int)p_char;
			*(arr + p) = *(arr + p) + 1;
		}
	}



	for (t1 = 0; t1 < t2; t1++)
	{
		// Class 1

		for (p = 0, p_num_b = 0; p < t1 + 1; p++) // number of pixel
		{
			p_num_b += *(arr + p);
		}

		for (p = 0, p_wgt_b = 0; p < t1 + 1; p++) // weight
		{
			p_wgt_b += (double)(*(arr + p)) / (double)(m*n);
		}


		for (p = 0, p_avg_b = 0; p < t1 + 1; p++) // mean
		{
			if (p_num_b == 0)
			{
				p_avg_b = 0;
			}
			else
				p_avg_b += (double)(*(arr + p) * p) / (double)(p_num_b);
		}


		for (p = 0, p_var_b = 0; p < t1 + 1; p++) // variance
		{
			if (p_num_b == 0)
			{
				p_avg_b = 0;
			}
			else
				p_var_b += pow(((double)p - p_avg_b), 2)*(double)(*(arr + p)) / (double)p_num_b;
		}

		// Class 2

		for (p = t1, p_num_f = 0; p < t2; p++) // number of pixel
		{
			p_num_f += *(arr + p);
		}

		for (p = t1, p_wgt_f = 0; p < t2; p++) // weight
		{
			p_wgt_f += (double)*(arr + p) / (double)(m*n);
		}

		for (p = t1, p_avg_f = 0; p < t2; p++) // mean
		{
			if (p_num_f == 0)
			{
				p_avg_f = 0;
			}
			else
				p_avg_f += (double)(*(arr + p) * p) / (double)(p_num_f);
		}

		for (p = t1, p_var_f = 0; p < t2; p++) // variance
		{
			if (p_num_f == 0)
			{
				p_avg_f = 0;
			}
			else
				p_var_f += pow(((double)p - p_avg_f), 2)*(double)(*(arr + p)) / (double)p_num_f;
		}


		// Class 3

		for (p = t2, p_num_f = 0; p < 256; p++) // number of pixel
		{
			p_num_f += *(arr + p);
		}

		for (p = t2, p_wgt_f = 0; p < 256; p++) // weight
		{
			p_wgt_f += (double)*(arr + p) / (double)(m*n);
		}

		for (p = t2, p_avg_f = 0; p < 256; p++) // mean
		{
			if (p_num_f == 0)
			{
				p_avg_f = 0;
			}
			else
				p_avg_f += (double)(*(arr + p) * p) / (double)(p_num_f);
		}

		for (p = t2, p_var_f = 0; p < 256; p++) // variance
		{
			if (p_num_f == 0)
			{
				p_avg_f = 0;
			}
			else
				p_var_f += pow(((double)p - p_avg_f), 2)*(double)(*(arr + p)) / (double)p_num_f;
		}

		*(arr3 + t) = p_wgt_b * p_wgt_f * (p_avg_b - p_avg_f) * (p_avg_b - p_avg_f);
		*(arr5 + t) = p_wgt_b * p_wgt_f * (p_avg_b - p_avg_f) * (p_avg_b - p_avg_f);
	}


	//arr4 = bubble_double(256, arr4); 
	arr5 = bubble_double(256, arr5);

	for (p = 0; p < 256; p++)
	{
		//	printf( " %lf " , *(arr3 + p));
		//	printf(" %lf \n", *(arr5 + p));
	}

	for (p = 0; p < 256; p++)
	{
		*(arr + p) = 0;
	}
	double thr_var = *(arr5 + 254);

	printf(" selected threshold variance is : %lf \n ", thr_var);
	double thr_var_d = thr_var;
	for (t = 0; t <256; t++)
	{
		if (*(arr3 + t) == thr_var_d)
			break;
	}

	printf(" selected T is %d ", t);

	for (j = 0; j < filtered->height; j++)
	{
		for (i = 0; i < filtered->width; i++)
		{
			unsigned char p_char = img->imageData[j*nIdx + i*mIdx + k];

			if (p_char <= (unsigned char)t)
				p_char = 0;

			else if (p_char >(unsigned char)t)
				p_char = 255;

			filtered->imageData[j*nIdx + i*mIdx + k] = p_char;
		}
	}

	result = (double)(clock() - before) / CLOCKS_PER_SEC;
	printf(" Time elapsed : %lf \n", result);

	cvNamedWindow("otsu", CV_WINDOW_AUTOSIZE);
	cvShowImage("otsu", filtered);
	cvSaveImage("otsu.bmp", filtered);
	printf("\n Saved Successfully \n\n");

	display_img_info(filtered);

	return filtered;
}