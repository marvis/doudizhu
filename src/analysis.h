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

#define DOUT cerr

//扑克类型
#define CT_ERROR                       0                                  //错误类型
#define CT_SINGLE                      1                                  //单牌类型
#define CT_DOUBLE                      2                                  //对牌类型
#define CT_THREE                       3                                  //三条类型
#define CT_THREE_TAKE_ONE              4                                  //三带一
#define CT_THREE_TAKE_TWO              5                                  //三带二
#define CT_SINGLE_LINE                 6                                  //单连类型
#define CT_DOUBLE_LINE                 7                                  //对连类型
#define CT_THREE_LINE                  8                                  //三连类型
#define CT_THREE_LINE_TAKE_ONE         9                                  //三带一单
#define CT_THREE_LINE_TAKE_TWO        10                                  //三带一对
#define CT_FOUR_LINE_TAKE_ONE         11                                  //四带两单
#define CT_FOUR_LINE_TAKE_TWO         12                                  //四带两对
#define CT_BOMB_CARD                  13                                  //炸弹类型
#define CT_MISSILE_CARD               14                                  //火箭类型
#define CT_UNKNOWN                    15                                  //没有计算

enum {ANIMATION_LIANDUI, ANIMATION_SHUNZI, ANIMATION_NONE};

string cards_str(vector<Card> & cards)
{
	string out;
	for(int i = 0; i < cards.size(); i++) out += cards[i].str();
	return out;
}

class PlayAnalysisResult
{
	public:
		int nfour;
		int nthree;
		int ndouble;
		int nsingle;
		int ntotal;

		set<int> fours;
		set<int> threes;
		set<int> doubles;
		set<int> singles;

		PlayAnalysisResult()
		{
			init();
		}
		void init()
		{
			nfour = 0;
			nthree = 0;
			ndouble = 0;
			nsingle = 0;
			ntotal = 0;
			fours.clear();
			threes.clear();
			doubles.clear();
			singles.clear();
		}
};
class OnePlay
{
	public:
		vector<Card> cards;
		string who;
		int card_type;
		PlayAnalysisResult result;

		OnePlay()
		{
			init("");
		}
		OnePlay(vector<Card> & _cards, string _who): cards(_cards), who(_who)
		{
		}

		void init(string _who)
		{
			cards.clear();
			who = _who;
			card_type = CT_UNKNOWN;
			result.init();
		}
		void disp()
		{
			disp_cards(cards, who);
		}
		string str()
		{
			string out;
			for(int i = 0; i < cards.size(); i++) out += cards[i].str();
			return out;
		}

		string type_name()
		{
			assert(card_type != CT_UNKNOWN);
			string typenames[] = {"错误", "单牌", "对牌", "三条",
				"三带一", "三带二", "顺子", "对连", "飞机", "三带一单",
				"三带一对", "四带两单", "四带两对", "炸弹", "火箭"};
			return typenames[card_type];
		}

		bool is_larger_than(OnePlay & other)
		{
			assert(card_type != CT_UNKNOWN && other.card_type != CT_UNKNOWN);
			PlayAnalysisResult result1 = this->result;
			PlayAnalysisResult result2 = other.result;
			int type1 = this->card_type;
			int type2 = other.card_type;

			if(type1 == CT_ERROR && type2 == CT_ERROR) return true;
			if(type1 == CT_ERROR) return false;
			if(type2 == CT_ERROR) return true;

			if(type1 == CT_MISSILE_CARD) return true;
			if(type2 == CT_MISSILE_CARD) return false;
			if(type1 == CT_BOMB_CARD && type2 == CT_BOMB_CARD)
			{
				if((*result1.fours.begin()) > (*result2.fours.begin())) return true;
				else return false;
			}
			else if(type1 == CT_BOMB_CARD && type2 != CT_BOMB_CARD) return true;
			else if(type1 != CT_BOMB_CARD && type2 == CT_BOMB_CARD) return false;
			
			// 都不是炸弹
			if(type1 != type2) return false;

			if(type1 == CT_FOUR_LINE_TAKE_ONE || type1 == CT_FOUR_LINE_TAKE_TWO)
			{
				if((*result1.fours.begin()) > (*result2.fours.begin())) return true;
				else return false;
			}

			if(type1 == CT_THREE_LINE_TAKE_ONE || type1 == CT_THREE_LINE_TAKE_TWO || type1 == CT_THREE_LINE)
			{
				if(result1.threes.size() != result2.threes.size()) return false;
				else if(*result1.threes.begin() > *result2.threes.begin()) return true;
				else return false;
			}
			if(type1 == CT_DOUBLE_LINE)
			{
				if(result1.doubles.size() != result2.doubles.size()) return false;
				else if(*result1.doubles.begin() > *result2.doubles.begin()) return true;
				else return false;
			}
			if(type1 == CT_SINGLE_LINE)
			{
				if(result1.singles.size() != result2.singles.size()) return false;
				else if(*result1.singles.begin() > *result2.singles.begin()) return true;
				else return false;
			}
			if(type1 == CT_THREE_TAKE_TWO || type1 == CT_THREE_TAKE_ONE || type1 == CT_THREE)
			{
				if(*result1.threes.begin() > *result2.threes.begin()) return true;
				else return false;
			}
			if(type1 == CT_DOUBLE)
			{
				if(*result1.doubles.begin() > *result2.doubles.begin()) return true;
				else return false;
			}
			if(type1 == CT_SINGLE)
			{
				if(*result1.singles.begin() > *result2.singles.begin()) return true;
				else return false;
			}
			return true;
		}
		void refresh_card_type()
		{
			card_type = compute_card_type();
		}

