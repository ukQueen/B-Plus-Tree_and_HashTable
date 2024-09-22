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
	virtual vector<Node*> Get_Links() = 0;
	virtual vector<string> Get_Words() = 0;
	virtual void Add_word(string)=0;
	virtual void Add_word(string, vector<Node*>)=0;
	virtual int Get_Count_words() = 0;
	virtual void Set_Links(Node*)=0;
	virtual void Set_Links(vector<Node*>)=0;
	virtual void Set_parent_link(Node*) = 0;
	virtual vector<Node*> Splitting(string) = 0; 
	virtual Node* Get_Parent_link() = 0;
	virtual void Clear_Links() = 0;
	//virtual void Delete_word(string) = 0;
	//virtual void Delete_word(string old,string neew, Node*) = 0;
	virtual void Delete_word(Node*) = 0; //для удаления ссылки на пустого ребенка
	virtual void Rebalancing(Node*) = 0;
	virtual void Rebalancing_keys(Node*) = 0;
	virtual void Clear_word() = 0;
	virtual void Set_Words(vector<string>) = 0;
	//virtual void Set_Words(string) = 0;
	virtual void Clear(Node*) = 0;


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
	void Add_word(string, vector<Node*>) override;
	int Get_Count_words() override;
	vector<string> Get_Words() override;
	vector<Node*> Get_Links()override;
	void Set_New_Root(vector<string>);
	void Set_Links(Node*)override;
	void Set_Links(vector<Node*>) override;
	void Set_parent_link(Node*) override;
	vector<Node*> Splitting(string) override;
	Node* Get_Parent_link() override;
	void Root_Cleaning();
	void Clear_Links() override;
	//void Delete_word(string old, string neew, Node*) override;
	void Delete_word(Node*) override;
	bool Find_word(string);
	Node* Delete_word(string);
	void Rebalancing(Node*) override;
	void Rebalancing_keys(Node*) override;
	void Clear_word() override;
	void Set_Words(vector<string>) override;
	void Clear(Node*) override;
};

///////////////////////////// InnerNode //////////////////////////////////////////

class InnerNode : public Node {


public:
	InnerNode();
	void Add_word(string) override;
	void Add_word(string, vector<Node*>) override;
	int Get_Count_words() override;
	vector<string> Get_Words() override;
	vector<Node*> Get_Links()override;
	void Set_Links(Node*)override;
	void Set_Links(vector<Node*>) override;
	void Set_parent_link(Node*) override;
	vector<Node*> Splitting(string) override;
	Node* Get_Parent_link() override;
	void Clear_Links() override;
	//void Delete_word(string);
	//void Delete_word(string old, string neew, Node*) override;
	void Delete_word(Node*)override;
	void Rebalancing(Node*) override;
	void Rebalancing_keys(Node*) override;
	void Clear_word() override;
	void Set_Words(vector<string>) override;
	void Clear(Node*) override;

};

///////////////////////////// Leaf ///////////////////////////////////////////////

class Leaf : public Node {
	Node* left_Brother = nullptr;
public:
	Leaf();
	void Add_word(string) override;
	void Add_word(string, vector<Node*>) override;
	void Add_word_without_key(string);
	void Delete_word(string);
	int Get_Count_words() override;
	vector<string> Get_Words() override;
	vector<Node*> Get_Links() override;
	void Set_Links(Node*)override;
	void Set_Links(vector<Node*>) override;
	void new_link(Node*);
	void Set_parent_link(Node*) override;
	vector<Node*> Splitting(string) override;
	Node* Get_Parent_link() override;
	void Clear_Links() override;
	//void Delete_word(string old, string neew, Node*) override;
	void Delete_word(Node*) override;
	void Set_brother(Node*);
	Node* Get_brother();
	bool Is_Sibling(Node*);
	void Rebalancing(Node*) override;
	void Rebalancing_keys(Node*) override;
	void Clear_word() override;
	void Set_Words(vector<string>) override;
	void Clear(Node*) override;

};

///////////////////////////// Tree ///////////////////////////////////////////////

class Tree
{
	Root* root;
	int DEGREE = 2;
	int levels = 1;
	vector<string> conj;
	bool is_a_conj(string str);
	//string toDown(string);
	void From_file(string);


public:
	Tree();
	void Add_word(string);
	void Delete_word(string);
	bool Find_word(string);
	void Print();
	void Print(Node* root, int level = 0);
	//void Rebalance_after_deletion(Node* node);
	vector<string> getKeys();
	void getKeys(Node* node, vector<string>&);
	void Menu();
	void Delete_all();//!!!
	void Rebalancing(Node*);

}; 

string toDown(string);