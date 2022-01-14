/* 

    LibConnTor, a C++ library to easily connect to a Tor SOCKS5 proxy server
    Created by glblduh

*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "libconntor.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

void libconntor::printstat(bool iserror, std::string message) {
    if (libconntor::haveout) {
        switch (iserror) {
            case true:
                std::cerr << "[ERROR] " << message << std::endl;
                break;
            case false:
                std::cout << "[INFO] " << message << std::endl;
                break;
        }
    }
}

libconntor::libconntor(WORD wsdllver, bool haveoutput) {
    libconntor::dllver = wsdllver;
    libconntor::haveout = haveoutput;
}

bool libconntor::torconnect(std::string ip, int port) {
    WSADATA wsad;
    libconntor::printstat(false, "Initializing WinSock");
    // WSAStartup to inialize WinSock
    if (WSAStartup(libconntor::dllver, &wsad) != 0) {
        libconntor::printstat(true, "WSAStartup failed");
        return 1;
    }
    libconntor::torsock = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    addr.sin_port = htons(port);
    libconntor::printstat(false, "Connecting to Tor's SOCKS5 proxy");
    // Attempt to connect to Tor's SOCKS5 proxy
    if (connect(libconntor::torsock, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
        libconntor::printstat(true, "Connection to Tor's SOCKS5 proxy failed. Closing socket.");
        closesocket(libconntor::torsock);
        return 1;
    }
    // Send first packet to SOCKS5
    char* firstpacket = new char[3];
    char recvbuf[2];
    firstpacket[0] = 5; // SOCKS version, which is 5
    firstpacket[1] = 1; // The NAUTH which is 1
    firstpacket[2] = 0; // Type of authentication, which is none so 0
    if (send(libconntor::torsock, firstpacket, 3, 0) == SOCKET_ERROR) {
        libconntor::printstat(true, "Failed to send first packet. Closing socket.");
        closesocket(libconntor::torsock);
        return 1;
    }
    if (recv(libconntor::torsock, recvbuf, 2, 0) <= 0) {
        libconntor::printstat(true, "SOCKS5 server response error. Closing socket.");
        closesocket(libconntor::torsock);
        return 1;
    }
    libconntor::printstat(false, "Connection to Tor's SOCKS5 proxy successful");
    libconntor::initialized = true;
    return 0;
}

SOCKET libconntor::hostconnect(std::string hostname, int port) {
    if (!libconntor::initialized) {
        libconntor::printstat(true, "Not connected, connect first");
        return NULL;
    }
    // Convert to suitable data types
    short hport = htons(port);
    const char* hname = hostname.c_str();
    int hnamelen = strlen(hname);
    char secondpacket[256];
    secondpacket[0] = 0x05; // SOCKS version
    secondpacket[1] = 0x01; // CMD, 1 which is connect
    secondpacket[2] = 0x00; // Reserved
    secondpacket[3] = 0x03; // ATYP, 3 which is domain name
    secondpacket[4] = hnamelen; // Domain name string length
    memcpy(secondpacket + 5, hname, hnamelen); // Copies the domain name to buffer
    memcpy(secondpacket + 5 + hnamelen, &hport, 2); // Copies the host port to buffer
    char resp[256];
    libconntor::printstat(false, "Attempting to send second packet");
    if (send(libconntor::torsock, secondpacket, 256, 0) == SOCKET_ERROR) {
        libconntor::printstat(true, "Sending packet failed. Closing socket");
        closesocket(libconntor::torsock);
        return NULL;
    }
    if (recv(libconntor::torsock, resp, 256, 0) <= 0) {
        libconntor::printstat(true, "Server response error. Closing socket.");
        closesocket(libconntor::torsock);
        return NULL;
    }
    libconntor::printstat(false, "Second packet sent. Releasing socket for client");
    return libconntor::torsock;
}

libconntor::~libconntor() {
    closesocket(libconntor::torsock);
}