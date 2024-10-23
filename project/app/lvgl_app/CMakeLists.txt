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

PROJECT(lvgl)

include_directories(
    ${CMAKE_SOURCE_DIR}
    lvgl
    lvgl/src/font
    lvgl/src
    lv_drivers/
    lv_app/
    lv_app/src
    lv_port/
    include
    include/libdrm
    include/libjpeg9
    custom/
    generated/
    generated/guider_fonts
    generated/guider_customer_fonts
    generated/images
    generated/mPythonImages
    recycle/
    ${CMAKE_SOURCE_DIR}/../../../media/rockit/rockit/mpi/sdk/include
    ${CMAKE_SOURCE_DIR}/../../../media/rockit/rockit/mpi/example/include/
)

# Define library directory
set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
link_directories(${LIB_DIR})

set(CXX_SRCS
    main.c
    lv_app/src/ui.c
    )

file(GLOB_RECURSE GENERATED_SOURCES generated/*.c)
file(GLOB_RECURSE CUSTOM_SOURCES custom/*.c)
file(GLOB_RECURSE RECYCLE_SOURCES recycle/*.c)

add_subdirectory(lvgl)
add_subdirectory(lv_drivers)

ADD_EXECUTABLE(lvgl_app ${RECYCLE_SOURCES} ${CUSTOM_SOURCES} ${GENERATED_SOURCES} ${CXX_SRCS})

# Add linker flags for rpath and rpath-link
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,${LIB_DIR}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath-link,${LIB_DIR}")

target_link_libraries(lvgl_app
    PRIVATE lvgl::lvgl
    PRIVATE lvgl::drivers
    PRIVATE lvgl::demos
    PRIVATE lvgl::examples
    libdrm.a
    libcurl.so
    libcrypto.so
    libcjson.so
    libjpeg.so
    pthread
    librockit.so
    librga.so
    librkmuxer.so
    librkdemuxer.so
    librockchip_mpp.so
    m
)

install(TARGETS lvgl_app DESTINATION usr/bin/)
