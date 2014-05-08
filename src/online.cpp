#include "analysis.h"

string infile = "";

bool file_exists(string filename){
	ifstream ifile(filename);
	return ifile;
}
int main(int argc, char ** argv)
{
	system("rm -rf /Users/xiaohang/Test/doudizhu/data2/*");
	build_patch_map(); // 为使用isImagePatchSame 做准备

	Analysis ana;
	int fid = 1;
	while(1)
	{
		ostringstream oss;
		oss << "adb shell screencap /storage/sdcard0/screen"<<fid<<".png && ";
		oss << "adb pull /storage/sdcard0/screen"<<fid<<".png ../data2/ > /dev/null 2>&1 && ";
		oss << "adb shell rm /storage/sdcard0/screen"<<fid<<".png &";
		system(oss.str().c_str());

		vector<int> fids;
		for(int id = fid - 15; id < fid; id++)
		{
			infile = "/Users/xiaohang/Test/doudizhu/data2/screen" + num2str(id) + ".png";
			if(file_exists(infile))
			{
				if(fids.empty()) fids.push_back(id);
				else if(*fids.rbegin() == id-1) fids.push_back(id);
				else break;
			}
		}
		if(!fids.empty())
		{
			for(int i = 0; i < fids.size() - 1; i++)
			{
				infile = "/Users/xiaohang/Test/doudizhu/data2/screen" + num2str(fids[i]) + ".png";
				ana.process(infile);
				if(argc == 2)
				{
					string cmd = "mv " + infile + " /Users/xiaohang/Test/doudizhu/data1/";
					system(cmd.c_str());
				}
				else
				{
					string cmd = "rm " + infile;
					system(cmd.c_str());
				}
			}
		}
		fid++;
		//usleep(500000);
		sleep(1);
	}
	return 0;
}
