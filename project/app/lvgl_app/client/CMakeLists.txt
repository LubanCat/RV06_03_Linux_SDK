cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
add_compile_options(-fPIC -Wall -Wno-error -O2 -g)
add_compile_options(-Wno-unused-variable
    -Wno-unused-but-set-variable
    -Wno-unused-function
    -Wno-misleading-indentation
    -Wno-unused-label
    -Wno-format-truncation
    -Wall
    -Wshadow -Wundef -Wmissing-prototypes -Wno-discarded-qualifiers
    -Wall -Wextra -Wno-unused-function -Wno-error=strict-prototypes
    -Wpointer-arith -fno-strict-aliasing -Wno-error=cpp -Wuninitialized
    -Wmaybe-uninitialized -Wno-unused-parameter -Wno-missing-field-initializers
    -Wtype-limits -Wsizeof-pointer-memaccess -Wno-format-nonliteral
    -Wno-cast-qual -Wunreachable-code -Wno-switch-default -Wreturn-type
    -Wmultichar -Wformat-security -Wno-ignored-qualifiers
    -Wno-error=pedantic -Wno-sign-compare -Wno-error=missing-prototypes
    -Wdouble-promotion -Wclobbered -Wdeprecated -Wempty-body
    -Wshift-negative-value -Wstack-usage=2048 -Wno-unused-value
    -Wno-unused-parameter -Wno-missing-field-initializers -Wuninitialized
    -Wmaybe-uninitialized -Wall -Wextra -Wno-unused-parameter
    -Wno-missing-field-initializers -Wtype-limits -Wsizeof-pointer-memaccess
    -Wpointer-arith -Wno-cast-qual -Wmissing-prototypes -Wunreachable-code
    -Wno-switch-default -Wreturn-type -Wmultichar -Wno-discarded-qualifiers
    -Wformat-security -Wno-ignored-qualifiers -Wno-sign-compare
    )

PROJECT(recycle_app_client)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ../include/
    recycle/
)

# Define library directory
set(LIB_DIR ${CMAKE_SOURCE_DIR}/../lib)
link_directories(${LIB_DIR})

# 判断cmake传入的-D CLIENT=true参数，如果为true，则编译客户端程序，否则编译服务端程序
    
set(CXX_SRCS
    main.c
    )

file(GLOB_RECURSE RECYCLE_SOURCES recycle/*.c)


ADD_EXECUTABLE(recycle_app_client ${RECYCLE_SOURCES} ${CXX_SRCS})

# Add linker flags for rpath and rpath-link
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,${LIB_DIR}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath-link,${LIB_DIR}")

target_link_libraries(recycle_app_client
    pthread
    m
    libcjson.so
)

install(TARGETS recycle_app_client DESTINATION usr/bin/)
