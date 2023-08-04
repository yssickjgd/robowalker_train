#include <stdio.h>

// 这是一个类 
class student
{
public:
	// 随意访问 
    void Init(char *__name, int __year); // 其实还有一种更标准或者说更适宜的初始化方式, 但理解起来可能有些复杂 
    void PrintInfo();
    
    // 一种成员函数的声明方式 
    void foo()
    {
    	printf("%d\n", 1);
	}
protected:
    // 可在类内与派生类(继承)内访问 
	char *name;
	int year;
private:
	// 仅可在类内访问 
	int useless;
};

void student::Init(char *__name, int __year)
{
	name = __name;
	year = __year;
}

void student::PrintInfo()
{
	printf("Student`s name is %s. Enrollment year is %d\n", name, year);
}

// 类的派生与继承 
// student类派生了此类, 或者说此类继承于student
// 继承方式为public 
class student_with_department : public student
{
public:
    void Init(char *__name, int __year, int __department); // 重名函数会覆盖, 之前的不能用了 
    // 重载示例 
    void PrintInfo(int a, int b)
    {
    	printf("Student`s name is %s. Enrollment year is %d\n", name, year);
	}
    void PrintInfo(int a, int b, int c)
    {
    	printf("Student`s name is %s. Enrollment year is %d. Department code is %d\n", name, year, department);
	}
protected:
	int department;
};

void student_with_department::Init(char *__name, int __year, int __department)
{
	name = __name;
	year = __year;
	department = __department; 
}

int main()
{
	student_with_department newstudent;
	newstudent.Init("Yij", 2018, 6);
	newstudent.PrintInfo(1, 1);
	newstudent.PrintInfo(1, 1, 1);
} 
