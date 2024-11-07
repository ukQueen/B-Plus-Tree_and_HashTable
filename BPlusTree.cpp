#define NOMINMAX
#include "BPlusTree.h"

////////////////////////////// Node //////////////////////////////////////////////

Node::Node() {
	this->count_words = 0;
	this->DEGREE = 2;
}

vector<Node*> Node::Get_Links() {
	return links;
}

vector<string> Node::Get_Words() {
	return words;
}

int Node::Get_Count_words() {
	return count_words;
}

void Node::Set_Links(Node* link) {
	links.push_back(link);
}

void Node::Set_Links(vector<Node*> link) {
	links = link;
}

void Node::Set_parent_link(Node* link) {
	parent_link = link;
	for (auto link : links) {
		link->Set_parent_link(this);
	}
}

Node* Node::Get_Parent_link() {
	return this->parent_link;
}

void Node::Clear_Links() {
	links.clear();
}

void Node::Clear_word() {
	words.clear();
	count_words = 0;
}

void Node::Set_Words(vector<string> words) {
	this->words = words;
	count_words = words.size();
}

void Node::Add_word(string str) {
	if (words.size() == 0) {
		words.push_back(str);
		count_words++;
	}
	else {
		bool flag = true;
		for (int i = 0; i < words.size(); i++) {
			if (str < words[i]) {
				words.insert(words.begin() + i, str);
				count_words++;
				flag = false;
				break;
			}
		}
		if (flag) {
			words.push_back(str);
			count_words++;
		}
	}
}

void Node::Add_word(string str, vector<Node*> link) {
	bool flag_link = false;
	if (Leaf* buf_leaf = dynamic_cast<Leaf*>(link[0]))
		flag_link = true;
	if (words.size() == 0) {
		words.push_back(str);
		links.push_back(link[0]);
		links.push_back(link[1]);
		count_words++;
	}
	else {
		bool flag = true;
		for (int i = 0; i < words.size(); i++) {
			if (str < words[i]) {
				words.insert(words.begin() + i, str);
				links.erase(links.begin() + i);
				links.insert(links.begin() + i, link[0]);
				links.insert(links.begin() + i + 1, link[1]);
				flag = false;
				count_words++;
				break;
			}
		}
		if (flag) {
			words.push_back(str);
			links.pop_back();
			links.push_back(link[0]);
			links.push_back(link[1]);
			count_words++;
		}
	}
	if (flag_link) {
		for (int i = 0; i < links.size() - 1; i++) {
			links[i]->Clear_Links();
			links[i]->Set_Links(links[i + 1]);
		}
	}
}

void Node::Rebalancing(Node* node) {
	if (!node) return;

	if (InnerNode* inner_node = dynamic_cast<InnerNode*>(node->Get_Links()[0])) {
		auto current_links = node->Get_Links();
		int count_links = node->Get_Links().size();
		vector<Node*> new_links;

		for (auto link : current_links) {
			auto children_links = link->Get_Links();
			for (auto child_link : children_links) {
				new_links.push_back(child_link);
			}
		}

		int count = new_links.size() / count_links;

		if (count > 1) {
			for (int i = 0; i < current_links.size(); i++) {
				current_links[i]->Clear_Links();
				if (i != current_links.size() - 1) {
					for (int ii = 0; ii < count; ii++) {
						current_links[i]->Set_Links(new_links[0]);
						new_links[0]->Set_parent_link(current_links[i]);// поправляем ссылку на родителя
						new_links.erase(new_links.begin());
					}
				}
				else {
					current_links[i]->Set_Links(new_links);
					for (auto link : new_links) {
						link->Set_parent_link(current_links[i]);
					}
				}
			}

		}
		else {//если остается одна ссылка в каком то из узлов -> нужно убрать однуссылку из родителя
			count++;
			for (int i = 0; i < current_links.size() - 1; i++) {
				current_links[i]->Clear_Links();
				if (i != current_links.size() - 2) {
					for (int ii = 0; ii < count; ii++) {
						current_links[i]->Set_Links(new_links[0]);
						new_links[0]->Set_parent_link(current_links[i]);// поправляем ссылку на родителя
						new_links.erase(new_links.begin());
					}
				}
				else {
					current_links[i]->Set_Links(new_links);
					for (auto link : new_links) {
						link->Set_parent_link(current_links[i]);
					}
				}
			}
			links.pop_back();
		}
		for (auto child : inner_node->Get_Links()) {
			Rebalancing(child);
		}
	}
	else if (Root* root = dynamic_cast<Root*>(node)) {

		for (auto child : root->Get_Links()) {
			Rebalancing(child);
		}
	}
}

