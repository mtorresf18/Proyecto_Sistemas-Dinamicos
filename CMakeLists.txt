# Establece la versión mínima requerida de CMake para garantizar la compatibilidad con características modernas.
cmake_minimum_required(VERSION 3.12)

# Define el nombre del proyecto general y especifica que se utilizará el lenguaje C++.
project(SistemasDinamicos LANGUAGES CXX)

# Especifica el estándar C++17 para soportar características avanzadas como std::complex y tipos de datos modernos.
set(CMAKE_CXX_STANDARD 17)

# Hace obligatorio el uso del estándar C++17 sin permitir degradación a versiones anteriores del compilador.
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Desactiva las extensiones específicas o propietarias del compilador para garantizar la máxima portabilidad.
set(CMAKE_CXX_EXTENSIONS OFF)

# Registra el directorio de cabeceras 'include/' para que el compilador localice los archivos .hpp del sistema.
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Colecciona la lista explícita de todos los archivos fuente de C++ (.cpp) que integran el programa.
set(SOURCE_FILES
    src/main.cpp
    src/Matrix2D.cpp
    src/SystemAnalyzer.cpp
    src/TrajectoryExporter.cpp
)

# Crea la regla para compilar y enlazar el ejecutable binario denominado 'SistemasDinamicos'.
add_executable(SistemasDinamicos ${SOURCE_FILES})

# Especifica los directorios de inclusión privados asociados explícitamente al objetivo ejecutable 'SistemasDinamicos'.
target_include_directories(SistemasDinamicos PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Copia la carpeta scripts al directorio de compilación (build) automáticamente
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/scripts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})