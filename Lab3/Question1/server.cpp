#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[]) {
    if(argc < 2) {
        fprintf(stderr, "Port Number Not Provided, Program Terminated.");
        exit(1);
    }

    int sockfd;
    int portno;

    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    socklen_t cli_len;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0) {
        fprintf(stderr, "Error Opening Socket.");
        exit(1);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        fprintf(stderr, "Binding Failed.");
        exit(1);
    }

    listen(sockfd, 5);
    cli_len = sizeof(cli_addr);
    
    fprintf(stdout, "Listening on port: %s\n", argv[1]);
    
    while (true) {
        close(accept(sockfd, (struct sockaddr*) &cli_addr, &cli_len));
    }

    close(sockfd);
    return 0;
}

