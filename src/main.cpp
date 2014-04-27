#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "recog_last.h"
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
	loadAllTemplates();

	IplImage * image = cvLoadImage(argv[1], 1);
	IplImage * drawImage = cvLoadImage(argv[1], 1);

	RecogLast last("last");
	vector<Card> cards = last.recog_cards(image);
	for(int i = 0; i < cards.size(); i++) cout<<cards[i].str();
	cout<<endl;

	last.drawResult(drawImage);
	return 0;
}
