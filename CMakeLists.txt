cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(notifications-in-gd)

add_library(notifications-in-gd SHARED src/main.cpp "src/gdaddresses.h" "src/notifications.cpp" "src/notifications.h" "src/parsers.cpp" "src/parsers.h" "src/CCCoroutine.h"  )

include(cmake/CPM.cmake)

CPMAddPackage("gh:matcool/mat-dash#cdc9f70")
CPMAddPackage("gh:HJfod/cocos-headers#cbf79fc")
CPMAddPackage("gh:HJfod/gd.h#90f2110")
CPMAddPackage("gh:TsudaKageyu/minhook#4a45552")

target_compile_definitions(notifications-in-gd PRIVATE CC_DLL=)

target_include_directories(notifications-in-gd PRIVATE
    ${gd.h_SOURCE_DIR}/include ${gd.h_SOURCE_DIR}
    ${minhook_SOURCE_DIR}/include
)

target_link_libraries(notifications-in-gd mat-dash cocos2d minhook)
