#ifndef LOGGER_UTILS_H
#define LOGGER_UTILS_H

#include <string>
#include <ctime>

inline void fng_getCurrentTime(
    std::string &date,
    std::string &timeStr)
{
    std::time_t now = std::time(nullptr);

    std::tm ltm;

#ifdef _WIN32
    localtime_s(&ltm, &now);
#else
    localtime_r(&now, &ltm);
#endif

    // YYYY-MM-DD
    date.resize(10);

    // HH-MM-SS
    timeStr.resize(8);

    char *d = &date[0];
    char *t = &timeStr[0];

    auto twoDigit = [](char *buf, int val)
    {
        buf[0] = '0' + (val / 10);
        buf[1] = '0' + (val % 10);
    };

    auto fourDigit = [](char *buf, int val)
    {
        buf[0] = '0' + ((val / 1000) % 10);
        buf[1] = '0' + ((val / 100) % 10);
        buf[2] = '0' + ((val / 10) % 10);
        buf[3] = '0' + (val % 10);
    };

    // YYYY
    fourDigit(d + 0, ltm.tm_year + 1900);

    d[4] = '-';

    // MM
    twoDigit(d + 5, ltm.tm_mon + 1);

    d[7] = '-';

    // DD
    twoDigit(d + 8, ltm.tm_mday);

    // HH
    twoDigit(t + 0, ltm.tm_hour);

    t[2] = '-';

    // MM
    twoDigit(t + 3, ltm.tm_min);

    t[5] = '-';

    // SS
    twoDigit(t + 6, ltm.tm_sec);
}
inline std::string fng_getCurrentTime()
{
    std::time_t now = std::time(nullptr);

    std::tm ltm;

#ifdef _WIN32
    localtime_s(&ltm, &now);
#else
    localtime_r(&now, &ltm);
#endif

    char buf[32];
    int len = 0;

    auto twoDigit = [&](int val)
    {
        buf[len++] = '0' + (val / 10);
        buf[len++] = '0' + (val % 10);
    };

    auto fourDigit = [&](int val)
    {
        buf[len++] = '0' + ((val / 1000) % 10);
        buf[len++] = '0' + ((val / 100) % 10);
        buf[len++] = '0' + ((val / 10) % 10);
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

    return std::string(buf, len);
}

#endif