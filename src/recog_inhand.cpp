#include "recog_inhand.h"

bool isPixelSame(IplImage * image, int x1, int y1, int x2, int y2, double thresh = 20)
{
	int R1 = CV_IMAGE_ELEM(image, unsigned char, y1, 3*x1+2);
	int G1 = CV_IMAGE_ELEM(image, unsigned char, y1, 3*x1+1);
	int B1 = CV_IMAGE_ELEM(image, unsigned char, y1, 3*x1+0);
	int R2 = CV_IMAGE_ELEM(image, unsigned char, y2, 3*x2+2);
	int G2 = CV_IMAGE_ELEM(image, unsigned char, y2, 3*x2+1);
	int B2 = CV_IMAGE_ELEM(image, unsigned char, y2, 3*x2+0);
	double diff = sqrt((R2-R1)*(R2-R1) + (G2-G1)*(G2-G1) + (B2-B1)*(B2-B1));
	return (diff < thresh);
}

bool isPixelSame(IplImage * image, int x1, int y1, CvScalar rgb, double thresh = 20)
{
	int R1 = CV_IMAGE_ELEM(image, unsigned char, y1, 3*x1+2);
	int G1 = CV_IMAGE_ELEM(image, unsigned char, y1, 3*x1+1);
	int B1 = CV_IMAGE_ELEM(image, unsigned char, y1, 3*x1+0);
	int R2 = (int) rgb.val[2];
	int G2 = (int) rgb.val[1];
	int B2 = (int) rgb.val[0];
	double diff = sqrt((R2-R1)*(R2-R1) + (G2-G1)*(G2-G1) + (B2-B1)*(B2-B1));
	return (diff < thresh);
}

RecogInhand::RecogInhand(string _prefix) : Recog(_prefix)
{
	first_thresh = 20;
	second_thresh = 50;
	shiftnum = 2;
}

void RecogInhand::recog_bbox()
{
	int i0 = 138;
	int top = -1;
	for(int j = 0; j < image->height; j++)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+0);
		double dist = sqrt((R - 239)*(R-239) + (G-239)*(G-239) + (B-239)*(B-239));
		if(dist < 30) 
		{
			double sumR = 0.0, sumG = 0.0, sumB = 0.0;
			for(int i = 72; i < 230; i++)
			{
				sumR += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+2);
				sumG += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+1);
				sumB += CV_IMAGE_ELEM(image, unsigned char, j, 3*i);
			}
			double avgR = sumR/158.0;
			double avgG = sumG/158.0;
			double avgB = sumB/158.0;
			double dist2 = sqrt((avgR - 239)*(avgR-239) + (avgG-239)*(avgG-239) + (avgB-239)*(avgB-239));
			if(dist2 < 30)
			{
				top = j;
				break;
			}
		}
	}
	int bot = -1;
	for(int j = image->height - 1; j >= 0; j--)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+0);
		double dist = sqrt((R - 239)*(R-239) + (G-239)*(G-239) + (B-239)*(B-239));
		if(dist < 30) 
		{
			double sumR = 0.0, sumG = 0.0, sumB = 0.0;
			for(int i = 72; i < 230; i++)
			{
				sumR += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+2);
				sumG += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+1);
				sumB += CV_IMAGE_ELEM(image, unsigned char, j, 3*i);
			}
			double avgR = sumR/158.0;
			double avgG = sumG/158.0;
			double avgB = sumB/158.0;
			double dist2 = sqrt((avgR - 239)*(avgR-239) + (avgG-239)*(avgG-239) + (avgB-239)*(avgB-239));
			if(dist2 < 30)
			{
				bot = j;
				break;
			}
		}
	}
	if(bot <= 639) return; // 由于对称, bot 不能小于中线, 同时bot会出错, 但不能超过第一张牌的最左边缘
	int num = 0;
	for(int j = top; j <= bot-150; j++)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+0);
		double avg = (R+G+B)/3.0;
		if(avg < 190)
		{
			num++;
			j += 20;
		}
	}
	double step = 66.66;
	int ncards = num + 1;//(bot - top - 150)/step + 1;
	if(ncards > 1) step = (bot - 159 - top)/(double)(ncards-1);

	for(int n = 0; n < ncards; n++)
	{
		int shift = 0;
		if(!isPixelSame(image, 70, top+n*step+16, CV_RGB(214, 214, 214), 20))
			shift = 18;

		int x1 = 180 + shift;
		int y1 = top + n * step + 2;
		int width1 = 46;
		int height1 = 51;
		numBBox.push_back(cvRect(x1, y1, width1 , height1));
		int x2 = 149 + shift;
		int y2 = top + n * step + 10;
		int width2 = 30;
		int height2 = 36;
		typeBBox.push_back(cvRect(x2, y2, width2 , height2));
	}
}
