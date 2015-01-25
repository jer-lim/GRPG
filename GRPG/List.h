#ifndef LIST_H
#define LIST_H

#include <string>
#include <iostream>

using namespace std;

template<class ItemType>
class List
{
private:
	struct Node
	{
		ItemType item;
		Node *next;
	};

	Node *firstNode;
	int size;
public:
	List();
	bool add(ItemType newItem);
	bool add(int index, ItemType newItem);
	ItemType get(int index);
	void remove(int index);
	void clear();
	int getLength();
	bool isEmpty();
	void print();
};

#endif