#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <string>
#include "path.h"

using namespace std;
using namespace cv;

enum Stage {STAGE_GAME_WAITING = 0, STAGE_FETCH_CARDS, STAGE_LAND_CHOOSE, STAGE_DOUBLE_SCORE, 
	STAGE_PLAYING, STAGE_UNKNOWN};
class ImagePatch
{
	public:
		int x;
		int y;
		string file;
		ImagePatch()
		{
			x = -1;
			y = -1;
			file = "zzzzzz";
		}
		ImagePatch(int _x, int _y, string _file) : x(_x), y(_y), file(_file)
		{
		}
		bool operator < (const ImagePatch & other)
		{
			return this->file < other.file;
		}
};

void build_patch_map();
bool isImagePatchSame(IplImage * image, string file);
int which_game_stage(IplImage * image);
