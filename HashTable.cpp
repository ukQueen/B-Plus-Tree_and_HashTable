#define NOMINMAX
#include "HashTable.h"

HashTable::HashTable() {
	vector<vector<string>> buf(6);
	backets = buf;
	count_backets = 6;
	count_words = 0;
	percent = 0;
	conj = { "и", "да", "ни", "также", "тоже", "а", "но", "однако", "зато", "же", "или", "либо", "то"
		"не", "потому", "что", "так", "как", "ибо", "поскольку", "пока", "если","бы", "коли", "ежели", "чтобы", "для",
		"того","чтобы", "хотя", "я", "меня", "мне", "мной", "ты", "тебя", "тебе", "тобой", "он", "она",
		"оно", "его", "её", "ему", "ей", "им", "ею", "мы", "нас", "нам", "нами", "вы", "вас", "вам", "вами", "они",
		"их", "им", "ими", "себя", "себе", "собой", "мой", "моя", "моё", "мои", "моего", "моему", "моим", "моём",
		"твой", "твоя", "твоё", "твои", "его", "её", "их", "наш", "наша", "наше", "наши", "ваш", "ваша", "ваше", "ваши",
		"свой", "своя", "своё", "свои", "этот", "эта", "это", "эти", "этого", "этому", "этим", "этом", "тот", "та",
		"то", "те", "такой", "такая", "такое", "такие", "столько", "стольких", "стольким", "весь", "вся", "всё", "все",
		"всего", "всему", "всем", "сам", "сама", "само", "сами", "каждый", "каждая", "каждое", "каждые", "любой",
		"любая", "любое", "любые", "иной", "иная", "иное", "иные", "другой", "другая", "другое", "другие", "всякий",
		"всякая", "всякое", "всякие", "кто", "кого", "кому", "кем", "что", "чего", "чему", "чем", "какой", "какая",
		"какое", "какие", "чей", "чья", "чьё", "чьи", "который", "которая", "которое", "которые", "сколько",
		"скольких", "скольким", "никто", "никого", "никому", "никем", "ничто", "ничего", "ничему", "ничем", "никакой",
		"никакая", "никакое", "никакие", "ничей", "ничья", "ничьё", "ничьи", "некто", "нечто", "некоторые", "нибудь", 
		"в", "к", "на","под", "не", "тут", "из", "своим", "от", "из", "изза", "из-за", "еще", "нет" };
}

//Хэш функция
int HashTable::HashCode(string str) {
	int result = 0;
	int indx = 7;

	result = indx;
	
	for (int i = 1; i < str.size() + 1; i++) {
		int x = static_cast<int>(str[i - 1]);
		for (int j = 1; j < i; j++) {
			x *= x;
		}
		result += (indx + i) * x;
	}
	return result;
}


bool HashTable::Contains(string str) {
	int code = HashCode(str);
	int n = code % count_backets;
	n = n < 0 ? -1 * n : n;
	for (string word : backets[n]) {
		if (word == str)
			return true;
	}
	return false;
}


void HashTable::Add(string str) {
	str = toDown(str);
	if (!Contains(str)) {
		if (percent >= 75) {
			vector<vector<string>> buf(count_backets * 2);
			count_backets *= 2;
			for (vector<string> strings : backets) {
				for (string s : strings) {
					int code = HashCode(s);
					int n = code % count_backets;
					n = n < 0 ? -1 * n : n;
					buf[n].push_back(s);
				}

			}
			backets = buf;
		}
		int code = HashCode(str);
		int n = code % count_backets;
		n = n < 0 ? -1 * n : n;
		backets[n].push_back(str);
		count_words++;
		percent = static_cast<double>(count_words) / count_backets * 100;
	}
}

void HashTable::Delete(string str) {
	str = toDown(str);
	if (Contains(str)) {
		int code = HashCode(str);
		int n = code % count_backets;
		n = n < 0 ? -1 * n : n;
		for (int i = 0; i < backets[n].size(); i++) {
			if (backets[n][i] == str) {
				backets[n].erase(backets[n].begin() + i);
				count_words--;
				break;
			}
		}
	}
}

