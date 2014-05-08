#ifndef __ANALYSIS_H__
#define __ANALYSIS_H__

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "recog_last.h"
#include "recog_play.h"
#include "recog_inhand.h"
#include "recog_ming.h"
#include "image_patches.h"
#include "common.h"
#include "analysis.h"
#include "unistd.h"
#include <fstream>

using namespace std;
using namespace cv;

class Analysis
{
	enum {UNKNOWN = 1, SHOWED, UNSHOWED};
	public:
		vector<Card> myself_cards;
		vector<Card> lshowed_cards;
		vector<Card> rshowed_cards;
		vector<Card> lplayed_cards;
		vector<Card> rplayed_cards;
		vector<Card> iplayed_cards;
		vector<Card> last3_cards;

		vector<Card> prev_myself_cards;
		vector<Card> prev_lshowed_cards;
		vector<Card> prev_rshowed_cards;
		vector<Card> prev_lplayed_cards;
		vector<Card> prev_rplayed_cards;
		vector<Card> prev_iplayed_cards;
		vector<Card> prev_last3_cards;

		int islshowed;
		int isrshowed;

		set<Card> allplayed_history;
		set<Card> iplayed_history;
		set<Card> lplayed_history;
		set<Card> rplayed_history;
		vector<vector<Card> > history_plays;
		vector<string> history_players;

		int prev_stage;
		int cur_stage;
		IplImage * image;

