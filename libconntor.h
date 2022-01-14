/* 

    LibConnTor, a C++ library to easily connect to a Tor SOCKS5 proxy server
    Created by glblduh

*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

extern "C" {
    class __declspec(dllexport) libconntor{
        public:
            ~libconntor();
            libconntor(WORD wsdllver, bool output);
            bool torconnect(std::string ip, int port);
            SOCKET hostconnect(std::string hostname, int port);
        private:
            WORD dllver;
            bool haveout = false;
            bool initialized = false;
            SOCKET torsock;
            void printstat(bool iserror, std::string message);
    };
}