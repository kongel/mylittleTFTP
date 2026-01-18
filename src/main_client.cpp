#include "tftp_client.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    using namespace tftp;

    if (argc < 5) {
        std::cerr << "Usage: " << argv[0]
                  << " <server_ip> <get|put> <remote_file> <local_file>\n";
        return 1;
    }

    std::string serverIp   = argv[1];
    std::string command    = argv[2];
    std::string remoteFile = argv[3];
    std::string localFile  = argv[4];

    TftpClient client(serverIp);

    try {
        if (command == "get") {
            client.downloadFile(remoteFile, localFile);
        } else if (command == "put") {
            client.uploadFile(localFile, remoteFile);
        } else {
            std::cerr << "Unknown command: " << command << "\n";
            return 1;
        }
    } catch (const TftpException& ex) {
        std::cerr << "TFTP error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}