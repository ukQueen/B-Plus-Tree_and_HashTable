#define NOMINMAX
#include "HashTable.h"

HashTable::HashTable() {
	vector<vector<string>> buf(6);
	backets = buf;
	count_backets = 6;
	count_words = 0;
	percent = 0;
	conj = { "�", "��", "��", "�����", "����", "�", "��", "������", "����", "��", "���", "����", "��"
		"��", "������", "���", "���", "���", "���", "���������", "����", "����","��", "����", "�����", "�����", "���",
		"����","�����", "����", "�", "����", "���", "����", "��", "����", "����", "�����", "��", "���",
		"���", "���", "�", "���", "��", "��", "��", "��", "���", "���", "����", "��", "���", "���", "����", "���",
		"��", "��", "���", "����", "����", "�����", "���", "���", "��", "���", "�����", "�����", "����", "���",
		"����", "����", "���", "����", "���", "�", "��", "���", "����", "����", "����", "���", "����", "����", "����",
		"����", "����", "���", "����", "����", "���", "���", "���", "�����", "�����", "����", "����", "���", "��",
		"��", "��", "�����", "�����", "�����", "�����", "�������", "��������", "��������", "����", "���", "��", "���",
		"�����", "�����", "����", "���", "����", "����", "����", "������", "������", "������", "������", "�����",
		"�����", "�����", "�����", "����", "����", "����", "����", "������", "������", "������", "������", "������",
		"������", "������", "������", "���", "����", "����", "���", "���", "����", "����", "���", "�����", "�����",
		"�����", "�����", "���", "���", "���", "���", "�������", "�������", "�������", "�������", "�������",
		"��������", "��������", "�����", "������", "������", "�����", "�����", "������", "������", "�����", "�������",
		"�������", "�������", "�������", "�����", "�����", "�����", "�����", "�����", "�����", "���������", "������", 
		"�", "�", "��","���", "��", "���", "��", "�����", "��", "��", "����", "��-��", "���", "���" };
}

//��� �������
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
		printf("\n�� ������� ������� ����\n\n");
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
				&& c != '"' && c != '�') {
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
				//word += to_string(k); //�������������� ��� �������� 
				this->Add(word);
			}
		}

		printf("\n����� �� ����� ��������� � �������\n\n");
	}
}

string HashTable::toDown(string str) {
	string result = "";
	for (char c : str) {
		if (c >= 'A' && c <= 'Z') {
			c = c + ('a' - 'A');
		}
		if (c >= '�' && c <= '�') {
			c = c + ('�' - '�');
		}
		result += c;
	}
	return result;
}


