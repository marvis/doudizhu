#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <string>
#include <cassert>
#include <map>
#include "path.h"

using namespace std;
using namespace cv;
#define ABS(x) ((x) > 0 ? (x) : (-(x)))

string infile;
enum {PLAYER_ME = 0, PLAYER_LEFT, PLAYER_RIGHT, PLAYER_UNKNOWN,
	TYPE_SPADE , TYPE_HEART, TYPE_CLUB, TYPE_DIAMOND, TYPE_RED_JOKER, TYPE_BLACK_JOKER, TYPE_UNKNOWN,
	NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9, NUM_10, NUM_J, NUM_Q, NUM_K, NUM_A, NUM_2, NUM_JOKER, NUM_UNKNOWN,
};

class Card
{
	public:
		int num;
		int type;

	public:
		Card()
		{
			num = NUM_UNKNOWN;
			type = TYPE_UNKNOWN;
		}
		string str()
		{
			string out;
			if(type == TYPE_SPADE) out = "(黑桃";
			else if(type == TYPE_HEART) out = "(红桃";
			else if(type == TYPE_CLUB) out = "(梅花";
			else if(type == TYPE_DIAMOND) out = "(方片";
			else if(type == TYPE_RED_JOKER) out = "(大";
			else if(type == TYPE_BLACK_JOKER) out = "(小";
			else out = "(未知";
			if(num == NUM_3) out += "3)";
			else if(num == NUM_4) out += "4)";
			else if(num == NUM_5) out += "5)";
			else if(num == NUM_6) out += "6)";
			else if(num == NUM_7) out += "7)";
			else if(num == NUM_8) out += "8)";
			else if(num == NUM_9) out += "9)";
			else if(num == NUM_10) out += "10)";
			else if(num == NUM_J) out += "J)";
			else if(num == NUM_Q) out += "Q)";
			else if(num == NUM_K) out += "K)";
			else if(num == NUM_A) out += "A)";
			else if(num == NUM_2) out += "2)";
			else if(num == NUM_JOKER) out += "王)";
			else out +="?)";
			return out;
		}
};

map<string, IplImage *> map_allImgs;
typedef pair<string, IplImage *> PAIR;
void loadAllTemplates()
{
	string file;
	file = "ddz_mycards_num_b3.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_num_b4.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_num_b5.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_num_b6.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_num_b7.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_num_b8.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_num_b9.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_num_b10.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_num_bJ.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_num_bQ.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_num_bK.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_num_bA.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_num_b2.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);

	file = "ddz_mycards_num_r3.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_num_r4.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_num_r5.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_num_r6.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_num_r7.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_num_r8.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_num_r9.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_num_r10.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_num_rJ.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_num_rQ.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_num_rK.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_num_rA.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_num_r2.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);

	file = "ddz_mycards_type_spade.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_type_heart.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_type_club.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_type_diamond.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);

	file = "ddz_mycards_red_joker.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_mycards_black_joker.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);

	// play cards
	file = "ddz_play_num_b3.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_num_b4.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_num_b5.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_num_b6.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_num_b7.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_num_b8.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_num_b9.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_num_b10.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_num_bJ.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_num_bQ.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_num_bK.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_num_bA.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_num_b2.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);

	file = "ddz_play_num_r3.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_num_r4.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_num_r5.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_num_r6.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_num_r7.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_num_r8.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_num_r9.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_num_r10.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_num_rJ.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_num_rQ.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_num_rK.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_num_rA.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_num_r2.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);

	file = "ddz_play_type_spade.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_type_heart.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_type_club.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_type_diamond.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);

	file = "ddz_play_red_joker.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	file = "ddz_play_black_joker.png"; map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);


	for(map<string, IplImage*>::iterator it = map_allImgs.begin(); it != map_allImgs.end(); it++)
	{
		string file = it->first;
		IplImage * image = it->second;
		if(!image) cerr<<"unable to load template "<<file<<endl;
	}
}

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
bool isPixelSame(IplImage * image, int x1, int y1, int x2, int y2, double thresh = 20)
{
	int R1 = CV_IMAGE_ELEM(image, unsigned char, y1, 3*x1+2);
	int G1 = CV_IMAGE_ELEM(image, unsigned char, y1, 3*x1+1);
	int B1 = CV_IMAGE_ELEM(image, unsigned char, y1, 3*x1+0);
	int R2 = CV_IMAGE_ELEM(image, unsigned char, y2, 3*x2+2);
	int G2 = CV_IMAGE_ELEM(image, unsigned char, y2, 3*x2+1);
	int B2 = CV_IMAGE_ELEM(image, unsigned char, y2, 3*x2+0);
	double diff = sqrt((R2-R1)*(R2-R1) + (G2-G1)*(G2-G1) + (B2-B1)*(B2-B1));
	return (diff < thresh);
}

