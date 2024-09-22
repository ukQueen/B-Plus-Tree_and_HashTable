#define NOMINMAX
#include <iostream>
#include "HashTable.h"
#include "BPlusTree.h"

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "rus");

	//Tree b;
	//b.Add_word("а");//1
	////b.Print();
	//b.Add_word("б");//2
	////b.Print();
	//b.Add_word("в");//3
	////b.Print();
	//b.Add_word("г");//4
	//b.Print();
	//b.Add_word("д");//5
	//b.Print();
	//b.Add_word("е");//6
	//b.Print();
	//b.Add_word("ж");//7
	//b.Print();
	//b.Add_word("з");//8 
	//b.Print();
	//b.Add_word("и"); //9
	//b.Print();
	//b.Add_word("к"); //10
	//b.Print();
	//b.Add_word("л"); //11
	//b.Print();
	//b.Add_word("м"); //12
	//b.Print();
	//b.Add_word("н"); //13
	//b.Print();
	//b.Add_word("о"); //14
	//b.Print();
	//b.Add_word("п"); //15
	//b.Print();
	//b.Add_word("р"); //16
	//b.Print();
	//b.Add_word("с"); //17
	//b.Print();
	//b.Add_word("т"); //18
	//b.Print();
	//b.Add_word("у"); //19
	//b.Print();
	//b.Add_word("ф"); //20
	//b.Print();
	//b.Add_word("аа");//1
	//b.Print();	
	//b.Add_word("аб");//2
	//b.Print();	
	//b.Add_word("ав");//3
	//b.Print();	
	//b.Add_word("аг");//4
	//b.Print();	
	//b.Add_word("ад");//5
	//b.Print();	
	//b.Add_word("ае");//6
	//b.Print();	
	//b.Add_word("аж");//7
	//b.Print();	
	//b.Add_word("аз");//8 
	//b.Print();	
	//b.Add_word("аи"); //9
	//b.Print();	
	//b.Add_word("ак"); //10
	//b.Print();	
	//b.Add_word("ал"); //11
	//b.Print();	
	//b.Add_word("ам"); //12
	//b.Print();	
	//b.Add_word("ан"); //13
	//b.Print();	
	//b.Add_word("ао"); //14
	//b.Print();	
	//b.Add_word("ап"); //15
	//b.Print();	
	//b.Add_word("ар"); //16
	//b.Print();	
	//b.Add_word("ас"); //17
	//b.Print();	
	//b.Add_word("ат"); //18
	//b.Print();	
	//b.Add_word("ау"); //19
	//b.Print();	
	//b.Add_word("аф"); //20
	//b.Print();

	//b.Print();

	//
	//cout << "в " << b.Find_word("в") << endl;
	//cout << "с " << b.Find_word("с") << endl;
	//cout << "я " << b.Find_word("я") << endl;
	//cout << "к " << b.Find_word("к") << endl;


	//b.Delete_word("а");//1
	//b.Print();
	//b.Delete_word("б");//2
	//b.Print();
	//b.Delete_word("в");//3
	//b.Print();
	//b.Delete_word("г");//4
	//b.Print();
	//b.Delete_word("д");//5
	//b.Print();
	//b.Delete_word("е");//6
	//b.Print();
	//b.Delete_word("ж");//7
	//b.Print();
	//b.Delete_word("з");//8 
	//b.Print();
	//b.Delete_word("и"); //9
	//b.Print();
	//b.Delete_word("к"); //10
	//b.Print();
	//b.Delete_word("л"); //11
	//b.Print();
	//b.Delete_word("м"); //12
	//b.Print();
	//b.Delete_word("н"); //13
	//b.Print();
	//b.Delete_word("о"); //14
	//b.Print();
	//b.Delete_word("п"); //15
	//b.Print();
	//b.Delete_word("р"); //16
	//b.Print();
	//b.Delete_word("с"); //17
	//b.Print();
	//b.Delete_word("т"); //18
	//b.Print();
	//b.Delete_word("у"); //19
	//b.Print();
	//b.Delete_word("ф"); //20
	//b.Print();

	//b.Delete_word("з");//8 
	//b.Print();
	//b.Delete_word("б");//2
	//b.Print();
	//b.Delete_word("в");//3
	//b.Print();
	//b.Delete_word("о"); //14
	//b.Print();
	//b.Delete_word("д");//5
	//b.Print();
	//b.Delete_word("е");//6
	//b.Print();
	//b.Delete_word("ф"); //20
	//b.Print();
	//b.Delete_word("х"); //21
	//b.Print();
	//b.Delete_word("и"); //9
	//b.Print();
	//b.Delete_word("к"); //10
	//b.Print();
	//b.Delete_word("г");//4
	//b.Print();
	//b.Delete_word("м"); //12
	//b.Print();
	//b.Delete_word("с"); //17
	//b.Print();
	//b.Delete_word("а");//1
	//b.Print();
	//b.Delete_word("р"); //16
	//b.Print();
	//b.Delete_word("п"); //15
	//b.Print();
	//b.Delete_word("н"); //13
	//b.Print();
	//b.Delete_word("т"); //18
	//b.Print();
	//b.Delete_word("у"); //19
	//b.Print();
	//b.Delete_word("л"); //11
	//b.Print();
	//b.Delete_word("ж");//7
	//b.Print();


	HashTable table;
	Tree tree;
	
	
	while (true) {
		printf("\n--------------------- Структуры данных ---------------------\n");
		printf("Сделайте свой выбор:\n");
		printf("[1] - Хэш-таблицв\n");
		printf("[2] - B+ дерево\n");

		printf("[0] - выход из программы\n");
		printf("---------------------------------------------------\n");

		int choose = 0;
		bool out = false;
		string word;
		string file;

		while (true) {

			cin >> choose;

			// Проверка на корректность ввода
			if (std::cin.fail()) {
				std::cin.clear(); // Сброс состояния потока
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера ввода
				std::cout << "Некорректный ввод! Пожалуйста, введите число." << std::endl;
			}
			else if (choose < 0 || choose > 2) {
				// Проверка, что число находится в нужном диапазоне
				std::cout << "Число вне диапазона! Пожалуйста, введите число от 0 до 2." << std::endl;
			}
			else {
				break; // Выход из цикла, если ввод корректен и число в диапазоне
			}
		}
		switch (choose)
		{
		case 0:
			out = true;
			break;
		case 1:
			table.menu();
			break;
		case 2:
			tree.Menu();
			break;
		}

		if (out) {
			break;
		}
		system("pause");

	}
	
}

