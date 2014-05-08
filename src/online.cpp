#include "analysis.h"

string infile = "";

bool file_exists(string filename){
	ifstream ifile(filename);
	return ifile;
}
int main(int argc, char ** argv)
{
	build_patch_map(); // 为使用isImagePatchSame 做准备

	Analysis ana;
	int fid = 1;
	while(fid <= 1075)
	{
		ostringstream oss;
		oss << "adb shell screencap /storage/sdcard0/screen"<<fid<<".png && ";
		oss << "adb pull /storage/sdcard0/screen"<<fid<<".png ../data2/ > /dev/null 2>&1 && ";
		oss << "adb shell rm /storage/sdcard0/screen"<<fid<<".png &";
		system(oss.str().c_str());

		if(fid > 3)
		{
			infile = "/Users/xiaohang/Test/doudizhu/data2/screen" + num2str(fid - 5) + ".png";
			if(file_exists(infile))
			{
				system("clear");
				cout<<infile<<endl;
				ana.process(infile);
			}
			else
				cout<<infile<<" doesn't exist"<<endl;
		}
		fid++;
		usleep(500000);
	}
	return 0;
}
