#include "InstructionToken.h"

#include <utility>

ServerClient::InstructionToken::InstructionToken(const char *json_string) {

    std::string err;
    json11::Json test_obj = json11::Json::parse(json_string, err);

    sender_id = test_obj["sender_id"].int_value();
    task_id  = test_obj["task_id"].int_value();
    interface_type = test_obj["interface_type"].string_value();
    api_call = test_obj["api_call"].string_value();
    payload_total_fragments = test_obj["payload_total_fragments"].int_value();
    payload_frag_number = test_obj["payload_frag_number"].int_value();
    payload_size = test_obj["payload_size"].int_value();
    payload_hex_string = test_obj["payload_content"].string_value();

    payload_byte_length = (int)payload_hex_string.size() / 2;
    payload_content = hexStringToByteStream(payload_hex_string);
}

std::string ServerClient::InstructionToken::dump() {

    std::stringstream stringbuilder;

    stringbuilder   << "{"
                    << "\"sender_id\""                  << ":"              << this->sender_id                  << ","
                    << "\"task_id\""                    << ":"              << this->task_id                    << ","
                    << "\"interface_type\""             << ":"     << "\""  << this->interface_type << "\""     << ","
                    << "\"api_call\""                   << ":"     << "\""  << this->api_call       << "\""     << ","
                    << "\"payload_total_fragments\""    << ":"              << this->payload_total_fragments    << ","
                    << "\"payload_frag_number\""        << ":"              << this->payload_frag_number        << ","
                    << "\"payload_size\""               << ":"              << this->payload_size               << ","
                    << "\"payload_content\""            << ":" << "\""      << this->payload_hex_string << "\""
                    << "}\n";

    return stringbuilder.str();
}

ServerClient::InstructionToken::InstructionToken(int senderId, int taskId, std::string  interfaceType,
                                                 std::string apiCall, int payloadTotalFragments,
                                                 int payloadFragNumber, int payloadSize,
                                                 std::string &payloadHexString) : sender_id(senderId),
                                                                                      task_id(taskId),
                                                                                      interface_type(std::move(interfaceType)),
                                                                                      api_call(std::move(apiCall)),
                                                                                      payload_total_fragments(
                                                                                              payloadTotalFragments),
                                                                                      payload_frag_number(
                                                                                              payloadFragNumber),
                                                                                      payload_size(payloadSize),
                                                                                      payload_hex_string(std::move(payloadHexString)) {

    payload_byte_length = (int)payload_hex_string.size() / 2;

    payload_content = hexStringToByteStream(payload_hex_string);
}

ServerClient::InstructionToken::InstructionToken(int senderId, int taskId, std::string interfaceType,
                                                 std::string apiCall, int payloadTotalFragments, int payloadFragNumber,
                                                 int payloadSize, std::shared_ptr<uint8_t> payloadContent, int payloadByteLength) : sender_id(senderId),
                                                                                                                                    task_id(taskId),
                                                                                                                                    interface_type(std::move(interfaceType)),
                                                                                                                                    api_call(std::move(apiCall)),
                                                                                                                                    payload_total_fragments(
                                                                                                                            payloadTotalFragments),
                                                                                                                                    payload_frag_number(
                                                                                                                            payloadFragNumber),
                                                                                                                                    payload_size(payloadSize),
                                                                                                                                    payload_content(payloadContent),
                                                                                                                                    payload_byte_length(payloadByteLength) {


    payload_hex_string = byteStreamToHexString(payload_content, payload_byte_length);

}

std::string ServerClient::InstructionToken::byteStreamToHexString(std::shared_ptr<uint8_t> byte_stream, int count) {

    std::stringstream stringbuilder;

    for (int i = 0; i < count; i++) {
        std::stringstream ss;
        uint8_t a_byte = byte_stream.get()[i];

        ss << std::hex << std::setw(2) << std::setfill('0') << (int)a_byte;
        std::string temp = ss.str();
        stringbuilder << temp;
    }

    return stringbuilder.str();
}

std::shared_ptr<uint8_t> ServerClient::InstructionToken::hexStringToByteStream(std::string &hex_representation) {

    auto result = std::shared_ptr<uint8_t>(new uint8_t [hex_representation.size() / 2]);
    for (int i = 0; i < hex_representation.size(); i += 2) {
        uint8_t a_byte = (uint8_t) strtoul(hex_representation.substr(i, 2).c_str(), nullptr, 16);
        result.get()[i/2] = a_byte;
    }

    return result;
}
