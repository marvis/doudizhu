#include <map>
#include "image_patches.h"
#include "common.h"
#include "path.h"

using namespace std;
using namespace cv;

map<string, ImagePatch> map_patches;

ImagePatch allPatches[] = 
{
	// 等待开始阶段
	ImagePatch(221, 745, "ddz_patch_click_start.png"),
	ImagePatch(274, 496, "ddz_patch_waiting_start.png"),
	
	// 起牌阶段
	ImagePatch(281, 547, "ddz_patch_me_ming_pai.png"),             // 明牌吗?

	//叫地主阶段 
	ImagePatch(314, 592, "ddz_patch_me_clock_fight.png"),        // 叫/抢地主阶段
	ImagePatch(276, 362, "ddz_patch_me_is_qiang.png"),          // 抢地主吗?

	ImagePatch(270, 546, "ddz_patch_me_has_jiao.png"),          // 叫地主
	ImagePatch(270, 546, "ddz_patch_me_has_qiang.png"),         // 抢地主
	ImagePatch(270, 580, "ddz_patch_me_has_not_jiao.png"),      // 不叫
	ImagePatch(270, 580, "ddz_patch_me_has_not_qiang.png"),     // 不抢

	ImagePatch(370, 287, "ddz_patch_left_has_jiao.png"),        // 叫地主
	ImagePatch(370, 287, "ddz_patch_left_has_qiang.png"),       // 抢地主
	ImagePatch(370, 353, "ddz_patch_left_has_not_jiao.png"),    // 不叫
	ImagePatch(370, 353, "ddz_patch_left_has_not_qiang.png"),   // 不抢

	ImagePatch(369, 806, "ddz_patch_right_has_jiao.png"),       // 叫地主
	ImagePatch(369, 806, "ddz_patch_right_has_qiang.png"),      // 抢地主
	ImagePatch(369, 806, "ddz_patch_right_has_not_jiao.png"),   // 不叫
	ImagePatch(369, 806, "ddz_patch_right_has_not_qiang.png"),  // 不抢

	// 加倍阶段
	ImagePatch(277, 397, "ddz_patch_me_is_jia.png"),            // 加倍吗?
	ImagePatch(270, 546, "ddz_patch_me_has_not_jia.png"),       // 不加倍
	ImagePatch(370, 287, "ddz_patch_left_has_not_jia.png"),     // 不加倍
	ImagePatch(369, 806, "ddz_patch_right_has_not_jia.png"),    // 不加倍
	ImagePatch(270, 612, "ddz_patch_me_jia_or_not.png"),        // (不)加倍
	ImagePatch(370, 350, "ddz_patch_left_jia_or_not.png"),      // (不)加倍
	ImagePatch(369, 873, "ddz_patch_right_jia_or_not.png"),     // (不)加倍

	// playing
	ImagePatch(314, 480, "ddz_patch_me_clock.png"),             // 出牌阶段
	ImagePatch(270, 580, "ddz_patch_me_has_not_play.png"),      // 不出
	ImagePatch(270, 580, "ddz_patch_me_liandui.png"),           // 连对
	ImagePatch(270, 580, "ddz_patch_me_shunzi.png"),            // 顺子

	ImagePatch(409, 249, "ddz_patch_left_clock.png"),           // 出牌阶段
	ImagePatch(370, 353, "ddz_patch_left_has_not_play.png"),    // 不出
	ImagePatch(370, 260, "ddz_patch_left_liandui.png"),         // 连对
	ImagePatch(370, 260, "ddz_patch_left_shunzi.png"),          // 顺子

	ImagePatch(402, 942, "ddz_patch_right_clock.png"),          // 出牌阶段
	ImagePatch(369, 806, "ddz_patch_right_has_not_play.png"),   // 不出
	ImagePatch(369, 870, "ddz_patch_right_liandui.png"),        // 连对
	ImagePatch(369, 870, "ddz_patch_right_shunzi.png"),         // 顺子

	// other
	ImagePatch(639, 36, "ddz_patch_last3_bkg.png"),
	ImagePatch(363, 48, "ddz_patch_boss_left1.png"),
	ImagePatch(363, 48, "ddz_patch_boss_left2.png"),
	ImagePatch(358, 1208, "ddz_patch_boss_right1.png"),
	ImagePatch(358, 1208, "ddz_patch_boss_right2.png"),
	ImagePatch(470, 400, "ddz_patch_stage_profile.png"),

	// card number
	ImagePatch(319, 151, "ddz_patch_cardnum_left_l2.png"),
	ImagePatch(319, 151, "ddz_patch_cardnum_left_l1.png"),
	ImagePatch(319, 151, "ddz_patch_cardnum_left_l0.png"),
	ImagePatch(319, 166, "ddz_patch_cardnum_left_m1.png"),
	ImagePatch(319, 166, "ddz_patch_cardnum_left_m2.png"),
	ImagePatch(319, 166, "ddz_patch_cardnum_left_m3.png"),
	ImagePatch(319, 166, "ddz_patch_cardnum_left_m4.png"),
	ImagePatch(319, 166, "ddz_patch_cardnum_left_m5.png"),
	ImagePatch(319, 166, "ddz_patch_cardnum_left_m6.png"),
	ImagePatch(319, 166, "ddz_patch_cardnum_left_m7.png"),
	ImagePatch(319, 166, "ddz_patch_cardnum_left_m8.png"),
	ImagePatch(319, 166, "ddz_patch_cardnum_left_m9.png"),
	ImagePatch(319, 181, "ddz_patch_cardnum_left_r0.png"),
	ImagePatch(319, 181, "ddz_patch_cardnum_left_r1.png"),
	ImagePatch(319, 181, "ddz_patch_cardnum_left_r2.png"),
	ImagePatch(319, 181, "ddz_patch_cardnum_left_r3.png"),
	ImagePatch(319, 181, "ddz_patch_cardnum_left_r4.png"),
	ImagePatch(319, 181, "ddz_patch_cardnum_left_r5.png"),
	ImagePatch(319, 181, "ddz_patch_cardnum_left_r6.png"),
	ImagePatch(319, 181, "ddz_patch_cardnum_left_r7.png"),
	ImagePatch(319, 181, "ddz_patch_cardnum_left_r8.png"),
	ImagePatch(319, 181, "ddz_patch_cardnum_left_r9.png"),

	ImagePatch(319, 1043, "ddz_patch_cardnum_right_l0.png"),
	ImagePatch(319, 1043, "ddz_patch_cardnum_right_l1.png"),
	ImagePatch(319, 1043, "ddz_patch_cardnum_right_l2.png"),
	ImagePatch(319, 1058, "ddz_patch_cardnum_right_m1.png"),
	ImagePatch(319, 1058, "ddz_patch_cardnum_right_m2.png"),
	ImagePatch(319, 1058, "ddz_patch_cardnum_right_m3.png"),
	ImagePatch(319, 1058, "ddz_patch_cardnum_right_m4.png"),
	ImagePatch(319, 1058, "ddz_patch_cardnum_right_m5.png"),
	ImagePatch(319, 1058, "ddz_patch_cardnum_right_m6.png"),
	ImagePatch(319, 1058, "ddz_patch_cardnum_right_m7.png"),
	ImagePatch(319, 1058, "ddz_patch_cardnum_right_m8.png"),
	ImagePatch(319, 1058, "ddz_patch_cardnum_right_m9.png"),
	ImagePatch(319, 1073, "ddz_patch_cardnum_right_r0.png"),
	ImagePatch(319, 1073, "ddz_patch_cardnum_right_r1.png"),
	ImagePatch(319, 1073, "ddz_patch_cardnum_right_r2.png"),
	ImagePatch(319, 1073, "ddz_patch_cardnum_right_r3.png"),
	ImagePatch(319, 1073, "ddz_patch_cardnum_right_r4.png"),
	ImagePatch(319, 1073, "ddz_patch_cardnum_right_r5.png"),
	ImagePatch(319, 1073, "ddz_patch_cardnum_right_r6.png"),
	ImagePatch(319, 1073, "ddz_patch_cardnum_right_r7.png"),
	ImagePatch(319, 1073, "ddz_patch_cardnum_right_r8.png"),
	ImagePatch(319, 1073, "ddz_patch_cardnum_right_r9.png"),
};

