#pragma once

#include "tftp_common.hpp"
#include "udp_socket.hpp"
#include "logger.hpp"
#include <string>

namespace tftp {

class TftpClient {
public:
    TftpClient(std::string serverIp, uint16_t serverPort = DEFAULT_PORT,
               int timeoutSec = DEFAULT_TIMEOUT_SEC,
               int maxRetries = DEFAULT_MAX_RETRIES);

    void downloadFile(const std::string& remoteFile,
                      const std::string& localFile);

    void uploadFile(const std::string& localFile,
                    const std::string& remoteFile);

private:
    std::string serverIp_;
    uint16_t serverPort_;
    int timeoutSec_;
    int maxRetries_;
    Logger logger_{LogLevel::INFO};
};

} // namespace tftp