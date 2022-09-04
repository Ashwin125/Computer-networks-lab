#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>

using namespace std;

//
// Data will be sent with sending its length first
// using that length receive will read received data
//
// custom send function to send data
int send_data(int sockfd, const char * buffer, size_t length) {
    // send length of the buffer
    int res = send(sockfd, &length, sizeof(size_t), 0);

    if (res == -1) {
        fprintf(stderr, "Error sending data");
        exit(1);
    }

    // send rest of the data
    return send(sockfd, buffer, length, 0);
}


int send_data(int sockfd, size_t num) {
    return send_data(sockfd, (char *) &num, sizeof(size_t));
}

int send_data(int sockfd, string data) {
    return send_data(sockfd, data.c_str(), data.size());
}

// custom receive function to recieve data
char * receive(int sockfd) {
    size_t length;
    recv(sockfd, &length, sizeof(size_t), 0);

    char * buffer = new char[length + 1];
    
    int i = 0;
    char ch;

    while (i < length) {
        recv(sockfd, &ch, sizeof(char), 0);
        buffer[i] = ch;
        i += 1;
    }

    buffer[i] = 0; // and '\0' at the end
    
    return buffer;
}

size_t receive_size_t(int sockfd) {
    // receive the response and change it to size_t
    char * response = receive(sockfd);

    size_t number = *((size_t *)response);
    delete[] response;

    return number;
}

string receiveString(int sockfd) {
    char * buffer = receive(sockfd);
    
    string res = string(buffer, buffer + strlen(buffer));
    delete[] buffer;

    return res;
}