//build map_patches and load images
void build_patch_map()
{
	for(int i = 0; i < sizeof(allPatches)/sizeof(ImagePatch); i++)
	{
		ImagePatch p = allPatches[i];
		string file = p.file;
		map_patches[file] = p;
		//map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	}
}

bool isImagePatchSame(IplImage * image, string file, double thresh)
{
	if(map_patches.find(file) == map_patches.end())
	{
		cerr<<"Undefined file "<<file<<endl;
		return false;
	}
	ImagePatch p = map_patches[file];
	assert(file == p.file);
	return isImageSame(image, p.x, p.y, p.file, thresh);
}

// 等待开始
// 起牌阶段
// 抢地主阶段
// 出牌阶段
// 游戏结束
bool isCardExist(IplImage * image)
{
	int x0 = 321, y0 = 216;
	int x1 = 354, y1 = 246;
	for(int x = x0; x <= x1; x++)
	{
		int y = y0 + (x - x0);
		int R = CV_IMAGE_ELEM(image, unsigned char, y, 3*x + 2);
		int G = CV_IMAGE_ELEM(image, unsigned char, y, 3*x + 1);
		int B = CV_IMAGE_ELEM(image, unsigned char, y, 3*x + 0);
		double diff = ABS(R - 132) + ABS(G - 190) + ABS(B - 238);
		if(diff < 30) return true;
	}
	return false;
}

int which_game_stage(IplImage * image)
{
	if(isImagePatchSame(image, "ddz_patch_click_start.png") || isImagePatchSame(image, "ddz_patch_waiting_start.png")) return STAGE_GAME_WAITING;
	if(isImagePatchSame(image, "ddz_patch_me_ming_pai.png")) return STAGE_FETCH_CARDS; // 起牌到叫地主之间的那个状态可以忽略
	bool is_card_exist = isCardExist(image);
	if(isImagePatchSame(image, "ddz_patch_last3_bkg.png") && is_card_exist) return STAGE_LAND_CHOOSE;
	if(isImagePatchSame(image, "ddz_patch_me_is_jia.png") || isImagePatchSame(image, "ddz_patch_left_jia_or_not.png") ||
	   isImagePatchSame(image, "ddz_patch_left_jia_or_not.png") || isImagePatchSame(image, "ddz_patch_me_jia_or_not.png")) return STAGE_DOUBLE_SCORE;
	if(isImagePatchSame(image, "ddz_patch_left_clock.png") || isImagePatchSame(image, "ddz_patch_me_clock.png") ||
	   isImagePatchSame(image, "ddz_patch_right_clock.png") || is_card_exist) return STAGE_PLAYING;
	if(isImagePatchSame(image, "ddz_patch_stage_profile.png")) return STAGE_UNKNOWN; 
	return STAGE_GAME_WAITING;
}
