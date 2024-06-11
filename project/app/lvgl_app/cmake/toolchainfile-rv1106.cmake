# Get current source and binary directories
SET(CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
SET(CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

# Set relative path to toolchain
SET(TOOLCHAIN_PATH ../../../tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf)

# Set cross-compilation toolchain
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR arm)

SET(CMAKE_C_COMPILER ${CURRENT_SOURCE_DIR}/${TOOLCHAIN_PATH}/bin/arm-rockchip830-linux-uclibcgnueabihf-gcc)
SET(CMAKE_CXX_COMPILER ${CURRENT_SOURCE_DIR}/${TOOLCHAIN_PATH}/bin/arm-rockchip830-linux-uclibcgnueabihf-g++)

SET(CMAKE_FIND_ROOT_PATH ${CURRENT_SOURCE_DIR}/${TOOLCHAIN_PATH}/arm-rockchip830-linux-uclibcgnueabihf/sysroot)

SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Additional flags or settings
SET(CMAKE_C_FLAGS "--sysroot=${CURRENT_SOURCE_DIR}/${TOOLCHAIN_PATH}/arm-rockchip830-linux-uclibcgnueabihf/sysroot -Wall")
SET(CMAKE_CXX_FLAGS "--sysroot=${CURRENT_SOURCE_DIR}/${TOOLCHAIN_PATH}/arm-rockchip830-linux-uclibcgnueabihf/sysroot -Wall")

# Linker settings
SET(CMAKE_EXE_LINKER_FLAGS "-Wl,-rpath-link,${CURRENT_SOURCE_DIR}/${TOOLCHAIN_PATH}/runtime_lib")

# Include directories
INCLUDE_DIRECTORIES(${CURRENT_SOURCE_DIR}/${TOOLCHAIN_PATH}/include)

# Library directories
LINK_DIRECTORIES(${CURRENT_SOURCE_DIR}/${TOOLCHAIN_PATH}/lib)
LINK_DIRECTORIES(${CURRENT_SOURCE_DIR}/${TOOLCHAIN_PATH}/runtime_lib)

