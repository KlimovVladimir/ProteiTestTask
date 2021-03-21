#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <cctype> 
#include <cstdlib> 
#include <cstring>
#include <list>
#include <pthread.h>


#define MAXPACKETSIZE 1024
#define MAX_CLIENT 100

class Server
{
	private:
	int listener_TCP_socket;
	int listener_UDP_socket;
	int coonected_socked;
	struct sockaddr_in serverAddress_TCP;
	struct sockaddr_in serverAddress_UDP;
	struct sockaddr_in clientAddress_UDP;
	unsigned int clientAddressLen;
	int recvMsgSize;
	pthread_t serverThread[ MAX_CLIENT ];
	pthread_mutex_t mutex;

	char recieve_buf[ MAXPACKETSIZE ];

	public:
	void Setup(char* serverIP, int TCP_port, int UDP_port);
	int Accept_TCP();
	void TCP_Send(int socket, char* message);
	void UDP_Send(int socket, char* message);
	char* Get_Buffer();
	int Get_UDP_Socket();
	int TCP_Recieve(int socket);
	char* UDP_Recieve(int socket);
	int TCPUDP_Recieve(int socket);
	char* Parse_message(char* buffer);
};
