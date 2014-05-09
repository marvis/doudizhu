#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "recog_last.h"
#include "recog_play.h"
#include "recog_inhand.h"
#include "recog_ming.h"
#include "image_patches.h"
#include "common.h"

using namespace std;
using namespace cv;

void recog_cardnum(IplImage * image, int &nlcards, int &nrcards);

string infile;
string stage_str[] = {"等待游戏", "发牌阶段", "叫地主阶段", "加分阶段", "游戏进行中", "未知阶段"};

int main(int argc, char ** argv)
{
	if(argc == 1) 
	{
		cout<<"Usage: "<<argv[0]<<" <screenimg>"<<endl;
		return 0;
	}
	infile = argv[1];

	IplImage * image = cvLoadImage(argv[1], 1);
	IplImage * drawImage = cvLoadImage(argv[1], 1);

	build_patch_map();
	string stage = stage_str[which_game_stage(image)];
	cout<<stage<<endl;

	vector<Card> cards;
	if(!isImagePatchSame(image, "ddz_patch_last3_bkg.png"))
	{
		cout<<"======  last  ======"<<endl;
		RecogLast last("last");
		cards = last.recog_cards(image);
		last.drawResult(drawImage);
		disp_cards(cards);
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
			cout<<"Left played : "<<endl;;
			RecogPlay lplay("play", "left");
			cards = lplay.recog_cards(image);
			lplay.drawResult(drawImage);
			disp_cards(cards);
		}

		if(isImagePatchSame(image, "ddz_patch_me_clock.png")) cout<<"It is my turn"<<endl;
		else if(isImagePatchSame(image, "ddz_patch_me_has_not_play.png")) cout<<"I don't play"<<endl;
		else if(isImagePatchSame(image, "ddz_patch_me_liandui.png")) cout<<"I played lian dui"<<endl;
		else if(isImagePatchSame(image, "ddz_patch_me_shunzi.png")) cout<<"I played lian shunzi"<<endl;
		else
		{
			cout<<"I played "<<endl;
			RecogPlay mplay("play", "myself");
			cards = mplay.recog_cards(image);
			mplay.drawResult(drawImage);
			disp_cards(cards);
		}

		if(isImagePatchSame(image, "ddz_patch_right_clock.png")) cout<<"It is right turn"<<endl;
		else if(isImagePatchSame(image, "ddz_patch_right_has_not_play.png")) cout<<"Right doesn't play"<<endl;
		else if(isImagePatchSame(image, "ddz_patch_right_liandui.png")) cout<<"Right played liandui"<<endl;
		else if(isImagePatchSame(image, "ddz_patch_right_shunzi.png")) cout<<"Right played shunzi"<<endl;
		else
		{
			cout<<"Right played : "<<endl;
			RecogPlay rplay("play", "right");
			cards = rplay.recog_cards(image);
			rplay.drawResult(drawImage);
			disp_cards(cards);
		}
		cout<<endl;
	}


	cout<<"======  Inhand  ======"<<endl;
	RecogInhand inhand("inhand");
	cards = inhand.recog_cards(image);
	inhand.drawResult(drawImage, -100);
	disp_cards(cards);

	//cout<<"====== Display  ======"<<endl;
	cout<<"Left showed: "<<endl;
	RecogMing lming("showed", "left");
	cards = lming.recog_cards(image);
	lming.drawResult(drawImage);
	disp_cards(cards);

	cout<<"Right showed: "<<endl;
	RecogMing rming("showed", "right");
	cards = rming.recog_cards(image);
	rming.drawResult(drawImage);
	disp_cards(cards);

	cout<<"======= Card Num ========"<<endl;
	int nlcards = 0, nrcards = 0;
	recog_cardnum(image, nlcards, nrcards);

	cout<<"nlcards = "<<nlcards<<endl;
	cout<<"nrcards = "<<nrcards<<endl;

	cvSaveImage((infile + ".out.png").c_str(), drawImage);
	releaseAllTemplates();
	cvReleaseImage(&image);
	cvReleaseImage(&drawImage);
	return 0;
}

void recog_cardnum(IplImage * image, int &nlcards, int &nrcards)
{
	/*
	if(isImagePatchSame(image, "ddz_patch_cardnum_left_l2.png", 40)) nlcards = 20;
	else if(isImagePatchSame(image, "ddz_patch_cardnum_left_l1.png", 40))
	{
		if(isImagePatchSame(image, "ddz_patch_cardnum_left_r0.png")) nlcards = 10;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_left_r1.png")) nlcards = 11;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_left_r2.png")) nlcards = 12;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_left_r3.png")) nlcards = 13;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_left_r4.png")) nlcards = 14;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_left_r5.png")) nlcards = 15;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_left_r6.png")) nlcards = 16;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_left_r7.png")) nlcards = 17;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_left_r8.png")) nlcards = 18;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_left_r9.png")) nlcards = 19;
		else 
		{
			cerr<<"unknown cards number for left"<<endl;
		}
	}
	else if(isImagePatchSame(image, "ddz_patch_cardnum_left_l0.png", 40))
	{
		if(isImagePatchSame(image, "ddz_patch_cardnum_left_m1.png")) nlcards = 1;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_left_m2.png")) nlcards = 2;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_left_m3.png")) nlcards = 3;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_left_m4.png")) nlcards = 4;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_left_m5.png")) nlcards = 5;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_left_m6.png")) nlcards = 6;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_left_m7.png")) nlcards = 7;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_left_m8.png")) nlcards = 8;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_left_m9.png")) nlcards = 9;
		else
		{
			cerr<<"unknown cards number for left"<<endl;
		}
	}
	else
	{
		cerr<<"unknown card number"<<endl;
	}
	*/

	if(isImagePatchSame(image, "ddz_patch_cardnum_right_l2.png", 60)) nrcards = 20;
	else if(isImagePatchSame(image, "ddz_patch_cardnum_right_l1.png", 60))
	{
		if(isImagePatchSame(image, "ddz_patch_cardnum_right_r0.png")) nrcards = 10;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_right_r1.png")) nrcards = 11;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_right_r2.png")) nrcards = 12;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_right_r3.png")) nrcards = 13;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_right_r4.png")) nrcards = 14;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_right_r5.png")) nrcards = 15;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_right_r6.png")) nrcards = 16;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_right_r7.png")) nrcards = 17;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_right_r8.png")) nrcards = 18;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_right_r9.png")) nrcards = 19;
		else 
		{
			cerr<<"unknown cards number for right right"<<endl;
		}
	}
	else if(isImagePatchSame(image, "ddz_patch_cardnum_right_l0.png", 60))
	{
		if(isImagePatchSame(image, "ddz_patch_cardnum_right_m1.png")) nrcards = 1;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_right_m2.png")) nrcards = 2;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_right_m3.png")) nrcards = 3;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_right_m4.png")) nrcards = 4;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_right_m5.png")) nrcards = 5;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_right_m6.png")) nrcards = 6;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_right_m7.png")) nrcards = 7;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_right_m8.png")) nrcards = 8;
		else if(isImagePatchSame(image, "ddz_patch_cardnum_right_m9.png")) nrcards = 9;
		else
		{
			cerr<<"unknown cards number for right middle"<<endl;
		}
	}
	else
	{
		cerr<<"unknown card number"<<endl;
	}
}
