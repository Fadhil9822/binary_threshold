// 10, July, 2017 Digital Image Processing 3rd week
// This program was made by Kim Dong Hyun for Studying

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
#pragma warning(disable: 4819)

int main(int argc, char** argv)
{
	int sel;
	// load image
	IplImage* img = cvLoadImage("1043.tif");

	if (img == NULL)
	{
		printf(" could not find the image");
		return 0;
	}
	cvNamedWindow("img", CV_WINDOW_AUTOSIZE);
	cvShowImage("img", img);
	display_img_info(img);
	while (1)
	{
		printf(" \n-------------------------------------\n");
		printf("  input number to start processing\n");
		printf(" (1) Erosion \n");
		printf(" (2) Dilation \n");
		printf(" (3) Opening \n"); // it cares only for (0,1,0; 1,1,1,; 0,1,0)
		printf(" (4) Closing \n");
		printf(" (5) Global Threshold Basic \n");
		printf(" (6) Otsu's Method \n");
		printf(" (7) Local Variable Threshold  \n");
		printf(" (8) Image Partitioning \n");
		printf(" (9) Moving Average \n");


		int num;
		scanf("%d", &num);

		switch (num)
		{

		case 1:
			img = cvLoadImage("opening.tif");
			display_img_info(img);
			erosion_binary(img);
			break;

		case 2:
			img = cvLoadImage("Erosion_Binary.bmp");
			display_img_info(img);
			dilation_binary(img);
			break;

		case 3:
			img = cvLoadImage("dilation_binary.bmp");
			dilation_binary(img);
			break;

		case 4:
			img = cvLoadImage("dilation_binary.bmp");
			display_img_info(img);
			erosion_binary(img);
			break;

		case 5 :
			img = cvLoadImage("1049.tif", 0);
			display_img_info(img);
			threshold_global_basic(img);
			break;

		case 6:
			img = cvLoadImage("1043.tif", 0);
			display_img_info(img);
			otsu(img);
			break;

		case 7 :
			img = cvLoadImage("1048.tif", 0);
			display_img_info(img);
			var_thre_local(img);
			break;

		case 8 :
			img = cvLoadImage("1046.tif", 0);
			display_img_info(img);
			img_partition(img);
			break;

		case 9 :
			img = cvLoadImage("1049.tif", 0);
			display_img_info(img);
			moving_avg(img);
			break;

		default :
			printf(" selected process does not exist\n");
			break;

		}
	}
}