#include "Logger.h"
#include <iostream>
#include <cstring>
#include <signal.h>
std::mutex Logger::memutexS_mu;
// std::shared_ptr<Logger> Logger::meCS_instance;
int Logger::mei_logLevel = 0;
std::string Logger::mes_appName = "APP";
std::string Logger::mes_filePath = "./LOGS";
int Logger::mei_maxFileSizeMB = 50;
int Logger::mei_fileGenPeriodMin = 60;
bool Logger::meb_isCashEnable = true;
Logger::Logger()
{
    signal(SIGINT, [](int)
           { exit(0); });

    meui_buff_len = 0;
    memset(mecs_databuffer, 0, MAX_SIZE_BUFF);
    met_CashInitialTime = time(0);

    // meb_isCashEnable = false;
    mei_bundilSizeKb = 128;
    mei_CashTimeLimitSec = 3;

    // mei_maxFileSizeMB = 50;
    meul_curentFileSize = 0;

#ifdef _WIN32
    _putenv_s("TZ", "India Standard Time");
    _tzset();
#else
    setenv("TZ", "Asia/Kolkata", 1);
    tzset();
#endif
    isActiveFile = false;
}
Logger::~Logger()
{
    // std::cout << "Distructor call";
    std::lock_guard<std::mutex> lg(memutexS_mu);
    if (isActiveFile)
    {
        if (mei_logLevel & (int)LogLevel::Console)
        {
            std::cout << mecs_databuffer;
        }
        meC_current_file << mecs_databuffer;
        meC_current_file.close();
        isActiveFile = false;
    }
}
void Logger::mefn_getCurrentTime(std::string &date, std::string &timeStr)
{
    time_t now = time(nullptr);

    tm ltm;

#ifdef _WIN32
    localtime_s(&ltm, &now);
#else
    localtime_r(&now, &ltm);
#endif

    // Pre-size strings (avoid realloc + strlen)
    date.resize(10);   // "DD-MM-YYYY"
    timeStr.resize(8); // "HH:MM:SS"

    // ---- DATE ----
    char *d = date.data();

    auto twoDigit = [](char *buf, int val)
    {
        buf[0] = '0' + (val / 10);
        buf[1] = '0' + (val % 10);
    };

    auto fourDigit = [](char *buf, int val)
    {
        buf[0] = '0' + (val / 1000) % 10;
        buf[1] = '0' + (val / 100) % 10;
        buf[2] = '0' + (val / 10) % 10;
        buf[3] = '0' + (val % 10);
    };

    twoDigit(d + 0, ltm.tm_mday);
    d[2] = '-';
    twoDigit(d + 3, ltm.tm_mon + 1);
    d[5] = '-';
    fourDigit(d + 6, ltm.tm_year + 1900);

    // ---- TIME ----
    char *t = timeStr.data();

    twoDigit(t + 0, ltm.tm_hour);
    t[2] = ':';
    twoDigit(t + 3, ltm.tm_min);
    t[5] = ':';
    twoDigit(t + 6, ltm.tm_sec);
}

