cmake_minimum_required(VERSION 3.10)

project(Lab3DLL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# VirusDLL.dll
add_library(VirusDLL SHARED VirusDLL/VirusDLL.cpp)
target_link_libraries(VirusDLL PRIVATE psapi)

# TargetProcess.exe
add_executable(TargetProcess TargetProcess/TargetProcess.cpp)

# DLLLoader.exe
add_executable(DLLLoader DLLLoader/DLLLoader.cpp)

# DLLInjectorAsProcess.exe
add_executable(DLLInjectorAsProcess DLLInjectorAsProcess/DLLInjectorAsProcess.cpp)

# DllInjectorAsDll.dll
add_library(DllInjectorAsDll SHARED DllInjectorAsDll/DllInjectorAsDll.cpp)

# ProtectedProcess.exe
add_executable(ProtectedTargetProcess ProtectedTargetProcess/ProtectedTargetProcess.cpp)