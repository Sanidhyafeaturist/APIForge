#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <stdexcept>

class Logger {
public:
    Logger(const std::string& logFilePath);
    ~Logger();
    void logInfo(const std::string& message);
    void logWarning(const std::string& message);
    void logError(const std::string& message);

private:
    std::ofstream logFile;
    std::string logFilePath;
    void log(const std::string& level, const std::string& message);
};

#endif // LOGGER_H
