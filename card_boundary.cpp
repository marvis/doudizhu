#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cmath>
#include <string>

using namespace std;
using namespace cv;

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

// myself
void player0(IplImage * image, IplImage * drawImage)
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
	cvRectangle(drawImage, cvPoint(53, top), cvPoint(231, bot), CV_RGB(255, 0, 0), 1);
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
	cout<<"step = "<<step<<endl;
	cout<<"ncards = "<<ncards<<endl;
	for(int n = 0; n < ncards; n++)
	{
		int shift = 0;
		if(!isPixelSame(image, 70, top+n*step+16, CV_RGB(214, 214, 214), 20))
			shift = 18;

		int x1 = 180 + shift;
		int y1 = top + n * step + 2;
		int width1 = 51;
		int height1 = 51;
		cvRectangle(drawImage, cvPoint(x1, y1), cvPoint(x1 + width1, y1 + height1), CV_RGB(0, 255, 0), 1);
		int x2 = 149 + shift;
		int y2 = top + n * step + 10;
		int width2 = 30;
		int height2 = 36;
		cvRectangle(drawImage, cvPoint(x2, y2), cvPoint(x2 + width2, y2 + height2), CV_RGB(0, 0, 255), 1);
	}
}

#define PLAYER1_LEFT 258
#define PLAYER1_RIGHT 336
#define PLAYER1_TOP 270
#define PLAYER1_BOT 994
#define PLAYER1_MID_X 283
#define PLAYER1_STEP 40
#define PLAYER1_CARD_HEI 68
#define PLAYER1_NUM_GAP 1
#define PLAYER1_NUM_LEFT 305
#define PLAYER1_NUM_WID 27
#define PLAYER1_NUM_HEI 32
#define PLAYER1_TYPE_GAP 6
#define PLAYER1_TYPE_LEFT 285
#define PLAYER1_TYPE_WID 19
#define PLAYER1_TYPE_HEI 22

void player1(IplImage * image, IplImage * drawImage)
{
	int top = -1;
	int bot = -1;
	int i0 = PLAYER1_MID_X;
	for(int j = PLAYER1_TOP; j < PLAYER1_BOT; j++)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+0);
		double dist = sqrt((R - 239)*(R-239) + (G-239)*(G-239) + (B-239)*(B-239));
		if(dist < 30) 
		{
			double sumR = 0.0, sumG = 0.0, sumB = 0.0;
			for(int i = PLAYER1_LEFT; i <= PLAYER1_RIGHT; i++)
			{
				sumR += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+2);
				sumG += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+1);
				sumB += CV_IMAGE_ELEM(image, unsigned char, j, 3*i);
			}
			double avgR = sumR/(PLAYER1_RIGHT - PLAYER1_LEFT + 1);
			double avgG = sumG/(PLAYER1_RIGHT - PLAYER1_LEFT + 1);
			double avgB = sumB/(PLAYER1_RIGHT - PLAYER1_LEFT + 1);
			double dist2 = sqrt((avgR - 239)*(avgR-239) + (avgG-239)*(avgG-239) + (avgB-239)*(avgB-239));
			if(dist2 < 30)
			{
				top = j;
				break;
			}
		}
	}
	for(int j = PLAYER1_BOT-1; j >= PLAYER1_TOP; j--)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+0);
		double dist = sqrt((R - 239)*(R-239) + (G-239)*(G-239) + (B-239)*(B-239));
		if(dist < 30) 
		{
			double sumR = 0.0, sumG = 0.0, sumB = 0.0;
			for(int i = PLAYER1_LEFT; i <= PLAYER1_RIGHT; i++)
			{
				sumR += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+2);
				sumG += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+1);
				sumB += CV_IMAGE_ELEM(image, unsigned char, j, 3*i);
			}
			double avgR = sumR/(PLAYER1_RIGHT - PLAYER1_LEFT + 1);
			double avgG = sumG/(PLAYER1_RIGHT - PLAYER1_LEFT + 1);
			double avgB = sumB/(PLAYER1_RIGHT - PLAYER1_LEFT + 1);
			double dist2 = sqrt((avgR - 239)*(avgR-239) + (avgG-239)*(avgG-239) + (avgB-239)*(avgB-239));
			if(dist2 < 30)
			{
				bot = j;
				break;
			}
		}
	}
	cvRectangle(drawImage, cvPoint(PLAYER1_LEFT, top), cvPoint(PLAYER1_RIGHT+1, bot), CV_RGB(255, 0, 0), 1);

	int step = PLAYER1_STEP;
	int ncards = (bot - top - PLAYER1_CARD_HEI + 2)/step + 1;
	for(int n = 0; n < ncards; n++)
	{
		int x1 = PLAYER1_NUM_LEFT;//438;
		int y1 = top + n * step + PLAYER1_NUM_GAP;
		int width1 = PLAYER1_NUM_WID;//27;
		int height1 = PLAYER1_NUM_HEI;//34;
		cvRectangle(drawImage, cvPoint(x1, y1), cvPoint(x1 + width1, y1 + height1), CV_RGB(0, 255, 0), 1);
		int x2 = PLAYER1_TYPE_LEFT;//418;
		int y2 = top + n * step + PLAYER1_TYPE_GAP;
		int width2 = PLAYER1_TYPE_WID;//19;
		int height2 = PLAYER1_TYPE_HEI;//23;
		cvRectangle(drawImage, cvPoint(x2, y2), cvPoint(x2 + width2, y2 + height2), CV_RGB(0, 0, 255), 1);
	}
}


