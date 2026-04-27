#include "Logger.h"
#include <iostream>
#include <cstring>
#include <signal.h>
#include <thread>
std::mutex Logger::memutexS_mu;
// std::mutex Logger::me_QueueMutex;
// std::shared_ptr<Logger> Logger::meCS_instance;
int Logger::mei_logLevel = 0;
std::string Logger::mes_appName = "APP";
std::string Logger::mes_filePath = "./LOGS";
// Queue<std::pair<bool, std::string>> Logger::meC_logQueue;
int Logger::mei_maxFileSizeMB = 50;
int Logger::mei_fileGenPeriodMin = 60;
bool Logger::meb_isCashEnable = true;
bool Logger::mei_isShoutDown = 0;
// std::condition_variable Logger::mec_Queue_cv;
// Queue<std::pair<bool, std::string>> Logger::meC_logQueue;
Logger::Logger()
{
    mei_isShoutDown = 0;
    signal(SIGINT, [](int)
           {
           Logger::mei_isShoutDown=1;
             exit(0); });

    meui_buff_len = 0;
    memset(mecs_databuffer, 0, MAX_SIZE_BUFF);
    met_CashInitialTime = time(0);

    // meb_isCashEnable = false;
    mei_bundilSizeKb = 512;
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

    // std::thread (&Logger::fileWriter, this).detach();
    me_writerThread = std::thread(&Logger::fileWriter, this);
    // std::cout << "After thread sapn" << std::endl;
    // me_writerThread.();
}

Logger::~Logger()
{
    // std::cout << "Distructor call" << std::endl;
    mei_isShoutDown = 1;
    meC_logQueue.close();
    if (me_writerThread.joinable())
    {
        // std::cout << "Joinable" << std::endl;
        me_writerThread.join();
        // std::cout << "joined" << std::endl;
    }
    if (isActiveFile)
    {
        std::lock_guard<std::mutex> lg(memutexS_mu);
        // if (mei_logLevel & (int)LogLevel::Console)
        // {
        //     std::cout << mecs_databuffer;
        // }
        // meC_current_file << mecs_databuffer;
        if (meC_current_file.is_open())
            meC_current_file.close();
        // isActiveFile = false;
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
        return "[-ER-]";
    case LogLevel::Info:
        return "[-IN-]";
    case LogLevel::Warn:
        return "[-WR-]";
    case LogLevel::Verbose:
        return "[-VB-]";
    case LogLevel::Critical:
        return "[-CR-]";
    case LogLevel::Debug:
        return "[-DB-]";
    }
    return "";
}

void Logger::write(const char *file, int line, LogLevel LOG_LEVEL, const std::string &msg)
{
    // std::cout << file << std::endl;
    // std::cout << " Log wrightimgs " << std::endl;
    // std::cout << "Log writeg";
    if (mei_isShoutDown)
    {
        return;
    }
    if (mei_logLevel > 0)
    {

        if (mei_logLevel & (int)LOG_LEVEL)
        {
            bool consol_e = mei_logLevel & (int)LogLevel::Console;

            // std::stringstream logbuff;
            time_t tl_currentTime = time(0);

            // logbuff << "[" << mes_appName << "]" << mefn_getCurrentTime() << mefn_getLogType(LOG_LEVEL) << "[" << file << ":" << line << "][" << msg << "]" << std::endl;
            bool console_e = mei_logLevel & (int)LogLevel::Console;

            std::string logbuff;
            logbuff.reserve(256);

            logbuff += "[";
            logbuff += mes_appName;
            logbuff += "]";
            logbuff += mefn_getCurrentTime();
            logbuff += mefn_getLogType(LOG_LEVEL);
            logbuff += "[";
            logbuff += file;
            logbuff += ":";
            logbuff += std::to_string(line);
            logbuff += "][";
            logbuff += msg;
            logbuff += "]\n";
            // {
            //     std::lock_guard<std::mutex> lg(me_QueueMutex);
            meC_logQueue.insert({console_e, std::move(logbuff)});
            // std::cout << "put into queue";
            // }
            // mec_Queue_cv.notify_one();
        }
    }
}

