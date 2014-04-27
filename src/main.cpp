#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "recog_last.h"
#include "recog_play.h"
#include "recog_inhand.h"
#include "recog_display.h"
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
	//return 0;

	vector<Card> cards;
	if(!isImagePatchSame(image, "ddz_patch_last3_bkg.png"))
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
		if(isImagePatchSame(image, "ddz_patch_left_clock.png")) cout<<"It is left turn"<<endl;
		else if(isImagePatchSame(image, "ddz_patch_left_has_not_play.png")) cout<<"Left doesn't play"<<endl;
		else if(isImagePatchSame(image, "ddz_patch_left_liandui.png")) cout<<"Left played liandui"<<endl;
		else if(isImagePatchSame(image, "ddz_patch_left_shunzi.png")) cout<<"Left played shunzi"<<endl;
		else
		{
			cout<<"Left played ";
			RecogPlay lplay("play", "left");
			cards = lplay.recog_cards(image);
			lplay.drawResult(drawImage);
			for(int i = 0; i < cards.size(); i++) cout<<cards[i].str();
			cout<<endl;
		}

		if(isImagePatchSame(image, "ddz_patch_me_clock.png")) cout<<"It is my turn"<<endl;
		else if(isImagePatchSame(image, "ddz_patch_me_has_not_play.png")) cout<<"I don't play"<<endl;
		else if(isImagePatchSame(image, "ddz_patch_me_liandui.png")) cout<<"I played lian dui"<<endl;
		else if(isImagePatchSame(image, "ddz_patch_me_shunzi.png")) cout<<"I played lian shunzi"<<endl;
		else
		{
			cout<<"I played ";
			RecogPlay mplay("play", "myself");
			cards = mplay.recog_cards(image);
			mplay.drawResult(drawImage);
			for(int i = 0; i < cards.size(); i++) cout<<cards[i].str();
			cout<<endl;
		}

		if(isImagePatchSame(image, "ddz_patch_right_clock.png")) cout<<"It is right turn"<<endl;
		else if(isImagePatchSame(image, "ddz_patch_right_has_not_play.png")) cout<<"Right doesn't play"<<endl;
		else if(isImagePatchSame(image, "ddz_patch_right_liandui.png")) cout<<"Right played liandui"<<endl;
		else if(isImagePatchSame(image, "ddz_patch_right_shunzi.png")) cout<<"Right played shunzi"<<endl;
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

	cout<<"====== Display  ======"<<endl;
	RecogDisplay disp("showed");
	cards = disp.recog_cards(image);
	//disp.drawResult(drawImage);
	for(int i = 0; i < cards.size(); i++) cout<<cards[i].str();
	cout<<endl;

	cvSaveImage((infile + ".out.png").c_str(), drawImage);
	releaseAllTemplates();
	cvReleaseImage(&image);
	cvReleaseImage(&drawImage);
	return 0;
}
