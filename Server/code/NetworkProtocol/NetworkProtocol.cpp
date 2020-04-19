/*! ###########################################################
 *
 * @file NetworkProtocol.cpp
 *
 * @author Michael Hollmer
 *
 * @date 17.04.2020
 *
 * @brief Implements the method protoypes from NetworkProtocol.h
########################################################### */

#include "NetworkProtocol.h"

NetworkProtocol::NetworkProtocol()
{
    _winsockServer.Create();
    _winsockServer.StartConnectionThread();

    this->LoadAssets();

    std::thread Thread(&NetworkProtocol::HandleNewConnections, this);
    Thread.detach();
}

void NetworkProtocol::Run()
{
    std::cout << "Choosing a random string..." << std::endl;

    //When a new round starts get a string from the assets
    int category = rand() % 2;

    switch (category)
    {
        case 0: {

            //Get a string from category 1 
            _currentRound = _cat1[rand() % _cat1.size()];

            //Send the string to all connected clients
            _winsockServer.SendToAllClients(_currentRound);

            //Wait for response of all clients
            this->WaitForClients();
        }
              break;
        case 1: {

            //Get a string from category 2
            _currentRound = _cat2[rand() % _cat2.size()];

            //Send the string to all connected clients
            _winsockServer.SendToAllClients(_currentRound);

            //Wait for response of all clients
            this->WaitForClients();
        }
              break;
    }
}

/* ##################### Private ##################### */

void NetworkProtocol::LoadAssets()
{
    std::ifstream read("Assets\\cat1.dat");
    _cat1.push_back(std::string());
    while (read.good())
    {
        char c = read.get();
        if (c == '\n')
        {
            _cat1.push_back(std::string());
        }
        else
        {
            _cat1[_cat1.size() - 1].push_back(c);
        }
    }

    std::ifstream read2("Assets\\cat2.dat");
    _cat2.push_back(std::string());
    while (read2.good())
    {
        char c = read2.get();
        if (c == '\n')
        {
            _cat2.push_back(std::string());
        }
        else
        {
            _cat2[_cat2.size() - 1].push_back(c);
        }
    }
}

void NetworkProtocol::HandleNewConnections()
{
    unsigned int lastConnected = 0;

    while (1)
    {
        std::vector<WinsockServer::SocketConnection> clients = _winsockServer.GetConnectedClients();
        if (clients.size() > 0)
        {
            if (lastConnected != clients[clients.size() - 1].clientID)
            {
                //A new client has connected -> send the current round
                _winsockServer.SendToClient(_currentRound, clients[clients.size() - 1]);
                lastConnected = clients[clients.size() - 1].clientID;
            }
        }

        Sleep(500);
    }
}

void NetworkProtocol::WaitForClients()
{    
    std::cout << "Waiting for clients..." << std::endl;

    std::vector<WinsockServer::SocketConnection> clients = _winsockServer.GetConnectedClients();
    
    //Wait for clients to connect to the server
    while (clients.size() == 0)
    {
        clients = _winsockServer.GetConnectedClients();
        Sleep(500);
    }

    //While there are still clients who aren't ready
    while(clients.size() != 0)
    {
        //In the mean time, new clients could have connected to the server, so we need to update our list
        clients = _winsockServer.GetConnectedClients();
       
        //Now check if every client send a message. We assume that the message means the client has read the current round and is ready for
        //the next one
        if (clients.size() == _winsockServer.GetMessageQueue()->size())
        {
            //Delete the message queue
            _winsockServer.GetMessageQueue()->clear();

            return;
        }

        Sleep(500);
    
    }

    std::cout << "Clients are ready!" << std::endl;
}