bool HashTable::is_a_conj(string str) {
	str = toDown(str);
	for (string word : conj) {
		if (str == word) {
			return true;
		}
	}
	return false;
}

void HashTable::Delete_all() {
	vector<vector<string>> buf(6);
	backets = buf;
	count_backets = 6;
	count_words = 0;
}


void HashTable::From_file(string str) {
	//std::filesystem::path file_path = str;
	//ifstream file(R"(str)");
	ifstream file(str);
	if (!file.is_open()) {
		printf("\nНе удалось открыть файл\n\n");
	}
	else {
		string line;
		vector<string> words = { "" };
		int last_index = 0;
		char c;
		int k = 0;
		while (file.get(c)) {
			if (c != ',' && c != '.' && c != ':' && c != ';' && c != '!' && c != '?' && c != '-' && c != '?'
				&& c != '(' && c != ')' && c != '{' && c != '}' && c != '[' && c != ']' && c != '\''
				&& c != '"' && c != '—') {
				if (c != ' ' && c != '\n') {
					words[last_index] += c;
				}
				else {
					if (words[last_index].size() > 0) {
						words.push_back("");
						last_index++;
					}
				}
			}

		}

		if (words[words.size() - 1] == "") {
			words.pop_back();
		}

		file.close();
		for (auto word : words) {
			if (!this->is_a_conj(word)) {
				//k++;
				//word += to_string(k); //использовалось для проверки 
				this->Add(word);
			}
		}

		printf("\nСлова из файла добавлены в словарь\n\n");
	}
}

string HashTable::toDown(string str) {
	string result = "";
	for (char c : str) {
		if (c >= 'A' && c <= 'Z') {
			c = c + ('a' - 'A');
		}
		if (c >= 'А' && c <= 'Я') {
			c = c + ('а' - 'А');
		}
		result += c;
	}
	return result;
}


