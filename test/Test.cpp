#include "Logger.h"
#include <thread>
#include <chrono>

int main()
{

    Logger::getInstance();
    Logger::setLogLevel(127); // 127-64 =63
    Logger::setAppName("MY_TEST_APP");
    Logger::setMaxFileSizeMB(50);
    Logger::setMaxFileGenPeriodMin(1);
    // Logger::desableCash();

    int data = 0;
    // std::thread([]
    //             {
    //     while (true)
    //     {
    //         int thread_id = 1;
    //         log_error << thread_id << " Execption : ";
    //         log_info << thread_id << " INFO a";
    //         log_debug << thread_id << " MY DEBUG MESSGAE";
    //         // std::this_thread::sleep_for(std::chrono::nanoseconds(1));
    //     } })
    //     .detach();
    // std::thread([]
    //             {
    //     while (true)
    //     {
    //         int thread_id = 2;
    //         log_error << thread_id << " MY test : ";
    //         log_info << thread_id << " INFO test 2";
    //         log_debug << thread_id << " MY DEBUG test test test test";
    //         // std::this_thread::sleep_for(std::chrono::nanoseconds(2));
    //     } })
    //     .detach();
    log_info << " Login Thread : ";
    // while (true)
    // {
    // };
    // {
    //     std::this_thread::sleep_for(std::chrono::seconds(10));
    //     exit(0);
    // };
    return 0;
}