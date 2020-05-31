#include "Buffer.h"
Buffer::Buffer(): countForBuffer(0)
{
	buffer[0] = '\0';
}
const  Buffer& operator++(Buffer& buffer)
{
	buffer.countForBuffer++;
	return buffer;
}
char& Buffer::operator[](const int index)
{
	return buffer[index];
}
int Buffer::CountForBuffer()
{
	return countForBuffer;
}
void Buffer::ChangeCountForBuffer(int i)
{
	countForBuffer = i;
}
void Buffer::Add(char c)
{
	buffer[countForBuffer] = c;
	++(*this);
	buffer[countForBuffer] = '\0';
}

char* Buffer::ReturnBuffer()
{
	return buffer;
}