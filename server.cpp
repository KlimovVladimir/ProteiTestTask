#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <cctype> 
#include <cstdlib> 
#include <cstring>
#include <list>

int main(int argc, char *argv[])
{
	//std::cout << __cplusplus;
    int sock, listener;
    struct sockaddr_in addr;
    char recieve_buf[1024];
    int bytes_read;

    /*if (argc != 4) {
		fprintf(stderr, "Usage:  %s <Server IP> <Server Port> <TCP/UDP>\n",
			argv[0]);
		exit(1);
	}*/

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        fprintf(stderr, "socket\n");
        exit(1);
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[1]));
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        fprintf(stderr, "bind\n");
        exit(2);
    }

    listen(listener, 1);
    
    while(1)
    {
        sock = accept(listener, NULL, NULL);
        if(sock < 0)
        {
            fprintf(stderr, "accept\n");
            exit(3);
        }

        while(1)
        {
            bytes_read = recv(sock, recieve_buf, 1024, 0);
            if(bytes_read <= 0) break;
            printf("%s\n",recieve_buf );
			
			bool has_digits = false;
			char temp_parse[1024];
			temp_parse[0] = 0;
			char temp_symb[1];
			temp_symb[0] = 0;
			std::list<int> digits;
			int temp_i = 0;
			char send_string[1024];
			send_string[0] = 0;
			int digits_summ = 0;

			for (int i = 0; recieve_buf[i] != '\0'; i++) {
				if (isdigit(recieve_buf[i])) {
					has_digits = true;
					sprintf(temp_symb, "%c", recieve_buf[i]);
					strcat(temp_parse, temp_symb);
					temp_i++;
					if(recieve_buf[i+1] == '\0')
						digits.push_back(atoi(temp_parse));
			    }
			    else if (temp_i > 0) {
			    	digits.push_back(atoi(temp_parse));
			    	temp_parse[0] = 0;
			    	temp_i = 0;
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
				printf("%s\n",send_string );
				
				send(sock, send_string, bytes_read, 0);
			}
			else
            	send(sock, recieve_buf, bytes_read, 0);
        }
    
        close(sock);
    }
    
    return 0;
}