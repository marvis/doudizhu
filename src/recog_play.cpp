#include "recog_play.h"
#include "common.h"

extern string infile;
SearchPara myself_para = {
	270, 994, 258, 336, 
	40, 68,
	2, 305, 27, 32,
	7, 285, 19, 22
};

SearchPara left_para_down = {
	285, 640, 368, 390, 
	40, 68,
	2, 410, 27, 32,
	7, 390, 19, 22
};
SearchPara left_para_mid = {
	285, 640, 391, 469,
	40, 68, 
	2, 438, 27, 32, 
	7, 418, 19, 22
};
SearchPara left_para_up = {
	285, 640, 445, 480, 
	40, 68,
	2, 466, 27, 32,
	7, 446, 19, 22
};
SearchPara right_para_down = {
	640, 1000, 368, 390, 
	40, 68,
	2, 410, 27, 32,
	7, 390, 19, 22
};
SearchPara right_para_mid = {
	640, 1000, 391, 469, 
	40, 68,
	2, 438, 27, 32,
	7, 418, 19, 22
};
SearchPara right_para_up = {
	640, 1000, 445, 480, 
	40, 68,
	2, 466, 27, 32,
	7, 446, 19, 22
};

RecogPlay::RecogPlay(string _prefix, string _direction) : Recog(_prefix)
{
	first_thresh = 20;
	second_thresh = 55;
	shiftnum = 2;
	direction = _direction;
}

int RecogPlay::numCards(SearchPara _para)
{
	int top = -1;
	int bot = -1;
	int i0 = (_para.check_left + _para.check_right)/2;
	for(int j = _para.top_most; j < _para.bot_most; j++)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+0);
		double dist = sqrt((R - 239)*(R-239) + (G-239)*(G-239) + (B-239)*(B-239));
		if(dist < 30) 
		{
			double sumR = 0.0, sumG = 0.0, sumB = 0.0;
			for(int i = _para.check_left; i < _para.check_right; i++)
			{
				sumR += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+2);
				sumG += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+1);
				sumB += CV_IMAGE_ELEM(image, unsigned char, j, 3*i);
			}
			double avgR = sumR/(_para.check_right - _para.check_left);
			double avgG = sumG/(_para.check_right - _para.check_left);
			double avgB = sumB/(_para.check_right - _para.check_left);
			double dist2 = sqrt((avgR - 239)*(avgR-239) + (avgG-239)*(avgG-239) + (avgB-239)*(avgB-239));
			if(dist2 < 30)
			{
				top = j;
				break;
			}
		}
	}
	for(int j = _para.bot_most-1; j >= _para.top_most; j--)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+0);
		double dist = sqrt((R - 239)*(R-239) + (G-239)*(G-239) + (B-239)*(B-239));
		if(dist < 30) 
		{
			double sumR = 0.0, sumG = 0.0, sumB = 0.0;
			for(int i = _para.check_left; i < _para.check_right; i++)
			{
				sumR += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+2);
				sumG += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+1);
				sumB += CV_IMAGE_ELEM(image, unsigned char, j, 3*i);
			}
			double avgR = sumR/(_para.check_right - _para.check_left);
			double avgG = sumG/(_para.check_right - _para.check_left);
			double avgB = sumB/(_para.check_right - _para.check_left);
			double dist2 = sqrt((avgR - 239)*(avgR-239) + (avgG-239)*(avgG-239) + (avgB-239)*(avgB-239));
			if(dist2 < 30)
			{
				bot = j;
				break;
			}
		}
	}

	int step = _para.step;

	int ncards = (bot > top) ?  (bot - top - _para.card_height + 2)/step + 1 : 0;
	if(bot > top && ncards == 0) ncards = 1;
	//cout<<"ncards = "<<ncards<<endl;
	return ncards;
}

void RecogPlay::recog_bbox()
{
	if(direction == "myself") return recog_bbox(myself_para);
	else if(direction == "left") 
	{
		if(numCards(left_para_down) > 0) 
		{
			recog_bbox(left_para_up);
			recog_bbox(left_para_down);
		}
		else recog_bbox(left_para_mid);
	}
	else if(direction == "right")
	{
		if(numCards(right_para_down) > 0) 
		{
			recog_bbox(right_para_up);
			recog_bbox(right_para_down);
		}
		else recog_bbox(right_para_mid);
	}
}
void RecogPlay::recog_bbox(SearchPara para)
{
	int top = -1;
	int bot = -1;
	int i0 = (para.check_left + para.check_right)/2;
	for(int j = para.top_most; j < para.bot_most; j++)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+0);
		double dist = sqrt((R - 239)*(R-239) + (G-239)*(G-239) + (B-239)*(B-239));
		if(dist < 30) 
		{
			double sumR = 0.0, sumG = 0.0, sumB = 0.0;
			for(int i = para.check_left; i < para.check_right; i++)
			{
				sumR += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+2);
				sumG += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+1);
				sumB += CV_IMAGE_ELEM(image, unsigned char, j, 3*i);
			}
			double avgR = sumR/(para.check_right - para.check_left);
			double avgG = sumG/(para.check_right - para.check_left);
			double avgB = sumB/(para.check_right - para.check_left);
			double dist2 = sqrt((avgR - 239)*(avgR-239) + (avgG-239)*(avgG-239) + (avgB-239)*(avgB-239));
			if(dist2 < 30)
			{
				top = j;
				break;
			}
		}
	}
	for(int j = para.bot_most-1; j >= para.top_most; j--)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+0);
		double dist = sqrt((R - 239)*(R-239) + (G-239)*(G-239) + (B-239)*(B-239));
		if(dist < 30) 
		{
			double sumR = 0.0, sumG = 0.0, sumB = 0.0;
			for(int i = para.check_left; i < para.check_right; i++)
			{
				sumR += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+2);
				sumG += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+1);
				sumB += CV_IMAGE_ELEM(image, unsigned char, j, 3*i);
			}
			double avgR = sumR/(para.check_right - para.check_left);
			double avgG = sumG/(para.check_right - para.check_left);
			double avgB = sumB/(para.check_right - para.check_left);
			double dist2 = sqrt((avgR - 239)*(avgR-239) + (avgG-239)*(avgG-239) + (avgB-239)*(avgB-239));
			if(dist2 < 30)
			{
				bot = j;
				break;
			}
		}
	}

	int step = para.step;
	int ncards = (bot - top - para.card_height + 2)/step + 1;// : 0;
	//cout<<"ncards = "<<ncards<<endl;
	if(bot > top && ncards == 0) ncards = 1;
	for(int n = 0; n < ncards; n++)
	{
		int x1 = para.num_leftx;//438;
		int y1 = top + n * step + para.num_gap;
		int width1 = para.num_width;//27;
		int height1 = para.num_height;//34;
		numBBox.push_back(cvRect(x1, y1, width1, height1));
		int x2 = para.type_leftx;//418;
		int y2 = top + n * step + para.type_gap;
		int width2 = para.type_width;//19;
		int height2 = para.type_height;//23;
		typeBBox.push_back(cvRect(x2, y2, width2, height2));
	}
}

/*void RecogPlay::recog_card_nums()
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
*/
