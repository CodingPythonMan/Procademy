#include "RingBuffer.h"
#include <cstring>

RingBuffer::RingBuffer() : BufferSize{ DEFAULT_SIZE }
{
	Buffer = new char[BufferSize + 1];
	Front = 0;
	Rear = 0;
}

RingBuffer::RingBuffer(int bufferSize) : BufferSize{ bufferSize }
{
	Buffer = new char[BufferSize + 1];
	Front = 0;
	Rear = 0;
}

RingBuffer::~RingBuffer()
{
	delete[] Buffer;
}

int RingBuffer::GetBufferSize()
{
	return BufferSize;
}

int RingBuffer::GetUseSize()
{
	int size = Rear - Front;

	if (size < 0)
	{
		size = BufferSize + size;
	}

	return size;
}

int RingBuffer::GetFreeSize()
{
	return BufferSize - GetUseSize();
}

int RingBuffer::Enqueue(char* src, int size)
{
	if (size > 0)
	{
		Rear++;

		if (Rear + size > BufferSize)
		{
			int rest = BufferSize + 1 - Rear;
			memcpy(&Buffer[Rear], src, rest);
			memcpy(&Buffer[0], src + rest, size - rest);
			Rear = size - rest - 1;
		}
		else
		{
			memcpy(&Buffer[Rear], src, size);
			Rear += size - 1;
		}
	}
	else
	{
		size = 0;
	}

	return size;
}

int RingBuffer::Dequeue(char* dest, int size)
{
	if (size > 0)
	{
		Front++;

		if (Front + size > BufferSize)
		{
			int rest = BufferSize + 1 - Front;
			memcpy(dest, &Buffer[Front], rest);
			memcpy(dest + rest, &Buffer[0], size - rest);
			Front = size - rest - 1;
		}
		else
		{
			memcpy(dest, &Buffer[Front], size);
			Front += size - 1;
		}
	}
	else
	{
		size = 0;
	}

	return size;
}

int RingBuffer::Peek(char* dest, int size)
{
	if (size > 0)
	{
		if (Front + 1 + size > BufferSize)
		{
			int rest = BufferSize - Front;
			memcpy(dest, &Buffer[Front + 1], rest);
			memcpy(dest + rest, &Buffer[0], size - rest);
		}
		else
		{
			memcpy(dest, &Buffer[Front + 1], size);
		}
	}
	else
	{
		size = 0;
	}

	return size;
}

void RingBuffer::ClearBuffer()
{
	Front = Rear;
}

int RingBuffer::DirectEnqueueSize()
{
	int size = Rear - Front;

	if (size < 0)
	{
		size = Front - Rear - 1;
	}
	else
	{
		size = BufferSize - Rear;
	}

	return size;
}

int RingBuffer::DirectDequeueSize()
{
	int size = Rear - Front;

	if (size < 0)
	{
		size = BufferSize - Front;
	}

	return size;
}

int RingBuffer::MoveRear(int size)
{
	if (size > 0)
	{
		if (Rear + 1 + size > BufferSize)
		{
			Rear = Rear + size - BufferSize - 1;
		}
		else
		{
			Rear += size;
		}
	}

	return size;
}

int RingBuffer::MoveFront(int size)
{
	if (size > 0)
	{
		if (Front + 1 + size > BufferSize)
		{
			Front = Front + size - BufferSize - 1;
		}
		else
		{
			Front += size;
		}
	}

	return size;
}

char* RingBuffer::GetFrontBufferPtr()
{
	int IncreaseFront = 0;

	if (Front < BufferSize)
		IncreaseFront = Front + 1;

	return &Buffer[IncreaseFront];
}

char* RingBuffer::GetRearBufferPtr()
{
	int IncreaseRear = 0;

	if (Rear < BufferSize)
		IncreaseRear = Rear + 1;

	return &Buffer[IncreaseRear];
}
