

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>   // для функции isdigit
#include <cstdlib>  // для функции atoi
#include "Buffer.h" 
using namespace std;

const int NUM_OF_WORDS = 1;
const int NUM_OF_WORDS_E1 = 3;
class Lexator
{
public:

	Lexator(  string fileNameOpen,  string fileNameSave);
	~Lexator();
	enum ID 
	{
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
		let,
		equalEqual,
		equalBig,
		equalLit,
		Bigger,
		Litter
	};
	enum errorID
	{
		openFile,
		notOpenFile

	};
	

	int lineNumber;
	void CloseFile();
	bool updateLine;
	ofstream out;
	ifstream in;
	void StateMachine();
	string fileNameOpen, fileNameSave;
	int OpenFile();
	enum State
	{
		H,
		Id,
		Word,
		Number,
		E5,
		E6,
		E7
	};
	State currentState;
	char currentLex;
	Buffer buffer;
	char stateE1[NUM_OF_WORDS_E1] = { ' ','\r','\t' };
	char keywords[NUM_OF_WORDS][10] = {"while"};
	bool CheckTabulation();
	bool CheckOperatorE1();
	bool CheckE1();
	bool CheckE6();
	void TabLexem(int number, string str);
	void ErrorFun(string str);
	void ErrorFun(string str, char lex);
	void UpdateLine();
	bool CheckWord();
	bool IsLetter();
	bool CheckReservedWords();
	int currentReservedWord;
	bool CheckOperatorE1_2();
	bool CheckNumber();

};

