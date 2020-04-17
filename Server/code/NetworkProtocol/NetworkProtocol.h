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

#include "..\WinsockServer\WinsockServer.h"

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
        * @brief Loads all the application data
        */
        void LoadAssets();

        /*!
        * @brief If a new client connects it should be able to participate, so we need to send the current round
        */
        void HandleNewConnections();

        /*!
        * @brief Waits for all clients until they are ready for the next round
        */
        void WaitForClients();

        WinsockServer _winsockServer;

        std::vector<std::string> _cat1;
        std::vector<std::string> _cat2;

        std::string _currentRound;

};