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


int avg_of_img(IplImage* img)
{
	int max = 0;
	int i, j, k, nIdx, mIdx;
	double avg_d = 0;

	for (j = 0; j < img->height; j++)
	{
		for (i = 0; i < img->width; i++)
		{
			for (k = 0; k < 3; k++)
			{
				nIdx = img->widthStep; // y position increment -- transform space
				mIdx = img->nChannels; // x position increment

				unsigned char tmp = img->imageData[j*nIdx + i*mIdx + k];
				avg_d += tmp;

			}
		}
	}
	return max;
}