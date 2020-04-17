#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")


int main()
{
	WSADATA wsa;
	int rc = WSAStartup(MAKEWORD(2, 0), &wsa);
	
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	rc = connect(s, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));

	while (1)
	{
		std::string message;
		std::cin >> message;

		send(s, message.c_str(), static_cast<int>(message.length()), 0);

		char buffer[256];
		memset(buffer, '\0', 256);
		rc = recv(s, buffer, 256, 0);
		std::cout << buffer << std::endl;
	}
}