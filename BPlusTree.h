#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <limits>
#include <iomanip> 

using namespace std;


////////////////////////////// Node //////////////////////////////////////////////

class Node {
public:
	virtual vector<Node*> Get_Links();
	virtual vector<string> Get_Words();
	virtual void Add_word(string);
	virtual void Add_word(string, vector<Node*>);
	virtual int Get_Count_words();
	virtual void Set_Links(Node*);
	virtual void Set_Links(vector<Node*>);
	virtual void Set_parent_link(Node*);
	virtual vector<Node*> Splitting(string) = 0; 
	virtual Node* Get_Parent_link();
	virtual void Clear_Links();
	virtual void Delete_word(Node*) = 0; //для удаления ссылки на пустого ребенка
	virtual void Rebalancing(Node*);
	virtual void Rebalancing_keys(Node*);
	virtual void Clear_word();
	virtual void Set_Words(vector<string>);
	virtual void Clear(Node*);


protected:
	int DEGREE; //степень дерева 
	int count_words; //количество слов узле
	vector<string> words;
	vector<Node*> links;
	Node* parent_link = nullptr;//ссылка на родителя

	Node();
};

////////////////////////////// Root //////////////////////////////////////////////

class Root : public Node {


public:
	Root();
	void Add_word(string) override;
	void Set_New_Root(vector<string>);
	vector<Node*> Splitting(string) override;
	void Root_Cleaning();
	void Delete_word(Node*) override;
	Node* Delete_word(string);
	bool Find_word(string);
	void Rebalancing(Node*) override;
	void Rebalancing_keys(Node*) override;
};

///////////////////////////// InnerNode //////////////////////////////////////////

class InnerNode : public Node {


public:
	InnerNode();
	vector<Node*> Splitting(string) override;
	void Delete_word(Node*)override;

};

///////////////////////////// Leaf ///////////////////////////////////////////////

class Leaf : public Node {
	Node* left_Brother = nullptr;
public:
	Leaf();
	using Node::Add_word;
	void Add_word(string, vector<Node*>) override;
	void Add_word_without_key(string);
	void Delete_word(string);
	void new_link(Node*);
	void Set_parent_link(Node*) override;
	vector<Node*> Splitting(string) override;
	void Delete_word(Node*) override;
	void Set_brother(Node*);
	Node* Get_brother();
	bool Is_Sibling(Node*);

};

///////////////////////////// Tree ///////////////////////////////////////////////

class Tree
{
	Root* root;
	int DEGREE = 2;
	int levels = 1;
	vector<string> conj;
	bool is_a_conj(string str);
	void From_file(string);


public:
	Tree();
	void Add_word(string);
	void Delete_word(string);
	bool Find_word(string);
	void Print();
	void Print(Node* root, int level = 0);
	void getKeys(Node* node, vector<string>&);
	void Menu();
	void Delete_all();
	void Rebalancing(Node*);

}; 

string toDown(string);