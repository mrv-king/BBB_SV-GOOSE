cmake_minimum_required(VERSION 3.10)
project(IEC61850_PROJ VERSION 1.0)

############################################################
# Create an executable
############################################################

add_executable(iec61850 source/main.cpp)

IF (UNIX)
    SET(Include_DIR "${PROJECT_SOURCE_DIR}/headers;")
    FIND_PACKAGE(Boost 1.62.0 REQUIRED COMPONENTS system)
    FIND_PACKAGE(Python COMPONENTS Interpreter Development)

    target_include_directories(iec61850 PUBLIC "${Include_DIR};${Boost_INCLUDE_DIRS};${Python_INCLUDE_DIRS}")
    target_link_libraries(iec61850 PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/libIEC61850LIB.so" -lpthread -lboost_system ${Python_LIBRARIES})
ENDIF (UNIX)
