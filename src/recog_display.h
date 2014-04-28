#pragma once
#include "recog.h"

class RecogDisplay : public Recog
{
	public:
		string direction;

		RecogDisplay(string _prefix, string _direction);

	public:
		void recog_bbox();
};
