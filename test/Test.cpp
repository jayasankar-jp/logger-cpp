#include "Logger.h"
#include <thread>
#include <chrono>
int main()
{
    std::shared_ptr<Logger> log = Logger::getInstance();
    log->setLogLevel(31); // 31
    log->setAppName("MY_TEST_APP");
    log->setMaxFileSizeMB(10);
    log->setMaxFileGenPeriodMin(1);

    int data = 0;
    std::thread([]
                {
        while (true)
        {
            int thread_id = 1;
            log_error << thread_id << " Execption : ";
            log_info << thread_id << " INFO a";
            log_debug << thread_id << " MY DEBUG MESSGAE";
            // std::this_thread::sleep_for(std::chrono::seconds(1));
        } })
        .detach();
    std::thread([]
                {
        while (true)
        {
            int thread_id = 2;
            log_error << thread_id << " MY test : ";
            log_info << thread_id << " INFO test 2";
            log_debug << thread_id << " MY DEBUG test test test test";
            // std::this_thread::sleep_for(std::chrono::seconds(2));
        } })
        .detach();
    log_info << " Login Thread : ";
    while (true)
        ;
    return 0;
}