void Node::Rebalancing_keys(Node* node) {
	if (!node) return;

	if (Root* root = dynamic_cast<Root*>(node)) {
		auto links = node->Get_Links();
		vector<string> new_words;

		for (int i = 1; i < links.size(); i++) {
			Node* current_link = links[i];
			while (true) {
				if (Leaf* leaf = dynamic_cast<Leaf*>(current_link))
					break;
				current_link = current_link->Get_Links()[0];
			}
			new_words.push_back(current_link->Get_Words()[0]);
		}
	}

	if (InnerNode* root = dynamic_cast<InnerNode*>(node)) {
		auto links = node->Get_Links();
		vector<string> new_words;

		for (int i = 1; i < links.size(); i++) {
			Node* current_link = links[i];
			while (true) {
				if (Leaf* leaf = dynamic_cast<Leaf*>(current_link))
					break;
				current_link = current_link->Get_Links()[0];
			}
			new_words.push_back(current_link->Get_Words()[0]);
		}
	}
}

void Node::Clear(Node* node) {
	if (Leaf* leaf = dynamic_cast<Leaf*>(node)) {
		delete leaf;
	}
	else if (InnerNode* i_node = dynamic_cast<InnerNode*>(node)) {
		for (auto child : i_node->Get_Links()) {
			Clear(child);
		}
		delete i_node;
	}
	else if (Root* root = dynamic_cast<Root*>(node)) {
		for (auto child : root->Get_Links()) {
			Clear(child);
		}
		delete root;
	}
}

////////////////////////////// Root //////////////////////////////////////////////

Root::Root() {}

vector<Node*> Root::Splitting(string str) {
	bool flag = true;
	for (int i = 0; i < words.size(); i++) {
		if (str < words[i]) {
			words.insert(words.begin() + i, str);
			flag = false;
			count_words++;
			break;
		}
	}
	if (flag) {
		words.push_back(str);
		count_words++;
	}

	InnerNode* root1 = new InnerNode;
	root1->Add_word(words[0]);
	root1->Add_word(words[1]);

	InnerNode* root2 = new InnerNode;
	root2->Add_word(words[3]);

	if (links.size() != 0) {
		root1->Set_Links(vector<Node*>{links[0], links[1], links[2]});
		root2->Set_Links(vector<Node*>{links[3], links[4]});
	}

	return vector<Node*>{root1, root2};
}

void Root::Root_Cleaning() {
	count_words = 0;
	words.clear();
	links.clear();
}

bool Root::Find_word(string str) {
	if (words.size() == 0) {
		return false;
	}
	if (links.size() == 0) {
		for (auto word : words) {
			if (word == str)
				return true;
		}
	}
	else {
		if (links.size() == 0) {
			for (auto word : words) {
				if (word == str)
					return true;
			}
		}
		else {
			Node* current_link = this;
			while (true) {
				if (Leaf* buf_leaf = dynamic_cast<Leaf*>(current_link))
					break;
				vector<string> keys = current_link->Get_Words();
				int index = 0;
				for (int i = 0; i < keys.size(); i++) {
					if (str >= keys[i])
						index = i + 1;
				}
				current_link = current_link->Get_Links()[index];
			}
			for (auto word : current_link->Get_Words()) {
				if (str == word)
					return true;
			}
			//если не нашлось на всякий проверим брата справа, вдруг он там
			if (current_link->Get_Links().size() > 0) {
				current_link = current_link->Get_Links()[0];
				for (auto word : current_link->Get_Words()) {
					if (str == word)
						return true;
				}
			}

		}
	}
	return false;
}


