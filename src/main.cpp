#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "recog_last.h"
#include "recog_play.h"
#include "recog_inhand.h"
#include "image_patches.h"
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
	//loadAllTemplates(); // 最好在getImage中装载

	IplImage * image = cvLoadImage(argv[1], 1);
	IplImage * drawImage = cvLoadImage(argv[1], 1);

	build_patch_map();
	string stage = which_game_stage(image);
	cout<<stage<<endl;
	return 0;

	vector<Card> cards;
	if(!isImageSame(image, 639, 36, "ddz_patch_last3_bkg.png", 10))
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
		if(isImageSame(image, 408, 244, "ddz_patch_lturn.png", 10)) cout<<"It is left turn"<<endl;
		else if(isImageSame(image, 373, 355, "ddz_patch_left_no.png", 10)) cout<<"Left doesn't play"<<endl;
		else if(isImageSame(image, 360, 256, "ddz_patch_effect_shunzi_left.png", 10)) cout<<"Left played shunzi"<<endl;
		else
		{
			cout<<"Left played ";
			RecogPlay lplay("play", "left");
			cards = lplay.recog_cards(image);
			lplay.drawResult(drawImage);
			for(int i = 0; i < cards.size(); i++) cout<<cards[i].str();
			cout<<endl;
		}

		if(isImageSame(image, 316, 485, "ddz_patch_myturn.png", 10)) cout<<"It is my turn"<<endl;
		else if(isImageSame(image, 274, 578, "ddz_patch_me_no.png", 10))
		{
			if(isImageSame(image, 277, 648, "ddz_patch_me_jiao.png", 10))
				cout<<"I don't want master"<<endl;
			else
				cout<<"I don't play"<<endl;
		}
		else if(isImageSame(image, 270, 563, "ddz_patch_effect_liandui_me.png", 10)) cout<<"I played lian dui"<<endl;
		else
		{
			cout<<"I played ";
			RecogPlay mplay("play", "myself");
			cards = mplay.recog_cards(image);
			mplay.drawResult(drawImage);
			for(int i = 0; i < cards.size(); i++) cout<<cards[i].str();
			cout<<endl;
		}

		if(isImageSame(image, 409, 934, "ddz_patch_rturn.png", 10)) cout<<"It is right turn"<<endl;
		else if(isImageSame(image, 377, 806, "ddz_patch_right_no.png", 10)) cout<<"Right doesn't play"<<endl;
		else
		{
			cout<<"Right played : ";
			RecogPlay rplay("play", "right");
			cards = rplay.recog_cards(image);
			rplay.drawResult(drawImage);
			for(int i = 0; i < cards.size(); i++) cout<<cards[i].str();
			cout<<endl;
		}
		cout<<endl;
	}


	cout<<"======  Inhand  ======"<<endl;
	RecogInhand inhand("inhand");
	cards = inhand.recog_cards(image);
	inhand.drawResult(drawImage, -100);
	for(int i = 0; i < cards.size(); i++) cout<<cards[i].str();
	cout<<endl<<endl;;

	cvSaveImage((infile + ".out.png").c_str(), drawImage);
	releaseAllTemplates();
	cvReleaseImage(&image);
	cvReleaseImage(&drawImage);
	return 0;
}
