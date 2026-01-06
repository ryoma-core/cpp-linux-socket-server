#include<iostream>
#include "socket.h"

int main()
{
    Server_Socket server(1869); // 포트 8080으로 서버 소켓 생성
    server.start(); // 서버 시작
    return 0;
}