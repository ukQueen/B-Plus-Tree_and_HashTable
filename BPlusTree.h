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
};

///////////////////////////// Leaf ///////////////////////////////////////////////

class Leaf : public Node {

public:
	Leaf();
	void Add_word(string) override;
	void Add_word(string, vector<Node*>) override;
	int Get_Count_words() override;
	vector<string> Get_Words() override;
	vector<Node*> Get_Links() override;
	void Set_Links(Node*)override;
	void Set_Links(vector<Node*>) override;
	void Set_parent_link(Node*) override;
	vector<Node*> Splitting(string) override;
	Node* Get_Parent_link() override;
};

///////////////////////////// Tree ///////////////////////////////////////////////

class Tree
{
	Root* root;
	int DEGREE = 2;
	int levels = 1;

public:
	Tree();
	void Add_word(string);
	void Print();
	void Print(Node* root, int level =0);
};