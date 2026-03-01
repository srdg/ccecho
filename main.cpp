#if defined(_WIN32) || defined(_WIN64)
#include <WinSock2.h>
#include <WS2tcpip.h>
#endif
#include <stdio.h>
#include <iostream>
#include <thread>
#include "server.cpp"

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    int iResult;

    // Parse command-line arguments for UDP flag
    bool useUdp = false;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-u") == 0 || strcmp(argv[i], "--udp") == 0) {
            useUdp = true;
        }
    }

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    Protocol proto = useUdp ? Protocol::UDP : Protocol::TCP;
    std::cout << "Starting server with protocol: " << (useUdp ? "UDP" : "TCP") << std::endl;

    // Create and start the server on port 7
    Server server("127.0.0.1", 7, proto);
    
    // Run server in a separate thread
    std::thread serverThread(&Server::Start, &server);

    std::cin.get();

    server.Stop();
    serverThread.join();

    WSACleanup();
    return 0;
}