#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <cstdint>
#include "system.h"
using namespace std;

struct Config
{
    uint16_t server_port;
};

class Server_Socket
{
    public:
        Server_Socket(uint16_t port);
        ~Server_Socket();

        void start();
        void stop();
    private:
    // members ========================================
    bool isRunning = true;
    bool isConnected = false;

    int sockets = 0;
    sockaddr_in addr{};
    Config server_config;

    CPU_Resource cpu_resource;
    Memory_Resource memory_resource;
    Disk_Resource disk_resource;
    // functions =======================================
    void recvCall(int &clientSocket);
    void sendCall(int &clientSocket,char* buffer);

};