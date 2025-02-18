#include "CMyNode.h"
#include <string>

CMyNode::CMyNode() {
	memset(&m_Node, 0, sizeof(MYNODE));
	pPrev = nullptr;
	pNext = nullptr;
}

CMyNode::~CMyNode() {

}