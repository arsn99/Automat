// Lexator.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>   // для функции isdigit
#include <cstdlib>  // для функции atoi
using namespace std;

class Lexator
{
public:

	Lexator(  string fileNameOpen,  string fileNameSave);
	~Lexator();
	int OpenFile(string fileNameOpen, string fileNameSave);
	void TabLexem(int number, string str);
	void ErrorFun(string str);
	void UpdateLine();
	void IsDigit();
	void CloseFile();
	bool updateLine;
	ofstream out;
	ifstream in;
	string currentLexem;
	enum ID 
	{
		fn,
		whileFn,
		leftBrace,	//	{
		rightBrace, //	}
		leftBracket,//	(
		rightBracket,// )
		id,
		equal,
		multiply,
		plus,
		minus,
		divide,
		moduloDivision,
		comment,
		typeInt,
		typeFloat,
		colon,
		semicolon,
		constInt,
		constFloat,
		let
	};
	enum errorID
	{
		openFile,
		notOpenFile

	};
	int lineNumber;
	string fileNameOpen;
	string fileNameSave;
};

Lexator::Lexator( string fileNameOpen,  string fileNameSave)
{
	this->fileNameOpen = fileNameOpen;
	this->fileNameSave = fileNameSave;

	updateLine = false;
	if (OpenFile(fileNameOpen, fileNameSave) == openFile)
	{
		while (!in.eof())
		{
			in >> currentLexem;
			UpdateLine();
			if (currentLexem.compare("fn")==0)
			{
				TabLexem(fn, "Ключевое слово");
				continue;
			}

			if (currentLexem.compare("{") == 0)
			{
				TabLexem(leftBrace, "{");
				continue;
			}
			if (currentLexem.compare("}") == 0)
			{
				TabLexem(rightBrace, "}");
				continue;
			}
			if (currentLexem.compare("(") == 0)
			{
				TabLexem(leftBracket, "(");
				continue;
			}
			if (currentLexem.compare(")") == 0)
			{
				TabLexem(rightBracket, ")");
				continue;
			}
			if (currentLexem.compare(";") == 0)
			{
				TabLexem(semicolon, ";");
				continue;
			}
			if (currentLexem.compare("let") == 0)
			{
				TabLexem(let, "Ключевое слово");
				continue;
			}
			if (currentLexem.compare("int") == 0)
			{
				TabLexem(typeInt, "Тип данных int");
				continue;
			}
			if (currentLexem.compare("float") == 0)
			{
				TabLexem(typeFloat, "Тип данных float");
				continue;
			}
			if (currentLexem.compare(":") == 0)
			{
				TabLexem(colon, ":");
				continue;
			}
			if (currentLexem.compare("*") == 0)
			{
				TabLexem(multiply, "*");
				continue;
			}
			if (currentLexem.compare("/") == 0)
			{
				TabLexem(divide, "/");
				continue;
			}
			if (currentLexem.compare("+") == 0)
			{
				TabLexem(plus, "+");
				continue;
			}
			if (currentLexem.compare("-") == 0)
			{
				TabLexem(minus, "-");
				continue;
			}
			if (currentLexem.compare("%") == 0)
			{
				TabLexem(moduloDivision, "%");
				continue;
			}
			if (currentLexem.compare("=") == 0)
			{
				TabLexem(equal, "Равно");
				continue;
				
			}
			if (isdigit(currentLexem[0]))
			{
				IsDigit();
				continue;;
			}
			else
			{
				TabLexem(id, "Идентификатор : "+currentLexem);
			}
			
		}
	}
	else
		cout << "Неудалось открыть файл" << endl;
}
Lexator::~Lexator()
{
	CloseFile();
}
void Lexator::UpdateLine()
{
	if (updateLine)
	{
		lineNumber++;
		updateLine = false;
	}
	if (in.peek() == '\n')
	{
		updateLine = true;
	}
}
void Lexator::IsDigit()
{
	bool dot = false;
	for (int i = 0; i < currentLexem.size(); i++)
	{
		if (isdigit(currentLexem[i]))
			continue;
		else 
			if (currentLexem[i] == '.')
			{
				if (!dot)
					dot = true;
				else
				{
					ErrorFun("Повторное использование '.' ");
					exit(1);
				}
					
			}
			else
			{
				ErrorFun("Число не может содержать буквы");
				exit(1);
			}
	}
	if (!dot)
	{
		TabLexem(constInt, "Целочисленная константа : "+currentLexem);
	}
	else
	{
		TabLexem(constFloat, "Константа с плавующей запятой : " + currentLexem);
	}
	
}

void Lexator::ErrorFun(string str)
{
	cout << lineNumber << " " << str << endl;
	exit(1);
}

void Lexator::TabLexem( int number, string str)
{
	out << lineNumber << "\t" << number << "\t" << str<<endl;
}

void Lexator::CloseFile()
{
	in.close();
	out.close();
}
int Lexator::OpenFile(string fileNameOpen, string fileNameSave)
{
	in.open(fileNameOpen);
	out.open(fileNameSave);
	if (in.is_open() && out.is_open())
		return openFile;

	else
		return notOpenFile;
		

}

int main(int argc, char* argv[])
{
	setlocale(0, "");
	string nameIn, nameOut;
	cout << "Введите имя файла для открытия"<<endl;
	cin >> nameIn;
	cout << "Введите имя файла для Лексем" << endl;;
	cin >> nameOut;

	Lexator lex("geka.txt", "Lexem.txt");

}

