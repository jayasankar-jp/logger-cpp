#ifndef __LOGGER_LIB_H__
#define __LOGGER_LIB_H__
#include "Defines.h"
#include "LogStream.h"
#include <fstream>
#include <mutex>
#ifdef _WIN32
#include <cstdlib>

#else
#include <unistd.h>
#endif
#include <ctime>
#include <sstream>
#include <iomanip>
#include <memory>
#define MAX_SIZE_BUFF 1025 * 512
#include <Queue.h>
#include <thread>
#include "FileWriter.h"
class Logger
{
private:
    static int mei_logLevel;
    static std::string mes_appName;
    static std::string mes_filePath;
    static int mei_maxFileSizeMB;
    static int mei_fileGenPeriodMin;
    static bool meb_isCashEnable;

    static bool mei_isShoutDown;

    std::thread me_writerThread;

    std::string mefn_getLogType(LogLevel LOG_LEVEL);
    time_t met_initialTime;
    static FileWriter mec_fileWriter;
    Logger();

    // static Logger meCS_instance;

public:
    ~Logger();
    static void setLogLevel(int level);
    static void setAppName(std::string appname);
    static void setLogPath(std::string logPath);
    static void setMaxFileSizeMB(int level);
    static void setMaxFileGenPeriodMin(int per);
    static void desableCash();
    void fileWriter();
    void write(const char *file, int line, LogLevel LOG_LEVEL, const std::string &msg);
    // static std::shared_ptr<Logger> getInstance();
    static Logger &getInstance();
};
inline void Logger::desableCash()
{
    // meb_isCashEnable = false;
    mec_fileWriter.meb_isCashEnable = false;
}
inline void Logger::setMaxFileGenPeriodMin(int per)
{
    // mei_fileGenPeriodMin = per;
    mec_fileWriter.mei_fileGenPeriodMin = per;
}
inline void Logger::setMaxFileSizeMB(int size)
{
    mec_fileWriter.mei_maxFileSizeMB = size;
}
inline void Logger::setLogLevel(int level)
{
    mei_logLevel = level;
}
inline void Logger::setAppName(std::string appname)
{
    mes_appName = appname;
    mec_fileWriter.mes_appName = appname;
}

inline void Logger::setLogPath(std::string logPath)
{
    mec_fileWriter.mes_filePath = logPath;
}
#define log_error LogStream(__FILE__, __LINE__, LogLevel::Error)
#define log_info LogStream(__FILE__, __LINE__, LogLevel::Info)
#define log_critical LogStream(__FILE__, __LINE__, LogLevel::Critical)
#define log_verbose LogStream(__FILE__, __LINE__, LogLevel::Verbose)
#define log_warn LogStream(__FILE__, __LINE__, LogLevel::Warn)
#define log_debug LogStream(__FILE__, __LINE__, LogLevel::Debug)
#endif