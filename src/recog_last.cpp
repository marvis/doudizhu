#include "recog_last.h"
#include "common.h"

extern string infile;

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

void RecogLast::recog_card_types()
{
	Recog::recog_card_types();

	int i = 2;

	if(0)
	{
		CvRect rect = typeBBox[2];
		IplImage * unknownImage = cropImage(image, rect.x, rect.y, rect.width, rect.height);
		cvSaveImage((infile + ".ddz_" + prefix + "_type_unknown" + num2str(i) + ".png").c_str(), unknownImage);
		cvReleaseImage(&unknownImage);
	}

	if(cards[i].type == TYPE_UNKNOWN)
	{
		vector<string> typeFiles2;
		typeFiles2.push_back("ddz_" + prefix + "_type_spade_2.png");
		typeFiles2.push_back("ddz_" + prefix + "_type_heart_2.png");
		typeFiles2.push_back("ddz_" + prefix + "_type_club_2.png");
		typeFiles2.push_back("ddz_" + prefix + "_type_diamond_2.png");
		//typeFiles2.push_back("ddz_" + prefix + "_type_rjoker_2.png"); // no need, because type_rjoker is very small
		//typeFiles2.push_back("ddz_" + prefix + "_type_bjoker_2.png");

		CvRect rect = typeBBox[i];

		vector<double> diffvals;
		double diffval;
		vector<double>::iterator min_elem;

		for(int type = TYPE_SPADE; type <= TYPE_DIAMOND; type++)
		{
			if(isImageSame(image, rect.x, rect.y, typeFiles2[type], diffval, first_thresh, shiftnum)) 
			{
				cards[i].type = type;
				break;
			}
			diffvals.push_back(diffval);
		}
	
		if(diffvals.size() == 4) // not 6
		{
			min_elem =  min_element(diffvals.begin(), diffvals.end());
			if(*min_elem < second_thresh) cards[i].type = (int)(min_elem - diffvals.begin()) + TYPE_SPADE;
			else
			{
				IplImage * unknownImage = cropImage(image, rect.x, rect.y, rect.width, rect.height);
				cvSaveImage((infile + ".ddz_" + prefix + "_type_unknown" + num2str(i) + ".png").c_str(), unknownImage);
				cvReleaseImage(&unknownImage);
			}
		}
	}
}
