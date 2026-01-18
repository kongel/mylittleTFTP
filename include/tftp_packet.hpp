#pragma once

#include "tftp_common.hpp"
#include <vector>
#include <string>

namespace tftp {

struct TftpPacket {
    Opcode opcode{};

    // For RRQ/WRQ
    std::string filename;
    std::string mode;

    // For DATA/ACK
    uint16_t block{0};
    std::vector<uint8_t> data;

    // For ERROR
    ErrorCode errorCode{ErrorCode::UNDEFINED};
    std::string errorMsg;

    static TftpPacket makeRRQ(const std::string& filename, const std::string& mode = "octet");
    static TftpPacket makeWRQ(const std::string& filename, const std::string& mode = "octet");
    static TftpPacket makeDATA(uint16_t block, const std::vector<uint8_t>& data);
    static TftpPacket makeACK(uint16_t block);
    static TftpPacket makeERROR(ErrorCode code, const std::string& msg);

    std::vector<uint8_t> serialize() const;
    static TftpPacket parse(const uint8_t* buf, size_t len);
};

} // namespace tftp