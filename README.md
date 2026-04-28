# 📘 Logger C++ Library

A **lightweight, high-performance, thread-friendly logging library** for C++ featuring stream-style macros, automatic log rotation, configurable log levels, file logging, console logging, and internal cache support.

---

# 🔗 Repository

```bash
git clone git@github.com:jayasankar-jp/logger-cpp.git
cd logger-cpp
```

---

# ⚙️ Installation

## ✅ Build

```bash
mkdir build
cd build
cmake ..
make
```

---

## ✅ Install

```bash
sudo make install
```

### 📦 Installed Files

| Type         | Path                           |
| ------------ | ------------------------------ |
| Library      | `/usr/local/lib/libLogger.a`   |
| Headers      | `/usr/local/include/`          |
| CMake Config | `/usr/local/lib/cmake/Logger/` |

---

# 🚀 Quick Start

## ✅ Basic Example

```cpp
#include <Logger.h>

int main()
{
    Logger::getInstance();

    Logger::setAppName("MQTT_APP");
    Logger::setLogPath("./LOGS");
    Logger::setLogLevel(127);

    log_info     << "Application Started";
    log_error    << "Connection Failed";
    log_warn     << "Retrying";
    log_debug    << "Debug Data = " << 10;
    log_critical << "Critical Error";

    return 0;
}
```

---

# ⚡ Multithreaded Example

```cpp
#include <Logger.h>
#include <thread>
#include <chrono>

int main()
{
    Logger::getInstance();

    Logger::setAppName("MY_TEST_APP");
    Logger::setLogLevel(127);
    Logger::setMaxFileSizeMB(50);
    Logger::setMaxFileGenPeriodMin(1);

    std::thread([]{
        while(true)
        {
            log_info  << "Thread 1 Running";
            log_debug << "Thread 1 Debug";
        }
    }).detach();

    std::thread([]{
        while(true)
        {
            log_warn     << "Thread 2 Warning";
            log_critical << "Thread 2 Critical";
        }
    }).detach();

    while(true)
        std::this_thread::sleep_for(std::chrono::seconds(10));
}
```

---

# 🧩 Logging Macros

```cpp
#define log_error     LogStream(__FILE__, __LINE__, LogLevel::Error)
#define log_info      LogStream(__FILE__, __LINE__, LogLevel::Info)
#define log_critical  LogStream(__FILE__, __LINE__, LogLevel::Critical)
#define log_verbose   LogStream(__FILE__, __LINE__, LogLevel::Verbose)
#define log_warn      LogStream(__FILE__, __LINE__, LogLevel::Warn)
#define log_debug     LogStream(__FILE__, __LINE__, LogLevel::Debug)
```

Each log automatically includes:

* Timestamp
* File Name
* Line Number
* Log Level
* Message

---

# 📊 Log Levels

## Enum Definition

```cpp
enum class LogLevel
{
    Error    = 1 << 0,
    Info     = 1 << 1,
    Critical = 1 << 2,
    Verbose  = 1 << 3,
    Warn     = 1 << 4,
    Debug    = 1 << 5,
    Console  = 1 << 6
};
```

---

## Bitmask Values

| Level    | Value |
| -------- | ----- |
| Error    | 1     |
| Info     | 2     |
| Critical | 4     |
| Verbose  | 8     |
| Warn     | 16    |
| Debug    | 32    |
| Console  | 64    |

---

## Examples

| Value | Enabled Logs                             |
| ----- | ---------------------------------------- |
| 1     | Error                                    |
| 3     | Error + Info                             |
| 31    | Error + Info + Critical + Verbose + Warn |
| 63    | All File Logs                            |
| 127   | All Logs + Console Output                |

---

## Recommended

```cpp
Logger::setLogLevel(127);
```

---

# 📁 Log File Output

```text
<LOG_PATH>/<APP_NAME>_<DATE>_<TIME>.log
```

### Example

```text
./LOGS/MY_TEST_APP_27-04-2026_13:40:12.log
```

---

# 🔁 Log Rotation

A new log file is created automatically when:

* 📦 File size exceeds limit
* ⏱️ Rotation time reached

---

# ⚙️ Configuration

## 🔹 Set App Name

```cpp
Logger::setAppName("MY_APP");
```

---

## 🔹 Set Log Path

```cpp
Logger::setLogPath("./LOGS");
```

---

## 🔹 Set Log Level

```cpp
Logger::setLogLevel(127);
```

---

## 🔹 Max File Size

```cpp
Logger::setMaxFileSizeMB(50);
```

---

## 🔹 Rotation Time (Minutes)

```cpp
Logger::setMaxFileGenPeriodMin(10);
```

---

## 🔹 Disable Internal Cache

```cpp
Logger::desableCash();
```

**Description:**

* Disables internal cache/buffer
* Writes logs more directly to file
* Useful for debugging
* May reduce performance under high logging load

---

# 🧠 Features

* ⚡ High-speed logging
* 🧵 Thread-friendly
* 📁 File logging
* 🖥️ Console logging
* 🔁 File rotation
* 📍 File & line info
* 🔧 Bitmask level control
* 🧱 Stream-style syntax (`<<`)
* 🔌 Easy CMake integration
* 🧍 Singleton architecture
* 💾 Internal cache support

---

# 🧩 CMake Usage

## Installed Library

```cmake
find_package(Logger REQUIRED)

add_executable(app main.cpp)
target_link_libraries(app Logger::Logger)
```

---

## Custom Path

```cmake
list(APPEND CMAKE_PREFIX_PATH "/your/install/path")
find_package(Logger REQUIRED)
```

---

## Add Subdirectory

```cmake
add_subdirectory(logger-cpp)

add_executable(app main.cpp)
target_link_libraries(app Logger)
```

---

# ⚠️ Notes

* Include:

```cpp
#include <Logger.h>
```

* Ensure log directory exists
* Static library build by default
* Use `127` for all logging

---

# 🔮 Future Improvements

* Async lock-free queue
* Shared library (`.so`)
* JSON structured logs
* Per-module filtering
* Colored console logs
* Cache tuning APIs

---

# 👨‍💻 Author

**Jayasankar JP**

---

# ⭐ Contributing

Pull requests are welcome.

If you find bugs or want new features, open an issue on GitHub.
