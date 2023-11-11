#pragma once
#include <cstring>

#define DEFAULT_SIZE 1000

class RingBuffer {
public:
	RingBuffer();
	RingBuffer(int bufferSize);

	~RingBuffer();

	int GetBufferSize();
	
	// ������� �뷮 ���
	int GetUseSize();
	
	// ���� �뷮 ���
	int GetFreeSize();

	// WritePos �� ����Ÿ ����.
	int Enqueue(char* data, int size);

	// ReadPos ���� ����Ÿ ������. ReadPos �̵�.
	int Dequeue(char* dest, int size);

	// ReadPos ���� ����Ÿ �о��. ReadPos ����.
	int Peek(char* dest, int size);

	// ������ ��� ������ ����
	void ClearBuffer();

private:
	int Front;
	int Rear;
	int BufferSize;
	int FreeSize;

	char* Buffer;
};