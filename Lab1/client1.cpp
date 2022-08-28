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
    
    char buffer[0x100];

    fprintf(stdout, "Enter the string: ");
    fgets(buffer, 0x100, stdin);

    buffer[strcspn(buffer, "\n")] = 0;
    fprintf(stdout, "Sending: %s\n", buffer);
    send_data(sockfd, buffer, strlen(buffer));

    close(sockfd);
    return 0;
}
