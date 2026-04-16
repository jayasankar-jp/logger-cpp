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
    // isActiveFile = false;
}
Logger::~Logger()
{

    if (filePtr)
    {
        filePtr->close();
    }
    filePtr.reset();
}
void Logger::mefn_getCurrentTime(std::string &date, std::string &timeStr)
{
    setenv("TZ", "Asia/Kolkata", 1);
    tzset();
    time_t now = time(0);
    tm *ltm = localtime(&now);

    std::stringstream d, t;

    // Format: DD-MM-YYYY
    d << std::setw(2) << std::setfill('0') << ltm->tm_mday << "-"
      << std::setw(2) << std::setfill('0') << (ltm->tm_mon + 1) << "-"
      << (1900 + ltm->tm_year);

    // Format: HH:MM:SS
    t << std::setw(2) << std::setfill('0') << ltm->tm_hour << ":"
      << std::setw(2) << std::setfill('0') << ltm->tm_min << ":"
      << std::setw(2) << std::setfill('0') << ltm->tm_sec;

    date = d.str();
    timeStr = t.str();
}
std::string Logger::mefn_getCurrentTime()
{
    setenv("TZ", "Asia/Kolkata", 1);
    tzset();
    time_t now = time(0);
    tm *ltm = localtime(&now);

    std::stringstream data;

    // Format: DD-MM-YYYY
    data << "[" << std::setw(2) << std::setfill('0') << ltm->tm_mday << "-"
         << std::setw(2) << std::setfill('0') << (ltm->tm_mon + 1) << "-"
         << (1900 + ltm->tm_year) << "]:["
         << std::setw(2) << std::setfill('0') << ltm->tm_hour << ":"
         << std::setw(2) << std::setfill('0') << ltm->tm_min << ":"
         << std::setw(2) << std::setfill('0') << ltm->tm_sec << "]";

    return data.str();
}
std::string Logger::mefn_getLogType(int LOG_LEVEL)
{
    switch (LOG_LEVEL)
    {
    case 1:
        return "[error]";
    case 2:
        return "[info]";
    case 4:
        return "[verbose]";
    case 8:
        return "[debug]";
    }
    return "";
}
void Logger::write(const char *file, int line, int LOG_LEVEL, const std::string &msg)
{
    // std::cout << file << std::endl;
    std::lock_guard<std::mutex> lg(mtx);
    if (loglevel > 0)
    {
        if (!filePtr)
        {
            try
            {
                std::string date, Time;
                lastTime = time(0);

                mefn_getCurrentTime(date, Time);
                std::string file_name = path + "/" + appName + "_" + date + "_" + Time + ".log";
                // std::cout << file_name << std::endl;
                filePtr = std::make_shared<std::ofstream>();

                filePtr->open(file_name, std::ios::app);
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        if (filePtr)
        {

            // if (loglevel & CONSOLE_LEVEL)
            // {
            //     std::cout <<mefn_getLogType()
            // }
            if (loglevel & LOG_LEVEL)
            {
                bool consol_e = loglevel & CONSOLE_LEVEL;
                bool file_e = filePtr->is_open();
                std::stringstream logbuff;

                if (consol_e || file_e)
                {
                    logbuff << "[" << appName << "]" << mefn_getCurrentTime() << mefn_getLogType(LOG_LEVEL) << "[" << file << ":" << line << "][" << msg << "]" << std::endl;
                    if (consol_e)
                        std::cout << logbuff.str();
                    if (file_e)
                    {
                        // std::cout << "Write file " << std::endl;
                        (*filePtr) << logbuff.str();
                        filePtr->flush();
                        time_t current_time = time(0);
                        if (maxFileSizeMB)
                        {
                            // std::cout << "file size : " << curentFileSize << std::endl;
                            curentFileSize += logbuff.str().length();
                            if (maxFileSizeMB * 1024 * 1024 <= curentFileSize)
                            {
                                curentFileSize = 0;
                                filePtr->close();
                                filePtr.reset();
                                lastTime = current_time;
                            }
                        }
                        // std::cout << "last - curr : " << current_time - lastTime << std::endl;
                        if (current_time - lastTime > fileGenPeriodMin * 60)
                        {
                            curentFileSize = 0;
                            filePtr->close();
                            filePtr.reset();
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
