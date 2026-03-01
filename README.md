# Coding Challenges - Echo Server (ccecho)

A multi-threaded echo server built with C++ and Windows Sockets (Winsock2), supporting both TCP and UDP, built as part of [Coding Challenges](https://codingchallenges.fyi/challenges/challenge-echo) series.

## Overview

This project implements a concurrent echo server that listens on port 7 and accepts multiple client connections simultaneously. Each TCP client connection is handled in a separate thread, allowing the server to echo data from multiple clients independently. UDP traffic is processed in a single loop and echoed back to the sender.

[![Build](https://github.com/srdg/ccecho/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/srdg/ccecho/actions/workflows/c-cpp.yml) 
[![Release](https://github.com/srdg/ccecho/actions/workflows/release.yml/badge.svg)](https://github.com/srdg/ccecho/actions/workflows/release.yml)

## Features

- **Protocol Support**: TCP by default, UDP optionally via command-line `-u` or `--udp` flag
- **Multi-threaded**: Handles multiple concurrent client connections
- **Port 7 Default**: Listens on port 7 by default (configurable)
- **Echo Functionality**: Echoes back any data received from clients
- **Graceful Shutdown**: Cleanly stops the server on user command
- **Error Handling**: Comprehensive error reporting for socket operations

## Built and tested with

- **Windows OS** (uses Winsock2)
- **C++11 or later** (for threading support)
- **CMake 3.10+** (for building)
- **Visual Studio Build Tools** or similar C++ compiler
- **ncat** for evaluation