std::string Logger::mefn_getCurrentTime()
{
    time_t now = time(nullptr);

    tm ltm;

#ifdef _WIN32
    localtime_s(&ltm, &now);
#else
    localtime_r(&now, &ltm);
#endif

    char buf[32]; // exact size needed

    // Manual formatting (FASTER than snprintf)
    int len = 0;

    auto twoDigit = [&](int val)
    {
        buf[len++] = '0' + (val / 10);
        buf[len++] = '0' + (val % 10);
    };

    auto fourDigit = [&](int val)
    {
        buf[len++] = '0' + (val / 1000) % 10;
        buf[len++] = '0' + (val / 100) % 10;
        buf[len++] = '0' + (val / 10) % 10;
        buf[len++] = '0' + (val % 10);
    };

    buf[len++] = '[';
    twoDigit(ltm.tm_mday);
    buf[len++] = '-';
    twoDigit(ltm.tm_mon + 1);
    buf[len++] = '-';
    fourDigit(ltm.tm_year + 1900);
    buf[len++] = ']';
    buf[len++] = ':';
    buf[len++] = '[';
    twoDigit(ltm.tm_hour);
    buf[len++] = ':';
    twoDigit(ltm.tm_min);
    buf[len++] = ':';
    twoDigit(ltm.tm_sec);
    buf[len++] = ']';

    return std::string(buf, len); // no strlen scan
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
    if (mei_logLevel > 0)
    {
        std::lock_guard<std::mutex> lg(memutexS_mu);
        if (!isActiveFile)
        {
            try
            {
                std::string date, Time;
                met_initialTime = time(0);

                mefn_getCurrentTime(date, Time);
                std::string file_name = mes_filePath + "/" + mes_appName + "_" + date + "_" + Time + ".log";
                meC_current_file.open(file_name, std::ios::app);
                isActiveFile = true;
            }
            catch (const std::exception &e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        if (isActiveFile)
        {

            if (mei_logLevel & (int)LOG_LEVEL)
            {
                bool consol_e = mei_logLevel & (int)LogLevel::Console;
                bool file_e = meC_current_file.is_open();
                std::stringstream logbuff;
                time_t tl_currentTime = time(0);
                if (consol_e || file_e)
                {
                    logbuff << "[" << mes_appName << "]" << mefn_getCurrentTime() << mefn_getLogType(LOG_LEVEL) << "[" << file << ":" << line << "][" << msg << "]" << std::endl;
                    if (meb_isCashEnable)
                    {

                        std::string tempBuf = logbuff.str();

                        // strcat(mecs_databuffer, tempBuf.c_str());
                        // meui_buff_len += tempBuf.length();
                        memcpy(mecs_databuffer + meui_buff_len, tempBuf.data(), tempBuf.size());
                        meui_buff_len += tempBuf.size();
                        mecs_databuffer[meui_buff_len] = '\0'; // maintain null termination

                        if (meui_buff_len >= mei_bundilSizeKb * 1000 || tl_currentTime - met_CashInitialTime > mei_CashTimeLimitSec)
                        {
                            if (consol_e)
                                std::cout << mecs_databuffer;
                            if (file_e)
                            {
                                meC_current_file << mecs_databuffer;
                                meC_current_file.flush();
                                meul_curentFileSize += meui_buff_len;
                            }
                            met_CashInitialTime = tl_currentTime;
                            meui_buff_len = 0;
                            memset(mecs_databuffer, 0, MAX_SIZE_BUFF);
                        }
                    }
                    else
                    {
                        if (consol_e)
                            std::cout << logbuff.str();
                        if (file_e)
                        {
                            meC_current_file << logbuff.str();
                            if (tl_currentTime - met_CashInitialTime > mei_CashTimeLimitSec)
                            {
                                meC_current_file.flush();
                                met_CashInitialTime = tl_currentTime;
                            }

                            meul_curentFileSize += logbuff.str().length();
                        }
                    }

                    if (file_e)
                    {
                        // std::cout << "Write file " << std::endl;

                        // time_t current_time = time(0);
                        if (mei_maxFileSizeMB)
                        {
                            // std::cout << "file size : " << meul_curentFileSize << std::endl;

                            if (mei_maxFileSizeMB * 1024 * 1024 <= meul_curentFileSize)
                            {
                                meul_curentFileSize = 0;
                                meC_current_file.close();
                                isActiveFile = false;
                                met_initialTime = tl_currentTime;
                            }
                        }
                        // std::cout << "last - curr : " << current_time - met_initialTime << std::endl;
                        if (tl_currentTime - met_initialTime > mei_fileGenPeriodMin * 60)
                        {
                            meul_curentFileSize = 0;
                            meC_current_file.close();
                            isActiveFile = false;
                            met_initialTime = tl_currentTime;
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
}

Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
}