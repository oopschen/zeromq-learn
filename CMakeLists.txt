cmake_minimum_required (VERSION 3.3)
project (ZeromqLearning)

set(PROJ_BASE ${PROJECT_SOURCE_DIR}/src)

include_directories("${PROJ_BASE}/include")


# library
find_library(Libzmq zmq)
if(${Libzmq} STREQUAL "Libzmq-NOTFOUND")
  message(FATAL_ERROR "zeromq library not found!")
endif()

find_library(Libthread pthread)
if(${Libthread} STREQUAL "Libthread-NOTFOUND")
  message(FATAL_ERROR "thread library not found!")
endif()

##### request reply example
add_executable(Send ${PROJ_BASE}/req-rep/send.c)
target_link_libraries(Send ${Libzmq})

add_executable(Recv ${PROJ_BASE}/req-rep/recv.c)
target_link_libraries(Recv ${Libzmq})

##### publisher subscriber example
add_executable(Pub ${PROJ_BASE}/pub-sub/pub.c)
target_link_libraries(Pub ${Libzmq})
add_executable(Sub ${PROJ_BASE}/pub-sub/sub.c)
target_link_libraries(Sub ${Libzmq})

#### dealer rely example
add_executable(DealerRep ${PROJ_BASE}/dealer-rep/main.c)
target_link_libraries(DealerRep ${Libzmq} ${Libthread})

#### req router example
add_executable(ReqRouterReq ${PROJ_BASE}/req-router/send.c)
target_link_libraries(ReqRouterReq  ${Libzmq})

add_executable(ReqRouter ${PROJ_BASE}/req-router/router.c)
target_link_libraries(ReqRouter  ${Libzmq} ${Libthread})

