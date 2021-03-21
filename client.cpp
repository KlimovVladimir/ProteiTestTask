#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <cstring>
#include <limits>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    char message[1024];
    char buf[1024];

    int sock;
    struct sockaddr_in addr;
    struct sockaddr_in fromAddr;
    unsigned int fromSize; 
    int input;
    std::string read_message;
    char protocol[10];
    int respStringLen = 0;

    if (argc != 5) {
		fprintf(stderr, "Usage:  %s <Server IP> <Server TCP Port> <Server UDP Port> <TCP/UDP connection>\n", argv[0]);
		exit(1);
	}

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock < 0)
    {
        fprintf(stderr, "socket\n");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        fprintf(stderr, "connect\n");
    }

    strcpy(protocol, argv[4]);
    send(sock, protocol, sizeof(protocol), 0);

    if(strcmp(protocol, "UDP") == 0) {
        close(sock);
        sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        addr.sin_port = htons(atoi(argv[3]));
        struct timeval read_timeout;
        read_timeout.tv_sec = 0;
        read_timeout.tv_usec = 10;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);
        if(sock < 0)
        {
            fprintf(stderr, "socket\n");
            exit(1);
        }
    }
    
    while (1)
    {
        std::cout<<"Введите номер нужного вам действия:\n";
        std::cout<<"    1 - отправить сообщение на сервер\n";
        std::cout<<"    2 - переподключиться к серверу (TCP only)\n";
        std::cout<<"    3 - завершить работу\n";
        std::cin >> input;
        switch ( input ) {
        case 1:
            std::cout<<"Введите ваше сообщение\n";
            std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
            std::getline(std::cin,read_message, '\n');
            strcpy(message,"");
            strcpy(buf,"");
            strcpy(message, read_message.c_str());

            if(strcmp(protocol, "TCP") == 0) {
                send(sock, message, sizeof(message), 0);
                recv(sock, buf, sizeof(buf), 0);
                
                std::cout<<"Сервер ответил следующее:\n";
                std::cout<< buf << "\n";
            }
            else if(strcmp(protocol, "UDP") == 0) {
                while (addr.sin_addr.s_addr != fromAddr.sin_addr.s_addr) {
                    sendto(sock, message, strlen(message), 0, (struct sockaddr *) &addr, sizeof(addr));
                    respStringLen = recvfrom(sock, buf, sizeof(message), 0, 
                                (struct sockaddr *) &fromAddr, &fromSize);
                }
                buf[respStringLen] = '\0';
                std::cout<<"Сервер ответил следующее:\n";
                std::cout<< buf << "\n";
                bzero(&fromAddr, sizeof(fromAddr));
            }
            break;
        case 2:
            if(strcmp(protocol, "TCP") == 0) {
                close(sock);
                sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
                {
                    printf("connect error\n");
                }
                send(sock, protocol, sizeof(protocol), 0);
            }

            break;
        case 3:
            close(sock);
            return 0;
            break;
        default:
            std::cout<<"Ошибка! Указан некорректный номер дейсвия.\n";
            break;
        }
    }
    
    close(sock);

    return 0;
}