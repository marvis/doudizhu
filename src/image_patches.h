#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <string>
#include "path.h"

using namespace std;
using namespace cv;

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
