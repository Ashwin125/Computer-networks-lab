#include <stdio.h>
#include <algorithm>
#include <array>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <string>
#include <vector>
#include <numeric>
#include <random>

#include "helpers.cpp"
#include "make_chunks.cpp"

using namespace std;

array<size_t, 5> five_random_number_less_than_10() {
    srand(time(NULL));

    array<size_t, 10> nums;
    iota(nums.begin(), nums.end(), 0);
    
    // shuffle the array
    random_device rd;
    mt19937 g(rd());
    shuffle(nums.begin(), nums.end(), g);

    array<size_t, 5> res;

    for (int i = 0; i < 5; ++i) {
        res[i] = nums[i];
    }

    return res;
}

/**
 * send random five chunks to the client
 */
void handleClient(int sockfd) {
    // get random five chunks
    array<size_t, 5> chunknumbers = five_random_number_less_than_10();

    // get all the chunks
    array<string, 10> chunks = load_and_divide_file();

    // send those 5 random chunks
    for (size_t i = 0; i < 5; ++i) {
        size_t chunk_number = chunknumbers[i];
        
        fprintf(stdout, "Sending chunk number: %ld\n", chunk_number);
        
        // send chunk number
        send_data(sockfd, chunk_number);

        // send the actual data
        send_data(sockfd, chunks[chunk_number]);
    }
    
    // receive the thanks message
    char* res = receive(sockfd);

    fprintf(stdout, "Message: %s\n", res);
    delete[] res;
    fprintf(stdout, "Disconnected sockfd: %d\n", sockfd);
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        fprintf(stderr, "Port Number Not Provided, Program Terminated.\n");
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
        fprintf(stderr, "Error Opening Socket.\n");
        exit(1);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
    if(::bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        fprintf(stderr, "Binding Failed.\n");
        exit(1);
    }

    listen(sockfd, 5);

    fprintf(stdout, "Listening to port: %d\n", portno);

    cli_len = sizeof(cli_addr);
    
    newsockfd = accept(sockfd, (struct sockaddr *) & cli_addr, &cli_len);

    if (newsockfd < 0) {
        fprintf(stderr, "Error On Accept.\n");
        exit(1);
    }

    fprintf(stdout, "Connected to sockfd: %d\n", newsockfd);
    handleClient(newsockfd);
    
    close(newsockfd);
    close(sockfd);
    return 0;
}

