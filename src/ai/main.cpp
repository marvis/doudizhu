#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "unistd.h"
#include "GameLogic.h"
#define BOOL bool
#define RED_T(x) ("\033[1;31m" + string(x) + "\033[0m")
#define BLK_T(x) ("\033[1;30m" + string(x) + "\033[0m")

using namespace std;

string num2str(int num)
{
	ostringstream oss;
	oss<<num;
	return oss.str();
}

void GetInitCards(BYTE player1[MAX_COUNT], int n1, BYTE player2[MAX_COUNT], int n2, BYTE player3[MAX_COUNT], int n3)
{
	BYTE cbFullData[FULL_COUNT]=
	{
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
		0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K
		0x4E,0x4F,
	}; 
	vector<BYTE> curdata(cbFullData, cbFullData + FULL_COUNT);
	for(int i = 0; i < n1; i++)
	{
		int id = rand() % curdata.size();
		player1[i] = curdata[id];
		curdata.erase(curdata.begin() + id);
	}
	for(int i = 0; i < n2; i++)
	{
		int id = rand() % curdata.size();
		player2[i] = curdata[id];
		curdata.erase(curdata.begin() + id);
	}
	for(int i = 0; i < n3; i++)
	{
		int id = rand() % curdata.size();
		player3[i] = curdata[id];
		curdata.erase(curdata.begin() + id);
	}
}

vector<string> disp_str(BYTE card)
{
	string line1 = " -----  ";
	string line2 = "|     | ";
	string line3 = "|     | ";
	string line4 = "|     | ";
	string line5 = " -----  ";
	vector<string> matrix;
	BYTE num = card & MASK_VALUE;
	BYTE type = card & MASK_COLOR;

	string num_str = "     ", type_str = " ";
	if(num == 0x03)      num_str = " 3   ";
	else if(num == 0x04) num_str = " 4   ";
	else if(num == 0x05) num_str = " 5   ";
	else if(num == 0x06) num_str = " 6   ";
	else if(num == 0x07) num_str = " 7   ";
	else if(num == 0x08) num_str = " 8   ";
	else if(num == 0x09) num_str = " 9   ";
	else if(num == 0x0A) num_str = " 10  ";
	else if(num == 0x0B) num_str = " J   ";
	else if(num == 0x0C) num_str = " Q   ";
	else if(num == 0x0D) num_str = " K   ";
	else if(num == 0x01) num_str = " A   ";
	else if(num == 0x02) num_str = " 2   ";
	else if(num == 0x0E) num_str = "JOKER";
	else if(num == 0x0F) num_str = RED_T("JOKER");
	else num_str =" ?   ";

	if(type == 0x30) type_str = "♠";
	else if(type == 0x20) 
	{
		type_str = RED_T("♥");
		num_str = RED_T(num_str);
	}
	else if(type == 0x10) type_str = "♣";
	else if(type == 0x00) 
	{
		type_str = RED_T("♦");
		num_str = RED_T(num_str);
	}
	else type_str = " ";
	line2 = "|" + num_str + "| ";
	line3 = "| " + type_str + "   | ";
	matrix.push_back(line1);
	matrix.push_back(line2);
	matrix.push_back(line3);
	matrix.push_back(line4);
	matrix.push_back(line5);
	return matrix;
}
void disp_cards(BYTE cbCardData[], BYTE cbCardCount, string msg = "")
{
	string line1 = "";
	string line2 = "";
	string line3 = "";
	string line4 = "";
	string line5 = "";
	for(int i = 0; i < cbCardCount; i++)
	{
		vector<string> matrix = disp_str(cbCardData[i]);
		line1 += matrix[0];
		line2 += matrix[1];
		line3 += matrix[2];
		line4 += matrix[3];
		line5 += matrix[4];
	}
	if(msg != "")
	{
		if(line1.size() <= msg.size()) line1 = msg;
		else for(int i = 0; i < msg.size(); i++) line1[i] = msg[i];
	}
	int num = cbCardCount;
	line5 = line5 + num2str(num) + "x";
	cout<<line1<<endl;
	cout<<line2<<endl;
	cout<<line3<<endl;
	cout<<line4<<endl;
	cout<<line5<<endl;

}

