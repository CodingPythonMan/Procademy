#include "StarServer.h"

SOCKET listenSock;
MyList<Session*> ClientList;
MyList<Session*> DeleteList;
int UniqueID = 0;

bool SetServerSocket()
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return true;

	// ���� ����
	listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSock == INVALID_SOCKET)
		return true;

	// bind
	SOCKADDR_IN listenAddr;
	memset(&listenAddr, 0, sizeof(listenAddr));
	listenAddr.sin_family = AF_INET;
	listenAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	listenAddr.sin_port = htons(SERVER_PORT);
	retval = bind(listenSock, (SOCKADDR*)&listenAddr, sizeof(listenAddr));

	if (retval == SOCKET_ERROR)
		return true;

	return false;
}

bool ListenSocket()
{
	// listen
	int retval = listen(listenSock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		return true;

	// ����ŷ �������� ��ȯ
	u_long on = 1;
	retval = ioctlsocket(listenSock, FIONBIO, &on);
	if (retval == SOCKET_ERROR)
		return true;

	return false;
}

bool SelectLoop()
{
	// ������ ��ſ� ����� ����
	FD_SET rset;
	int retval;

	while (1)
	{
		FD_ZERO(&rset);
		FD_SET(listenSock, &rset);

		MyList<Session*>::iterator iter;
		for (iter = ClientList.begin(); iter != ClientList.end(); ++iter)
		{
			FD_SET((*iter)->Sock, &rset);
		}

		// ���� ���
		retval = select(0, &rset, nullptr, nullptr, nullptr);
		if (retval == SOCKET_ERROR)
			return true;

		// �������� �˻�
		if (FD_ISSET(listenSock, &rset))
		{
			AcceptProc();
		}

		// ������ ���
		for (iter = ClientList.begin(); iter != ClientList.end(); ++iter)
		{
			if (FD_ISSET((*iter)->Sock, &rset))
			{
				// ������ �ޱ�
				ReadProc(*iter);
			}
		}

		// ������ Client ����
		DeleteExecute();

		// ������
		Rendering();
	}

	return false;
}

bool EndSocket()
{
	closesocket(listenSock);

	WSACleanup();

	return false;
}

void AcceptProc()
{
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(clientAddr);
	SOCKET clientSock = accept(listenSock, (SOCKADDR*)&clientAddr, &addrLen);
	if (clientSock == INVALID_SOCKET)
		return;

	Session* session = new Session;
	session->Sock = clientSock;
	session->ID = UniqueID;
	session->X = MAX_X / 2;
	session->Y = MAX_Y / 2;

	InetNtop(AF_INET, &(clientAddr.sin_addr), session->IP, 16);
	session->Port = ntohs(clientAddr.sin_port);
	ClientList.push_back(session);
	
	// 1. Send AssignID(�����), StarCreate(��ü)
	AssignID assignID;
	assignID.ID = UniqueID;
	assignID.Type = (int)MessageType::AssignID;
	SendUnicast(session, (char*)&assignID);
	
	CreateStar createStar;
	createStar.ID = UniqueID;
	createStar.Type = (int)MessageType::CreateStar;
	createStar.X = MAX_X / 2;
	createStar.Y = MAX_Y / 2;
	SendBroadcast(nullptr, (char*)&createStar);

	// 2. �ٸ� ����� StarCreate �� �޾ƾ� �Ѵ�.
	MyList<Session*>::iterator iter;
	for (iter = ClientList.begin(); iter != ClientList.end(); ++iter)
	{
		if (session == *iter)
			continue;

		Session* otherSession = *iter;
		createStar.ID = otherSession->ID;
		createStar.X = otherSession->X;
		createStar.Y = otherSession->Y;
		SendUnicast(session, (char*)&createStar);
	}

	UniqueID++;
}

void ReadProc(Session* session)
{
	// ������ ���� �� ó�� �ʿ�
	char buf[16];
	int retval;
	while(1)
	{
		retval = recv(session->Sock, buf, 16, 0);
		
		if (retval == SOCKET_ERROR) 
		{
			retval = GetLastError();

			if(retval == WSAEWOULDBLOCK)
				break;
			
			if (retval == WSAECONNRESET)
			{
				Disconnect(session);
				break;
			}
		}
		else if (retval == 0)
		{
			Disconnect(session);
			break;
		}
			
		int type = *((int*)buf);
		switch ((MessageType)type)
		{
		case MessageType::MoveStar:
		{
			MoveStar moveStar;
			// 1. �ް� ������ ���� ��ǥ ����
			moveStar.Type = ((MoveStar*)buf)->Type;
			moveStar.ID = ((MoveStar*)buf)->ID;
			moveStar.X = ((MoveStar*)buf)->X;
			moveStar.Y = ((MoveStar*)buf)->Y;

			if (moveStar.X < 0 || moveStar.Y < 0 || moveStar.X >= MAX_X || moveStar.Y >= MAX_Y)
				break;
			
			session->X = moveStar.X;
			session->Y = moveStar.Y;

			// 2. �ٸ� �̿��� BroadCast
			SendBroadcast(session, (char*)&moveStar);

			// ������
			Rendering();
		}
		break;
		default:
			Disconnect(session);
			break;
		}
	}
}

void SendUnicast(Session* session, char* buf)
{
	int retval = send(session->Sock, buf, 16, 0);
	if (retval == SOCKET_ERROR)
	{
		Disconnect(session);
	}
}

void SendBroadcast(Session* session, char* buf)
{
	int retval;

	MyList<Session*>::iterator iter;
	for (iter = ClientList.begin(); iter != ClientList.end(); ++iter)
	{
		if (*iter == session)
			continue;

		retval = send((*iter)->Sock, buf, 16, 0);
		if (retval == SOCKET_ERROR)
		{
			retval = GetLastError();

			if(retval == WSAECONNRESET)
				Disconnect(*iter);
		}
	}
}

void Disconnect(Session* session)
{
	// 1. ������ ��Ŷ ����
	DeleteStar deleteStar;
	deleteStar.ID = session->ID;
	deleteStar.Type = (int)MessageType::DeleteStar;
	SendBroadcast(session, (char*)&deleteStar);

	// 2. ClientList ���� ���� => �켱 �ӽ÷� ���� ����Ʈ�� �߰�
	DeleteList.push_back(session);
}

void DeleteExecute()
{
	MyList<Session*>::iterator iter;
	for (iter = DeleteList.begin(); iter != DeleteList.end(); ++iter)
	{
		closesocket((*iter)->Sock);
		ClientList.remove(*iter);
		delete(*iter);
	}

	DeleteList.clear();
}

void Rendering()
{
	MyList<Session*>::iterator iter;
	Clear();
	for (iter = ClientList.begin(); iter != ClientList.end(); ++iter)
	{
		SpriteDraw((*iter)->X, (*iter)->Y, '*');
	}
	Flip();
}

void Flip()
{
	for (int count = 0; count < MAX_Y; count++)
	{
		MoveCursor(0, count);
		printf(_ScreenBuffer[count]);
	}
}

void Clear()
{
	for (int count = 0; count < MAX_Y; count++)
	{
		memset(_ScreenBuffer[count], ' ', MAX_X);
		_ScreenBuffer[count][MAX_X - 1] = (char)NULL;
	}
}

void SpriteDraw(int X, int Y, char Sprite)
{
	if (X < 0 || Y < 0 || X >= MAX_X - 1 || Y >= MAX_Y)
		return;

	_ScreenBuffer[Y][X] = Sprite;
}