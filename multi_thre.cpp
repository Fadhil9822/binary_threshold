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


IplImage* multi_thre(IplImage* img)
{
	IplImage* filtered = img;
	int i, j, nIdx, mIdx;
	int k = 0; // Gray color
	int g1, g2; // the number of pixel in G1, G2 
	double t1, t2;
	double dt = 0;

	printf(" Multiple Threshold \n\n");

	double min = (double)min_of_img(img);
	double max = (double)max_of_img(img);

	printf(" Maximum of the image : %d\n", (unsigned char)max);
	printf(" minimum of the image : %d\n", (unsigned char)min);

	printf(" Input Threshold values T1 : \n");
	scanf("%lf", &t1);

	printf(" Input Threshold values T2 : \n");
	scanf("%lf", &t2);

	clock_t before;
	double result;
	before = clock();



	for (j = 0; j < filtered->height; j++)
	{
		for (i = 0; i < filtered->width; i++)
		{

			nIdx = filtered->widthStep; // y position increment -- transform space
			mIdx = filtered->nChannels; // x position increment

			unsigned char p_char = filtered->imageData[j*nIdx + i*mIdx + k];
			int p = (int)p_char;

			if (p > (unsigned char) t1)
			{
				filtered->imageData[j*nIdx + i*mIdx + k] = 255;
			}
			else if (p <= (unsigned char) t1)
			{
				filtered->imageData[j*nIdx + i*mIdx + k] = 0;
			}
			else
				filtered->imageData[j*nIdx + i*mIdx + k] = 127;

		}
	}

	result = (double)(clock() - before) / CLOCKS_PER_SEC;
	printf(" Time elapsed : %lf \n", result);


	cvNamedWindow("Global_Threshold_Basic", CV_WINDOW_AUTOSIZE);
	cvShowImage("Global_Threshold_Basic", filtered);
	cvSaveImage("Global_Threshold_Basic.bmp", filtered);
	printf("\n Saved Successfully \n\n");

	display_img_info(filtered);

	return filtered;
}