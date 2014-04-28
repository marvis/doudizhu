// 识别明牌
#include "common.h"
#include "recog_display.h"

RecogDisplay::RecogDisplay(string _prefix, string _direction): Recog(_prefix)
{
	first_thresh = 20;
	second_thresh = 60;
	shiftnum = 2;
	direction = _direction;
}

void RecogDisplay::recog_bbox()
{
	if(direction == "left")
	{
		double step = 34.5;
		int y0 = 40;
		int count = 0;
		while(1)
		{
			if(count > 10) break;

			double sumdiff = 0;
			int  y = y0 + count*step;
			for(int x = 569; x < 569 + 28; x++)
			{
				int R = CV_IMAGE_ELEM(image, unsigned char, y, 3*x+2);
				int G = CV_IMAGE_ELEM(image, unsigned char, y, 3*x+1);
				int B = CV_IMAGE_ELEM(image, unsigned char, y, 3*x+0);
				double diff = ABS(R - 255) + ABS(G - 255) + ABS(B - 255);// sqrt((R - 255)*(R-255) + (G-255)*(G-255) + (G-255)*(G-255));
				sumdiff += diff;
			}
			//cout<<count<<": "<<sumdiff/28<<endl;
			if(sumdiff/28 < 20) count++;
			else break;
		}
		count--;
		if(count <= 0) return;
		CvRect rect0 = cvRect(583, 42, 19, 27); // for num
		CvRect rect1 = cvRect(568, 45, 14, 22); // for type
		for(int i = 0; i < count; i++)
		{
			numBBox.push_back(cvRect(rect0.x, rect0.y + i*step, rect0.width, rect0.height));
			typeBBox.push_back(cvRect(rect1.x, rect1.y + i*step, rect1.width, rect1.height));
		}

		if(count == 10)
		{
			count = 0;
			while(1)
			{
				if(count > 10) break;

				double sumdiff = 0;
				int  y = y0 + count*step;
				for(int x = 520; x < 520 + 28; x++)
				{
					int R = CV_IMAGE_ELEM(image, unsigned char, y, 3*x+2);
					int G = CV_IMAGE_ELEM(image, unsigned char, y, 3*x+1);
					int B = CV_IMAGE_ELEM(image, unsigned char, y, 3*x+0);
					double diff = ABS(R - 255) + ABS(G - 255) + ABS(B - 255);// sqrt((R - 255)*(R-255) + (G-255)*(G-255) + (G-255)*(G-255));
					sumdiff += diff;
				}
				//cout<<count<<": "<<sumdiff/28<<endl;
				if(sumdiff/28 < 20) count++;
				else break;
			}
			count--;
			if(count <= 0) return;
			CvRect rect0 = cvRect(534, 42, 19, 27); // for num
			CvRect rect1 = cvRect(519, 45, 14, 22); // for type
			for(int i = 0; i < count; i++)
			{
				numBBox.push_back(cvRect(rect0.x, rect0.y + i*step, rect0.width, rect0.height));
				typeBBox.push_back(cvRect(rect1.x, rect1.y + i*step, rect1.width, rect1.height));
			}
		}
	}
	else if(direction == "right")
	{
		double step = 34.5;
		int y0 = 1201;
		int count = 0;
		while(1)
		{
			if(count > 10) break;

			double sumdiff = 0;
			int  y = y0 - count*step;
			for(int x = 569; x < 569 + 28; x++)
			{
				int R = CV_IMAGE_ELEM(image, unsigned char, y, 3*x+2);
				int G = CV_IMAGE_ELEM(image, unsigned char, y, 3*x+1);
				int B = CV_IMAGE_ELEM(image, unsigned char, y, 3*x+0);
				double diff = ABS(R - 255) + ABS(G - 255) + ABS(B - 255);// sqrt((R - 255)*(R-255) + (G-255)*(G-255) + (G-255)*(G-255));
				sumdiff += diff;
			}
			//cout<<y<<" "<<count<<" "<<sumdiff/28<<endl;
			if(sumdiff/28 < 20) count++;
			else break;
		}
		//count--; // different from left side
		if(count <= 0) return;
		CvRect rect0 = cvRect(583, 1203, 19, 27); // for num
		CvRect rect1 = cvRect(568, 1206, 14, 22); // for type
		for(int i = count-1; i >= 0; i--)
		{
			numBBox.push_back(cvRect(rect0.x, rect0.y - i*step, rect0.width, rect0.height));
			typeBBox.push_back(cvRect(rect1.x, rect1.y - i*step, rect1.width, rect1.height));
		}
		if(count == 10)
		{
			count = 0;
			while(1)
			{
				if(count > 10) break;

				double sumdiff = 0;
				int  y = y0 - count*step;
				for(int x = 520; x < 520 + 28; x++)
				{
					int R = CV_IMAGE_ELEM(image, unsigned char, y, 3*x+2);
					int G = CV_IMAGE_ELEM(image, unsigned char, y, 3*x+1);
					int B = CV_IMAGE_ELEM(image, unsigned char, y, 3*x+0);
					double diff = ABS(R - 255) + ABS(G - 255) + ABS(B - 255);// sqrt((R - 255)*(R-255) + (G-255)*(G-255) + (G-255)*(G-255));
					sumdiff += diff;
				}
				//cout<<y<<" "<<count<<" "<<sumdiff/28<<endl;
				if(sumdiff/28 < 20) count++;
				else break;
			}
			//count--; // different from left side
			//cout<<"count = "<<count<<endl;
			if(count <= 0) return;
			CvRect rect0 = cvRect(534, 1203, 19, 27); // for num
			CvRect rect1 = cvRect(519, 1206, 14, 22); // for type
			for(int i = count-1; i >= 0; i--)
			{
				numBBox.push_back(cvRect(rect0.x, rect0.y - i*step, rect0.width, rect0.height));
				typeBBox.push_back(cvRect(rect1.x, rect1.y - i*step, rect1.width, rect1.height));
			}

		}

	}
	else
	{
		cerr<<"unknown direction"<<endl;
	}
}