#define PLAYER2_LEFT 391
#define PLAYER2_RIGHT 469
#define PLAYER2_TOP 285
#define PLAYER2_BOT 640
#define PLAYER2_MID_X 437
#define PLAYER2_STEP 40
#define PLAYER2_CARD_HEI 68
#define PLAYER2_NUM_GAP 1
#define PLAYER2_NUM_LEFT 438
#define PLAYER2_NUM_WID 27
#define PLAYER2_NUM_HEI 32
#define PLAYER2_TYPE_GAP 6
#define PLAYER2_TYPE_LEFT 418
#define PLAYER2_TYPE_WID 19
#define PLAYER2_TYPE_HEI 22

void player2(IplImage * image, IplImage * drawImage)
{
	int top = -1;
	int bot = -1;
	int i0 = PLAYER2_MID_X;
	for(int j = PLAYER2_TOP; j < PLAYER2_BOT; j++)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+0);
		double dist = sqrt((R - 239)*(R-239) + (G-239)*(G-239) + (B-239)*(B-239));
		if(dist < 30) 
		{
			double sumR = 0.0, sumG = 0.0, sumB = 0.0;
			for(int i = PLAYER2_LEFT; i <= PLAYER2_RIGHT; i++)
			{
				sumR += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+2);
				sumG += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+1);
				sumB += CV_IMAGE_ELEM(image, unsigned char, j, 3*i);
			}
			double avgR = sumR/(PLAYER2_RIGHT - PLAYER2_LEFT + 1);
			double avgG = sumG/(PLAYER2_RIGHT - PLAYER2_LEFT + 1);
			double avgB = sumB/(PLAYER2_RIGHT - PLAYER2_LEFT + 1);
			double dist2 = sqrt((avgR - 239)*(avgR-239) + (avgG-239)*(avgG-239) + (avgB-239)*(avgB-239));
			if(dist2 < 30)
			{
				top = j;
				break;
			}
		}
	}
	for(int j = PLAYER2_BOT-1; j >= PLAYER2_TOP; j--)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+0);
		double dist = sqrt((R - 239)*(R-239) + (G-239)*(G-239) + (B-239)*(B-239));
		if(dist < 30) 
		{
			double sumR = 0.0, sumG = 0.0, sumB = 0.0;
			for(int i = PLAYER2_LEFT; i <= PLAYER2_RIGHT; i++)
			{
				sumR += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+2);
				sumG += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+1);
				sumB += CV_IMAGE_ELEM(image, unsigned char, j, 3*i);
			}
			double avgR = sumR/(PLAYER2_RIGHT - PLAYER2_LEFT + 1);
			double avgG = sumG/(PLAYER2_RIGHT - PLAYER2_LEFT + 1);
			double avgB = sumB/(PLAYER2_RIGHT - PLAYER2_LEFT + 1);
			double dist2 = sqrt((avgR - 239)*(avgR-239) + (avgG-239)*(avgG-239) + (avgB-239)*(avgB-239));
			if(dist2 < 30)
			{
				bot = j;
				break;
			}
		}
	}
	cvRectangle(drawImage, cvPoint(PLAYER2_LEFT, top), cvPoint(PLAYER2_RIGHT+1, bot), CV_RGB(255, 0, 0), 1);

	int step = PLAYER2_STEP;
	int ncards = (bot - top - PLAYER2_CARD_HEI + 2)/step + 1;
	for(int n = 0; n < ncards; n++)
	{
		int x1 = PLAYER2_NUM_LEFT;//438;
		int y1 = top + n * step + PLAYER2_NUM_GAP;
		int width1 = PLAYER2_NUM_WID;//27;
		int height1 = PLAYER2_NUM_HEI;//34;
		cvRectangle(drawImage, cvPoint(x1, y1), cvPoint(x1 + width1, y1 + height1), CV_RGB(0, 255, 0), 1);
		int x2 = PLAYER2_TYPE_LEFT;//418;
		int y2 = top + n * step + PLAYER2_TYPE_GAP;
		int width2 = PLAYER2_TYPE_WID;//19;
		int height2 = PLAYER2_TYPE_HEI;//23;
		cvRectangle(drawImage, cvPoint(x2, y2), cvPoint(x2 + width2, y2 + height2), CV_RGB(0, 0, 255), 1);
	}
}
#define PLAYER3_LEFT 391
#define PLAYER3_RIGHT 469
#define PLAYER3_TOP 640
#define PLAYER3_BOT 1000
#define PLAYER3_MID_X 437
#define PLAYER3_STEP 40
#define PLAYER3_CARD_HEI 68
#define PLAYER3_NUM_GAP 1
#define PLAYER3_NUM_LEFT 438
#define PLAYER3_NUM_WID 27
#define PLAYER3_NUM_HEI 32
#define PLAYER3_TYPE_GAP 6
#define PLAYER3_TYPE_LEFT 418
#define PLAYER3_TYPE_WID 19
#define PLAYER3_TYPE_HEI 22

