#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <thread>
#include "server.cpp"

#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    // Create and start the server on port 7 with TCP
    Server server("127.0.0.1");  // Default port is 7, default protocol is TCP
    
    std::cin.get();

    WSACleanup();
    return 0;
}