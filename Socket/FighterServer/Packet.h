/////////////////////////////////////////////////////////////////////
// www.gamecodi.com						������ master@gamecodi.com
//
//
/////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------

	Packet.

	��Ʈ��ũ ��Ŷ�� Ŭ����.
	�����ϰ� ��Ŷ�� ������� ����Ÿ�� In, Out �Ѵ�.

	- ����.

	CPacket cPacket;  or CMessage Message;

	�ֱ�.
	clPacket << 40030;		or	clPacket << iValue;	(int �ֱ�)
	clPacket << 1.4;		or	clPacket << fValue;	(float �ֱ�)


	����.
	clPacket >> iValue;		(int ����)
	clPacket >> byValue;		(BYTE ����)
	clPacket >> fValue;		(float ����)

	CPacket Packet2;

	!.	���ԵǴ� ����Ÿ FIFO ������ �����ȴ�.
		ȯ�� ť�� �ƴϹǷ�, �ֱ�(<<).����(>>) �� ȥ���ؼ� ������� �ʵ��� �Ѵ�



	* ���� ��Ŷ ���ν��������� ó��

	BOOL	netPacketProc_CreateMyCharacter(CPacket *clpPacket)
	{
		DWORD dwSessionID;
		short shX, shY;
		char chHP;
		BYTE byDirection;

//		*clpPacket >> dwSessionID >> byDirection >> shX >> shY >> chHP;


		*clpPacket >> dwSessionID;
		*clpPacket >> byDirection;
		*clpPacket >> shX;
		*clpPacket >> shY;
		*clpPacket >> chHP;

		...
		...
	}


	* ���� �޽���(��Ŷ) �����ο����� ó��

	CPacket MoveStart;
	mpMoveStart(&MoveStart, dir, x, y);
	SendPacket(&MoveStart);


	void	mpMoveStart(CPacket *clpPacket, BYTE byDirection, short shX, short shY)
	{
		st_NETWORK_PACKET_HEADER	stPacketHeader;
		stPacketHeader.byCode = dfNETWORK_PACKET_CODE;
		stPacketHeader.bySize = 5;
		stPacketHeader.byType = dfPACKET_CS_MOVE_START;

		clpPacket->PutData((char *)&stPacketHeader, dfNETWORK_PACKET_HEADER_SIZE);

		*clpPacket << byDirection;
		*clpPacket << shX;
		*clpPacket << shY;

	}

----------------------------------------------------------------*/
#ifndef  __PACKET__
#define  __PACKET__

#define MAX_PACKET_SIZE 1400

class Packet
{
public:
	//////////////////////////////////////////////////////////////////////////
	// ������, �ı���.
	//
	// Return:
	//////////////////////////////////////////////////////////////////////////
	Packet();
	Packet(int bufferSize);

	virtual	~Packet();


	//////////////////////////////////////////////////////////////////////////
	// ��Ŷ û��.
	//
	// Parameters: ����.
	// Return: ����.
	//////////////////////////////////////////////////////////////////////////
	void	Clear(void);


	//////////////////////////////////////////////////////////////////////////
	// ���� ������ ���.
	//
	// Parameters: ����.
	// Return: (int)��Ŷ ���� ������ ���.
	//////////////////////////////////////////////////////////////////////////
	int		GetBufferSize(void) { return BufferSize; }
	//////////////////////////////////////////////////////////////////////////
	// ���� ������� ������ ���.
	//
	// Parameters: ����.
	// Return: (int)������� ����Ÿ ������.
	//////////////////////////////////////////////////////////////////////////
	int		GetDataSize(void) { return DataSize; }



	//////////////////////////////////////////////////////////////////////////
	// ���� ������ ���.
	//
	// Parameters: ����.
	// Return: (char *)���� ������.
	//////////////////////////////////////////////////////////////////////////
	char* GetBufferPtr(void) { return Buffer; }

	//////////////////////////////////////////////////////////////////////////
	// ���� Pos �̵�. (�����̵��� �ȵ�)
	// GetBufferPtr �Լ��� �̿��Ͽ� �ܺο��� ������ ���� ������ ������ ��� ���. 
	//
	// Parameters: (int) �̵� ������.
	// Return: (int) �̵��� ������.
	//////////////////////////////////////////////////////////////////////////
	int		MoveWritePos(int size);
	int		MoveReadPos(int size);






	/* ============================================================================= */
	// ������ �����ε�
	/* ============================================================================= */
	Packet& operator = (Packet& srcPacket);

	//////////////////////////////////////////////////////////////////////////
	// �ֱ�.	�� ���� Ÿ�Ը��� ��� ����.
	//////////////////////////////////////////////////////////////////////////
	Packet& operator << (unsigned char value);
	Packet& operator << (char value);

	Packet& operator << (short value);
	Packet& operator << (unsigned short value);

	Packet& operator << (int value);
	Packet& operator << (long value);
	Packet& operator << (float value);

	Packet& operator << (__int64 value);
	Packet& operator << (double value);

	//////////////////////////////////////////////////////////////////////////
	// ����.	�� ���� Ÿ�Ը��� ��� ����.
	//////////////////////////////////////////////////////////////////////////
	Packet& operator >> (unsigned char& value);
	Packet& operator >> (char& value);

	Packet& operator >> (short& value);
	Packet& operator >> (unsigned short& value);

	Packet& operator >> (int& value);
	Packet& operator >> (unsigned int& value);
	Packet& operator >> (float& value);

	Packet& operator >> (__int64& value);
	Packet& operator >> (double& value);

	//////////////////////////////////////////////////////////////////////////
	// ����Ÿ ���.
	//
	// Parameters: (char *)Dest ������. (int)Size.
	// Return: (int)������ ������.
	//////////////////////////////////////////////////////////////////////////
	int		GetData(char* chpDest, int size);

	//////////////////////////////////////////////////////////////////////////
	// ����Ÿ ����.
	//
	// Parameters: (char *)Src ������. (int)SrcSize.
	// Return: (int)������ ������.
	//////////////////////////////////////////////////////////////////////////
	int		PutData(char* chpSrc, int size);

protected:
	char* Buffer;
	int ReadPos;
	int WritePos;
	int	BufferSize;
	//------------------------------------------------------------
	// ���� ���ۿ� ������� ������.
	//------------------------------------------------------------
	int	DataSize;
};

#endif