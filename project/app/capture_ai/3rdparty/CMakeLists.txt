cmake_minimum_required(VERSION 3.15)

project(rknn_model_zoo_3rdparty_libs)

# stb image
set(STB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/stb_image)
set(STB_INCLUDES ${STB_PATH} PARENT_SCOPE)

# jpeg turbo
set(JPEG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/jpeg_turbo)
set(LIBJPEG ${JPEG_PATH}/lib/libturbojpeg.a PARENT_SCOPE)
set(LIBJPEG_INCLUDES ${JPEG_PATH}/include PARENT_SCOPE)

# rknn runtime (rknpu2 for RV1106)
set(RKNN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/rknpu2)
set(LIBRKNNRT ${RKNN_PATH}/lib/librknnmrt.so)
set(LIBRKNNRT_INCLUDES ${RKNN_PATH}/include PARENT_SCOPE)
install(PROGRAMS ${LIBRKNNRT} DESTINATION lib)
set(LIBRKNNRT ${LIBRKNNRT} PARENT_SCOPE)

# rga
set(RGA_PATH ${CMAKE_CURRENT_SOURCE_DIR}/librga)
set(LIBRGA ${RGA_PATH}/lib/librga.a PARENT_SCOPE)
set(LIBRGA_INCLUDES ${RGA_PATH}/include PARENT_SCOPE)
install(PROGRAMS ${RGA_PATH}/lib/librga.so DESTINATION lib)

# timer
set(TIMER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/timer)
set(LIBTIMER_INCLUDES ${TIMER_PATH} PARENT_SCOPE)