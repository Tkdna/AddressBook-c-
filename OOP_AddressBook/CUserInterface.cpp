#include "CUserInterface.h"
#include "CLinkedList.h"
#include "CUserData.h"
#include "CMyNode.h"
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <iostream>

CUserInterface::CUserInterface(CLinkedList& db) : m_list(db)
{
	
}

CUserInterface::~CUserInterface() {

}

void CUserInterface::AddObserver(IObserver* observer) {
	observers.push_back(observer);	//������ �����͸� ������ ���� ����
}

void CUserInterface::RemoveObserver(IObserver* observer)
{
	auto it = std::find(observers.begin(), observers.end(), observer);
	if (it != observers.end()) {
		observers.erase(it);
	}
}

void CUserInterface::NotifyObservers()
{
	//���� ���� Ž��
	for (IObserver* observer : observers) {
		observer->Update(); // �� �������� Update() �Լ� ȣ��
	}
}

MY_MENU CUserInterface::PrintMenu(void) 
{
	MY_MENU input;

	system("cls");
	printf("[1]ADD\t[2]PRINT\t[3]SEARCH\t[4]REMOVE\t[5]EDIT\t[0]EXIT\n");
	printf("��ȣ �Է�: ");
	scanf_s("%d%*c", &input);

	return input;
}

void CUserInterface::PrintList()
{
	/*CMyNode* pTmp = m_list.GetHeadNode();
	while (pTmp != NULL)
	{
		printf("[%p] %s, removed: %d, offset: %d ",
			pTmp, pTmp->GetKey(), pTmp->GetbRemoved(), pTmp->GetOffset());

		printf("[%p]\n", pTmp->GetNext());
		pTmp = pTmp->GetNext();

	}
	putchar('\n');

	std::cout << "List Count: " << m_list.GetListCount() << std::endl;
	std::cout << "Removed Node Count: " << m_list.GetRemovedNode() << std::endl;
	_getch();*/

	NotifyObservers();
}

void CUserInterface::AddNewUser()
{
	CUserData user;
	//memset(&user, 0, sizeof(USERDATA));

	int age;
	char name[32];
	char phone[32];

	printf("age: ");
	scanf_s("%d%*c", &age);
	user.SetAge(age);

	printf("name: ");
	gets_s(name, sizeof(name));	//user.name��ŭ
	user.SetName(name);

	printf("phone: ");
	gets_s(phone, sizeof(phone));
	user.SetPhone(phone);

	//�ߺ��̸� ��� �ݺ�
	while (m_list.CheckDuplicate(user.GetPhone()) == 1) {
		printf("��ȭ��ȣ�� �ߺ��� �� �����ϴ�.\n");
		printf("phone: ");
		gets_s(phone, sizeof(phone));
		user.SetPhone(phone);
	}

	//������ ��尡������
	CMyNode* pTmp = m_list.FindRemovedNode();

	if (pTmp != NULL)
	{
		//������ ��忡�� ��������Ʈ
		m_list.OverwirteNewNode(user.GetPhone(), &user, sizeof(user), false, 0, pTmp);
		m_list.SaveNewNodeToFile(user, pTmp->GetOffset());
		free(pTmp);
		NotifyObservers();
		return;
	}
	else {
		int MaxOffset = m_list.getMaxOffset() + 1;
		m_list.AddNewNode(user.GetPhone(), &user, sizeof(user), false, MaxOffset);
		m_list.SaveNewNodeToFile(user, MaxOffset);
	}

	NotifyObservers();
	return;
}


void CUserInterface::RemoveUser()
{
	CUserData user;
	char phone[32];

	printf("��ȭ��ȣ: ");
	gets_s(phone, sizeof(phone));
	user.SetPhone(phone);

	CMyNode* pNode = m_list.SearchListByPhone(user.GetPhone());
	if (pNode == NULL) {
		printf("��ġ�ϴ� ��ȭ��ȣ�� �������� �ʽ��ϴ�.\n");
		_getch();
		return;
	}
	else {
		m_list.RemoveData(pNode);
		printf("���� �Ϸ�\n");
		_getch();
		NotifyObservers();
		return;
	}
}



