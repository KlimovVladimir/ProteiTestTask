#include "Server.h"

void Server::Setup(char* serverIP,int TCP_port, int UDP_port) {

    recieve_buf[0] = 0;
    listener_TCP_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listener_TCP_socket < 0)
    {
        fprintf(stderr, "socket\n");
        exit(1);
    }

    listener_UDP_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(listener_UDP_socket < 0)
    {
        fprintf(stderr, "socket\n");
        exit(1);
    }

    serverAddress_TCP.sin_family = AF_INET;
    serverAddress_TCP.sin_addr.s_addr = inet_addr(serverIP);
    //serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress_TCP.sin_port = htons(TCP_port);
    if(bind(listener_TCP_socket, (struct sockaddr *)&serverAddress_TCP, sizeof(serverAddress_TCP)) < 0)
    {
        fprintf(stderr, "bind\n");
        exit(2);
    }

    serverAddress_UDP.sin_family = AF_INET;
    serverAddress_UDP.sin_addr.s_addr = inet_addr(serverIP);
    serverAddress_UDP.sin_port = htons(UDP_port);
    if(bind(listener_UDP_socket, (struct sockaddr *)&serverAddress_UDP, sizeof(serverAddress_UDP)) < 0)
    {
        fprintf(stderr, "bind\n");
        exit(2);
    }

    listen(listener_TCP_socket, MAX_CLIENT);
}

int Server::Accept_TCP()
{
    int sock = accept(listener_TCP_socket, NULL, NULL);
    printf("Client connected!\n");
    return sock;
}

void Server::TCP_Send(int socket, char* message)
{
    send(socket, message, strlen(message), 0);
}

void Server::UDP_Send(int socket, char* message)
{
    sendto(socket, message, strlen(message), 0, 
             (struct sockaddr *) &clientAddress_UDP, sizeof(clientAddress_UDP));
}

char* Server::Get_Buffer()
{
    return recieve_buf;
}

int Server::Get_UDP_Socket()
{
    return listener_UDP_socket;
}

int Server::TCP_Recieve(int socket)
{
    int bytes_read;
    bytes_read = recv(socket, recieve_buf, MAXPACKETSIZE, 0);
    printf("Recieve message: %s\n",recieve_buf );
    return bytes_read;
}

char* Server::UDP_Recieve(int socket)
{
    char* ret;
    char Buffer[MAXPACKETSIZE];
    int bytes_read;
    recvMsgSize = recvfrom(socket, Buffer, MAXPACKETSIZE, 0,
        (struct sockaddr *) &clientAddress_UDP, &clientAddressLen);
    Buffer[recvMsgSize] = '\0';
    printf("Recieve message: %s\n",Buffer );
    ret = Buffer;
    return ret;
}

int Server::TCPUDP_Recieve(int socket)
{
    int bytes_read;
    char recieveTCPUDP[ 10 ];
    bytes_read = recv(socket, recieveTCPUDP, 10, 0);
    if (strcmp(recieveTCPUDP, "TCP") == 0) {
        return 1;
    }
    else if (strcmp(recieveTCPUDP, "UDP") == 0) {
        return 2;
    }
    return bytes_read;
}

char* Server::Parse_message(char* buffer)
{
    bool has_digits = false;
    char temp_parse[1024] = "";
    char temp_symb[3];
    sprintf(temp_symb, "%c", buffer[0]);
    std::list<int> digits;
    int temp_i = 0;
    char send_string[1024];
    send_string[0] = 0;
    int digits_summ = 0;
    char* ret;

    for (int i = 0; buffer[i] != '\0'; i++) {
#ifdef Trace
        printf("Symb: %c\n",buffer[i]);
#endif
        if (isdigit(buffer[i])) {
            has_digits = true;
            sprintf(temp_symb, "%c", buffer[i]);
#ifdef Trace
            printf("temp_symb: %s\n",temp_symb);
#endif
            strcat(temp_parse, temp_symb);
#ifdef Trace
            printf("temp_parse: %s\n",temp_parse);
#endif
            if(buffer[i+1] == '\0' || !isdigit(buffer[i+1])) {
                digits.push_back(atoi(temp_parse));
#ifdef Trace
                printf("List: %d\n",digits.back());
#endif
                temp_parse[0] = 0;
            }
        }
    }

    if (has_digits) {
        digits.sort();
        int digit_size = digits.size();
        for(int i = 0; i < digit_size; i++) {
            sprintf(temp_parse, "%d", digits.front());
            strcat(send_string, temp_parse);
            strcat(send_string, " ");
            digits_summ += digits.front();
            digits.pop_front();
        }

        strcat(send_string, "\n");
        sprintf(temp_parse, "%d", digits_summ);
        strcat(send_string, temp_parse);
        strcat(send_string, "\n");
        printf("Parsed message: %s\n",send_string );

        ret = send_string;
        return ret;
    }

    ret = buffer;
    return ret;
        
}