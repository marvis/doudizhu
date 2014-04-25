#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cmath>
#include <string>

using namespace std;
using namespace cv;

string filename;
int main(int argc, char ** argv)
{
	if(argc == 1)
	{
		cout<<"Usage: "<<argv[0]<<" <screenimg>"<<endl;
		return 0;
	}
	filename = argv[1];
	IplImage * image = cvLoadImage(argv[1], 1);
	IplImage * drawImage = cvLoadImage(argv[1], 1);
	int i1 = 138;
	int top1 = -1;
	for(int j = 0; j < image->height; j++)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i1+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i1+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i1+0);
		double dist = sqrt((R - 239)*(R-239) + (G-239)*(G-239) + (B-239)*(B-239));
		if(dist < 30) 
		{
			double sumR = 0.0, sumG = 0.0, sumB = 0.0;
			for(int i = 54; i < 230; i++)
			{
				sumR += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+2);
				sumG += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+1);
				sumB += CV_IMAGE_ELEM(image, unsigned char, j, 3*i);
			}
			double avgR = sumR/176.0;
			double avgG = sumG/176.0;
			double avgB = sumB/176.0;
			double dist2 = sqrt((avgR - 239)*(avgR-239) + (avgG-239)*(avgG-239) + (avgB-239)*(avgB-239));
			if(dist2 < 30)
			{
				top1 = j;
				break;
			}
		}
	}
	int bot1 = -1;
	for(int j = image->height - 1; j >= 0; j--)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i1+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i1+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i1+0);
		double dist = sqrt((R - 239)*(R-239) + (G-239)*(G-239) + (B-239)*(B-239));
		if(dist < 30) 
		{
			double sumR = 0.0, sumG = 0.0, sumB = 0.0;
			for(int i = 54; i < 230; i++)
			{
				sumR += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+2);
				sumG += CV_IMAGE_ELEM(image, unsigned char, j, 3*i+1);
				sumB += CV_IMAGE_ELEM(image, unsigned char, j, 3*i);
			}
			double avgR = sumR/176.0;
			double avgG = sumG/176.0;
			double avgB = sumB/176.0;
			double dist2 = sqrt((avgR - 239)*(avgR-239) + (avgG-239)*(avgG-239) + (avgB-239)*(avgB-239));
			if(dist2 < 30)
			{
				bot1 = j;
				break;
			}
		}
	}
	cvRectangle(drawImage, cvPoint(53, top1), cvPoint(231, bot1), CV_RGB(255, 0, 0), 1);
	int num = 0;
	for(int j = top1; j <= bot1-150; j++)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i1+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i1+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i1+0);
		double avg = (R+G+B)/3.0;
		if(avg < 190)
		{
			num++;
			j += 20;
		}
		//cout<<j<<": ("<<R<<","<<G<<","<<B<<")"<<endl;
	}
	double step1 = 66.66;
	int ncards1 = num + 1;//(bot1 - top1 - 150)/step1 + 1;
	step1 = (bot1 - 159 - top1)/(double)(ncards1-1);
	cout<<"step1 = "<<step1<<endl;
	cout<<"ncards1 = "<<ncards1<<endl;
	for(int n = 0; n < ncards1; n++)
	{
		int x1 = 180;
		int y1 = top1 + n * step1 + 2;
		int width1 = 51;
		int height1 = 51;
		cvRectangle(drawImage, cvPoint(x1, y1), cvPoint(x1 + width1, y1 + height1), CV_RGB(0, 255, 0), 1);
		int x2 = 149;
		int y2 = top1 + n * step1 + 10;
		int width2 = 30;
		int height2 = 36;
		cvRectangle(drawImage, cvPoint(x2, y2), cvPoint(x2 + width2, y2 + height2), CV_RGB(0, 0, 255), 1);
	}

	// player2's cards
	int top2 = -1;
	int bot2 = -1;
	int i2 = 437;
	for(int j = 285; j < 673; j++)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i2+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i2+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i2+0);
		double dist = sqrt((R - 239)*(R-239) + (G-239)*(G-239) + (B-239)*(B-239));
		if(dist < 30) 
		{
			top2 = j;
			break;
		}
	}
	for(int j = 672; j >= 285; j--)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i2+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i2+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i2+0);
		double dist = sqrt((R - 239)*(R-239) + (G-239)*(G-239) + (B-239)*(B-239));
		if(dist < 30) 
		{
			bot2 = j;
			break;
		}
	}
	cvRectangle(drawImage, cvPoint(389, top2), cvPoint(471, bot2), CV_RGB(255, 0, 0), 1);

	int step2 = 40;
	int ncards2 = (bot2 - top2 - 68)/step2 + 1;
	for(int n = 0; n < ncards2; n++)
	{
		int x1 = 438;
		int y1 = top2 + n * step2 + 2;
		int width1 = 27;
		int height1 = 34;
		cvRectangle(drawImage, cvPoint(x1, y1), cvPoint(x1 + width1, y1 + height1), CV_RGB(0, 255, 0), 1);
		int x2 = 418;
		int y2 = top2 + n * step2 + 7;
		int width2 = 19;
		int height2 = 23;
		cvRectangle(drawImage, cvPoint(x2, y2), cvPoint(x2 + width2, y2 + height2), CV_RGB(0, 0, 255), 1);
	}

	// player3's cards
	int top3 = -1;
	int bot3 = -1;
	int i3 = 437;
	for(int j = 629; j < 1080; j++)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i3+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i3+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i3+0);
		double dist = sqrt((R - 239)*(R-239) + (G-239)*(G-239) + (B-239)*(B-239));
		if(dist < 30) 
		{
			top3 = j;
			break;
		}
	}
	for(int j = 1079; j >= 629; j--)
	{
		int R = CV_IMAGE_ELEM(image, unsigned char, j, 3*i3+2);
		int G = CV_IMAGE_ELEM(image, unsigned char, j, 3*i3+1);
		int B = CV_IMAGE_ELEM(image, unsigned char, j, 3*i3+0);
		double dist = sqrt((R - 239)*(R-239) + (G-239)*(G-239) + (B-239)*(B-239));
		if(dist < 30) 
		{
			bot3 = j;
			break;
		}
	}
	cvRectangle(drawImage, cvPoint(389, top3), cvPoint(471, bot3), CV_RGB(255, 0, 0), 1);

	int step3 = 40;
	int ncards3 = (bot3 - top3 - 68)/step3 + 1;
	for(int n = 0; n < ncards2; n++)
	{
		int x1 = 438;
		int y1 = top3 + n * step3 + 2;
		int width1 = 27;
		int height1 = 34;
		cvRectangle(drawImage, cvPoint(x1, y1), cvPoint(x1 + width1, y1 + height1), CV_RGB(0, 255, 0), 1);
		int x2 = 418;
		int y2 = top3 + n * step3 + 7;
		int width2 = 19;
		int height2 = 23;
		cvRectangle(drawImage, cvPoint(x2, y2), cvPoint(x2 + width2, y2 + height2), CV_RGB(0, 0, 255), 1);
	}
	cvSaveImage((filename + ".out.png").c_str(), drawImage);
	cvReleaseImage(&image);
	cvReleaseImage(&drawImage);
	return 0;
}
