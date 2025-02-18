#pragma once

#include "CLinkedList.h"
#include "IObserver.h"
#include <vector>

typedef enum { EXIT, ADD, PRINT, SEARCH, REMOVE, EDIT } MY_MENU;

class CUserInterface
{
private:
	CLinkedList &m_list;
	std::vector<IObserver*> observers; //������ ��ü�� �ּҸ� ���� ����
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

