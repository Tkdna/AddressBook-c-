#include "CLinkedList.h"
#include "CUserInterface.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>


CLinkedList::CLinkedList() {
	InitList();
	LoadListFromFile();
}

CLinkedList::~CLinkedList() {
	ReleaseList();
}

void CLinkedList::InitList()
{
	//ReleaseList();
	m_HeadNode.SetPrev(nullptr);
	m_HeadNode.SetNext(&m_TailNode);
	m_TailNode.SetPrev(&m_HeadNode);
	m_TailNode.SetNext(nullptr);
	m_listCount = 0;
}


int CLinkedList::IsEmpty()
{
	if (m_HeadNode.GetNext() == &m_TailNode ||
		m_HeadNode.GetNext() == nullptr)
		return 1;

	return 0;
}


void CLinkedList::ReleaseList()
{
	if (IsEmpty() == 1)
		return;

	CMyNode* pTmp = (CMyNode*)m_HeadNode.GetNext();
	CMyNode* pDelete = { 0 };

	while (pTmp != &m_TailNode)
	{
		pDelete = pTmp;
		pTmp = (CMyNode*)pTmp->GetNext();

		if (pDelete->GetpDataCache() != nullptr)
			delete pDelete->GetpDataCache();

		delete pDelete->GetKey();
		delete pDelete;
	}
	m_HeadNode.SetNext(&m_TailNode);
	m_TailNode.SetPrev(&m_HeadNode);
	m_listCount = 0;
	return;
}


int CLinkedList::LoadListFromFile()
{
	//ReleaseList();
	FILE* fp = NULL;
	fopen_s(&fp, "listdata.dat", "rb");
	if (fp == NULL)
		return 0;

	CUserData user;
	unsigned int offset = 0;

	while (fread(&user, sizeof(CUserData), 1, fp) != 0)
	{
		//0초기화 된 것들은 삭제된 것들이므로 bRemoved true로 설정
		if (user.GetAge() == 0 && strcmp("", user.GetName()) == 0 && strcmp("", user.GetPhone()) == 0) {
			AddNewNode(user.GetPhone(), NULL, sizeof(CUserData), true, offset);
			memset(&user, 0, sizeof(CUserData));
			++offset;
			++m_removedNode;
			continue;
		}

		AddNewNode(user.GetPhone(), NULL, sizeof(CUserData), false, offset);
		memset(&user, 0, sizeof(CUserData));
		++offset;
	}

	fclose(fp);
	return 1;
}


void CLinkedList::AddNewNode(const char* pszKey, const void* pData, 
	unsigned int sizeData, bool bRemoved, unsigned int offset)
{
	CMyNode* pNewNode = new CMyNode;	//연결리스트 노드 동적할당
	int keylen = strlen(pszKey);
	pNewNode->SetKey(pszKey, keylen);			//키를 담을 메모리 동적할당
	//strcpy_s(pNewNode->pszKey, keylen + 1, pszKey);

	if (pData != NULL)
	{
		CUserData* pNewData = new CUserData;		//데이터 노드 동적할당
		memcpy(pNewData, pData, sizeData);			//
		pNewNode->SetpDataCache(pNewData);
		pNewNode->SetSizeData(sizeData);
	}

	//pNewNode->bNew = bNew;
	pNewNode->SetbRemoved(bRemoved);
	pNewNode->SetOffset(offset);

	//연결리스트 맨뒤에 붙임
	CMyNode* pPrevNode = m_TailNode.GetPrev();

	if (pPrevNode == nullptr) { // 리스트가 비어있는 경우
		pNewNode->SetPrev(&m_HeadNode);
		pNewNode->SetNext(&m_TailNode);
		m_HeadNode.SetNext(pNewNode);
		m_TailNode.SetPrev(pNewNode);
	}
	else { // 리스트에 이미 노드가 있는 경우
		pNewNode->SetNext(&m_TailNode);
		pNewNode->SetPrev(pPrevNode);
		pPrevNode->SetNext(pNewNode);
		m_TailNode.SetPrev(pNewNode);
	}

	++m_listCount;
}


void CLinkedList::OverwirteNewNode(const char* pszKey, const void* pData, 
	unsigned int sizeData, bool bRemoved, unsigned int offset, CMyNode* pNode)
{
	CMyNode* pNewNode = new CMyNode;
	int keylen = strlen(pszKey);
	pNewNode->SetKey(pszKey, keylen);
	//free(pNode->pszKey);		//기존 노드의 key free
	delete pNode->GetKey();

	if (pData != NULL)
	{
		CUserData* pNewData = new CUserData;		//데이터 노드 동적할당
		memcpy(pNewData, pData, sizeData);			//
		pNewNode->SetpDataCache(pNewData);
		pNewNode->SetSizeData(sizeData);
	}

	pNewNode->SetbRemoved(bRemoved);

	//NewNode 교통정리
	pNewNode->SetOffset(pNode->GetOffset());	//삭제된 노드의 offset을 가짐
	pNewNode->SetPrev(pNode->GetPrev());
	pNewNode->SetNext(pNode->GetNext());

	//NewNode의 앞뒤 교통정리
	CMyNode* pPrevNode = pNode->GetPrev();
	CMyNode* pNextNode = pNode->GetNext();
	pPrevNode->SetNext(pNewNode);
	pNextNode->SetPrev(pNewNode);

	--m_removedNode;
}