Lexator::Lexator( string fileNameOpen,  string fileNameSave)
{
	this->fileNameOpen = fileNameOpen;
	this->fileNameSave = fileNameSave;
	updateLine = false;
	if (OpenFile() == openFile)
	{
		StateMachine();
	}
	else
		cout << "Неудалось открыть файл" << endl;
}
bool Lexator::CheckWord()
{
	if (buffer.CountForBuffer() == 0 && IsLetter())
	{
		buffer.Add(currentLex);
		return 1;
	}

	if ((IsLetter() || isdigit(currentLex))&& buffer.CountForBuffer() > 0)
	{
		buffer.Add(currentLex);
		return 1;
	}
	
	return 0;
}
/// <summary>
/// Проверяет является ли данный символ буквой.
/// </summary>
bool Lexator::IsLetter()
{
	if ((currentLex >= 'a' && currentLex <= 'z') || (currentLex >= 'A' && currentLex <= 'Z'))
	{
		return 1;
	}
	else
		return 0;
}
/// <summary>
/// Проверяет является ли данный символ табуляцией
/// </summary>
bool Lexator::CheckTabulation()
{
	if (currentLex == '\n')
	{
		UpdateLine();
		return 1;
	}

	for (int i = 0; i < NUM_OF_WORDS_E1; i++)
	{
		if (currentLex == stateE1[i])
		{
			return 1;
		}
	}
	return 0;
}
/// <summary>
/// Проверяет является ли данный символ оператором, если является то заносится в таблицу
/// </summary>
bool Lexator::CheckOperatorE1()
{
	switch (currentLex)
	{
	case ';':
		TabLexem(semicolon, " ; ");
		return 1;

	case '*':
		TabLexem(multiply, " * ");
		return 1;

	case '+':
		TabLexem(plus, " + ");
		return 1;
	case '-':
		TabLexem(minus, " - ");
		return 1;

	case '>':
		TabLexem(Bigger, " > ");
		return 1;

	case '%':
		TabLexem(moduloDivision, " % ");
		return 1;

	case ')':
		TabLexem(rightBrace, " ) ");
		return 1;

	case '(':
		TabLexem(leftBrace, " ( ");
		return 1;

	case '{':
		TabLexem(leftBracket, " { ");
		return 1;

	case '}':
		TabLexem(rightBracket, " } ");
		return 1;

	case '=':
		TabLexem(equal, " = ");
		return 1;
	case '/':
		TabLexem(divide, " / ");
		return 1;
	default:
		return false;
		break;
	}
}
/// <summary>
/// Проверяет является ли данный символ табуляцией
/// </summary>
bool Lexator::CheckOperatorE1_2()
{
	switch (currentLex)
	{
	case ';':
		return 1;

	case '*':
		return 1;

	case '+':
		return 1;
	case '-':
		return 1;
	case '>':
		return 1;

	case '%':
		return 1;

	case ')':
		return 1;

	case '(':
		return 1;

	case '{':
		return 1;

	case '}':
		return 1;

	case '=':
		return 1;
	case '/':
		return 1;
	default:
		return false;
		break;
	}
}
/// <summary>
/// Проверяет является ли данное слово, которое содержится в буфере зарезервированым
/// </summary>
bool Lexator::CheckReservedWords()
{
	for ( currentReservedWord = 0; currentReservedWord < NUM_OF_WORDS; currentReservedWord++)
	{
		for (int currentLetter = 0; currentLetter < buffer.CountForBuffer(); currentLetter++)
		{
			if (keywords[currentReservedWord][currentLetter] != buffer[currentLetter])
				return 0;	
		}
	}

	return 1;
}
/// <summary>
/// Вызываются функции, которые нужны в состоянии Н
/// </summary>
bool Lexator::CheckE1()
{
	if (CheckTabulation())
		return 1;
	
	if (IsLetter())
	{
		buffer.ChangeCountForBuffer(0);
		CheckWord();
		currentState = Word;
		return 1;
	}
	if (isdigit(currentLex))
	{
		buffer.ChangeCountForBuffer(0);	
		buffer.Add(currentLex);
		currentState = Number;
		return 1;
		/**/
	}
	if (CheckOperatorE1())
		return 1;
	
	if (currentLex == '/' && in.peek() == '/')
	{
		while (currentLex != '\n' && in.get(currentLex))
			;
		return 1;
	}
	if (currentLex == '/' && in.peek() == '*')
	{
		in.get(currentLex);
		in.get(currentLex);
	
		while (	(	(currentLex != '*') || (in.peek() != '/')	)&&in.get(currentLex) )
		{
			cout << (currentLex != '*') << " " << (in.peek() != '/') << endl;
			
		}
		cout << currentLex << " CHECK" << endl;
		in.get(currentLex);
		return 1;
	}

	return 0;
}
/// <summary>
/// Функция для проверки слова.
/// </summary>
bool Lexator::CheckE6()
{
	if (CheckTabulation()|| CheckOperatorE1_2())
	{
		if (buffer.CountForBuffer()>0)
		{
			string word = string(buffer.ReturnBuffer());
			if (CheckReservedWords())
				TabLexem(currentReservedWord-1, " Служебное слово : " + word);
			else
				TabLexem(id, " Идентификатор : " + word);

			buffer.ChangeCountForBuffer(0);
			CheckOperatorE1();
		}
		currentState = H;
		return 1;
	}
	if (CheckWord())
		return 1;
}
/// <summary>
/// Проверяет является ли текущий символ числом, если да то заносит в буфер.
/// </summary>
bool Lexator::CheckNumber()
{
	bool isFloat = false;
	
	if (isalpha(currentLex))
	{
		currentState = H;
		return 0;
	}
	while (!CheckTabulation()&&!CheckOperatorE1_2())
	{
		
		if (isdigit(currentLex))
		{
			buffer.Add(currentLex);
		}
		else
			if (currentLex == '.')
				if (!isFloat)
				{
					isFloat = true;
					buffer.Add(currentLex);
				}
				else
				{
					currentState = H;
					return 0;
				}
			else
			{
				currentState = H;
				return 0;
			}

		in.get(currentLex);

	}
	
	if (isFloat)
	{
		TabLexem(typeFloat, " Числовая константа с плавающей запятой " + string(buffer.ReturnBuffer()));
	}
	else
	{
		TabLexem(typeInt, " Целочисленная константа " + string(buffer.ReturnBuffer()));
	}

	currentState = H;

	if (CheckTabulation() || CheckOperatorE1())
		return 1;
	else
		return 0;
}
/// <summary>
/// Переход по состояниям
/// </summary>
void Lexator::StateMachine()
{
	currentState = H;
	while (in.get(currentLex))
	{
		switch (currentState)
		{
			case H:
				if (!CheckE1())
					ErrorFun("Неправильный символ в состояние H: ", currentLex);
				break;
			case Word:
				if(!CheckE6())
					ErrorFun("Неправильный символ в состояние Word : ", currentLex);
				break;
			case Number:
				if (!CheckNumber())
					ErrorFun("Неправильный символ в состояние Number :  ", currentLex);
				break;
				
			default:
				break;
		}

	}
}
Lexator::~Lexator()
{
	CloseFile();
}
/// <summary>
/// Обновление номера строки
/// </summary>
void Lexator::UpdateLine()
{
		lineNumber++;
}
void Lexator::ErrorFun(string str)
{
	cout << lineNumber << " " << str << endl;
}
void Lexator::ErrorFun(string str,char lex)
{
	cout << lineNumber << " " << str <<lex<< endl;
}
/// <summary>
/// Занесение информации в файл
/// </summary>
void Lexator::TabLexem( int number, string str)
{
	out << lineNumber << "\t" << number << "\t" << str<<endl;
}
void Lexator::CloseFile()
{
	in.close();
	out.close();
}
int Lexator::OpenFile()
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

