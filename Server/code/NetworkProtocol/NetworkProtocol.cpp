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

    _assets = JsonParser::DeserializeJson("Assets\\stringObjects.json");

    std::thread Thread(&NetworkProtocol::HandleNewConnections, this);
    Thread.detach();
}

void NetworkProtocol::Run()
{
    std::cout << "Choosing a random string..." << std::endl;

    //When a new round starts get a string from the assets
    _currentRound = rand() % _assets.size();

    //Send the new chosen string to all clients
    _winsockServer.SendToAllClients(_assets[_currentRound].GetObjectData());

    //Wait for the clients
    this->WaitForClients();
}

/* ##################### Private ##################### */

void NetworkProtocol::HandleNewConnections()
{
    //If new clients connect to the server they should participate to the current round.
    //So we send them the current string
    unsigned int lastConnected = 0;

    while (1)
    {
        std::vector<WinsockServer::SocketConnection> clients = _winsockServer.GetConnectedClients();
        if (clients.size() > 0)
        {
            if (lastConnected < clients[clients.size() - 1].clientID)
            {
                //A new client has connected -> send the current round
                _winsockServer.SendToClient(_assets[_currentRound].GetObjectData(), clients[clients.size() - 1]);
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
    //If no clients are connected to the server yet, we will wait for clients
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