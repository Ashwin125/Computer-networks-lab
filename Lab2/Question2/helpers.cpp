#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

//
// Data will be sent with sending its length first
// using that length receive will read received data
//

// custom send function to send data
int send_data(int sockfd, char * buffer, size_t length) {
    // send length of the buffer
    int res = send(sockfd, &length, sizeof(size_t), 0);

    if (res == -1) {
        fprintf(stderr, "Error sending data");
        exit(1);
    }

    // send rest of the data
    return send(sockfd, buffer, length, 0);
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


