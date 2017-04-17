#include "TicTacToe.h"
#include <WinSock2.h>
#include <iostream>

// getServers.cpp
//  Given a socket, a broadcast address and a port number, this function returns the number
//  of servers as well as an array of structs.  Each struct contains the name, IP_Address and 
//  port number of a remote server.

//	IN parameters:
//	SOCKET s				= Allocate UDP socket handle that can be used for communcations
//	char *broadcastAddress	= The broadcast address that should be used for current LAN
//	char *broadcastPort		= The port that should be used for the broadcast

//	OUT parameters:
//	ServerStruct server[]	= An array that contains the structs.  The members of each struct
//							  contain the name, IP Address and port number of remote server.
//	numServers				= Number of elements in the server[] array

//	Function return value	= Number of remote servers (size of server[] array)

int getServers(SOCKET s, char *broadcastAddress, char *broadcastPort, ServerStruct server[])
{
	// This function returns the number of TicTacToe servers that may be found within the current Broadcast Domain.
	// The last parameter, server[], will contain all of the servers' names, IP addresses and port numbers.

	int numServers = 0;
	char recvBuf[MAX_RECV_BUF];
	char host[MAX_RECV_BUF];
	char port[MAX_RECV_BUF];
	// Send TicTacToe_QUERY to broadcastAddress using broadcastPort
/****			
Task 3: Add code here that will send the TicTacToe_QUERY message to the broadcastAddress using the broadcastPort (see function header).
****/

	int charsSent;
	charsSent = UDP_send(s, TicTacToe_QUERY, strlen(TicTacToe_QUERY)+1, broadcastAddress, broadcastPort);
	// Receive incoming UDP datagrams (with a maximum of 2 second wait before each UDP_recv() function call
	// As you read datagrams, if they start with the prefix: TicTacToe_NAME, parse out the server's name
	// and add the name, host address and port number to the server[] array.  Don't forget to increment numServers.
	int status = wait(s,2,0);
	if (status > 0) {
		int len;
/****			
Task 4a: Add code here that will receive a response to the broadcast message
****/		
		len = UDP_recv(s, recvBuf, sizeof(recvBuf), host, port);

		while (status > 0 && len > 0) {
/****			
Task 4b: Inside this while loop, extract a response, which should be a C string that looks like "Name=some server's name".
		 If the response doesn't begin with the characters, "Name=", ignore it.
		 If it does begin with the characters, "Name=", parse the actual name that follows and
		 (i) assign that name to the array of structs, server[numServers].name
		 (ii) assign the IP Address from which the response originated to server[numServers].host
		 (iii) assign the server's port number to server[numServers].port
		 (iv) increment numServers
****/

			if (strncmp(recvBuf, TicTacToe_NAME, strlen(TicTacToe_NAME)) == 0)
			{
				char serverName[80];
				
				int i = 0;
				while (recvBuf[i] != '=')
					i++;
				i++;
				int j = 0;
				while (recvBuf[i] != '\0')
				{
					serverName[j] = recvBuf[i];
					i++;
					j++;
				}
				serverName[j] = '\0';
				server[numServers].name = serverName;
				server[numServers].host = host;
				server[numServers].port = port;

				numServers++;
				// Now, we'll see if there is another response.
				status = wait(s, 2, 0);
				if (status > 0)
					len = UDP_recv(s, recvBuf, sizeof(recvBuf), host, port);
			}
		}
	}
	return numServers;
}
