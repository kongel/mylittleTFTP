# 基础工作流程

基于 UDP，默认端口 69，简单文件传输协议。
客户端发起：
下载：RRQ（Read Request）
上传：WRQ（Write Request）
服务端收到 RRQ/WRQ 后：
不再用 69 端口传数据，而是开一个新的 UDP 端口跟客户端交互。
数据传输：
双方通过 DATA（带 block 编号）和 ACK（确认 block 编号）往返。
默认 block size = 512 bytes。
最后一个 DATA 包长度 < block size（通常 < 512）表示传输结束。
ERROR 报文：出现非法操作、文件不存在等情况时返回。

# UDP实现了可靠传输

TFTP 设计初衷是简单、开销小，常用在局域网、嵌入式、引导场景（比如 PXE Boot）。
UDP：
不建立连接，无握手，头部开销小，简单。
但不保证可靠性、顺序性、无重复。
本项目的“可靠性”是协议自己实现的：
数据块有序编号：DATA(block N) + ACK(block N)
超时重传机制：
发出 DATA/ACK 后等待对方响应，超时就重发（最多 N 次）。
如果重试仍失败，返回 ERROR / 终止会话。

# TFP的报文格式和OPcode

- 2 字节 Opcode（大端序）：
    1: RRQ
    - 2: WRQ
    - 3: DATA
    - 4: ACK
    - 5: ERROR
- RRQ/WRQ：
    - `Opcode (2)` + `filename (string)` + `0` + `mode (string)` + `0`
- DATA：
    - `Opcode (2)` + `block (2)` + `data (<= block_size)`
- ACK：
    - `Opcode (2)`+ `block (2)`
- ERROR：
    -`Opcode (2)` + `ErrorCode (2)` + `errMsg (string)` + `0`


# 简单多线程并发


- 主线程：
    - 在 69 端口上 `recvfrom()`，只负责接收 RRQ/WRQ 请求。
- 每次收到 RRQ/WRQ：
    - 创建一个新线程（或从线程池取一个工作线程）。
    - 线程里创建一个新的 UDP socket（不绑定特定端口，由内核分配随机端口）。
    - 使用这个 socket 与客户端完成后续 DATA/ACK 通信。
- 优点：
    - 69 端口不被长期占用在单个会话上，可同时服务多个客户端。
    - 各会话使用不同端口，避免数据包混淆。
- 可以顺带提到：
    - 控制最大并发数：比如通过计数器或线程池限制。


# 乱序处理和超时重传

socket 上设置 SO_RCVTIMEO（setsockopt）：
比如超时 3 秒。
发送一个 DATA / ACK 之后：
循环 recvfrom()：
如果超时（EAGAIN / EWOULDBLOCK），就重发上一次包。
重传次数超过 maxRetries（例如 5 次），当前会话失败，记录日志并退出。
使用 block 编号来匹配：
例如发送 DATA(block N)，只接受 ACK(block N)。
收到错误的 block 号或非法包会返回 ERROR 或忽略。


TFTP 规范里场景比较简单，一般不做乱序包的缓存，直接按 block 编号处理：
如果 DATA(block N) 已经处理过，再次收到可能是对方重传，可以选择：
重新发送 ACK(N)（比较安全的做法），避免对方一直重传。
乱序：
正常情况下 block 号是严格递增的，UDP 在小范围内也很少乱序。
若收到和预期 block 不一致的包，可以视为非法或重复，根据实现：
记录日志，可能发送 ERROR(ILLEGAL_OPERATION)。
可以强调：自己的实现重点在正确性 + 简单性，没有做 TCP 那种复杂乱序重组。