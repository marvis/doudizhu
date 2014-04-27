#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "recog_last.h"
#include "recog_play.h"
#include "common.h"

using namespace std;
using namespace cv;

string infile;
int main(int argc, char ** argv)
{
	if(argc == 1) 
	{
		cout<<"Usage: "<<argv[0]<<" <screenimg>"<<endl;
		return 0;
	}
	infile = argv[1];
	loadAllTemplates();

	IplImage * image = cvLoadImage(argv[1], 1);
	IplImage * drawImage = cvLoadImage(argv[1], 1);
	vector<Card> cards;

	if(isImageSame(image, 221, 745, "ddz_check_start_game.png", 10))
	{
		cout<<"开始游戏"<<endl;
		return 0;
	}
	if(isImageSame(image, 281, 547, "ddz_check_ming_pai.png", 10)) 
	{
		cout<<"是否明牌"<<endl;
		return 0;
	}


	if(!isImageSame(image, 639, 36, "ddz_check_last_bkg.png", 10))
	{
		cout<<"======  last  ======"<<endl;
		RecogLast last("last");
		cards = last.recog_cards(image);
		last.drawResult(drawImage);

		for(int i = 0; i < cards.size(); i++) cout<<cards[i].str();
		cout<<endl<<endl;
		//cout<<"======  last end    ======"<<endl;
	}
	if(1)
	{
		cout<<"======  play  ======"<<endl;
		if(!isImageSame(image, 408, 244, "ddz_check_lturn.png", 10))
		{
			cout<<" lplay: ";
			RecogPlay lplay("play", "left");
			cards = lplay.recog_cards(image);
			lplay.drawResult(drawImage);
			for(int i = 0; i < cards.size(); i++) cout<<cards[i].str();
			cout<<endl;
		}
		else cout<<"left turn"<<endl;

		if(!isImageSame(image, 316, 485, "ddz_check_myturn.png", 20))
		{
			cout<<"myplay: ";
			RecogPlay mplay("play", "myself");
			cards = mplay.recog_cards(image);
			mplay.drawResult(drawImage);
			for(int i = 0; i < cards.size(); i++) cout<<cards[i].str();
			cout<<endl;
		}
		else cout<<"my turn"<<endl;

		if(!isImageSame(image, 409, 934, "ddz_check_rturn.png", 10))
		{
			cout<<" rplay: ";
			RecogPlay rplay("play", "right");
			cards = rplay.recog_cards(image);
			rplay.drawResult(drawImage);
			for(int i = 0; i < cards.size(); i++) cout<<cards[i].str();
			cout<<endl;
		}
		else cout<<"right turn"<<endl;
		cout<<endl;
	}

	cvSaveImage((infile + ".out.png").c_str(), drawImage);
	releaseAllTemplates();
	cvReleaseImage(&image);
	cvReleaseImage(&drawImage);
	return 0;
}
