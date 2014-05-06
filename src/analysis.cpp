#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "recog_last.h"
#include "recog_play.h"
#include "recog_inhand.h"
#include "recog_ming.h"
#include "image_patches.h"
#include "common.h"
#include "unistd.h"

using namespace std;
using namespace cv;

string infile = "";
string stage_str[] = {"等待游戏", "发牌阶段", "叫地主阶段", "加分阶段", "游戏进行中", "游戏结束"};

bool is_unknown_exist(vector<Card> & cards)
{
	for(int i = 0; i < cards.size(); i++)
	{
		if(cards[i].num == NUM_UNKNOWN) return true;
		if(cards[i].type == TYPE_UNKNOWN) return true;
	}
	return false;
}

class Analysis
{
	public:
		vector<Card> myself_cards;
		vector<Card> lshowed_cards;
		vector<Card> rshowed_cards;
		vector<Card> lplayed_cards;
		vector<Card> rplayed_cards;
		vector<Card> iplayed_cards;
		vector<Card> last3_cards;

		set<Card> allplayed_cards;
		vector<vector<Card> > history_plays;
		vector<string> history_players;

		int prev_stage;
		int cur_stage;
		IplImage * image;

		Analysis()
		{
			init();
		}
		void init()
		{
			image = 0;
			prev_stage = STAGE_UNKNOWN;
			cur_stage = STAGE_UNKNOWN;

			last3_cards.clear();
			myself_cards.clear();
			lplayed_cards.clear();
			iplayed_cards.clear();
			rplayed_cards.clear();

			allplayed_cards.clear();
			history_plays.clear();
			history_players.clear();
		}
		bool is_valid_stage()
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
		void add_to_history(vector<Card> & cards, string who)
		{
			if(cards.empty()) return;
			if(is_unknown_exist(cards)) return;
			for(int i = 0; i < cards.size(); i++) 
				if(allplayed_cards.find(cards[i]) != allplayed_cards.end()) return; // alread add

			history_plays.push_back(cards);
			history_players.push_back(who);
			for(int i = 0; i < cards.size(); i++) allplayed_cards.insert(cards[i]);
		}
		void disp_hidden_cards()
		{
			set<Card> known_cards = allplayed_cards;
			for(int i = 0; i < myself_cards.size(); i++) known_cards.insert(myself_cards[i]);
			vector<Card> hidden_cards;
			Card card;
			card.num = NUM_JOKER;
			card.type = TYPE_RED_JOKER;
			if(known_cards.find(card) == known_cards.end()) hidden_cards.push_back(card);
			card.type = TYPE_BLACK_JOKER;
			if(known_cards.find(card) == known_cards.end()) hidden_cards.push_back(card);
			for(int num = NUM_2; num >= NUM_3; num--)
			{
				for(int type = TYPE_SPADE; type <= TYPE_DIAMOND; type++)
				{
					card.num = num;
					card.type = type;
					if(known_cards.find(card) == known_cards.end()) hidden_cards.push_back(card);
				}
			}
			cout<<"**********************************************************************************************************"<<endl;
			int i = 0;
			while(i < hidden_cards.size())
			{
				vector<Card> cards;
				int ncards = 0;
				while(ncards < 17 && i < hidden_cards.size())
				{
					cards.push_back(hidden_cards[i]);
					ncards++;
					i++;
				}
				disp_cards(cards, "hidden");
			}
		}
		void disp_history()
		{
			if(!history_plays.empty()) cout<<"**********************************************************************************************************"<<endl;
			int i = 0;
			while(i < history_plays.size())
			{
				vector<vector<Card> > cards_list;
				vector<string> msg_list;
				int ncards = 0;
				while(ncards < 14 && i < history_plays.size())
				{
					vector<Card> & cards = history_plays[i];
					string & msg = history_players[i];
					cards_list.push_back(cards);
					msg_list.push_back(msg);
					ncards += cards.size();
					i++;
				}
				disp_played_cards( cards_list, msg_list);
				//disp_cards(history_plays[i], history_players[i]);
			}
		}
		void recog_last3()
		{
			if(!image)
			{
				last3_cards.clear();
				return;
			}
			if(cur_stage == STAGE_PLAYING)
			{
				last3_cards = RecogLast("last").recog_cards(image);
			}
			else last3_cards.clear();
		}
		void recog_myself()
		{
			if(!image) 
			{
				myself_cards.clear();
				return;
			}
			if(cur_stage == STAGE_LAND_CHOOSE || cur_stage == STAGE_DOUBLE_SCORE || cur_stage == STAGE_PLAYING)
			{
				myself_cards = RecogInhand("inhand").recog_cards(image);
			}
			else myself_cards.clear();
		}
		void recog_iplayed()
		{
			if(!image)
			{
				iplayed_cards.clear();
				return;
			}
			if(cur_stage == STAGE_PLAYING && !isImagePatchSame(image, "ddz_patch_me_clock.png") && !isImagePatchSame(image, "ddz_patch_me_has_not_play.png") &&
			   !isImagePatchSame(image, "ddz_patch_me_liandui.png") && !isImagePatchSame(image, "ddz_patch_me_shunzi.png"))
				iplayed_cards = RecogPlay("play", "myself").recog_cards(image);
			else iplayed_cards.clear();
		}
		void recog_lplayed()
		{
			if(!image)
			{
				lplayed_cards.clear();
				return;
			}
			if(cur_stage == STAGE_PLAYING && !isImagePatchSame(image, "ddz_patch_left_clock.png") && !isImagePatchSame(image, "ddz_patch_left_has_not_play.png") &&
			   !isImagePatchSame(image, "ddz_patch_left_liandui.png") && !isImagePatchSame(image, "ddz_patch_left_shunzi.png"))
				lplayed_cards = RecogPlay("play", "left").recog_cards(image);
			else lplayed_cards.clear();
		}
		void recog_rplayed()
		{
			if(!image)
			{
				rplayed_cards.clear();
				return;
			}
			if(cur_stage == STAGE_PLAYING && !isImagePatchSame(image, "ddz_patch_right_clock.png") && !isImagePatchSame(image, "ddz_patch_right_has_not_play.png") &&
			   !isImagePatchSame(image, "ddz_patch_right_liandui.png") && !isImagePatchSame(image, "ddz_patch_right_shunzi.png"))
				rplayed_cards = RecogPlay("play", "right").recog_cards(image);
			else rplayed_cards.clear();
		}

