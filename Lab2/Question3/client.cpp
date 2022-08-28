#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>

#include "helpers.cpp"

int main(int argc, char *argv[]) {
    int sockfd;
    int portno;
    int n;

    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    if(argc < 3) {
        fprintf(stderr, "Usage %s Hostname Port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(sockfd < 0) {
        fprintf(stderr, "Error Opening Socket.");
        exit(1);
    }

    server = gethostbyname(argv[1]);
    
    if(server == NULL) {
        fprintf(stderr, "Error, No Such Host.");
        exit(1);
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    
    if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        fprintf(stderr, "Connection Failed.");
        exit(1);
    }

    fprintf(stdout, "connected!\n");

    char * buffer = receive(sockfd);

    // if received data is send_file the send the file
    // otherwise print the recevied data
    if (strncmp(buffer, "send_file", 9) == 0) {
        // send the file
        delete[] buffer;

        FILE* f = fopen("source.c", "r");
        int size = 0;

        if (f == nullptr) {
            fprintf(stderr, "Unable to open file.\n");
            exit(1);
        }

        char ch;

        while ((ch = fgetc(f)) != EOF) {
            size += 1;
        }

        // create buffer of size length
        buffer = new char[size + 1];

        f = fopen("source.c", "r");
        int i = 0;

        while ((ch = fgetc(f)) != EOF) {
            buffer[i] = ch;
            i += 1;
        }

        buffer[i] = 0;
        send_data(sockfd, buffer, size);
       
        fprintf(stdout, "file sent!\n");
        delete[] buffer;
        fclose(f);
        // receive the output of program
        buffer = receive(sockfd);
    }
    
    fprintf(stdout, "Output: %s\n", buffer);
    delete[] buffer;

    close(sockfd);
    return 0;
}
