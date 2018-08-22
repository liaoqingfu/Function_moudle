#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;
//������
class Animal{
public:
	virtual void speak() = 0;//���麯��
};

//����
class Dog :public Animal{
public:
	Dog(string name)
	{
		this->name = name;
	}
	//������д����Ĵ��麯������Ȼ�����Ҳ���Ϊ������
	virtual void speak()
	{
		cout << "С��" << name << "������" << endl;
	}

private:
	string name;
};

//��
class Chikn :public Animal{
public:
	Chikn(string name,string type)
	{
		this->name = name;
		this->type = type;
	}
	virtual void speak()
	{
		cout <<type<< "С��" << name << "������" << endl;
	}

private:
	string name;
	string type;
};

//�ϻ�
class Tiger :public Animal{
public:
	Tiger(string name,int age)
	{
		this->name = name;
		this->age = age;
	}
	virtual void speak()
	{
		cout << age << "�ϻ�" << name << "���" << endl;
	}
private:
	string name;
	int age;
};

//Ѽ��
class Dark :public Animal{
public:
	Dark(string name)
	{
		this->name = name;
	}
	virtual void speak()
	{
		cout << "Ѽ��" << name << "�¸¸�" << endl;
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
		cout  << name << "������" << endl;
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
		//����ռ���Animal*;
		p = new Animal*[mCapacity];
	}

	//���Ӷ���ķ���
	void AddAnimal(Animal *animal)
	{
		if (msize == mCapacity)
		{
			return;
		}

		this->p[msize] = animal;
		this->msize++;

	}
	//�ö���԰�Ķ��ﺿ��
	void StartSpeak()
	{
		for (int i = 0; i < msize; i++)
		{
			this->p[i]->speak();
		}
	}
	~Zoo()
	{
		cout << "zoo����" << endl;
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
	int mCapacity;//����԰����
	int msize;//�������и���
};

void test()
{
	//��������԰
	Zoo *zoo = new Zoo;

	//��Ӷ���
	zoo->AddAnimal(new Dog("����"));//Aniaml *animal=new Dog("����");
	zoo->AddAnimal(new Dark("����Ѽ"));
	zoo->AddAnimal(new Chikn("����ʦ", "�ձ�"));
	zoo->AddAnimal(new Tiger("���ϻ�", 5));

	zoo->AddAnimal(new SheepTuo("����"));

	zoo->StartSpeak();

	delete zoo;

}

int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

