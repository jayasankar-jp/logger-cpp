cmake_minimum_required(VERSION 3.15)

project(Logger VERSION 1.0 LANGUAGES CXX)

# Use modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------
# Library Target
# -----------------------------
add_library(Logger STATIC
    src/Logger.cpp
    src/LogStream.cpp
)

# Proper include visibility
target_include_directories(Logger PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/includes>
    $<INSTALL_INTERFACE:include>
)

# -----------------------------
# Test Executable
# -----------------------------
add_executable(test
    test/Test.cpp
)

target_link_libraries(test PRIVATE Logger)

set_target_properties(test PROPERTIES
    OUTPUT_NAME "test.exe"
)

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