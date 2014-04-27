#pragma once

#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;
using namespace cv;

enum CardType {TYPE_SPADE = 0 , TYPE_HEART, TYPE_CLUB, TYPE_DIAMOND, 
	TYPE_RED_JOKER, TYPE_BLACK_JOKER, TYPE_UNKNOWN};

enum CardNum {NUM_3 = 0, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, NUM_10,
	NUM_J, NUM_Q, NUM_K, NUM_A, NUM_2, NUM_JOKER, NUM_UNKNOWN};

class Card
{
	public:
		int num;
		int type;

	public:
		Card()
		{
			num = NUM_UNKNOWN;
			type = TYPE_UNKNOWN;
		}
		string str()
		{
			string out;
			if(type == TYPE_SPADE) out = "(黑桃";
			else if(type == TYPE_HEART) out = "(红桃";
			else if(type == TYPE_CLUB) out = "(梅花";
			else if(type == TYPE_DIAMOND) out = "(方片";
			else if(type == TYPE_RED_JOKER) out = "(大";
			else if(type == TYPE_BLACK_JOKER) out = "(小";
			else out = "(未知";
			if(num == NUM_3) out += "3)";
			else if(num == NUM_4) out += "4)";
			else if(num == NUM_5) out += "5)";
			else if(num == NUM_6) out += "6)";
			else if(num == NUM_7) out += "7)";
			else if(num == NUM_8) out += "8)";
			else if(num == NUM_9) out += "9)";
			else if(num == NUM_10) out += "10)";
			else if(num == NUM_J) out += "J)";
			else if(num == NUM_Q) out += "Q)";
			else if(num == NUM_K) out += "K)";
			else if(num == NUM_A) out += "A)";
			else if(num == NUM_2) out += "2)";
			else if(num == NUM_JOKER) out += "王)";
			else out +="?)";
			return out;
		}
};

class Recog
{
	public:
		IplImage * image;

		vector<CvRect> typeBBox;
		vector<CvRect> numBBox;
		vector<Card> cards;

		string prefix;
		double first_thresh;
		double second_thresh;
		int shiftnum;

		vector<string> typeFiles;
		vector<string> blackNumFiles;
		vector<string> redNumFiles;

		Recog(string _prefix);
		vector<Card> recog_cards(IplImage * _image);
		void drawResult(IplImage * drawImage, int shiftx = -50);

	public:
		virtual void recog_bbox() = 0;
		virtual void recog_card_types();
		virtual void recog_card_nums();
};