	private:
		int compute_card_type()
		{
			if(cards.empty()) return CT_ERROR;
			// besure that NUM_3 is 0
			int ncards[14] = {0};
			result.ntotal = cards.size();
			
			for(int i = 0; i < result.ntotal; i++)
			{
				if(cards[i].num == NUM_UNKNOWN) return CT_ERROR;
				else ncards[cards[i].num]++;
			}

			// 有王的情况
			if(ncards[NUM_JOKER] == 2)
			{
				if(result.ntotal == 2) return CT_MISSILE_CARD;
				else if(result.ntotal > 3) return CT_ERROR;
			}
			else if(ncards[NUM_JOKER] == 1)
			{
				if(result.ntotal == 1) return CT_SINGLE;
				else return CT_ERROR;
			}
			// 没有王的情况
			for(int num = NUM_3; num <= NUM_2; num++)
			{
				if(ncards[num] == 4) 
				{
					result.nfour++;
					result.fours.insert(num);
				}
				else if(ncards[num] == 3)
				{
					result.nthree++;
					result.threes.insert(num);
				}
				else if(ncards[num] == 2)
				{
					result.ndouble++;
					result.doubles.insert(num);
				}
				else if(ncards[num] == 1)
				{
					result.nsingle++;
					result.singles.insert(num);
				}
			}
			// 有炸弹的情况
			if(result.nfour >= 2)
			{
				cerr<<"不允许多个炸弹同时出"<<endl;
				return CT_ERROR;
			}
			else if(result.nfour == 1)
			{
				if(result.ntotal == 4) return CT_BOMB_CARD;
				else if(result.ntotal == 5) return CT_FOUR_LINE_TAKE_ONE;
				else if(result.ntotal == 6) return CT_FOUR_LINE_TAKE_ONE;
				else if(result.ntotal == 7) return CT_ERROR;
				else if(result.ntotal == 8)
				{
					if(result.ndouble == 2) return CT_FOUR_LINE_TAKE_TWO;
					else return CT_ERROR;
				}
				else return CT_ERROR;
			}
			// 无炸弹的情况
			if(result.nthree == 1)
			{
				if(result.ntotal == 3) return CT_THREE;
				else if(result.ntotal == 4) return CT_THREE_TAKE_ONE;
				else if(result.ntotal == 5)
				{
					if(result.ndouble == 1) return CT_THREE_TAKE_TWO;
					else return CT_ERROR;
				}
				else return CT_ERROR;
			}
			// 飞机 todo: 找出最大的连
			else if(result.nthree >= 2)
			{
				if((*result.threes.rbegin()) - (*result.threes.begin()) == result.nthree - 1)
				{
					if(result.ntotal == result.nthree*3 + result.nthree) return CT_THREE_LINE_TAKE_ONE;
					else if(result.ndouble > 0 && result.nsingle > 0) return CT_ERROR;
					else if(result.ndouble > 0) 
					{
						if(result.ndouble == result.nthree) return CT_THREE_LINE_TAKE_TWO;
						else return CT_ERROR;
					}
					else if(result.nsingle > 0)
					{
						if(result.nsingle == result.nthree) return CT_THREE_LINE_TAKE_ONE;
						else return CT_ERROR;
					}
					else return CT_THREE_LINE;
				}
				else
				{
					// 不连续情况, 至少要有4个三
					if(result.nthree < 4) return CT_ERROR;
					// 最多一个不连续, 只能是首或者尾
					// todo: 这种情况假定不会发生
					int nthree_line = result.nthree - 1;
					if(result.ntotal == nthree_line * 3 + nthree_line) return CT_THREE_LINE_TAKE_ONE;
					else return CT_ERROR;
				}
			}

			if(result.ndouble >= 3)
			{
				if((*result.doubles.rbegin()) - (*result.doubles.begin()) == result.ndouble - 1)
				{
					if(result.nsingle == 0) return CT_DOUBLE_LINE;
					else return CT_ERROR;
				}
				else return CT_ERROR;
			}
			else if(result.ndouble == 2) return CT_ERROR;
			else if(result.ndouble == 1)
			{
				if(result.nsingle == 0) return CT_DOUBLE;
				else return CT_ERROR;
			}

			if(result.nsingle >= 5)
			{
				if((*result.singles.rbegin()) - (*result.singles.begin()) == result.nsingle - 1) return CT_SINGLE_LINE;
				else return CT_ERROR;
			}
			else if(result.nsingle > 1) return CT_ERROR;
			else if(result.nsingle == 1) return CT_SINGLE;
			return CT_ERROR;
		}
};
class Analysis
{
	enum {CLOCK_LEFT = 0, CLOCK_ME, CLOCK_RIGHT, CLOCK_UNKNOWN};
	public:
		vector<Card> cur_handcards;
		vector<Card> cur_lshowed_cards;
		vector<Card> cur_rshowed_cards;
		vector<Card> cur_last3_cards;

