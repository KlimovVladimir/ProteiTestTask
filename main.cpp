#include "Server.h"

Server server;

struct ThreadArgs {
	int sock;
};
pthread_mutex_t mutex;

void *ClientTCPThread(void *threadArgs)
{
	int sock;
	char* message;
	char* buffer;
	sock = ((struct ThreadArgs *)threadArgs)->sock;
	pthread_detach(pthread_self());

	while(1) 
	{
		int bytes_read = server.TCP_Recieve(sock);
		pthread_mutex_lock(&mutex);
		buffer = 0;
		message = 0;
		buffer = server.Get_Buffer();
        if(bytes_read <= 0) {
        	pthread_mutex_unlock(&mutex);
        	break;
        }
        else {
        	message = server.Parse_message(buffer);
        	pthread_mutex_unlock(&mutex);
        }
        server.TCP_Send(sock, message);
	}
	printf("Client disconnected!\n");
	return NULL;
}

void *ClientUDPThread(void *threadArgs)
{
	int sock = server.Get_UDP_Socket();
	char* message;
	char* buffer;
	pthread_detach(pthread_self());

	while(1) 
	{
		buffer = server.UDP_Recieve(sock);
		message = server.Parse_message(buffer);
		server.UDP_Send(sock, message);
	}
	return NULL;
}


int main (int argc, char** argv)
{
	struct ThreadArgs *threadArgs;
	char *serverIP;
	int TCP_port, UDP_port;
	int sock;
	pthread_t threadID;
	bool UDP_created = false;
	if (argc != 4) {
		fprintf(stderr, "Usage:  %s <Server IP> <Server Port TCP> <Server Port UDP>\n", argv[0]);
		exit(1);
	}

	serverIP = argv[1];
	TCP_port = atoi(argv[2]);
	UDP_port = atoi(argv[3]);
	server.Setup(serverIP, TCP_port, UDP_port);

	while(1)
    {
        sock = server.Accept_TCP();
        threadArgs->sock = sock;
        if(sock < 0)
        {
            printf("accept error\n");
        }
        else 
        {
        	int protocol_bufferd = server.TCPUDP_Recieve(sock);
			if (protocol_bufferd == 1) {
				pthread_create(&threadID, NULL, ClientTCPThread,(void *)threadArgs);
			}
			else if (protocol_bufferd == 2) {
				if (UDP_created == false) {
					pthread_create(&threadID, NULL, ClientUDPThread,NULL);
					UDP_created = true;
				}
			}
			else {
				printf("wrong protocol, thread not created\n");
			}
		}	
	}
	return 0;
}