void CUserInterface::EditUser()
{
	CUserData user;
	char phone[32];

	printf("��ȭ��ȣ: ");
	gets_s(phone, sizeof(phone));
	user.SetPhone(phone);

	CMyNode* pNode = m_list.SearchListByPhone(user.GetPhone());
	if (pNode == NULL) {
		printf("��ġ�ϴ� ��ȭ��ȣ�� �������� �ʽ��ϴ�.\n");
		_getch();
		return;
	}
	else {
		//printf("name: %s, phone: %s, age: %d\n", user.name, user.phone, user.age);
		printf("�ش� �����͸� ���� �Ͻðڽ��ϱ�? (y/n)");
		char ch = _getch();
		putchar('\n');
		if (ch == 'y' || ch == 'Y') {
			printf("New Phone Number: ");
			gets_s(phone, sizeof(phone));
			user.SetPhone(phone);

			while (m_list.CheckDuplicate(user.GetPhone()) == 1) {
				printf("��ȭ��ȣ�� �ߺ��� �� �����ϴ�.\n");
				printf("New Phone Number: ");

				gets_s(phone, sizeof(phone));
				user.SetPhone(phone);
			}

			m_list.EditPhoneNumber(pNode, user.GetPhone());
			printf("���� �Ϸ�\n");
			NotifyObservers();
			_getch();
		}
		return;
	}
}



void CUserInterface::SearchUser()
{
	char query[60];
	const char delimeters[] = " ='";
	char* context = NULL;
	char* tokens[5];
	int tokenCount = 0;

	printf("�����Է�: ");
	gets_s(query, sizeof(query));

	char* token = strtok_s(query, delimeters, &context);

	while (token != NULL)
	{
		tokens[tokenCount] = token;
		tokenCount++;
		token = strtok_s(NULL, delimeters, &context);
	}

	char name[32] = { 0 };
	char oper[4] = { 0 };
	char phone[32] = { 0 };

	for (int i = 0; i < tokenCount; i++) {
		if (strcmp(tokens[i], "name") == 0) {
			if (i + 1 < tokenCount) {
				strcpy(name, tokens[i + 1]);
			}
			else {
				printf("���� ���Ŀ� �°� �Է����ּ���.");
				_getch();
				return;
			}
		}
		else if (strcmp(tokens[i], "and") == 0 || strcmp(tokens[i], "or") == 0) {
			strcpy(oper, tokens[i]);
		}
		else if (strcmp(tokens[i], "phone") == 0) {
			if (i + 1 < tokenCount) {
				strcpy(phone, tokens[i + 1]);
			}
			else {
				printf("���� ���Ŀ� �°� �Է����ּ���.");
				_getch();
				return;
			}
		}
	}

	//�̸��� �Է��Ѱ��
	if (name[0] != '\0' && phone[0] == '\0') {
		LARGE_INTEGER start, end, frequency;
		QueryPerformanceFrequency(&frequency);

		// ���� �ð� ����
		QueryPerformanceCounter(&start);

		m_list.SearchDataByName(name, NULL, NULL);

		// ���� �ð� ����
		QueryPerformanceCounter(&end);

		// �ð� ���� ��� (������ ������ ��ȯ)
		LONGLONG elapsed = end.QuadPart - start.QuadPart;
		double nanoseconds = (double)elapsed * 1e9 / frequency.QuadPart;

		printf("����ӵ�: %.0f ns\n", nanoseconds);
	}
	else if (name[0] != '\0' && phone[0] != '\0') {			//�̸�, ��ȣ ��� �Է��Ѱ��
		if (strcmp(oper, "and") == 0 || strcmp(oper, "or") == 0) {
			LARGE_INTEGER start, end, frequency;
			QueryPerformanceFrequency(&frequency);

			// ���� �ð� ����
			QueryPerformanceCounter(&start);

			m_list.SearchDataByName(name, phone, oper);

			// ���� �ð� ����
			QueryPerformanceCounter(&end);

			// �ð� ���� ��� (������ ������ ��ȯ)
			LONGLONG elapsed = end.QuadPart - start.QuadPart;
			double nanoseconds = (double)elapsed * 1e9 / frequency.QuadPart;

			printf("����ӵ�: %.0f ns\n", nanoseconds);
		}
		else {
			printf("�������Ŀ� �°� �Է����ּ���.");
		}
	}
	else {
		printf("�������Ŀ� �°� �Է����ּ���.");
	}

	_getch();

	return;
}



int CUserInterface::EventLoopRun()
{
	
	MY_MENU menu;
	while ((menu = PrintMenu()) != 0)
	{
		switch (menu)
		{
		case ADD:
			AddNewUser();
			break;
		case PRINT:
			PrintList();
			break;
		case SEARCH:
			SearchUser();
			break;
		case REMOVE:
			RemoveUser();
			break;
		case EDIT:
			EditUser();
			break;
		default:
			break;
		}
	}

	puts("Bye~!");

	return 0;
}