		vector<Card> prev_handcards;
		vector<Card> prev_lshowed_cards;
		vector<Card> prev_rshowed_cards;
		vector<Card> prev_last3_cards;

		OnePlay cur_lplay;
		OnePlay cur_rplay;
		OnePlay cur_iplay;

		OnePlay prev_lplay;
		OnePlay prev_rplay;
		OnePlay prev_iplay;

		vector<OnePlay> history;

		int last_lplay_id; // To avoid duplicated adding 
		int last_iplay_id; // to history
		int last_rplay_id; // it is the last id in history


		string boss;
		int nlcards;
		int nrcards;

		int prev_stage;
		int cur_stage;

		int cur_clock;
		int prev_clock;

		int animation_me;
		int animation_left;
		int animation_right;
		
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
		bool isExistUnknownNumCard(vector<Card> & cards)
		{
			for(int i = 0; i < cards.size(); i++)
			{
				if(cards[i].num == NUM_UNKNOWN) return true;
			}
			return false;
		}
		int numberOfUnknownTypes(vector<Card> & cards)
		{
			int n = 0;
			for(int i = 0; i < cards.size(); i++)
			{
				if(cards[i].type == TYPE_UNKNOWN) n++;
			}
			return n;
		}
		int numberOfUnknownCards(vector<Card> & cards)
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
		bool isConsistCards(vector<Card> & cards1, vector<Card> & cards2)
		{
			if(isExistEqualCard(cards1, cards2)) return true;
			int card_num1[14] = {0};
			int card_num2[14] = {0};
			for(int i = 0; i < cards1.size(); i++)
			{
				Card & card = cards1[i];
				if(card.num != NUM_UNKNOWN) card_num1[card.num]++;
			}
			for(int i = 0; i < cards2.size(); i++)
			{
				Card & card = cards2[i];
				if(card.num != NUM_UNKNOWN) card_num1[card.num]++;
			}
			for(int i = 0; i < 14; i++)
			{
				if(card_num1[i] != card_num2[i]) return false;
			}
			return true;
		}
		// cards1 and cards2 is assumed to be ordered
		bool isEqualCards(vector<Card> &cards1, vector<Card> &cards2)
		{
			if(cards1.empty() && cards2.empty()) return true;
			if(cards1.empty()) return false;
			if(cards2.empty()) return false;
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
		vector<Card> missingCards(vector<Card> &more_cards, vector<Card> &less_cards)
		{
			assert(more_cards.size() >= less_cards.size());
			vector<Card> diffCards;
			for(int i = 0; i < more_cards.size(); i++)
			{
				bool isexist = false;
				for(int j = 0; j < less_cards.size(); j++)
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

			cur_clock = CLOCK_UNKNOWN;
			prev_clock = CLOCK_UNKNOWN;

			cur_last3_cards.clear();
			cur_handcards.clear();
			cur_lshowed_cards.clear();
			cur_rshowed_cards.clear();
			cur_lplay.init("left");
			cur_iplay.init("me");
			cur_rplay.init("right");

			prev_last3_cards.clear();
			prev_handcards.clear();
			prev_lshowed_cards.clear();
			prev_rshowed_cards.clear();
			prev_lplay.init("left");
			prev_iplay.init("me");
			prev_rplay.init("right");

			history.clear();
			last_lplay_id = -1;
			last_iplay_id = -1;
			last_rplay_id = -1;

			boss = "";
			nlcards = 0;
			nrcards = 0;

			animation_me = ANIMATION_NONE;
			animation_left = ANIMATION_NONE;
			animation_right = ANIMATION_NONE;
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
		void add_to_history(OnePlay & play)
		{
			assert(play.card_type != CT_UNKNOWN);
			if(play.cards.empty()) return;
			if(isExistUnknownNumCard(play.cards)) return;

			if(play.card_type == CT_ERROR)
			{
				cerr<<"不能添加错误的牌"<<endl;
				return;
			}

			if(history.empty())
			{
				history.push_back(play);
				if(play.who == "left") last_lplay_id = history.size() - 1;
				else if(play.who == "me") last_iplay_id = history.size() - 1;
				else if(play.who == "right") last_rplay_id = history.size() - 1;
				return;
			}

			// 如果是重复识别的牌, 更新history记录
			if(play.who == "left" && last_lplay_id >= 0)
			{
				OnePlay & last_play = history[last_lplay_id];
				if(isEqualCards(last_play.cards, play.cards)) return;
				// 如果是重复识别的牌, 更新识别更好的结果
				else if(isConsistCards(last_play.cards, play.cards))
				{
					if(play.cards.size() < last_play.cards.size()) return;
					else if(numberOfUnknownTypes(play.cards) < numberOfUnknownTypes(last_play.cards))
					{
						last_play = play;
						return;
					}
				}
			}
			if(play.who == "me" && last_iplay_id >= 0)
			{
				OnePlay & last_play = history[last_iplay_id];
				if(isEqualCards(last_play.cards, play.cards)) return;
				// 如果是重复识别的牌, 更新识别更好的结果
				else if(isConsistCards(last_play.cards, play.cards))
				{
					if(play.cards.size() < last_play.cards.size()) return;
					else if(numberOfUnknownTypes(play.cards) < numberOfUnknownTypes(last_play.cards))
					{
						last_play = play;
						return;
					}
				}
			}
			if(play.who == "right" && last_rplay_id >= 0)
			{
				OnePlay & last_play = history[last_rplay_id];
				if(isEqualCards(last_play.cards, play.cards)) return;
				else if(isConsistCards(last_play.cards, play.cards))
				{
					if(play.cards.size() < last_play.cards.size()) return;
					// 如果是重复识别的牌, 更新识别更好的结果
					else if(numberOfUnknownTypes(play.cards) < numberOfUnknownTypes(last_play.cards))
					{
						last_play = play;
						return;
					}
				}
			}

			// 如果是新打出的牌, 与上次加入的牌比较大小
			if(1)
			{
				// 比较与上家牌的大小
				OnePlay & last_play = history[history.size() - 1];
				if(play.who == last_play.who || play.is_larger_than(last_play))
				{
					history.push_back(play); 
					if(play.who == "left") last_lplay_id = history.size() - 1;
					else if(play.who == "me") last_iplay_id = history.size() - 1;
					else if(play.who == "right") last_rplay_id = history.size() - 1;
				}
				else
				{
					cerr<<"Can not add play to history:"<<endl;
					cout<<play.who<<"'s play "<<play.str()<<" is not larger than "<<last_play.who<<"'s "<<last_play.str()<<endl;
					play.disp();
					// 注意这里会改变play的内容
					if(play.cards[0].num == NUM_JOKER)
					{
						cerr<<"添加丢失的王"<<endl;
						vector<Card> cards;
						cards.push_back(Card(NUM_JOKER, TYPE_RED_JOKER));
						cards.push_back(Card(NUM_JOKER, TYPE_BLACK_JOKER));
						play.cards = cards;
						play.refresh_card_type();

						history.push_back(play); 
						if(play.who == "left") last_lplay_id = history.size() - 1;
						else if(play.who == "me") last_iplay_id = history.size() - 1;
						else if(play.who == "right") last_rplay_id = history.size() - 1;
					}
				}
			}
		}

		vector<Card> get_hidden_cards()
		{
			int card_num[14] = {0};
			vector<set<int> > card_types(14, set<int>());
			bool is_unknown_type[14] = {false};
			for(int j = 0; j < history.size(); j++)
			{
				OnePlay & play = history[j];
				vector<Card> & cards = play.cards;
				for(int i = 0; i < cards.size(); i++)
				{
					if(cards[i].num == NUM_UNKNOWN) continue;
					int num = cards[i].num;
					int type = cards[i].type;
					card_num[num]++;
					card_types[num].insert(type);
					if(type == TYPE_UNKNOWN) is_unknown_type[num] = true;
				}
			}
			for(int i = 0; i < cur_handcards.size(); i++)
			{
				if(cur_handcards[i].num == NUM_UNKNOWN) continue;
				int num = cur_handcards[i].num;
				int type = cur_handcards[i].type;
				card_num[num]++;
				card_types[num].insert(type);
				if(type == TYPE_UNKNOWN) is_unknown_type[num] = true;
			}
			for(int i = 0; i < cur_lshowed_cards.size(); i++)
			{
				if(cur_lshowed_cards[i].num == NUM_UNKNOWN) continue;
				int num = cur_lshowed_cards[i].num;
				int type = cur_lshowed_cards[i].type;
				card_num[num]++;
				card_types[num].insert(type);
				if(type == TYPE_UNKNOWN) is_unknown_type[num] = true;
			}
			for(int i = 0; i < cur_rshowed_cards.size(); i++)
			{
				if(cur_rshowed_cards[i].num == NUM_UNKNOWN) continue;
				int num = cur_rshowed_cards[i].num;
				int type = cur_rshowed_cards[i].type;
				card_num[num]++;
				card_types[num].insert(type);
				if(type == TYPE_UNKNOWN) is_unknown_type[num] = true;
			}

			vector<Card> hidden_cards;
			if(card_num[13] == 0)
			{
				hidden_cards.push_back(Card(NUM_JOKER, TYPE_RED_JOKER));
				hidden_cards.push_back(Card(NUM_JOKER, TYPE_BLACK_JOKER));
			}
			else if(card_num[13] == 1)
			{
				int exist_type = *card_types[13].begin();
				if(exist_type == TYPE_BLACK_JOKER) hidden_cards.push_back(Card(NUM_JOKER, TYPE_RED_JOKER));
				else hidden_cards.push_back(Card(NUM_JOKER, TYPE_BLACK_JOKER));
			}
			for(int i = 12; i >= 0; i--)
			{
				if(card_num[i] == 4) continue;
				if(is_unknown_type[i])
				{
					int n = 4 - card_num[i];
					for(int j = 0; j < n; j++) hidden_cards.push_back(Card(i, TYPE_UNKNOWN));
				}
				else
				{
					for(int type = TYPE_SPADE; type <= TYPE_DIAMOND; type++)
					{
						if(card_types[i].find(type) == card_types[i].end()) hidden_cards.push_back(Card(i, type));
					}
				}
			}
			
			return hidden_cards;
		}
		void disp_hidden_cards()
		{
			vector<Card> hidden_cards = get_hidden_cards();
			if(!cur_lshowed_cards.empty()) disp_cards(hidden_cards, "right");
			else if(!cur_rshowed_cards.empty()) disp_cards(hidden_cards, "left");
			else
			{
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
		}
		void disp_history()
		{
			if(!history.empty()) cout<<"**********************************************************************************************************"<<endl;
			int i = 0;
			while(i < history.size())
			{
				vector<vector<Card> > cards_list;
				vector<string> msg_list;
				int ncards = 0;
				while(ncards < 14 && i < history.size())
				{
					vector<Card> & cards = history[i].cards;
					string & msg = history[i].who;
					cards_list.push_back(cards);
					msg_list.push_back(msg);
					ncards += cards.size();
					i++;
				}
				disp_cards( cards_list, msg_list, -1);
			}
		}
		void recog_last3()
		{
			assert(image);

			if(cur_stage == STAGE_PLAYING)
			{
				cur_last3_cards = RecogLast("last").recog_cards(image);
				assert(!cur_last3_cards.empty()); 
			}
			else
				cur_last3_cards.clear();
		}
		void recog_handcards()
		{
			assert(image);

			if(cur_stage == STAGE_LAND_CHOOSE || cur_stage == STAGE_DOUBLE_SCORE || cur_stage == STAGE_PLAYING)
			{
				cur_handcards = RecogInhand("inhand").recog_cards(image);
			}
			else
				cur_handcards.clear();
		}
		void recog_iplay()
		{
			assert(image);
			if(cur_stage == STAGE_PLAYING)
			{
				if(isImagePatchSame(image, "ddz_patch_me_clock.png"))
				{
					cur_clock = CLOCK_ME;
					cur_iplay.cards.clear();
				}
				else if(isImagePatchSame(image, "ddz_patch_me_has_not_play.png"))
					cur_iplay.cards.clear();
				else if(isImagePatchSame(image, "ddz_patch_me_liandui.png"))
				{
					animation_me = ANIMATION_LIANDUI;
					cur_iplay.cards.clear();// = RecogPlay("play", "me").recog_cards_liandui(image);
				}
				else if(isImagePatchSame(image, "ddz_patch_me_shunzi.png"))
				{
					animation_me = ANIMATION_SHUNZI;
					cur_iplay.cards.clear();// = RecogPlay("play", "me").recog_cards_shunzi(image);
				}
				else
					cur_iplay.cards = RecogPlay("play", "me").recog_cards(image);
			}
			else
				cur_iplay.cards.clear();
		}
		void recog_lplay()
		{
			assert(image);
			if(cur_stage == STAGE_PLAYING)
			{
				if(isImagePatchSame(image, "ddz_patch_left_clock.png"))
				{
					cur_clock = CLOCK_LEFT;
					cur_lplay.cards.clear();
				}
				else if(isImagePatchSame(image, "ddz_patch_left_has_not_play.png"))
					cur_lplay.cards.clear();
				else if(isImagePatchSame(image, "ddz_patch_left_liandui.png"))
				{
					animation_left = ANIMATION_LIANDUI;
					cur_lplay.cards = RecogPlay("play", "left").recog_cards_liandui(image);
				}
				else if(isImagePatchSame(image, "ddz_patch_left_shunzi.png"))
				{
					animation_left = ANIMATION_SHUNZI;
					cur_lplay.cards = RecogPlay("play", "left").recog_cards_shunzi(image);
				}
				else
					cur_lplay.cards = RecogPlay("play", "left").recog_cards(image);
			}
			else
				cur_lplay.cards.clear();
		}
		void recog_rplay()
		{
			assert(image);
			if(cur_stage == STAGE_PLAYING)
			{
				if(isImagePatchSame(image, "ddz_patch_right_clock.png"))
				{
					cur_clock = CLOCK_RIGHT;
					cur_rplay.cards.clear();
				}
				else if(isImagePatchSame(image, "ddz_patch_right_has_not_play.png"))
					cur_rplay.cards.clear();
				else if(isImagePatchSame(image, "ddz_patch_right_liandui.png"))
				{
					animation_right = ANIMATION_LIANDUI;
					cur_rplay.cards = RecogPlay("play", "right").recog_cards_liandui(image);
				}
				else if(isImagePatchSame(image, "ddz_patch_right_shunzi.png"))
				{
					animation_right = ANIMATION_SHUNZI;
					cur_rplay.cards = RecogPlay("play", "right").recog_cards_shunzi(image);
				}
				else
					cur_rplay.cards = RecogPlay("play", "right").recog_cards(image);
			}
			else
				cur_rplay.cards.clear();
		}
		void verify_cards()
		{
			if(cur_stage == STAGE_PLAYING)
			{
				// 根据prev_handcards, cur_handcards 以及 cur_iplay 相互之间进行验证
				// 前提得是prev_handcards没有错误
				if(!isExistUnknownCard(prev_handcards))
				{
					if(!isExistUnknownCard(cur_handcards))
					{
						if(prev_handcards.size() > cur_handcards.size())
						{
							if(animation_me != ANIMATION_NONE)
							{
								cerr<<"refresh iplay by cur_handcards "<<cards_str(cur_handcards)<<" and prev_handcards"<<cards_str(prev_handcards)<<endl;
								cur_iplay.cards = missingCards(prev_handcards, cur_handcards);
								cur_iplay.refresh_card_type();
							}
						}
						else if(isEqualCards(prev_handcards, cur_handcards))
						{
							if(animation_me != ANIMATION_NONE)
							{
								if(prev_iplay.card_type != CT_ERROR)
								{
									cerr<<"As handcars doesn't change, and there is move, set cur_iplay to prev_iplay"<<endl;
									cur_iplay = prev_iplay;
								}
							}
						}
					}
					else if(cur_iplay.card_type != CT_ERROR)
					{
						//if(animation_me == NONE)
						{
							cerr<<"refresh cur_handcards by prev_handcards and cur_iplay"<<endl;
							cur_handcards = missingCards(prev_handcards, cur_iplay.cards);
						}
					}
					else if(numberOfUnknownCards(cur_handcards) >= 3)
						cur_handcards = prev_handcards;

				}

			}
		}
		void recog_lshowed()
		{
			assert(image);
			if(cur_stage == STAGE_PLAYING || cur_stage == STAGE_LAND_CHOOSE || cur_stage == STAGE_DOUBLE_SCORE)
				cur_lshowed_cards = RecogMing("showed", "left").recog_cards(image);
			else
				cur_lshowed_cards.clear();

		}
		void recog_rshowed()
		{
			assert(image);
			if(cur_stage == STAGE_PLAYING || cur_stage == STAGE_LAND_CHOOSE || cur_stage == STAGE_DOUBLE_SCORE)
				cur_rshowed_cards = RecogMing("showed", "right").recog_cards(image);
			else
				cur_rshowed_cards.clear();
		}
		void recog_cardnum()
		{
			assert(image);
			if(cur_stage == STAGE_PLAYING || cur_stage == STAGE_LAND_CHOOSE || cur_stage == STAGE_DOUBLE_SCORE)
			{
				if(isImagePatchSame(image, "ddz_patch_cardnum_left_l2.png", 40)) nlcards = 20;
				else if(isImagePatchSame(image, "ddz_patch_cardnum_left_l1.png", 40))
				{
					if(isImagePatchSame(image, "ddz_patch_cardnum_left_r0.png")) nlcards = 10;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_left_r1.png")) nlcards = 11;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_left_r2.png")) nlcards = 12;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_left_r3.png")) nlcards = 13;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_left_r4.png")) nlcards = 14;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_left_r5.png")) nlcards = 15;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_left_r6.png")) nlcards = 16;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_left_r7.png")) nlcards = 17;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_left_r8.png")) nlcards = 18;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_left_r9.png")) nlcards = 19;
					else 
					{
						cerr<<"unknown cards number for left"<<endl;
					}
				}
				else if(isImagePatchSame(image, "ddz_patch_cardnum_left_l0.png", 40))
				{
					if(isImagePatchSame(image, "ddz_patch_cardnum_left_m1.png")) nlcards = 1;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_left_m2.png")) nlcards = 2;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_left_m3.png")) nlcards = 3;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_left_m4.png")) nlcards = 4;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_left_m5.png")) nlcards = 5;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_left_m6.png")) nlcards = 6;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_left_m7.png")) nlcards = 7;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_left_m8.png")) nlcards = 8;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_left_m9.png")) nlcards = 9;
					else
					{
						cerr<<"unknown cards number for left"<<endl;
					}
				}

				if(isImagePatchSame(image, "ddz_patch_cardnum_right_l2.png", 60)) nrcards = 20;
				else if(isImagePatchSame(image, "ddz_patch_cardnum_right_l1.png", 60))
				{
					if(isImagePatchSame(image, "ddz_patch_cardnum_right_r0.png")) nrcards = 10;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_right_r1.png")) nrcards = 11;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_right_r2.png")) nrcards = 12;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_right_r3.png")) nrcards = 13;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_right_r4.png")) nrcards = 14;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_right_r5.png")) nrcards = 15;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_right_r6.png")) nrcards = 16;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_right_r7.png")) nrcards = 17;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_right_r8.png")) nrcards = 18;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_right_r9.png")) nrcards = 19;
					else 
					{
						cerr<<"unknown cards number for right"<<endl;
					}
				}
				else if(isImagePatchSame(image, "ddz_patch_cardnum_right_l0.png", 60))
				{
					if(isImagePatchSame(image, "ddz_patch_cardnum_right_m1.png")) nrcards = 1;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_right_m2.png")) nrcards = 2;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_right_m3.png")) nrcards = 3;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_right_m4.png")) nrcards = 4;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_right_m5.png")) nrcards = 5;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_right_m6.png")) nrcards = 6;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_right_m7.png")) nrcards = 7;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_right_m8.png")) nrcards = 8;
					else if(isImagePatchSame(image, "ddz_patch_cardnum_right_m9.png")) nrcards = 9;
					else
					{
						cerr<<"unknown cards number for right"<<endl;
					}
				}

			}
		}
		void recog_boss()
		{
			assert(image);
			if(cur_stage == STAGE_PLAYING)
			{
				if(isImagePatchSame(image, "ddz_patch_boss_left1.png") || isImagePatchSame(image, "ddz_patch_boss_left2.png")) boss = "left";
				else if(isImagePatchSame(image, "ddz_patch_boss_right1.png") || isImagePatchSame(image, "ddz_patch_boss_right2.png")) boss = "right";
				else boss = "me";
			}
		}
		// this function is invoke in the last step
		void refresh_states()
		{
			//if(cur_stage == STAGE_PLAYING)
			{
				prev_last3_cards = cur_last3_cards;
				
				prev_iplay = cur_iplay; cur_iplay.init("me");
				prev_lplay = cur_lplay; cur_lplay.init("left");
				prev_rplay = cur_rplay;  cur_rplay.init("right");

				prev_handcards = cur_handcards;
				prev_lshowed_cards = cur_lshowed_cards;
				prev_rshowed_cards = cur_rshowed_cards;

				prev_clock = cur_clock; 
				// 这里要将时钟初始化
				cur_clock= CLOCK_UNKNOWN; 
				
				prev_stage = cur_stage;

				animation_me = ANIMATION_NONE;
				animation_left = ANIMATION_NONE;
				animation_right = ANIMATION_NONE;
			}
		}
		void process(string file)
		{
			image = cvLoadImage(file.c_str(), 1);
			if(!image) return;
			cur_stage = which_game_stage(image);
			string stage_str[] = {"等待游戏", "发牌阶段", "叫地主阶段", "加倍阶段", "游戏进行中", "未知阶段"};
			if(cur_stage == STAGE_UNKNOWN)
			{
				//system("clear");
				cout<<file<<endl;
				cout<<stage_str[cur_stage]<<endl;
				cvReleaseImage(&image);
				prev_stage = cur_stage;
				return;
			}
			
			if(cur_stage == STAGE_GAME_WAITING)
			{
				//system("clear");
				cout<<file<<endl;
				cout<<stage_str[cur_stage]<<endl;
				init();
				cvReleaseImage(&image);
				prev_stage = cur_stage;
				return;
			}
			recog_boss();
			recog_cardnum();
			recog_last3();
			recog_handcards();
			recog_lshowed();
			recog_rshowed();

			recog_lplay(); cur_lplay.refresh_card_type();
			recog_iplay(); cur_iplay.refresh_card_type();
			recog_rplay(); cur_rplay.refresh_card_type();

			verify_cards();

			// 考虑没有时钟的情况
			if(cur_clock == CLOCK_UNKNOWN && prev_clock != CLOCK_UNKNOWN)
				cur_clock = (prev_clock + 1) % 3;

			if(cur_clock == CLOCK_LEFT)
			{
				add_to_history(cur_iplay);
				add_to_history(cur_rplay);
			}
			else if(cur_clock == CLOCK_ME)
			{
				add_to_history(cur_rplay);
				add_to_history(cur_lplay);
			}
			else if(cur_clock == CLOCK_RIGHT) 
			{
				add_to_history(cur_lplay);
				add_to_history(cur_iplay);
			}
			//display
			{
				//system("clear");
				cout<<file<<endl;
				cout<<stage_str[cur_stage]<<endl;
				cout<<boss<<" is boss, nlcards = "<<nlcards<<", nrcards = "<<nrcards<<endl;
	
				if(!cur_lshowed_cards.empty()) disp_cards(cur_lshowed_cards, "left");
				if(!cur_handcards.empty()) disp_cards(cur_handcards, "me");
				if(!cur_rshowed_cards.empty()) disp_cards(cur_rshowed_cards, "right");
				if(cur_lshowed_cards.empty() || cur_rshowed_cards.empty()) disp_hidden_cards();
				disp_history();

				cout<<"**********************************************************************************************************"<<endl;
				if(!cur_lplay.cards.empty())
				{
					disp_cards(cur_lplay.cards, cur_lplay.who + " " + cur_lplay.type_name());
				}
				if(!cur_iplay.cards.empty())
				{
					disp_cards(cur_iplay.cards, cur_iplay.who + " " + cur_iplay.type_name());
				}
				if(!cur_rplay.cards.empty())
				{
					disp_cards(cur_rplay.cards, cur_rplay.who + " " + cur_rplay.type_name());
				}
			}

			refresh_states();

			cvReleaseImage(&image);
		}
};

#endif
