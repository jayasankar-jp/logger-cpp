cmake_minimum_required(VERSION 3.15)

project(Logger VERSION 1.0 LANGUAGES CXX)

# Use modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------
# Dependencies (cpp-utils)
# -----------------------------
include(FetchContent)
find_package(cpp-utils QUIET)

if(NOT cpp-utils_FOUND)
    message(STATUS "cpp-utils not found locally. Fetching from GitHub repository...")
    FetchContent_Declare(
        cpp-utils
        GIT_REPOSITORY https://github.com/jayasankar-jp/cpp-utils.git
        GIT_TAG        master
    )
    FetchContent_MakeAvailable(cpp-utils)
endif()

# -----------------------------
# Library Target
# -----------------------------
add_library(Logger STATIC
    src/Logger.cpp
    src/LogStream.cpp
    src/FileWriter.cpp
)

# Proper include visibility & dependencies
target_include_directories(Logger PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/includes>
    $<INSTALL_INTERFACE:include>
)

if(TARGET internalLib)
    target_link_libraries(Logger PUBLIC internalLib)
    target_include_directories(Logger PUBLIC $<BUILD_INTERFACE:${cpp-utils_SOURCE_DIR}/includes>)
elseif(TARGET cpp-utils::cpp-utils)
    target_link_libraries(Logger PUBLIC cpp-utils::cpp-utils)
else()
    # Fallback to prefix path includes if installed to custom prefix
    target_include_directories(Logger PUBLIC $<BUILD_INTERFACE:${CMAKE_PREFIX_PATH}/include>)
endif()

# -----------------------------
# Test Executable
# -----------------------------
enable_testing()

add_executable(logger_test
    test/Test.cpp
)

target_link_libraries(logger_test PRIVATE Logger)

set_target_properties(logger_test PROPERTIES
    OUTPUT_NAME "test.exe"
)

add_test(NAME LoggerTest COMMAND logger_test)

add_executable(benchmark
    test/Benchmark.cpp
)
target_link_libraries(benchmark PRIVATE Logger)


# -----------------------------
# Output directories
# -----------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# -----------------------------
# INSTALL RULES 🔥
# -----------------------------

# Install library
install(TARGETS Logger
    EXPORT LoggerTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# Install headers
install(DIRECTORY includes/
    DESTINATION include
)

# Export targets (for find_package)
install(EXPORT LoggerTargets
    FILE LoggerTargets.cmake
    NAMESPACE Logger::
    DESTINATION lib/cmake/Logger
)

# Create Config file
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/LoggerConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/LoggerConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/LoggerConfig.cmake"
    @ONLY
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/LoggerConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/LoggerConfigVersion.cmake"
    DESTINATION lib/cmake/Logger
)