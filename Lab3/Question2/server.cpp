#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <string>
#include <vector>

#include "helpers.cpp"

using namespace std;

void handleClient(int sockfd) {
    // get total number of strings
    char* buffer = receive(sockfd);

    fprintf(stdout, "Received: %s\n", buffer);

    // send the same data back
    
    send_data(sockfd, buffer, strlen(buffer));

    fprintf(stdout, "disconnected sockfd: %d\n", sockfd);
    close(sockfd);
}

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

    fprintf(stdout, "Listening to port: %d\n", portno);

    cli_len = sizeof(cli_addr);
    
    // receive connection and then start thread
    // to handle that connection
    newsockfd = accept(sockfd, (struct sockaddr *) & cli_addr, &cli_len);

    if (newsockfd < 0) {
        fprintf(stderr, "Error On Accept.");
        exit(1);
    }

    fprintf(stdout, "Connected to sockfd: %d\n", newsockfd);
    
    handleClient(newsockfd);

    close(sockfd);
    return 0;
}

