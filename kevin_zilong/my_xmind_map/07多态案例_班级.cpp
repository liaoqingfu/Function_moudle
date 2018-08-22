#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

//人员
class Manito{
public:
	virtual void Skill() = 0;
};

class Maker :public Manito
{
public:
	Maker(string name,int age,string sex,int face,string skill)
	{
		this->name = name;
		this->age = age;
		this->sex = sex;
		this->Face = face;
		this->skill = skill;
	}
	virtual void Skill()
	{
		cout << name << "性别：" << sex << age << "岁 " << " 颜值" << Face << "分" << endl;
		cout << "技能为：" << skill << endl;
	}
private:
	string name;
	int age;
	string sex;
	int Face;
	string skill;
};

class Myclass{
public:
	Myclass()
	{
		this->mcapacity = 50;
		this->msize = 0;
		this->p = new Manito*[this->mcapacity];

	}

	//增加人员
	void AddManito(Manito* manito)
	{
		if (this->msize == this->mcapacity)
		{
			return;
		}

		this->p[this->msize] = manito;
		this->msize++;
	}

	//大家放大招
	void StartSkill()
	{
		for (int i = 0; i < this->msize; i++)
		{
			p[i]->Skill();
		}
	}
	~Myclass()
	{
		for (int i = 0; i < msize; i++)
		{
			if (this->p[i] != NULL)
			{
				delete this->p[i];
			}
		}

		delete[] this->p;
		this->p = NULL;
	}

private:
	Manito **p;
	int mcapacity;
	int msize;
};

void test()
{
	//创建班级
	Myclass *myclass = new Myclass;

	myclass->AddManito(new Maker("make", 30, "男", 80, "分配姑娘"));



	myclass->StartSkill();

	delete myclass;

}
int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

