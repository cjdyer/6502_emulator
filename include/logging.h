#ifndef __LOGGING_H__
#define __LOGGING_H__

enum class LogLevel
{
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG
};

static constexpr LogLevel current_log_level = LogLevel::LOG_DEBUG;

#define LOG_COLOR_RESET "\033[0m"

#define LOG_COLOR_WARN "\033[1;33m"  // Bold Yellow
#define LOG_COLOR_INFO "\033[1;32m"  // Bold Green
#define LOG_COLOR_DEBUG "\033[1;34m" // Bold Blue

#define LOG_WARN(msg)                            \
    if (current_log_level >= LogLevel::LOG_WARN) \
    std::cout << LOG_COLOR_WARN << "[WARNING] " << msg << LOG_COLOR_RESET << std::endl

#define LOG_INFO(msg)                            \
    if (current_log_level >= LogLevel::LOG_INFO) \
    std::cout << LOG_COLOR_INFO << "[INFO] " << msg << LOG_COLOR_RESET << std::endl

#define LOG_DEBUG(msg)                            \
    if (current_log_level >= LogLevel::LOG_DEBUG) \
    std::cout << LOG_COLOR_DEBUG << "[DEBUG] " << msg << LOG_COLOR_RESET << std::endl

#endif // __LOGGING_H__