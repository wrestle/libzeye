cmake_minimum_required(VERSION 3.3)
project(zoomeye)

LINK_DIRECTORIES(/usr/local/lib)
# If you Do use the json-c
# Just Add the -DJSON_DEPENDENCY to the set
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -DWSX_RELEASE") # Add here
# Replace the JSON_C_PATH with your json-c path, or Default by that.
set(JSON_C_PATH "-I/usr/local/include/json-c")

set(TEST_FILE main.c test/test_res_info.c test/test.h )
set(SOURCE_FILES
        model/base.c model/base.h
        model/zeye_info.h model/zeye_info.c model/zeye_host_srch.h model/zeye_host_srch.c model/util.h model/util.c
        util/zeye_util.h util/zeye_util.c)

#if you want the static library, Use it
#add_library(zoomeye STATIC ${SOURCE_FILES})
add_library(zeye SHARED ${SOURCE_FILES})
add_executable(zeyetest ${SOURCE_FILES} ${TEST_FILE})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

if (${CMAKE_C_FLAGS} MATCHES "-DJSON_DEPENDENCY")
    message(">>> Need the json-c library")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${JSON_C_PATH} -ljson-c")
    TARGET_LINK_LIBRARIES(zeye -ljson-c)
    TARGET_LINK_LIBRARIES(zeyetest -ljson-c)
endif ()
