#ifndef __RECOG_H__
#define __RECOG_H__

#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace std;
using namespace cv;
#define RED_T(x) ("\033[1;31m" + string(x) + "\033[0m")
#define BLK_T(x) ("\033[1;30m" + string(x) + "\033[0m")

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
		Card(int _num, int _type): num(_num), type(_type){}
		bool operator!=(const Card & other) const
		{
			return (this->num != other.num || this->type != other.type);
		}
		bool operator==(const Card & other) const
		{
			return (this->num == other.num && this->type == other.type);
		}
		bool operator<(const Card & other) const
		{
			if(this->num == NUM_UNKNOWN) return true;
			if(other.num == NUM_UNKNOWN) return false;
			if(this->num == NUM_JOKER && other.num == NUM_JOKER) 
			{
				if(this->type == other.type) return false;
				if(this->type == TYPE_RED_JOKER) return false;
				if(this->type == TYPE_BLACK_JOKER) return true;
			}
			if(this->num < other.num) return true;
			if(this->num > other.num) return false;
			
			if(this->type == TYPE_UNKNOWN) return true;
			if(other.type == TYPE_UNKNOWN) return false;
			if(this->type < other.type) return true;
			if(this->type > other.type) return false;
			return false;
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
		vector<string> disp()
		{
			string line1 = " -----  ";
			string line2 = "|     | ";
			string line3 = "|     | ";
			string line4 = "|     | ";
			string line5 = " -----  ";
			vector<string> matrix;
			string num_str = "     ", type_str = " ";
			if(num == NUM_3)      num_str = " 3   ";
			else if(num == NUM_4) num_str = " 4   ";
			else if(num == NUM_5) num_str = " 5   ";
			else if(num == NUM_6) num_str = " 6   ";
			else if(num == NUM_7) num_str = " 7   ";
			else if(num == NUM_8) num_str = " 8   ";
			else if(num == NUM_9) num_str = " 9   ";
			else if(num ==NUM_10) num_str = " 10  ";
			else if(num == NUM_J) num_str = " J   ";
			else if(num == NUM_Q) num_str = " Q   ";
			else if(num == NUM_K) num_str = " K   ";
			else if(num == NUM_A) num_str = " A   ";
			else if(num == NUM_2) num_str = " 2   ";
			else if(num == NUM_JOKER) num_str = "JOKER";
			else num_str =" ?   ";

			if(type == TYPE_SPADE) type_str = "♠";
			else if(type == TYPE_HEART) 
			{
				type_str = RED_T("♥");
				num_str = RED_T(num_str);
			}
			else if(type == TYPE_CLUB) type_str = "♣";
			else if(type == TYPE_DIAMOND) 
			{
				type_str = RED_T("♦");
				num_str = RED_T(num_str);
			}
			else if(type == TYPE_RED_JOKER) num_str = RED_T(num_str);
			else type_str = " ";
			line2 = "|" + num_str + "| ";
			line3 = "| " + type_str + "   | ";
			matrix.push_back(line1);
			matrix.push_back(line2);
			matrix.push_back(line3);
			matrix.push_back(line4);
			matrix.push_back(line5);
			return matrix;
		}
};

void disp_cards(vector<Card> & cards, string msg = "", int num = 0);
void disp_cards(vector<vector<Card> > & all_cards, vector<string> & all_msg, int num = 0);

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

		vector<string> tmpl_typeFiles;
		vector<string> tmpl_blackNumFiles;
		vector<string> tmpl_redNumFiles;

		Recog(string _prefix);
		vector<Card> recog_cards(IplImage * _image);
		void drawResult(IplImage * drawImage, int shiftx = -50);

	public:
		virtual void recog_bbox() = 0;
		virtual void recog_card_types();
		virtual void recog_card_nums();
};

#endif
