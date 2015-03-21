#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "RefPtr.h"
#include <string>
using namespace std;

class Title
{
public:
	Title()
	{
		m_count = 0;
		m_name = "";
	}
	void deref()
	{
		m_count--;
		if( m_count==0 )
			delete this;
	}
	void ref()
	{
		m_count++;
	}
	int m_count;
	string m_name;
};

class Document
{
public:
	RefPtr<Title> m_title;

	Document()
	{

	}
	static PassRefPtr<Title> createTitle()
	{
		PassRefPtr<Title> a = new Title();  
		a->m_name = "NewTitle";
		return a;  
	}
};

//int main( )
//{
//
//	RefPtr<Title> test = Document::createTitle();
//	test.release();
//	_CrtDumpMemoryLeaks();
//	return 0;
//}
#include <iostream>  
#include <vector>  
using namespace std;  
class TestCopyConstructor 
{
public:
	TestCopyConstructor(string nm)
	{
		name = nm;
	}
	TestCopyConstructor& operator=(const TestCopyConstructor& s)//重载运算符  
	{
		this->name = s.name+"New";
		printf("operator = : %s\n",name.c_str());
		return *this;
	}
	TestCopyConstructor(const TestCopyConstructor& s)//复制构造函数  
	{  
		this->name = s.name+"New";
		printf("Copy constructor: %s\n",name.c_str());
	}
public:
	string name;

}; 


TestCopyConstructor f()
{
	return TestCopyConstructor("Function");
}



int main()  
{
	TestCopyConstructor a("TestA");
	TestCopyConstructor b = a;

	TestCopyConstructor c("TestC");
	TestCopyConstructor d("TestD");
	d = c;

	TestCopyConstructor e = f();

	return 0;  
}  