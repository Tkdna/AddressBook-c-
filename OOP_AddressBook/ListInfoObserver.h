#pragma once

#include "IObserver.h"
#include "CLinkedList.h"
#include <iostream>
#include <conio.h>

using namespace std;

class ListInfoObserver : public IObserver
{
private:
	CLinkedList& m_list;
public:
	ListInfoObserver(CLinkedList& db) : m_list(db) {}
	~ListInfoObserver() {}

	void Update() override
	{
		std::cout << "Node Count: " << m_list.GetListCount() << std::endl;
		std::cout << "Removed Node Count: " << m_list.GetRemovedNode() << std::endl;
		_getch();
	}
};

