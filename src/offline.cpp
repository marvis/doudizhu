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
	if(argc == 2) fid = atoi(argv[1]);
	while(1)
	{
		infile = "/Users/xiaohang/Test/doudizhu/data2/screen" + num2str(fid) + ".png";
		if(file_exists(infile))
		{
			system("clear");
			cout<<infile<<endl;
			ana.process(infile);
		}
		else
		{
			cout<<infile<<" doesn't exist"<<endl;
			break;
		}
		//sleep(1);
		usleep(500000);
		fid++;
	}
	return 0;
}
