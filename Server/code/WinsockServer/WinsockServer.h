#pragma once
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <thread>
#include <vector>
#pragma comment(lib, "Ws2_32.lib")


class WinsockServer
{
	public:

		struct SocketConnection
		{
			SOCKET s;
			std::string username;
		};

		struct Message
		{
			std::string username;
			std::string messsage;
		};

		void Create();
		void StartConnectionThread();
		std::vector<Message>* GetMessageQueue();

	private:

		void WaitForConnection();
		void WaitForMessage(SocketConnection connection);

		SOCKET _serverSocket;
		std::vector<SocketConnection> _connections;
		std::vector<Message> _messageQueue;
		
};