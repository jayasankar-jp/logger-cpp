#include "Logger.h"
#include <iostream>
std::mutex Logger::mtx;
std::shared_ptr<Logger> Logger::instance;
Logger::Logger()
{
    loglevel = 0;
    appName = "APP";
    path = "./LOGS";
    maxFileSizeMB = 50;
    curentFileSize = 0;
    fileGenPeriodMin = 60;
    setenv("TZ", "Asia/Kolkata", 1);
    tzset();
    isActiveFile = false;
}
Logger::~Logger()
{

    if (isActiveFile)
    {
        current_file.close();
    }
}
void Logger::mefn_getCurrentTime(std::string &date, std::string &timeStr)
{

    time_t now = time(nullptr);
    tm ltm;
    localtime_r(&now, &ltm);

    char date_buf[11]; // "DD-MM-YYYY"
    char time_buf[9];  // "HH:MM:SS"

    std::snprintf(date_buf, sizeof(date_buf),
                  "%02u-%02u-%04u",
                  static_cast<unsigned>(ltm.tm_mday),
                  static_cast<unsigned>(ltm.tm_mon + 1),
                  static_cast<unsigned>(ltm.tm_year + 1900));

    std::snprintf(time_buf, sizeof(time_buf),
                  "%02u:%02u:%02u",
                  static_cast<unsigned>(ltm.tm_hour),
                  static_cast<unsigned>(ltm.tm_min),
                  static_cast<unsigned>(ltm.tm_sec));

    date.assign(date_buf);
    timeStr.assign(time_buf);
}
std::string Logger::mefn_getCurrentTime()
{

    time_t now = time(0);
    tm ltm;
    localtime_r(&now, &ltm);
    char buf[64];

    std::snprintf(buf, sizeof(buf),
                  "[%02d-%02d-%04d]:[%02d:%02d:%02d]",
                  ltm.tm_mday,
                  ltm.tm_mon + 1,
                  ltm.tm_year + 1900,
                  ltm.tm_hour,
                  ltm.tm_min,
                  ltm.tm_sec);

    return std::string(buf);
}
std::string Logger::mefn_getLogType(LogLevel LOG_LEVEL)
{
    switch (LOG_LEVEL)
    {
    case LogLevel::Error:
        return "[error]";
    case LogLevel::Info:
        return "[info]";
    case LogLevel::Verbose:
        return "[verbose]";
    case LogLevel::Debug:
        return "[debug]";
    }
    return "";
}
void Logger::write(const char *file, int line, LogLevel LOG_LEVEL, const std::string &msg)
{
    // std::cout << file << std::endl;

    if (loglevel > 0)
    {
        std::lock_guard<std::mutex> lg(mtx);
        if (!isActiveFile)
        {
            try
            {
                std::string date, Time;
                lastTime = time(0);

                mefn_getCurrentTime(date, Time);
                std::string file_name = path + "/" + appName + "_" + date + "_" + Time + ".log";
                current_file.open(file_name, std::ios::app);
                isActiveFile = true;
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        if (isActiveFile)
        {

            if (loglevel & (int)LOG_LEVEL)
            {
                bool consol_e = loglevel & (int)LogLevel::Console;
                bool file_e = current_file.is_open();
                std::stringstream logbuff;

                if (consol_e || file_e)
                {
                    logbuff << "[" << appName << "]" << mefn_getCurrentTime() << mefn_getLogType(LOG_LEVEL) << "[" << file << ":" << line << "][" << msg << "]" << std::endl;
                    if (consol_e)
                        std::cout << logbuff.str();
                    if (file_e)
                    {
                        // std::cout << "Write file " << std::endl;
                        current_file << logbuff.str();
                        current_file.flush();
                        time_t current_time = time(0);
                        if (maxFileSizeMB)
                        {
                            // std::cout << "file size : " << curentFileSize << std::endl;
                            curentFileSize += logbuff.str().length();
                            if (maxFileSizeMB * 1024 * 1024 <= curentFileSize)
                            {
                                curentFileSize = 0;
                                current_file.close();
                                isActiveFile = false;
                                lastTime = current_time;
                            }
                        }
                        // std::cout << "last - curr : " << current_time - lastTime << std::endl;
                        if (current_time - lastTime > fileGenPeriodMin * 60)
                        {
                            curentFileSize = 0;
                            current_file.close();
                            isActiveFile = false;
                            lastTime = current_time;
                        }
                    }
                }
                else
                {
                    std::cout << "Faild to open log file " << std::endl;
                }
            }
        }
    }
}

std::shared_ptr<Logger> Logger::getInstance()
{
    std::lock_guard<std::mutex> lg(mtx);
    if (!instance)
    {
        instance = std::shared_ptr<Logger>(new Logger());
    }
    return instance;
    // thread-safe (C++11+)
    // return instance;
}
