#include <iostream>
#include <fstream>
#include <stdio.h>
#include <algorithm>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>
#include <vector>

#include "helpers.cpp"
using namespace std;

int connectServer(char* host, char* portnumber) {
    int sockfd;
    int portno;

    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    portno = atoi(portnumber);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(sockfd < 0) {
        fprintf(stderr, "Error Opening Socket.");
        exit(1);
    }

    server = gethostbyname(host);
    
    if(server == NULL) {
        fprintf(stderr, "Error, No Such Host.");
        exit(1);
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        fprintf(stderr, "Connection Failed.");
        exit(1);      
    }

    return sockfd;
}

int main(int argc, char *argv[]) {
    if(argc < 4) {
        fprintf(stderr, "Usage %s Hostname server-A-port server-B-port\n", argv[0]);
        exit(0);
    }

    // connect to server_A 
    int sockfd_A = connectServer(argv[1], argv[2]);
    fprintf(stdout, "Connected to server A.\n");

    // receive 5 of the chunks
    fprintf(stdout, "Receiving the first five chunks.\n");
    
    vector<pair<size_t, string>> chunks;

    for (int i = 0; i < 5; ++i) {
        // first receive the chunk number
        size_t chunk = receive_size_t(sockfd_A);

        fprintf(stdout, "Receiving chunk number: %ld..\n", chunk);
        
        pair<size_t, string> res;

        // then receive the actual chunk
        res.first = chunk;
        res.second = receiveString(sockfd_A);

        chunks.push_back(res);
    }

    // connect to server_B
    int sockfd_B = connectServer(argv[1], argv[3]);
    fprintf(stdout, "Connected to server B.\n");
    
    fprintf(stdout, "Receiving the missing chunks.\n");

    // receive the rest of the 5 chunks
    // by simply asking for that chunk
    for (int i = 0; i < 10; ++i) {
        if (find_if(chunks.begin(), chunks.end(), [=] (auto val) { return val.first == i; }) != chunks.end()) {
            continue;
        }
        
        size_t chunk_number = i;

        fprintf(stdout, "Asking for chunk no: %ld\n", chunk_number);
        
        // send request for chunk number id
        send_data(sockfd_B, (size_t) chunk_number);
        
        // receive the missing chunk
        string chunk = receiveString(sockfd_B);
        
        pair<size_t, string> res;

        res.first = chunk_number;
        res.second = chunk;

        chunks.push_back(res);
        fprintf(stdout, "Chunk no %ld received.\n", chunk_number);
    }

    fprintf(stdout, "Writing to file..\n");
    // write the received chunks to file
    ofstream fout("target.txt");

    sort(chunks.begin(), chunks.end(), [] (auto a, auto b) {
        return a.first < b.first;
    });

    for (auto [i, data]: chunks) {
        fout << data;
    }

    fout.close();
    
    fprintf(stdout, "File saved.\n");
    fprintf(stdout, "File name: target.txt\n");
    
    // send the thanks message
    string message = "THANKS";
    
    send_data(sockfd_A, message);
    send_data(sockfd_B, message);
        
    fprintf(stdout, "Thanks sent!\n");

    close(sockfd_A);
    close(sockfd_B);
    return 0;
}
