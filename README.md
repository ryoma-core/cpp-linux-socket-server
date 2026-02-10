# cpp-linux-socket-server

개인용 C++ Linux TCP 서버 소켓 코드/라이브러리.  
**친절한 설명 없음.** 불편하면 쓰지 말고, 쓸 거면 **코드부터** 봐라.

- 목적: 내가 쓰려고 만든 것
- 상태: 필요한 만큼만 유지보수
- 보증: 없음 (진짜로 없음)

---

## What this is

- Linux 환경에서 돌아가는 **TCP 서버 소켓** 구현
- “재사용 가능한 코드”처럼 보이지만, 기본적으로 **내 프로젝트 부품**

---

## Where to look

- `include/` : 헤더 (외부에 보여줄 의도)
- `src/` : 구현 (진짜 내용)
- `examples/` : 있으면 참고, 없으면 알아서 읽어라

---

## Build (CMake)

```bash
git clone https://github.com/ryoma-core/cpp-linux-socket-server
cd cpp-linux-socket-server

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
