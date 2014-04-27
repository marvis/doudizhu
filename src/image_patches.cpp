#include <map>
#include "image_patches.h"
#include "common.h"
#include "path.h"

using namespace std;
using namespace cv;

map<string, ImagePatch> map_patches;

ImagePatch allPatches[] = 
{
	// before game
	ImagePatch(221, 745, "ddz_patch_start_game.png"),

	// in the begining
	ImagePatch(281, 547, "ddz_patch_ming_pai.png"),             // 明牌吗?
	ImagePatch(275, 387, "ddz_patch_me_is_jia.png"),            // 加倍吗?
	ImagePatch(276, 362, "ddz_patch_me_is_qiang.png"),          // 抢地主吗?

	ImagePatch(270, 546, "ddz_patch_me_has_jiao.png"),          // 叫地主
	ImagePatch(270, 546, "ddz_patch_me_has_qiang.png"),         // 抢地主
	ImagePatch(270, 546, "ddz_patch_me_has_not_jia.png"),       // 不加倍
	ImagePatch(270, 580, "ddz_patch_me_has_not_jiao.png"),      // 不叫
	ImagePatch(270, 580, "ddz_patch_me_has_not_qiang.png"),     // 不抢

	ImagePatch(370, 287, "ddz_patch_left_has_jiao.png"),        // 叫地主
	ImagePatch(370, 287, "ddz_patch_left_has_qiang.png"),       // 抢地主
	ImagePatch(370, 287, "ddz_patch_left_has_not_jia.png"),     // 不加倍
	ImagePatch(370, 353, "ddz_patch_left_has_not_jiao.png"),    // 不叫
	ImagePatch(370, 353, "ddz_patch_left_has_not_qiang.png"),   // 不抢

	ImagePatch(369, 806, "ddz_patch_right_has_jiao.png"),       // 叫地主
	ImagePatch(369, 806, "ddz_patch_right_has_qiang.png"),      // 抢地主
	ImagePatch(369, 806, "ddz_patch_right_has_not_jia.png"),    // 不加倍
	ImagePatch(369, 806, "ddz_patch_right_has_not_jiao.png"),   // 不叫
	ImagePatch(369, 806, "ddz_patch_right_has_not_qiang.png"),  // 不抢

	// start play
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
	ImagePatch(639, 36, "ddz_patch_last3_bkg.png")
};

//build map_patches and load images
void build_patch_map()
{
	for(int i = 0; i < sizeof(allPatches)/sizeof(ImagePatch); i++)
	{
		ImagePatch p = allPatches[i];
		string file = p.file;
		map_patches[file] = p;
		map_allImgs[file] = cvLoadImage((DDZTMPLPATH + file).c_str(), 1);
	}
}

bool isImagePatchSame(IplImage * image, string file)
{
	ImagePatch p = map_patches[file];
	assert(file == p.file);
	return isImageSame(image, p.x, p.y, p.file, 10);
}
