#ifndef __LOGSTREAM_LIB_H__
#define __LOGSTREAM_LIB_H__

#include <string>
#include <string_view>
#include <sstream>
#include <type_traits>
#include <cstdint>
#include "Defines.h"

class Logger;

constexpr const char* get_file_name(const char* path)
{
    if (!path) return "";
    const char* file = path;
    while (*path)
    {
        if (*path == '/' || *path == '\\')
        {
            file = path + 1;
        }
        path++;
    }
    return file;
}

class LogStream
{
public:
    LogStream(const char *file, int line, LogLevel level);
    ~LogStream();

    LogStream &operator<<(const char *val)
    {
        if (val) buffer.append(val);
        return *this;
    }

    LogStream &operator<<(const std::string &val)
    {
        buffer.append(val);
        return *this;
    }

    LogStream &operator<<(std::string_view val)
    {
        buffer.append(val);
        return *this;
    }

    LogStream &operator<<(char val)
    {
        buffer.push_back(val);
        return *this;
    }

    LogStream &operator<<(bool val)
    {
        buffer.append(val ? "true" : "false");
        return *this;
    }

    LogStream &operator<<(int val)
    {
        appendInt(val);
        return *this;
    }

    LogStream &operator<<(unsigned int val)
    {
        appendInt(val);
        return *this;
    }

    LogStream &operator<<(long val)
    {
        appendInt(val);
        return *this;
    }

    LogStream &operator<<(unsigned long val)
    {
        appendInt(val);
        return *this;
    }

    LogStream &operator<<(long long val)
    {
        appendInt(val);
        return *this;
    }

    LogStream &operator<<(unsigned long long val)
    {
        appendInt(val);
        return *this;
    }

    LogStream &operator<<(double val)
    {
        buffer.append(std::to_string(val));
        return *this;
    }

    LogStream &operator<<(float val)
    {
        buffer.append(std::to_string(val));
        return *this;
    }

    template <typename T>
    LogStream &operator<<(const T &value)
    {
        std::ostringstream ss;
        ss << value;
        buffer.append(ss.str());
        return *this;
    }

private:
    template <typename T>
    void appendInt(T val)
    {
        char buf[32];
        char *p = buf + sizeof(buf);
        if constexpr (std::is_signed_v<T>) {
            if (val < 0) {
                uint64_t uval = static_cast<uint64_t>(-(val + 1)) + 1;
                do {
                    *--p = '0' + static_cast<char>(uval % 10);
                    uval /= 10;
                } while (uval > 0);
                *--p = '-';
                buffer.append(p, buf + sizeof(buf) - p);
                return;
            }
        }
        uint64_t uval = static_cast<uint64_t>(val);
        do {
            *--p = '0' + static_cast<char>(uval % 10);
            uval /= 10;
        } while (uval > 0);
        buffer.append(p, buf + sizeof(buf) - p);
    }

    std::string buffer;
    const char *file;
    int line;
    LogLevel level;
};

#endif