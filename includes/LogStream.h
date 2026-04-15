#ifndef __LOGSTREAM_LIB_H__
#define __LOGSTREAM_LIB_H__

#include <sstream>
#include <string>
#include "Logger.h"

// Forward declaration (important)
class Logger;

class LogStream
{
public:
    LogStream(const char *file, int line, int level);

    template <typename T>
    LogStream &operator<<(const T &value)
    {
        buffer << value;
        return *this;
    }

    ~LogStream();

private:
    std::ostringstream buffer;
    const char *file;
    int line;
    int level;
};

#endif