void Logger::mefn_generatefile()
{

    try
    {
        std::string date, Time;
        met_initialTime = time(0);
        met_CashInitialTime = met_initialTime;
        mefn_getCurrentTime(date, Time);
        std::string file_name;
        file_name.reserve(60);

        file_name.append(mes_filePath);
        file_name.push_back('/');
        file_name.append(mes_appName);
        file_name.push_back('_');
        file_name.append(date);
        file_name.push_back('_');
        file_name.append(Time);
        file_name.append(".log");
        meC_current_file.open(file_name, std::ios::out | std::ios::binary);
        isActiveFile = true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
void Logger::fileWriter()
{

    while (true)
    {
        bool isTimeOut = false;
        std::string msg_data;
        size_t mesg_len;
        int resQue;

        std::pair<bool, std::string> cl_pair;
        // {
        //     std::unique_lock<std::mutex> ul(me_QueueMutex);
        //     mec_Queue_cv.wait_for(ul, std::chrono::seconds(2), [this]
        //                           { return meC_logQueue.getCount() > 0 || mei_isShoutDown; });
        // }

        time_t tL_currentTime = time(0);

        if (mei_isShoutDown && meC_logQueue.getCount() == 0)
        {
            // std::cout << "end" << std::endl;
            if (meb_isCashEnable)
            {
                if (meui_buff_len > 0)
                {
                    if (!isActiveFile)
                    {
                        mefn_generatefile();
                    }
                    if (meC_current_file.is_open())
                    {
                        meC_current_file.write(mecs_databuffer, meui_buff_len);
                        // meC_current_file << mecs_databuffer;

                        // meC_current_file.write(mecs_databuffer, size);
                    }
                }
            }
            break;
        }

        resQue = meC_logQueue.getElement(cl_pair, 2000);
        // std::cout << "After waiting" << std::endl;
        if (tL_currentTime - met_CashInitialTime >= mei_CashTimeLimitSec)
        {
            // std::cout << "Cash time : " << mei_CashTimeLimitSec << std::endl;
            isTimeOut = true;
            met_CashInitialTime = tL_currentTime;
        }
        if (resQue)
        {
            msg_data = cl_pair.second;
            mesg_len = msg_data.length();
            if (cl_pair.first && !meb_isCashEnable)
            {
                std::cout << msg_data;
            }
            if (meb_isCashEnable)
            {
                memcpy(mecs_databuffer + meui_buff_len, msg_data.c_str(), mesg_len);
                meui_buff_len += mesg_len;
                mecs_databuffer[meui_buff_len] = '\0';
            }
        }
        // std::cout << "timeout happen" << std::endl;

        if (meb_isCashEnable)
        {
            if (meui_buff_len >= mei_bundilSizeKb * 1024)
            {
                if (cl_pair.first)
                    std::cout << mecs_databuffer;
                if (!isActiveFile)
                {
                    mefn_generatefile();
                }
                if (meC_current_file.is_open())
                {
                    if (meul_curentFileSize + meui_buff_len >= mei_maxFileSizeMB * 1024 * 1024)
                    {
                        meC_current_file.close();
                        meul_curentFileSize = 0;
                        mefn_generatefile();
                        if (meC_current_file.is_open())
                        {
                            // meC_current_file << mecs_databuffer;
                            meC_current_file.write(mecs_databuffer, meui_buff_len);
                            meul_curentFileSize += meui_buff_len;
                            meui_buff_len = 0;
                        }
                    }
                    else
                    {
                        // meC_current_file << mecs_databuffer;

                        meC_current_file.write(mecs_databuffer, meui_buff_len);
                        meul_curentFileSize += meui_buff_len;
                        meui_buff_len = 0;

                        meC_current_file.flush();
                        met_CashInitialTime = tL_currentTime;
                        met_initialTime = tL_currentTime;
                    }
                    if (isTimeOut)
                    {

                        meC_current_file.flush();
                        // met_initialTime = tL_currentTime;
                        // std::cout << "timeout flush 1" << std::endl;
                    }
                }
            }
            if (isTimeOut)
            {
                if (cl_pair.first)
                    std::cout << mecs_databuffer;
                if (!isActiveFile)
                {
                    if (meui_buff_len > 0)
                        mefn_generatefile();
                    isActiveFile = 1;
                }
                if (meC_current_file.is_open())
                {
                    meC_current_file.write(mecs_databuffer, meui_buff_len);
                    meul_curentFileSize += meui_buff_len;
                    meui_buff_len = 0;
                    meC_current_file.flush();
                }
                // met_CashInitialTime = tL_currentTime;
                // met_initialTime = tL_currentTime;
                // std::cout << "timeout flush 2" << std::endl;
            }
            // if(meul_curentFileSize+)
            // if(meul_curentFileSize+mei_bundilSizeKb)
            // if (meui_buff_len + mei_bundilSizeKb)
        }
        else
        {
            // std::cout << "NO cash en" << std::endl;
            if (!isActiveFile)
            {
                mefn_generatefile();
            }
            if (meC_current_file.is_open())
            {
                if (meul_curentFileSize + mesg_len <= mei_maxFileSizeMB * 1024 * 1024)
                {
                    // meC_current_file << msg_data;
                    meC_current_file.write(msg_data.c_str(), mesg_len);
                    meul_curentFileSize += mesg_len;
                }
                else
                {
                    meC_current_file.close();
                    meul_curentFileSize = 0;
                    mefn_generatefile();
                    if (meC_current_file.is_open())
                    {
                        // meC_current_file << msg_data;
                        meC_current_file.write(msg_data.c_str(), mesg_len);
                        meul_curentFileSize += mesg_len;
                    }
                }
                if (isTimeOut)
                {
                    meC_current_file.flush();
                    // met_initialTime = tL_currentTime;
                    // std::cout << "timeout flush 3";
                    mei_CashTimeLimitSec = tL_currentTime;
                }
            }
            else
            {
                std::cerr << "Faild to open file";
            }
        }
        if (isActiveFile && meC_current_file.is_open() && (tL_currentTime - met_initialTime > mei_fileGenPeriodMin * 60))
        {
            meC_current_file.close();
            meul_curentFileSize = 0;
            isActiveFile = 0;
        }
    }
}
Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
}