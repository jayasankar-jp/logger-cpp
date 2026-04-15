# 📘 Logger C++ Library

A lightweight, thread-friendly logging library for C++ with stream-style macros, file logging, and configurable log levels.

---

## 🔗 Repository

```bash
git clone git@github.com:jayasankar-jp/logger-cpp.git
```

---

# ⚙️ Installation

## ✅ 1. Clone the repository

```bash
git clone git@github.com:jayasankar-jp/logger-cpp.git
cd logger-cpp
```

---

## ✅ 2. Build

```bash
mkdir build
cd build
cmake ..
make
```

---

## ✅ 3. Install (System-wide)

```bash
sudo make install
```

### 📦 Installed files

| Type         | Location                       |
| ------------ | ------------------------------ |
| Library      | `/usr/local/lib/libLogger.a`   |
| Headers      | `/usr/local/include/`          |
| CMake Config | `/usr/local/lib/cmake/Logger/` |

---

# 🚀 Usage

## ✅ Example Code

```cpp
#include <Logger.h>

int main() {
    std::shared_ptr<Logger> log = Logger::getInstance();

    log->setAppName("MQTT_APP");
    log->setLogPath("./LOGS");
    log->setLogLevel(31);

    log_debug << "Test debug print";
    log_error << "ERROR msg";
    log_verbos << "verbose message";
    log_info  << "My info";

    return 0;
}
```

---

# 🧩 Using in Another Project (CMake)

## ✅ Method 1: Installed Library (Recommended)

### CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.15)
project(MyApp)

find_package(Logger REQUIRED)

add_executable(app main.cpp)

target_link_libraries(app Logger::Logger)
```

---

## ▶️ Build

```bash
mkdir build
cd build
cmake ..
make
./app
```

---

## ✅ Method 2: If Installed in Custom Path

```cmake
list(APPEND CMAKE_PREFIX_PATH "/your/install/path")

find_package(Logger REQUIRED)
```

---

## ✅ Method 3: Use Without Installing

```cmake
add_subdirectory(logger-cpp)

add_executable(app main.cpp)

target_link_libraries(app Logger)
```

---

# 📊 Log Levels

| Macro        | Description     |
| ------------ | --------------- |
| `log_error`  | Error messages  |
| `log_debug`  | Debug messages  |
| `log_info`   | Informational   |
| `log_verbose` | Verbose logging |

---

# 📁 Log Output

Logs are stored in:

```
./LOGS/<APP_NAME>_<DATE>.log
```

### Example:

```
LOGS/MQTT_APP_15-04-2026.log
```

Here’s your updated **Configuration section** with the new options added, clean and consistent:

---

# ⚙️ Configuration

## 🔹 Set Application Name

```cpp
log->setAppName("MY_APP");
```

## 🔹 Set Log Path

```cpp
log->setLogPath("./LOGS");
```

## 🔹 Set Log Level

```cpp
log->setLogLevel(31);
```

## 🔹 Set Max File Generation Period (Minutes)

```cpp
log->setMaxFileGenPeriodMin(10);
```

**Description:**

* Creates a new log file after the specified time interval.
* Example: `10` → new file every 10 minutes.

---

## 🔹 Set Max File Size (MB)

```cpp
log->setMaxFileSizeMB(5);
```

**Description:**

* Limits how large a log file can grow.
* When exceeded, a new log file is created.
* Example: `5` → max 5 MB per file.

---

## 📌 Notes

* Log rotation happens when **either condition is met**:

  * ⏱️ Time limit (`setMaxFileGenPeriodMin`)
  * 📦 Size limit (`setMaxFileSizeMB`)

---



# 🧠 Features

* Singleton Logger instance
* Stream-style logging (`<<`)
* File-based logging
* Configurable log levels
* Easy integration with CMake

---

# ⚠️ Notes

* Include header as:

  ```cpp
  #include <Logger.h>
  ```
* Ensure log directory exists or is creatable
* Static library (`.a`) by default

---

# 🔮 Future Improvements

* Async logging
* Log rotation
* Shared library (.so)
* Colored console output

---

# 👨‍💻 Author

**Jayasankar JP**

---

# ⭐ Contributing

Pull requests are welcome!

If you encounter any issues, bugs, or have feature requests, please open an issue on GitHub.