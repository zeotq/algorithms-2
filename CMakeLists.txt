cmake_minimum_required(VERSION 3.10)
project(MyProject C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/src/*/*.c")
add_library(my_lib STATIC ${SRC_FILES})
# Expose include dirs (if you add headers to include/) and allow C files in src to be included
target_include_directories(my_lib PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)
# Ensure the library uses C11 features
target_compile_features(my_lib PUBLIC c_std_11)

file(GLOB APP_FILES "${CMAKE_SOURCE_DIR}/app/*.c")

foreach(APP_FILE ${APP_FILES})
    get_filename_component(TARGET_NAME ${APP_FILE} NAME_WE)

    add_executable(${TARGET_NAME} ${APP_FILE})
    
    target_link_libraries(${TARGET_NAME} PRIVATE my_lib)
endforeach()
