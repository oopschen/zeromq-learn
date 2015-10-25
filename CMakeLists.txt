cmake_minimum_required (VERSION 3.3)
project (ZeromqLearning C)

set(PROJ_BASE ${PROJECT_SOURCE_DIR}/src)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c11")
set(CMAKE_PREFIX_PATH /usr)

include_directories("${PROJ_BASE}/include")
include_directories("/include")

# modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/mods/")

# library
## zero mq
find_package(ZMQ 4 REQUIRED)
include_directories(${ZMQ_INCLUDE_DIRS})
set(EXTRALIBS ${ZMQ_LIBRARIES})
## threads
find_package(Threads 4 REQUIRED)
set(EXTRALIBS ${EXTRALIBS} ${CMAKE_THREAD_LIBS_INIT})

##### request reply example
add_executable(Send ${PROJ_BASE}/req-rep/send.c)
target_link_libraries(Send ${EXTRALIBS})

add_executable(Recv ${PROJ_BASE}/req-rep/recv.c)
target_link_libraries(Recv ${EXTRALIBS})

##### publisher subscriber example
add_executable(Pub ${PROJ_BASE}/pub-sub/pub.c)
target_link_libraries(Pub ${EXTRALIBS})
add_executable(Sub ${PROJ_BASE}/pub-sub/sub.c)
target_link_libraries(Sub ${EXTRALIBS})

#### dealer rely example
add_executable(DealerRep ${PROJ_BASE}/dealer-rep/main.c)
target_link_libraries(DealerRep ${EXTRALIBS})

#### req router example
add_executable(ReqRouterReq ${PROJ_BASE}/req-router/send.c)
target_link_libraries(ReqRouterReq  ${EXTRALIBS})

add_executable(ReqRouter ${PROJ_BASE}/req-router/router.c)
target_link_libraries(ReqRouter  ${EXTRALIBS})

