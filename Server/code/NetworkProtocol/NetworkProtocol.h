/*! ###########################################################
 *
 * @file NetworkProtocol.h
 *
 * @author Michael Hollmer
 *
 * @date 17.04.2020
 *
########################################################### */

#pragma once
#include <iostream>
#include <fstream>
#include <string>

#include "WinsockServer\WinsockServer.h"
#include "JsonParser/JsonParser.h"

#define CAT1_PROBABILITY 0.5
#define CAT2_PROBABILITY 0.5

/*! ####################################
 * @class NetworkProtocol
 *
 * @author Michael Hollmer
 *
 * @date 13.04.2020
 *
 * @brief Handles the communication and message exchange with the clients
 *
#################################### */

class NetworkProtocol
{
	public:

        /*!
        * @brief Creates a new instance of the network protocol
        */
		NetworkProtocol();

        /*!
        * @brief Sends new rounds, checks for client responses
        */
		void Run();

	private:

        /*!
        * @brief If a new client connects it should be able to participate, so we need to send the current round
        */
        void HandleNewConnections();

        /*!
        * @brief Waits for all clients until they are ready for the next round
        */
        void WaitForClients();

        WinsockServer _winsockServer;

        unsigned int _currentRound;
        std::vector<JsonObject> _assets;

};