#include "logger.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>

class Logger {
public:
    Logger(const std::string& logFilePath) : logFilePath(logFilePath) {
        logFile.open(logFilePath, std::ios::app);
        if (!logFile.is_open()) {
            throw std::runtime_error("Unable to open log file: " + logFilePath);
        }
    }

    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void logInfo(const std::string& message) {
        log("INFO", message);
    }

    void logWarning(const std::string& message) {
        log("WARNING", message);
    }

    void logError(const std::string& message) {
        log("ERROR", message);
    }

private:
    std::ofstream logFile;
    std::string logFilePath;

    void log(const std::string& level, const std::string& message) {
        std::time_t now = std::time(nullptr);
        std::tm* localTime = std::localtime(&now);

        // Format the timestamp
        std::ostringstream timestamp;
        timestamp << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");

        // Log message
        if (logFile.is_open()) {
            logFile << "[" << timestamp.str() << "] [" << level << "] " << message << std::endl;
        }

        // Optionally log to console
        std::cout << "[" << timestamp.str() << "] [" << level << "] " << message << std::endl;
    }
};

// Example usage
int main() {
    const std::string logFilePath = "application.log"; // Replace with your desired log file path

    try {
        Logger logger(logFilePath);

        logger.logInfo("Application started.");
        logger.logWarning("This is a warning message.");
        logger.logError("This is an error message.");
    } catch (const std::exception& e) {
        std::cerr << "Logger initialization failed: " << e.what() << std::endl;
    }

    return 0;
}
