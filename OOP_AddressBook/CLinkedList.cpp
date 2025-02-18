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
		//0�ʱ�ȭ �� �͵��� ������ �͵��̹Ƿ� bRemoved true�� ����
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
	CMyNode* pNewNode = new CMyNode;	//���Ḯ��Ʈ ��� �����Ҵ�
	int keylen = strlen(pszKey);
	pNewNode->SetKey(pszKey, keylen);			//Ű�� ���� �޸� �����Ҵ�
	//strcpy_s(pNewNode->pszKey, keylen + 1, pszKey);

	if (pData != NULL)
	{
		CUserData* pNewData = new CUserData;		//������ ��� �����Ҵ�
		memcpy(pNewData, pData, sizeData);			//
		pNewNode->SetpDataCache(pNewData);
		pNewNode->SetSizeData(sizeData);
	}

	//pNewNode->bNew = bNew;
	pNewNode->SetbRemoved(bRemoved);
	pNewNode->SetOffset(offset);

	//���Ḯ��Ʈ �ǵڿ� ����
	CMyNode* pPrevNode = m_TailNode.GetPrev();

	if (pPrevNode == nullptr) { // ����Ʈ�� ����ִ� ���
		pNewNode->SetPrev(&m_HeadNode);
		pNewNode->SetNext(&m_TailNode);
		m_HeadNode.SetNext(pNewNode);
		m_TailNode.SetPrev(pNewNode);
	}
	else { // ����Ʈ�� �̹� ��尡 �ִ� ���
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
	//free(pNode->pszKey);		//���� ����� key free
	delete pNode->GetKey();

	if (pData != NULL)
	{
		CUserData* pNewData = new CUserData;		//������ ��� �����Ҵ�
		memcpy(pNewData, pData, sizeData);			//
		pNewNode->SetpDataCache(pNewData);
		pNewNode->SetSizeData(sizeData);
	}

	pNewNode->SetbRemoved(bRemoved);

	//NewNode ��������
	pNewNode->SetOffset(pNode->GetOffset());	//������ ����� offset�� ����
	pNewNode->SetPrev(pNode->GetPrev());
	pNewNode->SetNext(pNode->GetNext());

	//NewNode�� �յ� ��������
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
	//�ߺ����� 0, �ߺ����� 1
	if (pTmp == NULL) {		//pTmp�� ���̸� �ߺ�����
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

	//���Ḯ��Ʈ��忡�� ������ ��带 ã��
	//������ ��尡 �ִ°�� �ش� ����� �ڸ��� �߰�
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

	//�̸��� �˻��Ѱ��
	if (oper==NULL) {
		//�̸��� ��ġ�ϸ� ���
		while (fread(&user, sizeof(CUserData), 1, fp) != 0) {
			if (strcmp(user.GetName(), name) == 0) {
				printf("[offset %d] name: %s, phone: %s, age: %d\n", cnt, user.GetName(), user.GetPhone(), user.GetAge());

				found = 1;
			}
			memset(&user, 0, sizeof(CUserData));
			++cnt;
		}
	}
	//and�� ���
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
	//or�� ���
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
		printf("��ġ�ϴ� �����Ͱ� �������� �ʽ��ϴ�.\n");
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
