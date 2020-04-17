/*! ###########################################################
 *
 * @file WinsockServer.h
 *
 * @author Michael Hollmer
 *
 * @date 13.04.2020
 *
########################################################### */

#pragma once
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <thread>
#include <vector>
#include <ctime>
#pragma comment(lib, "Ws2_32.lib")

/*! ####################################
 * @class WinsockServer
 *
 * @author Michael Hollmer
 *
 * @date 13.04.2020
 *
 * @brief The WinsockServer class implements the basic functions required to handle mulitple client connections at once
 *
#################################### */

#define HEARTBEAT_TIME 5000

class WinsockServer
{
	public:

		/*!
		* @brief A SocketConnection struct which stores the socket, the user name and a unique identifier 
		*/
		struct SocketConnection
		{
			SOCKET s;
			std::string username;
			unsigned int clientID;
		};

		/*!
		* @brief A Message struct which stores the message data and the username from the client, who send the data
		*/
		struct Message
		{
			std::string username;
			std::string messsage;
			unsigned int clientID;
		};

		WinsockServer();

		/*!
		* @brief Startsup the server
		*/
		void Create();

		/*!
		* @brief Starts a thread which handles new incoming clients and messages
		*/
		void StartConnectionThread();

		/*!
		* @brief Sends a message to all clients
		*/
		void SendToAllClients(std::string message);

		/*!
		* @brief Returns a list of the connected clients
		*/
		void SendToClient(std::string message, SocketConnection connection);

		/*!
		* @brief Returns a pointer to the message queue
		*/
		std::vector<Message>* GetMessageQueue();

		/*!
		* @brief Returns a list of the connected clients
		*/
		std::vector<SocketConnection> GetConnectedClients();

		/*!
		* @brief Starts a heartbeat thread which sends a small message after a fixed time to every client
		*/
		void StartHearbeatThread();

	private:

		/*!
		* @brief Waits for incoming connections (executed as a thread from the StartConnectionThread() method)
		*/
		void WaitForConnection();

		/*!
		* @brief Waits for incoming messages. This method is started as a thread for every client
		*/
		void WaitForMessage(SocketConnection connection);

		/*!
		* @brief Sends a small message after a fixed time to every client
		*/
		void Heartbeat();

		/*!
		* @brief The server socket to which the clients will establish their connection
		*/
		SOCKET _serverSocket;

		/*!
		* @brief A list of all running connections
		*/
		std::vector<SocketConnection> _connections;

		/*!
		* @brief The message queue
		*/
		std::vector<Message> _messageQueue;
		
};