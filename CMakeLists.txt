# 1. Versión Mínima de CMake Requerida
cmake_minimum_required(VERSION 3.10)

# 2. Define el nombre del proyecto
project(socketUDPComms) #project(HelloWorld_Alvaro CXX)


# Establece 'Debug' como valor predeterminado si no se especifica CMAKE_BUILD_TYPE
#forzamos meter los simbolos de depuración (flag -g) para poder usar valgrind
#Borrar para build release
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the build type (Debug/Release)" FORCE)
endif()
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g") #flag -g activa simbolos de depuración

# 3. Define dónde buscar los archivos de encabezado (headers)
#    Esto es crucial para que el compilador encuentre 'util.h'
include_directories(./server/include)
#target_include_directories(server PUBLIC ./server/include)

# 4. Define el ejecutable a construir
#    Sintaxis: add_executable(<nombre_ejecutable> <archivos_fuente...>)
#    Aquí listamos todos los archivos .cpp necesarios.
add_executable(server
    ./server/src/main.cpp
    server/src/util.cpp
)
#target_link_libraries(server PUBLIC socket)


include_directories(./client/include)

add_executable(client
    ./client/src/main.cpp
    ./client/src/util.cpp
)
#target_link_libraries(client PUBLIC socket)

# 5. Opcional: Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)