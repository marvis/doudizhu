#pragma once
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <string>

using namespace std;
using namespace cv;
#define ABS(x) ((x) > 0 ? (x) : (-(x)))

extern map<string, IplImage *> map_allImgs;

IplImage * getImage(string);
IplImage * cropImage(IplImage * src, int x, int y, int width, int height);
string num2str(int num);
void drawOnImage(IplImage * image, int x0, int y0, string file);
bool isImageSame(IplImage * bigImage, int x0, int y0, string filename, double & diffval, double thresh = 30, int shiftnum = 2);
void loadAllTemplates();
