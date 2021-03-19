#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string>
#include <cstring>
#include <limits>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

char message[] = "20 apples, 30 bananas, 15 peaches and 1 watermelon asd1das1das1sad1";

char buf[sizeof(message)];

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in addr;
    int input;
    std::string read_message;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        fprintf(stderr, "socket\n");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        fprintf(stderr, "connect\n");
        exit(2);
    }
    
    while (1)
    {
        std::cout<<"Введите номер нужного вам действия:\n";
        std::cout<<"    1 - отправить сообщение на сервер\n";
        std::cout<<"    2 - переподключиться к серверу\n";
        std::cout<<"    3 - завершить работу\n";
        std::cin >> input;
        switch ( input ) {
        case 1:
            std::cout<<"Введите ваше сообщение\n";
            std::cin.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
            std::getline(std::cin,read_message, '\n');

            strcpy(message, read_message.c_str());
            send(sock, message, sizeof(message), 0);
            recv(sock, buf, sizeof(message), 0);
            
            std::cout<<"Сервер ответил следующее:\n";
            std::cout<< buf << "\n";
            break;
        case 2:
            printf("coming soon...\n");
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
    
    send(sock, message, sizeof(message), 0);
    recv(sock, buf, sizeof(message), 0);
    
    printf(buf);
    close(sock);

    return 0;
}