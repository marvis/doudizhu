#pragma once

#include "recog.h"

class RecogInhand :public Recog
{
	public:
		RecogInhand(string _prefix);

	public:
		void recog_bbox();
};
