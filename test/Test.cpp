#include "Logger.h"
int main()
{
    std::shared_ptr<Logger> log = Logger::getInstance();
    log->setLogLevel(31);
    log->setAppName("MY_TEST_APP");
    log_error << "Execption";
    log_info << "INFO TEST";
    log_debug << "MY DEBUG MESSGAE";
    // log.write(DEBUG_LEVEL, "Helo");
    // log.write(INFO_LEVEL, "Helo INFO");
    // log.write(ERROR_LEVEL, "Helo ERROR");
    return 0;
}