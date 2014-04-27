#pragma once
#include "recog.h"

class RecogDisplay : public Recog
{
	public:
		RecogDisplay(string _prefix);

	public:
		void recog_bbox();
};
