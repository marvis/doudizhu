#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "recog.h"
#include "common.h"
using namespace std;
using namespace cv;

extern string infile;

void disp_cards(vector<Card> & cards, string msg)
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
	if(msg != "")
	{
		if(line1.size() <= msg.size()) line1 = msg;
		else for(int i = 0; i < msg.size(); i++) line1[i] = msg[i];
	}
	cout<<line1<<endl;
	cout<<line2<<endl;
	cout<<line3<<endl;
	cout<<line4<<endl;
	cout<<line5<<endl;
}

void disp_played_cards(vector<vector<Card> > & all_cards, vector<string> &all_msg)
{
	assert(all_cards.size() == all_msg.size());
	string line1 = "";
	string line2 = "";
	string line3 = "";
	string line4 = "";
	string line5 = "";
	int ncards = 0;
	for(int i = 0; i < all_cards.size(); i++)
	{
		vector<Card> & cards = all_cards[i];
		string & msg = all_msg[i];
		ncards += cards.size();

		string line11 = "";
		string line12 = "";
		string line13 = "";
		string line14 = "";
		string line15 = "";
		for(int i = 0; i < cards.size(); i++)
		{
			vector<string> matrix = cards[i].disp(); 
			line11 += matrix[0];
			line12 += matrix[1];
			line13 += matrix[2];
			line14 += matrix[3];
			line15 += matrix[4];
		}
		if(msg != "")
		{
			int n = line11.size() < msg.size() ? line11.size() : msg.size();
			for(int i = 0; i < n; i++) line11[i] = msg[i];
		}
		line1 += line11;
		line2 += line12;
		line3 += line13;
		line4 += line14;
		line5 += line15;
	}
	if(ncards == 0) return;
	cout<<line1<<endl;
	cout<<line2<<endl;
	cout<<line3<<endl;
	cout<<line4<<endl;
	cout<<line5<<endl;
}
void disp_played_cards(vector<Card> & cards1, string msg1, vector<Card> & cards2, string msg2)
{
	if(cards1.empty() && cards2.empty()) return;
	string line1 = "";
	string line2 = "";
	string line3 = "";
	string line4 = "";
	string line5 = "";
	if(!cards1.empty())
	{
		string line11 = "";
		string line12 = "";
		string line13 = "";
		string line14 = "";
		string line15 = "";
		for(int i = 0; i < cards1.size(); i++)
		{
			vector<string> matrix = cards1[i].disp(); 
			line11 += matrix[0];
			line12 += matrix[1];
			line13 += matrix[2];
			line14 += matrix[3];
			line15 += matrix[4];
		}
		if(msg1 != "")
		{
			int n = line11.size() < msg1.size() ? line11.size() : msg1.size();
			for(int i = 0; i < n; i++) line11[i] = msg1[i];
		}
		line1 += line11;
		line2 += line12;
		line3 += line13;
		line4 += line14;
		line5 += line15;
	}
	if(!cards2.empty())
	{
		string line11 = "";
		string line12 = "";
		string line13 = "";
		string line14 = "";
		string line15 = "";
		for(int i = 0; i < cards2.size(); i++)
		{
			vector<string> matrix = cards2[i].disp(); 
			line11 += matrix[0];
			line12 += matrix[1];
			line13 += matrix[2];
			line14 += matrix[3];
			line15 += matrix[4];
		}
		if(msg2 != "")
		{
			int n = line11.size() < msg2.size() ? line11.size() : msg2.size();
			for(int i = 0; i < n; i++) line11[i] = msg2[i];
		}
		line1 += line11;
		line2 += line12;
		line3 += line13;
		line4 += line14;
		line5 += line15;
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

	tmpl_typeFiles.clear();
	tmpl_typeFiles.push_back("ddz_" + prefix + "_type_spade.png");
	tmpl_typeFiles.push_back("ddz_" + prefix + "_type_heart.png");
	tmpl_typeFiles.push_back("ddz_" + prefix + "_type_club.png");
	tmpl_typeFiles.push_back("ddz_" + prefix + "_type_diamond.png");
	tmpl_typeFiles.push_back("ddz_" + prefix + "_type_rjoker.png");
	tmpl_typeFiles.push_back("ddz_" + prefix + "_type_bjoker.png");

	tmpl_blackNumFiles.clear();
	tmpl_blackNumFiles.push_back("ddz_" + prefix + "_num_b3.png");
	tmpl_blackNumFiles.push_back("ddz_" + prefix + "_num_b4.png");
	tmpl_blackNumFiles.push_back("ddz_" + prefix + "_num_b5.png");
	tmpl_blackNumFiles.push_back("ddz_" + prefix + "_num_b6.png");
	tmpl_blackNumFiles.push_back("ddz_" + prefix + "_num_b7.png");
	tmpl_blackNumFiles.push_back("ddz_" + prefix + "_num_b8.png");
	tmpl_blackNumFiles.push_back("ddz_" + prefix + "_num_b9.png");
	tmpl_blackNumFiles.push_back("ddz_" + prefix + "_num_b10.png");
	tmpl_blackNumFiles.push_back("ddz_" + prefix + "_num_bJ.png");
	tmpl_blackNumFiles.push_back("ddz_" + prefix + "_num_bQ.png");
	tmpl_blackNumFiles.push_back("ddz_" + prefix + "_num_bK.png");
	tmpl_blackNumFiles.push_back("ddz_" + prefix + "_num_bA.png");
	tmpl_blackNumFiles.push_back("ddz_" + prefix + "_num_b2.png");

	tmpl_redNumFiles.clear();
	tmpl_redNumFiles.push_back("ddz_" + prefix + "_num_r3.png");
	tmpl_redNumFiles.push_back("ddz_" + prefix + "_num_r4.png");
	tmpl_redNumFiles.push_back("ddz_" + prefix + "_num_r5.png");
	tmpl_redNumFiles.push_back("ddz_" + prefix + "_num_r6.png");
	tmpl_redNumFiles.push_back("ddz_" + prefix + "_num_r7.png");
	tmpl_redNumFiles.push_back("ddz_" + prefix + "_num_r8.png");
	tmpl_redNumFiles.push_back("ddz_" + prefix + "_num_r9.png");
	tmpl_redNumFiles.push_back("ddz_" + prefix + "_num_r10.png");
	tmpl_redNumFiles.push_back("ddz_" + prefix + "_num_rJ.png");
	tmpl_redNumFiles.push_back("ddz_" + prefix + "_num_rQ.png");
	tmpl_redNumFiles.push_back("ddz_" + prefix + "_num_rK.png");
	tmpl_redNumFiles.push_back("ddz_" + prefix + "_num_rA.png");
	tmpl_redNumFiles.push_back("ddz_" + prefix + "_num_r2.png");
}

// shiftx < 0, shift to left
// shiftx > 0, shift to right
void Recog::drawResult(IplImage * drawImage, int shiftx)
{
	for(int i = 0; i < typeBBox.size(); i++)
	{
		CvRect rect = typeBBox[i];
		cvRectangle(drawImage, cvPoint(rect.x, rect.y), cvPoint(rect.x+rect.width, rect.y + rect.height), CV_RGB(0, 0, 255), 1);
		if(cards[i].type != TYPE_UNKNOWN) drawOnImage(drawImage, rect.x + shiftx, rect.y, tmpl_typeFiles[cards[i].type]);

		rect = numBBox[i];
		cvRectangle(drawImage, cvPoint(rect.x, rect.y), cvPoint(rect.x+rect.width, rect.y + rect.height), CV_RGB(0, 255, 0), 1);
		if(cards[i].type != NUM_UNKNOWN)
		{
			if(isblack(cards[i].type))
				drawOnImage(drawImage, rect.x + shiftx, rect.y, tmpl_blackNumFiles[cards[i].num]);
			else if(isred(cards[i].type))
				drawOnImage(drawImage, rect.x + shiftx, rect.y, tmpl_redNumFiles[cards[i].num]);
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
			if(isImageSame(image, rect.x, rect.y, tmpl_typeFiles[type], diffval, first_thresh, shiftnum)) 
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

		vector<string> & numFiles = isblack(cards[i].type) ? tmpl_blackNumFiles : tmpl_redNumFiles;
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
