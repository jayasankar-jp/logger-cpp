cmake_minimum_required(VERSION 3.15)

project(Logger VERSION 1.0 LANGUAGES CXX)

# Use modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directory
include_directories(${PROJECT_SOURCE_DIR}/includes)

# -----------------------------
# Library Target
# -----------------------------
add_library(Logger STATIC
    src/Logger.cpp
    src/LogStream.cpp
)

# Optional: give proper include visibility
target_include_directories(Logger PUBLIC
    ${PROJECT_SOURCE_DIR}/includes
)

# -----------------------------
# Test Executable
# -----------------------------
add_executable(test
    test/Test.cpp
)

# Link library to test
target_link_libraries(test PRIVATE Logger)

# -----------------------------
# Output naming (test.exe)
# -----------------------------
set_target_properties(test PROPERTIES
    OUTPUT_NAME "test.exe"
)

# Optional: organize output dirs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)