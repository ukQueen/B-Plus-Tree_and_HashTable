#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <limits>


using namespace std;

class HashTable
{
	int count_backets;
	int count_words;
	vector<vector<string>> backets;
	vector<string> conj;
	double percent;

	int HashCode(string);
	// функция для преобразования заглавных букв в слове к строчным
	string toDown(string);
	void Add(string);
	bool Contains(string);
	void Delete(string);
	void Delete_all();
	void From_file(string);
	bool is_a_conj(string str);


public:

	void menu();
	HashTable();
};