cmake_minimum_required (VERSION 3.3)
project (ZeromqLearning)

set(PROJ_BASE ${PROJECT_SOURCE_DIR}/src)

include_directories("${PROJ_BASE}/include")


find_library(Libzmq zmq)

if(Libzmq EQUAL Libzmq-NOTFOUND)
  message(FATAL_ERROR "zeromq library not found!")
endif()

##### request reply example
add_executable(Send ${PROJ_BASE}/req-rep/send.c)
target_link_libraries(Send ${Libzmq})

add_executable(Recv ${PROJ_BASE}/req-rep/recv.c)
target_link_libraries(Recv ${Libzmq})