void player3(IplImage * image, IplImage * drawImage)
{
	int top = -1;
	int bot = -1;
	int i0 = PLAYER3_MID_X;
	for(int j = PLAYER3_TOP; j < PLAYER3_BOT; j++)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+0);
		double dist = sqrt((R - 239)*(R-239) + (G-239)*(G-239) + (B-239)*(B-239));
		if(dist < 30) 
		{
			double sumR = 0.0, sumG = 0.0, sumB = 0.0;
			for(int i = PLAYER3_LEFT; i <= PLAYER3_RIGHT; i++)
			{
				sumR += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+2);
				sumG += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+1);
				sumB += CV_IMAGE_ELEM(image, unsigned char, j, 3*i);
			}
			double avgR = sumR/(PLAYER3_RIGHT - PLAYER3_LEFT + 1);
			double avgG = sumG/(PLAYER3_RIGHT - PLAYER3_LEFT + 1);
			double avgB = sumB/(PLAYER3_RIGHT - PLAYER3_LEFT + 1);
			double dist2 = sqrt((avgR - 239)*(avgR-239) + (avgG-239)*(avgG-239) + (avgB-239)*(avgB-239));
			if(dist2 < 30)
			{
				top = j;
				break;
			}
		}
	}
	for(int j = PLAYER3_BOT-1; j >= PLAYER3_TOP; j--)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i0+0);
		double dist = sqrt((R - 239)*(R-239) + (G-239)*(G-239) + (B-239)*(B-239));
		if(dist < 30) 
		{
			double sumR = 0.0, sumG = 0.0, sumB = 0.0;
			for(int i = PLAYER3_LEFT; i <= PLAYER3_RIGHT; i++)
			{
				sumR += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+2);
				sumG += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+1);
				sumB += CV_IMAGE_ELEM(image, unsigned char, j, 3*i);
			}
			double avgR = sumR/(PLAYER3_RIGHT - PLAYER3_LEFT + 1);
			double avgG = sumG/(PLAYER3_RIGHT - PLAYER3_LEFT + 1);
			double avgB = sumB/(PLAYER3_RIGHT - PLAYER3_LEFT + 1);
			double dist2 = sqrt((avgR - 239)*(avgR-239) + (avgG-239)*(avgG-239) + (avgB-239)*(avgB-239));
			if(dist2 < 30)
			{
				bot = j;
				break;
			}
		}
	}
	cvRectangle(drawImage, cvPoint(PLAYER3_LEFT, top), cvPoint(PLAYER3_RIGHT+1, bot), CV_RGB(255, 0, 0), 1);

	int step = PLAYER3_STEP;
	int ncards = (bot - top - PLAYER3_CARD_HEI + 2)/step + 1;
	for(int n = 0; n < ncards; n++)
	{
		int x1 = PLAYER3_NUM_LEFT;//438;
		int y1 = top + n * step + PLAYER3_NUM_GAP;
		int width1 = PLAYER3_NUM_WID;//27;
		int height1 = PLAYER3_NUM_HEI;//34;
		cvRectangle(drawImage, cvPoint(x1, y1), cvPoint(x1 + width1, y1 + height1), CV_RGB(0, 255, 0), 1);
		int x2 = PLAYER3_TYPE_LEFT;//418;
		int y2 = top + n * step + PLAYER3_TYPE_GAP;
		int width2 = PLAYER3_TYPE_WID;//19;
		int height2 = PLAYER3_TYPE_HEI;//23;
		cvRectangle(drawImage, cvPoint(x2, y2), cvPoint(x2 + width2, y2 + height2), CV_RGB(0, 0, 255), 1);
	}
}

int main(int argc, char ** argv)
{
	if(argc == 1)
	{
		cout<<"Usage: "<<argv[0]<<" <screenimg>"<<endl;
		return 0;
	}
	string filename = argv[1];
	IplImage * image = cvLoadImage(argv[1], 1);
	IplImage * drawImage = cvLoadImage(argv[1], 1);
	
	player0(image, drawImage);
	player1(image, drawImage);
	player2(image, drawImage);
	player3(image, drawImage);

	cvSaveImage((filename + ".out.png").c_str(), drawImage);
	cvReleaseImage(&image);
	cvReleaseImage(&drawImage);
	return 0;
}
