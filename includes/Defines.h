#ifndef __LOGGER_LIB_DEFINES_H__
#define __LOGGER_LIB_DEFINES_H__
enum class LogLevel
{
    Error = 1,
    Info = 2,
    Verbose = 4,
    Debug = 8,
    Console = 16
};
#endif