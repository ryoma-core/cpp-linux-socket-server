#include <fstream>
#include <sstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <sys/statvfs.h>
#include <string>

#define CPU_PATH "/proc/stat"
#define MEM_PATH "/proc/meminfo"

struct CpuTimes {
    long long user=0, nice=0, system=0, idle=0, iowait=0, irq=0, softirq=0, steal=0 , total = 0 , idle_all = 0;
};


class CPU_Resource {
    public:
        CPU_Resource();
        ~CPU_Resource();
        double getCPUUsage();
    private:
        std::ifstream cpuFile;
        CpuTimes cpu_first_Times;
        CpuTimes cpu_second_Times;
        std::string line1 = "";
        std::string line2 = "";
        std::string cpu_label = "";

};

struct MemInfo {
    long long MemTotal=0;
    long long MemAvailable=0;
};

class Memory_Resource {
    public:
        Memory_Resource();
        ~Memory_Resource();
        double getMemoryUsage();
    private:
        std::ifstream memFile;
        MemInfo mem_info;
        std::string line = "";
};

struct DiskInfo {
    long long used=0;
    long long avail=0;
};

class Disk_Resource {
    public:
        Disk_Resource();
        ~Disk_Resource();
        double getDiskUsage();
    private:
        struct statvfs st_root{};
        DiskInfo disk_root_info;
};