bool isPixelSame(IplImage * image, int x1, int y1, CvScalar rgb, double thresh = 20)
{
	int R1 = CV_IMAGE_ELEM(image, unsigned char, y1, 3*x1+2);
	int G1 = CV_IMAGE_ELEM(image, unsigned char, y1, 3*x1+1);
	int B1 = CV_IMAGE_ELEM(image, unsigned char, y1, 3*x1+0);
	int R2 = (int) rgb.val[2];
	int G2 = (int) rgb.val[1];
	int B2 = (int) rgb.val[0];
	double diff = sqrt((R2-R1)*(R2-R1) + (G2-G1)*(G2-G1) + (B2-B1)*(B2-B1));
	return (diff < thresh);
}

double avgImageDiff(IplImage * bigImage, int x0, int y0, string filename)
{
	IplImage * smallImage = getImage(filename);// cvLoadImage(filename.c_str(), 1);
	if(!smallImage) return false;
	assert(bigImage);
	assert(bigImage->width >= smallImage->width && bigImage->height >= smallImage->height && bigImage->nChannels == smallImage->nChannels);
	int width = smallImage->width;
	int height = smallImage->height;
	int nchannels = bigImage->nChannels;
	assert(x0 >= 0 && y0 >= 0 && x0 + width <= bigImage->width && y0 + height <= bigImage->height);

	double sumdiff = 0;
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			int by = y0 + y ;
			int bx = x0 + x;
			for(int c = 0; c < nchannels; c++)
			{
				double val1 = CV_IMAGE_ELEM(smallImage, unsigned char, y, x*nchannels + c);
				double val2 = CV_IMAGE_ELEM(bigImage, unsigned char, by, bx*nchannels + c);
				sumdiff += ABS(val1-val2);
			}
		}
	}
	return sumdiff/(width*height);
}
bool isImageSame_shifty(IplImage * bigImage, int x0, int y0, string filename,  double thresh = 20, int shiftnum = 2)
{
	//filename = DDZTMPLPATH + filename;
	IplImage * smallImage = getImage(filename);// cvLoadImage(filename.c_str(), 1);
	if(!smallImage) return false;
	assert(bigImage);
	assert(bigImage->width >= smallImage->width && bigImage->height >= smallImage->height && bigImage->nChannels == smallImage->nChannels);
	int width = smallImage->width;
	int height = smallImage->height;
	int nchannels = bigImage->nChannels;
	assert(x0 >= 0 && y0 >= 0 && x0 + width <= bigImage->width && y0 + height <= bigImage->height);

	double sumdiff = 0;
	for(int s = 0; s < shiftnum; s++)
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
		if (sumdiff/(width*(height-s)) < thresh) return true;

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
			if (sumdiff/(width*(height-s)) < thresh) return true;
		}
	}
	return false;
}

