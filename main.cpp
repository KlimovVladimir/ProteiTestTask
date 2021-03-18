#include "Server.h"

Server server;

struct ThreadArgs {
	int sock;
};


int main (int argc, char** argv)
{
	struct ThreadArgs *threadArgs;
	char *serverIP;
	int TCP_port, UDP_port;
	int sock;
	pthread_t threadID;
	char* message;
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
            fprintf(stderr, "accept\n");
            exit(3);
        }
        while(1)
        {
            int bytes_read = server.Recieve(sock);
            if(bytes_read <= 0) break;
            message = server.Parse_message();
            server.Send(sock, message);
        }
        //if (pthread_create(&threadID, NULL, ClientThread,(void *)threadArgs) != 0) 
		//	exit(1);
	}
	return 0;
}