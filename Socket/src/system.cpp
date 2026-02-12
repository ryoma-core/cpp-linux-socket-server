#include "system.h"

CPU_Resource::CPU_Resource()
{
    // Constructor
    cpuFile.open(CPU_PATH);
    if(!cpuFile.is_open())
    {
        std::cerr << "[CPU_Resource] Failed to open " << CPU_PATH << '\n';
    }
}
CPU_Resource::~CPU_Resource()
{
    // Destructor
}
double CPU_Resource::getCPUUsage()
{
    cpuFile.close();
    cpuFile.open(CPU_PATH);
    {
        std::getline(cpuFile, line1);
        std::istringstream ss1(line1);
        ss1 >> cpu_label >> cpu_first_Times.user >> cpu_first_Times.nice >> cpu_first_Times.system >> cpu_first_Times.idle
            >> cpu_first_Times.iowait >> cpu_first_Times.irq >> cpu_first_Times.softirq >> cpu_first_Times.steal;
    }

    // 잠시 대기
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cpuFile.close();
    cpuFile.open(CPU_PATH); // 파일 다시 열기
    {
        std::getline(cpuFile, line2);
        std::istringstream ss2(line2);
        ss2 >> cpu_label >> cpu_second_Times.user >> cpu_second_Times.nice >> cpu_second_Times.system >> cpu_second_Times.idle
            >> cpu_second_Times.iowait >> cpu_second_Times.irq >> cpu_second_Times.softirq >> cpu_second_Times.steal;
    }

    if(cpu_first_Times.user ==0 && cpu_first_Times.nice ==0 && cpu_first_Times.system ==0 && cpu_first_Times.idle ==0)
    {
        std::cerr << "[CPU_Resource] Failed to read CPU info from " << CPU_PATH << '\n';
        return 0.0;
    }

    cpu_first_Times.total = cpu_first_Times.user + cpu_first_Times.nice + cpu_first_Times.system + cpu_first_Times.idle +
                            cpu_first_Times.iowait + cpu_first_Times.irq + cpu_first_Times.softirq + cpu_first_Times.steal;
    
    cpu_second_Times.total = cpu_second_Times.user + cpu_second_Times.nice + cpu_second_Times.system + cpu_second_Times.idle +
                             cpu_second_Times.iowait + cpu_second_Times.irq + cpu_second_Times.softirq + cpu_second_Times.steal;

    cpu_first_Times.idle_all = cpu_first_Times.idle + cpu_first_Times.iowait;
    cpu_second_Times.idle_all = cpu_second_Times.idle + cpu_second_Times.iowait;

    double totald = static_cast<double>(cpu_second_Times.total - cpu_first_Times.total);
    double idled = static_cast<double>(cpu_second_Times.idle_all - cpu_first_Times.idle_all);

    cpuFile.close();
    return (totald - idled) / totald * 100.0;
}

Memory_Resource::Memory_Resource()
{
    // Constructor
    memFile.open(MEM_PATH);
    if(!memFile.is_open())
    {
        std::cerr << "[Memory_Resource] Failed to open " << MEM_PATH << '\n';
    }
}
Memory_Resource::~Memory_Resource()
{
    // Destructor
}

double Memory_Resource::getMemoryUsage()
{
    memFile.close();
    memFile.open(MEM_PATH);
    while(memFile.eof() == false)
    {
        std::getline(memFile, line);
        std::istringstream ss(line);
        std::string key;
        ss >> key;
        if(key == "MemTotal:")
        {
            ss >> mem_info.MemTotal;
        }
        if(key == "MemAvailable:")
        {
            ss >> mem_info.MemAvailable;
        }
    }
    if(mem_info.MemTotal ==0 || mem_info.MemAvailable ==0)
    {
        std::cerr << "[Memory_Resource] Failed to read memory info from " << MEM_PATH << '\n';
        return 0.0;
    }

    memFile.close();
    return (static_cast<double>(mem_info.MemTotal - mem_info.MemAvailable) / static_cast<double>(mem_info.MemTotal)) * 100.0;
}

Disk_Resource::Disk_Resource()
{
       // Constructor
    if (statvfs("/", &st_root) != 0) {
        std::cerr << "[Disk_Resource] Failed to get / disk info using statvfs\n";
    }
}

Disk_Resource::~Disk_Resource()
{
    // Destructor
}

double Disk_Resource::getDiskUsage()
{
    if(st_root.f_frsize == 0) {
        std::cerr << "[Disk_Resource] Filesystem block size is zero, cannot calculate usage.\n";
        return 0.0;
    }
    
    // Root 디스크 정보 계산
    disk_root_info.used = (st_root.f_blocks - st_root.f_bfree) * st_root.f_frsize;
    disk_root_info.avail = st_root.f_bavail * st_root.f_frsize;

    long long total = disk_root_info.used + disk_root_info.avail;
    if (total == 0) {
        std::cerr << "[Disk_Resource] Total disk size is zero, cannot calculate usage.\n";
        return 0.0;
    }

    return (static_cast<double>(disk_root_info.used) / static_cast<double>(total)) * 100.0;
}