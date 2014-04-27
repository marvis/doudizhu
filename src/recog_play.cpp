#include "recog_play.h"
#include "common.h"

extern string infile;
RecogPlay::RecogPlay(string _prefix, string direction) : Recog(_prefix)
{
	first_thresh = 20;
	second_thresh = 55;
	shiftnum = 2;

	if(direction == "myself")
	{
		SearchPara _para = {
			258, 336, 270, 994,
			283, 40, 68,
			2, 305, 27, 32,
			7, 285, 19, 22
		};
		para = _para;
	}
	else if(direction == "left")
	{
		SearchPara _para = {
			391, 469, 285, 640,
			437, 40, 68, 
			2, 438, 27, 32, 
			7, 418, 19, 22
		};
		para = _para;
	}
	else if(direction == "right")
	{
		SearchPara _para = {
			391, 469, 640, 1000,
			437, 40, 68,
			2, 438, 27, 32,
			7, 418, 19, 22
		};
		para = _para;
	}
}

void RecogPlay::recog_bbox()
{
	int top = -1;
	int bot = -1;
	int i0 = para.play_midx;
	for(int j = para.play_top_most; j < para.play_bot_most; j++)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+0);
		double dist = sqrt((R - 239)*(R-239) + (G-239)*(G-239) + (B-239)*(B-239));
		if(dist < 30) 
		{
			double sumR = 0.0, sumG = 0.0, sumB = 0.0;
			for(int i = para.play_leftx; i <= para.play_rightx; i++)
			{
				sumR += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+2);
				sumG += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+1);
				sumB += CV_IMAGE_ELEM(image, unsigned char, j, 3*i);
			}
			double avgR = sumR/(para.play_rightx - para.play_leftx + 1);
			double avgG = sumG/(para.play_rightx - para.play_leftx + 1);
			double avgB = sumB/(para.play_rightx - para.play_leftx + 1);
			double dist2 = sqrt((avgR - 239)*(avgR-239) + (avgG-239)*(avgG-239) + (avgB-239)*(avgB-239));
			if(dist2 < 30)
			{
				top = j;
				break;
			}
		}
	}
	for(int j = para.play_bot_most-1; j >= para.play_top_most; j--)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+0);
		double dist = sqrt((R - 239)*(R-239) + (G-239)*(G-239) + (B-239)*(B-239));
		if(dist < 30) 
		{
			double sumR = 0.0, sumG = 0.0, sumB = 0.0;
			for(int i = para.play_leftx; i <= para.play_rightx; i++)
			{
				sumR += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+2);
				sumG += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+1);
				sumB += CV_IMAGE_ELEM(image, unsigned char, j, 3*i);
			}
			double avgR = sumR/(para.play_rightx - para.play_leftx + 1);
			double avgG = sumG/(para.play_rightx - para.play_leftx + 1);
			double avgB = sumB/(para.play_rightx - para.play_leftx + 1);
			double dist2 = sqrt((avgR - 239)*(avgR-239) + (avgG-239)*(avgG-239) + (avgB-239)*(avgB-239));
			if(dist2 < 30)
			{
				bot = j;
				break;
			}
		}
	}

	int step = para.play_step;
	int ncards = (bot - top - para.play_card_height + 2)/step + 1;
	//cout<<"ncards = "<<ncards<<endl;
	if(bot > top && ncards == 0) ncards = 1;
	for(int n = 0; n < ncards; n++)
	{
		int x1 = para.play_num_leftx;//438;
		int y1 = top + n * step + para.play_num_gap;
		int width1 = para.play_num_width;//27;
		int height1 = para.play_num_height;//34;
		numBBox.push_back(cvRect(x1, y1, width1, height1));
		int x2 = para.play_type_leftx;//418;
		int y2 = top + n * step + para.play_type_gap;
		int width2 = para.play_type_width;//19;
		int height2 = para.play_type_height;//23;
		typeBBox.push_back(cvRect(x2, y2, width2, height2));
	}
}

void RecogPlay::recog_card_nums()
{
	Recog::recog_card_nums();
	return;
	for(int i = 0; i < cards.size(); i++)
	{
		Rect rect = numBBox[i];
		if(cards[i].num == NUM_6 || cards[i].num == NUM_8)
		{
			IplImage * unknownImage = cropImage(image, rect.x, rect.y, rect.width, rect.height);
			cvSaveImage((infile + ".ddz_" + prefix + "_type_unknown" + num2str(i) + ".png").c_str(), unknownImage);
			cvReleaseImage(&unknownImage);
		}
	}
}
