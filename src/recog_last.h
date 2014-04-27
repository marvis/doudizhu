#pragma once

#include "recog.h"

class RecogLast :public Recog
{
	public:
		RecogLast(string prefix);
	private:
		void recog_bbox();
};
