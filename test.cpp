#include "libconntor.h"
#include <iostream>
#include <windows.h>

int main() {
    libconntor tor(MAKEWORD(2,2), true);
    if(tor.torconnect("127.0.0.1", 9050) != 0) {
        ExitProcess(EXIT_FAILURE);
    }
    SOCKET sock = tor.hostconnect("google.com", 80);
    const char getreq[] = "HEAD / HTTP/1.0\r\n\r\n";
    send(sock, getreq, strlen(getreq), 0);
    char buf[4096];
    while(recv(sock, buf, strlen(buf), 0)) {
        std::cout << buf;
    }
}