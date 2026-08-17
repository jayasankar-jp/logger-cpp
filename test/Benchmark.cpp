#include "Logger.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

void benchmark(int num_threads, int logs_per_thread)
{
    Logger::getInstance();
    Logger::setLogLevel(63); // 63 = All log levels except Console
    Logger::setAppName("BENCHMARK");
    Logger::setMaxFileSizeMB(500);

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    for (int t = 0; t < num_threads; ++t)
    {
        threads.emplace_back([t, logs_per_thread]() {
            for (int i = 0; i < logs_per_thread; ++i)
            {
                log_info << "Thread " << t << " iteration " << i << " test message for logger performance benchmarking.";
            }
        });
    }

    for (auto &th : threads)
    {
        th.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    uint64_t total_logs = (uint64_t)num_threads * logs_per_thread;
    std::cout << "Threads: " << num_threads 
              << " | Total Logs: " << total_logs 
              << " | Time: " << elapsed.count() << " s"
              << " | Throughput: " << (total_logs / elapsed.count()) << " logs/sec"
              << " | Avg Latency: " << (elapsed.count() * 1e9 / total_logs) << " ns/log"
              << std::endl;
}

int main()
{
    std::cout << "--- Starting Performance Benchmark ---" << std::endl;
    benchmark(4, 50000); // 200,000 total logs
    return 0;
}