bool isImageSame(IplImage * bigImage, int x0, int y0, string filename,  double thresh = 20)
{
	//filename = DDZTMPLPATH + filename;
	IplImage * smallImage = getImage(filename);// cvLoadImage(filename.c_str(), 1);
	if(!smallImage) return false;
	assert(bigImage);
	assert(bigImage->width >= smallImage->width && bigImage->height >= smallImage->height && bigImage->nChannels == smallImage->nChannels);
	int width = smallImage->width;
	int height = smallImage->height;
	int nchannels = bigImage->nChannels;
	assert(x0 >= 0 && y0 >= 0 && x0 + width <= bigImage->width && y0 + height <= bigImage->height);
	double sumdiff = 0;
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			int by = y0 + y;
			int bx = x0 + x;
			for(int c = 0; c < nchannels; c++)
			{
				double val1 = CV_IMAGE_ELEM(smallImage, unsigned char, y, x*nchannels + c);
				double val2 = CV_IMAGE_ELEM(bigImage, unsigned char, by, bx*nchannels + c);
				sumdiff += ABS(val1-val2);
			}
		}
	}
	return (sumdiff/(width*height) < thresh);
}

bool recog_mycards_bbox(IplImage * image, vector<CvRect> &numBBox, vector<CvRect> &typeBBox)
{
	int i0 = 138;
	int top = -1;
	for(int j = 0; j < image->height; j++)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+0);
		double dist = sqrt((R - 239)*(R-239) + (G-239)*(G-239) + (B-239)*(B-239));
		if(dist < 30) 
		{
			double sumR = 0.0, sumG = 0.0, sumB = 0.0;
			for(int i = 72; i < 230; i++)
			{
				sumR += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+2);
				sumG += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+1);
				sumB += CV_IMAGE_ELEM(image, unsigned char, j, 3*i);
			}
			double avgR = sumR/158.0;
			double avgG = sumG/158.0;
			double avgB = sumB/158.0;
			double dist2 = sqrt((avgR - 239)*(avgR-239) + (avgG-239)*(avgG-239) + (avgB-239)*(avgB-239));
			if(dist2 < 30)
			{
				top = j;
				break;
			}
		}
	}
	int bot = -1;
	for(int j = image->height - 1; j >= 0; j--)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+0);
		double dist = sqrt((R - 239)*(R-239) + (G-239)*(G-239) + (B-239)*(B-239));
		if(dist < 30) 
		{
			double sumR = 0.0, sumG = 0.0, sumB = 0.0;
			for(int i = 72; i < 230; i++)
			{
				sumR += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+2);
				sumG += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+1);
				sumB += CV_IMAGE_ELEM(image, unsigned char, j, 3*i);
			}
			double avgR = sumR/158.0;
			double avgG = sumG/158.0;
			double avgB = sumB/158.0;
			double dist2 = sqrt((avgR - 239)*(avgR-239) + (avgG-239)*(avgG-239) + (avgB-239)*(avgB-239));
			if(dist2 < 30)
			{
				bot = j;
				break;
			}
		}
	}
	int num = 0;
	for(int j = top; j <= bot-150; j++)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+0);
		double avg = (R+G+B)/3.0;
		if(avg < 190)
		{
			num++;
			j += 20;
		}
	}
	double step = 66.66;
	int ncards = num + 1;//(bot - top - 150)/step + 1;
	if(ncards > 1) step = (bot - 159 - top)/(double)(ncards-1);
	for(int n = 0; n < ncards; n++)
	{
		int shift = 0;
		if(!isPixelSame(image, 70, top+n*step+16, CV_RGB(214, 214, 214), 20))
			shift = 18;

		int x1 = 180 + shift;
		int y1 = top + n * step + 2;
		int width1 = 51;
		int height1 = 51;
		numBBox.push_back(cvRect(x1, y1, width1 , height1));
		int x2 = 149 + shift;
		int y2 = top + n * step + 10;
		int width2 = 30;
		int height2 = 36;
		typeBBox.push_back(cvRect(x2, y2, width2 , height2));
	}
	return true;
}

bool isblack(int type)
{
	return (type == TYPE_SPADE || type == TYPE_CLUB);
}
bool isred(int type)
{
	return (type == TYPE_HEART || type == TYPE_DIAMOND);
}

