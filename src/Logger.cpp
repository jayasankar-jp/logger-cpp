#include "Logger.h"
#include <iostream>
#include <thread>
#include "LoggerUtils.h"

int Logger::mei_logLevel = 0;
std::string Logger::mes_appName = "APP";

bool Logger::mei_isShoutDown = 0;
FileWriter Logger::mec_fileWriter;

Logger::Logger()
{
    mei_isShoutDown = 0;

#ifdef _WIN32
    _putenv_s("TZ", "India Standard Time");
    _tzset();
#else
    setenv("TZ", "Asia/Kolkata", 1);
    tzset();
#endif

    me_writerThread = std::thread(&Logger::fileWriter, this);
}

Logger::~Logger()
{
    mei_isShoutDown = 1;
    mec_fileWriter.mei_isShoutDown.store(true);
    if (me_writerThread.joinable())
    {
        me_writerThread.join();
    }
}

void Logger::fileWriter()
{
    mec_fileWriter.mcfn_writer();
}

const char* Logger::mefn_getLogType(LogLevel LOG_LEVEL)
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
    default:
        return "";
    }
}

void Logger::write(const char *file, int line, LogLevel LOG_LEVEL, const std::string &msg)
{
    if (mei_isShoutDown)
    {
        return;
    }
    if (mei_logLevel > 0)
    {
        if (mei_logLevel & static_cast<int>(LOG_LEVEL))
        {
            bool consol_e = mei_logLevel & static_cast<int>(LogLevel::Console);

            std::string logbuff;
            logbuff.reserve(mes_appName.length() + msg.length() + 64);

            logbuff.push_back('[');
            logbuff.append(mes_appName);
            logbuff.push_back(']');
            fng_formatCurrentTime(logbuff);
            logbuff.append(mefn_getLogType(LOG_LEVEL));
            logbuff.push_back('[');
            logbuff.append(file ? file : "");
            logbuff.push_back(':');

            char lineBuf[16];
            char *p = lineBuf + sizeof(lineBuf);
            int l = line;
            do {
                *--p = '0' + static_cast<char>(l % 10);
                l /= 10;
            } while (l > 0);
            logbuff.append(p, lineBuf + sizeof(lineBuf) - p);

            logbuff.append("][");
            logbuff.append(msg);
            logbuff.append("]\n");

            mec_fileWriter.mcfn_insert(consol_e, logbuff);
        }
    }
}

Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
}