#include "recog_last.h"

RecogLast::RecogLast(string _prefix) : Recog(_prefix)
{
}

void RecogLast::recog_bbox()
{
	numBBox.clear();
	typeBBox.clear();
	numBBox.push_back(cvRect(661, 33, 27, 28));
	numBBox.push_back(cvRect(661, 65, 27, 28));
	numBBox.push_back(cvRect(661, 97, 27, 28));
	typeBBox.push_back(cvRect(644, 40, 17, 21));
	typeBBox.push_back(cvRect(644, 72, 17, 21));
	typeBBox.push_back(cvRect(644, 104, 17, 21));
}
