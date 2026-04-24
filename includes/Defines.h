#ifndef __LOGGER_LIB_DEFINES_H__
#define __LOGGER_LIB_DEFINES_H__
enum class LogLevel
{
    Error = 1 << 0,
    Info = 1 << 1,
    Critical = 1 << 2,
    Verbose = 1 << 3,
    Warn = 1 << 4,
    Debug = 1 << 5,
    Console = 1 << 6
};
#endif