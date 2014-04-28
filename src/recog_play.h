#pragma once

#include "recog.h"

struct SearchPara
{
	int top_most;
	int bot_most;
	int check_left;
	int check_right;

	int step;
	int card_height;

	int num_gap;
	int num_leftx;
	int num_width;
	int num_height;
	
	int type_gap;
	int type_leftx;
	int type_width;
	int type_height;
};

class RecogPlay :public Recog
{
	public:
		string direction;
		RecogPlay(string _prefix, string direction);

	public:
		int numCards(SearchPara para);
		void recog_bbox();
		void recog_bbox(SearchPara para);
		//void recog_card_nums();
};