		Analysis()
		{
			init();
		}
		bool isExistUnknownCard(vector<Card> & cards)
		{
			for(int i = 0; i < cards.size(); i++)
			{
				if(cards[i].num == NUM_UNKNOWN) return true;
				if(cards[i].type == TYPE_UNKNOWN) return true;
			}
			return false;
		}
		int unknownCardNum(vector<Card> & cards)
		{
			int n = 0;
			for(int i = 0; i < cards.size(); i++)
			{
				if(cards[i].num == NUM_UNKNOWN || cards[i].type == TYPE_UNKNOWN) n++;
			}
			return n;
		}
		bool isExistEqualCard(vector<Card> &cards1, vector<Card> &cards2)
		{
			if(cards1.empty() || cards2.empty()) return false;
			for(int i = 0; i < cards1.size(); i++)
			{
				for(int j = 0; j < cards2.size(); j++)
				{
					if(cards1[i] == cards2[j]) return true;
				}
			}
			return false;
		}
		// cards1 and cards2 is assumed to be ordered
		bool isEqualCards(vector<Card> &cards1, vector<Card> &cards2)
		{
			if(cards1.empty() && cards2.empty()) return true;
			if(cards1.empty()) return true;
			if(cards2.empty()) return true;
			if(cards1.size() != cards2.size()) return false;
			for(int i = 0; i < cards1.size(); i++)
			{
				Card card1 = cards1[i];
				Card card2 = cards2[i];
				if(card1 != card2) return false;
			}
			return true;
		}
		// more_cards have much more cards and contains less_cards
		vector<Card> diffCards(vector<Card> &more_cards, vector<Card> &less_cards)
		{
			vector<Card> diffCards;
			for(int i = 0; i < more_cards.size(); i++)
			{
				bool isexist = false;
				for(int j = 0; j < less_cards.size(); i++)
				{
					if(more_cards[i] == less_cards[j]) 
					{
						isexist = true;
						break;
					}
				}
				if(!isexist) diffCards.push_back(more_cards[i]);
			}
			return diffCards;
		}
		void init()
		{
			image = 0;
			prev_stage = STAGE_UNKNOWN;
			cur_stage = STAGE_UNKNOWN;
			islshowed = UNKNOWN;
			isrshowed = UNKNOWN;

			last3_cards.clear();
			myself_cards.clear();
			lshowed_cards.clear();
			rshowed_cards.clear();
			lplayed_cards.clear();
			iplayed_cards.clear();
			rplayed_cards.clear();

			prev_last3_cards.clear();
			prev_myself_cards.clear();
			prev_lshowed_cards.clear();
			prev_rshowed_cards.clear();
			prev_lplayed_cards.clear();
			prev_iplayed_cards.clear();
			prev_rplayed_cards.clear();

			allplayed_history.clear();
			iplayed_history.clear();
			lplayed_history.clear();
			rplayed_history.clear();
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
			if(isExistUnknownCard(cards)) return;
			for(int i = 0; i < cards.size(); i++)
				if(allplayed_history.find(cards[i]) != allplayed_history.end()) return; // alread add

			history_plays.push_back(cards);
			history_players.push_back(who);
			for(int i = 0; i < cards.size(); i++) allplayed_history.insert(cards[i]);
			if(who == "lplayed") for(int i = 0; i < cards.size(); i++) lplayed_history.insert(cards[i]);
			else if(who == "iplayed") for(int i = 0; i < cards.size(); i++) iplayed_history.insert(cards[i]);
			else if(who == "rplayed") for(int i = 0; i < cards.size(); i++) rplayed_history.insert(cards[i]);
		}
		void remove_from_history(vector<Card> & cards, string who)
		{
			if(cards.empty()) return;
			if(isExistUnknownCard(cards)) return;
			for(int i = 0; i < cards.size(); i++)
			{
				Card card = cards[i];
				set<Card>::iterator it = allplayed_history.find(card);
				if(it == allplayed_history.end())
				{
					cerr<<"can't remove card "<<card.str()<<" from history"<<endl;
				}
				else allplayed_history.erase(it);
			}
			if(isEqualCards(*history_plays.rbegin(), cards))
			{
				history_plays.erase(history_plays.begin() + (history_plays.size() - 1));
				history_players.erase(history_players.begin() + (history_players.size() - 1));
			}
			if(who == "iplayed")
			{
				for(int i = 0; i < cards.size(); i++)
				{
					Card card = cards[i];
					set<Card>::iterator it = iplayed_history.find(card);
					if(it == iplayed_history.end())
					{
						cerr<<"can't remove card "<<card.str()<<" from history"<<endl;
					}
					else iplayed_history.erase(it);
				}
			}
			else if(who == "lplayed")
			{
				for(int i = 0; i < cards.size(); i++)
				{
					Card card = cards[i];
					set<Card>::iterator it = lplayed_history.find(card);
					if(it == lplayed_history.end())
					{
						cerr<<"can't remove card "<<card.str()<<" from history"<<endl;
					}
					else lplayed_history.erase(it);
				}
			}
			else if(who == "rplayed")
			{
				for(int i = 0; i < cards.size(); i++)
				{
					Card card = cards[i];
					set<Card>::iterator it = rplayed_history.find(card);
					if(it == rplayed_history.end())
					{
						cerr<<"can't remove card "<<card.str()<<" from history"<<endl;
					}
					else rplayed_history.erase(it);
				}
			}
		}
		vector<Card> get_hidden_cards()
		{
			set<Card> known_cards = allplayed_history;
			for(int i = 0; i < myself_cards.size(); i++) known_cards.insert(myself_cards[i]);
			if(islshowed == SHOWED) for(int i = 0; i < lshowed_cards.size(); i++) known_cards.insert(lshowed_cards[i]);
			if(isrshowed == SHOWED) for(int i = 0; i < rshowed_cards.size(); i++) known_cards.insert(rshowed_cards[i]);
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
			return hidden_cards;
		}
		void disp_hidden_cards()
		{
			vector<Card> hidden_cards = get_hidden_cards();
			cout<<"**********************************************************************************************************"<<endl;
			vector<vector<Card> > cards_list;
			vector<string> msg_list;
			int i = 0;
			while(i < hidden_cards.size())
			{
				Card card = hidden_cards[i];
				if(card.num == NUM_JOKER)
				{
					vector<Card> cards;
					cards.push_back(card);
					cards_list.push_back(cards);
					msg_list.push_back("");
					i++;
				}
				else
				{
					int j = i+1;
					while(j < hidden_cards.size() && hidden_cards[j].num == card.num) j++;
					ostringstream oss;
					oss<<j-i<<"x";
					vector<Card> cards;
					card.type = TYPE_UNKNOWN;
					cards.push_back(card);
					cards_list.push_back(cards);
					msg_list.push_back(oss.str());
					i = j;
				}
			}
			disp_cards(cards_list, msg_list, hidden_cards.size());
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
				disp_cards( cards_list, msg_list, -1);
				//disp_cards(history_plays[i], history_players[i]);
			}
		}
		void recog_last3()
		{
			assert(image);

			if(cur_stage == STAGE_PLAYING)
			{
				last3_cards = RecogLast("last").recog_cards(image);
				assert(!last3_cards.empty()); 
			}
			else
				last3_cards.clear();
		}
		void recog_myself()
		{
			assert(image);

			if(cur_stage == STAGE_LAND_CHOOSE || cur_stage == STAGE_DOUBLE_SCORE || cur_stage == STAGE_PLAYING)
			{
				myself_cards = RecogInhand("inhand").recog_cards(image);
			}
			else
				myself_cards.clear();
		}
		void recog_iplayed()
		{
			assert(image);
			if(cur_stage == STAGE_PLAYING && 
					!isImagePatchSame(image, "ddz_patch_me_clock.png") &&
					!isImagePatchSame(image, "ddz_patch_me_has_not_play.png") &&
					!isImagePatchSame(image, "ddz_patch_me_liandui.png") &&
					!isImagePatchSame(image, "ddz_patch_me_shunzi.png"))
				iplayed_cards = RecogPlay("play", "myself").recog_cards(image);
			else
				iplayed_cards.clear();
		}
		bool verify_cards()
		{
			/* todo: 将Verification代码放到一起
			 if(!isEqualCards(diffCards(prev_myself_cards, myself_cards), iplayed_cards))
			 {
			 cerr<<"iplayed is not the difference between previous cards and current cards"<<endl;
			 }
			 */

			//todo: 测试打出的牌数目和手牌数目之和是17或者20
			if(cur_stage == STAGE_PLAYING)
			{
				if(!isExistUnknownCard(prev_myself_cards) && !prev_myself_cards.empty() && myself_cards.size() >= prev_myself_cards.size())
					myself_cards = prev_myself_cards;
				else if(!isExistUnknownCard(prev_myself_cards) && unknownCardNum(myself_cards) >= 3)
					myself_cards = prev_myself_cards;

				if(!isEqualCards(prev_lplayed_cards, lplayed_cards) && 
					isExistEqualCard(prev_lplayed_cards, lplayed_cards) &&
					prev_lplayed_cards.size() < lplayed_cards.size())
				{
					remove_from_history(prev_lplayed_cards, "lplayed");
				}

				if(!isEqualCards(prev_iplayed_cards, iplayed_cards) && 
					isExistEqualCard(prev_iplayed_cards, iplayed_cards) &&
					prev_iplayed_cards.size() < iplayed_cards.size())
				{
					remove_from_history(prev_iplayed_cards, "iplayed");
				}

				if(!isEqualCards(prev_rplayed_cards, rplayed_cards) && 
					isExistEqualCard(prev_rplayed_cards, rplayed_cards) &&
					prev_rplayed_cards.size() < rplayed_cards.size())
				{
					remove_from_history(prev_rplayed_cards, "rplayed");
				}
			}
			return true;
		}	
		void recog_lplayed()
		{
			assert(image);
			if(cur_stage == STAGE_PLAYING && !isImagePatchSame(image, "ddz_patch_left_clock.png") && !isImagePatchSame(image, "ddz_patch_left_has_not_play.png") &&
			   !isImagePatchSame(image, "ddz_patch_left_liandui.png") && !isImagePatchSame(image, "ddz_patch_left_shunzi.png"))
				lplayed_cards = RecogPlay("play", "left").recog_cards(image);
			else
				lplayed_cards.clear();
		}
		void recog_rplayed()
		{
			assert(image);
			if(cur_stage == STAGE_PLAYING && !isImagePatchSame(image, "ddz_patch_right_clock.png") && !isImagePatchSame(image, "ddz_patch_right_has_not_play.png") &&
			   !isImagePatchSame(image, "ddz_patch_right_liandui.png") && !isImagePatchSame(image, "ddz_patch_right_shunzi.png"))
				rplayed_cards = RecogPlay("play", "right").recog_cards(image);
			else
				rplayed_cards.clear();
		}
		void recog_lshowed()
		{
			assert(image);
			if(cur_stage == STAGE_PLAYING || cur_stage == STAGE_LAND_CHOOSE || cur_stage == STAGE_DOUBLE_SCORE)
			{
				if(islshowed == UNKNOWN)
				{
					lshowed_cards = RecogMing("showed", "left").recog_cards(image);
					if(lshowed_cards.empty())
					{
						islshowed = UNSHOWED;
					}
					else islshowed = SHOWED;
				}
				else if(islshowed == SHOWED)
				{
					lshowed_cards = RecogMing("showed", "left").recog_cards(image);
					if(lshowed_cards.empty())
					{
						cerr<<"lshowed_cards should not be empty"<<endl;
					}
				}
				else if(isrshowed == SHOWED)
				{
					lshowed_cards = get_hidden_cards();
				}
				else if(cur_stage != prev_stage)
				{
					islshowed = UNKNOWN;
				}
			}
			else
				rshowed_cards.clear();

		}
		void recog_rshowed()
		{
			assert(image);
			if(cur_stage == STAGE_PLAYING || cur_stage == STAGE_LAND_CHOOSE || cur_stage == STAGE_DOUBLE_SCORE)
			{
				if(isrshowed == UNKNOWN)
				{
					rshowed_cards = RecogMing("showed", "right").recog_cards(image);
					if(rshowed_cards.empty())
					{
						isrshowed = UNSHOWED;
					}
					else isrshowed = SHOWED;
				}
				else if(isrshowed == SHOWED)
				{
					rshowed_cards = RecogMing("showed", "right").recog_cards(image);
					if(rshowed_cards.empty())
					{
						cerr<<"rshowed_cards should not be empty"<<endl;
					}
				}
				else if(islshowed == SHOWED)
				{
					rshowed_cards = get_hidden_cards();
				}
				else if(cur_stage != prev_stage)
				{
					isrshowed = UNKNOWN;
				}
			}
			else
				rshowed_cards.clear();
		}
		// this function is invoke in the last step
		void refresh_prev_cards()
		{
			if(cur_stage == STAGE_PLAYING)
			{
				prev_last3_cards = last3_cards;
				prev_iplayed_cards = iplayed_cards;
				prev_lplayed_cards = lplayed_cards;
				prev_rplayed_cards = rplayed_cards;
				prev_myself_cards = myself_cards;
				if(islshowed == SHOWED) prev_lshowed_cards = lshowed_cards;
				if(isrshowed == SHOWED) prev_rplayed_cards = rshowed_cards;
			}
		}
		void process(string file)
		{
			image = cvLoadImage(file.c_str(), 1);
			if(!image) return;
			cur_stage = which_game_stage(image);

			if(!is_valid_stage())
			{
				cerr<<"Invalid stage"<<endl;
				cvReleaseImage(&image);
				prev_stage = cur_stage;
				return;
			}
			string stage_str[] = {"等待游戏", "发牌阶段", "叫地主阶段", "加倍阶段", "游戏进行中", "游戏结束"};
			if(cur_stage == STAGE_GAME_WAITING)
			{
				system("clear");
				cout<<file<<endl;
				cout<<stage_str[cur_stage]<<endl;
				init();
				cvReleaseImage(&image);
				prev_stage = cur_stage;
				return;
			}
			recog_last3(); //if(!last3_cards.empty()) disp_cards(last3_cards, "last3");
			recog_myself();
			recog_lshowed();
			recog_rshowed(); //if(!rshowed_cards.empty()) disp_cards(rshowed_cards, "rshowed");
			recog_lplayed(); //if(!lplayed_cards.empty()) disp_cards(lplayed_cards, "lplayed");
			recog_iplayed(); //if(!iplayed_cards.empty()) disp_cards(iplayed_cards, "iplayed");
			recog_rplayed(); //if(!rplayed_cards.empty()) disp_cards(rplayed_cards, "rplayed");
			verify_cards();
			refresh_prev_cards();
			if(lplayed_cards.empty())
			{
				add_to_history(iplayed_cards, "iplayed");
				add_to_history(rplayed_cards, "rplayed");
			}
			else if(iplayed_cards.empty())
			{
				add_to_history(rplayed_cards, "rplayed");
				add_to_history(lplayed_cards, "lplayed");
			}
			else 
			{
				add_to_history(lplayed_cards, "lplayed");
				add_to_history(iplayed_cards, "iplayed");
			}
			//display
			{
				system("clear");
				cout<<file<<endl;
				cout<<stage_str[cur_stage]<<endl;
				if(!isEqualCards(myself_cards, prev_myself_cards)) cerr<<"myself cards changed"<<endl;
				if(!isEqualCards(lshowed_cards, prev_lshowed_cards)) cerr<<"lshowed cards changed"<<endl;
				if(!isEqualCards(rshowed_cards, prev_rshowed_cards)) cerr<<"rshowed cards changed"<<endl;
				if(!lshowed_cards.empty()) disp_cards(lshowed_cards, "left");
				if(!myself_cards.empty()) disp_cards(myself_cards, "me");
				if(!rshowed_cards.empty()) disp_cards(rshowed_cards, "right");
				if(islshowed != SHOWED && isrshowed != SHOWED) disp_hidden_cards();
				disp_history();
			}

			refresh_prev_cards();

			prev_stage = cur_stage;
			cvReleaseImage(&image);
		}
};

#endif
