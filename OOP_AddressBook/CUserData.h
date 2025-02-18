#pragma once

#include <string>

class CUserData
{
public:
	CUserData();
	~CUserData();

	//±¸Á¶Ã¼ ¸â¹ö
	struct USERDATA {
		int age;
		char name[32];
		char phone[32];
	};

	//getter
	const int GetAge() {
		return m_Data.age;
	}
	const char* GetName() {
		return m_Data.name;
	}
	const char* GetPhone() {
		return m_Data.phone;
	}

	//setter
	void SetAge(int age) {
		m_Data.age = age;
	}
	void SetName(char* name) {
		strcpy(m_Data.name, name);
	}
	void SetPhone(char* phone) {
		strcpy(m_Data.phone, phone);
	}
	//int Read(FILE* fp);
private:
	USERDATA m_Data;
};

