cmake_minimum_required(VERSION 3.11) # 최소 CMake 버전 설정
project(ServerSocketProject LANGUAGES CXX) # 프로젝트 이름 및 언어 설정

set(CMAKE_CXX_STANDARD 20) # C++ 버전 설정
set(CMAKE_CXX_STANDARD_REQUIRED ON) # C++ 버전 으로 빌드 실패시 오류 발생

add_library(serversocketlib STATIC
    src/socket.cpp
    src/system.cpp
)   # 라이브러리 CPP 파일 추가

target_include_directories(serversocketlib PUBLIC 
${CMAKE_CURRENT_SOURCE_DIR}/include
${CMAKE_CURRENT_SOURCE_DIR}/src
) # 헤더 파일 경로 설정

add_executable(example example/main.cpp) # 실행 파일 추가 예제 CPP 파일 추가
target_link_libraries(example PRIVATE serversocketlib) # 라이브러리 링크 설정 

find_package(Threads REQUIRED) # 스레드 패키지 찾기
target_link_libraries(serversocketlib PUBLIC Threads::Threads) # 스레드 라이브러리 링크 설정