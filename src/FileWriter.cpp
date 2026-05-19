#include "FileWriter.h"
#include <string>
#include <time.h>
#include <iostream>
#include "LoggerUtils.h"
FileWriter::~FileWriter()
{
    mei_isShoutDown.store(true);
    // std::cout << "Distructor Queue count : " << meC_logQueue.getCount() << std::endl;
    if (isActiveFile)
    {
        std::lock_guard<std::mutex> lg(memutexS_mu);

        if (meC_current_file.is_open())
            meC_current_file.close();
        // isActiveFile = false;
    }
}
FileWriter::FileWriter()
{
    meui_buff_len = 0;
    memset(mecs_databuffer, 0, MAX_SIZE_BUFF);
    met_CashInitialTime = time(0);

    // meb_isCashEnable = false;
    mei_bundilSizeKb = 512;
    mei_CashTimeLimitSec = 3;

    // mei_maxFileSizeMB = 50;
    meul_curentFileSize = 0;
    isActiveFile = false;
    mes_filePath = "./LOGS";

    mes_appName = "APP";
    mei_maxFileSizeMB = 50;
    mei_fileGenPeriodMin = 60;
    meb_isCashEnable = true;
    mei_isShoutDown = 0;
}
void FileWriter::mefn_generatefile()
{

    try
    {
        std::string date, Time;
        met_initialTime = time(0);
        met_CashInitialTime = met_initialTime;
        fng_getCurrentTime(date, Time);
        std::string file_name;
        file_name.reserve(60);

        file_name.append(mes_filePath);
        file_name.push_back('/');
        file_name.append(mes_appName);
        file_name.push_back('_');
        file_name.append(date);
        file_name.push_back('_');
        file_name.append(Time);
        file_name.append(".log");
        {
            std::lock_guard<std::mutex> lg(memutexS_mu);
            // std::cout<<"File name : "<<file_name<<std::endl;
            meC_current_file.open(file_name, std::ios::out | std::ios::binary);
        }
        isActiveFile = true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void FileWriter::mcfn_writer()
{

    while (true)
    {

        // std::cout<<"Queue count "<< meC_logQueue.getCount()<<std::endl;
        bool isTimeOut = false;
        std::string msg_data;
        size_t mesg_len;
        int resQue;

        std::pair<bool, std::string> cl_pair;
        // {
        //     std::unique_lock<std::mutex> ul(me_QueueMutex);
        //     mec_Queue_cv.wait_for(ul, std::chrono::seconds(2), [this]
        //                           { return meC_logQueue.getCount() > 0 || mei_isShoutDown; });
        // }

        time_t tL_currentTime = time(0);

        if (mei_isShoutDown && meC_logQueue.getCount() == 0)
        {
            std::cout << "Distructor Queue Count is zero " << std::endl;
            if (meb_isCashEnable)
            {
                if (meui_buff_len > 0)
                {
                    if (!isActiveFile)
                    {
                        mefn_generatefile();
                    }
                    if (meC_current_file.is_open())
                    {
                        meC_current_file.write(mecs_databuffer, meui_buff_len);
                        // meC_current_file << mecs_databuffer;

                        // meC_current_file.write(mecs_databuffer, size);
                    }
                }
            }
            break;
        }

        resQue = meC_logQueue.getElement(cl_pair, 2000);
        // std::cout << "After waiting" << std::endl;
        if (tL_currentTime - met_CashInitialTime >= mei_CashTimeLimitSec)
        {
            // std::cout << "Cash time : " << mei_CashTimeLimitSec << std::endl;
            isTimeOut = true;
            met_CashInitialTime = tL_currentTime;
        }
        if (resQue)
        {
            msg_data = cl_pair.second;
            mesg_len = msg_data.length();
            if (cl_pair.first && !meb_isCashEnable)
            {
                std::cout << msg_data;
            }
            if (meb_isCashEnable)
            {
                memcpy(mecs_databuffer + meui_buff_len, msg_data.c_str(), mesg_len);
                meui_buff_len += mesg_len;
                mecs_databuffer[meui_buff_len] = '\0';
            }
        }
        // std::cout << "timeout happen" << std::endl;

        if (meb_isCashEnable)
        {
            if (meui_buff_len >= mei_bundilSizeKb * 1024)
            {
                if (cl_pair.first)
                    std::cout << mecs_databuffer;
                if (!isActiveFile)
                {
                    mefn_generatefile();
                }
                if (meC_current_file.is_open())
                {
                    if (meul_curentFileSize + meui_buff_len >= mei_maxFileSizeMB * 1024 * 1024)
                    {
                        {
                            std::lock_guard<std::mutex> lg(memutexS_mu);
                            meC_current_file.close();
                        }
                        meul_curentFileSize = 0;
                        mefn_generatefile();
                        if (meC_current_file.is_open())
                        {
                            // meC_current_file << mecs_databuffer;
                            meC_current_file.write(mecs_databuffer, meui_buff_len);
                            meul_curentFileSize += meui_buff_len;
                            meui_buff_len = 0;
                        }
                    }
                    else
                    {
                        // meC_current_file << mecs_databuffer;

                        meC_current_file.write(mecs_databuffer, meui_buff_len);
                        meul_curentFileSize += meui_buff_len;
                        meui_buff_len = 0;

                        meC_current_file.flush();
                        met_CashInitialTime = tL_currentTime;
                        met_initialTime = tL_currentTime;
                    }
                    if (isTimeOut)
                    {

                        meC_current_file.flush();
                        // met_initialTime = tL_currentTime;
                        // std::cout << "timeout flush 1" << std::endl;
                    }
                }
            }
            if (isTimeOut)
            {
                if (cl_pair.first)
                    std::cout << mecs_databuffer;
                if (!isActiveFile)
                {
                    if (meui_buff_len > 0)
                        mefn_generatefile();
                    isActiveFile = 1;
                }
                if (meC_current_file.is_open())
                {
                    meC_current_file.write(mecs_databuffer, meui_buff_len);
                    meul_curentFileSize += meui_buff_len;
                    meui_buff_len = 0;
                    meC_current_file.flush();
                }
                // met_CashInitialTime = tL_currentTime;
                // met_initialTime = tL_currentTime;
                // std::cout << "timeout flush 2" << std::endl;
            }
            // if(meul_curentFileSize+)
            // if(meul_curentFileSize+mei_bundilSizeKb)
            // if (meui_buff_len + mei_bundilSizeKb)
        }
        else
        {
            // std::cout << "NO cash en" << std::endl;
            if (!isActiveFile)
            {
                mefn_generatefile();
            }
            if (meC_current_file.is_open())
            {
                if (meul_curentFileSize + mesg_len <= mei_maxFileSizeMB * 1024 * 1024)
                {
                    // meC_current_file << msg_data;
                    meC_current_file.write(msg_data.c_str(), mesg_len);
                    meul_curentFileSize += mesg_len;
                }
                else
                {
                    {
                        std::lock_guard<std::mutex> lg(memutexS_mu);
                        meC_current_file.close();
                    }
                    meul_curentFileSize = 0;
                    mefn_generatefile();
                    if (meC_current_file.is_open())
                    {
                        // meC_current_file << msg_data;
                        meC_current_file.write(msg_data.c_str(), mesg_len);
                        meul_curentFileSize += mesg_len;
                    }
                }
                if (isTimeOut)
                {
                    meC_current_file.flush();
                    // met_initialTime = tL_currentTime;
                    // std::cout << "timeout flush 3";
                    mei_CashTimeLimitSec = tL_currentTime;
                }
            }
            else
            {
                std::cerr << "Faild to open file" << std::endl;
            }
        }
        if (isActiveFile && meC_current_file.is_open() && (tL_currentTime - met_initialTime > mei_fileGenPeriodMin * 60))
        {
            {
                std::lock_guard<std::mutex> lg(memutexS_mu);
                meC_current_file.close();
            }

            meul_curentFileSize = 0;
            isActiveFile = 0;
        }
    }
}
void FileWriter::mcfn_insert(bool &console, std::string &clData)
{
    meC_logQueue.insert({console, std::move(clData)});
}
