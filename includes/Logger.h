#ifndef __LOGGER_LIB_H__
#define __LOGGER_LIB_H__
#include "LogStream.h"
#include <fstream>
#include <mutex>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <memory>
#define ERROR_LEVEL 1
#define INFO_LEVEL 2
#define VERBOSE_LEVEL 4
#define DEBUG_LEVEL 8
#define CONSOLE_LEVEL 16
class Logger
{
private:
    int loglevel;

    std::string appName;
    std::string path;
    int maxFileSizeMB;
    unsigned long long curentFileSize;
    int fileGenPeriodMin;

    static std::mutex mtx;
    // std::ofstream file;
    std::shared_ptr<std::ofstream> filePtr;
    // Logger *instance;
    static std::shared_ptr<Logger> instance;
    // bool isActiveFile;
    void mefn_getCurrentTime(std::string &date, std::string &timeStr);
    // void mefn_write(const std::string& msg);
    std::string mefn_getCurrentTime();
    std::string mefn_getLogType(int LOG_LEVEL);
    time_t lastTime;
    Logger();
    // static Logger instance;

public:
    ~Logger();
    void setLogLevel(int level);
    void setAppName(std::string appname);
    void setLogPath(std::string logPath);
    void setMaxFileSizeMB(int level);
    void setMaxFileGenPeriodMin(int per);
    void write(const char *file, int line, int LOG_LEVEL, const std::string &msg);
    static std::shared_ptr<Logger> getInstance();
};
inline void Logger::setMaxFileGenPeriodMin(int per)
{
    fileGenPeriodMin = per;
}
inline void Logger::setMaxFileSizeMB(int size)
{
    maxFileSizeMB = size;
}
inline void Logger::setLogLevel(int level)
{
    loglevel = level;
}
inline void Logger::setAppName(std::string appname)
{
    appName = appname;
}

inline void Logger::setLogPath(std::string logPath)
{
    path = logPath;
}
#define log_error LogStream(__FILE__, __LINE__, ERROR_LEVEL)
#define log_verbose LogStream(__FILE__, __LINE__, VERBOSE_LEVEL)
#define log_info LogStream(__FILE__, __LINE__, INFO_LEVEL)
#define log_debug LogStream(__FILE__, __LINE__, DEBUG_LEVEL)
#endif