void Root::Add_word(string str) {
	str = toDown(str);
	if (!Find_word(str))
	if (links.size() == 0) {//первый случай
		if (words.size() == 0) {
			words.push_back(str);
			count_words++;
		}
		else {
			bool flag = true;
			for (int i = 0; i < words.size(); i++) {
				if (str < words[i]) {
					words.insert(words.begin() + i, str);
					flag = false;
					count_words++;
					break;
				}
			}
			if (flag) {
				words.push_back(str);
				count_words++;
			}
		}
	}
	else { //остальные случаи
		Node* current_link = this;
		Node* previous_link = nullptr;

		//проходит по дереву до листа
		while (true) {
			if (Leaf* buf_leaf = dynamic_cast<Leaf*>(current_link)) 
				break;
			vector<string> keys = current_link->Get_Words();
			int index = 0;
			for (int i = 0; i < keys.size(); i++) {
				if (str >= keys[i])
					index = i + 1;
			}
			previous_link = current_link;
			current_link = current_link->Get_Links()[index];
		}

		if (current_link->Get_Count_words() < 2 * DEGREE - 1) {
			current_link->Add_word(str);
		}
			//сейчас будет подниматься если происходит переполнение
		else {

			while (true) {
				if (current_link == nullptr)
					break;
				if (Leaf* buf_leaf = dynamic_cast<Leaf*>(current_link)) {
					if (current_link->Get_Count_words() < 2 * DEGREE-1)
						break;
					vector<Node*> splited_leaf = current_link->Splitting(str);
					previous_link->Add_word(current_link->Get_Words()[2], splited_leaf);
					splited_leaf[0]->Set_parent_link(previous_link);
					splited_leaf[1]->Set_parent_link(previous_link);

					if (buf_leaf->Get_brother() != nullptr) {
						buf_leaf->Get_brother()->Clear_Links();
						buf_leaf->Get_brother()->Set_Links(splited_leaf[0]);
						if (Leaf* leaf = dynamic_cast<Leaf*>(splited_leaf[0])) {
							leaf->Set_brother(buf_leaf);
						}
					}
					if (buf_leaf->Get_Links().size() > 0) {
						splited_leaf[1]->Set_Links(buf_leaf->Get_Links()[0]);
						if (Leaf* leaf = dynamic_cast<Leaf*>(buf_leaf->Get_Links()[0])) {
							leaf->Set_brother(splited_leaf[1]);
						}
					}
				}
				if (InnerNode* buf_node = dynamic_cast<InnerNode*>(current_link)) {
					if (current_link->Get_Count_words() < 2 * DEGREE)
						break;
					vector<Node*> splited_node = current_link->Splitting(str);
					previous_link->Add_word(current_link->Get_Words()[2], splited_node);
					splited_node[0]->Set_parent_link(previous_link);
					splited_node[1]->Set_parent_link(previous_link);
				}
				if (Root* buf_root = dynamic_cast<Root*>(current_link)) {
					if (current_link->Get_Count_words() < 2 * DEGREE)
						break;
					vector<Node*> splited_root = current_link->Splitting(str);
					auto actual_key = current_link->Get_Words()[2];
					buf_root->Root_Cleaning();
					current_link->Add_word(actual_key, splited_root);
					splited_root[0]->Set_parent_link(current_link);
					splited_root[1]->Set_parent_link(current_link);
				}
				if (previous_link != nullptr) {
					//delete current_link;
					current_link = previous_link;
					previous_link = current_link->Get_Parent_link();
				}
				
			}
		}
	}
}


void Root::Set_New_Root(vector<string> actual_words) {
	if (this->links.size() == 0) {
		this->Add_word(actual_words[2]);

		Leaf* leaf1 = new Leaf;
		leaf1->Add_word(actual_words[0]);
		leaf1->Add_word(actual_words[1]);

		Leaf* leaf2 = new Leaf;
		leaf2->Add_word(actual_words[2]);
		leaf2->Add_word(actual_words[3]);

		this->links.push_back(leaf1);
		this->links.push_back(leaf2);
		
		leaf1->Set_parent_link(this);
		leaf2->Set_parent_link(this);

		leaf1->Set_Links(leaf2);
		leaf2->Set_brother(leaf1);
	}

}


