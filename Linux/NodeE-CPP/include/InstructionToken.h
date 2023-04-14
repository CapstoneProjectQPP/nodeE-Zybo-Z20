#pragma once

#include <string>
#include <sstream>
#include <cstdio>
#include <iomanip>
#include "json11.hpp"


namespace ServerClient {
    class InstructionToken {
    public:

        int sender_id;
        int task_id;
        std::string interface_type;
        std::string api_call;
        int payload_total_fragments;
        int payload_frag_number;
        int payload_size;
        std::string payload_hex_string;
        std::shared_ptr<uint8_t> payload_content;
        int payload_byte_length;

        InstructionToken(const char* json_string);

        InstructionToken(int senderId, int taskId, std::string interfaceType, std::string apiCall,
                         int payloadTotalFragments, int payloadFragNumber, int payloadSize,
                         std::string &payloadHexString);

        InstructionToken(int senderId, int taskId, std::string interfaceType, std::string apiCall,
                         int payloadTotalFragments, int payloadFragNumber, int payloadSize,
                         std::shared_ptr<uint8_t> payloadContent, int payloadByteLength);


        static std::string byteStreamToHexString(std::shared_ptr<uint8_t> byte_stream, int count);

        static std::shared_ptr<uint8_t> hexStringToByteStream(std::string &hex_representation);

        std::string dump();
    };
}
