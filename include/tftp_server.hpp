#pragma once

#include "tftp_common.hpp"
#include "udp_socket.hpp"
#include "logger.hpp"
#include <string>
#include <thread>
#include <atomic>
#include <vector>

namespace tftp {

class TftpServer {
public:
    TftpServer(uint16_t port, std::string rootDir,
               int timeoutSec = DEFAULT_TIMEOUT_SEC,
               int maxRetries = DEFAULT_MAX_RETRIES);

    void start();   // 阻塞运行，监听并处理请求
    void stop();    // 停止服务器（可选）

private:
    uint16_t port_;
    std::string rootDir_;
    int timeoutSec_;
    int maxRetries_;
    std::atomic<bool> running_{false};
    UdpSocket socket_;
    Logger logger_{LogLevel::INFO};
    std::vector<std::thread> workers_;

    void handleLoop();
    void handleRequest(const std::vector<uint8_t>& buf,
                       const sockaddr_in& clientAddr);

    void serveReadRequest(const std::string& filename,
                          const sockaddr_in& clientAddr);
    void serveWriteRequest(const std::string& filename,
                           const sockaddr_in& clientAddr);

    std::string sanitizePath(const std::string& filename);
};

} // namespace tftp