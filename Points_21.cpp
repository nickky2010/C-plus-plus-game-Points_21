//                          Курс. Объектно - ориентированное программирование на C++.
//                                          Игра "Блэкджек"(21 очко)
#include <iostream>
#include <windows.h>
#include <time.h>
#include "io.h"
#include "fcntl.h"
using namespace std;
class Points_21
{
private:
	char сard_deck[4][13] = { { '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A' },
	                          { '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A' },
	                          { '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A' },
	                          { '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A' } };
	char сard_deck_tmp[4][13];
	wchar_t suit; char card;              // suit - масть, card - карта
	int max_score_bank;           // ограничение для банкира (компьютера) - рандомно
	wchar_t hand_bank[18];           // на руках у банкира (максимум при котором проигрыш: 4 раза по 2 + 4 раза по 3 + 1 раз по 4 = 9*2 - на масть)
	wchar_t hand_gamer[18];          // на руках у игрока (максимум при котором проигрыш: 4 раза по 2 + 4 раза по 3 + 1 раз по 4 = 9*2 - на масть)
	int move_bank=0;
	int move_gamer = 0;
	int tmp_score_bank = 0;
	int tmp_score_gamer = 0;
	int tmp_bet = 0;
	bool win_bank = false;
	bool win_gamer = false;
public:
	static double score;                // счетчик очков за сессию
	static int game;                 // счетчик количества игр
	static int win;                  // счетчик количества побед
	static double win_rate;          // счетчик % побед
	Points_21()                      // конструктор по умолчанию
	{
	}
	~Points_21()                     // деструктор
	{
	}
	int init_Rand(int max, int min)                // инициализация случайных чисел 
	{
		int number = rand() % (max - min + 1) + min;
		return number;
	}
	void choice_card()      // выбираем случайную карту
	{	
		int i, j;
		do{
			card = сard_deck_tmp[i=init_Rand(3, 0)][j=init_Rand(12, 0)];  // ищем 
		} while (card == ' ');                                            // пока не получим значение
		// устанавливаем масть
		if (i == 0) { suit = L'\u2665\ '; } // черва
		if (i == 1) { suit = L'\u2666\ '; } // бубна
		if (i == 2) { suit = L'\u2663\ '; } // трефа
		if (i == 3) { suit = L'\u2660\ '; } // пика
		сard_deck_tmp[i][j] = ' ';                                        // убираем из колоды
	}
	void set_new_сard_deck_tmp()                   // инициализируем новую временную (рабочую) колоду карт
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 13; j++) {
				сard_deck_tmp[i][j] = сard_deck[i][j];
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
			if (game > 10 && win_rate > 50) max_score_bank = init_Rand(17, 16);            // для имитации более взвешенного подхода при выигрыше игрока
			else if (game > 10 && win_rate <= 50) max_score_bank = init_Rand(19, 16);      // для имитации более рискового подхода при проигрыше игрока
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
		set_new_сard_deck_tmp();
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
			std::wcout << L"Ставка не может быть меньше 1 или больше суммы ваших денег!!!" << endl;
			std::wcout << L"Сделайте вашу ставку: ";
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
double Points_21::score = 100;              // счетчик очков за сессию            
int Points_21::game = 0;                 // счетчик количества игр
int Points_21::win = 0;                  // счетчик количества побед
double Points_21::win_rate;              // счетчик % побед
void quit() {                            // функция выхода
	std::wcout << L"Завершение работы!\n";
	system("pause");
	exit(0);
}
void choice(bool &v) {
	do {
		std::wcout << L"1 - да\t0 - нет\n";
		std::wcout << L"Ваш выбор: ";
		cin >> v;
		if (v != 0 && v != 1)
			std::wcout << L"\nОшибка! Неверный ввод! Введите 0 или 1!\n";
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
	std::wcout << L"\t\t\tИгра ""Блэкджек""\n";
	std::wcout << L"\t\t\tПравила игры:\nВ игре используется одна колода состоящая из 52 карт.\n";
	std::wcout << L"Значения очков каждой карты: от двойки до десятки — соответственно от 2 до 10.\n";
	std::wcout << L"Валет, Дама и Король - 10 очков, а Туз - 11 очков.\n";
	std::wcout << L"Если у игрока сразу после раздачи набралось 21 очко (то есть у игрока туз и 10 или два туза),\n";
	std::wcout << L"то такая ситуация и называется блек-джек. В таком случае игроку сразу выплачивается выигрыш 3 к 2 \n";
	std::wcout << L"(то есть в 1,5 раза превышающий его ставку).\n";
	std::wcout << L"Компьютер, также как и игрок, при выигрыше склонен больше рисковать, а при проигрыше - более расчетлив.\n\n";
	system("pause");
	while (v) {
		system("cls");
		if (points_21.game>0) points_21.win_rate =  ((double)points_21.win / points_21.game)*100.;
		else points_21.win_rate = 0;
		++points_21.game;
		points_21.new_game(new_bank_card, new_gamer_card); // инициализируем новую временную (рабочую) колоду карт, обнуляем счетчики
		std::wcout<< L"Ваши деньги: " << points_21.score << endl;
		std::wcout << L"Введите вашу ставку: ";
		points_21.init_tmp_bet();
		points_21.choice_card();                           // выбираем случайную карту
		points_21.set_hand_gamer();                        // отдаем карту игроку
		while (new_gamer_card || new_bank_card) {
			if (new_bank_card==true) {
				points_21.choice_card();                  // выбираем случайную карту
				points_21.set_hand_bank();                // отдаем карту банку 
			}
			if (points_21.get_tmp_score_bank() >= 21 || points_21.get_tmp_score_gamer() >= 21) {
				new_gamer_card = false;
			}
			if (new_gamer_card) {
				points_21.choice_card();                      // выбираем случайную карту
				points_21.set_hand_gamer();                   // отдаем карту игроку
				system("cls");
				std::wcout << L"\t\t\tИгра ""Блэкджек""\n\n";
				std::wcout << L"Количество сыгранных игр: " << points_21.game - 1;
				std::wcout << L"\tКоличество побед: " << points_21.win << L"\tПроцент побед: " << points_21.win_rate << L"%" << endl << endl;
				std::wcout << L"Карты компьютера:\n"; points_21.show_hand_bank();
				std::wcout << L"Сумма карт компьютера: " << points_21.get_tmp_score_bank() << endl;
				std::wcout << L"\nВаши карты:\n"; points_21.show_hand_gamer();
				std::wcout << L"Сумма ваших карт: " << points_21.get_tmp_score_gamer() << endl;
				std::wcout << L"Ваши деньги: " << points_21.score << endl;
				std::wcout << L"Ваша ставка: " << points_21.get_tmp_bet() << endl;
				if (points_21.get_tmp_score_bank() >= 21 || points_21.get_tmp_score_gamer() >= 21) {
					new_gamer_card = false;
					new_bank_card = false;
				}
				else {
					std::wcout << L"\n\nВзять новую карту???\n";
					choice(new_gamer_card);
				}
			}
			if (new_bank_card) {
				system("cls");
				std::wcout << L"\t\t\tИгра ""Блэкджек""\n\n";
				std::wcout << L"Количество сыгранных игр: " << points_21.game - 1;
				std::wcout << L"\tКоличество побед: " << points_21.win << L"\tПроцент побед: " << points_21.win_rate << L"%" << endl << endl;
				std::wcout << L"Карты компьютера:\n"; points_21.show_hand_bank();
				std::wcout << L"Сумма карт компьютера: " << points_21.get_tmp_score_bank() << endl;
				std::wcout << L"\nВаши карты:\n"; points_21.show_hand_gamer();
				std::wcout << L"Сумма ваших карт: " << points_21.get_tmp_score_gamer() << endl;
				std::wcout << L"Ваши деньги: " << points_21.score << endl;
				std::wcout << L"Ваша ставка: " << points_21.get_tmp_bet() << endl;
			}
			points_21.new_bank_card(new_bank_card, new_gamer_card);       // устанавливаем будет ли брать следующую карту компьютер
			if (points_21.get_tmp_score_bank() >= 21 || points_21.get_tmp_score_gamer() >= 21) {
				new_gamer_card = false;
				new_bank_card = false;
			}
		}
		points_21.set_win();                                              // определяем победителя
		if (points_21.get_win_bank()) {
			std::wcout << L"\nВы проиграли!!!" << endl;
			if (points_21.get_tmp_score_bank() <= 21 && points_21.get_tmp_score_gamer() > 21) std::wcout << L"У Вас перебор!" << endl;
			if (points_21.score - points_21.get_tmp_bet() > 0) 
				points_21.score -= points_21.get_tmp_bet();
			else points_21.score = 0;
		}
		else if (points_21.get_win_gamer()) {
			std::wcout << L"\nВы выиграли!!!" << endl;
			if (points_21.get_tmp_score_bank() > 21 && points_21.get_tmp_score_gamer() <= 21) std::wcout << L"У компьютера перебор!" << endl;
			if (points_21.get_tmp_score_gamer() == 21) 
				points_21.score += (double) points_21.get_tmp_bet()*1.5;
			else points_21.score += points_21.get_tmp_bet();
			++points_21.win;
		}
		if (points_21.score > 0) {
			std::wcout << L"\n\nЖелаете продолжить игру???\n";
			choice(v);
		}
		else std::wcout << L"\n\nВы проиграли все свои деньги!!!!!\n";
	}
	std::wcout << L"\nДо новых встреч!\n";
	system("pause");
	return 0;
}
