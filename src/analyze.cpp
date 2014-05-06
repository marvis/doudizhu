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

string infile;

bool check_stage(int prev_stage, int cur_stage)
{
	if(prev_stage == STAGE_UNKNOWN) return true;
	if(cur_stage == STAGE_UNKNOWN) return false;
	if(prev_stage == STAGE_GAME_WAITING) return (cur_stage == STAGE_GAME_WAITING || cur_stage == STAGE_FETCH_CARDS);
	if(prev_stage == STAGE_FETCH_CARDS) return (cur_stage == STAGE_FETCH_CARDS || cur_stage == STAGE_LAND_CHOOSE);
	if(prev_stage == STAGE_LAND_CHOOSE) return (cur_stage == STAGE_LAND_CHOOSE || cur_stage == STAGE_DOUBLE_SCORE || cur_stage == STAGE_PLAYING);
	if(prev_stage == STAGE_DOUBLE_SCORE) return (cur_stage == STAGE_DOUBLE_SCORE || cur_stage == STAGE_PLAYING);
	if(prev_stage == STAGE_PLAYING) return (cur_stage == STAGE_PLAYING || cur_stage == STAGE_GAME_WAITING);
	return false;
}

string stage_str[] = {"等待游戏", "发牌阶段", "叫地主阶段", "加分阶段", "游戏进行中", "游戏结束"};
int main(int argc, char ** argv)
{
	IplImage * image = 0;
	int fid = 1;
	string cmd = "adb shell screencap /storage/sdcard0/screen.png && adb pull /storage/sdcard0/screen.png ";
	build_patch_map(); // 为使用isImagePatchSame 做准备

	int prev_stage = STAGE_UNKNOWN;
	while(fid++)
	{
		infile = "data/screen" + num2str(fid) + ".png";
		//system((cmd + infile).c_str());

		image = cvLoadImage(infile.c_str(), 1);
		int stage = which_game_stage(image);
		cout<<infile<<" "<<stage_str[stage]<<endl;
		if(!check_stage(prev_stage, stage))
		{
			cerr<<"Invalid stage"<<endl;
			continue;
		}
		prev_stage = stage;
		cvReleaseImage(&image);
	}
	return 0;
}
