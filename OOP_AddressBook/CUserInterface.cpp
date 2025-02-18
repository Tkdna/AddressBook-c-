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
	observers.push_back(observer);	//옵저버 포인터를 벡터의 끝에 저장
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
	//벡터 범위 탐색
	for (IObserver* observer : observers) {
		observer->Update(); // 각 옵저버의 Update() 함수 호출
	}
}

MY_MENU CUserInterface::PrintMenu(void) 
{
	MY_MENU input;

	system("cls");
	printf("[1]ADD\t[2]PRINT\t[3]SEARCH\t[4]REMOVE\t[5]EDIT\t[0]EXIT\n");
	printf("번호 입력: ");
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
	gets_s(name, sizeof(name));	//user.name만큼
	user.SetName(name);

	printf("phone: ");
	gets_s(phone, sizeof(phone));
	user.SetPhone(phone);

	//중복이면 계속 반복
	while (m_list.CheckDuplicate(user.GetPhone()) == 1) {
		printf("전화번호는 중복될 수 없습니다.\n");
		printf("phone: ");
		gets_s(phone, sizeof(phone));
		user.SetPhone(phone);
	}

	//삭제된 노드가져오기
	CMyNode* pTmp = m_list.FindRemovedNode();

	if (pTmp != NULL)
	{
		//삭제된 노드에다 오버라이트
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

	printf("전화번호: ");
	gets_s(phone, sizeof(phone));
	user.SetPhone(phone);

	CMyNode* pNode = m_list.SearchListByPhone(user.GetPhone());
	if (pNode == NULL) {
		printf("일치하는 전화번호가 존재하지 않습니다.\n");
		_getch();
		return;
	}
	else {
		m_list.RemoveData(pNode);
		printf("삭제 완료\n");
		_getch();
		NotifyObservers();
		return;
	}
}



void CUserInterface::EditUser()
{
	CUserData user;
	char phone[32];

	printf("전화번호: ");
	gets_s(phone, sizeof(phone));
	user.SetPhone(phone);

	CMyNode* pNode = m_list.SearchListByPhone(user.GetPhone());
	if (pNode == NULL) {
		printf("일치하는 전화번호가 존재하지 않습니다.\n");
		_getch();
		return;
	}
	else {
		//printf("name: %s, phone: %s, age: %d\n", user.name, user.phone, user.age);
		printf("해당 데이터를 수정 하시겠습니까? (y/n)");
		char ch = _getch();
		putchar('\n');
		if (ch == 'y' || ch == 'Y') {
			printf("New Phone Number: ");
			gets_s(phone, sizeof(phone));
			user.SetPhone(phone);

			while (m_list.CheckDuplicate(user.GetPhone()) == 1) {
				printf("전화번호는 중복될 수 없습니다.\n");
				printf("New Phone Number: ");

				gets_s(phone, sizeof(phone));
				user.SetPhone(phone);
			}

			m_list.EditPhoneNumber(pNode, user.GetPhone());
			printf("수정 완료\n");
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

	printf("쿼리입력: ");
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
				printf("쿼리 형식에 맞게 입력해주세요.");
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
				printf("쿼리 형식에 맞게 입력해주세요.");
				_getch();
				return;
			}
		}
	}

	//이름만 입력한경우
	if (name[0] != '\0' && phone[0] == '\0') {
		LARGE_INTEGER start, end, frequency;
		QueryPerformanceFrequency(&frequency);

		// 시작 시간 측정
		QueryPerformanceCounter(&start);

		m_list.SearchDataByName(name, NULL, NULL);

		// 종료 시간 측정
		QueryPerformanceCounter(&end);

		// 시간 차이 계산 (나노초 단위로 변환)
		LONGLONG elapsed = end.QuadPart - start.QuadPart;
		double nanoseconds = (double)elapsed * 1e9 / frequency.QuadPart;

		printf("수행속도: %.0f ns\n", nanoseconds);
	}
	else if (name[0] != '\0' && phone[0] != '\0') {			//이름, 번호 모두 입력한경우
		if (strcmp(oper, "and") == 0 || strcmp(oper, "or") == 0) {
			LARGE_INTEGER start, end, frequency;
			QueryPerformanceFrequency(&frequency);

			// 시작 시간 측정
			QueryPerformanceCounter(&start);

			m_list.SearchDataByName(name, phone, oper);

			// 종료 시간 측정
			QueryPerformanceCounter(&end);

			// 시간 차이 계산 (나노초 단위로 변환)
			LONGLONG elapsed = end.QuadPart - start.QuadPart;
			double nanoseconds = (double)elapsed * 1e9 / frequency.QuadPart;

			printf("수행속도: %.0f ns\n", nanoseconds);
		}
		else {
			printf("쿼리형식에 맞게 입력해주세요.");
		}
	}
	else {
		printf("쿼리형식에 맞게 입력해주세요.");
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
