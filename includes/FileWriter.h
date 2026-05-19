#ifndef __FILE_WRITER_H__
#define __FILE_WRITER_H__
#include <atomic>
#include <fstream>
#include <cstring>
#include <Queue.h>
// #ifdef _WIN32
// #include <cstdlib>

// #else
// #include <unistd.h>
// #endif
#define MAX_SIZE_BUFF 1025 * 512
class Logger;
class FileWriter
{
private:
    friend Logger;
    std::atomic<int> mei_isShoutDown;
    bool meb_isCashEnable;
    unsigned int meui_buff_len;
    bool isActiveFile;
    int mei_maxFileSizeMB;
    int mei_fileGenPeriodMin;
    time_t met_initialTime, met_CashInitialTime;
    std::string mes_filePath, mes_appName;
    std::ofstream meC_current_file;
    char mecs_databuffer[MAX_SIZE_BUFF];
    int mei_CashTimeLimitSec;
    int mei_bundilSizeKb;
    unsigned long long meul_curentFileSize;
    Queue<std::pair<bool, std::string>> meC_logQueue;
    std::mutex memutexS_mu;
    /* data */
    void mefn_generatefile();

public:
    void mcfn_writer();
    void mcfn_insert(bool & console,std::string & clData);

    FileWriter(/* args */);
    ~FileWriter();
    
};


#endif
