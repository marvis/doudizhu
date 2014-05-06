#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "recog.h"
#include "common.h"
using namespace std;
using namespace cv;

extern string infile;

void disp_cards(vector<Card> & cards)
{
	string line1 = "";
	string line2 = "";
	string line3 = "";
	string line4 = "";
	string line5 = "";
	for(int i = 0; i < cards.size(); i++)
	{
		vector<string> matrix = cards[i].disp(); 
		line1 += matrix[0];
		line2 += matrix[1];
		line3 += matrix[2];
		line4 += matrix[3];
		line5 += matrix[4];
	}
	cout<<line1<<endl;
	cout<<line2<<endl;
	cout<<line3<<endl;
	cout<<line4<<endl;
	cout<<line5<<endl;
}

bool isblack(int type)
{
	return (type == TYPE_SPADE || type == TYPE_CLUB);
}
bool isred(int type)
{
	return (type == TYPE_HEART || type == TYPE_DIAMOND);
}

Recog::Recog(string _prefix) : prefix(_prefix)
{
	image = 0;
	first_thresh = 10;
	second_thresh = 40;
	shiftnum = 0;

	typeFiles.clear();
	typeFiles.push_back("ddz_" + prefix + "_type_spade.png");
	typeFiles.push_back("ddz_" + prefix + "_type_heart.png");
	typeFiles.push_back("ddz_" + prefix + "_type_club.png");
	typeFiles.push_back("ddz_" + prefix + "_type_diamond.png");
	typeFiles.push_back("ddz_" + prefix + "_type_rjoker.png");
	typeFiles.push_back("ddz_" + prefix + "_type_bjoker.png");

	blackNumFiles.clear();
	blackNumFiles.push_back("ddz_" + prefix + "_num_b3.png");
	blackNumFiles.push_back("ddz_" + prefix + "_num_b4.png");
	blackNumFiles.push_back("ddz_" + prefix + "_num_b5.png");
	blackNumFiles.push_back("ddz_" + prefix + "_num_b6.png");
	blackNumFiles.push_back("ddz_" + prefix + "_num_b7.png");
	blackNumFiles.push_back("ddz_" + prefix + "_num_b8.png");
	blackNumFiles.push_back("ddz_" + prefix + "_num_b9.png");
	blackNumFiles.push_back("ddz_" + prefix + "_num_b10.png");
	blackNumFiles.push_back("ddz_" + prefix + "_num_bJ.png");
	blackNumFiles.push_back("ddz_" + prefix + "_num_bQ.png");
	blackNumFiles.push_back("ddz_" + prefix + "_num_bK.png");
	blackNumFiles.push_back("ddz_" + prefix + "_num_bA.png");
	blackNumFiles.push_back("ddz_" + prefix + "_num_b2.png");

	redNumFiles.clear();
	redNumFiles.push_back("ddz_" + prefix + "_num_r3.png");
	redNumFiles.push_back("ddz_" + prefix + "_num_r4.png");
	redNumFiles.push_back("ddz_" + prefix + "_num_r5.png");
	redNumFiles.push_back("ddz_" + prefix + "_num_r6.png");
	redNumFiles.push_back("ddz_" + prefix + "_num_r7.png");
	redNumFiles.push_back("ddz_" + prefix + "_num_r8.png");
	redNumFiles.push_back("ddz_" + prefix + "_num_r9.png");
	redNumFiles.push_back("ddz_" + prefix + "_num_r10.png");
	redNumFiles.push_back("ddz_" + prefix + "_num_rJ.png");
	redNumFiles.push_back("ddz_" + prefix + "_num_rQ.png");
	redNumFiles.push_back("ddz_" + prefix + "_num_rK.png");
	redNumFiles.push_back("ddz_" + prefix + "_num_rA.png");
	redNumFiles.push_back("ddz_" + prefix + "_num_r2.png");
}

