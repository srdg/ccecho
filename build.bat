@echo off

set config=Release
if "%1"=="Debug" (
    set config=Debug
)
if exist .build (
    rmdir /s /q .build
)
if exist .bin (
    rmdir /s /q .bin
)

cmake -B .build -DCMAKE_BUILD_TYPE=%config% .
cmake --build .build --config %config% 

@echo on