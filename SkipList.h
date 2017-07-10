#pragma once
#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <string>
#include <vector>
class SkipList {
	struct Node;
	Node* header; 
	Node* nil;
	int llevel; //level of list
public:
	SkipList();

	SkipList(int lvl);
	
	/*Found a *string value with key.
	In case nothing to find returns nullptr*/
	std::string* find(int key); 

	/*Inserts new node with new value and key
	  if node with the same key already exist – 
	  update value.*/
	void insert(int key, const std::string& val);

	/*Erase node with the same key*/
	void erase(int key);

	/*Max level of list*/
	int level() const;

	/*Destructor*/
	~SkipList();

private:
	/*provides random levels for inseration*/
	int random_level(); 

	/*Fills vector<Node*> update with values before supposed 
	key node for succssesful insetion and deleting (erase)*/
	std::vector<Node*> fill_update(int key) const;
};

#endif SKIPLIST_H
