#!/bin/bash

# Target arch
export RK_ARCH=arm

# Target CHIP
export RK_CHIP=rv1106

# Target Toolchain Cross Compile
export RK_TOOLCHAIN_CROSS=arm-rockchip830-linux-uclibcgnueabihf

# Target boot medium: emmc/spi_nor/spi_nand
export RK_BOOT_MEDIUM=spi_nand

# Uboot defconfig
export RK_UBOOT_DEFCONFIG=rv1106-spi-nand-tb_defconfig

# Uboot defconfig fragment
export RK_UBOOT_DEFCONFIG_FRAGMENT=rk-sfc.config

# Uboot Loader ini overlay
# export RK_UBOOT_RKBIN_INI_OVERLAY=RKBOOT/RV1106MINIALL_TB_SC3336_SC3338.ini
# build.sh mcu info --> rv1106_evb-SC3338-ADC
export RK_UBOOT_RKBIN_MCU_CFG=rv1106_evb-SC3338-ADC

# Kernel defconfig
export RK_KERNEL_DEFCONFIG=rv1106_defconfig

# Kernel defconfig fragment
export RK_KERNEL_DEFCONFIG_FRAGMENT="rv1106-tb.config rv1106-battery-ipc.config rv1106-nand.config rv1106-sdiowifi.config"

# Recovery defconfig fragment
export RK_RECOVERY_KERNEL_DEFCONFIG_FRAGMENT="rv1106-tb.config rv1106-recovery.config"

# Kernel dts
export RK_KERNEL_DTS=rv1106g-evb2-v12-spi-nand-tb.dts

#misc image
export RK_MISC=wipe_all-misc.img

# Config sensor IQ files
# RK_CAMERA_SENSOR_IQFILES format:
#     "iqfile1 iqfile2 iqfile3 ..."
# ./build.sh media and copy <SDK root dir>/output/out/media_out/isp_iqfiles/$RK_CAMERA_SENSOR_IQFILES
export RK_CAMERA_SENSOR_IQFILES="sc3338_FKO1_30IRC-F16.bin"

# Config CMA size in environment
export RK_BOOTARGS_CMA_SIZE="32M"

# config partition in environment
# RK_PARTITION_CMD_IN_ENV format:
#     <partdef>[,<partdef>]
#       <partdef> := <size>[@<offset>](part-name)
# Note:
#   If the first partition offset is not 0x0, it must be added. Otherwise, it needn't adding.
export RK_PARTITION_CMD_IN_ENV="256K(env),1M@256K(idblock),512K(uboot),2M(meta),32M(boot),-(userdata)"

export RK_META_SIZE=393216

# config partition's filesystem type (squashfs is readonly)
# emmc:    squashfs/ext4
# nand:    squashfs/ubifs
# spi nor: squashfs/jffs2
# RK_PARTITION_FS_TYPE_CFG format:
#     AAAA:/BBBB/CCCC@ext4
#         AAAA ----------> partition name
#         /BBBB/CCCC ----> partition mount point
#         ext4 ----------> partition filesystem type
export RK_PARTITION_FS_TYPE_CFG=boot@IGNORE@erofs,userdata@/userdata@ubifs

# config filesystem compress (Just for squashfs or ubifs)
# squashfs: lz4/lzo/lzma/xz/gzip, default xz
# ubifs:    lzo/zlib, default lzo
# export RK_SQUASHFS_COMP=xz
# export RK_UBIFS_COMP=lzo

# app config
export RK_APP_TYPE="RK_FASTBOOT_SERVER RK_FASTBOOT_CLIENT"

# enable rockchip test
export RK_ENABLE_ROCKCHIP_TEST=n

# specify post.sh for delete/overlay files
export RK_POST_BUILD_SCRIPT=rv1106-tb-spi_nor-post.sh

# enable fastboot
export RK_ENABLE_FASTBOOT=y

# config AI-ISP model
export RK_AIISP_MODEL=NONE

# config AUDIO model
export RK_AUDIO_MODEL=NONE

# config NPU model
export RK_NPU_MODEL="object_detection_pfp_896x512.data"

# disable adb
export RK_ENABLE_ADBD=n

# disable gdb
export RK_ENABLE_GDB=n

# disable udev
export RK_ENABLE_EUDEV=n

# build ipc web backend
export RK_APP_IPCWEB_BACKEND=n

# enable rndis
export RK_ENABLE_RNDIS=y

# enable wifi
export RK_ENABLE_WIFI=y
export RK_ENABLE_WIFI_CHIP="HI3861L"

# declare overlay directory
export RK_POST_OVERLAY=overlay-battery-ipc

export RK_NAND_BLOCK_SIZE=0x20000
export RK_NAND_PAGE_SIZE=2048
