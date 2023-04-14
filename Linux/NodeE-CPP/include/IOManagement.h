#pragma once

#if _TRACK_STATS
#include "iostream"
#include "fstream"
#include "iomanip"
#include "Timing.h"
#include "mutex"


extern std::chrono::time_point<std::chrono::system_clock> begin_stamp;
namespace ProdCon {
    class IOManagement {
    public:

        explicit IOManagement(std::string &file_name) {
            reset_point = std::cout.rdbuf();
            std::cout.rdbuf(target_file.rdbuf());
            target_file = std::ofstream(file_name);
        }


        void bind() {
            reset_point = std::cout.rdbuf();
            std::cout.rdbuf(target_file.rdbuf());
        }


        void release() {
            std::cout.rdbuf(reset_point);
        }


        static void write_list(double time_stamp, int ID, int Q_number, std::string status, int N_number) {
            std::cout << std::left << std::setw(6) << std::setfill(' ') << time_stamp;
            std::cout << "ID= ";
            std::cout << std::right << std::setw(2) << std::setfill(' ') << ID;
            if (Q_number != -1) {
                std::cout << " Q=";
                std::cout << std::right << std::setw(2) << std::setfill(' ') << Q_number;
            }
            else {
                std::cout << "  ";
                std::cout << std::right << std::setw(3) << std::setfill(' ') << ' ';
            }
            std::cout << " ";
            std::cout << std::left << std::setw(11) << std::setfill(' ') << status;
            if (N_number != -1) {
                std::cout << std::right << std::setw(3) << std::setfill(' ') << N_number;
            }
            else {
                std::cout << std::right << std::setw(3) << std::setfill(' ') << ' ';
            }
            std::cout << std::endl;
        }


        // void record(int tID, int Q_number, std::string status, int N_number) {
        //     std::unique_lock<std::mutex> lk{t};
        //     double stamp = Shell379::Utilities::totalTiming::stamp(begin_stamp) / 1000000;
        //     this->bind();
        //     ProdCon::IOManagement::write_list(stamp, tID, Q_number, status, N_number);
        //     this->release();
        // }

        void printSummary(std::vector<double> &summary) {
            std::unique_lock<std::mutex> lk{t};
            this->bind();
            std::cout << "Summary:" << std::endl;
            std::cout << "    Encrypt time     ";
            std::cout << std::right << std::setw(6) << std::setfill(' ') << summary.at(0) << std::endl;
            std::cout << "    Decrypt time      ";
            std::cout << std::right << std::setw(6) << std::setfill(' ') << summary.at(1) << std::endl;
            std::cout << "    Setup time        ";
            std::cout << std::right << std::setw(6) << std::setfill(' ') << summary.at(2) << std::endl;


            // std::cout << "Transactions per second: " << summary.at(0)/(end_stamp/1000000) << std::endl;
            this->release();
        }

    private:
        std::ofstream target_file;
        std::streambuf *reset_point;
        std::mutex t;
    };
}
#endif