vector<Card> recog_mycards(IplImage * image)
{
	vector<CvRect> numBBox;
	vector<CvRect> typeBBox;
	recog_mycards_bbox(image, numBBox, typeBBox);
	vector<Card> cards(numBBox.size());
	for(int i = 0; i < typeBBox.size(); i++)
	{
		CvRect rect = typeBBox[i];
		if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_type_spade.png")) cards[i].type = TYPE_SPADE;
		else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_type_heart.png")) cards[i].type = TYPE_HEART;
		else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_type_club.png")) cards[i].type = TYPE_CLUB;
		else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_type_diamond.png")) cards[i].type = TYPE_DIAMOND;
		else
		{
			if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_red_joker.png"))
			{
				cards[i].type = TYPE_RED_JOKER;
				cards[i].num = NUM_JOKER;
			}
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_black_joker.png"))
			{
				cards[i].type = TYPE_BLACK_JOKER;
				cards[i].num = NUM_JOKER;
			}
			else
			{
				IplImage * unknownImage = cropImage(image, rect.x, rect.y, rect.width, rect.height);
				cvSaveImage((infile + ".ddz_mycards_type_unknown" + num2str(i) + ".png").c_str(), unknownImage);
				cvReleaseImage(&unknownImage);
			}
		}

		rect = numBBox[i];
		if(isblack(cards[i].type))
		{
			if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_b3.png")) cards[i].num = NUM_3;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_b4.png")) cards[i].num = NUM_4;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_b5.png")) cards[i].num = NUM_5;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_b6.png")) cards[i].num = NUM_6;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_b7.png")) cards[i].num = NUM_7;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_b8.png")) cards[i].num = NUM_8;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_b9.png")) cards[i].num = NUM_9;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_b10.png")) cards[i].num = NUM_10;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_bJ.png")) cards[i].num = NUM_J;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_bQ.png")) cards[i].num = NUM_Q;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_bK.png")) cards[i].num = NUM_K;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_bA.png")) cards[i].num = NUM_A;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_b2.png")) cards[i].num = NUM_2;
			else
			{
				IplImage * unknownImage = cropImage(image, rect.x, rect.y, rect.width, rect.height);
				cvSaveImage((infile + ".ddz_mycards_num_unknown" + num2str(i) + ".png").c_str(), unknownImage);
				cvReleaseImage(&unknownImage);
			}
		}
		else if(isred(cards[i].type))
		{
			if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_r3.png")) cards[i].num = NUM_3;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_r4.png")) cards[i].num = NUM_4;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_r5.png")) cards[i].num = NUM_5;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_r6.png")) cards[i].num = NUM_6;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_r7.png")) cards[i].num = NUM_7;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_r8.png")) cards[i].num = NUM_8;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_r9.png")) cards[i].num = NUM_9;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_r10.png")) cards[i].num = NUM_10;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_rJ.png")) cards[i].num = NUM_J;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_rQ.png")) cards[i].num = NUM_Q;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_rK.png")) cards[i].num = NUM_K;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_rA.png")) cards[i].num = NUM_A;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_mycards_num_r2.png")) cards[i].num = NUM_2;
			else
			{
				IplImage * unknownImage = cropImage(image, rect.x, rect.y, rect.width, rect.height);
				cvSaveImage((infile + ".ddz_mycards_num_unknown" + num2str(i) + ".png").c_str(), unknownImage);
				cvReleaseImage(&unknownImage);
			}
		}
	}
	return cards;
}

struct PlayPara
{
	int play_leftx;
	int play_rightx;
	int play_top_most;
	int play_bot_most;
	int play_midx;
	int play_step;
	int play_card_height;
	int play_num_gap;
	int play_num_leftx;
	int play_num_width;
	int play_num_height;
	int play_type_gap;
	int play_type_leftx;
	int play_type_width;
	int play_type_height;
};

PlayPara myplay_para = {258, 336, 270, 994, 283, 40, 68, 1, 305, 27, 32, 6, 285, 19, 22};
PlayPara leftplay_para = {391, 469, 285, 640, 437, 40, 68, 1, 438, 27, 32, 6, 418, 19, 22};
PlayPara rightplay_para = {258, 336, 270, 994, 283, 40, 68, 1, 305, 27, 32, 6, 285, 19, 22};

