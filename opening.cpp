
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

IplImage* opening(IplImage* img)
{

	//IplImage* filtered = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 3);
	IplImage* filtered = img;

	int i, j, nIdx, mIdx;
	int mx, my, m_ref_x, m_ref_y; // masking size
	int k; // RGB color sweep
	int t;  // array sweep
	unsigned char pm;
	unsigned int pm_int;
	double pm_double;
	k = 0;

	printf(" Opening \n");
	printf(" Input Mask Size of x : \n");
	scanf("%d", &m_ref_x);
	printf(" Input Mask Size of y : \n");
	scanf("%d", &m_ref_y);

	mx = m_ref_x;
	my = m_ref_y;

	int msize = mx*my;
	zero_set(filtered, 0);

	arr = (int*)malloc(sizeof(int)*mx*my);
	
	for (j = 0; j < filtered->height; j++)
	{
		for (i = 0; i < filtered->width; i++)
		{
			//for (k = 0; k < 3; k++)
			//{
				nIdx = filtered->widthStep; // y position increment -- transform space
				mIdx = filtered->nChannels; // x position increment

				pm = 0;
				pm_double = 0;
				t = 0;

				if (j >= m_ref_y / 2 && i >= m_ref_x / 2 && j <= img->height - m_ref_y / 2 + 1 && i <= img->width - m_ref_x / 2 + 1)
				{
					for (mx = (-m_ref_x / 2); mx < (m_ref_x / 2) + 1; mx++)
					{
						for (my = (-m_ref_y / 2); my < (m_ref_y / 2) + 1; my++)
						{
							pm = img->imageData[(j + my)*nIdx + (i + mx)*mIdx + k];
							*(arr + t) = (int)pm;
							t++;
							if (t == msize)
								break;
						}
					}

					for (t = 0; t < msize; t++)
					{
						if (*(arr + t) == 0)
						{
							filtered->imageData[j*nIdx + i*mIdx + k] = 0;
							break;
						}
						else
							filtered->imageData[j*nIdx + i*mIdx + k] = 255;
					}
				}
			//}
		}
	}

	for (j = 0; j < filtered->height; j++)
	{
		for (i = 0; i < filtered->width; i++)
		{
			for (k = 0; k < 3; k++)
			{
				nIdx = filtered->widthStep; // y position increment -- transform space
				mIdx = filtered->nChannels; // x position increment

				pm = 0;
				pm_double = 0;
				t = 0;

				if (j >= m_ref_y / 2 && i >= m_ref_x / 2 && j <= img->height - m_ref_y / 2 + 1 && i <= img->width - m_ref_x / 2 + 1)
				{
					for (mx = (-m_ref_x / 2); mx < (m_ref_x / 2) + 1; mx++)
					{
						for (my = (-m_ref_y / 2); my < (m_ref_y / 2) + 1; my++)
						{
							pm = filtered->imageData[(j + my)*nIdx + (i + mx)*mIdx + k];
							*(arr + t) = (int)pm;
							t++;
							if (t == msize)
								break;
						}
					}

					for (t = 0; t < msize; t++)
					{
						if (*(arr + 2 * t + 1) == 255)
						{
							filtered->imageData[j*nIdx + i*mIdx + k] = 255;
							break;
						}
						else
							filtered->imageData[j*nIdx + i*mIdx + k] = 0;

					}

					if (*(arr + 4) == 255)
					{
						filtered->imageData[j*nIdx + i*mIdx + k] = 255;
						break;
					}
					else
						filtered->imageData[j*nIdx + i*mIdx + k] = 0;


					
				}
			}
		}
	}


	cvNamedWindow("opening", CV_WINDOW_AUTOSIZE);
	cvShowImage("opening", filtered);
	cvSaveImage("opening.bmp", filtered);
	printf("\n Saved Successfully \n\n");

	// display image information
	display_img_info(filtered);

	return filtered;
}