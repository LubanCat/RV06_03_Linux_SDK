cmake_minimum_required(VERSION 3.10)
project(capture_ai C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

# RV1106定义
add_definitions(-DRV1106_1103)

# 编译选项
add_compile_options(-Wall -Wno-error -O2 -g)

# 路径设置
set(ROCKIT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../media/rockit/rockit/mpi)
set(MEDIA_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../media/out/lib)
set(RKNPU_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/rknpu2)

# 头文件
include_directories(
    ${ROCKIT_PATH}/sdk/include
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/allocator/dma
    ${RKNPU_PATH}/include
)

# 库目录
link_directories(
    ${MEDIA_LIB_PATH}
    ${RKNPU_PATH}/lib
)

# 可执行文件
add_executable(capture_ai main.c)

# 设置RPATH
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath-link,${MEDIA_LIB_PATH}")

# 链接库
target_link_libraries(capture_ai
    rockit
    rockchip_mpp
    rga
    rknnmrt
    pthread
    m
    dl
)

# 安装
install(TARGETS capture_ai RUNTIME DESTINATION usr/bin)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/model/yolov5n-face-rv1106.rknn DESTINATION model)