bool recog_play_bbox(IplImage * image, PlayPara para, vector<CvRect> &numBBox, vector<CvRect> &typeBBox)
{
	int top = -1;
	int bot = -1;
	int i0 = para.play_midx;
	for(int j = para.play_top_most; j < para.play_bot_most; j++)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+0);
		double dist = sqrt((R - 239)*(R-239) + (G-239)*(G-239) + (B-239)*(B-239));
		if(dist < 30) 
		{
			double sumR = 0.0, sumG = 0.0, sumB = 0.0;
			for(int i = para.play_leftx; i <= para.play_rightx; i++)
			{
				sumR += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+2);
				sumG += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+1);
				sumB += CV_IMAGE_ELEM(image, unsigned char, j, 3*i);
			}
			double avgR = sumR/(para.play_rightx - para.play_leftx + 1);
			double avgG = sumG/(para.play_rightx - para.play_leftx + 1);
			double avgB = sumB/(para.play_rightx - para.play_leftx + 1);
			double dist2 = sqrt((avgR - 239)*(avgR-239) + (avgG-239)*(avgG-239) + (avgB-239)*(avgB-239));
			if(dist2 < 30)
			{
				top = j;
				break;
			}
		}
	}
	for(int j = para.play_bot_most-1; j >= para.play_top_most; j--)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+0);
		double dist = sqrt((R - 239)*(R-239) + (G-239)*(G-239) + (B-239)*(B-239));
		if(dist < 30) 
		{
			double sumR = 0.0, sumG = 0.0, sumB = 0.0;
			for(int i = para.play_leftx; i <= para.play_rightx; i++)
			{
				sumR += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+2);
				sumG += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+1);
				sumB += CV_IMAGE_ELEM(image, unsigned char, j, 3*i);
			}
			double avgR = sumR/(para.play_rightx - para.play_leftx + 1);
			double avgG = sumG/(para.play_rightx - para.play_leftx + 1);
			double avgB = sumB/(para.play_rightx - para.play_leftx + 1);
			double dist2 = sqrt((avgR - 239)*(avgR-239) + (avgG-239)*(avgG-239) + (avgB-239)*(avgB-239));
			if(dist2 < 30)
			{
				bot = j;
				break;
			}
		}
	}

	int step = para.play_step;
	int ncards = (bot - top - para.play_card_height + 2)/step + 1;
	for(int n = 0; n < ncards; n++)
	{
		int x1 = para.play_num_leftx;//438;
		int y1 = top + n * step + para.play_num_gap;
		int width1 = para.play_num_width;//27;
		int height1 = para.play_num_height;//34;
		numBBox.push_back(cvRect(x1, y1, width1, height1));
		int x2 = para.play_type_leftx;//418;
		int y2 = top + n * step + para.play_type_gap;
		int width2 = para.play_type_width;//19;
		int height2 = para.play_type_height;//23;
		typeBBox.push_back(cvRect(x2, y2, width2, height2));
	}
	return true;
}

