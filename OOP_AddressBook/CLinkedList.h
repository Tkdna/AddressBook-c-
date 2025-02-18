#pragma once

#include "CMyNode.h"
#include "CUserData.h"

class CLinkedList
{
private:
	CLinkedList();			//�����ڸ� private ����� �־� Ŭ������ ������ ����
	~CLinkedList();
	CMyNode m_HeadNode;
	CMyNode m_TailNode;
	unsigned int m_listCount = 0;
	unsigned int m_removedNode = 0;
public:
	//�̱��� ����
	//�ܺο����� GetInstance �Լ��� ����Ͽ� ������� 
	static CLinkedList& GetInstance() {
		static CLinkedList instance;
		return instance;
	}
	CLinkedList(const CLinkedList&) = delete;
	CLinkedList& operator=(const CLinkedList&) = delete;
	
	void InitList();
	int IsEmpty();
	void ReleaseList();
	int LoadListFromFile();
	void AddNewNode(const char* pszKey, const void* pData, 
		unsigned int sizeData, bool bRemoved, unsigned int offset);
	void OverwirteNewNode(const char* pszKey, const void* pData, 
		unsigned int sizeData, bool bRemoved, unsigned int offset, CMyNode* pNode);
	int SaveNewNodeToFile(CUserData user, int offset);
	int getMaxOffset();
	CMyNode* SearchListByPhone(const char* pszKey);
	int CheckDuplicate(const char* pszData);
	int RemoveData(CMyNode* pNode);
	CMyNode* FindRemovedNode();
	void SearchDataByName(const char* name, const char* phone, const char* oper);
	int EditPhoneNumber(CMyNode* pNode, const char* phone);
	unsigned int GetListCount() {
		return m_listCount;
	}
	unsigned int GetRemovedNode() {
		return m_removedNode;
	}
	CMyNode* GetHeadNode() {
		return &m_HeadNode;
	}
};