Node* Root::Delete_word(string str) {
	Node* current_link = this;
	Node* previous_link = nullptr;

	if (links.size() == 0) {
		for (int i = 0; i < words.size(); i++) {
			if (str == words[i]) {
				words.erase(words.begin() + i);
				count_words--;
				return current_link;
			}
		}
	}
	//проходит по дереву до листа
	while (true) {
		if (Leaf* buf_leaf = dynamic_cast<Leaf*>(current_link))
			break;
		vector<string> keys = current_link->Get_Words();
		int index = 0;
		for (int i = 0; i < keys.size(); i++) {
			if (str >= keys[i])
				index = i + 1;
		}
		previous_link = current_link;
		current_link = current_link->Get_Links()[index];
	}
	////еще разобрать ситуацию когда удаляется в корне, а не в листе
	if (Leaf* leaf = dynamic_cast<Leaf*>(current_link)) {
		leaf->Delete_word(str);
	}
	return current_link;
}


void Root::Delete_word(Node* node) {
	for (int i = 0; i < links.size(); i++) {
		if (links[i] == node) {
			links.erase(links.begin() + i);
			words.pop_back();
			count_words--;
			break;
		}
	}
}


void Root::Rebalancing(Node* node) {
	if (!node) return;
	if (Leaf* leaf = dynamic_cast<Leaf*>(node)) {
		return;
	}
	if (InnerNode* inner_node = dynamic_cast<InnerNode*>(node->Get_Links()[0])) { 
		auto current_links = node->Get_Links(); 
		int count_links = node->Get_Links().size();
		vector<Node*> new_links;

		for (auto link : current_links) {
			auto children_links = link->Get_Links();
			for (auto child_link : children_links) {
				new_links.push_back(child_link);
			}
		}

		if (new_links.size() < 4 && current_links.size()==2) { //когда остается два узла (1 ребенок и 2 ребенка) то нужно их соеденить с узлом-родителем ( в родителе будет 3 ссылки на детей)
			node->Clear_Links();
			node->Set_Links(new_links);
			for (auto link : new_links) {
				link->Set_parent_link(node);
			}
			for (auto child : node->Get_Links()) {
				Rebalancing(child);
			}
		}
		else {

			int count = new_links.size() / count_links;
			if (count > 1) {
				for (int i = 0; i < current_links.size(); i++) {
					current_links[i]->Clear_Links();
					if (i != current_links.size() - 1) {
						for (int ii = 0; ii < count; ii++) {
							current_links[i]->Set_Links(new_links[0]);
							new_links[0]->Set_parent_link(current_links[i]);// поправляем ссылку на родителя
							new_links.erase(new_links.begin());
						}
					}
					else {
						current_links[i]->Set_Links(new_links);
						for (auto link : new_links) {
							link->Set_parent_link(current_links[i]);
						}
					}
				}
			}
			else {//если остается одна ссылка в каком то из узлов -> нужно убрать одну ссылку из родителя
				count++;
				for (int i = 0; i < current_links.size() - 1; i++) {
					current_links[i]->Clear_Links();
					if (i != current_links.size() - 2) {
						for (int ii = 0; ii < count; ii++) {
							current_links[i]->Set_Links(new_links[0]);
							new_links[0]->Set_parent_link(current_links[i]);// поправляем ссылку на родителя
							new_links.erase(new_links.begin());
						}
					}
					else {
						current_links[i]->Set_Links(new_links);
						for (auto link : new_links) {
							link->Set_parent_link(current_links[i]);
						}
					}
				}
				links.pop_back();
			}

			for (auto child : current_links) {
				Rebalancing(child);
			}
		}
	}

	else if (Root* root = dynamic_cast<Root*>(node)) {
		if (node->Get_Links().size() == 1) {
			auto current_links = node->Get_Links();
			vector<string> new_words;
			for (auto link : current_links) {
				for (auto word : link->Get_Words()) {
					new_words.push_back(word);
				}
			}
			node->Clear_word();
			node->Set_Words(new_words);
			node->Clear_Links();
		}
		for (auto child : root->Get_Links()) {
			Rebalancing(child);
		}
	}
}



