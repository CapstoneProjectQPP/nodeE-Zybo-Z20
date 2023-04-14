#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

/* User defines */
#define PACKET_SIZE 1024

/* Private variables */
static char* plaintext = "Hello World";


int main(int argc, char* argv[]) {
    int sockfd = 0, client_fd;
    struct sockaddr_in serv_addr;


    printf("Using port %s\n", argv[1]);
    printf("Using server address %s\n", argv[2]);

    char recvBuff[PACKET_SIZE];
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));

    // Convert IPv4 and IPv6 addresses from text to binary
    if (inet_pton(AF_INET, argv[2], &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        exit(EXIT_FAILURE);
    }
    if ((client_fd = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        perror("\nConnection Failed \n");
        exit(EXIT_FAILURE);
    }

    printf("Sending plaintext: %s\n", plaintext);
    int bytes;
    if((bytes = send(sockfd, plaintext, strlen(plaintext)+1, 0)) != strlen(plaintext)+1) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }

    printf("Awaiting reply\n");

    if(recv(sockfd, recvBuff, sizeof(recvBuff)-1, 0) < 0) {
        perror("Read failed");
        exit(EXIT_FAILURE);
    }

    printf("Received reply: %s\n", recvBuff);
    return 0;
}