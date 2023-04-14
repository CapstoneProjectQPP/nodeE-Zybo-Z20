#include <iostream>
#include <string>
#include <thread>
#include <condition_variable>
#include <chrono>
#include <cstdio>
#include <cstring>

#include "QPP_HLS.h"
#include "ClientNetworking.h"
#include "InstructionToken.h"
#include "ThreadSync.h"


static const int core_complex_port = 64998;
static const int nodeE_ID = 3003;


using namespace Xceed::ThreadSync;

int main() {

    std::cout << "                 _      _____ \n"
                 " _ __   ___   __| | ___| ____|\n"
                 "| '_ \\ / _ \\ / _` |/ _ \\  _|  \n"
                 "| | | | (_) | (_| |  __/ |___ \n"
                 "|_| |_|\\___/ \\__,_|\\___|_____|" << std::endl;

    /**
     * Buffers and sync techniques
     */
    ConcurrentBlockingDeque<ServerClient::InstructionToken> incoming_buffer;
    // std::mutex incoming_lock;
    // std::condition_variable incoming_not_empty;

    ConcurrentBlockingDeque<ServerClient::InstructionToken> outgoing_buffer;
    // std::mutex outgoing_lock;
    // std::condition_variable out_going_not_empty;


    /**
     * Networking objects
     */
    int sock;
    struct sockaddr_in socket_address;


    /**
     * Initialization of connection
     */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Fail to create socket");
        exit(-1);
    }

    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(core_complex_port);


    /**
     * Listening, parsing incoming data on this thread
     */
    std::thread incoming_thread([&] {
        ServerClient::ClientNetworking::producing(sock, core_complex_port, "192.168.1.11", &incoming_buffer,
                                                  socket_address);
    });


    /**
     * Sending processed data on this thread
     */
    std::thread outgoing_thread([&] {
       ServerClient::ClientNetworking::sending(sock, core_complex_port, "192.168.1.11", &outgoing_buffer,
                                               socket_address);
    });


    /**
     * Data pointers
     */
    Xceed::QPP_HLS* qpp_hls;
    uint8_t* seed = new uint8_t[Xceed::Constants::block_size];


    /**
     * Processing tokens on this thread
     */
    using namespace ServerClient;
    using namespace Xceed::ThreadSync;
    std::thread consumer([&] {
        while (true) {

            /**
             * Polling for token
             */
            InstructionToken acquired = incoming_buffer.block_poll();


            /**
             * Action menu switch case, as per API standard
             */
            if (acquired.api_call == "REQUEST_HANDSHAKE") {
                std::cout << "Handshaking is initiated by the Core Complex" << std::endl;

                std::string temp = "03000003";
                InstructionToken send_ID_token = InstructionToken(0, 0, "T1", "SEND_ID", 1,
                                               0, 4, temp);

                outgoing_buffer.push_back(send_ID_token);


                InstructionToken seed_token = incoming_buffer.block_poll();

                if (seed_token.api_call != "SET_SEED") {
                    std::cout << "Error handshake sequence" << std::endl;
                    exit(-1);
                }
                std::cout << "Seed received" << std::endl;
                std::memcpy(seed, seed_token.payload_content.get(), Xceed::Constants::block_size);

                std::string temp2 = "";
                InstructionToken complete_HS_token(nodeE_ID, 0, "T1", "HS_COMPLETE", 1,
                                               0, 1, temp2);

                qpp_hls = new Xceed::QPP_HLS(seed);
                outgoing_buffer.push_back(complete_HS_token);

            }
            else if (acquired.api_call == "ENCRYPT") {
                qpp_hls->setPlainText(acquired.payload_content.get(), acquired.payload_size);
                std::shared_ptr<uint8_t> result(qpp_hls->encrypt());
                // qpp->decrypt();

                InstructionToken encrypted = InstructionToken(nodeE_ID, 0, "T1", "SEND_ENCRYPTED", 1, 0,
                                                              qpp_hls->getStringLength(), result, Xceed::Constants::block_size);

                outgoing_buffer.push_back(encrypted);
            }
            else if (acquired.api_call == "DECRYPT") {
                qpp_hls->setCipherText(acquired.payload_content.get(), acquired.payload_size);
                std::shared_ptr<uint8_t> result(qpp_hls->decrypt());

                InstructionToken decrypted = InstructionToken(nodeE_ID, 0, "T1", "SEND_DECRYPTED", 1, 0,
                                                                                 qpp_hls->getStringLength(), result, qpp_hls->getStringLength());

                outgoing_buffer.push_back(decrypted);
            }
            else {
                std::cout << "Message is acquired but API call is undefined" << std::endl;
            }

        }
    });

    delete[] seed;

    incoming_thread.join();
    outgoing_thread.join();
    consumer.join();

    return 0;
}
