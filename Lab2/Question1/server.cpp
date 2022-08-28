#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "helpers.cpp"
#include "postfix.cpp"

int main(int argc, char *argv[]) {
    if(argc < 2) {
        fprintf(stderr, "Port Number Not Provided, Program Terminated.");
        exit(1);
    }

    int sockfd;
    int newsockfd;
    int portno;
    int n;
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
    
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len);
    
    if(newsockfd < 0) {
        fprintf(stderr, "Error On Accept.");
        exit(1);
    }

    // we have socket now transfer info using send and recv function
    bool go_on = true;
    char load[0x100];

    fprintf(stdout, "Connected\n");
    
    while (go_on) {
        // receive data from the server
        char * data = receive(newsockfd);

        if (strncmp(data, "q", 1) == 0) {
            go_on = false;
        } else {
            // evaluate expression
            // send response
            
            string res = solvePostfix(data);
            strncpy(load, res.c_str(), res.size());

            send_data(newsockfd, load, strlen(load));
        }
    }

    close(newsockfd);
    close(sockfd);
    return 0;
}