void HashTable::menu() {
	while (true) {
		printf("\n--------------------- ������� (���-�������)---------------------\n");
		printf("�������� ��������:\n");
		printf("[1] - �������� ���������� ������� \n");
		printf("[2] - ���������� ������ �����\n");
		printf("[3] - �������� �����\n");
		printf("[4] - �������� ������� ���� � �������\n");
		printf("[5] - ������ ������� �������\n");
		printf("[6] - ���������� ���� �� �����\n");
		printf("[7] - �������� ������ ��������������� ����\n");

		printf("[0] - ����� �� �������\n");
		printf("---------------------------------------------------\n");

		int choose;
		bool out = false;
		string word;
		string file;
		//vector<string> files = { "Master and Margarita.txt", "Crime and punishment.txt" };
		vector<string> files = { "������ � ���������.txt", "������������ � ���������.txt" };
		int k;
		bool flag_out_path;

		while (true) {
			
		cin >> choose;

			// �������� �� ������������ �����
			if (std::cin.fail()) {
				std::cin.clear(); // ����� ��������� ������
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ������� ������ �����
				std::cout << "������������ ����! ����������, ������� �����." << std::endl;
			}
			else if (choose < 0 || choose > 7) {
				// ��������, ��� ����� ��������� � ������ ���������
				std::cout << "����� ��� ���������! ����������, ������� ����� �� 0 �� 7."<< std::endl;
			}
			else {
				break; // ����� �� �����, ���� ���� ��������� � ����� � ���������
			}
		}


		switch (choose)
		{
		case 0:
			out = true;
			break;
		case 1:
			if (count_words == 0) {
				printf("\n������� ����\n\n");
			}
			else {
				printf("\n���������� �������:\n");
				printf("[1] - �� ������� \n");
				printf("[2] - ������� \n\n");
				printf("��� �� ������? ������� ���� �����: ");
				while (true) {

				cin >> k;

					// �������� �� ������������ �����
					if (std::cin.fail()) {
						std::cin.clear(); // ����� ��������� ������
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ������� ������ �����
						std::cout << "������������ ����! ����������, ������� �����." << std::endl;
					}
					else if (k < 1 || k > 2) {
						// ��������, ��� ����� ��������� � ������ ���������
						std::cout << "����� ��� ���������! ����������, ������� ����� �� 1 �� 2." << std::endl;
					}
					else {
						break; // ����� �� �����, ���� ���� ��������� � ����� � ���������
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
					printf("\n���������� ����: %d\n\n", count_words);
					break;
				}
			}
			break;
		case 2:
			while (true) {
				printf("������� ����� ��� ���������� � ������� (��� ������ � ���� �������� '0'): ");
				cin >> word;
				if (word == "0") {
					break;
				}
				else {
					if (!Contains(word)) {
						if (!is_a_conj(word)) {
							this->Add(word);
							printf("\n\t����� '%s' ��������� � �������\n\n", word.c_str());
							//break;
						}
						else {
							printf("\n\t����� '%s' �� ��������� � �������, ��� ��� ��� ������ � ������ ��������������� ���� :(\n\n", word.c_str());
						}
					}
					else {
						printf("\n\t����� '%s' �� ��������� � �������, ��� ��� ��� �����������\n\n", word.c_str());
					}
				}

			};
			break;

		case 3:
			while (true) {
				printf("������� �����, ������� ������ ������� (��� ������ � ���� �������� '0'): ");
				cin >> word;
				if (word == "0") {
					break;
				}
				else {
					if (!this->Contains(word)) {
						printf("\n\t����� '%s' �����������\n\n", word.c_str());
					}
					else {
						this->Delete(word);
						printf("\n\t����� '%s' �������\n\n", word.c_str());
						//break;
					}
				}
			}
			break;

		case 4:
			while (true) {
				printf("������� �����, ������� ������ ������ ��������� (��� ������ � ���� �������� '0'): ");
				cin >> word;
				if (word == "0") {
					break;
				}
				else {
					if (this->Contains(word)) {
						printf("\n\t����� '%s' ������������ � �������\n\n", word.c_str());
					}
					else {
						printf("\n\t����� '%s' ���������� � �������\n\n", word.c_str());
					}
					//break;
				}
			}
			break;

		case 5:
			this->Delete_all();
			printf("\n������� ��������� ������\n\n");
			break;

		case 6:
			printf("\n��������� ���� �� ������������:\n");
			printf("1. %s\n", files[0].c_str());
			printf("2. %s\n", files[1].c_str());
			printf("3. ������...\n\n");
			printf("0. �����\n\n");
			printf("��� �����: ");

			flag_out_path = true;

			while (true) {

			cin >> k;

				// �������� �� ������������ �����
				if (std::cin.fail()) {
					std::cin.clear(); // ����� ��������� ������
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ������� ������ �����
					std::cout << "������������ ����! ����������, ������� �����." << std::endl;
				}
				else if (k < 0 || k > 3) {
					// ��������, ��� ����� ��������� � ������ ���������
					std::cout << "����� ��� ���������! ����������, ������� ����� �� 0 �� 3." << std::endl;
				}
				else {
					break; // ����� �� �����, ���� ���� ��������� � ����� � ���������
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
				printf("�������� �������� ������ ����� � '.txt': ");
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
			printf("C����� ��������������� ����\n");

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