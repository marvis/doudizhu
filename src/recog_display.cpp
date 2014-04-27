// 识别明牌
#include "common.h"
#include "recog_display.h"

RecogDisplay::RecogDisplay(string _prefix): Recog(_prefix)
{
}

void RecogDisplay::recog_bbox()
{
	double step = 34.5;
	int y0 = 40;
	int count = 0;
	while(1)
	{
		if(count > 10) break;

		double sumdiff = 0;
		int  y = y0 + count*step;
		for(int x = 569; x < 569 + 31; x++)
		{
			int R = CV_IMAGE_ELEM(image, unsigned char, y, 3*x+2);
			int G = CV_IMAGE_ELEM(image, unsigned char, y, 3*x+1);
			int B = CV_IMAGE_ELEM(image, unsigned char, y, 3*x+0);
			double diff = ABS(R - 255) + ABS(G - 255) + ABS(B - 255);// sqrt((R - 255)*(R-255) + (G-255)*(G-255) + (G-255)*(G-255));
			sumdiff += diff;
		}
		//cout<<count<<": "<<sumdiff/31<<endl;
		if(sumdiff/31 < 10) count++;
		else break;
	}
	count--;
	if(count == 0) return;
	CvRect rect0 = cvRect(582, 42, 19, 27); // for num
	CvRect rect1 = cvRect(565, 45, 17, 22); // for type
	for(int i = 0; i < count; i++)
	{
		numBBox.push_back(cvRect(rect0.x, rect0.y + i*step, rect0.width, rect0.height));
		typeBBox.push_back(cvRect(rect1.x, rect1.y + i*step, rect1.width, rect1.height));
	}
}
