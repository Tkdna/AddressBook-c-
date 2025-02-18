#pragma once

#include "CLinkedList.h"
#include "IObserver.h"
#include <vector>

typedef enum { EXIT, ADD, PRINT, SEARCH, REMOVE, EDIT } MY_MENU;

class CUserInterface
{
private:
	CLinkedList &m_list;
	std::vector<IObserver*> observers; //옵저버 객체의 주소를 담을 벡터
public:
	CUserInterface(CLinkedList& db);
	~CUserInterface();

	void AddObserver(IObserver* observer);
	void RemoveObserver(IObserver* observer);
	void NotifyObservers();
	
	MY_MENU PrintMenu(void);
	void PrintList();
	void AddNewUser();
	void RemoveUser();
	void EditUser();
	void SearchUser();
	int EventLoopRun();
};

