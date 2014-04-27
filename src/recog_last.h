#pragma once

#include "recog.h"

class RecogLast :public Recog
{
	public:
		RecogLast(string prefix);

	public:
		void recog_bbox();
		void recog_card_types();
};
