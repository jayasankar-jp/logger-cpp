#include "FileWriter.h"
#include <string>
#include <time.h>
#include <iostream>
#include "LoggerUtils.h"

FileWriter::~FileWriter()
{
    mei_isShoutDown.store(true);
    if (isActiveFile)
    {
        std::lock_guard<std::mutex> lg(memutexS_mu);
        if (meC_current_file.is_open())
            meC_current_file.close();
    }
}

FileWriter::FileWriter()
{
    meui_buff_len = 0;
    memset(mecs_databuffer, 0, MAX_SIZE_BUFF);
    met_CashInitialTime = time(0);

    mei_bundilSizeKb = 512;
    mei_CashTimeLimitSec = 3;

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
            if (meC_current_file.is_open())
            {
                meC_current_file.close();
            }
            meC_current_file.open(file_name, std::ios::out | std::ios::binary);
        }
        if (meC_current_file.is_open())
        {
            isActiveFile = true;
        }
        else
        {
            isActiveFile = false;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        isActiveFile = false;
    }
}

void FileWriter::mcfn_writer()
{
    while (true)
    {
        std::pair<bool, std::string> cl_pair;
        int resQue = meC_logQueue.getElement(cl_pair, 1000);
        time_t tL_currentTime = time(0);

        bool isShutdownRequested = mei_isShoutDown.load();

        if (isShutdownRequested && meC_logQueue.getCount() == 0)
        {
            std::cout << "Distructor Queue Count is zero " << std::endl;
            if (meb_isCashEnable && meui_buff_len > 0)
            {
                if (!isActiveFile)
                {
                    mefn_generatefile();
                }
                if (meC_current_file.is_open())
                {
                    meC_current_file.write(mecs_databuffer, meui_buff_len);
                    meui_buff_len = 0;
                    meC_current_file.flush();
                }
            }
            break;
        }

        bool isTimeOut = (tL_currentTime - met_CashInitialTime >= mei_CashTimeLimitSec);

        if (resQue)
        {
            const std::string &msg_data = cl_pair.second;
            size_t mesg_len = msg_data.length();

            if (cl_pair.first && !meb_isCashEnable)
            {
                std::cout << msg_data;
            }

            if (meb_isCashEnable)
            {
                // Buffer Overflow Protection
                if (meui_buff_len + mesg_len >= MAX_SIZE_BUFF)
                {
                    if (cl_pair.first && meui_buff_len > 0)
                        std::cout.write(mecs_databuffer, meui_buff_len);

                    if (!isActiveFile)
                    {
                        mefn_generatefile();
                    }

                    if (meC_current_file.is_open())
                    {
                        meC_current_file.write(mecs_databuffer, meui_buff_len);
                        meul_curentFileSize += meui_buff_len;
                        meui_buff_len = 0;
                    }
                    else
                    {
                        meui_buff_len = 0;
                    }

                    if (mesg_len >= MAX_SIZE_BUFF)
                    {
                        mesg_len = MAX_SIZE_BUFF - 1;
                    }
                }

                memcpy(mecs_databuffer + meui_buff_len, msg_data.data(), mesg_len);
                meui_buff_len += mesg_len;
            }
        }

        if (meb_isCashEnable)
        {
            if (meui_buff_len >= (size_t)mei_bundilSizeKb * 1024)
            {
                if (cl_pair.first && meui_buff_len > 0)
                    std::cout.write(mecs_databuffer, meui_buff_len);

                if (!isActiveFile)
                {
                    mefn_generatefile();
                }

                if (meC_current_file.is_open())
                {
                    if (meul_curentFileSize + meui_buff_len >= (unsigned long long)mei_maxFileSizeMB * 1024 * 1024)
                    {
                        meC_current_file.write(mecs_databuffer, meui_buff_len);
                        meui_buff_len = 0;
                        {
                            std::lock_guard<std::mutex> lg(memutexS_mu);
                            meC_current_file.close();
                        }
                        meul_curentFileSize = 0;
                        mefn_generatefile();
                    }
                    else
                    {
                        meC_current_file.write(mecs_databuffer, meui_buff_len);
                        meul_curentFileSize += meui_buff_len;
                        meui_buff_len = 0;
                        met_CashInitialTime = tL_currentTime;
                        met_initialTime = tL_currentTime;
                    }
                }
            }

            if (isTimeOut && meui_buff_len > 0)
            {
                if (cl_pair.first)
                    std::cout.write(mecs_databuffer, meui_buff_len);

                if (!isActiveFile)
                {
                    mefn_generatefile();
                }

                if (meC_current_file.is_open())
                {
                    meC_current_file.write(mecs_databuffer, meui_buff_len);
                    meul_curentFileSize += meui_buff_len;
                    meui_buff_len = 0;
                    meC_current_file.flush();
                }
                met_CashInitialTime = tL_currentTime;
            }
        }
        else if (resQue)
        {
            const std::string &msg_data = cl_pair.second;
            size_t mesg_len = msg_data.length();

            if (!isActiveFile)
            {
                mefn_generatefile();
            }

            if (meC_current_file.is_open())
            {
                if (meul_curentFileSize + mesg_len <= (unsigned long long)mei_maxFileSizeMB * 1024 * 1024)
                {
                    meC_current_file.write(msg_data.data(), mesg_len);
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
                        meC_current_file.write(msg_data.data(), mesg_len);
                        meul_curentFileSize += mesg_len;
                    }
                }

                if (isTimeOut)
                {
                    meC_current_file.flush();
                    met_CashInitialTime = tL_currentTime;
                }
            }
            else
            {
                std::cerr << "Failed to open file" << std::endl;
            }
        }

        if (isActiveFile && meC_current_file.is_open() && (tL_currentTime - met_initialTime >= mei_fileGenPeriodMin * 60))
        {
            {
                std::lock_guard<std::mutex> lg(memutexS_mu);
                if (meui_buff_len > 0 && meb_isCashEnable)
                {
                    meC_current_file.write(mecs_databuffer, meui_buff_len);
                    meul_curentFileSize += meui_buff_len;
                    meui_buff_len = 0;
                }
                meC_current_file.close();
            }

            meul_curentFileSize = 0;
            isActiveFile = false;
        }
    }
}

void FileWriter::mcfn_insert(bool &console, std::string &clData)
{
    meC_logQueue.insert({console, std::move(clData)});
}
