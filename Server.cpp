#include "Server.h"

void Server::Setup(char* serverIP,int TCP_port, int UDP_port) {

    recieve_buf[0] = 0;
    listener_TCP_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(listener_TCP_socket < 0)
    {
        fprintf(stderr, "socket\n");
        exit(1);
    }

    serverAddress.sin_family = AF_INET;
    //serverAddress.sin_addr.s_addr = htonl(inet_addr(serverIP));
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(TCP_port);
    if(bind(listener_TCP_socket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        fprintf(stderr, "bind\n");
        exit(2);
    }

    listen(listener_TCP_socket, MAX_CLIENT);
}

int Server::Accept_TCP()
{
    int sock = accept(listener_TCP_socket, NULL, NULL);
    return sock;
}

void Server::Send(int socket, char* message)
{
    send(socket, message, strlen(message), 0);
}

char* Server::Get_Buffer()
{
    return recieve_buf;
}

int Server::Recieve(int socket)
{
    int bytes_read;
    bytes_read = recv(socket, recieve_buf, MAXPACKETSIZE, 0);
    printf("Recieve message: %s\n",recieve_buf );
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