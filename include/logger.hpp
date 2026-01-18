#pragma once

#include <string>
#include <mutex>
#include <iostream>
#include <chrono>
#include <iomanip>

namespace tftp {

enum class LogLevel { DEBUG, INFO, WARN, ERROR };

class Logger {
public:
    explicit Logger(LogLevel level = LogLevel::INFO)
        : level_(level) {}

    void setLevel(LogLevel level) { level_ = level; }

    template <typename... Args>
    void log(LogLevel lvl, Args&&... args) {
        if (lvl < level_) return;
        std::lock_guard<std::mutex> lock(mu_);
        printHeader(lvl);
        (std::cout << ... << args) << '\n';
    }

    template <typename... Args>
    void info(Args&&... args)  { log(LogLevel::INFO, std::forward<Args>(args)...); }
    template <typename... Args>
    void warn(Args&&... args)  { log(LogLevel::WARN, std::forward<Args>(args)...); }
    template <typename... Args>
    void error(Args&&... args) { log(LogLevel::ERROR, std::forward<Args>(args)...); }
    template <typename... Args>
    void debug(Args&&... args) { log(LogLevel::DEBUG, std::forward<Args>(args)...); }

private:
    LogLevel level_;
    std::mutex mu_;

    void printHeader(LogLevel lvl) {
        using namespace std::chrono;
        auto now   = system_clock::now();
        auto t     = system_clock::to_time_t(now);
        auto ms    = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
        std::tm tm{};
        localtime_r(&t, &tm);
        std::cout << '[' << std::put_time(&tm, "%F %T") << '.'
                  << std::setw(3) << std::setfill('0') << ms.count() << "] ";

        switch (lvl) {
        case LogLevel::DEBUG: std::cout << "[DEBUG] "; break;
        case LogLevel::INFO:  std::cout << "[INFO ] "; break;
        case LogLevel::WARN:  std::cout << "[WARN ] "; break;
        case LogLevel::ERROR: std::cout << "[ERROR] "; break;
        }
    }
};

} // namespace tftp