void Root::Rebalancing_keys(Node* node) {
	if (!node) return;

	if (Root* root= dynamic_cast<Root*>(node)) {
		if (root->Get_Links().size() > 1) {
			auto links = node->Get_Links();
			vector<string> new_words;
			
			for (int i = 1; i < links.size(); i++) {
				Node* current_link = links[i];
				while (true) {
					if (Leaf* leaf = dynamic_cast<Leaf*>(current_link))
						break;
					current_link = current_link->Get_Links()[0];
				}
				new_words.push_back(current_link->Get_Words()[0]);
			}
			root->Clear_word();
			root->Set_Words(new_words);
			

			for (auto child : root->Get_Links()) {
				Rebalancing_keys(child);
			}
		}
	}

	if (InnerNode* inner_node= dynamic_cast<InnerNode*>(node)) {
		auto links = node->Get_Links();
		vector<string> new_words;
		
		for (int i = 1; i < links.size(); i++) {
			Node* current_link = links[i];
			while (true) {
				if (Leaf* leaf = dynamic_cast<Leaf*>(current_link))
					break;
				current_link = current_link->Get_Links()[0];
			}
			new_words.push_back(current_link->Get_Words()[0]);
		}
		node->Clear_word();
		node->Set_Words(new_words);

		for (auto child : inner_node->Get_Links()) {
			Rebalancing_keys(child);
		}
	}
}


///////////////////////////// InnerNode //////////////////////////////////////////

InnerNode::InnerNode() {}


vector<Node*> InnerNode::Splitting(string str) {
	InnerNode* node1 = new InnerNode;
	node1->Add_word(words[0]);
	node1->Add_word(words[1]);

	InnerNode* node2 = new InnerNode;
	node2->Add_word(words[3]);

	if (links.size() != 0) {
		node1->Set_Links(vector<Node*>{links[0], links[1], links[2]});
		node2->Set_Links(vector<Node*>{links[3], links[4]});
	}
	return vector<Node*>{node1, node2};
}



void InnerNode::Delete_word(Node* node) {
	for (int i = 0; i < links.size(); i++) {
		if (links[i] == node) {
			links.erase(links.begin() + i);
			break;
		}
	}
}


///////////////////////////// Leaf ///////////////////////////////////////////////

Leaf::Leaf() {}


void Leaf::Add_word_without_key(string str) {
	if (words.size() == 0) {
		words.push_back(str);
		count_words++;
	}
	else {
		bool flag = true;
		for (int i = 0; i < words.size(); i++) {
			if (str < words[i]) {
				words.insert(words.begin() + i, str);
				count_words++;
				flag = false;
				break;
			}
		}
		if (flag) {
			words.push_back(str);
			count_words++;
		}
	}
}


void Leaf::Add_word(string str, vector<Node*> link) {
	this->Add_word(str);
}



void Leaf::new_link(Node* link) {
	vector<Node*> buf{ link };
	links=buf;
}


void Leaf::Set_parent_link(Node* link) {
	parent_link = link;
}


vector<Node*> Leaf::Splitting(string str) {
	bool flag = true;
	for (int i = 0; i < words.size(); i++) {
		if (str < words[i]) {
			words.insert(words.begin() + i, str);
			flag = false;
			count_words++;
			break;
		}
	}
	if (flag) {
		words.push_back(str);
		count_words++;
	}

	Leaf* leaf1 = new Leaf;
	leaf1->Add_word(words[0]);
	leaf1->Add_word(words[1]);

	Leaf* leaf2 = new Leaf;
	leaf2->Add_word(words[2]);
	leaf2->Add_word(words[3]);

	leaf1->Set_Links(leaf2);
	leaf2->Set_brother(leaf1);

	return vector<Node*>{leaf1, leaf2};
}


void Leaf::Set_brother(Node* br) {
	left_Brother = br;
}


void Leaf::Delete_word(string str) {
	for (int i = 0; i < words.size(); i++) {
		if (str == words[i]) {
			words.erase(words.begin() + i);
			count_words--;
			break;
		}
	}
}


bool Leaf::Is_Sibling(Node* node) {
	auto links = parent_link->Get_Links();
	for (auto link : links) {
		if (node == link) {
			return true;
		}
	}
	return false;
}


void Leaf::Delete_word(Node* node) {
	if (this == node) {
		
		if (left_Brother == nullptr) { //еслb он левый в дерево, то у след листа указатель на брата будет ноль
			if (Leaf* leaf = dynamic_cast<Leaf*>(links[0])) {
				leaf->Set_brother(nullptr);
			}
		}
		else if (links.size() == 0) {// если он правый в дереве, то у него в ссылках будет ноль ссылок
			left_Brother->Clear_Links();
		}
		else {

			left_Brother->Clear_Links();
			left_Brother->Set_Links(this->links[0]);

			if (Leaf* leaf = dynamic_cast<Leaf*>(links[0])) {
				leaf->Set_brother(this->left_Brother);
			}
		}
	}
}


