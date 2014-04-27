#pragma once

#include "recog.h"

struct SearchPara
{
	int play_leftx;
	int play_rightx;
	int play_top_most;
	int play_bot_most;

	int play_midx;
	int play_step;
	int play_card_height;

	int play_num_gap;
	int play_num_leftx;
	int play_num_width;
	int play_num_height;
	
	int play_type_gap;
	int play_type_leftx;
	int play_type_width;
	int play_type_height;
};

class RecogPlay :public Recog
{
	public:
		SearchPara para;
		RecogPlay(string _prefix, string direction);

	public:
		void recog_bbox();
		void recog_card_nums();
};
