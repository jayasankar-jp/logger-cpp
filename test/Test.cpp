#include "Logger.h"
#include <thread>
#include <chrono>
int main()
{
    std::shared_ptr<Logger> log = Logger::getInstance();
    log->setLogLevel(15); // 31
    log->setAppName("MY_TEST_APP");
    while (true)
    {
        log_error << "Execption";
        log_info << "INFO TEST";
        log_debug << "MY DEBUG MESSGAE";
        // std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    // log.write(DEBUG_LEVEL, "Helo");
    // log.write(INFO_LEVEL, "Helo INFO");
    // log.write(ERROR_LEVEL, "Helo ERROR");
    return 0;
}