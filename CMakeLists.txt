cmake_minimum_required(VERSION 3.3)
project(libzeye)

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu99 -DWSX_RELEASE")

set(SOURCE_FILES main.c base.c base.h model/zeye_info.h model/zeye_info.c test/test_res_info.c test/test.h model/zeye_host_srch.h model/zeye_host_srch.c model/util.h model/util.c)
add_executable(libzeye ${SOURCE_FILES})