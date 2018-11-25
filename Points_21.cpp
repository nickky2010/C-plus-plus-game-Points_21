//                          ����. �������� - ��������������� ���������������� �� C++.
//                                          ���� "��������"(21 ����)
#include <iostream>
#include <windows.h>
#include <time.h>
#include "io.h"
#include "fcntl.h"
using namespace std;
class Points_21
{
private:
	char �ard_deck[4][13] = { { '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A' },
	                          { '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A' },
	                          { '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A' },
	                          { '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A' } };
	char �ard_deck_tmp[4][13];
	wchar_t suit; char card;              // suit - �����, card - �����
	int max_score_bank;           // ����������� ��� ������� (����������) - ��������
	wchar_t hand_bank[18];           // �� ����� � ������� (�������� ��� ������� ��������: 4 ���� �� 2 + 4 ���� �� 3 + 1 ��� �� 4 = 9*2 - �� �����)
	wchar_t hand_gamer[18];          // �� ����� � ������ (�������� ��� ������� ��������: 4 ���� �� 2 + 4 ���� �� 3 + 1 ��� �� 4 = 9*2 - �� �����)
	int move_bank=0;
	int move_gamer = 0;
	int tmp_score_bank = 0;
	int tmp_score_gamer = 0;
	int tmp_bet = 0;
	bool win_bank = false;
	bool win_gamer = false;
public:
	static double score;                // ������� ����� �� ������
	static int game;                 // ������� ���������� ���
	static int win;                  // ������� ���������� �����
	static double win_rate;          // ������� % �����
	Points_21()                      // ����������� �� ���������
	{
	}
	~Points_21()                     // ����������
	{
	}
	int init_Rand(int max, int min)                // ������������� ��������� ����� 
	{
		int number = rand() % (max - min + 1) + min;
		return number;
	}
	void choice_card()      // �������� ��������� �����
	{	
		int i, j;
		do{
			card = �ard_deck_tmp[i=init_Rand(3, 0)][j=init_Rand(12, 0)];  // ���� 
		} while (card == ' ');                                            // ���� �� ������� ��������
		// ������������� �����
		if (i == 0) { suit = L'\u2665\ '; } // �����
		if (i == 1) { suit = L'\u2666\ '; } // �����
		if (i == 2) { suit = L'\u2663\ '; } // �����
		if (i == 3) { suit = L'\u2660\ '; } // ����
		�ard_deck_tmp[i][j] = ' ';                                        // ������� �� ������
	}
	void set_new_�ard_deck_tmp()                   // �������������� ����� ��������� (�������) ������ ����
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 13; j++) {
				�ard_deck_tmp[i][j] = �ard_deck[i][j];
			}
		}
	}
	void set_score(int move, wchar_t hand[], int &tmp_score)
	{
		int tmp;
		if (hand[move] == '2') tmp = 2;
		if (hand[move] == '3') tmp = 3;
		if (hand[move] == '4') tmp = 4;
		if (hand[move] == '5') tmp = 5;
		if (hand[move] == '6') tmp = 6;
		if (hand[move] == '7') tmp = 7;
		if (hand[move] == '8') tmp = 8;
		if (hand[move] == '9') tmp = 9;
		if (hand[move] == 'T' || hand[move] == 'J' || hand[move] == 'Q' || hand[move] == 'K') tmp = 10;
		if (hand[move] == 'A') tmp = 11;
		if (move == 4) {
			if (hand[2] == 'A' && hand[4] == 'A') tmp_score = 21;
			else tmp_score += tmp;
		}
		else tmp_score += tmp;
	}
	void set_hand_bank()
	{
		hand_bank[move_bank] = suit;
		hand_bank[++move_bank] = card;
		set_score(move_bank, hand_bank, tmp_score_bank);
		++move_bank;
	}
	void set_hand_gamer()
	{
		hand_gamer[move_gamer] = suit;
		hand_gamer[++move_gamer] = card;
		set_score(move_gamer, hand_gamer, tmp_score_gamer);
		++move_gamer;
	}
	void show_hand_gamer()
	{
		for (int i = 0; i < move_gamer; i++) {
			if (i & 1) {
				if (hand_gamer[i] == 'T') std::wcout << L" " << "10" << endl;
				else std::wcout << L" " << hand_gamer[i] << endl;
			}
			else 
				std::wcout << L"" << hand_gamer[i];
		}
	}
	void show_hand_bank()
	{
		for (int i = 0; i < move_bank; i++) {
			if (i & 1) {
				if (hand_bank[i] == 'T') std::wcout << L" " << "10" << endl;
				else std::wcout << L" " << hand_bank[i] << endl;
			}
			else
				std::wcout << L"" << hand_bank[i];
		}
	}
	void new_bank_card(bool &new_bank_card, bool new_gamer_card) {
		if (new_gamer_card == false && tmp_score_bank < tmp_score_gamer) new_bank_card = true;
		else {
			if (game > 10 && win_rate > 50) max_score_bank = init_Rand(17, 16);            // ��� �������� ����� ����������� ������� ��� �������� ������
			else if (game > 10 && win_rate <= 50) max_score_bank = init_Rand(19, 16);      // ��� �������� ����� ��������� ������� ��� ��������� ������
			else if (game < 10) max_score_bank = 17;
			if (tmp_score_bank < max_score_bank) new_bank_card = true;
			else new_bank_card = false;
		}
	}
	void new_game(bool &new_bank_card, bool &new_gamer_card) {
		new_bank_card = true;
		new_gamer_card = true;
		win_bank = false;
		win_gamer = false;
		set_new_�ard_deck_tmp();
		move_bank = 0;
		move_gamer = 0;
		tmp_score_bank = 0;
		tmp_score_gamer = 0;
		tmp_bet = 0;
		for (int i = 0; i < 18; i++) {
			hand_bank[i] = ' ';
			hand_gamer[i] = ' ';
		}
	}
	void set_win() {
		if (tmp_score_bank<=21 && tmp_score_gamer<=21 && tmp_score_bank >= tmp_score_gamer) win_bank = true;
		else if (tmp_score_bank<21 && tmp_score_gamer<=21 && tmp_score_bank < tmp_score_gamer) win_gamer = true;
		else if (tmp_score_bank<=21 && tmp_score_gamer>21) win_bank = true;
		else if (tmp_score_bank>21 && tmp_score_gamer<=21) win_gamer = true;
	}
	void init_tmp_bet() {
		cin >> tmp_bet;
		while (tmp_bet<1 || tmp_bet>score) {
			std::wcout << L"������ �� ����� ���� ������ 1 ��� ������ ����� ����� �����!!!" << endl;
			std::wcout << L"�������� ���� ������: ";
			cin >> tmp_bet;
		}
	}
	bool get_win_bank() {
		return win_bank;
	}
	bool get_win_gamer() {
		return win_gamer;
	}
	int get_tmp_score_bank() {
		return tmp_score_bank;
	}
	int get_tmp_score_gamer() {
		return tmp_score_gamer;
	}
	int get_tmp_bet() {
		return tmp_bet;
	}
};
double Points_21::score = 100;              // ������� ����� �� ������            
int Points_21::game = 0;                 // ������� ���������� ���
int Points_21::win = 0;                  // ������� ���������� �����
double Points_21::win_rate;              // ������� % �����
void quit() {                            // ������� ������
	std::wcout << L"���������� ������!\n";
	system("pause");
	exit(0);
}
void choice(bool &v) {
	do {
		std::wcout << L"1 - ��\t0 - ���\n";
		std::wcout << L"��� �����: ";
		cin >> v;
		if (v != 0 && v != 1)
			std::wcout << L"\n������! �������� ����! ������� 0 ��� 1!\n";
	} while (v != 0 && v != 1);
}
int main() {
	/*setlocale(LC_ALL, "rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);*/
	_setmode(_fileno(stdout), _O_U16TEXT);
	srand((unsigned int)time(0));
	Points_21 points_21;
	bool v = true; 
	bool new_bank_card = true; 
	bool new_gamer_card = true;
	std::wcout << L"\t\t\t���� ""��������""\n";
	std::wcout << L"\t\t\t������� ����:\n� ���� ������������ ���� ������ ��������� �� 52 ����.\n";
	std::wcout << L"�������� ����� ������ �����: �� ������ �� ������� � �������������� �� 2 �� 10.\n";
	std::wcout << L"�����, ���� � ������ - 10 �����, � ��� - 11 �����.\n";
	std::wcout << L"���� � ������ ����� ����� ������� ��������� 21 ���� (�� ���� � ������ ��� � 10 ��� ��� ����),\n";
	std::wcout << L"�� ����� �������� � ���������� ����-����. � ����� ������ ������ ����� ������������� ������� 3 � 2 \n";
	std::wcout << L"(�� ���� � 1,5 ���� ����������� ��� ������).\n";
	std::wcout << L"���������, ����� ��� � �����, ��� �������� ������� ������ ���������, � ��� ��������� - ����� ���������.\n\n";
	system("pause");
	while (v) {
		system("cls");
		if (points_21.game>0) points_21.win_rate =  ((double)points_21.win / points_21.game)*100.;
		else points_21.win_rate = 0;
		++points_21.game;
		points_21.new_game(new_bank_card, new_gamer_card); // �������������� ����� ��������� (�������) ������ ����, �������� ��������
		std::wcout<< L"���� ������: " << points_21.score << endl;
		std::wcout << L"������� ���� ������: ";
		points_21.init_tmp_bet();
		points_21.choice_card();                           // �������� ��������� �����
		points_21.set_hand_gamer();                        // ������ ����� ������
		while (new_gamer_card || new_bank_card) {
			if (new_bank_card==true) {
				points_21.choice_card();                  // �������� ��������� �����
				points_21.set_hand_bank();                // ������ ����� ����� 
			}
			if (points_21.get_tmp_score_bank() >= 21 || points_21.get_tmp_score_gamer() >= 21) {
				new_gamer_card = false;
			}
			if (new_gamer_card) {
				points_21.choice_card();                      // �������� ��������� �����
				points_21.set_hand_gamer();                   // ������ ����� ������
				system("cls");
				std::wcout << L"\t\t\t���� ""��������""\n\n";
				std::wcout << L"���������� ��������� ���: " << points_21.game - 1;
				std::wcout << L"\t���������� �����: " << points_21.win << L"\t������� �����: " << points_21.win_rate << L"%" << endl << endl;
				std::wcout << L"����� ����������:\n"; points_21.show_hand_bank();
				std::wcout << L"����� ���� ����������: " << points_21.get_tmp_score_bank() << endl;
				std::wcout << L"\n���� �����:\n"; points_21.show_hand_gamer();
				std::wcout << L"����� ����� ����: " << points_21.get_tmp_score_gamer() << endl;
				std::wcout << L"���� ������: " << points_21.score << endl;
				std::wcout << L"���� ������: " << points_21.get_tmp_bet() << endl;
				if (points_21.get_tmp_score_bank() >= 21 || points_21.get_tmp_score_gamer() >= 21) {
					new_gamer_card = false;
					new_bank_card = false;
				}
				else {
					std::wcout << L"\n\n����� ����� �����???\n";
					choice(new_gamer_card);
				}
			}
			if (new_bank_card) {
				system("cls");
				std::wcout << L"\t\t\t���� ""��������""\n\n";
				std::wcout << L"���������� ��������� ���: " << points_21.game - 1;
				std::wcout << L"\t���������� �����: " << points_21.win << L"\t������� �����: " << points_21.win_rate << L"%" << endl << endl;
				std::wcout << L"����� ����������:\n"; points_21.show_hand_bank();
				std::wcout << L"����� ���� ����������: " << points_21.get_tmp_score_bank() << endl;
				std::wcout << L"\n���� �����:\n"; points_21.show_hand_gamer();
				std::wcout << L"����� ����� ����: " << points_21.get_tmp_score_gamer() << endl;
				std::wcout << L"���� ������: " << points_21.score << endl;
				std::wcout << L"���� ������: " << points_21.get_tmp_bet() << endl;
			}
			points_21.new_bank_card(new_bank_card, new_gamer_card);       // ������������� ����� �� ����� ��������� ����� ���������
			if (points_21.get_tmp_score_bank() >= 21 || points_21.get_tmp_score_gamer() >= 21) {
				new_gamer_card = false;
				new_bank_card = false;
			}
		}
		points_21.set_win();                                              // ���������� ����������
		if (points_21.get_win_bank()) {
			std::wcout << L"\n�� ���������!!!" << endl;
			if (points_21.get_tmp_score_bank() <= 21 && points_21.get_tmp_score_gamer() > 21) std::wcout << L"� ��� �������!" << endl;
			if (points_21.score - points_21.get_tmp_bet() > 0) 
				points_21.score -= points_21.get_tmp_bet();
			else points_21.score = 0;
		}
		else if (points_21.get_win_gamer()) {
			std::wcout << L"\n�� ��������!!!" << endl;
			if (points_21.get_tmp_score_bank() > 21 && points_21.get_tmp_score_gamer() <= 21) std::wcout << L"� ���������� �������!" << endl;
			if (points_21.get_tmp_score_gamer() == 21) 
				points_21.score += (double) points_21.get_tmp_bet()*1.5;
			else points_21.score += points_21.get_tmp_bet();
			++points_21.win;
		}
		if (points_21.score > 0) {
			std::wcout << L"\n\n������� ���������� ����???\n";
			choice(v);
		}
		else std::wcout << L"\n\n�� ��������� ��� ���� ������!!!!!\n";
	}
	std::wcout << L"\n�� ����� ������!\n";
	system("pause");
	return 0;
}
