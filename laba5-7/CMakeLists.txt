project(ProjectName)
cmake_minimum_required(VERSION 3.16)

include_directories(include)

find_package(PkgConfig)

pkg_check_modules(PC_ZeroMQ QUIET zmq)

find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.hpp
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )

find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )

#add_executable(manager ./src/socket.cpp ./src/tree.cpp ./src/manager.cpp)
#add_executable(computer ./src/socket.cpp ./src/tree.cpp ./src/computer.cpp)

add_executable(manager ${SOURCE_DIR}/manager.cpp)
add_executable(computer ${SOURCE_DIR}/computer.cpp)
add_executable(tree ${SOURCE_DIR}/tree.cpp)
add_executable(socket ${SOURCE_DIR}/socket.cpp)

## add the include directory to our compile directives
target_include_directories(manager PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_include_directories(computer PUBLIC ${ZeroMQ_INCLUDE_DIR})
## at the 0mq library to our link directive
target_link_libraries(manager PUBLIC ${ZeroMQ_LIBRARY})
target_link_libraries(computer PUBLIC ${ZeroMQ_LIBRARY})