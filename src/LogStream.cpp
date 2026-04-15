#include "LogStream.h"
#include "Logger.h"
#include "iostream"
#include <filesystem>
LogStream::LogStream(const char *file, int line, int level)
    : file(file), line(line), level(level)
{
    // std::cout << "console trigger" << std::endl;
}

LogStream::~LogStream()
{
    std::filesystem::path p(file);
    Logger::getInstance()->write(p.filename().c_str(), line, level, buffer.str());
}