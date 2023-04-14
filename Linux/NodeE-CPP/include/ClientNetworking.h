#pragma once

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <mutex>
#include <deque>
#include <condition_variable>


#include "InstructionToken.h"
#include "ThreadSync.h"


namespace ServerClient {
    class ClientNetworking {
    public:
        static void producing(int &sock, int port_num, const std::string &IP_string,
                              Xceed::ThreadSync::ConcurrentBlockingDeque<InstructionToken> *incoming,
                              struct sockaddr_in &socket_address);

        static void sending(int &sock, int port_num, const std::string &IP_string,
                            Xceed::ThreadSync::ConcurrentBlockingDeque<InstructionToken> *outgoing,
                            struct sockaddr_in &socket_address);

    private:
        const static int buffer_length = 4096;
    };
}
