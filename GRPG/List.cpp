#include <iostream>
#include <string>
#include "List.h"
#include <sstream>

using namespace std;

template<class ItemType>
List<ItemType>::List()
{
	firstNode = NULL;
	size = 0;
}

template<class ItemType>
bool List<ItemType>::add(ItemType newItem)
{
	return add(size+1, newItem);
}

template<class ItemType>
bool List<ItemType>::add(int location, ItemType newItem)
{
	if(location <= 0 || location > size + 1)
	{
		return false;
	}
	Node *newNode = new Node();
	newNode->item = newItem;
	newNode->next = NULL;
	if(firstNode == NULL)
	{
		//Location is definitely 1
		firstNode = newNode;
	}
	else
	{
		Node *nodeToChange = firstNode;
		while(--location > 1)
		{
			nodeToChange = nodeToChange->next;
		}
		newNode->next = nodeToChange->next;
		nodeToChange->next = newNode;
	}
	size++;
	return true;
}

template<class ItemType>
ItemType List<ItemType>::get(int location)
{
	if(location <= 0 || location > size+1)
	{
		return NULL;
	}
	Node *nodeToReturn = firstNode;
	while(--location > 0)
	{
		nodeToReturn = nodeToReturn->next;
	}
	return nodeToReturn->item;
}

template<class ItemType>
void List<ItemType>::remove(int location)
{
	if(location <= 0 || location > size)
	{
		return;
	}
	Node *nodeToChange = firstNode;
	if(location == 1)
	{
		Node *temp = firstNode;
		firstNode = nodeToChange->next;
		delete temp;
	}
	else
	{
		while(--location > 1)
		{
			//This is the node BEFORE the node we want to remove
			nodeToChange = nodeToChange->next;
		}
		Node *temp = nodeToChange->next;
		nodeToChange->next = nodeToChange->next->next;
		delete temp;
	}
	size--;
	return;
}

template<class ItemType>
void List<ItemType>::clear()
{
	while(!isEmpty())
	{
		remove(1);
	}
}

template<class ItemType>
int List<ItemType>::getLength()
{
	return size;
}

template<class ItemType>
bool List<ItemType>::isEmpty()
{
	return size == 0;
}

template<class ItemType>
void List<ItemType>::print()
{
	stringstream ss;
	ss << "Printing list with size " << getLength() << "..." << endl;
	for(int i = 1; i <= getLength(); i++)
	{
		ss << i << ": " << get(i) << endl;
	}
	ss << "Printing completed." << endl;
	OutputDebugString(ss.str().c_str());
}