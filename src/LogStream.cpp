#include "LogStream.h"
#include "Logger.h"

LogStream::LogStream(const char *file, int line, LogLevel level)
    : file(file), line(line), level(level)
{
    buffer.reserve(128);
}

LogStream::~LogStream()
{
    Logger::getInstance().write(get_file_name(file), line, level, buffer);
}