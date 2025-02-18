#include "CUserData.h"
#include <string>

CUserData::CUserData() 
{
	memset(&m_Data, 0, sizeof(USERDATA));
}

CUserData::~CUserData() 
{

}

//int Read(FILE* fp) {
//	int result = fread(&m_Data, sizeof(USERDATA), 1, fp);
//	return
//}