#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "path.h"
#include "common.h"

using namespace std;
using namespace cv;

IplImage * drawImage;
map<string, IplImage *> map_allImgs;

IplImage * getImage(string file)
{
	if(map_allImgs.find(file) != map_allImgs.end()) return map_allImgs[file];
	cerr<<"image "<<file<<" is not loaded as template"<<endl;

	IplImage * image = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	if(!image)
	{
		cerr<<"unable to load \""<<file<<"\""<<endl;
		return 0;
	}
	return image;
}

IplImage * cropImage(IplImage * src, int x, int y, int width, int height)
{
	cvSetImageROI(src, cvRect(x, y, width , height));
	IplImage * dst = cvCreateImage(cvSize(width,height), IPL_DEPTH_8U , src->nChannels);
	cvCopy(src, dst, 0);
    cvResetImageROI(src);
	return dst;
}

string num2str(int num)
{
	ostringstream oss;
	oss<<num;
	return oss.str();
}

void drawOnImage(IplImage * image, int x0, int y0, string file)
{
	IplImage * patchImg = getImage(file);
	if(!patchImg) return;
	int nchannels = image->nChannels;
	for(int j = 0; j < patchImg->height; j++)
	{
		for(int i = 0; i < patchImg->width; i++)
		{
			int jj = j + y0;
			int ii = i + x0;
			for(int c = 0; c < nchannels; c++)
			{
				CV_IMAGE_ELEM(image, unsigned char, jj, ii*nchannels + c) = CV_IMAGE_ELEM(patchImg, unsigned char, j, i*nchannels + c);
			}
		}
	}
}
bool isImageSame(IplImage * bigImage, int x0, int y0, string filename, double & diffval, double thresh, int shiftnum)
{
	diffval = 100000;
	IplImage * smallImage = getImage(filename);
	if(!smallImage) cerr<<filename<<" is unable to load"<<endl;
	if(!smallImage) return false;
	assert(bigImage);
	assert(bigImage->width >= smallImage->width && bigImage->height >= smallImage->height && bigImage->nChannels == smallImage->nChannels);
	int width = smallImage->width;
	int height = smallImage->height;
	int nchannels = bigImage->nChannels;
	assert(x0 >= 0 && y0 >= 0 && x0 + width <= bigImage->width && y0 + height <= bigImage->height);

	double sumdiff = 0;
	for(int s = 0; s <= shiftnum; s++)
	{
		sumdiff = 0;
		for(int y = s; y < height; y++)
		{
			for(int x = 0; x < width; x++)
			{
				int by = y0 + y - s;
				int bx = x0 + x;
				for(int c = 0; c < nchannels; c++)
				{
					double val1 = CV_IMAGE_ELEM(smallImage, unsigned char, y, x*nchannels + c);
					double val2 = CV_IMAGE_ELEM(bigImage, unsigned char, by, bx*nchannels + c);
					sumdiff += ABS(val1-val2);
				}
			}
		}
		double avgdiff = sumdiff/((height - s) * width);
		if(avgdiff < diffval) diffval = avgdiff;

		if (diffval < thresh) return true;

		if(s != 0)
		{
			sumdiff = 0;
			for(int y = 0; y < height - s; y++)
			{
				for(int x = 0; x < width; x++)
				{
					int by = y0 + y + s;
					int bx = x0 + x;
					for(int c = 0; c < nchannels; c++)
					{
						double val1 = CV_IMAGE_ELEM(smallImage, unsigned char, y, x*nchannels + c);
						double val2 = CV_IMAGE_ELEM(bigImage, unsigned char, by, bx*nchannels + c);
						sumdiff += ABS(val1-val2);
					}
				}
			}
			double avgdiff = sumdiff/((height - s) * width);
			if(avgdiff < diffval) diffval = avgdiff;

			if (diffval < thresh) return true;
		}
	}
	return false;
}

void loadAllTemplates()
{
	string file;
	string prefixs[] = {"last"};
	string nums[] = {"3", "5", "4", "6", "7", "8", "9", "10", "J", "Q", "K", "A", "2"};
	string types[] = {"spade", "heart", "club", "diamond", "rjoker", "bjoker"};
	for(int i = 0; i < sizeof(prefixs)/sizeof(string); i++)
	{
		string prefix = prefixs[i];
		for(int j = 0; j < sizeof(nums)/sizeof(string); j++)
		{
			string file = "ddz_" + prefix + "_num_b" + nums[j] + ".png";
			map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
			file = "ddz_" + prefix + "_num_r" + nums[j] + ".png";
			map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
		}
		for(int j = 0; j < sizeof(types)/sizeof(string); j++)
		{
			string file = "ddz_" + prefix + "_type_" + types[j] + ".png";
			map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
		}
	}

	for(map<string, IplImage*>::iterator it = map_allImgs.begin(); it != map_allImgs.end(); it++)
	{
		string file = it->first;
		IplImage * image = it->second;
		if(!image) cerr<<"unable to load template "<<file<<endl;
	}
}
