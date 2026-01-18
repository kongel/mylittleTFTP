#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <netinet/in.h>

namespace tftp {

class UdpSocket {
public:
    UdpSocket();
    explicit UdpSocket(int fd);
    ~UdpSocket();

    UdpSocket(const UdpSocket&) = delete;
    UdpSocket& operator=(const UdpSocket&) = delete;

    UdpSocket(UdpSocket&& other) noexcept;
    UdpSocket& operator=(UdpSocket&& other) noexcept;

    void bind(uint16_t port);
    void bindToAddress(const std::string& ip, uint16_t port);
    void setRecvTimeout(int sec);

    ssize_t sendTo(const std::vector<uint8_t>& data, const sockaddr_in& addr);
    ssize_t recvFrom(std::vector<uint8_t>& buf, sockaddr_in& addr);

    int fd() const { return fd_; }

private:
    int fd_{-1};
    void closeIfNeeded();
};

} // namespace tftp