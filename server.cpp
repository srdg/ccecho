#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

enum Protocol {
    TCP = 0,
    UDP = 1
};

class Server {

public:
    Server(const std::string& iIp = "127.0.0.1", unsigned int iPort = 7, Protocol iProtocol = Protocol::TCP)
        : _ip(iIp), _port(iPort), _proto(iProtocol), _serverSocket(INVALID_SOCKET), _clientSocket(INVALID_SOCKET) {
        std::cout << "Initializing server on " << _ip << ":" << _port << "..." << std::endl;
    }

    ~Server() {
        Shutdown();
    }

    bool Start() {
        // Create socket based on protocol
        int socketType = (_proto == Protocol::TCP) ? SOCK_STREAM : SOCK_DGRAM;

        _serverSocket = socket(AF_INET, socketType, (_proto == Protocol::TCP) ? IPPROTO_TCP : IPPROTO_UDP);
        if (_serverSocket == INVALID_SOCKET) {
            std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
            return false;
        }

        // Setup address structure
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(_port);

        // Convert IP string to address
        if (inet_pton(AF_INET, _ip.c_str(), &serverAddr.sin_addr) <= 0) {
            std::cerr << "Invalid IP address: " << _ip << std::endl;
            closesocket(_serverSocket);
            return false;
        }

        // Bind socket
        if (bind(_serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
            closesocket(_serverSocket);
            return false;
        }

        // Listen for incoming connections (TCP only)
        if (_proto == Protocol::TCP) {
            if (listen(_serverSocket, SOMAXCONN) == SOCKET_ERROR) {
                std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
                closesocket(_serverSocket);
                return false;
            }

            std::cout << "Server listening on " << _ip << ":" << _port << std::endl;

            // Accept a connection
            sockaddr_in clientAddr;
            int clientAddrLen = sizeof(clientAddr);

            _clientSocket = accept(_serverSocket, (sockaddr*)&clientAddr, &clientAddrLen);
            if (_clientSocket == INVALID_SOCKET) {
                std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
                closesocket(_serverSocket);
                return false;
            }

            // Get client IP address
            char clientIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);

            std::cout << "Connection accepted from " << clientIP << ":" << ntohs(clientAddr.sin_port) << std::endl;
        }

        return true;
    }

    
    void Shutdown() {
        if (_clientSocket != INVALID_SOCKET) {
            closesocket(_clientSocket);
            _clientSocket = INVALID_SOCKET;
        }
        if (_serverSocket != INVALID_SOCKET) {
            closesocket(_serverSocket);
            _serverSocket = INVALID_SOCKET;
        }
    }

private:
    std::string _ip;
    unsigned int _port = 7;
    Protocol _proto;
    SOCKET _serverSocket;
    SOCKET _clientSocket;
};