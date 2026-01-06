#include "socket.h"

Server_Socket::Server_Socket(uint16_t port)
{
    // Constructor
    server_config.server_port = port;
}
Server_Socket::~Server_Socket()
{
    // Destructor
}

void Server_Socket::start()
{
    // 서버 시작
    isRunning = true;  // 서버 실행 상태 설정
    sockets = socket(AF_INET, SOCK_STREAM, 0); // 소켓 생성
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(server_config.server_port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sockets, (struct sockaddr*)&addr, sizeof(addr)); // 소켓 바인딩
    listen(sockets, SOMAXCONN); // 연결 대기
    while (isRunning)  // accept 루프
    {
        sockaddr_in caddr{}; // 클라이언트 주소 구조체
        socklen_t clen = sizeof(caddr); // 주소 길이

        int clientSocket = accept(sockets, (sockaddr*)&caddr, &clen); // 클라이언트 연결 수락 
        if (clientSocket < 0) { // 수락 실패 시
            if (!isRunning) break;
            continue;
        }

        while (isRunning) // 통신 루프
        {
            char cmd;
            int n = recv(clientSocket, &cmd, 1, 0);
            if (n <= 0) break;
            sendCall(clientSocket, &cmd);
        }
        close(clientSocket);
    }
}

void Server_Socket::stop()
{
    // 프로그램 중지
    isRunning = false;
    shutdown(sockets, SHUT_RDWR); // 소켓 종료
    close(sockets);
}

void Server_Socket::recvCall(int &clientSocket)
{
    // 데이터 수신 처리
    char buffer[1];
    int num = recv(clientSocket, buffer, 1, 0);
    if(num <= 0)
    {
        this->isConnected = false;
        cerr << "[Server_Socket] recv failed or connection closed\n";
        close(clientSocket);
        return;
    }
    this->isConnected = true;
    sendCall(clientSocket,buffer); // 전송 호출
}

void Server_Socket::sendCall(int& clientSocket,char* buffer)
{ 
    // 데이터 전송 처리
    // 사이드 프로젝트 명령어 처리
    switch (*buffer)
    {
        case '0': 
        {
            // 여기에 명령 0에 대한 처리 로직 추가
            // CPU 사용률 전송 
            string temp = to_string(cpu_resource.getCPUUsage());
            temp.push_back('\n');
            send(clientSocket, temp.data(),temp.size(), 0);
            break;
        }
        case '1':
        {
            // 여기에 명령 1에 대한 처리 로직 추가
            // 메모리 사용률 전송
            string temp = to_string(memory_resource.getMemoryUsage());
            temp.push_back('\n');
            send(clientSocket, temp.data() ,temp.size(), 0);
            break;
        }
        case '2':
        {
            // 여기에 명령 2에 대한 처리 로직 추가
            // 디스크 사용률 전송
            string temp = to_string(disk_resource.getDiskUsage());
            temp.push_back('\n');
            send(clientSocket, temp.data() , temp.size(), 0);
            break;
        }
        default:
            cout << "Unknown command" << '\n';
            break;
    }
}
