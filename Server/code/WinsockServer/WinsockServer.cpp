#include "WinsockServer.h"

void WinsockServer::Create()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 0), &wsa))
	{
		std::cerr << "Could not start Winsock!" << std::endl;
		return;
	}

	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket == INVALID_SOCKET)
	{
		std::cerr << "Could not create socket!" << std::endl;
		return;
	}

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.s_addr = ADDR_ANY;

	if (bind(_serverSocket, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		std::cerr << "Could not bind socket to port 12345" << std::endl;
		return;
	}

	std::cout << "Started server!" << std::endl;
}

void WinsockServer::StartConnectionThread()
{
	std::thread Thread(&WinsockServer::WaitForConnection, this);
	Thread.detach();
}

void WinsockServer::WaitForConnection()
{
	while (1)
	{
		listen(_serverSocket, 0);
		
		SocketConnection newConnection;
		newConnection.s = accept(_serverSocket, NULL, NULL);
		
		char buffer[20];
		memset(buffer, '\0', 256);
		recv(newConnection.s, buffer, 20, 0);
		newConnection.username = buffer;

		_connections.push_back(newConnection);

		std::thread Thread(&WinsockServer::WaitForMessage, this, newConnection);
		Thread.detach();
	}
}

void WinsockServer::WaitForMessage(SocketConnection connection)
{
	while (1)
	{
		char buffer[256];
		memset(buffer, '\0', 256);
		int rc = recv(connection.s, buffer, 256, 0);
		if (rc <= 0)
		{
			return;
		}

		Message newMessage;
		newMessage.messsage = buffer;
		newMessage.username = connection.username;

		_messageQueue.push_back(newMessage);
	}
}

std::vector<WinsockServer::Message>* WinsockServer::GetMessageQueue()
{
	return &_messageQueue;
}