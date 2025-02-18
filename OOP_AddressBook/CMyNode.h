#pragma once

#include <string>

class CMyNode
{
public:
	CMyNode();
	~CMyNode();

	struct MYNODE {
		bool bRemoved;
		void* pDataCache;
		char* pszKey;
		unsigned int offset;
		unsigned int sizeData;
	};

	//getter
	const bool GetbRemoved() {
		return m_Node.bRemoved;
	}
	const void* GetpDataCache() {
		return m_Node.pDataCache;
	}
	char* GetKey() {
		return m_Node.pszKey;
	}
	const unsigned int GetOffset() {
		return m_Node.offset;
	}
	const unsigned int GetSizeData() {
		return m_Node.sizeData;
	}
	CMyNode* GetPrev() {
		return pPrev;
	}
	CMyNode* GetNext() {
		return pNext;
	}


	//setter
	void SetbRemoved(bool removed) {
		m_Node.bRemoved = removed;
	}
	void SetpDataCache(void* Cache) {
		m_Node.pDataCache = Cache;
	}
	void SetKey(const char* key, int keylen) {
		m_Node.pszKey = new char[keylen + 1];	//문자열을 담을 메모리 동적할당
		strcpy(m_Node.pszKey, key);
	}
	void SetOffset(unsigned int offset) {
		m_Node.offset = offset;
	}
	void SetSizeData(unsigned int sizeData) {
		m_Node.sizeData = sizeData;
	}
	void SetPrev(CMyNode* node) {
		pPrev = node;
	}
	void SetNext(CMyNode* node) {
		pNext = node;
	}

private:
	CMyNode* pPrev;
	CMyNode* pNext;
	MYNODE m_Node;
};

