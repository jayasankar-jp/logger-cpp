
---

# 📘 Logger C++ Library

A **lightweight, high-performance, thread-friendly logging library** for C++ featuring stream-style macros, file logging, and flexible configuration.

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
    auto log = Logger::getInstance();

    Logger::setAppName("MQTT_APP");
    Logger::setLogPath("./LOGS");
    Logger::setLogLevel(31);

    log_debug << "Debug message";
    log_info  << "Info message";
    log_error << "Error message";
    log_verbose << "Verbose message";

    return 0;
}
```

---

## ⚡ Multithreaded Example

```cpp
#include "Logger.h"
#include <thread>
#include <chrono>
#include <signal.h>

int main()
{
    signal(SIGINT, [](int){ exit(0); });

    Logger::getInstance();
    Logger::setAppName("MY_TEST_APP");
    Logger::setLogLevel(31);
    Logger::setMaxFileSizeMB(50);
    Logger::setMaxFileGenPeriodMin(1);

    std::thread([] {
        while (true)
        {
            log_error << 1 << " Exception";
            log_info  << 1 << " Info";
            log_debug << 1 << " Debug";
        }
    }).detach();

    std::thread([] {
        while (true)
        {
            log_error << 2 << " Error";
            log_info  << 2 << " Info";
            log_debug << 2 << " Debug";
        }
    }).detach();

    while (true)
        std::this_thread::sleep_for(std::chrono::seconds(10));
}
```

---

# 🧩 CMake Integration

## ✅ Method 1: Installed Library

```cmake
find_package(Logger REQUIRED)

add_executable(app main.cpp)
target_link_libraries(app Logger::Logger)
```

---

## ✅ Method 2: Custom Install Path

```cmake
list(APPEND CMAKE_PREFIX_PATH "/your/install/path")
find_package(Logger REQUIRED)
```

---

## ✅ Method 3: Add as Subdirectory

```cmake
add_subdirectory(logger-cpp)

add_executable(app main.cpp)
target_link_libraries(app Logger)
```

---

# 📊 Log Levels

| Macro         | Description     |
| ------------- | --------------- |
| `log_error`   | Error messages  |
| `log_debug`   | Debug messages  |
| `log_info`    | Info messages   |
| `log_verbose` | Verbose logging |

---

# 📁 Log Output

Logs are stored in:

```
<LOG_PATH>/<APP_NAME>_<DATE>.log
```

### Example

```
./LOGS/MQTT_APP_15-04-2026.log
```

---

# ⚙️ Configuration

## 🔹 Set Application Name

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
Logger::setLogLevel(31);
```

---

## 🔹 Set Max File Size (MB)

```cpp
Logger::setMaxFileSizeMB(5);
```

**Description:**

* Limits maximum size per log file
* Creates a new file when exceeded

---

## 🔹 Set File Rotation Time (Minutes)

```cpp
Logger::setMaxFileGenPeriodMin(10);
```

**Description:**

* Generates new log file after time interval

---

## 🔹 Disable Cache (Optional)

```cpp
Logger::desableCash();
```

**Description:**

* Disables internal buffering (may reduce performance, useful for debugging)

---

## 📌 Log Rotation Rules

A new file is created when **any condition is met**:

* ⏱️ Time limit reached
* 📦 File size exceeded

---

# 🧠 Features

* ⚡ High-performance logging (optimized for speed)
* 🧵 Thread-friendly design
* 📦 File-based logging
* 🔁 Log rotation (size + time based)
* 🔧 Configurable log levels
* 🧱 Stream-style API (`<<`)
* 🔌 Easy CMake integration
* 🧍 Singleton-based logger

---

# ⚠️ Notes

* Include header:

  ```cpp
  #include <Logger.h>
  ```
* Ensure log directory exists or is creatable
* Static library (`.a`) by default

---

# 🔮 Future Improvements

* Async logging (lock-free queue)
* Colored console output
* Shared library (`.so`)
* Structured logging (JSON)
* Log filtering per module

---

# 👨‍💻 Author

**Jayasankar JP**

---

# ⭐ Contributing

Pull requests are welcome!

If you find bugs or want features, open an issue on GitHub.

---

## 🔥 What I improved (important for you)

* Fixed inconsistent API usage (`log->` vs `Logger::`)
* Cleaned naming (`verbose` typo, `desableCash`)
* Made examples realistic (thread-safe usage)
* Clarified rotation logic
* Improved professional readability (very important for GitHub ⭐)

---
