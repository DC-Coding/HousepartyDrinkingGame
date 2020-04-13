#include <iostream>
#include "WinsockServer/WinsockServer.h"

int main()
{
	WinsockServer server;
	server.Create();
	server.StartConnectionThread();

	while (1)
	{
		std::vector<WinsockServer::Message>* queue = server.GetMessageQueue();
		if (queue->size() > 0)
		{
			std::cout << "From: " << queue->at(0).username << std::endl;
			std::cout << queue->at(0).messsage << std::endl;

			queue->clear();
		}

		Sleep(100);
	}

	system("pause");
	return 0;
}