void HashTable::menu() {
	while (true) {
		printf("\n--------------------- Словарь (хэш-таблица)---------------------\n");
		printf("Выберите действие:\n");
		printf("[1] - просмотр содержимое словаря \n");
		printf("[2] - добавление нового слова\n");
		printf("[3] - удаление слова\n");
		printf("[4] - проверка наличие слов в словаре\n");
		printf("[5] - полная очистка словаря\n");
		printf("[6] - добавление слов из файла\n");
		printf("[7] - просмотр список недобавляющихся слов\n");

		printf("[0] - выход из словаря\n");
		printf("---------------------------------------------------\n");

		int choose;
		bool out = false;
		string word;
		string file;
		//vector<string> files = { "Master and Margarita.txt", "Crime and punishment.txt" };
		vector<string> files = { "Мастер и Маргарита.txt", "Преступление и наказание.txt" };
		int k;
		bool flag_out_path;

		while (true) {
			
		cin >> choose;

			// Проверка на корректность ввода
			if (std::cin.fail()) {
				std::cin.clear(); // Сброс состояния потока
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера ввода
				std::cout << "Некорректный ввод! Пожалуйста, введите число." << std::endl;
			}
			else if (choose < 0 || choose > 7) {
				// Проверка, что число находится в нужном диапазоне
				std::cout << "Число вне диапазона! Пожалуйста, введите число от 0 до 7."<< std::endl;
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
			if (count_words == 0) {
				printf("\nСловарь пуст\n\n");
			}
			else {
				printf("\nСодержимое словаря:\n");
				printf("[1] - по бакетам \n");
				printf("[2] - списком \n\n");
				printf("Как вы хотите? Введите свой выбор: ");
				while (true) {

				cin >> k;

					// Проверка на корректность ввода
					if (std::cin.fail()) {
						std::cin.clear(); // Сброс состояния потока
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера ввода
						std::cout << "Некорректный ввод! Пожалуйста, введите число." << std::endl;
					}
					else if (k < 1 || k > 2) {
						// Проверка, что число находится в нужном диапазоне
						std::cout << "Число вне диапазона! Пожалуйста, введите число от 1 до 2." << std::endl;
					}
					else {
						break; // Выход из цикла, если ввод корректен и число в диапазоне
					}
				}


				switch (k) {
				case 2:
					k = 1;
					for (auto words : backets) {
						for (auto word : words) {
							printf("%d. %s \n", k, word.c_str());
							k++;
						}
					}

					break;

				case 1:
					k = 0;
					for (auto words : backets) {
						printf("[%d]: ", k);
						for (auto word : words) {
							cout << word << ' ';
						}
						cout << endl;
						k++;
					}
					printf("\nКоличество слов: %d\n\n", count_words);
					break;
				}
			}
			break;
		case 2:
			while (true) {
				printf("Введите слово для добавления в словарь (для выхода в меню напишите '0'): ");
				cin >> word;
				if (word == "0") {
					break;
				}
				else {
					if (!Contains(word)) {
						if (!is_a_conj(word)) {
							this->Add(word);
							printf("\n\tСлово '%s' добавлено в словарь\n\n", word.c_str());
							//break;
						}
						else {
							printf("\n\tСлово '%s' не добавлено в словарь, так как оно входит в список недобавляющихся слов :(\n\n", word.c_str());
						}
					}
					else {
						printf("\n\tСлово '%s' не добавлено в словарь, оно уже там присутсвует\n\n", word.c_str());
					}
				}

			};
			break;

		case 3:
			while (true) {
				printf("введите слово, которое хотите удалить (для выхода в меню напишите '0'): ");
				cin >> word;
				if (word == "0") {
					break;
				}
				else {
					if (!this->Contains(word)) {
						printf("\n\tСлово '%s' отсутствует\n\n", word.c_str());
					}
					else {
						this->Delete(word);
						printf("\n\tСлово '%s' удалено\n\n", word.c_str());
						//break;
					}
				}
			}
			break;

		case 4:
			while (true) {
				printf("Введите слово, наличие корого хотите проверить (для выхода в меню напишите '0'): ");
				cin >> word;
				if (word == "0") {
					break;
				}
				else {
					if (this->Contains(word)) {
						printf("\n\tСлово '%s' присутствует в словаре\n\n", word.c_str());
					}
					else {
						printf("\n\tСлово '%s' отсутсвует в словаре\n\n", word.c_str());
					}
					//break;
				}
			}
			break;

		case 5:
			this->Delete_all();
			printf("\nСловарь полностью очищен\n\n");
			break;

		case 6:
			printf("\nВыбериете файл из существующих:\n");
			printf("1. %s\n", files[0].c_str());
			printf("2. %s\n", files[1].c_str());
			printf("3. другое...\n\n");
			printf("0. ВЫХОД\n\n");
			printf("Ваш выбор: ");

			flag_out_path = true;

			while (true) {

			cin >> k;

				// Проверка на корректность ввода
				if (std::cin.fail()) {
					std::cin.clear(); // Сброс состояния потока
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера ввода
					std::cout << "Некорректный ввод! Пожалуйста, введите число." << std::endl;
				}
				else if (k < 0 || k > 3) {
					// Проверка, что число находится в нужном диапазоне
					std::cout << "Число вне диапазона! Пожалуйста, введите число от 0 до 3." << std::endl;
				}
				else {
					break; // Выход из цикла, если ввод корректен и число в диапазоне
				}
			}


			switch (k) {
			case 1:
				//file = "D:/Another/CLion Projects/txt files/" + files[0];
				file = files[0];
				break;
			case 2:
				//file = "D:/Another/CLion Projects/txt files/" + files[1];
				file = files[1];
				break;
			case 3:
				printf("напишите название своего файла с '.txt': ");
				cin >> file;
				break;
			case 0:
				flag_out_path = false;
				break;
			}
			if (flag_out_path)
				this->From_file(file);
			break;

		case 7:
			printf("Cписок недобавляющихся слов\n");

			for (int i = 0; i < this->conj.size(); i++) {
				printf("%d. %s\n", i + 1, this->conj[i].c_str());
			}
			cout << "\n";
		}

		if (out) {
			break;
		}
		system("pause");
	}
}