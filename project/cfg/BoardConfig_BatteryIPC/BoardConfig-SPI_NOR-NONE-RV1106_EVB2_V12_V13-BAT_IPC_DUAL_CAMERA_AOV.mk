#!/bin/bash

# Target arch
export RK_ARCH=arm

# Target CHIP
export RK_CHIP=rv1106

# Target Toolchain Cross Compile
export RK_TOOLCHAIN_CROSS=arm-rockchip830-linux-uclibcgnueabihf

# Target boot medium: emmc/spi_nor/spi_nand
export RK_BOOT_MEDIUM=spi_nor

# Uboot defconfig
export RK_UBOOT_DEFCONFIG=rv1106-spi-nor-tb_defconfig

# Uboot defconfig fragment
export RK_UBOOT_DEFCONFIG_FRAGMENT=rk-sfc.config

# Uboot Loader ini overlay
# export RK_UBOOT_RKBIN_INI_OVERLAY=RKBOOT/RV1106MINIALL_TB_DUAL_CAMERA_SC301IOT_SC230AI.ini
# build.sh mcu info --> rv1106_evb2-dual_cam_SC301IOT_SC230AI-ADC
export RK_UBOOT_RKBIN_MCU_CFG=rv1106_evb2-dual_cam_SC301IOT_SC230AI-ADC

# Kernel defconfig
export RK_KERNEL_DEFCONFIG=rv1106_defconfig

# Kernel defconfig fragment
export RK_KERNEL_DEFCONFIG_FRAGMENT="rv1106-tb.config rv1106-battery-ipc.config rv1106-pm.config rv1106-wakeup.config rv1106-sdiowifi.config"

# Recovery defconfig fragment
export RK_RECOVERY_KERNEL_DEFCONFIG_FRAGMENT="rv1106-tb.config rv1106-recovery.config"

# Kernel dts
export RK_KERNEL_DTS=rv1106g-evb2-v12-dual-camera-aov-spi-nor.dts

#misc image
export RK_MISC=wipe_all-misc.img

# Config sensor IQ files
# RK_CAMERA_SENSOR_IQFILES format:
#     "iqfile1 iqfile2 iqfile3 ..."
# ./build.sh media and copy <SDK root dir>/output/out/media_out/isp_iqfiles/$RK_CAMERA_SENSOR_IQFILES
export RK_CAMERA_SENSOR_IQFILES="sc301iot_CMK-OT2349-PC1_65IRC-F20.bin sc230ai_CMK-OT2350-PC1_65IRC-F16.bin"

# Config CMA size in environment
export RK_BOOTARGS_CMA_SIZE="48M"

# config partition in environment
# RK_PARTITION_CMD_IN_ENV format:
#     <partdef>[,<partdef>]
#       <partdef> := <size>[@<offset>](part-name)
# Note:
#   If the first partition offset is not 0x0, it must be added. Otherwise, it needn't adding.
export RK_PARTITION_CMD_IN_ENV="64K(env),256K@64K(idblock),256K(uboot),704K(meta),11M(boot),3M(userdata)"

# config partition's filesystem type (squashfs is readonly)
# emmc:    squashfs/ext4
# nand:    squashfs/ubifs
# spi nor: squashfs/jffs2
# RK_PARTITION_FS_TYPE_CFG format:
#     AAAA:/BBBB/CCCC@ext4
#         AAAA ----------> partition name
#         /BBBB/CCCC ----> partition mount point
#         ext4 ----------> partition filesystem type
export RK_PARTITION_FS_TYPE_CFG=boot@IGNORE@erofs,userdata@/userdata@jffs2

# config filesystem compress (Just for squashfs or ubifs)
# squashfs: lz4/lzo/lzma/xz/gzip, default xz
# ubifs:    lzo/zlib, default lzo
# export RK_SQUASHFS_COMP=xz
# export RK_UBIFS_COMP=lzo

# app config
export RK_APP_TYPE="RK_SAMPLE_AOV"

export RK_ENABLE_AOV=y

# config AI-ISP model
export RK_AIISP_MODEL=NONE

# config AUDIO model
export RK_AUDIO_MODEL=NONE

# config NPU model
export RK_NPU_MODEL="object_detection_pfp_512x288.data"

# specify post.sh for delete/overlay files
export RK_POST_BUILD_SCRIPT=rv1106-tb-dual-cam-spi_nor-aov-post.sh

# enable fastboot
export RK_ENABLE_FASTBOOT=y

# enable wifi
export RK_ENABLE_WIFI=y
export RK_ENABLE_WIFI_CHIP="HI3861L"

# declare overlay directory
export RK_POST_OVERLAY=overlay-battery-ipc

export RK_META_PARAM=" \
 --rk_cam_mirror_flip=0 \
 --rk_cam2_mirror_flip=0 \
 --rk_cam_fps=15 \
 --rk_cam2_fps=15 \
 --rk_cam_hdr=0 \
 --rk_cam2_hdr=0 \
 --rk_led_value=20 \
 --rk_night_mode=0 \
 --cmdline=rockit.en_mcu=0 \
"

# disable adb
export RK_ENABLE_ADBD=n

# enable rockchip test
export RK_ENABLE_ROCKCHIP_TEST=n

# disable gdb
export RK_ENABLE_GDB=n

# disable udev
export RK_ENABLE_EUDEV=n

# build ipc web backend
export RK_APP_IPCWEB_BACKEND=n

# enable rndis
export RK_ENABLE_RNDIS=n
