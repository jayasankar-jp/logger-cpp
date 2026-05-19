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
    // std::cout << "Distructor call" << std::endl;
    mei_isShoutDown = 1;
    mec_fileWriter.mei_isShoutDown.store(true);
    if (me_writerThread.joinable())
    {
        // std::cout << "Joinable" << std::endl;
        me_writerThread.join();
        // std::cout << "joined" << std::endl;
    }
}

void Logger::fileWriter()
{
    mec_fileWriter.mcfn_writer();
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
            logbuff += fng_getCurrentTime();
            logbuff += mefn_getLogType(LOG_LEVEL);
            logbuff += "[";
            logbuff += file;
            logbuff += ":";
            logbuff += std::to_string(line);
            logbuff += "][";
            logbuff += msg;
            logbuff += "]\n";

            mec_fileWriter.mcfn_insert(consol_e, logbuff);
        }
    }
}

Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
}