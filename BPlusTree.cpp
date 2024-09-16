#include "BPlusTree.h"

////////////////////////////// Node //////////////////////////////////////////////

Node::Node() {
	this->count_words = 0;
	this->DEGREE = 2;
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


void Root::Add_word(string str, vector<Node*> link) {
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
}


void Root::Root_Cleaning() {
	count_words = 0;
	words.clear();
	links.clear();
}


void Root::Add_word(string str) {
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
		while (current_link->Get_Links()[0] != nullptr) {
			vector<string> keys = current_link->Get_Words();
			int index = 0;
			for (int i = 0; i < keys.size(); i++) {
				if (str >= keys[0])
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
					delete current_link;
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
	}

}


int Root::Get_Count_words() {
	return count_words;
}


vector<string> Root::Get_Words() {
	return words;
}


vector<Node*> Root::Get_Links() {
	return links;
}


void Root::Set_Links(Node* link) {
	links.push_back(link);
}


void Root::Set_Links(vector<Node*> link) {
	links = link;
}


void Root::Set_parent_link(Node* link) {
	parent_link = link;
}


Node* Root::Get_Parent_link() {
	return this->parent_link;
}


///////////////////////////// InnerNode //////////////////////////////////////////

InnerNode::InnerNode() {}

void InnerNode::Add_word(string str) {
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


void InnerNode::Add_word(string str, vector<Node*> link) {
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
				count_words++;
				links.erase(links.begin() + i);
				links.insert(links.begin() + i, link[0]);
				links.insert(links.begin() + i +1, link[1]);
				flag = false;
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
}


int InnerNode::Get_Count_words() {
	return count_words;
}


vector<string> InnerNode::Get_Words() {
	return words;
}


vector<Node*> InnerNode::Get_Links() {
	return links;
}


void InnerNode::Set_Links(Node* link) {
	links.push_back(link);
}


void InnerNode::Set_Links(vector<Node*> link) {
	links = link;
}



void InnerNode::Set_parent_link(Node* link) {
	parent_link = link;
}


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


Node* InnerNode::Get_Parent_link() {
	return this->parent_link;
}


///////////////////////////// Leaf ///////////////////////////////////////////////

Leaf::Leaf() {}


void Leaf::Add_word(string str) {

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


int Leaf::Get_Count_words() {
	return count_words;
}


vector<string> Leaf::Get_Words() {
	return words;
}


vector<Node*> Leaf::Get_Links() {
	return vector<Node*>(count_words+1, nullptr);////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

void Leaf::Set_Links(Node* link) {
	links.push_back(link);
}

void Leaf::Set_Links(vector<Node*> link) {
	links = link;
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

	return vector<Node*>{leaf1, leaf2};
}


Node* Leaf::Get_Parent_link() {
	return this->parent_link;
}



///////////////////////////// Tree ///////////////////////////////////////////////

Tree::Tree() {
	root = new Root;
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
	else {//остальные случаи
		root->Add_word(str);
	}
}


void Tree::Print() {
	printf("\n\tB+ tree\n");
	printf("\t-------\n\n");
	Print(root);
}


void Tree::Print(Node* node, int level) {
	if (!node) return;

	if (Leaf* leaf = dynamic_cast<Leaf*>(node)) {
		cout << setw(level * 4) << "";
		for (const auto& words : leaf->Get_Words()) {
			for (const auto& word : words) {
				cout << word << "  ";
			}
		}
		cout << endl;
	}
	else if (InnerNode* inner_node = dynamic_cast<InnerNode*>(node)) {
		cout << setw(level * 4) << "";
		for (const auto& words : inner_node->Get_Words()) {
			for (const auto& word : words) {
				cout << word << "*  ";
			}
		}
		cout << endl;
		for (auto child : inner_node->Get_Links()) {
			Print(child, level + 1);
		}
	}
	else if (Root* root = dynamic_cast<Root*>(node)) {
		cout << setw(level * 4) << "";
		for (const auto& words : root->Get_Words()) {
			for (const auto& word : words) {
				cout << word << "*  ";
			}
		}
		cout << endl;
		for (auto child : root->Get_Links()) {
			Print(child, level + 1);
		}
	}
}
