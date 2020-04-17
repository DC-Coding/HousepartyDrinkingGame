/*! ###########################################################
 *
 * @file main.cpp
 *
 * @author Michael Hollmer
 *
 * @date 13.04.2020
 *
########################################################### */

#include <iostream>
#include "NetworkProtocol/NetworkProtocol.h"

int main()
{
    srand(static_cast<unsigned int>(time(NULL)));

    NetworkProtocol protocol = NetworkProtocol();
    
    while (1)
    {
        protocol.Run();
    }

	system("pause");
	return 0;
}