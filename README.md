# 📘 Logger C++ Library

A **lightweight, high-performance, thread-friendly logging library** for C++ with stream-style macros, automatic file rotation, configurable log levels, console output, and easy CMake integration.

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

## ✅ Install (System-wide)

```bash
sudo make install
```

### 📦 Installed Files

| Type         | Location                       |
| ------------ | ------------------------------ |
| Library      | `/usr/local/lib/libLogger.a`   |
| Headers      | `/usr/local/include/`          |
| CMake Config | `/usr/local/lib/cmake/Logger/` |

---

# 🚀 Usage

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
    log_warn     << "Retrying...";
    log_debug    << "Debug Value = " << 10;
    log_critical << "Critical Shutdown";

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

Each log automatically captures:

* Source file name
* Line number
* Log level
* Message text
* Timestamp

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

## Example Configurations

| Value | Enabled Levels                           |
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

Enables all logging levels including console output.

---

# 📁 Log Output

Logs are stored in:

```text
<LOG_PATH>/<APP_NAME>_<DATE>_<TIME>.log
```

### Example

```text
./LOGS/MY_TEST_APP_27-04-2026_13:20:10.log
```

---

# 🔁 Log Rotation Rules

A new file is automatically created when **any condition is met**:

* 📦 Max file size exceeded
* ⏱️ Rotation time reached

---

# ⚙️ Configuration

## 🔹 Set Application Name

```cpp
Logger::setAppName("MY_APP");
```

---

## 🔹 Set Log Directory

```cpp
Logger::setLogPath("./LOGS");
```

---

## 🔹 Set Log Level

```cpp
Logger::setLogLevel(127);
```

---

## 🔹 Set Max File Size

```cpp
Logger::setMaxFileSizeMB(50);
```

---

## 🔹 Set Rotation Interval

```cpp
Logger::setMaxFileGenPeriodMin(10);
```

---

## 🔹 Disable Internal Cache

```cpp
Logger::desableCash();
```

> Useful for debugging or immediate disk writes.
> May reduce performance.

---

# 🧠 Features

* ⚡ High-performance logging
* 🧵 Thread-friendly design
* 📁 File logging
* 🔁 Automatic rotation (time + size)
* 🖥️ Optional console output
* 📍 File & line capture
* 🔧 Bitmask log control
* 🧱 Stream-style API (`<<`)
* 🔌 Easy CMake integration
* 🧍 Singleton logger

---

# 🧩 CMake Integration

## Method 1: Installed Library

```cmake
find_package(Logger REQUIRED)

add_executable(app main.cpp)
target_link_libraries(app Logger::Logger)
```

---

## Method 2: Custom Path

```cmake
list(APPEND CMAKE_PREFIX_PATH "/your/install/path")
find_package(Logger REQUIRED)
```

---

## Method 3: Add as Subdirectory

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

* Ensure log folder exists or is creatable.
* Static library (`.a`) by default.
* Use `127` for full logging.

---

# 🔮 Future Improvements

* Async lock-free queue
* Shared library (`.so`)
* JSON structured logs
* Per-module filtering
* Colored terminal logs
* Daily compression of old logs

---

# 👨‍💻 Author

**Jayasankar JP**

---

# ⭐ Contributing

Pull requests are welcome.

If you find bugs or want new features, open an issue in the repository.
