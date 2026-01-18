#include "tftp_server.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    using namespace tftp;

    uint16_t port = DEFAULT_PORT;
    std::string rootDir = "./tftp_root";

    if (argc >= 2) {
        rootDir = argv[1];
    }
    if (argc >= 3) {
        port = static_cast<uint16_t>(std::stoi(argv[2]));
    }

    std::cout << "Starting TFTP server on port " << port
              << " with root dir: " << rootDir << "\n";

    TftpServer server(port, rootDir);
    server.start();
    return 0;
}