int CLinkedList::SaveNewNodeToFile(CUserData user, int offset)
{
	if (offset >= 0) {
		FILE* fp = NULL;
		fopen_s(&fp, "listdata.dat", "rb+");
		if (fp == NULL)
			return 0;

		fseek(fp, offset * sizeof(CUserData), SEEK_SET);
		fwrite(&user, sizeof(CUserData), 1, fp);
		fclose(fp);
		return 1;
	}
	
	return 0;
}


int CLinkedList::getMaxOffset()
{
	return m_TailNode.GetPrev()->GetOffset();
}



CMyNode* CLinkedList::SearchListByPhone(const char* pszKey)
{
	CMyNode* pTmp = m_HeadNode.GetNext();
	while (pTmp != &m_TailNode) {
		if (strcmp(pTmp->GetKey(), pszKey) == 0) {
			return pTmp;
		}
		pTmp = pTmp->GetNext();
	}
	return nullptr;
}


int CLinkedList::CheckDuplicate(const char* pszData)
{
	CMyNode* pTmp = (CMyNode*)SearchListByPhone(pszData);
	//중복없음 0, 중복있음 1
	if (pTmp == NULL) {		//pTmp가 널이면 중복없음
		return 0;
	}
	else {
		return 1;
	}
}


int CLinkedList::RemoveData(CMyNode* pNode)
{
	CUserData user;
	FILE* fp = NULL;
	fopen_s(&fp, "listdata.dat", "rb+");

	if (fp == NULL)
		return 0;

	fseek(fp, pNode->GetOffset() * sizeof(CUserData), SEEK_SET);
	if (fwrite(&user, sizeof(CUserData), 1, fp) > 0)
	{
		pNode->SetbRemoved(true);
		memset(pNode->GetKey(), 0, sizeof(pNode->GetKey()));
		fclose(fp);
		++m_removedNode;
		return 1;
	}

	fclose(fp);

	return 0;
}


CMyNode* CLinkedList::FindRemovedNode()
{
	CMyNode* pTmp = m_HeadNode.GetNext();

	//연결리스트노드에서 삭제된 노드를 찾음
	//삭제된 노드가 있는경우 해당 노드의 자리에 추가
	while (pTmp != &m_TailNode) {
		if (pTmp->GetbRemoved() == true) {
			return pTmp;
		}
		pTmp = pTmp->GetNext();
	}
	return nullptr;
}


void CLinkedList::SearchDataByName(const char* name, const char* phone, const char* oper)
{
	int found = 0;
	int cnt = 0;
	FILE* fp = NULL;
	fopen_s(&fp, "listdata.dat", "rb");
	CUserData user;

	//이름만 검색한경우
	if (oper==NULL) {
		//이름이 일치하면 출력
		while (fread(&user, sizeof(CUserData), 1, fp) != 0) {
			if (strcmp(user.GetName(), name) == 0) {
				printf("[offset %d] name: %s, phone: %s, age: %d\n", cnt, user.GetName(), user.GetPhone(), user.GetAge());

				found = 1;
			}
			memset(&user, 0, sizeof(CUserData));
			++cnt;
		}
	}
	//and인 경우
	else if (strcmp(oper, "and") == 0) {
		while (fread(&user, sizeof(CUserData), 1, fp) != 0) {
			if (strcmp(user.GetName(), name) == 0 && strcmp(user.GetPhone(), phone) == 0) {
				printf("[offset %d] name: %s, phone: %s, age: %d\n", cnt, user.GetName(), user.GetPhone(), user.GetAge());
				found = 1;
			}
			memset(&user, 0, sizeof(CUserData));
			++cnt;
		}
	}
	//or인 경우
	else if (strcmp(oper, "or") == 0) {
		while (fread(&user, sizeof(CUserData), 1, fp) != 0) {
			if (strcmp(user.GetName(), name) == 0 || strcmp(user.GetPhone(), phone) == 0) {
				printf("[offset %d] name: %s, phone: %s, age: %d\n", cnt, user.GetName(), user.GetPhone(), user.GetAge());
				found = 1;
			}
			memset(&user, 0, sizeof(CUserData));
			++cnt;
		}
	}

	if (found == 0) {
		printf("일치하는 데이터가 존재하지 않습니다.\n");
	}

	fclose(fp);
	return;
}



int CLinkedList::EditPhoneNumber(CMyNode* pNode, const char* phone)
{
	CUserData user;
	FILE* fp = NULL;
	fopen_s(&fp, "listdata.dat", "rb+");

	if (fp == NULL)
		return 0;

	fseek(fp, pNode->GetOffset() * sizeof(CUserData) + sizeof(int) + sizeof(char)*32, SEEK_SET);

	if (fwrite(phone, sizeof(user.GetPhone()), 1, fp) > 0)
	{
		strcpy(pNode->GetKey(), phone);
		fclose(fp);
		return 1;
	}

	fclose(fp);
	return 0;
}
