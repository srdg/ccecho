# Echo Server

A multi-threaded TCP echo server built with C++ and Windows Sockets (Winsock2).

## Overview

This project implements a concurrent echo server that listens on port 7 (configurable) and accepts multiple client connections simultaneously. Each client connection is handled in a separate thread, allowing the server to echo data from multiple clients independently.

## Features

- **TCP Protocol**: Uses TCP/IP for reliable, connection-oriented communication
- **Multi-threaded**: Handles multiple concurrent client connections
- **Port 7 Default**: Listens on port 7 by default (configurable)
- **Echo Functionality**: Echoes back any data received from clients
- **Graceful Shutdown**: Cleanly stops the server on user command
- **Error Handling**: Comprehensive error reporting for socket operations

## Requirements

- **Windows OS** (uses Winsock2)
- **C++11 or later** (for threading support)
- **CMake 3.10+** (for building)
- **Visual Studio Build Tools** or similar C++ compiler

## Building

### Using CMake:

```bash
cd EchoServer
cmake -B build
cmake --build build
```

### Or using build.bat:

```bash
build.bat
```

