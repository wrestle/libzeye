cmake_minimum_required(VERSION 3.3)
project(libzeye)

LINK_DIRECTORIES(/usr/local/lib)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -DWSX_RELEASE -I/usr/local/include/json-c")
set(SOURCE_FILES
        main.c model/base.c model/base.h
        model/zeye_info.h model/zeye_info.c model/zeye_host_srch.h model/zeye_host_srch.c model/util.h model/util.c
        test/test_res_info.c test/test.h util/zeye_util.h util/zeye_util.c)
add_executable(libzeye ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(libzeye -ljson-c)