		void process(string file)
		{
			//cout<<file<<" ";
			image = cvLoadImage(file.c_str(), 1);
			cur_stage = which_game_stage(image);

			if(!is_valid_stage())
			{
				cerr<<"Invalid stage"<<endl;
				cvReleaseImage(&image);
				prev_stage = cur_stage;
				return;
			}
			cout<<stage_str[cur_stage]<<endl;
			if(cur_stage == STAGE_GAME_WAITING)
			{
				init();
				return;
			}
			//recog_last3(); if(!last3_cards.empty()) disp_cards(last3_cards, "last3");
			recog_myself(); if(!myself_cards.empty()) disp_cards(myself_cards, "myself");
			recog_lplayed(); //if(!lplayed_cards.empty()) disp_cards(lplayed_cards, "lplayed");
			recog_iplayed(); //if(!iplayed_cards.empty()) disp_cards(iplayed_cards, "iplayed");
			recog_rplayed(); //if(!rplayed_cards.empty()) disp_cards(rplayed_cards, "rplayed");
			if(lplayed_cards.empty())
			{
				add_to_history(iplayed_cards, "iplayed");
				add_to_history(rplayed_cards, "rplayed");
				//disp_played_cards(iplayed_cards, "iplayed", rplayed_cards, "rplayed");	
			}
			else if(iplayed_cards.empty())
			{
				add_to_history(rplayed_cards, "rplayed");
				add_to_history(lplayed_cards, "lplayed");
				//disp_played_cards(rplayed_cards, "rplayed", lplayed_cards, "lplayed");
			}
			else 
			{
				add_to_history(lplayed_cards, "lplayed");
				add_to_history(iplayed_cards, "iplayed");
				//disp_played_cards(lplayed_cards, "lplayed", iplayed_cards, "iplayed");
			}
			disp_history();
			disp_hidden_cards();
			prev_stage = cur_stage;
			cvReleaseImage(&image);
		}
};

int main(int argc, char ** argv)
{
	build_patch_map(); // 为使用isImagePatchSame 做准备

	Analysis ana;
	int fid = 1;
	while(fid <= 1075)
	{
		string cmd = "adb shell screencap /storage/sdcard0/screen.png && adb pull /storage/sdcard0/screen.png ";
		infile = "/Users/xiaohang/Test/doudizhu/data2/screen" + num2str(fid) + ".png";
		system((cmd + infile + " > /dev/null 2>&1").c_str());
		//sleep(2);
		system("clear");
		ana.process(infile);
		fid++;
	}
	return 0;
}
