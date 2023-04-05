#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>


#define PACKET_SIZE         1024
#define MAX_CONNECTIONS     500


struct sockaddr_in address;
static int server_fd;
static int client_ids;
static char * cyphertext = "5e4g89?*p8we4$";


int main(int argc, char* argv[]) {
    int server_fd, client_fd;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(atoi(argv[1]));

    char recvBuff[PACKET_SIZE];
    memset(recvBuff, 0, sizeof(recvBuff)); 

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    // Attach socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    // Listen for connections
    if (listen(server_fd, MAX_CONNECTIONS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    if((client_fd = accept(server_fd, (struct sockaddr*)NULL, NULL)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    } else {
        printf("Connection accepted\n");
    }

    if(recv(client_fd, recvBuff, PACKET_SIZE, 0) < 0) {
        perror("Read failed");
        exit(EXIT_FAILURE);
    }
    printf("message received: '%s', sending reply\n", recvBuff);


    send(client_fd, cyphertext, strlen(cyphertext), 0);

    return 0;
}