// shiftx < 0, shift to left
// shiftx > 0, shift to right
void Recog::drawResult(IplImage * drawImage, int shiftx)
{
	for(int i = 0; i < typeBBox.size(); i++)
	{
		CvRect rect = typeBBox[i];
		cvRectangle(drawImage, cvPoint(rect.x, rect.y), cvPoint(rect.x+rect.width, rect.y + rect.height), CV_RGB(0, 0, 255), 1);
		if(cards[i].type != TYPE_UNKNOWN) drawOnImage(drawImage, rect.x + shiftx, rect.y, typeFiles[cards[i].type]);

		rect = numBBox[i];
		cvRectangle(drawImage, cvPoint(rect.x, rect.y), cvPoint(rect.x+rect.width, rect.y + rect.height), CV_RGB(0, 255, 0), 1);
		if(cards[i].type != NUM_UNKNOWN)
		{
			if(isblack(cards[i].type))
				drawOnImage(drawImage, rect.x + shiftx, rect.y, blackNumFiles[cards[i].num]);
			else if(isred(cards[i].type))
				drawOnImage(drawImage, rect.x + shiftx, rect.y, redNumFiles[cards[i].num]);
		}

	}
}

vector<Card> Recog::recog_cards(IplImage * _image)
{
	image = _image;
	
	recog_bbox();
	cards.resize(numBBox.size());

	recog_card_types();
	recog_card_nums();
	return cards;
}

void Recog::recog_card_types()
{
	for(int i = 0; i < typeBBox.size(); i++)
	{
		CvRect rect = typeBBox[i];
		vector<double> diffvals;
		double diffval;
		vector<double>::iterator min_elem;

		for(int type = TYPE_SPADE; type <= TYPE_BLACK_JOKER; type++)
		{
			if(isImageSame(image, rect.x, rect.y, typeFiles[type], diffval, first_thresh, shiftnum)) 
			{
				cards[i].type = type;
				break;
			}
			diffvals.push_back(diffval);
		}
	
		if(diffvals.size() == 6)
		{
			min_elem =  min_element(diffvals.begin(), diffvals.end());
			if(*min_elem < second_thresh) cards[i].type = (int)(min_elem - diffvals.begin()) + TYPE_SPADE;
			else
			{
				if(prefix != "last" || i != 2) // otherwise, it will be judged again
				{
					IplImage * unknownImage = cropImage(image, rect.x, rect.y, rect.width, rect.height);
					cvSaveImage((infile + ".ddz_" + prefix + "_type_unknown" + num2str(i) + ".png").c_str(), unknownImage);
					cvReleaseImage(&unknownImage);
				}
			}
		}
	}
}

void Recog::recog_card_nums()
{
	for(int i = 0; i < numBBox.size(); i++)
	{
		CvRect rect = numBBox[i];
		vector<double> diffvals;
		double diffval;
		vector<double>::iterator min_elem;
		if(cards[i].type == TYPE_UNKNOWN) continue;
		if(cards[i].type == TYPE_RED_JOKER || cards[i].type == TYPE_BLACK_JOKER)
		{
			cards[i].num = NUM_JOKER;
			continue;
		}

		vector<string> & numFiles = isblack(cards[i].type) ? blackNumFiles : redNumFiles;
		for(int num = NUM_3; num <= NUM_2; num++)
		{
			if(isImageSame(image, rect.x, rect.y, numFiles[num], diffval, first_thresh, shiftnum)) 
			{
				cards[i].num = num;
				break;
			}
			diffvals.push_back(diffval);
		}
		if(diffvals.size() == 13)
		{
			min_elem =  min_element(diffvals.begin(), diffvals.end());
			if(*min_elem < second_thresh) cards[i].num = (int)(min_elem - diffvals.begin()) + NUM_3;
			else
			{
				IplImage * unknownImage = cropImage(image, rect.x, rect.y, rect.width, rect.height);
				cvSaveImage((infile + ".ddz_" + prefix + "_num_unknown" + num2str(i) + ".png").c_str(), unknownImage); 
				cvReleaseImage(&unknownImage); 
			}
		}
	}
}
