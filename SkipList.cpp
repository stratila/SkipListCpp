#include "SkipList.h"
#include <limits>
#include <string>
#include <vector>
#include <limits>
#include <stdexcept>
#include <random>
#include <iostream>
#include <ctime>

constexpr int max_level = 12;

struct SkipList::Node {
	Node(int k, const std::string& v, int lvl)
		:key{ k }, val{ v }, forward(lvl) {}

	int key;
	std::string val;
	std::vector<Node*> forward;

};

inline double rand_double()
{
	 static std::mt19937 gen(time(0)); //create gen once
	 std::uniform_real_distribution<> urd(0, 1);
	 return urd(gen);
}

int SkipList::random_level()
{
	static double probability = 0.50;
	int lvl = 0;
	//rand_double() returns double in range (0;1]
	while (rand_double() < probability && lvl <= llevel)
		++lvl;
	return lvl;
}

SkipList::SkipList() //default constructor is initialized level with max_level
	:SkipList(max_level) 
{}

SkipList::SkipList(int lvl)
	:llevel{ lvl }
{
	int nil_key = std::numeric_limits<int>::max(); //nil key greater than any legal key
	nil = new Node{ nil_key,"NIL",llevel };
	std::fill(nil->forward.begin(), nil->forward.end(), nullptr);

	int header_key = std::numeric_limits<int>::min();
	header = new Node{ header_key,"Header",llevel };
	std::fill(header->forward.begin(), header->forward.end(), nil); //all pointers of header node
																	//points to nil node
}



SkipList::~SkipList()
{
	Node* p = header;
	while (p){
		Node* temp = p;
		p = p->forward[0];
		delete temp;
	}
}

int SkipList::level() const
{
	return llevel;
}

std::string* SkipList::find(int search_key)
{
	Node* before_node = header;
	for (int level_number = llevel - 1; level_number >= 0; --level_number) {
		while (before_node->forward[level_number]->key < search_key) 
			before_node = before_node->forward[level_number];
	}
	//before_node < search_key <= before_node->forward[0]
	if (before_node->forward[0]->key == search_key)
		return &before_node->forward[0]->val;
	else return nullptr;
}

std::vector<SkipList::Node*> SkipList::fill_update(int key) const
{
	std::vector<Node*> v(llevel);
	Node* before_node = header;
	for (int level_number = llevel - 1; level_number >= 0; --level_number) { //find key and fill
		while (before_node->forward[level_number]->key < key)				//update vector
			before_node = before_node->forward[level_number];
		v[level_number] = before_node;
	}
	return v;
}

void SkipList::insert(int key, const std::string & val)
{
	std::vector<Node*> update = fill_update(key);
	
	if (update[0]->forward[0]->key == key) { //if key have been found 
		update[0]->forward[0]->val = val;	   //assign new value to found node
		return;
	} 
	else {
		int lvl = random_level();
		if (lvl > llevel-1) {
			++llevel;
			update.resize(llevel);

			header->forward.resize(llevel); //increase header levels
			header->forward[llevel - 1] = nil;
			nil->forward.resize(llevel); //increase nil levels
			nil->forward[llevel - 1] = nullptr;

			update[llevel - 1] = header;
			lvl = llevel-1;
		}
		std::cout << "Insert level: " << lvl << '\n';
		Node* new_node = new Node{ key,val,lvl+1 };
		for (int i = 0; i <= lvl; ++i) {
			new_node->forward[i] = update[i]->forward[i];
			update[i]->forward[i] = new_node;
		}
	}
}

void SkipList::erase(int key)
{
	std::vector<Node*> update = fill_update(key);
	Node* x = update[0]->forward[0];
	if (x->key == key) {
		for (int i = 0; i < x->forward.size(); ++i) 
			update[i]->forward[i] = x->forward[i];
		delete x;
	}
}

