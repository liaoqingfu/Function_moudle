#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;
//抽象类
class Animal{
public:
	virtual void speak() = 0;//纯虚函数
};

//狗类
class Dog :public Animal{
public:
	Dog(string name)
	{
		this->name = name;
	}
	//必须重写父类的纯虚函数，不然后该类也会成为抽象类
	virtual void speak()
	{
		cout << "小狗" << name << "汪汪汪" << endl;
	}

private:
	string name;
};

//鸡
class Chikn :public Animal{
public:
	Chikn(string name,string type)
	{
		this->name = name;
		this->type = type;
	}
	virtual void speak()
	{
		cout <<type<< "小鸡" << name << "咯咯咯" << endl;
	}

private:
	string name;
	string type;
};

//老虎
class Tiger :public Animal{
public:
	Tiger(string name,int age)
	{
		this->name = name;
		this->age = age;
	}
	virtual void speak()
	{
		cout << age << "老虎" << name << "嗷嗷嗷" << endl;
	}
private:
	string name;
	int age;
};

//鸭子
class Dark :public Animal{
public:
	Dark(string name)
	{
		this->name = name;
	}
	virtual void speak()
	{
		cout << "鸭子" << name << "嘎嘎嘎" << endl;
	}

private:
	string name;

};

class SheepTuo :public Animal{
public:
	SheepTuo(string name)
	{
		this->name = name;
	}
	virtual void speak()
	{
		cout  << name << "咩咩咩" << endl;
	}

private:
	string name;
};


class Zoo{
public:
	Zoo()
	{
		mCapacity = 1024;
		msize = 0;
		//申请空间存放Animal*;
		p = new Animal*[mCapacity];
	}

	//增加动物的方法
	void AddAnimal(Animal *animal)
	{
		if (msize == mCapacity)
		{
			return;
		}

		this->p[msize] = animal;
		this->msize++;

	}
	//让动物园的动物嚎叫
	void StartSpeak()
	{
		for (int i = 0; i < msize; i++)
		{
			this->p[i]->speak();
		}
	}
	~Zoo()
	{
		cout << "zoo析构" << endl;
		for (int i = 0; i < msize; i++)
		{
			if (this->p[i] != NULL)
			{
				delete this->p[i];
				this->p[i] = NULL;
			}
		}

		delete[] this->p;
		this->p = NULL;
	}


private:
	Animal **p;
	int mCapacity;//动物园容量
	int msize;//动物现有个数
};

void test()
{
	//创建动物园
	Zoo *zoo = new Zoo;

	//添加动物
	zoo->AddAnimal(new Dog("旺财"));//Aniaml *animal=new Dog("旺财");
	zoo->AddAnimal(new Dark("唐老鸭"));
	zoo->AddAnimal(new Chikn("苍老师", "日本"));
	zoo->AddAnimal(new Tiger("华南虎", 5));

	zoo->AddAnimal(new SheepTuo("羊驼"));

	zoo->StartSpeak();

	delete zoo;

}

int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