Node* Leaf::Get_brother() {
	return left_Brother;
}


///////////////////////////// Tree ///////////////////////////////////////////////

Tree::Tree() {
	root = new Root;
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


void Tree::Add_word(string str) {

	if (root->Get_Links().size() == 0) {//первый случай

		if (root->Get_Count_words() < (2 * DEGREE - 1)) {
			root->Add_word(str);
		}
		else {
			auto actual_words = root->Get_Words();
			bool flag = true;
			for (int i = 0; i < actual_words.size(); i++) {
				if (str < actual_words[i]) {
					actual_words.insert(actual_words.begin() + i, str);
					flag = false;
					break;
				}
			}
			if (flag) {
				actual_words.push_back(str);
			}
			Root* new_root = new Root;
			delete root;
			root = new_root;
			root->Set_New_Root(actual_words);
			levels++;
		}
	}
	//остальные случаи
	else {
		root->Add_word(str);
		root->Rebalancing_keys(root);
	}
}


void Tree::Print() {
	printf("\n\tB+ tree\n");
	printf("\t-------\n\n");
	Print(root);
	printf("\n------------------------------------------------------------------------\n\n");
}


void Tree::Print(Node* node, int level) {
	if (!node) return;

	if (Leaf* leaf = dynamic_cast<Leaf*>(node)) {
		cout << setw(level * 4) << "";
		for (const auto& words : leaf->Get_Words()) {
			//for (const auto& word : words) {
			cout << words << "  ";
			//}
		}
		cout << endl;
	}
	else if (InnerNode* inner_node = dynamic_cast<InnerNode*>(node)) {
		cout << setw(level * 4) << "";
		for (const auto& words : inner_node->Get_Words()) {
			//for (const auto& word : words) {
			cout << words << "*  ";
			//}
		}
		cout << endl;
		for (auto child : inner_node->Get_Links()) {
			Print(child, level + 1);
		}
	}
	else if (Root* root = dynamic_cast<Root*>(node)) {
		if (root->Get_Links().size() == 0 && root->Get_Count_words() == 0) {
			cout << "Словарь пуст" << "  ";
		}
		else {
			cout << setw(level * 4) << "";

			for (const auto& words : root->Get_Words()) {
				if (root->Get_Links().size() > 1) {
					cout << words << "*  ";
				}
				else {
					cout << words << "  ";
				}
			}
		}
		cout << endl;
		for (auto child : root->Get_Links()) {
			Print(child, level + 1);
		}
	}

}


bool Tree::Find_word(string str) {
	if (root->Get_Links().size() == 0) {
		for (auto word : root->Get_Words()) {
			if (word == str)
				return true;
		}
	}
	else {
		Node* current_link = root;
		while (true) {
			if (Leaf* buf_leaf = dynamic_cast<Leaf*>(current_link))
				break;
			vector<string> keys = current_link->Get_Words();
			int index = 0;
			for (int i = 0; i < keys.size(); i++) {
				if (str >= keys[i])
					index = i + 1;
			}
			current_link = current_link->Get_Links()[index];
		}
		for (auto word : current_link->Get_Words()) {
			if (str == word)
				return true;
		}
		//если не нашлось на всякий проверим брата справа, вдруг он там
		if (current_link->Get_Links().size() > 0) {
			current_link = current_link->Get_Links()[0];
			for (auto word : current_link->Get_Words()) {
				if (str == word)
					return true;
			}
		}

	}
		return false;
}


void Tree::getKeys(Node* node, vector<string>& result) {
	if (!node) return;

	if (InnerNode* inner_node = dynamic_cast<InnerNode*>(node)) {
		for (auto word : inner_node->Get_Words()) {
			result.push_back(word);
		}
		for (auto child : inner_node->Get_Links()) {
			getKeys(child, result);
		}
	}
	else if (Root* root = dynamic_cast<Root*>(node)) {
		for (auto word : root->Get_Words()) {
			result.push_back(word);
		}
		for (auto child : root->Get_Links()) {
			getKeys(child, result);
		}
	}
}


bool Tree::is_a_conj(string str) {
	str = toDown(str);
	for (string word : conj) {
		if (str == word) {
			return true;
		}
	}
	return false;
}

void Tree::From_file(string str) {
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
				this->Add_word(word);
			}
		}

		printf("\nСлова из файла добавлены в словарь\n\n");
	}
}