vector<Card> recog_play(IplImage * image, vector<CvRect> numBBox, vector<CvRect> typeBBox)
{
	IplImage * drawImage = cvCreateImage(cvGetSize(image), image->depth, image->nChannels);
	cvCopy(image, drawImage);
	vector<Card> cards(numBBox.size());
	for(int i = 0; i < typeBBox.size(); i++)
	{
		CvRect rect = typeBBox[i];
		cvRectangle(drawImage, cvPoint(rect.x, rect.y), cvPoint(rect.x+rect.width, rect.y + rect.height), CV_RGB(0, 255, 0), 1);
		if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_type_spade.png")) cards[i].type = TYPE_SPADE;
		else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_type_heart.png")) cards[i].type = TYPE_HEART;
		else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_type_club.png")) cards[i].type = TYPE_CLUB;
		else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_type_diamond.png")) cards[i].type = TYPE_DIAMOND;
		else
		{
			if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_red_joker.png"))
			{
				cards[i].type = TYPE_RED_JOKER;
				cards[i].num = NUM_JOKER;
			}
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_black_joker.png"))
			{
				cards[i].type = TYPE_BLACK_JOKER;
				cards[i].num = NUM_JOKER;
			}
			else
			{
				IplImage * unknownImage = cropImage(image, rect.x, rect.y, rect.width, rect.height);
				cvSaveImage((infile + ".ddz_play_type_unknown" + num2str(i) + ".png").c_str(), unknownImage);
				cvReleaseImage(&unknownImage);
			}
		}

		rect = numBBox[i];
		cvRectangle(drawImage, cvPoint(rect.x, rect.y), cvPoint(rect.x+rect.width, rect.y + rect.height), CV_RGB(0, 0, 255), 1);
		if(isblack(cards[i].type))
		{
			if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_b3.png")) cards[i].num = NUM_3;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_b4.png")) cards[i].num = NUM_4;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_b5.png")) cards[i].num = NUM_5;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_b6.png")) cards[i].num = NUM_6;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_b7.png")) cards[i].num = NUM_7;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_b8.png")) cards[i].num = NUM_8;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_b9.png")) cards[i].num = NUM_9;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_b10.png")) cards[i].num = NUM_10;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_bJ.png")) cards[i].num = NUM_J;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_bQ.png")) cards[i].num = NUM_Q;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_bK.png")) cards[i].num = NUM_K;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_bA.png")) cards[i].num = NUM_A;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_b2.png")) cards[i].num = NUM_2;
			else
			{
				IplImage * unknownImage = cropImage(image, rect.x, rect.y, rect.width, rect.height);
				cvSaveImage((infile + ".ddz_play_num_unknown" + num2str(i) + ".png").c_str(), unknownImage);
				cvReleaseImage(&unknownImage);
			}
		}
		else if(isred(cards[i].type))
		{
			if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_r3.png")) cards[i].num = NUM_3;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_r4.png")) cards[i].num = NUM_4;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_r5.png")) cards[i].num = NUM_5;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_r6.png")) cards[i].num = NUM_6;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_r7.png")) cards[i].num = NUM_7;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_r8.png")) cards[i].num = NUM_8;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_r9.png")) cards[i].num = NUM_9;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_r10.png")) cards[i].num = NUM_10;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_rJ.png")) cards[i].num = NUM_J;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_rQ.png")) cards[i].num = NUM_Q;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_rK.png")) cards[i].num = NUM_K;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_rA.png")) cards[i].num = NUM_A;
			else if(isImageSame_shifty(image, rect.x, rect.y, "ddz_play_num_r2.png")) cards[i].num = NUM_2;
			else
			{
				IplImage * unknownImage = cropImage(image, rect.x, rect.y, rect.width, rect.height);
				cvSaveImage((infile + ".ddz_play_num_unknown" + num2str(i) + ".png").c_str(), unknownImage);
				cvReleaseImage(&unknownImage);
			}
		}
	}
	cvSaveImage("out.png", drawImage);
	cvReleaseImage(&drawImage);
	return cards;
}

vector<Card> recog_myplay(IplImage * image)
{
	vector<CvRect> numBBox, typeBBox;
	recog_play_bbox(image, myplay_para, numBBox, typeBBox);
	return recog_play(image, numBBox, typeBBox);
}
vector<Card> recog_leftplay(IplImage * image)
{
	vector<CvRect> numBBox, typeBBox;
	recog_play_bbox(image, leftplay_para, numBBox, typeBBox);
	return recog_play(image, numBBox, typeBBox);
}
vector<Card> recog_rightplay(IplImage * image)
{
	vector<CvRect> numBBox, typeBBox;
	recog_play_bbox(image, rightplay_para, numBBox, typeBBox);
	return recog_play(image, numBBox, typeBBox);
}
class Play 
{
	public:
		set<int> myself;
		set<int> leftPlayer;
		set<int> rightPlayer;
		set<int> table;
		int king;
		Play()
		{
			init();
		}
		void init()
		{
			myself.clear();
			leftPlayer.clear();
			rightPlayer.clear();
			table.clear();
			king = PLAYER_UNKNOWN;
		}
};

