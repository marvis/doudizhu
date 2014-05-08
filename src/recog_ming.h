#ifndef __RECOG_MING_H__
#define __RECOG_MING_H__

#include "recog.h"

class RecogMing : public Recog
{
	public:
		string direction;

		RecogMing(string _prefix, string _direction);

	public:
		void recog_bbox();
};

#endif
