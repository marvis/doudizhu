#ifndef __RECOG_INHAND_H__
#define __RECOG_INHAND_H__

#include "recog.h"

class RecogInhand :public Recog
{
	public:
		RecogInhand(string _prefix);

	public:
		void recog_bbox();
};
#endif
