
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

static int* arr;

IplImage* img_partition(IplImage* img)
{
	int x_part, y_part;
	int i, j;
	int k = 0; // gray scale image

	
	// part into 6 pieces 
	IplImage* part1 = cvCreateImage(cvSize(271, 325), IPL_DEPTH_8U, 1); // gray scale image
	IplImage* part2 = cvCreateImage(cvSize(271, 325), IPL_DEPTH_8U, 1);
	IplImage* part3 = cvCreateImage(cvSize(271, 325), IPL_DEPTH_8U, 1);
	IplImage* part4 = cvCreateImage(cvSize(271, 325), IPL_DEPTH_8U, 1);
	IplImage* part5 = cvCreateImage(cvSize(271, 325), IPL_DEPTH_8U, 1);
	IplImage* part6 = cvCreateImage(cvSize(271, 325), IPL_DEPTH_8U, 1);

	IplImage* filtered = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 1);
	zero_set(filtered, 0);

	int nIdx = part1->widthStep; // y position increment -- transform space
	int mIdx = part1->nChannels; // x position increment

	int wIdx = img->widthStep;
	int vIdx = img->nChannels;

	for (j = 0; j < 325; j++)
	{
		for (i = 0; i < 271; i++)
		{
			
			{
				part1->imageData[j*nIdx + i*mIdx + k] = img->imageData[j*wIdx + i*vIdx + k];
				part2->imageData[j*nIdx + i*mIdx + k] = img->imageData[j*wIdx + (i + 271)*vIdx + k];
				part3->imageData[j*nIdx + i*mIdx + k] = img->imageData[j*wIdx + (i + 542)*vIdx + k];
				part4->imageData[j*nIdx + i*mIdx + k] = img->imageData[(j + 325)*wIdx  + i*vIdx + k];
				part5->imageData[j*nIdx + i*mIdx + k] = img->imageData[(j + 325)*wIdx  + (i + 271)*vIdx + k];
				part6->imageData[j*nIdx + i*mIdx + k] = img->imageData[(j + 325)*wIdx  + (i + 542)*vIdx + k];
			}
		}
	}

	cvNamedWindow("part1", CV_WINDOW_AUTOSIZE);
	cvShowImage("part1", part1);
	cvSaveImage("part1.bmp", part1);

	cvNamedWindow("part2", CV_WINDOW_AUTOSIZE);
	cvShowImage("part2", part2);
	cvSaveImage("part2.bmp", part2);

	cvNamedWindow("part3", CV_WINDOW_AUTOSIZE);
	cvShowImage("part3", part3);
	cvSaveImage("part3.bmp", part3);

	cvNamedWindow("part4", CV_WINDOW_AUTOSIZE);
	cvShowImage("part4", part4);
	cvSaveImage("part4.bmp", part4);

	cvNamedWindow("part5", CV_WINDOW_AUTOSIZE);
	cvShowImage("part5", part5);
	cvSaveImage("part5.bmp", part5);

	cvNamedWindow("part6", CV_WINDOW_AUTOSIZE);
	cvShowImage("part6", part6);
	cvSaveImage("part6.bmp", part6);
	
	
	part1 = otsu(part1);
	part2 = otsu(part2);
	part3 = otsu(part3);
	part4 = otsu(part4);
	part5 = otsu(part5);
	part6 = otsu(part6);

	for (j = 0; j < part1->height; j++)
	{
		for (i = 0; i < part1->width ; i++)
		{

			{
				filtered->imageData[j*wIdx + i*vIdx + k] = part1->imageData[j*nIdx + i*mIdx + k];
				filtered->imageData[j*wIdx + (i + 271)*vIdx + k] = part2->imageData[j*nIdx + i*mIdx + k];
				filtered->imageData[j*wIdx + (i + 542)*vIdx + k] = part3->imageData[j*nIdx + i*mIdx + k];
				filtered->imageData[(j + 325)*wIdx + i*vIdx + k] = part4->imageData[j*nIdx + i*mIdx + k];
				filtered->imageData[(j + 325)*wIdx + (i + 271)*vIdx + k] = part5->imageData[j*nIdx + i*mIdx + k];
				filtered->imageData[(j + 325)*wIdx + (i + 542)*vIdx + k] = part6->imageData[j*nIdx + i*mIdx + k];
			}
		}
	}

	cvNamedWindow("filtered", CV_WINDOW_AUTOSIZE);
	cvShowImage("filtered", filtered);
	cvSaveImage("partition.bmp", filtered);

	return img;
}