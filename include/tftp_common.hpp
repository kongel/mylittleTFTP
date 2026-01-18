#pragma once

#include <cstdint>
#include <string>
#include <stdexcept>

namespace tftp {

constexpr uint16_t DEFAULT_PORT = 69;
constexpr size_t   DEFAULT_BLOCK_SIZE = 512;
constexpr int      DEFAULT_TIMEOUT_SEC = 3;
constexpr int      DEFAULT_MAX_RETRIES = 5;

enum class Opcode : uint16_t {
    RRQ   = 1,
    WRQ   = 2,
    DATA  = 3,
    ACK   = 4,
    ERROR = 5,
};

enum class ErrorCode : uint16_t {
    UNDEFINED           = 0,
    FILE_NOT_FOUND      = 1,
    ACCESS_VIOLATION    = 2,
    DISK_FULL           = 3,
    ILLEGAL_OPERATION   = 4,
    UNKNOWN_TID         = 5,
    FILE_EXISTS         = 6,
    NO_SUCH_USER        = 7,
};

struct TftpException : public std::runtime_error {
    explicit TftpException(const std::string& msg)
        : std::runtime_error(msg) {}
};

} // namespace tftp