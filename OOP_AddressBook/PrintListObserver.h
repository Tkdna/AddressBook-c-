#pragma once

#include "IObserver.h"
#include "CLinkedList.h"
#include <conio.h>
#include <iostream>

class PrintListObserver : public IObserver
{
private:
	CLinkedList& m_list;
public:
	PrintListObserver(CLinkedList& db) : m_list(db) {}
	~PrintListObserver() {}
    void Update() override
    {
        CMyNode* pTmp = m_list.GetHeadNode();
        while (pTmp != NULL)
        {
            printf("[%p] %s, removed: %d, offset: %d ",
                pTmp, pTmp->GetKey(), pTmp->GetbRemoved(), pTmp->GetOffset());

            printf("[%p]\n", pTmp->GetNext());
            pTmp = pTmp->GetNext();
        }
        putchar('\n');
        _getch();
    }
};

