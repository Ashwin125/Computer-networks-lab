#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "helpers.cpp"

int main(int argc, char *argv[]) {
    if(argc < 2) {
        fprintf(stderr, "Port Number Not Provided, Program Terminated.");
        exit(1);
    }

    int sockfd;
    int newsockfd;
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
    

    // first connection to client1
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len);
    
    if(newsockfd < 0) {
        fprintf(stderr, "Error On Accept.");
        exit(1);
    }
    
    // first input
    char * buffer = receive(newsockfd);

    fprintf(stdout, "Received: %s\n", buffer);
    
    // close the first connection
    close(newsockfd);

    // get the second connection
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len);

    // increment the data
    for (int i = 0; i < strlen(buffer); ++i) {
        buffer[i] += 1;
    }

    fprintf(stdout, "Sending: %s\n", buffer);
    send_data(newsockfd, buffer, strlen(buffer));

    delete[] buffer;

    // close the second connection
    close(newsockfd);
    close(sockfd);
    return 0;
}