void Tree::Delete_word(string str) {
	if (this->Find_word(str)) {
		auto edited_node = root->Delete_word(str);
		Rebalancing(edited_node);
	}
}


void Tree::Rebalancing(Node* edited_leaf) {
	if (Root* edited_root = dynamic_cast<Root*>(edited_leaf)) {
		return;
	}
	else {
		if (edited_leaf->Get_Count_words() == 0) {
			edited_leaf->Delete_word(edited_leaf);
			edited_leaf->Get_Parent_link()->Delete_word(edited_leaf);
		}

		Node * current_link = root;
		Node* previous_link = nullptr;
		Node* first_leaf = nullptr;
		vector<Node*> links;
		vector<string> new_words;

		//проходит по дереву до листа
		while (true) {
			if (Leaf* buf_leaf = dynamic_cast<Leaf*>(current_link))
				break;
			previous_link = current_link;
			current_link = current_link->Get_Links()[0];
		}
		while (true) {
			links.push_back(current_link);
			auto words = current_link->Get_Words();
			for (auto word : words) {
				new_words.push_back(word);
			}

			if (current_link->Get_Links().size() == 0)
				break;
			current_link = current_link->Get_Links()[0];
		}

		int count = new_words.size() / links.size();

		
		if (count > 1) {
			for (int i = 0; i < links.size(); i++) {
				links[i]->Clear_word();
				if (i != links.size() - 1) {
					for (int ii = 0; ii < count; ii++) {
						links[i]->Add_word(new_words[0]);
						new_words.erase(new_words.begin());
					}
				}
				else {
					links[i]->Set_Words(new_words);
				}
			}

		}
		else {//если остается одна ссылка в каком то из узлов -> нужно убрать однуссылку из родителя
			count++;

			if (new_words.size() < 4) {

			}
			for (int i = 0; i < links.size() - 1; i++) {
				links[i]->Clear_word();
				if (i != links.size() - 2) {
					for (int ii = 0; ii < count; ii++) {
						links[i]->Add_word(new_words[0]);
						new_words.erase(new_words.begin());
					}
				}
				else {
					links[i]->Clear_word();
					links[i]->Set_Words(new_words);
				}
		}
			Node* link_for_deleteing = links[links.size() - 1];
			link_for_deleteing->Get_Parent_link()->Delete_word(link_for_deleteing);
			links[links.size()-2]->Clear_Links();
			links.pop_back();
		
		}
		root->Rebalancing(root);
		root->Rebalancing_keys(root);
	}
}

void Tree::Delete_all() {
	Root* buf = new Root;
	root->Clear(root);
	root = buf;
}



void Tree::Menu() {
	while (true) {
		printf("\n--------------------- Словарь (В+ дерево)---------------------\n");
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
				std::cout << "Число вне диапазона! Пожалуйста, введите число от 0 до 7." << std::endl;
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
			if (root->Get_Count_words() == 0) {
				printf("\nСловарь пуст\n\n");
			}
			else {
				printf("\nСодержимое словаря:\n\n");
				Print(root);
				cout << endl;
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
					if (root->Get_Count_words() != 0) {
						if (!Find_word(word)) {
							if (!is_a_conj(word)) {
								this->Add_word(word);
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
					else {
						if (!is_a_conj(word)) {
							this->Add_word(word);
							printf("\n\tСлово '%s' добавлено в словарь\n\n", word.c_str());
						}
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
					if (root->Get_Count_words() != 0) {

						if (!this->Find_word(word)) {
							printf("\n\tСлово '%s' отсутствует\n\n", word.c_str());
						}
						else {
							this->Delete_word(word);
							printf("\n\tСлово '%s' удалено\n\n", word.c_str());
							//break;
						}
					}
					else {
						printf("\nСловарь пуст\n\n");
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
					if (root->Get_Count_words() != 0) {
						if (this->Find_word(word)) {
							printf("\n\tСлово '%s' присутствует в словаре\n\n", word.c_str());
						}
						else {
							printf("\n\tСлово '%s' отсутсвует в словаре\n\n", word.c_str());
						}
						//break;
					}
				else {
					printf("\nСловарь пуст\n\n");
				}
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


string toDown(string str) {
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