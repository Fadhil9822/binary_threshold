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

static double* arr_histogram;
static double* arr_subwindow;

IplImage* var_thre_local(IplImage* img)
{
	IplImage* filtered = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 1);
	IplImage* std = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 1);

	int i, j, nIdx, mIdx;
	int mx, my, m_ref_x, m_ref_y; // masking size
	int k = 0; // Gray scale only
	int t = 0; // array sweep

	unsigned char pm;
	unsigned int pm_int;
	double p_sum = 0;
	
	double pm_double;
	
	printf(" Variable thresholding based on local image properties \n\n");

	double min = (double)min_of_img(img);
	double max = (double)max_of_img(img);

	printf(" Maximum of the image : %d\n", (unsigned char)max);
	printf(" minimum of the image : %d\n", (unsigned char)min);

	int m = filtered->width;
	int n = filtered->height;

	double local_avg;
	double local_std;
	double local_var;
	unsigned char t_local;
	double a_const, b_const;

	nIdx = filtered->widthStep; // y position increment -- transform space
	mIdx = filtered->nChannels; // x position increment

	printf(" Input Mask Size of x : \n");
	scanf("%d", &m_ref_x);
	printf(" Input Mask Size of y : \n");
	scanf("%d", &m_ref_y);

	printf(" Input Costant A : \n");
	scanf("%lf", &a_const);
	printf(" Input Costant B : \n");
	scanf("%lf", &b_const);


	mx = m_ref_x;
	my = m_ref_y;

	int msize = mx*my;

	zero_set(filtered, 255);
	zero_set(std, 255);

	arr_histogram = (double*)malloc(sizeof(double) * 256); // intensity histogram
	arr_subwindow = (double*)malloc(sizeof(double) * msize); // subwindow array

	clock_t before;
	double result;
	before = clock();

	for (j = 0; j < filtered->height; j++)
	{
		for (i = 0; i < filtered->width; i++)
		{
			unsigned char p_char = img->imageData[j*nIdx + i*mIdx + k];
			double p = (double)p_char;

			p_sum += p/(double)m ;
		}
	}

	unsigned char p_avg = (unsigned char)((double)p_sum / (double)n);

	printf(" Global Average is : %d \n", p_avg);

	for (j = 0; j < filtered->height; j++)
	{
		for (i = 0; i < filtered->width; i++)
		{
			//if (j >= m_ref_y / 2 && i >= m_ref_x / 2 && j <= img->height - m_ref_y / 2 + 1 && i <= img->width - m_ref_x / 2 + 1) 
			{
				for (mx = (-m_ref_x / 2), t= 0; mx < (m_ref_x / 2) + 1; mx++ )
				{
					for (my = (-m_ref_y / 2); my < (m_ref_y / 2) + 1; my++, t++)
					{

						if (((j + my < (0) || i + mx < (0)) && (mx < 0 || my < 0)) || (((j + my >= filtered->height - (1)) || (i + mx >= filtered->width - (1))) && (mx >= 0 || my >= 0)))
							*(arr_subwindow + t) = 0; // zero padding
						else
						{
							pm = img->imageData[(j + my)*nIdx + (i + mx)*mIdx + k];
							*(arr_subwindow + t) = (double)pm;
						}
						
					}
				}

				for (t = 0, local_avg = 0; t < msize; t++)
				{
					// local average
					local_avg += *(arr_subwindow + t);
				}
				local_avg = local_avg / (double)msize;
			
				for (t = 0, local_var = 0; t < msize; t++)
				{	// local standard deviation
					local_var += ((*(arr_subwindow + t)) - local_avg) * ((*(arr_subwindow + t)) - local_avg);
					
				}
				local_var = local_var / (double)msize;
				local_std = sqrt(local_var);
				unsigned char std_dv = (unsigned char)local_std;

				unsigned char a = (unsigned char)(a_const*local_std);
				unsigned char b = (unsigned char)(b_const*(double)p_avg);
		
				pm = img->imageData[j*nIdx + i*mIdx + k];

				if (pm > a && pm > b)
					pm = 255;
				else
					pm = 0;

				filtered->imageData[j*nIdx + i*mIdx + k] = pm ;
				std->imageData[j*nIdx + i*mIdx + k] = std_dv ;
			}
		}
	}

	result = (double)(clock() - before) / CLOCKS_PER_SEC;
	printf(" Time elapsed : %lf \n", result);

	cvNamedWindow("var_the_local", CV_WINDOW_AUTOSIZE);
	cvShowImage("var_the_local", filtered);
	cvSaveImage("var_the_local.bmp", filtered);
	printf("\n Saved Successfully \n\n");

	cvNamedWindow("std", CV_WINDOW_AUTOSIZE);
	cvShowImage("std", std);
	cvSaveImage("std.bmp", std);
	printf("\n Saved Successfully \n\n");

	display_img_info(filtered);

	return filtered;
}