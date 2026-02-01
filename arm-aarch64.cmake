# Establece el sistema operativo de destino
set(CMAKE_SYSTEM_NAME Linux)
#set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Especifica los compiladores cruzados
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

# Define el root del sistema objetivo (opcional, pero recomendado)
# set(CMAKE_FIND_ROOT_PATH "/path/to/sysroot/for/aarch64")

# No buscar programas o librerías en el host nativo
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)