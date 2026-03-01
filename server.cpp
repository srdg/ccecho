#if defined(_WIN32) || defined(_WIN64)
#include <WinSock2.h>
#include <WS2tcpip.h>
#endif
#include <stdio.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>

#pragma comment(lib, "ws2_32.lib")

enum Protocol {
    TCP = 0,
    UDP = 1
};

class Server {

public:
    Server(const std::string& iIp = "127.0.0.1", unsigned int iPort = 7, Protocol iProtocol = Protocol::TCP)
        : _ip(iIp), _port(iPort), _proto(iProtocol), _serverSocket(INVALID_SOCKET), _running(false) {
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

        // Behavior differs for TCP and UDP
        if (_proto == Protocol::TCP) {
            if (listen(_serverSocket, SOMAXCONN) == SOCKET_ERROR) {
                std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
                closesocket(_serverSocket);
                return false;
            }

            std::cout << "Server listening on " << _ip << ":" << _port << " (TCP)" << std::endl;

            _running = true;

            // Accept connections in a loop
            while (_running) {
                sockaddr_in clientAddr;
                int clientAddrLen = sizeof(clientAddr);

                SOCKET clientSocket = accept(_serverSocket, (sockaddr*)&clientAddr, &clientAddrLen);
                if (clientSocket == INVALID_SOCKET) {
                    if (_running) {
                        std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
                    }
                    break;
                }

                // Get client IP address
                char clientIP[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);

                std::cout << "Connection accepted from " << clientIP << ":" << ntohs(clientAddr.sin_port) << std::endl;

                // Create a thread to handle this client
                std::thread clientThread(&Server::HandleClient, this, clientSocket, std::string(clientIP), ntohs(clientAddr.sin_port));
                clientThread.detach();
            }
        } else if (_proto == Protocol::UDP) {
            std::cout << "Server listening on " << _ip << ":" << _port << " (UDP)" << std::endl;
            _running = true;

            const int BUFFER_SIZE = 1024;
            char buffer[BUFFER_SIZE];
            while (_running) {
                sockaddr_in clientAddr;
                int clientAddrLen = sizeof(clientAddr);

                int recvSize = recvfrom(_serverSocket, buffer, BUFFER_SIZE, 0,
                                        (sockaddr*)&clientAddr, &clientAddrLen);
                if (recvSize == SOCKET_ERROR) {
                    if (_running) {
                        std::cerr << "recvfrom failed: " << WSAGetLastError() << std::endl;
                    }
                    break;
                }
                if (recvSize > 0) {
                    // Echo back to sender
                    buffer[recvSize < BUFFER_SIZE ? recvSize : BUFFER_SIZE-1] = '\0';
                    int sent = sendto(_serverSocket, buffer, recvSize, 0,
                                      (sockaddr*)&clientAddr, clientAddrLen);
                    if (sent == SOCKET_ERROR) {
                        std::cerr << "sendto failed: " << WSAGetLastError() << std::endl;
                        break;
                    }
                }
            }
        }

        return true;
    }

    void Stop() {
        _running = false;
        if (_serverSocket != INVALID_SOCKET) {
            closesocket(_serverSocket);
            _serverSocket = INVALID_SOCKET;
        }
    }

    void Shutdown() {
        Stop();
    }

private:
    void HandleClient(SOCKET clientSocket, const std::string& clientIP, unsigned int clientPort) {
        const int BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];

        std::cout << "Handling client " << clientIP << ":" << clientPort << std::endl;

        // Simple echo loop - receive and send back
        while (_running) {
            int recvSize = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
            
            if (recvSize == SOCKET_ERROR) {
                std::cerr << "Recv failed for " << clientIP << ":" << clientPort << std::endl;
                break;
            }

            if (recvSize == 0) {
                std::cout << "Client " << clientIP << ":" << clientPort << " disconnected" << std::endl;
                break;
            }

            buffer[recvSize] = '\0';

            // Echo back to client
            if (send(clientSocket, buffer, recvSize, 0) == SOCKET_ERROR) {
                std::cerr << "Send failed for " << clientIP << ":" << clientPort << std::endl;
                break;
            }
        }

        closesocket(clientSocket);
    }

    std::string _ip;
    unsigned int _port = 7;
    Protocol _proto;
    SOCKET _serverSocket;
    bool _running;
};