enum {PLAYER_LEFT = 0, PLAYER_ME, PLAYER_RIGHT};

int main(int argc, char ** argv)
{
	srand(time(NULL));
	BYTE player_me[MAX_COUNT];
	BYTE player_left[MAX_COUNT];
	BYTE player_right[MAX_COUNT];
	int ncards_me = NORMAL_COUNT;
	int ncards_left = NORMAL_COUNT;
	int ncards_right = NORMAL_COUNT;
	int boss = rand() % 3;
	if(boss = PLAYER_ME) ncards_me = MAX_COUNT;
	else if(boss = PLAYER_LEFT) ncards_left = MAX_COUNT;
	else if(boss = PLAYER_RIGHT) ncards_right = MAX_COUNT;

	GetInitCards(player_me, ncards_me, player_left, ncards_left, player_right, ncards_right);

	string whoami[3] = {"left", "me", "right"};
	whoami[boss] = whoami[boss] + "(boss)";

	CGameLogic game;
	game.SetUserCard(PLAYER_ME, player_me, ncards_me);
	game.SetUserCard(PLAYER_LEFT, player_left, ncards_left);
	game.SetUserCard(PLAYER_RIGHT, player_right, ncards_right);
	game.SetBanker(boss);

	cout<<"**************************************************************************************"<<endl;
	for(int i = 0; i < 3; i++) disp_cards(game.m_cbAllCardData[i], game.m_cbUserCardCount[i], whoami[i]);

	tagOutCardResult out;
	BYTE cbTurnCardData[MAX_COUNT];
	BYTE cbTurnCardCount;

	game.BankerOutCard(game.m_cbAllCardData[boss], game.m_cbUserCardCount[boss], out);
	CopyMemory(cbTurnCardData, out.cbResultCard, out.cbCardCount);
	cbTurnCardCount = out.cbCardCount;

	cout<<whoami[boss]<<" played:"<<endl;
	disp_cards(cbTurnCardData, cbTurnCardCount);
	game.RemoveUserCardData(boss, cbTurnCardData, cbTurnCardCount);
	cout<<"**************************************************************************************"<<endl;
	for(int i = 0; i < 3; i++) disp_cards(game.m_cbAllCardData[i], game.m_cbUserCardCount[i], whoami[i]);
	
	int wOutCardUser = boss;
	int wMeChairID = (boss + 1) % 3;
	//for(int j = 0; j < 2; j++)
	while(1)
	{
		if(game.m_cbUserCardCount[0] == 0) break;
		else if(game.m_cbUserCardCount[1] == 0) break;
		else if(game.m_cbUserCardCount[2] == 0) break;

		if(wMeChairID == wOutCardUser) cbTurnCardCount = 0;

		game.SearchOutCard(game.m_cbAllCardData[wMeChairID], game.m_cbUserCardCount[wMeChairID], cbTurnCardData, cbTurnCardCount, wOutCardUser, wMeChairID, out);
		if(out.cbCardCount == 0)
		{
			cout<<whoami[wMeChairID]<<" doesn't play"<<endl;
			wMeChairID = (wMeChairID + 1) % 3;
		}
		else
		{
			CopyMemory(cbTurnCardData, out.cbResultCard, out.cbCardCount);
			cbTurnCardCount = out.cbCardCount;

			cout<<whoami[wMeChairID]<<" played:"<<endl;
			disp_cards(cbTurnCardData, cbTurnCardCount);
			game.RemoveUserCardData(wMeChairID, cbTurnCardData, cbTurnCardCount);
			cout<<"**************************************************************************************"<<endl;
			for(int i = 0; i < 3; i++) disp_cards(game.m_cbAllCardData[i], game.m_cbUserCardCount[i], whoami[i]);

			wOutCardUser = wMeChairID;
			wMeChairID = (wMeChairID + 1)%3;
		}
		sleep(1);
	}
	return 0;
}
