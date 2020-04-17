/*! ###########################################################
 *
 * @file WinsockServer.cpp
 *
 * @author Michael Hollmer
 *
 * @date 13.04.2020
 *
 * @brief Implements the method protoypes from WinsockServer.h
########################################################### */

#include "WinsockServer.h"

WinsockServer::WinsockServer() {  }

void WinsockServer::Create()
{
	//Startup Winsock
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 0), &wsa))
	{
		std::cerr << "Could not start Winsock!" << std::endl;
		return;
	}

	//Create the socket
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket == INVALID_SOCKET)
	{
		std::cerr << "Could not create socket!" << std::endl;
		return;
	}

	//Fill the sockaddr_in struct
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.s_addr = ADDR_ANY;

	//Bind the socket tothe port
	if (bind(_serverSocket, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		std::cerr << "Could not bind socket to port 12345" << std::endl;
		return;
	}

	std::cout << "Started server!" << std::endl;
}

void WinsockServer::StartConnectionThread()
{
	//Start the WaitForConnection method as a detached thread
	std::thread Thread(&WinsockServer::WaitForConnection, this);
	Thread.detach();
}

void WinsockServer::WaitForConnection()
{
	while (1)
	{
		//Wait for an incoming connection
		listen(_serverSocket, 0);
		
		//Create a new connection struct
		SocketConnection newConnection;
		newConnection.s = accept(_serverSocket, NULL, NULL);

		//Create a unique identifier for this client
		newConnection.clientID = (_connections.size() <= 0 ? 1 : _connections[_connections.size() - 1].clientID + 1);

		std::cout << "New connection with clientID " << newConnection.clientID << std::endl;

		//Receive the username of the newly connected client
		char buffer[20];
		memset(buffer, '\0', 256);
		recv(newConnection.s, buffer, 20, 0);
		newConnection.username = buffer;

		//Store the new connection in the list
		_connections.push_back(newConnection);

		//Create a thread which processes all incoming messages from this client
		std::thread Thread(&WinsockServer::WaitForMessage, this, newConnection);
		Thread.detach();
	}
}

void WinsockServer::WaitForMessage(SocketConnection connection)
{
	while (1)
	{
		//Create a message buffer
		char buffer[256];
		memset(buffer, '\0', 256);

		//Reveive any new message from this client
		int rc = recv(connection.s, buffer, 256, 0);

		//If the connection got lost to this client we need to remove it from the active connection list
		if (rc <= 0)
		{
			//Delete the connection from the list
			for (int i = 0; i < _connections.size(); i++)
			{
				//If we have found the client in the list
				if (_connections[i].clientID == connection.clientID)
				{
					//Delete the client from the list
					std::cout << "Deleting connection to " << connection.username << " with clientID " << connection.clientID << std::endl;
					_connections.erase(_connections.begin() + i, _connections.begin() + i + 1);
					
					//Exit the thread
					return;
				}
			}
		}

		//Create a new message struct
		Message newMessage;

		//Store the raw message and the username in the message struct
		newMessage.messsage = buffer;
		newMessage.username = connection.username;
		newMessage.clientID = connection.clientID;

		//Add the message to the message queue
		_messageQueue.push_back(newMessage);
	}
}

std::vector<WinsockServer::Message>* WinsockServer::GetMessageQueue()
{
	//Return the message queue
	return &_messageQueue;
}

std::vector<WinsockServer::SocketConnection> WinsockServer::GetConnectedClients()
{
	return _connections;
}

void WinsockServer::SendToAllClients(std::string message)
{
	//Send a message to all active connection
	for (int i = 0; i < _connections.size(); i++)
	{
		send(_connections[i].s, message.c_str(), static_cast<int>(message.length()), 0);
	}
}

void WinsockServer::SendToClient(std::string message, WinsockServer::SocketConnection connection)
{
	//Send the message to the specific client
	send(connection.s, message.c_str(), static_cast<int>(message.length()), 0);
}

void WinsockServer::StartHearbeatThread()
{
	std::thread Thread(&WinsockServer::Heartbeat, this);
	Thread.detach();
}

void WinsockServer::Heartbeat()
{
	while (1)
	{
		//Send a heartbeat to every client
		for (int i = 0; i < _connections.size(); i++)
		{
			send(_connections[i].s, "alive", 5, 0);
		}

		//Wait some time
		Sleep(HEARTBEAT_TIME);
	}
}