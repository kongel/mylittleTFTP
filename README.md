# TFTP File Transfer Tool (C++ / Linux / UDP)

A minimal yet feature-rich implementation of the Trivial File Transfer Protocol (TFTP, RFC 1350) in modern C++.  
The project provides both a TFTP server and client for file transfer over UDP under Linux.

## Features

- **Full TFTP basic protocol support**
  - RRQ (read request) / WRQ (write request)
  - DATA / ACK / ERROR packets
  - Binary mode (`octet`) file transfer
- **Reliability on top of UDP**
  - Configurable timeout and max retry count
  - Block-numbered DATA / ACK flow
  - Graceful error handling and reporting
- **Concurrent multi-session server**
  - Server listens on the standard TFTP port (default 69)
  - Each client request is handled by a dedicated worker thread and an ephemeral UDP port
- **Modern C++ design**
  - C++17, RAII wrapper for UDP sockets
  - Strongly typed enums, lightweight logger, clear class/module separation
- **Security and robustness (basic)**
  - Root directory sandbox to prevent directory traversal
  - Input validation and consistent error messages

## Project Structure

```text
.
├── include/
│   ├── tftp_common.hpp     # Common constants, enums, exception types
│   ├── tftp_packet.hpp     # TFTP packet encode/decode
│   ├── tftp_server.hpp     # TFTP server interface
│   ├── tftp_client.hpp     # TFTP client interface
│   ├── udp_socket.hpp      # RAII UDP socket wrapper
│   └── logger.hpp          # Simple thread-safe console logger
├── src/
│   ├── main_server.cpp     # Server entry point
│   ├── main_client.cpp     # Client entry point
│   └── ...                 # Class implementations
└── CMakeLists.txt
```

## Build

This project uses CMake and requires a C++17-capable compiler (e.g. `g++`).

```bash
mkdir build && cd build
cmake ..
make
```

This will generate two executables, typically:

- `tftp_server`
- `tftp_client`

## Usage

### Start the server

```bash
./tftp_server [root_dir] [port]
```

- `root_dir` (optional): directory used as the server file root  
  - default: `./tftp_root`
- `port` (optional): UDP port to listen on  
  - default: `69`

Example:

```bash
mkdir -p tftp_root
./tftp_server ./tftp_root 69
```

### Use the client

```bash
./tftp_client <server_ip> <get|put> <remote_file> <local_file>
```

- `server_ip`: IPv4 address of the TFTP server
- `get`: download `remote_file` from server to `local_file`
- `put`: upload `local_file` from client to server as `remote_file`

Examples:

```bash
# Download remote file 'test.bin' to local 'test.bin'
./tftp_client 127.0.0.1 get test.bin test.bin

# Upload local file 'data.img' as 'remote.img' to the server
./tftp_client 127.0.0.1 put remote.img data.img
```

## Implementation Highlights

- **Protocol correctness**:  
  Packets strictly follow RFC 1350 format, including opcode, block numbers, and terminating conditions (last DATA block `< block_size`).

- **Timeout & retransmission**:  
  Both client and server implement a configurable timeout mechanism; packets are retransmitted up to a maximum retry count before failing with a clear error.

- **Concurrency model**:  
  The server’s main thread is only responsible for listening on the well-known port and dispatching requests. Each RRQ/WRQ spawns a worker thread with its own UDP socket, enabling multiple independent file transfers.

- **Code quality**:  
  Clean separation between protocol logic (packet encoding/decoding), transport (UDP wrapper), and application logic (client/server). Uses RAII and modern C++ idioms for safer resource management.

## Notes

- TFTP uses UDP and is commonly restricted by firewalls; ensure the chosen port (and ephemeral ports) are allowed.
- This implementation focuses on the core protocol and reliability mechanisms. Advanced TFTP options (such as `blksize` negotiation, `tsize`, etc.) can be added as future work.

## License

This project is provided for learning and interview/demo purposes.  
You may reuse and modify it freely under your preferred license.