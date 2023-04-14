#include "ClientNetworking.h"

void ServerClient::ClientNetworking::producing(int &sock, int port_num, const std::string &IP_string,
                                               Xceed::ThreadSync::ConcurrentBlockingDeque<InstructionToken> *incoming,
                                               struct sockaddr_in &socket_address) {


    if (inet_pton(AF_INET, IP_string.c_str(), &socket_address.sin_addr) <= 0) {
        perror("Failed to use IP address");
        exit(-1);
    }

    if (connect(sock, (struct sockaddr *)&socket_address, sizeof(socket_address)) < 0) {
        perror("Failed to connect");
        exit(-1);
    }

    char* buffer;
    while (true) {
        buffer = new char[buffer_length + 1];
        int byte_length = read(sock, buffer, buffer_length - 1);
        buffer[byte_length] = 0x00;

        if (buffer[0] == '\n') {
            continue;
        }

        InstructionToken temp = InstructionToken(buffer);
        incoming->push_back(temp);

        delete[] buffer;
    }

    close(sock);
}

void ServerClient::ClientNetworking::sending(int &sock, int port_num, const std::string &IP_string,
                                             Xceed::ThreadSync::ConcurrentBlockingDeque<InstructionToken> *outgoing,
                                             struct sockaddr_in &socket_address) {

    while(true) {

        InstructionToken acquired = outgoing->block_poll();
        std::string temp = acquired.dump();
        write(sock, temp.c_str(), temp.size());

    }

}
