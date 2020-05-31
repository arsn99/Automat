#pragma once
class Buffer
{
	unsigned int countForBuffer;

	public :
		char buffer[32];
		Buffer();
		friend const Buffer& operator++(Buffer& buffer);
		int CountForBuffer();
		char& operator[](const int index);
		void Add(char c);
		void ChangeCountForBuffer(int i);
		char* ReturnBuffer();

};