Play play;

void saveImage(IplImage * image, int x0, int y0, int width, int height, string filename)
{
	IplImage * cropImg = cropImage(image, x0, y0, width, height);
	cvSaveImage(filename.c_str(), cropImg);
	cvReleaseImage(&cropImg);
}

int recog_card_num_left(IplImage * image)
{
	if(isImageSame(image, 317, 148, "ddz_left_left_num_1.png")) 
	{
		if(isImageSame(image, 317, 180, "ddz_left_right_num_0.png")) return 10;
		else if(isImageSame(image, 317, 180, "ddz_left_right_num_1.png")) return 11;
		else if(isImageSame(image, 317, 180, "ddz_left_right_num_2.png")) return 12;
		else if(isImageSame(image, 317, 180, "ddz_left_right_num_3.png")) return 13;
		else if(isImageSame(image, 317, 180, "ddz_left_right_num_4.png")) return 14;
		else if(isImageSame(image, 317, 180, "ddz_left_right_num_5.png")) return 15;
		else if(isImageSame(image, 317, 180, "ddz_left_right_num_6.png")) return 16;
		else if(isImageSame(image, 317, 180, "ddz_left_right_num_7.png")) return 17;
		else if(isImageSame(image, 317, 180, "ddz_left_right_num_8.png")) return 18;
		else if(isImageSame(image, 317, 180, "ddz_left_right_num_9.png")) return 19;
		else saveImage(image, 317, 180, 43, 33, infile + ".ddz_left_right_num_unknown.png");
	}
	else if(isImageSame(image, 317, 148, "ddz_left_left_num_2.png")) return 20;
	else
	{
		saveImage(image, 317, 148, 43, 33, infile + ".ddz_left_left_num_unknown.png");
	}
	return 0;
}
void process(IplImage * image)
{
	if(isImageSame(image, 221, 745, "ddz_start_game.png")) 
	{
		cout<<"开始游戏"<<endl;
		play.init();
		return;
	}
	if(isImageSame(image, 281, 547, "ddz_my_is_ming.png")) 
	{
		cout<<"我是否明牌"<<endl; // unable to recognize the cards
		play.init();
		return;
	}
	if(isImageSame(image, 312, 590, "ddz_my_turn.png")) 
	{
		cout<<"我出牌"<<endl;
		return;
	}
	if(isImageSame(image, 284, 365, "ddz_my_is_jiao.png")) cout<<"我是否叫地主"<<endl;
	if(isImageSame(image, 278, 548, "ddz_my_yi_jiao.png")) cout<<"我已叫地主"<<endl;
	if(isImageSame(image, 409, 934, "ddz_right_turn.png")) cout<<"右边出牌"<<endl;

	if(isImageSame(image, 408, 244, "ddz_left_turn.png")) cout<<"左边边出牌"<<endl;
	int nleft = recog_card_num_left(image); cout<<"左边牌数 "<<nleft<<endl;
}

int main(int argc, char ** argv)
{
	if(argc == 1)
	{
		cout<<"Usage: "<<argv[0]<<" <screenimage>"<<endl;
		return 0;
	}
	infile = argv[1];
	loadAllTemplates();

	IplImage * image = cvLoadImage(argv[1], 1);

	cout<<"mycards: ";
	vector<Card> mycards = recog_mycards(image);
	for(int i = 0; i < mycards.size(); i++) cout<<mycards[i].str();
	cout<<endl;
	
	cout<<"myplay: ";
	vector<Card> myplay = recog_myplay(image);
	for(int i = 0; i < myplay.size(); i++) cout<<myplay[i].str();
	cout<<endl;

	cout<<"leftplay: ";
	vector<Card> leftplay = recog_leftplay(image);
	for(int i = 0; i < leftplay.size(); i++) cout<<leftplay[i].str();
	cout<<endl;
/*
	vector<Card> rightplay = recog_rightplay(image);
	for(int i = 0; i < rightplay.size(); i++) cout<<rightplay[i].str();
	cout<<endl;
	*/
	return 0;
}
